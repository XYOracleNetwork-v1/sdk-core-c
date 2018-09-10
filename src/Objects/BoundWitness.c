#include "XYOHeuristicsBuilder.h"

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_getSigningData
*
*  DESCRIPTION
*      Retrieves and orders the bound witness data so that it can be signed or hashed.
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_getSigningData(BoundWitness* user_BoundWitness){
  uint32_t publicKeysSize = user_BoundWitness->publicKeys->size;
  uint32_t firstHeuristicsSize;
  uint32_t secondHeuristicsSize;

  /* Get Size of keysets + payloads first */
  IntStrongArray* user_payloadsArray = user_BoundWitness->payloads;
  XYResult* get_result = user_payloadsArray->get(user_payloadsArray, 0);
  XYObject* user_firstPayloadObject = get_result->result;
  char* user_firstPayload = user_firstPayloadObject->payload;
  free(get_result);
  firstHeuristicsSize = to_uint32((char*)user_firstPayload+(4*sizeof(char)));

  get_result = user_payloadsArray->get(user_payloadsArray, 1);
  XYObject* user_secondPayloadObject = get_result->result;
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

    XYResult* newObject_result = newObject((char*)&ShortStrongArray_id, user_BoundWitness->publicKeys);
    if(newObject_result->error!=OK){
      return newObject_result;
    }
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error!=OK){
      return lookup_result;
    }
    Object_Creator* SSA_Creator = lookup_result->result;

    free(lookup_result);

    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    free(newObject_result);
    if(toBytes_result->error!=OK){
      return toBytes_result;
    }

    memcpy(byteBuffer, toBytes_result->result, publicKeysSize);
    free(toBytes_result);

    // Copy signed heuristics from the first Payload into the ByteBuffer
    memcpy(byteBuffer+(publicKeysSize*sizeof(char)), user_firstPayload+(4*sizeof(char)), firstHeuristicsSize*sizeof(char));
    free(toBytes_result);

    // Copy signed heuristics from the second Payload into the ByteBuffer
    memcpy(byteBuffer+(publicKeysSize+firstHeuristicsSize*sizeof(char)), user_secondPayload+(4*sizeof(char)), secondHeuristicsSize*sizeof(char));
    free(toBytes_result);

    ByteArray* return_bytes = malloc(sizeof(ByteArray));
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result && return_bytes){
      return_bytes->size = totalSize;
      return_bytes->payload = byteBuffer;
      return_result->error = OK;
      return_result->result = return_bytes;
      return return_result;
    } else {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }

  } else {
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
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      publicKeysPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
    return lookup_result;
  }

  // Payload
  uint32_t payloadSize = to_uint32((char*)&data[4+publicKeysSize*sizeof(char)]);
 //free(lookup_result);
  lookup_result = lookup((char*)&IntStrongArray_id);
  if(lookup_result->error == OK){
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize*sizeof(char)]);

    if(fromBytes_result->error == OK)
    {
      payloadPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
    return lookup_result;
  }

  // Signatures
 //free(lookup_result);
  lookup_result = lookup((char*)&ShortStrongArray_id);
  if(lookup_result->error == OK){
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize+payloadSize*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      signaturesPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
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
  Object_Creator* SSA_Creator = lookup_result->result;
 //free(lookup_result);
  lookup_result = lookup((char*)&IntStrongArray_id);
  Object_Creator* ISA_Creator = lookup_result->result;
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
   //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t), toBytes_result->result, user_publicKeysSize);
     //free(toBytes_result);
    }
  }

  IntStrongArray* user_payload = user_BoundWitness->payloads;
  uint32_t user_payloadSize = user_payload->size;
  newObject_result = newObject((char*)&IntStrongArray_id, user_payload);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize, toBytes_result->result, user_payloadSize);
     //free(toBytes_result);
    }
  }

  ShortStrongArray* user_signatures = user_BoundWitness->signatures;
  uint16_t user_signaturesSize = user_signatures->size;
  newObject_result = newObject((char*)&ShortStrongArray_id, user_signatures);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize+user_payloadSize, toBytes_result->result, user_signaturesSize);

     //free(toBytes_result);
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
