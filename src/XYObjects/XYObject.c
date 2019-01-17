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

uint8_t lengthTypeToLength(int _VALUE_){
  switch (_VALUE_)
  {
  case XY_LENGTH_1BYTE:
    return 1;
  case XY_LENGTH_2BYTE:
    return 2;
  case XY_LENGTH_4BYTE:
    return 4;
  case XY_LENGTH_8BYTE:                                   
  return 8;
  }
  return 255;
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
  if(result.value.ui == 255){
    result.status = XY_STATUS_ERROR;
    return result;
  }
  return result;
}

XYResult_t XYObject_getFullLength(XYObject_t* self) {
  INIT_SELF_UNKNOWN();
  result.value.ui = getLength(self) + XY_HEADER_LENGTH;
  return result;
}

XYObject_t XYObject_fromPtr(void* ptr){
	char* start = (char*)ptr;
	return (XYObject_t){(XYObjectHeader_t*)&start[0], (XYObjectHeader_t*)&start[2]};
}

uint8_t getFullHeaderSize(unsigned char* header){
	XYObjectHeader_t* tempHeader;
	tempHeader = (XYObjectHeader_t*)((char*)header);
	
	uint32_t fetterSetSizeLength = lengthTypeToLength(tempHeader->flags.lengthType);
	return sizeof(XYObjectHeader_t)+fetterSetSizeLength;
}

uint8_t matchType(XYObjectHeader_t* header, uint8_t type){
  if(header == NULL) { return FALSE; }
  switch(header->type){
    case 1:
      return header->type == type;
    case 2:
      if(type == MINOR_ARRAY){
        return TRUE;
      } else {
        return FALSE;
      }
    default:
      return header->type == type;
  }
}
