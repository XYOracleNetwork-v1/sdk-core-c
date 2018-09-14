#include "xyo.h"
#include "XYOHeuristicsBuilder.h"


/*----------------------------------------------------------------------------*
*  NAME
*      ShortWeakArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied ShortWeakArray
*
*  PARAMETERS
*     *self_ShortWeakArray  [in]       XYObject*
*     *user_XYObject          [in]      ShortWeakArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ShortWeakArray_add(ShortWeakArray* self_ShortWeakArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject
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
      newSize = (self_ShortWeakArray->size + object_size + (sizeof(char)*2));
    }
    else if(user_object_creator->sizeIdentifierSize != 0){


      // Get a pointer to beginning of the user object payload to read size.
      char* user_object_payload = user_XYObject->payload;


      switch(user_object_creator->sizeIdentifierSize){
        case 1:
          object_size = user_object_payload[0];
          break;
        case 2:
          /* First we read 2 bytes of the payload to get the size,
           * the to_uint16 function reads ints in big endian.
           */
          object_size = to_uint16(user_object_payload); //TODO: Check compatibility on big endian devices.
          if(littleEndian()){
            object_size = to_uint16((char*)&object_size);
          }
          break;
        case 4:
          object_size = to_uint32(user_object_payload);
          if(littleEndian()){
            object_size = to_uint32((char*)&object_size);
          }
          break;
      }
      newSize = (self_ShortWeakArray->size + object_size + (sizeof(char)*2));
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
         Object_Creator* deeper_object_creator = lookup_result->result;
         if(deeper_object_creator->defaultSize != 0){

           // defaultSize + 2 Bytes representing ID
           object_size = deeper_object_creator->defaultSize + (sizeof(char)*2);

           newSize = (self_ShortWeakArray->size + object_size + (sizeof(char)*2));
         }
         else if(deeper_object_creator->sizeIdentifierSize != 0){
           /* Unimplemented */
         }
       }
    }
    // Total Size should not exceed the size mandated by the type (Integer)
    if(newSize < 16777216U){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_ShortWeakArray->payload != NULL){
        self_ShortWeakArray->payload = realloc(self_ShortWeakArray->payload, newSize-(sizeof(char)*2));
      }
      else {
        self_ShortWeakArray->payload = malloc(newSize-(sizeof(char)*2));
      }

      if(self_ShortWeakArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.
        char* object_payload = self_ShortWeakArray->payload;
        object_payload = &(object_payload[self_ShortWeakArray->size - (sizeof(char)*2)]);

        // Finally copy the element into the array
        memcpy(object_payload, user_XYObject->id, 2);
        XYResult* toBytes_result = user_object_creator->toBytes(user_XYObject);
        memcpy(object_payload+2, toBytes_result->result, object_size);

        self_ShortWeakArray->size = newSize;
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
*      ShortWeakArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied ShortWeakArray at a supplied index.
*
*  PARAMETERS
*     *self_ShortWeakArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns pointer to element in array.
*----------------------------------------------------------------------------*/
XYResult* ShortWeakArray_get(ShortWeakArray* self_ShortWeakArray, int index) {
  /* Here the program will iterate through each element. Getting each
   * element's size and iterating to the next element unless we are
   * at the user's specified index. Get pointer to start of array
   * and bounds check each loop.
   */
  int internal_index = 0;
  for(char* arrayPointer = self_ShortWeakArray->payload; (ShortWeakArray **)arrayPointer < (&self_ShortWeakArray+(sizeof(char)*self_ShortWeakArray->size));){
    XYResult* lookup_result = lookup(arrayPointer);
    if(lookup_result->error == OK){
      Object_Creator* element_creator = lookup_result->result;

      uint32_t element_size = 0;
      if(element_creator->defaultSize != 0 ){
        element_size = element_creator->defaultSize + (2 * sizeof(char));
      }
      else if(element_creator->sizeIdentifierSize != 0)
      {
        switch(element_creator->sizeIdentifierSize){
          case 1:
            element_size = arrayPointer[2];
            break;
          case 2:
            element_size = to_uint16(arrayPointer+(sizeof(char)*2));
            break;
          case 4:
            element_size = to_uint32(arrayPointer+(sizeof(char)*2));
            break;
        }

      }
      if(internal_index == index) {
        XYResult* return_result = malloc(sizeof(XYResult));
        XYResult* new_result = newObject(arrayPointer, arrayPointer+2);

        if(return_result && new_result->error == OK){
          return_result->error = OK;
          return_result->result = new_result->result;
          return return_result;
        }
        else {
          RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
        }
      }
      // Skip to next element
      else {
        arrayPointer = arrayPointer+(sizeof(char)*(element_size+2));
        if((ShortWeakArray **)arrayPointer > (&self_ShortWeakArray+(sizeof(char)*self_ShortWeakArray->size))){
          RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
        }
      }
      internal_index = internal_index + 1;
    }
    else
    {
      RETURN_ERROR(ERR_BADDATA);
    }
  }
  RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
}

/*----------------------------------------------------------------------------*
*  NAME
*      ShortWeakArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ShortWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* ShortWeakArray_creator_create(char id[2], void* user_data){
  ShortWeakArray* ShortWeakArrayObject = malloc(sizeof(ShortWeakArray));
  char ShortWeakArrayID[2] = {0x01, 0x05};
  XYResult* newObject_result = newObject(ShortWeakArrayID, ShortWeakArrayObject);
  if(newObject_result->error == OK && ShortWeakArrayObject != NULL){
    ShortWeakArrayObject->size = 2;
    ShortWeakArrayObject->add = &ShortWeakArray_add;
    ShortWeakArrayObject->get = &ShortWeakArray_get;
    ShortWeakArrayObject->payload = NULL;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = OK;
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
*      ShortWeakArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the ShortWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* ShortWeakArray_creator_fromBytes(char* data){

  XYResult* return_result = malloc(sizeof(XYResult));
  ShortWeakArray* return_array = malloc(sizeof(ShortWeakArray));
  if(return_result && return_array){
      return_array->add = &ShortWeakArray_add;
      return_array->remove = NULL;
      return_array->get = &ShortWeakArray_get;
      return_array->size = to_uint16(data);
      return_array->payload = malloc(sizeof(char)*(return_array->size-2));
      if(return_array->payload != NULL){
        memcpy(return_array->payload, &data[2], (return_array->size-2));
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
*      ShortWeakArray_creator_toBytes
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
XYResult* ShortWeakArray_creator_toBytes(struct XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x05){
    ShortWeakArray* user_array = user_XYObject->GetPayload(user_XYObject);
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
        user_array->size = to_uint16((char*)(uintptr_t)user_array->size);
      }

      memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 2);
      memcpy(byteBuffer+2, user_array->payload, sizeof(char)*(totalSize-2));
      if(littleEndian()){
        user_array->size = to_uint16((char*)(uintptr_t)user_array->size);
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
