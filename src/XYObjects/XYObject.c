/**
 ****************************************************************************************
 *
 * @file xyobject.c
 *
 * @XYO Core library source code.
 *
 * @brief primary xy object routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "XYObject.h"

/**
 * ==== Private Functions ====
 */

//assuming validation by caller
int getLength(XYObject_t* self) {
  uint8_t major = XYOBJ_READ_UINT8(XY_MAJOR_OFFSET);

  int result = 0;

  //if the top bit is set, then there is a length field
  //that we need to skip
  if (major & 0x80) {

    if (major & 0x40) {
      //if the second highest bit is 1, the length field is 4 bytes
      result = XYOBJ_READ_UINT8(XY_LENGTH_OFFSET);
    } else {
      //if the second highest bit is 0, the length field is 2 bytes
      result += 2;
    }
  } else {
    result = major;
  }
  return result;
}

//assuming validation by caller
int getLengthFieldSize(XYObject_t* self) {
  uint8_t major = XYOBJ_READ_UINT8(XY_MAJOR_OFFSET);
  int result = 0;
  //if the top bit is set, then there is a length field
  if (major & 0x80) {

    if (major & 0x40) {
      //if the second highest bit is 1, the length field is 4 bytes
      result = 4;
    } else {
      //if the second highest bit is 0, the length field is 2 bytes
      result = 2;
    }
  }
  return result;
}

/**
 * ==== Public Functions ====
 */

XYResult_t XYObject_getType(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  XYOBJ_COPY_UINT8_ARRAY(result.value.bytes, XY_TYPE_OFFSET, XY_TYPE_LENGTH);
  return result;
}

XYResult_t XYObject_getValue(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.ptr = ((uint8_t*)self) + getLengthFieldSize(self);
  return result;
}

XYResult_t XYObject_getLength(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.i = getLength(self);
  return result;
}

XYResult_t XYObject_getFullLength(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.i = getLength(self) + XY_TYPE_LENGTH;
  return result;
}