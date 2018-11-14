/**
 ****************************************************************************************
 *
 * @file byteweakarray.h
 *
 * @XY4 project source code.
 *
 * @brief primary bytestrongarray routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef BYTEWEAKARRAY_H
#define BYTEWEAKARRAY_H

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
typedef struct ByteWeakArray ByteWeakArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 struct ByteWeakArray {
   uint8_t  size;
   XYResult_t* (*add)(ByteWeakArray_t* self_ByteWeakArray, XYObject_t* user_XYObject);
   XYResult_t* (*remove)(ByteWeakArray_t* self_ByteWeakArray, int index);
   XYResult_t* (*get)(ByteWeakArray_t* self_ByteWeakArray, int index);
   void* payload;
 } ;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
XYResult_t* ByteWeakArray_add(ByteWeakArray_t* self_ByteWeakArray, XYObject_t* user_XYObject);
XYResult_t* ByteWeakArray_get(ByteWeakArray_t* self_ByteWeakArray, int index);
XYResult_t* ByteWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ByteWeakArray_creator_toBytes(XYObject_t* user_XYObject);
#endif

// end of file byteweakarray.h
