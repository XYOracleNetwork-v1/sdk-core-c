/**
 ****************************************************************************************
 *
 * @file PreviousHash.c
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

#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
/*----------------------------------------------------------------------------*
*  NAME
*      PreviousHash_creator_create
*
*  DESCRIPTION
*      Create an Previous Hash given user data.
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the PreviousHash type.
*----------------------------------------------------------------------------*/
XYResult* PreviousHash_creator_create(const char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      PreviousHash_creator_fromBytes
*
*  DESCRIPTION
*      Create an Prevoius Hash object given a set of Bytes. Bytes must be
*      payload of Prevoius Hash object
*
*  PARAMETERS
*     *hash_data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the PreviousHash type.
*----------------------------------------------------------------------------*/
XYResult* PreviousHash_creator_fromBytes(char* hash_data){
  char id[2];
  memcpy(id, hash_data, 2);
  char hash_id[2];
  memcpy(hash_id, hash_data+(sizeof(char)*2), 2);
  XYResult* lookup_result = lookup(id);
  XYResult* lookup_result2 = lookup(hash_id);
  PreviousHash* return_PH = malloc(sizeof(PreviousHash));
  if(lookup_result->error == OK && lookup_result2->error == OK && return_PH){
    ObjectProvider* PH_creator = lookup_result2->result;
    uint32_t element_size = 0;
    if(PH_creator->defaultSize != 0){
      element_size = PH_creator->defaultSize;
      return_PH->hash = malloc(element_size*sizeof(char));
    }
    else if(PH_creator->sizeIdentifierSize != 0)
    {
      switch(PH_creator->sizeIdentifierSize){
        case 1:
          element_size = hash_data[4];
          return_PH->hash = malloc(element_size*sizeof(char));
          break;
        case 2:
          element_size = to_uint16((unsigned char*)&hash_data[4]);
          return_PH->hash = malloc(element_size*sizeof(char));
          break;
        case 4:
          element_size = to_uint32((unsigned char*)&hash_data[4]);
          return_PH->hash = malloc(element_size*sizeof(char));
          break;
      }
    }
    else
    {
      RETURN_ERROR(ERR_CRITICAL);
    }
    if(return_PH->hash){
      memcpy(return_PH->id, &hash_id, 2);
      memcpy(return_PH->hash, hash_data+(sizeof(char)*4), (2 * sizeof(char))+(element_size*sizeof(char)));
    }
    else
    {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result){
      return_result->error = OK;
      return_result->result = return_PH;
      return return_result;
    }
    else
    {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  } else if(lookup_result->error == OK && lookup_result2->error == OK){
    if(lookup_result) free(lookup_result);
    if(lookup_result2) free(lookup_result2);
    if(return_PH) free(return_PH);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  } else{
    if(lookup_result) free(lookup_result);
    if(lookup_result2) free(lookup_result2);
    if(return_PH) free(return_PH);
    RETURN_ERROR(ERR_BADDATA);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      PreviousHash_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Next Public Key type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* PreviousHash_creator_toBytes(XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x06){
    //XYObject* PreviousHash_Object = user_XYObject->GetPayload(user_XYObject);
    PreviousHash* user_PH = user_XYObject->GetPayload(user_XYObject);
    char id[2];
    memcpy(&id, user_XYObject->id, sizeof(char)*2);
    XYResult* lookup_result = lookup(id);
    if(lookup_result->error == OK){
      ObjectProvider* PH_creator = lookup_result->result;
      uint32_t element_size = 0;
      char* byteBuffer;

      if(PH_creator->defaultSize != 0){
        element_size = PH_creator->defaultSize;
        byteBuffer = malloc((element_size*sizeof(char)));
        memcpy(byteBuffer, user_PH->id, 2);
        memcpy(byteBuffer+2, user_PH->hash, (element_size-2*sizeof(char)));
      }
      else if(PH_creator->sizeIdentifierSize != 0)
      {
        char* casted_PH = (char*)user_PH->hash;
        switch(PH_creator->sizeIdentifierSize){
          case 1:
            element_size = casted_PH[0];
            byteBuffer = malloc((element_size*sizeof(char)));
            memcpy(byteBuffer, user_PH->hash, (element_size*sizeof(char)));
            break;
          case 2:
          {
            element_size = to_uint16((unsigned char*)&casted_PH[0]);
            uint16_t encodedSize16 = (uint16_t)element_size;
            encodedSize16 = (element_size*sizeof(char));
            byteBuffer = malloc(element_size);
            if(littleEndian()){
               encodedSize16 = to_uint16((unsigned char*)&element_size);
            }
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize16, 2);
            memcpy(byteBuffer+(4*sizeof(char)), user_PH->hash+2, (element_size-(sizeof(char)*2)));
            break;
          }
          case 4:
            element_size = to_uint32((unsigned char*)&casted_PH[0]);
            uint32_t encodedSize32 = element_size;
            byteBuffer = malloc((element_size*sizeof(char)));
            if(littleEndian()){
               encodedSize32 = to_uint32((unsigned char*)&element_size);
            }
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize32, 4);
            memcpy(byteBuffer+(6*sizeof(char)), user_PH->hash+4, (element_size-(sizeof(char)*4)));
            break;
        }
      }
      else
      {
        RETURN_ERROR(ERR_CRITICAL);
      }
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result){
        return_result->error = OK;
        return_result->result = byteBuffer;
        return return_result;
      }
      else
      {
        free(byteBuffer);
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
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
