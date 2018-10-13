/**
 ****************************************************************************************
 *
 * @file crypto.c
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "BoundWitness.h"


/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_getSigningData
*
*  DESCRIPTION
*      Retrieves and orders the bound witness data so that it can be signed or hashed.
*      void* is being used here to use multiple types of objects (Bound witness and Bound Witness Transfer)
*      as parameters
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       void* (BoundWitness*)(BoundWitnessTransfer*)
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_getSigningData(void* user_BoundWitness){
  uint32_t publicKeysSize = ((BoundWitness*)user_BoundWitness)->publicKeys->size;
  uint32_t firstHeuristicsSize = 0;
  uint32_t secondHeuristicsSize = 0;

  /* Get Size of keysets + payloads first */
  IntStrongArray* user_payloadsArray = ((BoundWitness*)user_BoundWitness)->payloads;
  XYResult* get_result = user_payloadsArray->get(user_payloadsArray, 0);
  XYObject* user_firstPayloadObject = get_result->result;
  char* user_firstPayload = user_firstPayloadObject->payload;
  free(get_result);
  firstHeuristicsSize = to_uint32((unsigned char*)user_firstPayload+(4*sizeof(char)));

  get_result = user_payloadsArray->get(user_payloadsArray, 1);
  XYObject* user_secondPayloadObject = NULL;
  char* user_secondPayload = NULL;
  if(get_result->error == OK){
    user_secondPayloadObject = get_result->result;
    char* user_secondPayload = user_firstPayloadObject->payload;
    free(get_result);
    secondHeuristicsSize = to_uint32((unsigned char*)user_secondPayload+(4*sizeof(char)));
  }


  uint32_t totalSize = publicKeysSize+firstHeuristicsSize+secondHeuristicsSize+4+1;

  char* byteBuffer = malloc(sizeof(char)*totalSize);

  /* Copy all elements of BoundWitness.publicKeys, followed by
   * BoundWiness.payloads[0].signed_heuristics and BoundWiness.payloads[1].signed_heuristics
   * into the byteBuffer and return.
   */

  if(byteBuffer){
    // Copy Public Keys in.
    XYResult* newObject_result = newObject((char*)&ShortStrongArray_id, ((BoundWitness*)user_BoundWitness)->publicKeys);
    if(newObject_result->error!=OK){
      free(byteBuffer);
      return newObject_result;
    }
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error!=OK){
      free(byteBuffer);
      return lookup_result;
    }
    ObjectProvider* SSA_Creator = lookup_result->result;

    free(lookup_result);

    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    free(newObject_result);
    if(toBytes_result->error!=OK){
      free(byteBuffer);
      return toBytes_result;
    }

    memcpy(byteBuffer, toBytes_result->result, publicKeysSize);
    free(toBytes_result);

    // Copy signed heuristics from the first Payload into the ByteBuffer
    memcpy(byteBuffer+(publicKeysSize*sizeof(char)), user_firstPayload+(4*sizeof(char)), firstHeuristicsSize*sizeof(char));

    // Copy signed heuristics from the second Payload into the ByteBuffer
    if(user_secondPayload != NULL)
      memcpy(byteBuffer+(publicKeysSize+firstHeuristicsSize*sizeof(char)), user_secondPayload+(4*sizeof(char)), secondHeuristicsSize*sizeof(char));

    ByteArray* return_bytes = malloc(sizeof(ByteArray));
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result && return_bytes){
      return_bytes->size = totalSize;
      return_bytes->payload = byteBuffer;
      return_result->error = OK;
      return_result->result = return_bytes;
      return return_result;
    } else {
      if(return_bytes) free(return_bytes);
      if(return_result) free(return_result);
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }

  } else {
    free(byteBuffer);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_getHash
*
*  DESCRIPTION
*      Retrieves the bound witness data hash so that it can be used.
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_getHash(BoundWitness* user_BoundWitness, HashProvider* user_HashProvider){
  XYResult* signingData_result = user_BoundWitness->getSigningData(user_BoundWitness);
  if(signingData_result->error != OK){
    return signingData_result;
  }
  ByteArray* signingData = signingData_result->result;
  return user_HashProvider->Hash(signingData);

}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitnessTransfer_toBytes
*
*  DESCRIPTION
*      Method somewhat similar to BoundWitness_creator_toBytes that creates a
*      Byte array with the XYO Bound Witness Transfer object type bytes.
*
*  PARAMETERS
*     *user_XYObject                     [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a char* as the result.
*----------------------------------------------------------------------------*/
XYResult* BoundWitnessTransfer_toBytes(XYObject* user_XYObject){
  BoundWitnessTransfer* user_BoundWitness = user_XYObject->payload;
  uint32_t totalSize = 5;
  uint32_t publicKeysSize = 0;
  uint32_t payloadsSize = 0;
  uint32_t signaturesSize = 0;

  XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider* ShortStrongArray_creator = lookup_result->result;
  free(lookup_result);

  lookup_result = lookup((char*)&IntStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider* IntStrongArray_creator = lookup_result->result;
  free(lookup_result);

  XYResult* newObject_result;
  XYResult* publicKeysBytes_result;
  XYResult* payloadsBytes_result;
  XYResult* signaturesBytes_result;

  if(user_BoundWitness->publicKeys)
  {
    totalSize += user_BoundWitness->publicKeys->size;
    publicKeysSize = user_BoundWitness->publicKeys->size;
    newObject_result = newObject((char*)&ShortStrongArray_id, user_BoundWitness->publicKeys);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    publicKeysBytes_result = ShortStrongArray_creator->toBytes(newObject_result->result);
    free(newObject_result->result);
    free(newObject_result);
  }
  if(user_BoundWitness->payloads) {
    totalSize += user_BoundWitness->payloads->size-4-4-6;
    payloadsSize = user_BoundWitness->payloads->size-4;
    newObject_result = newObject((char*)&Payload_id, user_BoundWitness->payloads);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    payloadsBytes_result = IntStrongArray_creator->toBytes(newObject_result->result);
    free(newObject_result->result);
    free(newObject_result);
  }
  if(user_BoundWitness->signatures) {
    totalSize += user_BoundWitness->signatures->size-4;
    signaturesSize = user_BoundWitness->signatures->size-4;
    newObject_result = newObject((char*)&ShortStrongArray_id, user_BoundWitness->signatures);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    signaturesBytes_result = ShortStrongArray_creator->toBytes(newObject_result->result);
    free(newObject_result->result);
    free(newObject_result);
  }

  char* return_bytes = malloc(totalSize*sizeof(char));

  if(!return_bytes){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  uint32_t encodedSize = totalSize;
  if(littleEndian()){
    encodedSize = to_uint32((unsigned char*)&totalSize);
  }
  uint32_t offset = 4*sizeof(char);
  memcpy(return_bytes, &encodedSize, 4*sizeof(char));

  uint8_t stage = 255;
  if(publicKeysSize > 4 && payloadsSize > 4 && signaturesSize == 4){
    stage = 1;
  } else if(publicKeysSize > 4 && payloadsSize > 4 && signaturesSize > 4){
    stage = 2;
  } else if ( publicKeysSize == 4 && payloadsSize == 4 && signaturesSize > 4){
    stage = 3;
  }
  memcpy(return_bytes+offset, &stage, 1);
  offset = offset + 1;

  if(user_BoundWitness->publicKeys){
    memcpy(return_bytes+offset, publicKeysBytes_result->result, publicKeysSize);
    offset+=publicKeysSize;
  }
  if(user_BoundWitness->payloads) {
    /* TODO: I am not sure how payloadsSize got off by 4 */
    memcpy(return_bytes+offset, payloadsBytes_result->result, payloadsSize+4);
    offset+=payloadsSize+4;
  }
  if(user_BoundWitness->signatures){
    memcpy(return_bytes+offset, signaturesBytes_result->result, signaturesSize);
    offset+=signaturesSize;
  }
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = return_bytes;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitnessTransfer_fromBytes
*
*  DESCRIPTION
*      Method somewhat similar to BoundWitness_creator_toBytes that creates a
*      Byte array with the XYO Bound Witness Transfer object type bytes.
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the BoundWitnessTransfer type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitnessTransfer_fromBytes(char* user_Transfer){
  BoundWitnessTransfer* return_BoundWitness = malloc(sizeof(BoundWitnessTransfer));
  uint32_t BoundWitnessSize = to_uint32(user_Transfer);
  if(littleEndian()){
    BoundWitnessSize = to_uint32((char*)&BoundWitnessSize);
  }
  uint8_t choice = user_Transfer[4];

  return_BoundWitness->size = BoundWitnessSize;
  return_BoundWitness->choice = choice;
  return_BoundWitness->getSigningData = BoundWitness_getSigningData;
  return_BoundWitness->getHash = BoundWitness_getHash;
  ShortStrongArray* publicKeysPtr = NULL;
  IntStrongArray* payloadPtr = NULL;
  ShortStrongArray* signaturesPtr = NULL;

  if(choice == 0x01){

    // Set up the Public Key Array

    uint16_t publicKeysSize = to_uint16((char*)&user_Transfer[5]);
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);

    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5]);
      if(fromBytes_result->error == OK)
      {
        publicKeysPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }

    // Payload
    lookup_result = lookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5+publicKeysSize*sizeof(char)]);

      if(fromBytes_result->error == OK)
      {
        payloadPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }


  } else if(choice == 0x02){
    // Set up the Public Key Array
    uint32_t publicKeysSize = to_uint32((char*)&user_Transfer[5]);
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5*sizeof(char)]);
      if(fromBytes_result->error == OK)
      {
        publicKeysPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }

    // Payload
    uint32_t payloadSize = to_uint32((char*)&user_Transfer[5+publicKeysSize*sizeof(char)]);
    lookup_result = lookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5+publicKeysSize*sizeof(char)]);

      if(fromBytes_result->error == OK)
      {
        payloadPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }

    // Signatures
    lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4+publicKeysSize+payloadSize*sizeof(char)]);
      if(fromBytes_result->error == OK)
      {
        signaturesPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }

  } else if(choice == 0x03){
    // Signatures
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4*sizeof(char)]);
      if(fromBytes_result->error == OK)
      {
        signaturesPtr = fromBytes_result->result;
        free(fromBytes_result);
      } else {
        free(return_BoundWitness);
        return fromBytes_result;
      }
    } else {
      free(return_BoundWitness);
      return lookup_result;
    }
  } else {
    free(return_BoundWitness);
    RETURN_ERROR(ERR_BADDATA);
  }

  return_BoundWitness->publicKeys = publicKeysPtr;
  return_BoundWitness->payloads = payloadPtr;
  return_BoundWitness->signatures = signaturesPtr;
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = return_BoundWitness;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitnessTransfer_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitnessTransfer_creator_create(char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_create(char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_fromBytes
*
*  DESCRIPTION
*      Create an Bound Witness object given a set of Bytes. Bytes must not
*      include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_fromBytes(char* data){
  BoundWitness* return_BoundWitness = malloc(sizeof(BoundWitness));
  uint32_t BoundWitnessSize = to_uint32(data);
  return_BoundWitness->size = BoundWitnessSize;
  ShortStrongArray* publicKeysPtr;
  IntStrongArray* payloadPtr;
  ShortStrongArray* signaturesPtr;

  /*
   * We process each of the elements in a Bound Witness
   * in separate chunks here and assign all the pointers
   * at the end. First is publicKeys.
   */
  uint32_t publicKeysSize = to_uint32(&data[4]);
  XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      publicKeysPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      free(return_BoundWitness);
      return fromBytes_result;
    }
  } else {
    free(return_BoundWitness);
    return lookup_result;
  }

  // Payload
  uint32_t payloadSize = to_uint32((char*)&data[4+publicKeysSize*sizeof(char)]);
 //free(lookup_result);
  lookup_result = lookup((char*)&IntStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize*sizeof(char)]);

    if(fromBytes_result->error == OK)
    {
      payloadPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      free(return_BoundWitness);
      return fromBytes_result;
    }
  } else {
    free(return_BoundWitness);
    return lookup_result;
  }

  // Signatures
 //free(lookup_result);
  lookup_result = lookup((char*)&ShortStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize+payloadSize*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      signaturesPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      free(return_BoundWitness);
      return fromBytes_result;
    }
  } else {
    free(return_BoundWitness);
    return lookup_result;
  }
  return_BoundWitness->size = BoundWitnessSize;
  return_BoundWitness->publicKeys = publicKeysPtr;
  return_BoundWitness->payloads = payloadPtr;
  return_BoundWitness->signatures = signaturesPtr;
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = return_BoundWitness;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Bound Witness type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_toBytes(struct XYObject* user_XYObject){
  BoundWitness* user_BoundWitness = user_XYObject->payload;
  XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
  ObjectProvider* SSA_Creator = lookup_result->result;
  free(lookup_result);
  lookup_result = lookup((char*)&IntStrongArray_id);
  ObjectProvider* ISA_Creator = lookup_result->result;
  char* byteBuffer = malloc(user_BoundWitness->size*sizeof(char));
  if(littleEndian()){
    uint32_t encodedSize = to_uint32((char*)&user_BoundWitness->size);
    memcpy(byteBuffer, &encodedSize, sizeof(uint32_t));
  } else {
    memcpy(byteBuffer, &user_BoundWitness->size, sizeof(uint32_t));
  }

  ShortStrongArray* user_publicKeys = user_BoundWitness->publicKeys;
  uint16_t user_publicKeysSize = user_publicKeys->size;
  XYResult* newObject_result = newObject((char*)&ShortStrongArray_id, user_publicKeys);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
   free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t), toBytes_result->result, user_publicKeysSize);
      free(toBytes_result);
    }
  }

  IntStrongArray* user_payload = user_BoundWitness->payloads;
  uint32_t user_payloadSize = user_payload->size;
  newObject_result = newObject((char*)&IntStrongArray_id, user_payload);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize, toBytes_result->result, user_payloadSize);
      free(toBytes_result);
    }
  }

  ShortStrongArray* user_signatures = user_BoundWitness->signatures;
  uint16_t user_signaturesSize = user_signatures->size;
  newObject_result = newObject((char*)&ShortStrongArray_id, user_signatures);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize+user_payloadSize, toBytes_result->result, user_signaturesSize);
      free(toBytes_result);
    }
  }
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = byteBuffer;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}
