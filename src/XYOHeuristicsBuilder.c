/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: XYOHeuristicsBuilder.c
 * @Last modified by:   Nate Brune
 * @Last modified time: 09-Aug-2018
 * @Copyright: XY - The Findables Company
 */
 #include "xyobject.h"
 #include <stdlib.h>
 #include <string.h>
 #include "serializer.h"
 #include "xyo.h"
 #include "XYOHeuristicsBuilder.h"
 #include <stdio.h>

 uint32_t to_uint32(char* data) {
   return data[0] + 256U*data[1] + 65536U*data[2] + 16777216U*data[3];
 }

 uint16_t to_uint16(char* data) {
   return data[0] + 256U*data[1];
 }


XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi){
  return newObject(id, rssi);

}

XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[3];
  return newObject(id, &rssi);
}

XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObect){
  char encoded_bytes[3];
  encoded_bytes[0] = 0x09;
  encoded_bytes[1] = 0x01;
  char* rssi = (char*)user_XYObect->payload;
  encoded_bytes[2] = rssi[2];
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

/* todo */
void breakpoint(){}

XYResult* ByteStrongArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  ByteStrongArray* ByteStrongArrayObject = malloc(sizeof(ByteStrongArray));
  char byteStrongArrayID[2] = {0x01, 0x01};
  XYResult* newObject_result = newObject(byteStrongArrayID, ByteStrongArrayObject);
  if(newObject_result->error == OK && ByteStrongArrayObject != NULL){
    ByteStrongArrayObject->id[0] = id[0];
    ByteStrongArrayObject->id[1] = id[1];
    ByteStrongArrayObject->size = sizeof(ByteStrongArray);
    ByteStrongArrayObject->payload = 0;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = 0;
      XYObject* return_object = newObject_result->result;
      return_result->result = return_object;
      breakpoint();
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
  char id[2];
  id[0] = data[0];
  id[1] = data[1];
  uint8_t objectSize =  data[3];
  XYResult* creator_result = lookup(id);
  if(creator_result->error == OK){
    Object_Creator* byteStrongArrayCreator = creator_result->result;
    id[0] = data[3];
    id[1] = data[4];
    creator_result = lookup(id);
    if(creator_result->error == OK){
      Object_Creator* elementCreator = creator_result->result;
      XYObject* arrayElement = malloc(sizeof(XYObject));
      if(arrayElement != NULL){
        breakpoint();
        creator_result = byteStrongArrayCreator->create(id, "unused");
        if(creator_result->error == OK){
            XYObject* byteStrongArrayObject = creator_result->result;
            ByteStrongArray* byteStrongArrayRaw = byteStrongArrayObject->payload;
            byteStrongArrayRaw->id[0] = data[4];
            byteStrongArrayRaw->id[1] = data[5];
            arrayElement->id[0] = data[3];
            arrayElement->id[1] = data[4];
            if(elementCreator->defaultSize != 0){
              if(elementCreator->defaultSize == 1){
                arrayElement->payload = &data[6];
              }
            }
            byteStrongArrayRaw->add(byteStrongArrayRaw, arrayElement);
            XYResult* return_result = malloc(sizeof(XYResult));
            return_result->error = OK;
            return_result->result = byteStrongArrayObject;
        }
      }
    }
  }
  else {
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
        memcpy(byteBuffer, user_XYObect->GetPayload(user_XYObect), totalSize);
        return_result->error = OK;
        return_result->result = &byteBuffer;
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
