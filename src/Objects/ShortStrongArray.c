/**
 ****************************************************************************************
 *
 * @file ShortStrongArray.c
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

/*----------------------------------------------------------------------------*
*  NAME
*      ShortStrongArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied ShortStrongArray
*
*  PARAMETERS
*     *self_ShortStrongArray  [in]       XYObject*
*     *user_XYObject          [in]      ShortStrongArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ShortStrongArray_add(ShortStrongArray* self_ShortStrongArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject

  // Lookup the ObjectProvider for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.
  XYResult* lookup_result = lookup(user_XYObject->id);
  if(lookup_result->error == OK){
    ObjectProvider* user_ObjectProvider = lookup_result->result;
    free(lookup_result);

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint32_t newSize = 0;
    uint16_t object_size = 0;
    uint8_t byteArray = FALSE;

    if(user_ObjectProvider->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_ObjectProvider->defaultSize;
      newSize = (self_ShortStrongArray->size + object_size);
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
          object_size = to_uint16((unsigned char*)object_payload); //TODO: Check compatibility on big endian devices.
          if(littleEndian()){
            object_size = to_uint16((unsigned char*)&object_size);
          }
          break;
        case 4:
          object_size = to_uint32((unsigned char*)object_payload);
          if(littleEndian()){
            object_size = to_uint32((unsigned char*)&object_size);
          }
          break;
      }
      newSize = (self_ShortStrongArray->size + object_size * sizeof(char));
    }
    else
    {
      /*
       * If both the SizeOfSize identifier and defaultSize are 0,
       * we have to read one layer deeper to retrieve the defaultSize
       */
       byteArray = TRUE;
       ByteArray* user_array = user_XYObject->payload;
       char* user_object_payload = user_array->payload;
       char id[2];
       memcpy(id, user_object_payload, 2);
       lookup_result = lookup((char*)&id);
       if(lookup_result->error == OK){
         ObjectProvider* deeper_ObjectProvider = lookup_result->result;
         if(deeper_ObjectProvider->defaultSize != 0){

           object_size = deeper_ObjectProvider->defaultSize;

           newSize = (self_ShortStrongArray->size + object_size);
         }
         else if(deeper_ObjectProvider->sizeIdentifierSize != 0){
           return NULL;
         } else {
         object_size = user_array->size;
         newSize = (self_ShortStrongArray->size + object_size);
        }
       } else {
        RETURN_ERROR(ERR_BADDATA);
      }
    }
    // Total Size should not exceed the size mandated by the type (Short)
    if(newSize < 65536){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_ShortStrongArray->payload != NULL){
        self_ShortStrongArray->payload = realloc(self_ShortStrongArray->payload, newSize-(sizeof(char)*4));
      }
      else {
        self_ShortStrongArray->payload = malloc(newSize-(sizeof(char)*4));
      }

      if(self_ShortStrongArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.
        char* object_payload = self_ShortStrongArray->payload;
        object_payload = &(object_payload[self_ShortStrongArray->size - (sizeof(char)*4)]);

        // Finally copy the element into the array
        if(byteArray){
          ByteArray* user_array = user_XYObject->payload;
          memcpy(object_payload, user_array->payload, user_array->size);
        } else {
          XYResult* toBytes_result = user_ObjectProvider->toBytes(user_XYObject);
          memcpy(object_payload, toBytes_result->result, object_size);
          free(toBytes_result->result);
          free(toBytes_result);
        }
        self_ShortStrongArray->size = newSize;
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result != NULL){
          return_result->error = OK;
          return_result->result = 0;
          return return_result;
        }
        else {
          preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
          preallocated_result->result = 0;
          return preallocated_result;
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
*      ShortStrongArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied ShortStrongArray at a supplied index.
*
*  PARAMETERS
*     *self_ShortStrongArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ShortStrongArray_get(ShortStrongArray* self_ShortStrongArray, int index) {
  XYResult* general_result = lookup(self_ShortStrongArray->id);
  if(general_result->error == OK){
    ObjectProvider* element_creator = general_result->result;
    if(element_creator->defaultSize != 0){
      uint8_t totalSize = self_ShortStrongArray->size;
      totalSize = totalSize - 4*sizeof(char);
      if((totalSize % element_creator->defaultSize) == 0){
        char* array_elements = self_ShortStrongArray->payload;
        return newObject(self_ShortStrongArray->id, &array_elements[element_creator->defaultSize*index]);
      }
      else {
        RETURN_ERROR(ERR_BADDATA);
      }
    }
    else if(element_creator->sizeIdentifierSize != 0)
    {
      uint16_t totalSize = self_ShortStrongArray->size;
      char* array_elements = self_ShortStrongArray->payload;
      uint16_t array_offset = 0;
      for(int i = 0; i<=index; i++){
        if(array_offset>totalSize-4){
          RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
        }
        char* element_size = malloc(element_creator->sizeIdentifierSize);
        memcpy(element_size, &array_elements[array_offset], element_creator->sizeIdentifierSize);
        uint16_t int_size = to_uint16((unsigned char*)element_size);
        free(element_size);
        if(i == index){
          char* return_object_payload = malloc(int_size);
          memcpy(return_object_payload, &array_elements[array_offset], int_size);
          XYResult* return_result = newObject(self_ShortStrongArray->id, return_object_payload);
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
  RETURN_ERROR(ERR_BADDATA);
}

/*----------------------------------------------------------------------------*
*  NAME
*      ShortStrongArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ShortStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* ShortStrongArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  ShortStrongArray* ShortStrongArrayObject = malloc(sizeof(ShortStrongArray));
  if(ShortStrongArrayObject == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  char ShortStrongArrayID[2];
  memcpy(ShortStrongArrayID, id, 2);
  XYResult* newObject_result = newObject(ShortStrongArrayID, ShortStrongArrayObject);
  if(newObject_result->error == OK && ShortStrongArrayObject != NULL){
    ShortStrongArrayObject->id[0] = id[0];
    ShortStrongArrayObject->id[1] = id[1];
    ShortStrongArrayObject->size = 4;
    ShortStrongArrayObject->add = &ShortStrongArray_add;
    ShortStrongArrayObject->get = &ShortStrongArray_get;
    ShortStrongArrayObject->payload = NULL;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = OK;
      XYObject* return_object = newObject_result->result;
      free(newObject_result);
      return_result->result = return_object;
      return return_result;
    }
    else {
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = NULL;
      return preallocated_result;
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      ShortStrongArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ShortStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* ShortStrongArray_creator_fromBytes(char* data){

  XYResult* return_result = malloc(sizeof(XYResult));
  ShortStrongArray* return_array = malloc(sizeof(ShortStrongArray));
  if(return_result && return_array){
      return_array->add = &ShortStrongArray_add;
      return_array->remove = NULL;
      return_array->get = &ShortStrongArray_get;
      return_array->size = to_uint16((unsigned char*)data);
      char array_id[3];
      array_id[0] = data[2];
      array_id[1] = data[3];
      array_id[2] = '\00';
      strcpy(return_array->id, array_id);
      return_array->payload = malloc(sizeof(char)*(return_array->size-4));
      if(return_array->payload != NULL){
        memcpy(return_array->payload, &data[4], (return_array->size-4));
        return_result->error = OK;
        return_result->result = return_array;
        return return_result;
      }
      else
      {
        if(return_result) free(return_result);
        if(return_array) free(return_array);
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
  }
  else{
    if(return_result) free(return_result);
    if(return_array) free(return_array);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      ShortStrongArray_creator_create
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
XYResult* ShortStrongArray_creator_toBytes(struct XYObject* user_XYObject){
  if((user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x02) || (user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x03)){
    ShortStrongArray* user_array = user_XYObject->GetPayload(user_XYObject);
    uint16_t totalSize = user_array->size;
    char* byteBuffer = malloc(sizeof(char)*totalSize);
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL && byteBuffer != NULL){

      /*
       * Use the to_uint32 function to converter endian to Big Endian
       * if the host architecture isn't already Big Endian.
       * This switch happens so that when it's copied into a buffer we
       * are in the network byte order.
       */
      if(littleEndian()){
        user_array->size = to_uint16((unsigned char*)(uintptr_t)&user_array->size);
      }
      memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 4);
      memcpy(byteBuffer+4, user_array->payload, sizeof(char)*(totalSize-4));
      if(littleEndian()){
        user_array->size = to_uint16((unsigned char*)(uintptr_t)&user_array->size);
      }
      return_result->error = OK;
      return_result->result = byteBuffer;
      return return_result;
    }
    else {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA)
  }

}
