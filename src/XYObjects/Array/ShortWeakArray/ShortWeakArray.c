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

#include "ShortWeakArray2.h"
 
XYResult_t ShortWeakArray_add(XYObject_t* self,
                               unsigned char newItemType[2],
                               int length){
  INIT_SELF(MAJOR_2BYTE_ARRAY, MINOR_ARRAY_WEAK);

  XYResult_t currentLength = XYObject_getLength(self);
  CHECK_RESULT(currentLength);

  //put the new object at the end of the current object
  uint8_t* newObject = ((uint8_t*)self + XY_TYPE_LENGTH + currentLength.value.i);

  //set the type of the newly added item
  memcpy(newObject, newItemType, 2);

  //set the new length of the array object (old length + new object length)
  XYOBJ_INCREMENT_UINT16(self, XY_LENGTH_OFFSET, length);
  
  return result;
}

XYResult_t ShortWeakArray_get(XYObject_t* self, int index) {
  INIT_SELF(MAJOR_2BYTE_ARRAY, MINOR_ARRAY_WEAK);
  CHECK_IS_XYOBJECT( MAJOR_2BYTE_ARRAY, MINOR_ARRAY_WEAK )

  XYResult_t currentLength = XYObject_getLength(self);
  CHECK_RESULT(currentLength);

  //the nextPtr points to the first byte after the end of this object
  uint8_t* nextPtr = ((uint8_t*)self) + currentLength.value.i + XY_TYPE_LENGTH;

  XYResult_t currentValue = XYObject_getValue(self);
  CHECK_RESULT(currentValue);

  //we use the ptr to iterate over the items in the array
  uint8_t* ptr = currentValue.value.ptr;
  XYResult_t lengthOfSubObject;

  for (int i = 0; i < index; i++) {
    if (ptr >= nextPtr) {
      XYERROR(XY_STATUS_INDEXOUTOFRANGE)
    }
    lengthOfSubObject = XYObject_getFullLength((XYObject_t*)ptr);
    CHECK_RESULT(lengthOfSubObject);
    ptr += lengthOfSubObject.value.i;
  }

  //we iterated past all the other items, so we should be pointing to the item they need now
  result.value.ptr = ptr;
  
  return result;
  
}

// end of file ShortWeakArray.c
