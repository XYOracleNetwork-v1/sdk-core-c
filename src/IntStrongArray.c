/**
 ****************************************************************************************
 *
 * @file intstrongarray.c
 *
 * @XYO Core library source code.
 *
 * @brief primary int strong array routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "arrays.h"
/*
#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <stdio.h>
*/

extern uint16_t to_uint16(unsigned char* data);
extern uint32_t to_uint32(unsigned char* data);

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied IntStrongArray
*
*  PARAMETERS
*     *self_IntStrongArray  [in]       XYObject_t*
*     *user_XYObject          [in]      IntStrongArray_t*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult_t* IntStrongArray_add(IntStrongArray_t* self_IntStrongArray,
                               XYObject_t* user_XYObject){
                                                        //TODO: consider changing self to XYObject
  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!self_IntStrongArray || !user_XYObject) {RETURN_ERROR(ERR_BADDATA);}
  // Lookup the ObjectProvider for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.

  XYResult_t* lookup_result = tableLookup(user_XYObject->id);

  if(lookup_result->error == OK){

    ObjectProvider_t* user_ObjectProvider = lookup_result->result;

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint32_t newSize = 0;
    uint32_t object_size = 0;

    if(user_ObjectProvider->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_ObjectProvider->defaultSize;
      newSize = (self_IntStrongArray->size + object_size);
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
      newSize = (self_IntStrongArray->size + object_size * sizeof(char));
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
       lookup_result = tableLookup(id);
       if(lookup_result->error == OK){
         ObjectProvider_t* deeper_ObjectProvider = lookup_result->result;
         if(deeper_ObjectProvider->defaultSize != 0){

           object_size = deeper_ObjectProvider->defaultSize;

           newSize = (self_IntStrongArray->size + object_size);
         }
         else if(deeper_ObjectProvider->sizeIdentifierSize != 0){
           /* Unimplemented */
         }
       }
    }
    // Total Size should not exceed the size mandated by the type (Integer)
    if(newSize < 16777216U){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_IntStrongArray->payload != NULL){
        self_IntStrongArray->payload = realloc(self_IntStrongArray->payload, newSize-(sizeof(char)*6));
      }
      else {
        self_IntStrongArray->payload = malloc(newSize-(sizeof(char)*6));
      }

      if(self_IntStrongArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.

        char* object_payload = self_IntStrongArray->payload;
        object_payload = &(object_payload[self_IntStrongArray->size - (sizeof(char)*6)]);

        // Finally copy the element into the array
        if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x03){
          unsigned char* payload = user_XYObject->payload;
          uint32_t size = to_uint32(payload);
          memcpy(payload, &size, 4);
        }
        XYResult_t* toBytes_result = user_ObjectProvider->toBytes(user_XYObject);
        if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x03){
          unsigned char* payload = user_XYObject->payload;
          uint32_t size = to_uint32(payload);
          memcpy(payload, &size, 4);
        }
        memcpy(object_payload, toBytes_result->result, object_size);

        self_IntStrongArray->size = newSize;

        preallocated_return_result_ptr = &preallocated_return_result;

        preallocated_return_result_ptr->error = OK;
        preallocated_return_result_ptr->result = 0;

        return preallocated_return_result_ptr;
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
    RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied IntStrongArray at a supplied index.
*
*  PARAMETERS
*     *self_IntStrongArray  [in]       XYObject_t*
*     *index                [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult_t* IntStrongArray_get(IntStrongArray_t* self_IntStrongArray, int index) {

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!self_IntStrongArray) {RETURN_ERROR(ERR_BADDATA);}

  XYResult_t* general_result = tableLookup(self_IntStrongArray->id);

  if(general_result->error == OK){

    ObjectProvider_t* element_creator = general_result->result;

    if(element_creator->defaultSize != 0){

      uint8_t totalSize = self_IntStrongArray->size;
      totalSize = totalSize - 6*sizeof(char);
      if((totalSize % element_creator->defaultSize) == 0){
        char* array_elements = self_IntStrongArray->payload;
        return newObject(self_IntStrongArray->id, &array_elements[element_creator->defaultSize*index]);
      }
      else {
        RETURN_ERROR(ERR_BADDATA);
      }
    }
    else if(element_creator->sizeIdentifierSize != 0)
    {
      uint32_t totalSize = self_IntStrongArray->size;
      char* array_elements = self_IntStrongArray->payload;
      uint32_t array_offset = 0;
      for(int i = 0; i<=index; i++){
        if(array_offset>totalSize){
          RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
        }
        char* element_size = malloc(element_creator->sizeIdentifierSize);
        //TODO: wal, should check for any malloc errors

        memcpy(element_size, &array_elements[array_offset], element_creator->sizeIdentifierSize);
        uint32_t int_size = to_uint32((unsigned char*)element_size);
        free(element_size);
        if(i == index){
          char* return_object_payload = malloc(int_size);
          //TODO: wal, should check for any malloc errors

          memcpy(return_object_payload, &array_elements[array_offset], int_size);

          preallocated_return_result_ptr = newObject(self_IntStrongArray->id, return_object_payload);

          return preallocated_return_result_ptr;
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
  RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST)
}

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)    [in]       char*
*     *user_data              [in]       void*
*
*  RETURNS
*      XYResult_t*            [out]      bool   Returns XYObject_t* of the IntStrongArray type.
*----------------------------------------------------------------------------*/
XYResult_t* IntStrongArray_creator_create(const char id[2], void* user_data){
                                  // consider allowing someone to create an array with one object

  IntStrongArray_t* IntStrongArrayObject = malloc(sizeof(IntStrongArray_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!IntStrongArrayObject) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  char IntStrongArrayID[2] = {0x01, 0x03};
  XYResult_t* newObject_result = newObject(IntStrongArrayID, IntStrongArrayObject);

  preallocated_return_result_ptr = &preallocated_return_result;

  if(newObject_result->error == OK && IntStrongArrayObject != NULL){
    IntStrongArrayObject->id[0] = id[0];
    IntStrongArrayObject->id[1] = id[1];
    IntStrongArrayObject->size = 6;
    IntStrongArrayObject->add = &IntStrongArray_add;
    IntStrongArrayObject->get = &IntStrongArray_get;
    IntStrongArrayObject->payload = NULL;

    preallocated_return_result_ptr->error = OK;
    preallocated_return_result_ptr->result = newObject_result->result;
    return preallocated_return_result_ptr;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;
    return preallocated_return_result_ptr;
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*     IntStrongArray_creator_fromBytes
*
*  DESCRIPTION
*     Create an Strong Byte Array given a set of Bytes. Bytes must not include
*     major and minor of array.
*
*  PARAMETERS
*     *data             [in]       char*
*
*  RETURNS
*      XYResult_t*      [out]      bool   Returns XYObject_t* of the IntStrongArray type.
*----------------------------------------------------------------------------*/
XYResult_t* IntStrongArray_creator_fromBytes(char* data){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!data) {RETURN_ERROR(ERR_BADDATA);}

  preallocated_return_result_ptr = &preallocated_return_result;

  IntStrongArray_t* return_array = malloc(sizeof(IntStrongArray_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(return_array){

      return_array->add = &IntStrongArray_add;
      return_array->remove = NULL;
      return_array->get = &IntStrongArray_get;
      return_array->size = to_uint32((unsigned char*)data);
      char array_id[3];
      array_id[0] = data[4];
      array_id[1] = data[5];
      array_id[2] = '\00';
      strcpy(return_array->id, array_id);
      return_array->id[0] = data[4];
      return_array->id[1] = data[5];
      int flipped = 0;
      if(return_array->size > CAUSES_BUGS){ /* return array size is big endian) */
        return_array->size = to_uint32((unsigned char*)&return_array->size);
        flipped = 1;
      }
      return_array->payload = malloc(sizeof(char)*(return_array->size-6));

      /********************************/
      /* guard against malloc errors  */
      /********************************/

      if(return_array->payload != NULL){

        memcpy(return_array->payload, &data[6], (return_array->size-6));
        if(flipped == 1){
          return_array->size = to_uint32((unsigned char*)&return_array->size);
        }
        preallocated_return_result_ptr->error = OK;
        preallocated_return_result_ptr->result = return_array;

        return preallocated_return_result_ptr;
      }
      else
      {
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
  }
  else{
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_creator_create
*
*  DESCRIPTION
*      Given an XYObject of Byte Strong Array type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject_t*
*
*  RETURNS
*      XYResult_t*          [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult_t* IntStrongArray_creator_toBytes(XYObject_t* user_XYObject){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA);}

  if((user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x03) || (user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x04) || (user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x09)){
    IntStrongArray_t* user_array = (user_XYObject->GetPayload(user_XYObject))->result;
    uint32_t totalSize = user_array->size;
    char* byteBuffer = malloc(sizeof(char)*totalSize);

    preallocated_return_result_ptr = &preallocated_return_result;

    /********************************/
    /* guard against malloc errors  */
    /********************************/

    if(byteBuffer != NULL){

      /*
       * Use the to_uint32 function to converter endian to Big Endian
       * if the host architecture isn't already Big Endian.
       * This switch happens so that when it's copied into a buffer we
       * are in the network byte order.
       */
      if(littleEndian() && user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x04){
        user_array->size = to_uint32((unsigned char*)(uintptr_t)&user_array->size);
      }

      memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 6);
      if((user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x04)){ //|| (user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x03)){
        //uint32_t correctedSize = to_uint32(user_XYObject->GetPayload(user_XYObject));
        //memcpy(byteBuffer, &correctedSize, 4);
        memcpy(byteBuffer+4, (char*)&Payload_id , 2);
      }
      /* TODO: totalSize is rogue here. 10K is unlikely  */
      if(totalSize > CAUSES_BUGS){
        memcpy(byteBuffer, &totalSize, 4);
        totalSize = to_uint32((unsigned char*)&totalSize);
        memcpy(byteBuffer+6, user_array->payload, sizeof(char)*(totalSize-6));
      } else {
        memcpy(byteBuffer+6, user_array->payload, sizeof(char)*(totalSize-6));
      }

      if(littleEndian()){
        user_array->size = to_uint32((unsigned char*)(uintptr_t)user_array->size);
      }
      preallocated_return_result_ptr->error = OK;
      preallocated_return_result_ptr->result = byteBuffer;

      return preallocated_return_result_ptr;

    } else {
      if(byteBuffer) free(byteBuffer);

      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA);
  }
}
/*
typedef struct XYObject{
 char id[2];
 void* payload;
 XYResult_t* (*GetXyobjectId)(struct XYObject*);  // Fetch the above id object and return it.
 XYResult_t* (*GetPayload)(struct XYObject*);     // Fetch the above payload pointer object and return it.
} XYObject_t ;
*/
XYResult_t* BlockSet_creator_toBytes(XYObject_t* user_XYObject){
  
  if((user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x09)){
    
    IntStrongArray_t* user_array = (user_XYObject->GetPayload(user_XYObject))->result;  // wal, compiler warning fix
    uint32_t totalSize = user_array->size;
    char* byteBuffer = malloc(sizeof(char)*totalSize);
    XYResult_t* return_result = malloc(sizeof(XYResult_t));
    if(return_result != NULL && byteBuffer != NULL){

      /*
       * Use the to_uint32 function to converter endian to Big Endian
       * if the host architecture isn't already Big Endian.
       * This switch happens so that when it's copied into a buffer we
       * are in the network byte order.
       */

      uint32_t encodedSize = totalSize;
      if(littleEndian()){
        encodedSize = to_uint32((unsigned char*)&encodedSize);
      }
      memcpy(byteBuffer, &encodedSize, 4);
      char* bodyPtr = (char*)byteBuffer+4;
      char* userBodyPtr = (char*)user_XYObject->payload+4;
      memcpy(bodyPtr, userBodyPtr, 2);

      memcpy(byteBuffer+6, user_array->payload, sizeof(char)*(totalSize-6));
      return_result->error = OK;
      return_result->result = byteBuffer;
      return return_result;
    } else {
      if(byteBuffer) free(byteBuffer);
      if(return_result) free(return_result);
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA)
  }
}

// end of file intstrongarray.c
