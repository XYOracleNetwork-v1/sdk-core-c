#include "../../../XYResult.h"
#include "../../XYObject.h"
#include "../../XYObjectTypes.h"
//TODO: Debug
#include <stdio.h>

/**
 * @brief Add Item from a Weak Array
 * 
 * @param self 
 * @param newItemHeader 
 * @param newItemLength 
 * @return XYResult_t 
 */
XYResult_t WeakArray_add(XYObject_t* self,
                               XYObjectHeader_t newItemHeader,
                               int newItemLength);

/**
 * @brief Get Item from a Weak Array
 * 
 * @param self 
 * @param index 
 * @return XYResult_t 
 */
XYResult_t WeakArray_get(XYObject_t *self,
                              int index);


#define XYOBJ_INCREMENT(_VALUE_) \
  switch(((XYObject_t*)self)->header->flags.lengthType) { \
    case XY_LENGTH_1BYTE: \
    memset(((XYObject_t*)self)->payload, _VALUE_, 1); \
    case XY_LENGTH_2BYTE: \
      XYOBJ_INCREMENT_UINT16(_VALUE_); \
    case XY_LENGTH_4BYTE: \
      XYOBJ_INCREMENT_UINT32(_VALUE_); \
    case XY_LENGTH_8BYTE: \
      XYOBJ_INCREMENT_UINT64(_VALUE_); \
  }

#define XYOBJ_INCREMENT_UINT16(_VALUE_) to_uint16_be((unsigned char*)((XYObject_t *)self)->payload, to_uint16(self->payload) + _VALUE_)
#define XYOBJ_INCREMENT_UINT32(_VALUE_) to_uint32_be((unsigned char*)((XYObject_t *)self)->payload, to_uint32(self->payload) + _VALUE_)
#define XYOBJ_INCREMENT_UINT64(_VALUE_) to_uint64_be((unsigned char*)((XYObject_t *)self)->payload, to_uint64(self->payload) + _VALUE_)
