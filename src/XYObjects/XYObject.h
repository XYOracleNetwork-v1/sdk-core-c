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
  XYObjectHeader_t* header;
  void* payload;
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
  CHECK_NULL(self); \

#define INIT_SELF(_TYPE_) \
  INIT_SELF_UNKNOWN(); \
  CHECK_IS_XYOBJECT( _TYPE_ );

//check if the passed in buffer is the expected object type
#define IS_XYOBJECT(_TYPE_, _PTR_) (((XYObject_t*)_PTR_)->header->type == _TYPE_)

//check the object type and return a validation error if it fails
#define CHECK_IS_XYOBJECT(_TYPE_) \
  if( !IS_XYOBJECT(_TYPE_, self) ) \
  {                                         \
    XYERROR(XY_STATUS_ERROR);               \
  }

//helpers to read bytes from the object buffer
#define XYOBJ_READ_UINT8() (*(uint8_t*)(self->payload))
#define XYOBJ_READ_UINT16() to_uint16((unsigned char*)((XYObject_t *)self->payload))
#define XYOBJ_READ_UINT32() to_uint32((unsigned char*)((XYObject_t *)self->payload))
#define XYOBJ_READ_UINT64() to_uint64((unsigned char*)((XYObject_t *)self->payload))

#define XYOBJ_READ_UINT8_ARRAY(_DEST_, _LEN_) memcpy(_DEST_, (XYObject_t *)self->payload, _LEN_)
#define XYOBJ_WRITE_UINT8_ARRAY(_SRC_, _LEN_) memcpy((XYObject_t *)self->payload, _SRC_,  _LEN_)

#define XYOBJ_INCREMENT_UINT16(_VALUE_) to_uint16_be((unsigned char*)((XYObject_t *)self)->payload, to_uint16(self->payload) + _VALUE_)
#define XYOBJ_INCREMENT_UINT32(_VALUE_) to_uint32_be((unsigned char*)((XYObject_t *)self)->payload, to_uint32(self->payload) + _VALUE_)
#define XYOBJ_INCREMENT_UINT64(_VALUE_) to_uint64_be((unsigned char *)((XYObject_t *)self)->payload, to_uint64(self->payload) + _VALUE_)
