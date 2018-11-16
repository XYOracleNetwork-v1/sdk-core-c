#include <string.h>
#include <stdint.h>
#include "../XYResult.h"
#include "XYObjectHeader.h"
#include "../endian.h"

/*
 * TYPES
 ****************************************************************************************
 */

typedef struct XYObject
{
  XYObjectHeader_t header;
  void *payload;
} XYObject_t;

/*
 * FUNCTIONS
 ****************************************************************************************
 */

//the value is all the data in the object other than the header (header/length length)
XYResult_t XYObject_getValue(XYObject_t *self);

//this is the length of the object, including the length field (if one exists),
//but not including the header
XYResult_t XYObject_getLength(XYObject_t *self);

//this is the length of the object, including the length field (if one exists),
//and also including the header
XYResult_t XYObject_getFullLength(XYObject_t *self);

/*
 * MACROS
 ****************************************************************************************
 */

//should be called at the top of any XYObject function using self

#define INIT_SELF_UNKNOWN() \
  DECLARE_RESULT(); \
  CHECK_NULL(self);

#define INIT_SELF(_TYPE_) \
  INIT_SELF_UNKNOWN(); \
  CHECK_IS_XYOBJECT( _TYPE_ );

//check if the passed in buffer is the expected object type
#define IS_XYOBJECT(_TYPE_, _PTR_) (((XYObjectHeader_t*)_PTR_)->type == _TYPE_)

//check the object type and return a validation error if it fails
#define CHECK_IS_XYOBJECT(_TYPE_) \
  if (!IS_XYOBJECT(_TYPE_, self)) \
  {                                         \
    XYERROR(XY_STATUS_ERROR);               \
  }

//helpers to read bytes from the object buffer
#define XYOBJ_READ_UINT8(_OFFSET_) (((uint8_t *)self)[_OFFSET_])
#define XYOBJ_READ_UINT16(_OFFSET_) to_uint16(((uint8_t *)self) + _OFFSET_)
#define XYOBJ_READ_UINT32(_OFFSET_) to_uint32(((uint8_t *)self) + _OFFSET_)
#define XYOBJ_READ_UINT64(_OFFSET_) to_uint64(((uint8_t *)self) + _OFFSET_)

#define XYOBJ_COPY_UINT8_ARRAY(_DEST_, _OFFSET_, _LEN_) memcpy(_DEST_, self + _OFFSET_, _LEN_)
#define XYOBJ_INCREMENT_UINT16(_OFFSET_, _VALUE_) to_uint16_be((uint8_t *)self + _OFFSET_, to_uint16((uint8_t *)_buffer_ + _OFFSET_) + _VALUE_)
