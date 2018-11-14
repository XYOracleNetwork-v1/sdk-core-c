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

#ifndef INTWEAKARRAY_H
#define INTWEAKARRAY_H

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
typedef struct IntWeakArray IntWeakArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 struct IntWeakArray {
   uint32_t  size;
   XYResult_t* (*add)(IntWeakArray_t* self_IntWeakArray, XYObject_t* user_XYObject);
   XYResult_t* (*remove)(IntWeakArray_t* self_IntWeakArray, int index);
   XYResult_t* (*get)(IntWeakArray_t* self_IntWeakArray, int index);
   void* payload;
 } ; //0x06

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
XYResult_t* IntWeakArray_add(IntWeakArray_t* self_IntWeakArray, XYObject_t* user_XYObject);
XYResult_t* IntWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntWeakArray_creator_fromBytes(char* data);
XYResult_t* IntWeakArray_creator_toBytes(XYObject_t* user_XYObject);

#endif

// end of file intweakarray.h
