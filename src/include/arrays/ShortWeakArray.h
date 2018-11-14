/**
 ****************************************************************************************
 *
 * @file shortweakarray.h
 *
 * @XY4 project source code.
 *
 * @brief primary shortweakarray routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef SHORTWEAKARRAY_H
#define SHORTWEAKARRAY_H

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
typedef struct ShortWeakArray ShortWeakArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 struct ShortWeakArray {
   uint16_t size;
   XYResult_t* (*add)(ShortWeakArray_t* self_ShortWeakArray, XYObject_t* user_XYObject);
   XYResult_t* (*remove)(ShortWeakArray_t* self_ShortWeakArray, int index);
   XYResult_t* (*get)(ShortWeakArray_t* self_ShortWeakArray, int index);
   void* payload;
 } ; //0x05

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* ShortWeakArray_add(ShortWeakArray_t* self_ShortWeakArray, XYObject_t* user_XYObject);
XYResult_t* ShortWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_toBytes(XYObject_t* user_XYObject);

#endif

// end of file shortweakarray.h
