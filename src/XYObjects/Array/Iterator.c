/**
 * @file WeakArray.c
 * @author Arie Trouw (developer@xy.company)
 * @brief 
 * @version 0.1
 * @date 2018-11-15
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

#include "Iterator.h"
#include <stdlib.h>

XYArrayItr_t WeakArrayIterator(XYObject_t* self){
  XYArrayItr_t returnItr;
  XYObject_t  returnObject;
  returnObject.header = NULL;
  returnObject.payload = NULL;
  returnItr.array = returnObject;
  returnItr.index = 0;
  returnItr.indexPtr = 0;

  if(((XYObject_t*)self)->header->type != TYPE_ARRAY){
    return returnItr;
  }
  returnObject.header = self->header;
  returnObject.payload = self->payload;
  returnItr.array = returnObject;
  returnItr.index = 0;
  returnItr.indexPtr = ((char*)self->payload)+lengthTypeToLength(self->header->flags.lengthType);

  return returnItr;
}

XYObject_t IteratorNext(XYArrayItr_t* itr){
  XYObject_t  returnObject;
  returnObject.header = NULL;
  returnObject.payload = NULL;
  
  if((itr->array.header->type != TYPE_ARRAY)){
    return returnObject;
  }

  if(itr->array.header->flags.typed){
    returnObject.header = itr->indexPtr;
    returnObject.payload = itr->indexPtr + sizeof(XYObjectHeader_t);
    

    uint32_t jump = XYObject_getFullLength(&returnObject).value.ui;
    itr->indexPtr = itr->indexPtr + jump;

    returnObject.header = itr->indexPtr;
    returnObject.payload = itr->indexPtr + sizeof(XYObjectHeader_t);
  } else {
    returnObject.header = itr->array.header;
    returnObject.payload = itr->indexPtr;
    
    uint32_t jump = XYObject_getLength(&returnObject).value.ui;
    itr->indexPtr = itr->indexPtr + jump;
    returnObject.payload = itr->indexPtr;
  }

  return returnObject;
}

XYObject_t IteratorGet(XYArrayItr_t* itr){
  XYObject_t  returnObject;
  returnObject.header = NULL;
  returnObject.payload = NULL;

  if((itr->array.header->type != TYPE_ARRAY)){
    return returnObject;
  }

  if(itr->array.header->flags.typed){
    returnObject.header = itr->indexPtr;
    returnObject.payload = itr->indexPtr + sizeof(XYObjectHeader_t);

  } else {
    returnObject.header = itr->array.header;
    returnObject.payload = itr->indexPtr;
  }
  return returnObject;
}

#define Iterator_gap(_self, _element, _bytesAfter, _offset) Iterator_bookmark(_self, _element, _bytesAfter, _offset, NULL)
//#define Iterator_gap(_self, _element, _bytesAfter, _offset, _optional) Iterator_gap_exp(_self, _element, _bytesAfter, _offset, _optional)
XYResult_t Iterator_bookmark(XYObject_t* self, uint32_t element, uint32_t bytesAfter, uint32_t offset, XYArrayItr_t* optionalItr){
  
  INIT_SELF(TYPE_ARRAY);

  /*
   * Malloc needed here to prevent memcpy from 
   * copying into the buffer it's replacing
   */
  XYArrayItr_t* objectItr;
  if(optionalItr != NULL){
    objectItr = optionalItr;
  } else {
    XYArrayItr_t itr = WeakArrayIterator(self);
    objectItr = &itr;
  }
  
  for(int i = 0; i!=element+1; i++){
    XYObject_t currentObject = IteratorNext(objectItr);
    if(currentObject.header == NULL && (i!=-1)){
      result.status = XY_STATUS_ERROR;
      return result;
    }
  }
  
  char* current = objectItr->indexPtr;
  objectItr->gap = objectItr->indexPtr;
  char* tempBuffer = malloc(((char*)self->payload+(XYObject_getLength(self).value.ui))-current);
  
  memcpy(tempBuffer, current, bytesAfter);
  memset(current, 0, ((char*)self->payload+(XYObject_getLength(self).value.ui))-current);
  memcpy(current+offset, tempBuffer, bytesAfter);
  
  free(tempBuffer);

  XYOBJ_INCREMENT(offset);
  
  result.status = XY_STATUS_OK;
  return result;
}

// end of file Iterator.c
