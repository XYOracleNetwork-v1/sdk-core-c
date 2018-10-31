/**
 ****************************************************************************************
 *
 * @file index.c
 *
 * @XYO Core library source code.
 *
 * @brief primary indexing routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
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
*     *id                         [in]       char*
*     *user_data                  [in]       void*
*
*  RETURNS
*      newObject(id, user_data)   [out]      bool   Returns XYObject_t* of the Index type.
*----------------------------------------------------------------------------*/
XYResult_t* Index_creator_create(char id[2], void* user_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_data) {RETURN_ERROR(ERR_BADDATA)};

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
*     index_data              [in]       char*
*
*  RETURNS
*     newObject(id, &index)   [out]      XYResult_t*   Returns XYResult* of the Index type.
*----------------------------------------------------------------------------*/
XYResult_t* Index_creator_fromBytes(char* index_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!index_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  memcpy(id, index_data, 2);
  uint32_t* index = malloc(sizeof(uint32_t));
  uint32_t indexref = to_uint32((unsigned char*)&index_data[2]);
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
*    *user_XYObject         [in]       XYObject_t*
*
*  RETURNS
*      XYResult_t*          [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* Index_creator_toBytes(XYObject_t* user_XYObject){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  uint32_t encoded_bytes;
  char* index = (char*)user_XYObject->payload;
  
  encoded_bytes = to_uint32(&index[0]);
  
  if(!littleEndian()){
    encoded_bytes = to_uint32((char*)&encoded_bytes);
  }

  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &encoded_bytes;
    
  return preallocated_return_result_ptr;
}

// end of file index.c

