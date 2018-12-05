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

#include "WeakArray.h"
#include <stdlib.h>

uint8_t lengthTypeToLength(int _VALUE_){
  switch (_VALUE_)
  {
  case XY_LENGTH_1BYTE:
    return 1;
  case XY_LENGTH_2BYTE:
    return 2;
  case XY_LENGTH_4BYTE:
    return 4;
  case XY_LENGTH_8BYTE:                                   
  return 8;
  }
  return 255;
}

XYResult_t WeakArray_add(XYObject_t *self,
                              XYObject_t* newItem,
                              uint32_t newItemLength)
{
  INIT_SELF(TYPE_ARRAY);

  XYResult_t currentLength = XYObject_getLength(self);
  CHECK_RESULT(currentLength);

  XYResult_t fullLength = XYObject_getFullLength(self);
  CHECK_RESULT(fullLength);

  //put the new object at the end of the current object
  uint8_t *newObject = ((unsigned char*)((XYObject_t *)self)->payload) + currentLength.value.ui;
  
  //set the type of the newly added item
  if(!self->header->flags.typed || currentLength.value.ui == lengthTypeToLength(self->header->flags.lengthType)){
    memcpy(newObject, newItem->header, sizeof(XYObjectHeader_t));
    memcpy(newObject+sizeof(XYObjectHeader_t), newItem->payload, newItemLength-2);
  } else {
    memcpy(newObject, newItem->payload, newItemLength);
  }

  //set the new length of the array object (old length + new object length)
  XYOBJ_INCREMENT(newItemLength);

  return result;
}

XYResult_t WeakArray_get(XYObject_t *self, int index)
{
  INIT_SELF(TYPE_ARRAY);

  XYResult_t fullLength = XYObject_getFullLength(self);
  CHECK_RESULT(fullLength);

  //the nextPtr points to the first byte after the end of this object
  //we use this to prevent over flow and to check for end of list
  uint8_t *outOfBoundsPtr = ((unsigned char*)((XYObject_t *)self)->payload) + fullLength.value.i;

  XYResult_t currentValue = XYObject_getValue(self);
  CHECK_RESULT(currentValue);

  //we use the ptr to iterate over the items in the array
  uint8_t *ptr = currentValue.value.ptr;
  XYResult_t lengthOfSubObject;

  for (int i = 0; i < index; i++)
  {
    if (ptr >= outOfBoundsPtr)
    {
      XYERROR(XY_STATUS_INDEXOUTOFRANGE)
    }
    XYObject_t subObject;
    if(self->header->flags.typed){
      subObject.header = self->header;
      subObject.payload = ptr;
    } else {
      subObject.header = (XYObjectHeader_t*)ptr;
      subObject.payload = ptr+2;
    }
    lengthOfSubObject = XYObject_getFullLength(&subObject);
    CHECK_RESULT(lengthOfSubObject);
    ptr += lengthOfSubObject.value.i;
  }

  //we iterated past all the other items, so we should be pointing to the item they need now
  result.value.ptr = ptr;

  return result;
}

// end of file WeakArray.c
