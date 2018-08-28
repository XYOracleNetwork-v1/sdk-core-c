#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "serializer.h"
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <stdio.h>


/*----------------------------------------------------------------------------*
*  NAME
*      IntWeakArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied IntWeakArray
*
*  PARAMETERS
*     *self_IntWeakArray  [in]       XYObject*
*     *user_XYObject          [in]      IntWeakArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* IntWeakArray_add(IntWeakArray* self_IntWeakArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject
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
      newSize = (self_IntWeakArray->size + object_size);
    }
    else if(user_object_creator->sizeIdentifierSize != 0){

      // If each object is independantly sized, we need to figure out how many
      // bytes need to be read in order to interpret size, hence
      // sizeIdentifierSize variable.
      int object_sizeIdentiferSize = user_object_creator->sizeIdentifierSize;

      // Get a pointer to beginning of the array to read the size.
      char* object_payload = self_IntWeakArray->payload;

      // A 32 bit size will be the first element in the payload.
      object_size = to_uint32(object_payload);
      newSize = (self_IntWeakArray->size + object_size);
    }
    breakpoint();
    // Total Size should not exceed the size mandated by the type (Integer)
    if(newSize < 16777216U){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_IntWeakArray->payload != NULL){
        self_IntWeakArray->payload = realloc(self_IntWeakArray->payload, newSize);
      }
      else {
        self_IntWeakArray->payload = malloc(newSize*sizeof(char));
      }

      if(self_IntWeakArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.
        char* object_payload = self_IntWeakArray->payload;
        object_payload = &(object_payload[self_IntWeakArray->size - (sizeof(char)*4)]);

        // Finally copy the element into the array
        memcpy(object_payload, user_XYObject->payload, object_size);
        self_IntWeakArray->size = newSize;
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
*      IntWeakArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied IntWeakArray at a supplied index.
*
*  PARAMETERS
*     *self_IntWeakArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* IntWeakArray_get(IntWeakArray* self_IntWeakArray, int index) {
  /*
  XYResult* general_result = lookup(self_IntWeakArray->id);
  if(general_result->error == OK){
    Object_Creator* element_creator = general_result->result;
    if(element_creator->defaultSize != 0){
      uint8_t totalSize = self_IntWeakArray->size;
      totalSize = totalSize - 4*sizeof(char);
      if((totalSize % element_creator->defaultSize) == 0){
        char* array_elements = self_IntWeakArray->payload;
        return newObject(self_IntWeakArray->id, &array_elements[element_creator->defaultSize*index]);
      }
      else {
        RETURN_ERROR(ERR_BADDATA);
      }
    }
    else if(element_creator->sizeIdentifierSize != 0)
    {
      uint32_t totalSize = self_IntWeakArray->size;
      char* array_elements = self_IntWeakArray->payload;
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
          XYResult* return_result = newObject(self_IntWeakArray->id, return_object_payload);
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
    RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
  }
  */
}

/*----------------------------------------------------------------------------*
*  NAME
*      IntWeakArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the IntWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* IntWeakArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  /*
  IntWeakArray* IntWeakArrayObject = malloc(sizeof(IntWeakArray));
  char IntWeakArrayID[2] = {0x01, 0x06};
  XYResult* newObject_result = newObject(IntWeakArrayID, IntWeakArrayObject);
  if(newObject_result->error == OK && IntWeakArrayObject != NULL){
    IntWeakArrayObject->id[0] = id[0];
    IntWeakArrayObject->id[1] = id[1];
    IntWeakArrayObject->size = 4;
    IntWeakArrayObject->add = &IntWeakArray_add;
    IntWeakArrayObject->get = &IntWeakArray_get;
    IntWeakArrayObject->payload = NULL;
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
  */
}

/*----------------------------------------------------------------------------*
*  NAME
*      IntWeakArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the IntWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* IntWeakArray_creator_fromBytes(char* data){

  XYResult* return_result = malloc(sizeof(XYResult));
  IntWeakArray* return_array = malloc(sizeof(IntWeakArray));
  if(return_result && return_array){
      return_array->add = &IntWeakArray_add;
      return_array->remove = NULL;
      return_array->get = &IntWeakArray_get;
      return_array->size = to_uint32(data);
      return_array->payload = malloc(sizeof(char)*(return_array->size-4));
      if(return_array->payload != NULL){
        memcpy(return_array->payload, &data[4], (return_array->size-4));
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
*      IntWeakArray_creator_create
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
XYResult* IntWeakArray_creator_toBytes(struct XYObject* user_XYObect){
  if(user_XYObect->id[0] == 0x01 && user_XYObect->id[1] == 0x06){
    IntWeakArray* IntWeakArrayObject = malloc(sizeof(IntWeakArray));
    if(IntWeakArrayObject != NULL){
      IntWeakArray* user_array = user_XYObect->GetPayload(user_XYObect);
      uint8_t totalSize = user_array->size;
      char* byteBuffer = malloc(sizeof(char)*totalSize);
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result != NULL && byteBuffer != NULL){
        memcpy(byteBuffer, user_XYObect->GetPayload(user_XYObect), 4);
        memcpy(byteBuffer+4, user_array->payload, sizeof(char)*(totalSize-4));
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
