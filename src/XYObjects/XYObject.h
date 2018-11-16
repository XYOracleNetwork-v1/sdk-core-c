#include <string.h>
#include <stdint.h>
#include "../XYResult.h"
#include "../endian.h"

#define XY_TYPE_OFFSET (0)
#define XY_TYPE_LENGTH (2)

#define XY_MAJOR_OFFSET (0)
#define XY_MINOR_OFFSET (1)

#define XY_LENGTH_OFFSET (2)

/*
 * TYPES
 ****************************************************************************************
 */

typedef struct XYObject
{
  unsigned char id[2];
  void *payload;
} XYObject_t;

/*
 * FUNCTIONS
 ****************************************************************************************
 */

//the ID is a two byte array containing the major and minor
XYResult_t XYObject_getType(XYObject_t *self);

//the value is all the data in the object other than the header (major/minor/optional length)
XYResult_t XYObject_getValue(XYObject_t *self);

//this is the length of the object, including the length field (if one exists),
//but not including the major/minor fields
XYResult_t XYObject_getLength(XYObject_t *self);

//this is the length of the object, including the length field (if one exists),
//and also including the major/minor fields
XYResult_t XYObject_getFullLength(XYObject_t *self);

/*
 * MACROS
 ****************************************************************************************
 */

//should be called at the top of any XYObject function using self

#define INIT_SELF_UNKNOWN() \
  DECLARE_RESULT(); \
  CHECK_NULL(self);

#define INIT_SELF(_MAJOR_, _MINOR_) \
  INIT_SELF(); \
  CHECK_IS_XYOBJECT( _MAJOR_, _MINOR_ );

//check if the passed in buffer is the expected object type
#define IS_XYOBJECT(_MAJOR_, _MINOR_, _PTR_) ((((unsigned char *)_PTR_)[0] == _MAJOR_) && (((unsigned char *)_PTR_)[1] == _MINOR_))

//check the object type and return a validation error if it fails
#define CHECK_IS_XYOBJECT(_MAJOR_, _MINOR_) \
  if (!IS_XYOBJECT(_MAJOR_, _MINOR_, self)) \
  {                                         \
    XYERROR(XY_STATUS_ERROR);               \
  }

//helpers to read bytes from the object buffer
#define XYOBJ_READ_UINT8(_OFFSET_) (((uint8_t *)self)[_OFFSET_])
#define XYOBJ_READ_UINT16(_OFFSET_) to_uint16(((uint8_t *)self) + _OFFSET_)
#define XYOBJ_READ_UINT32(_OFFSET_) to_uint32(((uint8_t *)self) + _OFFSET_)

#define XYOBJ_COPY_UINT8_ARRAY(_DEST_, _OFFSET_, _LEN_) memcpy(_DEST_, self + _OFFSET_, _LEN_)
#define XYOBJ_INCREMENT_UINT16(_buffer_, _offset_, _value_) to_uint16_be((uint8_t *)_buffer_ + _offset_, to_uint16((uint8_t *)_buffer_ + _offset_) + _value_)
