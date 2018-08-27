#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "serializer.h"
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <stdio.h>

/* todo */
void breakpoint(){}

uint32_t to_uint32(char* data) {
  return data[0] + 256U*data[1] + 65536U*data[2] + 16777216U*data[3];
}

uint16_t to_uint16(char* data) {
  return data[0] + 256U*data[1];
}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteStrongArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied ByteStrongArray
*
*  PARAMETERS
     *self_ByteStrongArray  [in]       XYObject*
     *user_XYObject          [in]      ByteStrongArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_add(ByteStrongArray* self_ByteStrongArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject

  // Lookup the Object_Creator for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.
  XYResult* lookup_result = lookup(user_XYObject->id);
  if(lookup_result->error == OK){
    Object_Creator* user_object_creator = lookup_result->result;

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint16_t newSize = 0;
    uint8_t object_size = 0;

    if(user_object_creator->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_object_creator->defaultSize;
      newSize = (self_ByteStrongArray->size + object_size);
    }
    else if(user_object_creator->sizeIdentifierSize != 0){

      // If each object is independantly sized, we need to figure out how many
      // bytes need to be read in order to interpret size, hence
      // sizeIdentifierSize variable.
      int object_sizeIdentiferSize = user_object_creator->sizeIdentifierSize;

      // Get a pointer to beginning of the array to read the size.
      char* object_payload = self_ByteStrongArray->payload;

      // Since the total size of the array is less than 255 bytes, the
      // size identifier size for one element must be 1 byte.
      // the Author doesn't bother checking sizeIdentifierSize
      // even though typically we would.
      object_size = object_payload[0];
      newSize = (self_ByteStrongArray->size + object_size);
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
        memcpy(object_payload, user_XYObject->payload, object_size);
        self_ByteStrongArray->size = newSize;
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
*      ByteStrongArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied ByteStrongArray at a supplied index.
*
*  PARAMETERS
     *self_ByteStrongArray  [in]       XYObject*
     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ByteStrongArray_get(ByteStrongArray* self_ByteStrongArray, int index) {
  XYResult* general_result = lookup(self_ByteStrongArray->id);
  if(general_result->error == OK){
    Object_Creator* element_creator = general_result->result;
    if(element_creator->defaultSize != 0){
      uint8_t totalSize = self_ByteStrongArray->size;
      totalSize = totalSize - 3*sizeof(char);
      if((totalSize % element_creator->defaultSize) == 0){
        char* array_elements = self_ByteStrongArray->payload;
        breakpoint();
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
        memcpy(element_size, &array_elements[array_offset], 2);
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
    RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
  }

}

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

XYResult* ByteStrongArray_creator_toBytes(struct XYObject* user_XYObect){
  if(user_XYObect->id[0] == 0x01 && user_XYObect->id[1] == 0x01){
    ByteStrongArray* ByteStrongArrayObject = malloc(sizeof(ByteStrongArray));
    if(ByteStrongArrayObject != NULL){
      ByteStrongArray* user_array = user_XYObect->GetPayload(user_XYObect);
      uint8_t totalSize = user_array->size;
      char* byteBuffer = malloc(sizeof(char)*totalSize);
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result != NULL && byteBuffer != NULL){
        memcpy(byteBuffer, user_XYObect->GetPayload(user_XYObect), 3);
        memcpy(byteBuffer+3, user_array->payload, sizeof(char)*(totalSize-3));
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
