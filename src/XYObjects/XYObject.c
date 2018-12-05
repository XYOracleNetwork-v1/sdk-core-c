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
uint32_t getLength(XYObject_t* self) {
  switch(self->header->flags.lengthType) {
    case XY_LENGTH_1BYTE:
      return XYOBJ_READ_UINT8();
    case XY_LENGTH_2BYTE:
      return XYOBJ_READ_UINT16();
    case XY_LENGTH_4BYTE:
      return XYOBJ_READ_UINT32();
    //case XY_LENGTH_8BYTE:          // Disabled to avoid integer-overflow related bugs
    //  return XYOBJ_READ_UINT64();
  }

  return 0;
}

//assuming validation by caller
uint32_t getLengthFieldSize(XYObject_t* self) {
  switch(self->header->flags.lengthType) {
    case XY_LENGTH_1BYTE:
      return 1;
    case XY_LENGTH_2BYTE:
      return 2;
    case XY_LENGTH_4BYTE:
      return 4;
    //case XY_LENGTH_8BYTE:
    //  return 8;
  }

  return 0;
}

/**
 * ==== Public Functions ====
 */

XYResult_t XYObject_getValue(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.ptr = ((uint8_t*)self) + XY_HEADER_LENGTH + getLengthFieldSize(self);
  return result;
}

XYResult_t XYObject_getLength(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.ui = getLength(self);
  return result;
}

XYResult_t XYObject_getFullLength(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.ui = getLength(self) + XY_HEADER_LENGTH;
  return result;
}

uint8_t matchType(XYObject_t* obj, uint8_t type){
  switch(obj->header->type){
    case 1:
      return obj->header->type == type;
    case 2:
      if(type == MINOR_ARRAY){
        return TRUE;
      } else {
        return FALSE;
      }
    default:
      return obj->header->type == type;
  }
}
