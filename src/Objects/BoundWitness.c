/**
 ****************************************************************************************
 *
 * @file boundwitness.c
 *
 * @XY4 project source code.
 *
 * @brief primary bound witness routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#include "xyo.h"
#include <stdlib.h>
#include "BoundWitness.h"      // includes "xyobject.h", "hash.h", <stdint.h>

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
*     *user_BoundWitness    [in]       void* (BoundWitness_t*)(BoundWitnessTransfer_t*)
*
*  RETURNS
*      XYResult_t*            [out]      bool   Returns XYObject_t* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_getSigningData(void* user_BoundWitness){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_BoundWitness) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  uint32_t publicKeysSize = ((BoundWitness_t*)user_BoundWitness)->publicKeys->size;
  uint32_t firstHeuristicsSize;
  uint32_t secondHeuristicsSize;

  /* Get Size of keysets + payloads first */
  IntStrongArray_t* user_payloadsArray = ((BoundWitness_t*)user_BoundWitness)->payloads;
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

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(byteBuffer){
    // Copy Public Keys in.

    XYResult_t* newObject_result = newObject((char*)&ShortStrongArray_id, ((BoundWitness_t*)user_BoundWitness)->publicKeys);
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

    //free(lookup_result);

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
    if(user_secondPayload != NULL)
      memcpy(byteBuffer+(publicKeysSize+firstHeuristicsSize*sizeof(char)), user_secondPayload+(4*sizeof(char)), secondHeuristicsSize*sizeof(char));

    ByteArray_t* return_bytes = malloc(sizeof(ByteArray_t));

    preallocated_return_result_ptr = &preallocated_return_result;

    /********************************/
    /* guard against malloc errors  */
    /********************************/

    if(return_bytes){
      return_bytes->size = totalSize;
      return_bytes->payload = byteBuffer;
      preallocated_return_result_ptr->error = OK;
      preallocated_return_result_ptr->result = return_bytes;

      return preallocated_return_result_ptr;

    } else {
      if(return_bytes) free(return_bytes);
      
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
*     *user_BoundWitness    [in]       BoundWitness_t*
*
*  RETURNS
*      XYResult_t*            [out]      bool   Returns XYObject_t* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_getHash(BoundWitness_t* user_BoundWitness, HashProvider_t* user_HashProvider){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_BoundWitness || !user_HashProvider) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  XYResult_t* signingData_result = user_BoundWitness->getSigningData(user_BoundWitness);

  if(signingData_result->error != OK){
    return signingData_result;
  }

  ByteArray_t* signingData = signingData_result->result;

  return user_HashProvider->createHash(signingData);
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
*     *user_XYObject                     [in]       BoundWitness_t*
*
*  RETURNS
*      XYResult_t*            [out]      bool   Returns XYObject_t* with a char* as the result.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitnessTransfer_toBytes(XYObject_t* user_XYObject){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  BoundWitnessTransfer_t* user_BoundWitness = user_XYObject->payload;
  uint32_t totalSize = 5;
  uint32_t publicKeysSize = 0;
  uint32_t payloadsSize = 0;
  uint32_t signaturesSize = 0;

  XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider_t* ShortStrongArray_creator = lookup_result->result;
  //free(lookup_result);

  lookup_result = tableLookup((char*)&IntStrongArray_id);
  if(lookup_result->error != OK){
   RETURN_ERROR(ERR_CRITICAL);
  }
  ObjectProvider_t* IntStrongArray_creator = lookup_result->result;
  //free(lookup_result);

  XYResult_t* newObject_result = NULL;
  XYResult_t* publicKeysBytes_result = NULL;
  XYResult_t* payloadsBytes_result = NULL;
  XYResult_t* signaturesBytes_result = NULL;

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

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = return_bytes;
  
  return preallocated_return_result_ptr;
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
*     *user_BoundWitness  [in]       BoundWitness_t*
*
*  RETURNS
*      XYResult_t*          [out]      bool   Returns XYResult_t* of the BoundWitnessTransfer type.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitnessTransfer_fromBytes(char* user_Transfer){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_Transfer) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  BoundWitnessTransfer_t* return_BoundWitness = malloc(sizeof(BoundWitnessTransfer_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!return_BoundWitness) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  uint32_t BoundWitnessSize = to_uint32((char*)&user_Transfer);

  if(littleEndian()){
    BoundWitnessSize = to_uint32((char*)&BoundWitnessSize);
  }
  uint8_t choice = user_Transfer[4];

  return_BoundWitness->size = BoundWitnessSize;
  return_BoundWitness->choice = choice;
  return_BoundWitness->getSigningData = BoundWitness_getSigningData;
  return_BoundWitness->getHash = BoundWitness_getHash;
  return_BoundWitness->publicKeys = NULL;
  return_BoundWitness->payloads = NULL;
  return_BoundWitness->signatures = NULL;
  ShortStrongArray_t* publicKeysPtr = NULL;
  IntStrongArray_t* payloadPtr = NULL;
  ShortStrongArray_t* signaturesPtr = NULL;

  if(choice == 0x01){

    // Set up the Public Key Array

    uint16_t publicKeysSize = to_uint16((unsigned char*)&user_Transfer[5]);
    XYResult_t* lookup_result = tableLookup((const char*)&ShortStrongArray_id);

    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5]);
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
    lookup_result = tableLookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5+publicKeysSize*sizeof(char)]);

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


  } else if(choice == 0x02){
    // Set up the Public Key Array
    uint16_t publicKeysSize = to_uint16((unsigned char*)&user_Transfer[5]);
    XYResult_t* lookup_result = tableLookup((const char*)&ShortStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      //free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5*sizeof(char)]);
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
    lookup_result = tableLookup((char*)&IntStrongArray_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SWA_Creator = lookup_result->result;
      //free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5+publicKeysSize*sizeof(char)]);

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
      //free(lookup_result);
      XYResult_t* fromBytes_result = SWA_Creator->fromBytes((char*)&user_Transfer[5+publicKeysSize+payloadSize*sizeof(char)]);
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
    XYResult_t* lookup_result = tableLookup((char*)&SignatureSet_id);
    if(lookup_result->error == OK){
      ObjectProvider_t* SSA_Creator = lookup_result->result;
      //free(lookup_result);
      XYResult_t* fromBytes_result = SSA_Creator->fromBytes((char*)&user_Transfer[5]);
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
*      XYResult_t*            [out]      bool   Returns XYObject_t* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitnessTransfer_creator_create(const char id[2], void* user_data){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_data) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

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
*      XYResult_t*            [out]      bool   Returns XYObject_t* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_creator_create(const char id[2], void* user_data){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_data) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

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
*     *data       [in]       char*
*
*  RETURNS
*      XYResult_t*  [out]      bool   Returns XYResult_t* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_creator_fromBytes(char* data){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!data) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  BoundWitness_t* return_BoundWitness = malloc(sizeof(BoundWitness_t));
  uint32_t BoundWitnessSize = to_uint32((unsigned char*)data);
  return_BoundWitness->size = BoundWitnessSize;
  ShortStrongArray_t* publicKeysPtr;
  IntStrongArray_t* payloadPtr;
  ShortStrongArray_t* signaturesPtr;

  /*
   * We process each of the elements in a Bound Witness
   * in separate chunks here and assign all the pointers
   * at the end. First is publicKeys.
   */
  uint16_t publicKeysSize = to_uint16((unsigned char*)&data[4]);
  XYResult_t* lookup_result = tableLookup((const char*)&ShortStrongArray_id);
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
  uint32_t payloadSize = to_uint32((unsigned char*)&data[4+publicKeysSize*sizeof(char)]);
  if(!littleEndian()){
    payloadSize = to_uint32((unsigned char*)&payloadSize);
  }
  ////free(lookup_result);
  lookup_result = tableLookup((const char*)&IntStrongArray_id);
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
 ////free(lookup_result);
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

  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = return_BoundWitness;
  
  return preallocated_return_result_ptr;
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
*    *user_XYObject     [in]       XYObject_t*
*
*  RETURNS
*      XYResult_t*      [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* BoundWitness_creator_toBytes(XYObject_t* user_XYObject){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA);}  // return an error now

  BoundWitness_t* user_BoundWitness = user_XYObject->payload;
  XYResult_t* lookup_result = tableLookup((char*)&ShortStrongArray_id);
  ObjectProvider_t* SSA_Creator = lookup_result->result;
  ////free(lookup_result);
  lookup_result = tableLookup((char*)&IntStrongArray_id);
  ObjectProvider_t* ISA_Creator = lookup_result->result;
  /* TODO: AFAIK this should have worked. Instead of using boundwitness->size we dynamically calculate it. */
  /* TODO: Payload Size in incorrect here */
  //char* byteBuffer = malloc(user_BoundWitness->size*sizeof(char));
  //printf("Payload size: %d\n", to_uint32((unsigned char*)&user_BoundWitness->payloads->size));
  char* byteBuffer = NULL;
  uint32_t encodedSize = 0;
  if(user_BoundWitness->payloads->size > CAUSES_BUGS){
    byteBuffer = malloc((4*sizeof(char))+user_BoundWitness->publicKeys->size + to_uint32((unsigned char*)&user_BoundWitness->payloads->size) + user_BoundWitness->signatures->size);
    encodedSize = (4*sizeof(char))+user_BoundWitness->publicKeys->size + to_uint32((unsigned char*)&user_BoundWitness->payloads->size) + user_BoundWitness->signatures->size;
    user_BoundWitness->size = encodedSize;
  } else {
    byteBuffer = malloc((4*sizeof(char))+user_BoundWitness->publicKeys->size + user_BoundWitness->payloads->size + user_BoundWitness->signatures->size);
    encodedSize = (4*sizeof(char))+user_BoundWitness->publicKeys->size + user_BoundWitness->payloads->size + user_BoundWitness->signatures->size;
    user_BoundWitness->size = encodedSize;
  }

  if(littleEndian()){
    encodedSize = to_uint32((unsigned char*)&encodedSize);
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
  if(littleEndian() && user_payloadSize > CAUSES_BUGS){
    user_payloadSize = to_uint32((unsigned char*)&user_payload->size);
    memcpy(user_payload, &user_payloadSize, 4);
  }
  //printf("%d\n", user_payloadSize);
  newObject_result = newObject((char*)&Payload_id, user_payload); //TODO: Changed to payload_id here
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

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = byteBuffer;
  return preallocated_return_result_ptr;
}

// end of file boundwitness.c

