/**
 ****************************************************************************************
 *
 * @file shortstrongarray.h
 *
 * @XY4 project source code.
 *
 * @brief primary shortstrongarray routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 ****************************************************************************************
 */

#ifndef INTSTRONGARRAY_H
#define INTSTRONGARRAY_H

/*
 * INCLUDES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef struct IntStrongArray IntStrongArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
struct IntStrongArray {
  uint32_t  size;
  char      id[2];
  XYResult_t* (*add)(IntStrongArray_t*  self_IntStrongArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(IntStrongArray_t*  self_IntStrongArray, int index);
  XYResult_t* (*get)(IntStrongArray_t*  self_IntStrongArray, int index);
  void* payload;
} ; //0x03

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
XYResult_t* IntStrongArray_add(IntStrongArray_t* self_IntStrongArray, XYObject_t* user_XYObject);
XYResult_t* IntStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntStrongArray_creator_fromBytes(char* data);
XYResult_t* IntStrongArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* BlockSet_creator_toBytes(XYObject_t* user_XYObject);

#endif

// end of file intstrongarray.h
