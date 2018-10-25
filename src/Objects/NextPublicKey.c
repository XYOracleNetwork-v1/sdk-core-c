/**
 ****************************************************************************************
 *
 * @file nextpublickey.c
 *
 * @XY4 project source code.
 *
 * @brief primary next public key routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                   [in]       char*
*     *user_data            [in]       void*
*
*  RETURNS
*      XYResult_t*          [out]      bool   Returns XYObject_t* of the NextPublicKey type.
*----------------------------------------------------------------------------*/
XYResult_t* NextPublicKey_creator_create(char id[2], void* user_data){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_data) {RETURN_ERROR(ERR_BADDATA)};

  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_fromBytes
*
*  DESCRIPTION
*      Create an Next Public Key object given a set of Bytes. Bytes must be
*      payload of Next Public Key object.
*
*  PARAMETERS
*     *pubkey_data      [in]       char*
*
*  RETURNS
*      XYResult_t*      [out]      bool   Returns XYResult* of the NextPublicKey type.
*----------------------------------------------------------------------------*/
XYResult_t* NextPublicKey_creator_fromBytes(char* pubkey_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!pubkey_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  memcpy(id, pubkey_data, 2);
  char pubkey_id[2];
  memcpy(pubkey_id, pubkey_data+(sizeof(char)*2), 2);
  
  XYResult_t* lookup_result = tableLookup(id);
  XYResult_t* lookup_result2 = tableLookup(pubkey_id);
  
  NextPublicKey_t* return_NPK = malloc(sizeof(NextPublicKey_t));
  
  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(lookup_result->error == OK && lookup_result2->error == OK && return_NPK){
    
    ObjectProvider_t* NPK_creator = lookup_result2->result;
    uint32_t element_size = 0;
    
    if(NPK_creator->defaultSize != 0){
      element_size = NPK_creator->defaultSize;
      return_NPK->publicKey = malloc(element_size*sizeof(char));
    }
    else if(NPK_creator->sizeIdentifierSize != 0)
    {
      switch(NPK_creator->sizeIdentifierSize){
        case 1:
          element_size = pubkey_data[4];
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
        case 2:
          element_size = to_uint16(&pubkey_data[4]);
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
        case 4:
          element_size = to_uint32(&pubkey_data[4]);
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
      }
    }
    else
    {
      RETURN_ERROR(ERR_CRITICAL);
    }
    
    if(return_NPK->publicKey){
      memcpy(return_NPK->id, &pubkey_id, 2);
      memcpy(return_NPK->publicKey, pubkey_data+(sizeof(char)*4), (2 * sizeof(char))+(element_size*sizeof(char)));
    }
    else
    {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }

    preallocated_return_result_ptr = &preallocated_return_result;

    preallocated_return_result_ptr->error = OK;
    preallocated_return_result_ptr->result = return_NPK;
      
    return preallocated_return_result_ptr;

  } else if(lookup_result->error == OK && lookup_result2->error == OK){
    if(lookup_result) free(lookup_result);
    if(lookup_result2) free(lookup_result2);
    if(return_NPK) free(return_NPK);
    
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  } else {
    if(lookup_result) free(lookup_result);
    if(lookup_result2) free(lookup_result2);
    if(return_NPK) free(return_NPK);
    
    RETURN_ERROR(ERR_BADDATA);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Next Public Key type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult_t*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* NextPublicKey_creator_toBytes(XYObject_t* user_XYObject){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  if(user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x07){
    
    NextPublicKey_t* user_NPK = (user_XYObject->GetPayload(user_XYObject))->result;
    
    char id[2];
    memcpy(&id, user_NPK->id, 2);
    XYResult_t* lookup_result = tableLookup(id);
    
    if(lookup_result->error == OK){
      
      ObjectProvider_t* NPK_creator = lookup_result->result;
      uint32_t element_size = 0;
      char* byteBuffer;
      
      if(NPK_creator->defaultSize != 0){
        element_size = NPK_creator->defaultSize;
        byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
        //TODO: wal, should check for any malloc errors

        memcpy(byteBuffer, &id, 2);
        memcpy(byteBuffer+2, user_NPK->publicKey, (element_size*sizeof(char)));
      }
      else if(NPK_creator->sizeIdentifierSize != 0)
      {
        char* casted_NPK = (char*)user_NPK->publicKey;
        switch(NPK_creator->sizeIdentifierSize){
          case 1:
            element_size = casted_NPK[0];
            byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
            //TODO: wal, should check for any malloc errors

            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+2, user_NPK->publicKey, 2*sizeof(char) + (element_size*sizeof(char)));
            break;
          case 2:
          {
            element_size = to_uint16(&casted_NPK[0]);
            uint16_t encodedSize16 = element_size;
            byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
            //TODO: wal, should check for any malloc errors

            if(littleEndian()){
               encodedSize16 = to_uint16((char*)&element_size);
            }
            
            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize16, 2);
            memcpy(byteBuffer+(4*sizeof(char)), user_NPK->publicKey+2, (element_size-(sizeof(char)*2)));
            break;
          }
          case 4:
            element_size = to_uint32(&casted_NPK[0]);
            uint32_t encodedSize32 = element_size;
            byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
            //TODO: wal, should check for any malloc errors

            if(littleEndian()){
               encodedSize32 = to_uint32((char*)&element_size);
            }
            
            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize32, 4);
            memcpy(byteBuffer+(6*sizeof(char)), user_NPK->publicKey+4, (element_size-(sizeof(char)*4)));
            break;
        }
      }
      else
      {
        RETURN_ERROR(ERR_CRITICAL);
      }

      preallocated_return_result_ptr = &preallocated_return_result;

      preallocated_return_result_ptr->error = OK;
      preallocated_return_result_ptr->result = byteBuffer;
        
      return preallocated_return_result_ptr;
    }
    else
    {
      RETURN_ERROR(ERR_BADDATA);
    }
  }
  else
  {
    RETURN_ERROR(ERR_BADDATA);
  }
}

// end of file nextpublickey.c

