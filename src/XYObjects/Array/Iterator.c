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
    return (XYArrayItr_t){header, buffer, (XYObjectHeader_t*)(buffer+lenLen), (char*)(buffer+lenLen+2)};
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
    itr->innerHeader = (XYObjectHeader_t*)itr->indexPtr + jump;
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
    returnObject.header = (XYObjectHeader_t*)itr->indexPtr;
    returnObject.payload = itr->indexPtr + sizeof(XYObjectHeader_t);

  } else {
    returnObject.header = itr->header;
    returnObject.payload = itr->indexPtr;
  }
  return returnObject;
}

XYResult_t Iterator_bookmark(XYObject_t* self, uint32_t element, uint32_t bytesAfter, uint32_t offset, XYArrayItr_t* optionalItr){
  DECLARE_RESULT()
  XYArrayItr_t* objectItr;
 
  if(optionalItr != NULL){
    objectItr = optionalItr;
  } else {
    XYArrayItr_t itr = WeakArrayIterator(self->header, self->payload);
    objectItr = &itr;
  }
  XYObject_t currentObject = IteratorGet(objectItr);
  for(uint32_t i = 0; i!=element+1; i++){
    if(currentObject.header == NULL && (i!=0xFFFFFFFF)){
      result.status = XY_STATUS_ERROR;
      return result;
    }
    currentObject = IteratorNext(objectItr);
  }
  
  char* current = objectItr->indexPtr;
  
  memmove(current+offset, current, bytesAfter);
  
  XYOBJ_INCREMENT(offset);
  
  result.status = XY_STATUS_OK;
  return result;
}

XYResult_t Iterator_insert(XYObject_t* self, uint32_t element, uint32_t offset, uint32_t totalBytes, char* bytes){
  
  DECLARE_RESULT();
  XYArrayItr_t objectItr = WeakArrayIterator(self->header, self->payload);
  unsigned char* charCounter = (unsigned char*)objectItr.indexPtr;
  XYObject_t currentObject = IteratorGet(&objectItr);
  for(uint32_t i = 0; i!=element+1; i++){
    if(currentObject.header == NULL && (i!=0xFFFFFFFF-1)){
      result.status = XY_STATUS_ERROR;
      return result;
    }
    currentObject = IteratorNext(&objectItr);
  }
  
  unsigned char* current = (unsigned char*)objectItr.indexPtr;
  
  long bytesAfter = current-charCounter;
  if(bytesAfter > 0xFFFFFFFF && bytesAfter>0){
    #ifdef XY_DEBUGMODE
    printf("Tried to bridge too big a gap.\n");
    #endif
    exit(-100); //TODO: meaningful error code.
  }
  // Note: Cast from long to unsigned long
  memmove(current+offset, current, (unsigned long)bytesAfter); 
  memcpy(current, bytes, offset);
  

  XYOBJ_INCREMENT(offset);
  result.status = XY_STATUS_OK;
  return result;
}

// end of file Iterator.c
