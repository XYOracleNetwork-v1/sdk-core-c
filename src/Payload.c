/**
 ****************************************************************************************
 *
 * @file payload.c
 *
 * @XYO Core library source code.
 *
 * @brief primary payload routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include <string.h>
#include "objects.h"

/*----------------------------------------------------------------------------*
*  NAME
*      Payload_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult_t*           [out]      bool   Returns XYObject_t* of the Payload type.
*----------------------------------------------------------------------------*/
XYResult_t* Payload_creator_create(const char id[2], void* payload_data){

  /********************************/
  /* guard against bad input data */
  /******************************** /

  //if(!payload_data) {RETURN_ERROR(ERR_BADDATA);}

  if(payload_data == NULL){
    Payload_t* user_payload = malloc(sizeof(Payload_t));
    XYResult_t* lookup_result = tableLookup((const char*)&IntWeakArray_id);
    ObjectProvider_t* array_creator = lookup_result->result;
    XYResult_t* arrayCreate_result = array_creator->create((const char*)&IntWeakArray_id, NULL);
    user_payload->signedHeuristics = ((XYObject_t*)arrayCreate_result->result)->payload;
    arrayCreate_result = array_creator->create((const char*)&IntWeakArray_id, NULL);
    user_payload->unsignedHeuristics = ((XYObject_t*)arrayCreate_result->result)->payload;
    user_payload->size = user_payload->signedHeuristics->size + user_payload->unsignedHeuristics->size + 4;
    return newObject(id, user_payload);
  }
  return newObject(id, payload_data);
  */
}

/*----------------------------------------------------------------------------*
*  NAME
*      Payload_creator_fromBytes
*
*  DESCRIPTION
*      Create an Payload object given a set of Bytes. Bytes must not
*      include major and minor of array.
*
*  PARAMETERS
*     *payload_data     [in]       char*
*
*  RETURNS
*      XYResult_t*      [out]      bool   Returns XYResult_t* of the Payload type.
*----------------------------------------------------------------------------*/
XYResult_t* Payload_creator_fromBytes(char* payload_data){

  /********************************/
  /* guard against bad input data */
  /******************************** /

  if(!payload_data) {RETURN_ERROR(ERR_BADDATA);}

  Payload_t* return_payload = malloc(sizeof(Payload_t));

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(!return_payload) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  char IntWeakArrayID[2] = {0x01, 0x06};
  XYResult_t* lookup_result = tableLookup(IntWeakArrayID);
  ObjectProvider_t* weakArrayCreator = lookup_result->result;

  IntWeakArray_t* signedWeakArray;
  IntWeakArray_t* unsignedWeakArray;
  XYResult_t* array_result = weakArrayCreator->fromBytes(&payload_data[sizeof(char)*4]);

  if(array_result->error==OK){
    signedWeakArray = array_result->result;
    uint32_t firstSize = to_uint32((char*)&payload_data[4]);
    /*
     * We ask the first payload what it's size is and then add that to
     * the offset of 4 (for Payload.size) in order to derrive the position
     * of the second array in the payload.
     * /
    array_result = weakArrayCreator->fromBytes(&payload_data[sizeof(char)*(firstSize+4)]);

    if(array_result->error==OK){
      unsignedWeakArray = array_result->result;
    }
    else
    {
      free(return_payload);
      return array_result;
    }
  }
  else
  {
    free(return_payload);
    return array_result;
  }
  return_payload->size = to_uint32((unsigned char*)payload_data);
  return_payload->signedHeuristics = signedWeakArray;
  return_payload->unsignedHeuristics = unsignedWeakArray;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = return_payload;

  return preallocated_return_result_ptr;
  */
}

/*----------------------------------------------------------------------------*
*  NAME
*      Payload_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Bound Witness type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject_t*
*
*  RETURNS
*      XYResult_t*          [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* Payload_creator_toBytes(XYObject_t* user_XYObject){

  /********************************/
  /* guard against bad input data */
  /******************************** /

  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA);}

  Payload_t* user_Payload = user_XYObject->payload;
  uint32_t size = user_Payload->size;

  char IntWeakArrayID[2] = {0x01, 0x06};
  XYResult_t* lookup_result = tableLookup(IntWeakArrayID);
  ObjectProvider_t* weakArrayCreator = lookup_result->result;
  IntWeakArray_t* signedArray = user_Payload->signedHeuristics;
  IntWeakArray_t* unsignedArray = user_Payload->unsignedHeuristics;
  uint32_t size1 = 4;
  uint32_t size2 = 4;
  if(signedArray != NULL){
      size1 = signedArray->size;
  }
  if(unsignedArray != NULL){
    size2 = unsignedArray->size;
  }
  XYResult_t* toBytes_result1 = NULL;
  XYResult_t* toBytes_result2 = NULL;
  /* Take the array_raw in, endian the size around, then do toBytes() * /

  XYResult_t* newObject_result1 = newObject(IntWeakArrayID, user_Payload->signedHeuristics);
  if(newObject_result1->error == OK){
    toBytes_result1 = weakArrayCreator->toBytes( newObject_result1->result );
    if(toBytes_result1->error!=OK){
      return toBytes_result1;
    }
  }
  else {
    return newObject_result1;
  }
  char* return_buffer = malloc((sizeof(uint32_t))+(sizeof(char)*size1)+(sizeof(char)*size2));

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(!return_buffer) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  /* TODO: payloadsSize is off by 4 here and I am not sure why. disconserting. * /
  uint32_t encoded_size = size1 + size2 + (4*sizeof(char));
  if(littleEndian()){
    encoded_size = to_uint32((unsigned char*)&encoded_size);
  }

  char* signedHeuristicBytes = toBytes_result1->result;
  memcpy(return_buffer, &encoded_size, sizeof(uint32_t));

  memcpy(return_buffer+(sizeof(uint32_t)), signedHeuristicBytes, size1*sizeof(char));

  XYResult_t* newObject_result2 = newObject(IntWeakArrayID, user_Payload->unsignedHeuristics);
  if(newObject_result2->error == OK){
    toBytes_result2 = weakArrayCreator->toBytes( newObject_result2->result );
    if(toBytes_result2->error!=OK){
      return toBytes_result2;
    }
  }
  else {
    return newObject_result2;
  }
  char* unsignedHeuristicBytes = toBytes_result2->result;

  memcpy(return_buffer+(sizeof(uint32_t))+(sizeof(char)*size1), unsignedHeuristicBytes, size2*sizeof(char));

  // Cleanup XYResults
  /*
  free(newObject_result1);
  free(newObject_result2);
  free(toBytes_result1);
  free(toBytes_result2);
  //free(lookup_result);
  * /

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = return_buffer;

  return preallocated_return_result_ptr;
  */
}
