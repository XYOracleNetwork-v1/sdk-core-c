#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

//TODO Each array should have it's own header or one master header

/*----------------------------------------------------------------------------*
*  NAME
*      ByteWeakArray_add
*
*  DESCRIPTION
*      Adds a supplied XYObject to a supplied ByteWeakArray
*
*  PARAMETERS
*     *self_ByteWeakArray  [in]       XYObject*
*     *user_XYObject          [in]      ByteWeakArray*
*
*  RETURNS
*      XYResult  [out]      bool       Returns EXyoErrors::OK if adding succeeded.
*----------------------------------------------------------------------------*/
XYResult* ByteWeakArray_add(ByteWeakArray* self_ByteWeakArray, XYObject* user_XYObject){ //TODO: consider changing self to XYObject
  // Lookup the ObjectProvider for the object so we can infer if the object has a default
  // size or a variable size per each element. We know every element in a single-type array
  // has the same type, but we don't know if they have uniform size. An array of Bound Witness
  // objects will be variable size, but all the same type.
  XYResult* lookup_result = lookup(user_XYObject->id);
  if(lookup_result->error == OK){
    ObjectProvider* user_ObjectProvider = lookup_result->result;

    // First we calculate how much space we need for the payload with
    // the addition of this new element.
    uint32_t newSize = 0;
    uint32_t object_size = 0;

    if(user_ObjectProvider->defaultSize != 0){

      // This object type is always going to have the same size so no additional
      // logic is needed to derrive the new total size of the array.
      object_size = user_ObjectProvider->defaultSize;
      newSize = (self_ByteWeakArray->size + object_size + (sizeof(char)*2));
    }
    else if(user_ObjectProvider->sizeIdentifierSize != 0){

      // Get a pointer to beginning of the user object payload to read size.
      char* user_object_payload = user_XYObject->payload;

      // Get the number of bytes to read to get size.
      switch(user_ObjectProvider->sizeIdentifierSize){
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
          object_size = to_uint32((char*)user_object_payload);
          if(littleEndian()){
            object_size = to_uint32((char*)&object_size);
          }
          break;
      }
      newSize = (self_ByteWeakArray->size + object_size + (sizeof(char)*2));

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

           // defaultSize + 2 Bytes representing ID
           object_size = deeper_ObjectProvider->defaultSize + (sizeof(char)*2);

           newSize = (self_ByteWeakArray->size + object_size + (sizeof(char)*2));
         }
         else if(deeper_ObjectProvider->sizeIdentifierSize != 0){
           /* Unimplemented */
         }
       }
    }
    // Total Size should not exceed the size mandated by the type (Integer)
    if(newSize < 16777216U){

      // Here we are increasing the size of the payload to be able to hold our new element.
      if(self_ByteWeakArray->payload != NULL){
        self_ByteWeakArray->payload = realloc(self_ByteWeakArray->payload, newSize-(sizeof(char)*1));
      }
      else {
        self_ByteWeakArray->payload = malloc(newSize-(sizeof(char)*1));
      }

      if(self_ByteWeakArray->payload != NULL){

        // Get a pointer to the end of the array so we can insert an element there.
        char* object_payload = self_ByteWeakArray->payload;
        object_payload = &(object_payload[self_ByteWeakArray->size - (sizeof(char)*1)]);

        // Finally copy the element into the array
        memcpy(object_payload, user_XYObject->id, 2);
        XYResult* toBytes_result = user_ObjectProvider->toBytes(user_XYObject);
        memcpy(object_payload+2, toBytes_result->result, object_size);

        self_ByteWeakArray->size = newSize;
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result != NULL){
          return_result->error = OK;
          return_result->result = NULL;
          return return_result;
        }
        else {
          preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
          preallocated_result->result = NULL;
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
*      ByteWeakArray_get
*
*  DESCRIPTION
*      Get an XYObject from a supplied ByteWeakArray at a supplied index.
*
*  PARAMETERS
*     *self_ByteWeakArray  [in]       XYObject*
*     *index                 [in]       Int;
*
*  RETURNS
*      XYResult  [out]      bool       Returns pointer to element in array.
*----------------------------------------------------------------------------*/
XYResult* ByteWeakArray_get(ByteWeakArray* self_ByteWeakArray, int index) {
  /* Here the program will iterate through each element. Getting each
   * element's size and iterating to the next element unless we are
   * at the user's specified index. Get pointer to start of array
   * and bounds check each loop.
   */
  int internal_index = 0;
  /*
   * arrayPointer points to the beginning of the element list.
   * arrayPointer is incremented by the size of the object + (2 byte header)
   * to arive at the next element each loop.
   */
  for(char* arrayPointer = self_ByteWeakArray->payload; (ByteWeakArray **)arrayPointer < (&self_ByteWeakArray+(sizeof(char)*self_ByteWeakArray->size));){
    XYResult* lookup_result = lookup(arrayPointer);
    if(lookup_result->error == OK){
      ObjectProvider* element_creator = lookup_result->result;

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
        // Create an XYObject and return it to the user.
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
        /*
         * Same conditional as for loop. Check that arrayPointer is not pointing to
         * a locataion outside the bounds of the array.
         */
        if((ByteWeakArray **)arrayPointer > (&self_ByteWeakArray+(sizeof(char)*self_ByteWeakArray->size))){
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
*      ByteWeakArray_creator_create
*
*  DESCRIPTION
*      Create an empty Strong Byte Array
*
*  PARAMETERS
*     *id (id of elements)   [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the ByteWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* ByteWeakArray_creator_create(char id[2], void* user_data){ // consider allowing someone to create array with one object
  ByteWeakArray* ByteWeakArrayObject = malloc(sizeof(ByteWeakArray));
  char ByteWeakArrayID[2] = {0x01, 0x04};
  XYResult* newObject_result = newObject(ByteWeakArrayID, ByteWeakArrayObject);
  if(newObject_result->error == OK && ByteWeakArrayObject != NULL){
    ByteWeakArrayObject->size = 1;
    ByteWeakArrayObject->add = &ByteWeakArray_add;
    ByteWeakArrayObject->get = &ByteWeakArray_get;
    ByteWeakArrayObject->payload = NULL;
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
*      ByteWeakArray_creator_fromBytes
*
*  DESCRIPTION
*      Create an Strong Byte Array given a set of Bytes. Bytes must not include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the ByteWeakArray type.
*----------------------------------------------------------------------------*/
XYResult* ByteWeakArray_creator_fromBytes(char* data){

  XYResult* return_result = malloc(sizeof(XYResult));
  ByteWeakArray* return_array = malloc(sizeof(ByteWeakArray));
  if(return_result && return_array){
      return_array->add = &ByteWeakArray_add;
      return_array->remove = NULL;
      return_array->get = &ByteWeakArray_get;
      return_array->size = data[0]; // Size is one byte at the start of the (Byte)WeakArray.
      return_array->payload = malloc(sizeof(char)*(return_array->size-1));
      if(return_array->payload != NULL){
        memcpy(return_array->payload, &data[2], (return_array->size-1));
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
    if(return_result) free(return_result);
    if(return_array) free(return_array);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      ByteWeakArray_creator_toBytes
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
XYResult* ByteWeakArray_creator_toBytes(struct XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x01 && user_XYObject->id[1] == 0x04){
    ByteWeakArray* user_array = user_XYObject->GetPayload(user_XYObject);
    uint8_t totalSize = user_array->size;

    /*
     * Here we create a buffer and copy the size of the array
     * followed directly by the elements of the array.
     */

    char* byteBuffer = malloc(sizeof(char)*totalSize);
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL && byteBuffer != NULL){
      memcpy(byteBuffer, user_XYObject->GetPayload(user_XYObject), 1);
      memcpy(byteBuffer+1, user_array->payload, sizeof(char)*(totalSize-1));
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
