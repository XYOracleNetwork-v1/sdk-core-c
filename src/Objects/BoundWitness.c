/**
 ****************************************************************************************
 *
 * @file boundwitness.c
 *
 * @XY4 project source code.
 *
 * @brief primary bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
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
XYResult_t* BoundWitness_getSigningData(void* user_BoundWitness){
  uint32_t publicKeysSize = ((BoundWitness*)user_BoundWitness)->publicKeys->size;
  uint32_t firstHeuristicsSize;
  uint32_t secondHeuristicsSize;

  /* Get Size of keysets + payloads first */
  IntStrongArray_t* user_payloadsArray = ((BoundWitness*)user_BoundWitness)->payloads;
  XYResult_t* get_result = user_payloadsArray->get(user_payloadsArray, 0);
  XYObject_t* user_firstPayloadObject = get_result->result;
  char* user_firstPayload = user_firstPayloadObject->payload;
  free(get_result);
  firstHeuristicsSize = to_uint32((char*)user_firstPayload+(4*sizeof(char)));

  get_result = user_payloadsArray->get(user_payloadsArray, 1);
  XYObject_t* user_secondPayloadObject = get_result->result;
  char* user_secondPayload = user_secondPayloadObject->payload;
  free(get_result);
  secondHeuristicsSize = to_uint32((char*)user_secondPayload+(4*sizeof(char)));

  uint32_t totalSize = publicKeysSize+firstHeuristicsSize+secondHeuristicsSize+4;

  char* byteBuffer = malloc(sizeof(char)*totalSize);

  /* Copy all elements of BoundWitness.publicKeys, followed by
   * BoundWiness.payloads[0].signed_heuristics and BoundWiness.payloads[1].signed_heuristics
   * into the byteBuffer and return.
   */

  if(byteBuffer){
    // Copy Public Keys in.

    XYResult_t* newObject_result = newObject((char*)&ShortStrongArray_id, ((BoundWitness*)user_BoundWitness)->publicKeys);
    if(newObject_result->error!=OK){
      free(byteBuffer);
      return newObject_result;
    }
    XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
    if(lookup_result->error!=OK){
      free(byteBuffer);
      return lookup_result;
    }
    ObjectProvider_t* SSA_Creator = lookup_result->result;

    free(lookup_result);

    XYResult_t* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
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
    memcpy(byteBuffer+(publicKeysSize+firstHeuristicsSize*sizeof(char)), user_secondPayload+(4*sizeof(char)), secondHeuristicsSize*sizeof(char));

    ByteArray_t* return_bytes = malloc(sizeof(ByteArray_t));
    XYResult_t* return_result = malloc(sizeof(XYResult_t));
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
XYResult_t* BoundWitness_getHash(BoundWitness* user_BoundWitness, HashProvider* user_HashProvider){
  
  XYResult_t* signingData_result = user_BoundWitness->getSigningData(user_BoundWitness);
  
  if(signingData_result->error != OK){
    return signingData_result;
  }
  
  ByteArray_t* signingData = signingData_result->result;
  
  return user_HashProvider->hash(signingData);

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
XYResult_t* BoundWitnessTransfer_toBytes(XYObject_t* user_XYObect){
  
  BoundWitnessTransfer* user_BoundWitness = user_XYObect->payload;
  uint32_t totalSize = 4;
  uint32_t publicKeysSize = 0;
  uint32_t payloadsSize = 0;
  uint32_t signaturesSize = 0;

  XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider_t* ShortStrongArray_creator = lookup_result->result;
  free(lookup_result);

  lookup_result = tableLookup((char*)&IntStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider_t* IntStrongArray_creator = lookup_result->result;
  free(lookup_result);

  XYResult_t* newObject_result;
  XYResult_t* publicKeysBytes_result;
  XYResult_t* payloadsBytes_result;
  XYResult_t* signaturesBytes_result;

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
  } else if(user_BoundWitness->payloads) {
    totalSize += user_BoundWitness->payloads->size;
    payloadsSize = user_BoundWitness->payloads->size;
    newObject_result = newObject((char*)&IntStrongArray_id, user_BoundWitness->payloads);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    payloadsBytes_result = IntStrongArray_creator->toBytes(newObject_result->result);
    free(newObject_result->result);
    free(newObject_result);
  } else if(user_BoundWitness->signatures) {
    totalSize += user_BoundWitness->signatures->size;
    signaturesSize = user_BoundWitness->signatures->size;
    newObject_result = newObject((char*)&ShortStrongArray_id, user_BoundWitness->signatures);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    signaturesBytes_result = ShortStrongArray_creator->toBytes(newObject_result->result);
    free(newObject_result->result);
    free(newObject_result);
  } else {
    RETURN_ERROR(ERR_BADDATA);
  }

  char* return_bytes = malloc(totalSize*sizeof(char));

  if(!return_bytes){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  uint32_t encodedSize = totalSize;
  if(littleEndian()){
    encodedSize = to_uint32((char*)&totalSize);
  }
  uint32_t offset = 4*sizeof(char);
  memcpy(return_bytes, &encodedSize, 4*sizeof(char));

  if(user_BoundWitness->publicKeys){
    memcpy(return_bytes+offset, publicKeysBytes_result->result, publicKeysSize);
    offset+=publicKeysSize;
  } else if(user_BoundWitness->payloads) {
    memcpy(return_bytes+offset, payloadsBytes_result->result, payloadsSize);
    offset+=payloadsSize;
  } else if(user_BoundWitness->signatures){
    memcpy(return_bytes+offset, signaturesBytes_result->result, signaturesSize);
    offset+=signaturesSize;
  }
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
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
XYResult_t* BoundWitnessTransfer_fromBytes(char* user_Transfer){
  BoundWitnessTransfer* return_BoundWitness = malloc(sizeof(BoundWitnessTransfer));
  uint32_t BoundWitnessSize = to_uint32((char*)&user_Transfer);
  if(littleEndian()){
    BoundWitnessSize = to_uint32((char*)&BoundWitnessSize);
  }
  uint8_t choice = user_Transfer[4];

  return_BoundWitness->size = BoundWitnessSize;
  return_BoundWitness->choice = choice;
  ShortStrongArray_t* publicKeysPtr = NULL;
  IntStrongArray_t* payloadPtr = NULL;
  ShortStrongArray_t* signaturesPtr = NULL;

  if(choice == 0x01){

    // Set up the Public Key Array
    uint32_t publicKeysSize = to_uint32((char*)&user_Transfer[4]);
    XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4*sizeof(char)]);
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
    lookup_result = tableLookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4+publicKeysSize*sizeof(char)]);

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
    uint32_t publicKeysSize = to_uint32((char*)&user_Transfer[4]);
    XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4*sizeof(char)]);
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
    uint32_t payloadSize = to_uint32((char*)&user_Transfer[4+publicKeysSize*sizeof(char)]);
    lookup_result = tableLookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4+publicKeysSize*sizeof(char)]);

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
    lookup_result = tableLookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4+publicKeysSize+payloadSize*sizeof(char)]);
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
    XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[4*sizeof(char)]);
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
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
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
XYResult_t* BoundWitnessTransfer_creator_create(char id[2], void* user_data){
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
XYResult_t* BoundWitness_creator_create(char id[2], void* user_data){
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
XYResult_t* BoundWitness_creator_fromBytes(char* data){
  
  BoundWitness* return_BoundWitness = malloc(sizeof(BoundWitness));
  uint32_t BoundWitnessSize = to_uint32(data);
  return_BoundWitness->size = BoundWitnessSize;
  ShortStrongArray_t* publicKeysPtr;
  IntStrongArray_t* payloadPtr;
  ShortStrongArray_t* signaturesPtr;

  /*
   * We process each of the elements in a Bound Witness
   * in separate chunks here and assign all the pointers
   * at the end. First is publicKeys.
   */
  uint32_t publicKeysSize = to_uint32(&data[4]);
  XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider_t* SWA_Creator = lookup_result->result;
    XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4*sizeof(char)]);
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
  lookup_result = tableLookup((char*)&IntStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider_t* SWA_Creator = lookup_result->result;
    XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize*sizeof(char)]);

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
  lookup_result = tableLookup((char*)&ShortStrongArray_id);
  if(lookup_result->error == OK){
    ObjectProvider_t* SWA_Creator = lookup_result->result;
    XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize+payloadSize*sizeof(char)]);
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
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
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
*      XYResult_t*          [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_creator_toBytes(struct XYObject* user_XYObject){
  
  BoundWitness* user_BoundWitness = user_XYObject->payload;
  XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
  ObjectProvider_t* SSA_Creator = lookup_result->result;
 //free(lookup_result);
  lookup_result = tableLookup((char*)&IntStrongArray_id);
  ObjectProvider_t* ISA_Creator = lookup_result->result;
  char* byteBuffer = malloc(user_BoundWitness->size*sizeof(char));
  
  if(littleEndian()){
    uint32_t encodedSize = to_uint32((char*)&user_BoundWitness->size);
    memcpy(byteBuffer, &encodedSize, sizeof(uint32_t));
  } else {
    memcpy(byteBuffer, &user_BoundWitness->size, sizeof(uint32_t));
  }

  ShortStrongArray_t* user_publicKeys = user_BoundWitness->publicKeys;
  uint16_t user_publicKeysSize = user_publicKeys->size;
  XYResult_t* newObject_result = newObject((char*)&ShortStrongArray_id, user_publicKeys);
  if(newObject_result->error == OK){
    XYResult_t* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
   //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t), toBytes_result->result, user_publicKeysSize);
     //free(toBytes_result);
    }
  }

  IntStrongArray_t* user_payload = user_BoundWitness->payloads;
  uint32_t user_payloadSize = user_payload->size;
  newObject_result = newObject((char*)&IntStrongArray_id, user_payload);
  if(newObject_result->error == OK){
    XYResult_t* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize, toBytes_result->result, user_payloadSize);
     //free(toBytes_result);
    }
  }

  ShortStrongArray_t* user_signatures = user_BoundWitness->signatures;
  uint16_t user_signaturesSize = user_signatures->size;
  newObject_result = newObject((char*)&ShortStrongArray_id, user_signatures);
  if(newObject_result->error == OK){
    XYResult_t* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize+user_payloadSize, toBytes_result->result, user_signaturesSize);

     //free(toBytes_result);
    }
  }
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  if(return_result){
    return_result->error = OK;
    return_result->result = byteBuffer;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}

// end of file boundwitness.c

