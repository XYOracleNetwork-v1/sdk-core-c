#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <stdio.h>
#include <arpa/inet.h>

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied IntStrongArray
*
*  PARAMETERS
*     *self_IntStrongArray  [in]       XYObject*
*     *user_XYObject          [in]      IntStrongArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* IntStrongArray_add(IntStrongArray* self_IntStrongArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject
  // Lookup the Object_Creator for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.
  XYResult* lookup_result = lookup(user_XYObject->id);
  if(lookup_result->error == OK){
    Object_Creator* user_object_creator = lookup_result->result;

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint32_t newSize = 0;
    uint32_t object_size = 0;

    if(user_object_creator->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_object_creator->defaultSize;
      newSize = (self_IntStrongArray->size + object_size);
    }
    else if(user_object_creator->sizeIdentifierSize != 0){

      // Get a pointer to beginning of the array to read the size.
      char* object_payload = self_IntStrongArray->payload;

      // Size identifier Size tells you how many bytes to read for size
      switch(user_object_creator->sizeIdentifierSize){
        case 1:
          object_size = object_payload[0];
          break;
        case 2:
          object_size = ntohs(to_uint16(object_payload));
          break;
        case 4:
          object_size = ntohs(to_uint32(object_payload));
          break;
      }

      newSize = (self_IntStrongArray->size + object_size)*sizeof(char);
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
        XYResult* toBytes_result = user_object_creator->toBytes(user_XYObject);
        memcpy(object_payload, toBytes_result->result, object_size);

        self_IntStrongArray->size = newSize;
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
*     *self_IntStrongArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* IntStrongArray_get(IntStrongArray* self_IntStrongArray, int index) {
  XYResult* general_result = lookup(self_IntStrongArray->id);
  if(general_result->error == OK){
    Object_Creator* element_creator = general_result->result;
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
        memcpy(element_size, &array_elements[array_offset], element_creator->sizeIdentifierSize);
        uint32_t int_size = to_uint32(element_size);
        free(element_size);
        if(i == index){
          char* return_object_payload = malloc(int_size);
          memcpy(return_object_payload, &array_elements[array_offset], int_size);
          XYResult* return_result = newObject(self_IntStrongArray->id, return_object_payload);
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
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the IntStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* IntStrongArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  IntStrongArray* IntStrongArrayObject = malloc(sizeof(IntStrongArray));
  char IntStrongArrayID[2] = {0x01, 0x03};
  XYResult* newObject_result = newObject(IntStrongArrayID, IntStrongArrayObject);
  if(newObject_result->error == OK && IntStrongArrayObject != NULL){
    IntStrongArrayObject->id[0] = id[0];
    IntStrongArrayObject->id[1] = id[1];
    IntStrongArrayObject->size = 6;
    IntStrongArrayObject->add = &IntStrongArray_add;
    IntStrongArrayObject->get = &IntStrongArray_get;
    IntStrongArrayObject->payload = NULL;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = 0;
      XYObject* return_object = newObject_result->result;
      return_result->result = return_object;
      return return_result;
    }
    else {
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = 0;
      return preallocated_result;
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      IntStrongArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the IntStrongArray type.
*----------------------------------------------------------------------------*/
XYResult* IntStrongArray_creator_fromBytes(char* data){

  XYResult* return_result = malloc(sizeof(XYResult));
  IntStrongArray* return_array = malloc(sizeof(IntStrongArray));
  if(return_result && return_array){
      return_array->add = &IntStrongArray_add;
      return_array->remove = NULL;
      return_array->get = &IntStrongArray_get;
      return_array->size = to_uint32(data);
      char array_id[3];
      array_id[0] = data[4];
      array_id[1] = data[5];
      array_id[2] = '\00';
      strcpy(return_array->id, array_id);
      return_array->payload = malloc(sizeof(char)*(return_array->size-6));
      if(return_array->payload != NULL){
        memcpy(return_array->payload, &data[6], (return_array->size-6));
        return_result->error = OK;
        return_result->result = return_array;
        return return_result;
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
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* IntStrongArray_creator_toBytes(struct XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x03){
    IntStrongArray* IntStrongArrayObject = malloc(sizeof(IntStrongArray));
    if(IntStrongArrayObject != NULL){
      IntStrongArray* user_array = user_XYObject->GetPayload(user_XYObject);
      uint8_t totalSize = user_array->size;
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
          user_array->size = to_uint32((char*)user_array);
        }
        
        memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 6);
        memcpy(byteBuffer+6, user_array->payload, sizeof(char)*(totalSize-6));
        if(littleEndian()){
          user_array->size = ntohl(user_array->size);
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
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = 0;
      return preallocated_result;
    }
  }
  else {
    RETURN_ERROR(ERR_BADDATA)
  }

}
