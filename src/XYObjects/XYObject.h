#pragma once

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

static const char TypedArray_id[2]            = { 0x30, 0x01 };
static const char UnTypedArray_id[2]          = { 0x20, 0x01 };

static const char BoundWitness_id[2]          = { 0x02, 0x01 };
static const char KeySet_id[2]                = { 0x02, 0x02 };
static const char SignatureSet_id[2]          = { 0x02, 0x03 };
static const char Payload_id[2]               = { 0x02, 0x04 };
static const char Index_id[2]                 = { 0x02, 0x05 };
static const char PreviousHash_id[2]          = { 0x02, 0x06 };
static const char NextPublicKey_id[2]         = { 0x02, 0x07 };
static const char BridgeHashSet_id[2]         = { 0x02, 0x08 };
static const char BridgeBlockSet_id[2]        = { 0x02, 0x09 };
static const char BoundWitnessTransfer_id[2]  = { 0x02, 0x0a };

static const char Sha256_id[2]                = { 0x03, 0x05 };
static const char ECDSASecp256k1_id[2]        = { 0x04, 0x01 };
static const char ECDSASecp256k1Sig_id[2]     = { 0x05, 0x01 };
static const char Rssi_id[2]                  = { 0x08, 0x01 };

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
