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

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

//TODO Each array should have it's own header or one master header

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied ByteStrongArray
*
*  PARAMETERS
*     *self_ByteStrongArray  [in]       XYObject*
*     *user_XYObject          [in]      ByteStrongArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_add(ByteStrongArray* self_ByteStrongArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject

  // Lookup the ObjectProvider for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.
  XYResult* lookup_result = lookup(user_XYObject->id);
  if(lookup_result->error == OK){
    ObjectProvider* user_ObjectProvider = lookup_result->result;

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint16_t newSize = 0;
    uint8_t object_size = 0;

    if(user_ObjectProvider->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_ObjectProvider->defaultSize;
      newSize = (self_ByteStrongArray->size + object_size);
    }
    else if(user_ObjectProvider->sizeIdentifierSize != 0){

      // Get a pointer to beginning of the array to read the size.
      char* object_payload = user_XYObject->payload;

      // Size identifier Size tells you how many bytes to read for size
      switch(user_ObjectProvider->sizeIdentifierSize){
        case 1:
          object_size = object_payload[0];
          break;
        case 2:
          /* First we read 2 bytes of the payload to get the size,
           * the to_uint16 function reads ints in big endian.
           */
          object_size = to_uint16(object_payload); //TODO: Check compatibility on big endian devices.
          if(littleEndian()){
            object_size = to_uint16((char*)&object_size);
          }
          break;
        case 4:
          object_size = to_uint32((char*)object_payload);
          if(littleEndian()){
            object_size = to_uint32((char*)&object_size);
          }
          break;
      }
      newSize = (self_ByteStrongArray->size + object_size * sizeof(char));
    }
    else
    {
      /*
       * If both the SizeOfSize identifier and defaultSize are 0,
       * we have to read one layer deeper to retrieve the defaultSize
       */
       char* user_object_payload = user_XYObject->payload;
       char id[2];
       memcpy(id, user_object_payload, 2);
       lookup_result = lookup(id);
       if(lookup_result->error == OK){
         ObjectProvider* deeper_ObjectProvider = lookup_result->result;
         if(deeper_ObjectProvider->defaultSize != 0){

           object_size = deeper_ObjectProvider->defaultSize;

           newSize = (self_ByteStrongArray->size + object_size);
         }
         else if(deeper_ObjectProvider->sizeIdentifierSize != 0){
           /* Unimplemented */
         }
       }
    }

    // Total size (expressed in bytes) of the Byte Strong Array can't exceed
    // that which can be expressed by one byte. Thus the max acceptible totalSize
    // is only 255 bytes.
    if(newSize < 255){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_ByteStrongArray->payload != NULL){
        self_ByteStrongArray->payload = realloc(self_ByteStrongArray->payload, newSize);
      }
      else {
        self_ByteStrongArray->payload = malloc(newSize*sizeof(char));
      }

      if(self_ByteStrongArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.
        char* object_payload = self_ByteStrongArray->payload;
        object_payload = &(object_payload[self_ByteStrongArray->size - (sizeof(char)*3)]);

        // Finally copy the element into the array
        XYResult* toBytes_result = user_ObjectProvider->toBytes(user_XYObject);
        memcpy(object_payload, toBytes_result->result, object_size);

        self_ByteStrongArray->size = newSize;
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result != NULL){
          return_result->error = OK;
          return_result->result = NULL;
          return return_result;
        }
        else {
          RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
        }
      }
      else {
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
    }
    else {
      RETURN_ERROR(ERR_BADDATA);
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied ByteStrongArray at a supplied index.
*
*  PARAMETERS
*     *self_ByteStrongArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_get(ByteStrongArray* self_ByteStrongArray, int index) {
  XYResult* general_result = lookup(self_ByteStrongArray->id);
  if(general_result->error == OK){
    ObjectProvider* element_creator = general_result->result;
    if(element_creator->defaultSize != 0){
      uint8_t totalSize = self_ByteStrongArray->size;
      totalSize = totalSize - 3*sizeof(char);
      if((totalSize % element_creator->defaultSize) == 0){
        char* array_elements = self_ByteStrongArray->payload;
        return newObject(self_ByteStrongArray->id, &array_elements[element_creator->defaultSize*index]);
      }
      else {
        RETURN_ERROR(ERR_BADDATA);
      }
    }
    else if(element_creator->sizeIdentifierSize != 0)
    {
      uint8_t totalSize = self_ByteStrongArray->size;
      char* array_elements = self_ByteStrongArray->payload;
      uint8_t array_offset = 0;
      for(int i = 0; i<=index; i++){
        if(array_offset>totalSize){
          RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
        }
        char* element_size = malloc(element_creator->sizeIdentifierSize);
        memcpy(element_size, &array_elements[array_offset], element_creator->sizeIdentifierSize);
        uint16_t int_size = to_uint16(element_size);
        free(element_size);
        if(i == index){
          char* return_object_payload = malloc(int_size);
          memcpy(return_object_payload, &array_elements[array_offset], int_size);
          XYResult* return_result = newObject(self_ByteStrongArray->id, return_object_payload);
          return return_result;
        }
        else {
          array_offset += int_size;
        }
      }
    }
    else {
      RETURN_ERROR(ERR_BADDATA);
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA);
  }
  RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ByteStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  ByteStrongArray* ByteStrongArrayObject = malloc(sizeof(ByteStrongArray));
  char byteStrongArrayID[2] = {0x01, 0x01};
  XYResult* newObject_result = newObject(byteStrongArrayID, ByteStrongArrayObject);
  if(newObject_result->error == OK && ByteStrongArrayObject != NULL){
    ByteStrongArrayObject->id[0] = id[0];
    ByteStrongArrayObject->id[1] = id[1];
    ByteStrongArrayObject->size = 3;
    ByteStrongArrayObject->add = &ByteStrongArray_add;
    ByteStrongArrayObject->get = &ByteStrongArray_get;
    ByteStrongArrayObject->payload = NULL;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = OK;
      XYObject* return_object = newObject_result->result;
      return_result->result = return_object;
      return return_result;
    }
    else {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  }
  else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ByteStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_creator_fromBytes(char* data){
  XYResult* return_result = malloc(sizeof(XYResult));
  ByteStrongArray* return_array = malloc(sizeof(ByteStrongArray));
  if(return_result && return_array){
      return_array->add = &ByteStrongArray_add;
      return_array->remove = NULL;
      return_array->get = &ByteStrongArray_get;
      return_array->size = data[0];
      char array_id[3];
      array_id[0] = data[1];
      array_id[1] = data[2];
      array_id[2] = '\00';
      strcpy(return_array->id, array_id);
      return_array->payload = malloc(sizeof(char)*(return_array->size-3));
      memcpy(return_array->payload, &data[3], (return_array->size-3));
      return_result->error = OK;
      return_result->result = return_array;
      return return_result;
  }
  else{
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_creator_create
*
*  DESCRIPTION
*      Given an XYObject of Byte Strong Array type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_creator_toBytes(struct XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x01){
    ByteStrongArray* user_array = user_XYObject->GetPayload(user_XYObject);
    uint8_t totalSize = user_array->size;
    char* byteBuffer = malloc(sizeof(char)*totalSize);
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL && byteBuffer != NULL){
      memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 3);
      memcpy(byteBuffer+3, user_array->payload, sizeof(char)*(totalSize-3));
      return_result->error = OK;
      return_result->result = byteBuffer;
      return return_result;
      } else {
        if(return_result) free(return_result);
        if(byteBuffer) free(byteBuffer);
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA)
  }

}
