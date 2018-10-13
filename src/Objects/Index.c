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

#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

/*----------------------------------------------------------------------------*
*  NAME
*      Index_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the Index type.
*----------------------------------------------------------------------------*/
XYResult* Index_creator_create(char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      Index_creator_fromBytes
*
*  DESCRIPTION
*      Create an Bound Witness object given a set of Bytes. Bytes must not
*      include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the Index type.
*----------------------------------------------------------------------------*/
XYResult* Index_creator_fromBytes(char* index_data){
  char id[2];
  memcpy(id, index_data, 2);
  uint32_t* index = malloc(sizeof(uint32_t));
  uint32_t indexref = to_uint32(&index_data[2]);
  memcpy(index, &indexref, 4);
  return newObject(id, index);
}

/*----------------------------------------------------------------------------*
*  NAME
*      Index_creator_toBytes
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
XYResult* Index_creator_toBytes(struct XYObject* user_XYObject){
  uint32_t encoded_bytes;
  char* index = (char*)user_XYObject->payload;
  encoded_bytes = to_uint32(&index[0]);
  if(!littleEndian()){
    encoded_bytes = to_uint32((char*)&encoded_bytes);
  }


  struct XYResult* return_result = malloc(sizeof(struct XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = &encoded_bytes;
    return return_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }
}
