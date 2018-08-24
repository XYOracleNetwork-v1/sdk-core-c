#include <stdlib.h>
#include <string.h>
#include "serializer.h"
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"


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
  if(lookup_result->error != OK){
    struct Object_Creator* user_object_creator = lookup_result->result;

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
      self_ByteStrongArray->payload = realloc(self_ByteStrongArray->payload, newSize);
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
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result != NULL){
          return_result->error = ERR_INSUFFICIENT_MEMORY;
          return_result->result = 0;
          return return_result;
        }
        else {
          preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
          preallocated_result->result = 0;
          return preallocated_result;
        }
      }

    }
    else {
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result != NULL){
        return_result->error = ERR_BADDATA;
        return_result->result = 0;
        return return_result;
      }
      else {
        preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
        preallocated_result->result = 0;
        return preallocated_result;
      }
    }
  }
    else {
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result != NULL){
        return_result->error = ERR_KEY_DOES_NOT_EXIST;
        return_result->result = 0;
        return return_result;
      }
      else {
        preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
        preallocated_result->result = 0;
        return preallocated_result;
      }
    }
}

XYResult* ByteStrongArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  ByteStrongArray* ByteStrongArrayObject = malloc(sizeof(ByteStrongArray));
  XYResult* newObject_result = newObject(id, ByteStrongArrayObject);
  if(newObject_result->error == OK && ByteStrongArrayObject != NULL){
    ByteStrongArrayObject->id[0] = id[0];
    ByteStrongArrayObject->id[1] = id[1];
    ByteStrongArrayObject->size = sizeof(ByteStrongArray);
    ByteStrongArrayObject->payload = 0;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = 0;
      return_result->result = &ByteStrongArrayObject;
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

/* todo */
XYResult* ByteStrongArray_creator_fromBytes(char* data){
  ByteStrongArray* ByteStrongArrayObject = malloc(sizeof(ByteStrongArray));
  char id[2];
  id[0] = data[0];
  id[1] = data[1];
  ByteStrongArrayObject->id[0] = data[0];
  ByteStrongArrayObject->id[1] = data[1];
  XYResult* newObject_result = newObject(id, ByteStrongArrayObject);
  ByteStrongArrayObject->size =  data[2];
  ByteStrongArrayObject->add = &ByteStrongArray_add;
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result != NULL){
    return_result->error = 0;
    return_result->result = &ByteStrongArrayObject;
    return return_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }


}
