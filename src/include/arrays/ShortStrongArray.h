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

#ifndef SHORTSTRONGARRAY_H
#define SHORTSTRONGARRAY_H

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
typedef struct ShortStrongArray ShortStrongArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 struct ShortStrongArray {
   uint16_t size;
   char     id[2];
   XYResult_t* (*add)(ShortStrongArray_t*  self_ShortStrongArray, XYObject_t* user_XYObject);
   XYResult_t* (*remove)(ShortStrongArray_t*  self_ShortStrongArray, int index);
   XYResult_t* (*get)(ShortStrongArray_t*  self_ShortStrongArray, int index);
   void* payload;
 } ; //0x02

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 XYResult_t* ShortStrongArray_add(ShortStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);
 XYResult_t* ShortStrongArray_creator_create(const char id[2], void* user_data);
 XYResult_t* ShortStrongArray_creator_fromBytes(char* data);
 XYResult_t* ShortStrongArray_creator_toBytes(XYObject_t* user_XYObject);

#endif

// end of file shortstrongarray.h
