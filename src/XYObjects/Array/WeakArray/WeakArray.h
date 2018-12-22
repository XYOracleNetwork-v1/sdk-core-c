#pragma once
//TODO: Debug
#include <stdio.h>

#include "../../../XYResult.h"
#include "../../XYObject.h"
#include "../../XYObjectTypes.h"
#include "../../../defines.h"


/**
 * @brief Add Item from a Weak Array
 * 
 * @param self Pointer to self
 * @param newItemHeader Pointer to the object we will add
 * @param newItemLength Length of object in array. If typed array the first element newItemLength would be sizeof(XYObjectHeader_t)+sizeof(payload) subsequent adds will only be sizeof(payload)
 * @return XYResult_t Success or Failure
 */
XYResult_t WeakArray_add(XYObject_t *self, XYObject_t* newItemHeader);

/**
 * @brief Get Item from a Weak Array
 * 
 * @param self Pointer to self
 * @param index Index to retreive 
 * @return XYResult_t Object in array at index
 */
#ifdef BUILD_WEAKARRAY_GET
XYResult_t WeakArray_get(XYObject_t *self,
                         int index);
#endif

#define XYARRAY_INCREMENT(_ARRAY_, _VALUE_)\
  switch (((XYObject_t *)_ARRAY_)->header->flags.lengthType)\
  {                                                       \
  case XY_LENGTH_1BYTE:                                   \
    memset(((XYObject_t *)_ARRAY_)->payload, (uint8_t)_VALUE_, 1);\
    break;                                                \
  case XY_LENGTH_2BYTE:                                   \
    to_uint16_be((unsigned char*)((XYObject_t *)_ARRAY_)->payload, to_uint16((unsigned char*)((XYObject_t *)_ARRAY_)->payload) + _VALUE_);\
    break;                                                \
  case XY_LENGTH_4BYTE:                                   \
    to_uint32_be((unsigned char*)((XYObject_t *)_ARRAY_)->payload, to_uint32((unsigned char*)((XYObject_t *)_ARRAY_)->payload) + _VALUE_);\
    break;                                                \
  case XY_LENGTH_8BYTE:                                   \
    to_uint64_be((unsigned char*)((XYObject_t *)_ARRAY_)->payload, to_uint64((unsigned char*)((XYObject_t *)_ARRAY_)->payload) + _VALUE_);\
    break;                                                \
  }

#define XYOBJ_INCREMENT(_VALUE_)                          \
  switch (((XYObject_t *)self)->header->flags.lengthType) \
  {                                                       \
  case XY_LENGTH_1BYTE:                                   \
    memset(((XYObject_t *)self)->payload, (uint8_t)_VALUE_, 1);    \
    break;                                                \
  case XY_LENGTH_2BYTE:                                   \
    XYOBJ_INCREMENT_UINT16((uint16_t)_VALUE_);                      \
    break;                                                \
  case XY_LENGTH_4BYTE:                                   \
    XYOBJ_INCREMENT_UINT32(_VALUE_);                      \
    break;                                                \
  case XY_LENGTH_8BYTE:                                   \
    XYOBJ_INCREMENT_UINT64((uint64_t)_VALUE_);                      \
    break;                                                \
  }

uint8_t lengthTypeToLength(int _VALUE_);
