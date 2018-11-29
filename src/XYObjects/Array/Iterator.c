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

XYArrayItr_t WeakArrayIterator(XYObjectHeader_t* header, char* buffer){
  if(!header || !buffer){
    return (XYArrayItr_t){0, 0, 0, 0};
  } else {
    uint8_t lenLen = lengthTypeToLength(header->flags.lengthType);
    return (XYArrayItr_t){header, buffer, (XYObjectHeader_t*)(buffer+lenLen), (XYObjectHeader_t*)(buffer+lenLen+2)};
    /*
    if(header->flags.typed == 1){
      return (XYArrayItr_t){header, buffer, (XYObjectHeader_t*)buffer+lenLen, buffer+lenLen+2};
    } else {
      return (XYArrayItr_t){header, buffer, (XYObjectHeader_t*)buffer+lenLen, buffer+lenLen};
    }
    */
  }
}

XYObject_t IteratorNext(XYArrayItr_t* itr){
  XYObject_t  returnObject;
  returnObject.header = NULL;
  returnObject.payload = NULL;
  
  if((itr->header->flags.iteratable != 1)){
    return returnObject;
  }

  if(!itr->header->flags.typed){
    returnObject.header = itr->innerHeader;
    returnObject.payload = itr->indexPtr;
    uint32_t jump = XYObject_getLength(&returnObject).value.ui;
    itr->innerHeader = itr->indexPtr + jump;
    itr->indexPtr = (char*)itr->innerHeader + sizeof(XYObjectHeader_t);
    returnObject.header = itr->innerHeader;
    returnObject.payload = itr->indexPtr;
  } else {
    returnObject.header = itr->innerHeader;
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

  if((itr->header->type != TYPE_ARRAY)){
    return returnObject;
  }

  if(itr->header->flags.typed){
    returnObject.header = itr->indexPtr;
    returnObject.payload = itr->indexPtr + sizeof(XYObjectHeader_t);

  } else {
    returnObject.header = itr->header;
    returnObject.payload = itr->indexPtr;
  }
  return returnObject;
}

//void breakpoint();
//void breakpoint(){}
//#define Iterator_gap(_self, _element, _bytesAfter, _offset, _optional) Iterator_bookmark(_self, _element, _bytesAfter, _offset, _optional)

XYResult_t Iterator_bookmark(XYObject_t* self, uint32_t element, uint32_t bytesAfter, uint32_t offset, XYArrayItr_t* optionalItr){
  DECLARE_RESULT()

  /*
   * Malloc needed here to prevent memcpy from 
   * copying into the buffer it's replacing
   */
  XYArrayItr_t* objectItr;
 
  if(optionalItr != NULL){
    objectItr = optionalItr;
  } else {
    XYArrayItr_t itr = WeakArrayIterator(self->header, self->payload);
    objectItr = &itr;
  }
  XYObject_t currentObject = IteratorGet(objectItr);
  for(int i = 0; i!=element+1; i++){
    if(currentObject.header == NULL && (i!=-1)){
      result.status = XY_STATUS_ERROR;
      return result;
    }
    XYObject_t currentObject = IteratorNext(objectItr);
  }
  
  char* current = objectItr->indexPtr;
  ///char* tempBuffer = malloc(((char*)self->payload+(XYObject_getLength(self).value.ui))-current);
  
  memmove(current+offset, current, bytesAfter);
  //memcpy(tempBuffer, current, bytesAfter);
  //memset(current, 0, ((char*)self->payload+(XYObject_getLength(self).value.ui))-current);
  //memcpy(current+offset, tempBuffer, bytesAfter);
  
  //free(tempBuffer);
  
  XYOBJ_INCREMENT(offset);
  
  result.status = XY_STATUS_OK;
  return result;
}

XYResult_t Iterator_insert(XYObject_t* self, uint32_t element, uint32_t offset, uint32_t totalBytes, char* bytes){
  
  DECLARE_RESULT();

  /*
   * Malloc needed here to prevent memcpy from 
   * copying into the buffer it's replacing
   */
  
  
  XYArrayItr_t objectItr = WeakArrayIterator(self->header, self->payload);
  char* charCounter = objectItr.indexPtr;
  XYObject_t currentObject = IteratorGet(&objectItr);
  for(int i = 0; i!=element+1; i++){
    if(currentObject.header == NULL && (i!=-1)){
      result.status = XY_STATUS_ERROR;
      return result;
    }
    currentObject = IteratorNext(&objectItr);
  }
  
  char* current = objectItr.indexPtr;
  
  uint32_t bytesAfter = current-charCounter;
  
  memmove(current+offset, current, bytesAfter);
  //memcpy(tempBuffer, current, bytesLeft);
  memcpy(current, bytes, offset);
  //memcpy(current+offset, tempBuffer, bytesLeft);
  

  XYOBJ_INCREMENT(offset);
  result.status = XY_STATUS_OK;
  return result;
}

// end of file Iterator.c
