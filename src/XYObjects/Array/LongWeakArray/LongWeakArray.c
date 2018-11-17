/**
 * @file ShortWeakArray.c
 * @author Arie Trouw (developer@xy.company)
 * @brief 
 * @version 0.1
 * @date 2018-11-15
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

#include "LongWeakArray.h"
 
XYResult_t LongWeakArray_add(XYObject_t *self,
                              XYObjectHeader_t newItemHeader,
                              int newItemLength)
{
  INIT_SELF(MINOR_NULL);

  XYResult_t currentLength = XYObject_getLength(self);
  CHECK_RESULT(currentLength);

  XYResult_t fullLength = XYObject_getFullLength(self);
  CHECK_RESULT(fullLength);

  //put the new object at the end of the current object
  uint8_t *newObject = ((uint8_t *)self + fullLength.value.i);

  //set the type of the newly added item
  memcpy(newObject, &newItemHeader, sizeof(newItemHeader));

  //set the new length of the array object (old length + new object length)
  XYOBJ_INCREMENT_UINT16(XY_LENGTH_OFFSET, newItemLength);

  return result;
}

XYResult_t LongWeakArray_get(XYObject_t *self, int index)
{
  INIT_SELF(MINOR_NULL);

  XYResult_t fullLength = XYObject_getFullLength(self);
  CHECK_RESULT(fullLength);

  //the nextPtr points to the first byte after the end of this object
  //we use this to prevent over flow and to check for end of list
  uint8_t *outOfBoundsPtr = ((uint8_t *)self + fullLength.value.i);

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
    lengthOfSubObject = XYObject_getFullLength((XYObject_t *)ptr);
    CHECK_RESULT(lengthOfSubObject);
    ptr += lengthOfSubObject.value.i;
  }

  //we iterated past all the other items, so we should be pointing to the item they need now
  result.value.ptr = ptr;

  return result;
}

// end of file LongWeakArray.c
