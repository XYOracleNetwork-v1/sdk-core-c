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

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * STRUCTURES
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* ByteWeakArray_add_t(ByteWeakArray_t* self_ByteWeakArray, 
                                XYObject_t* user_XYObject); 
XYResult_t* ByteWeakArray_get(ByteWeakArray_t* self_ByteWeakArray, int index);
XYResult_t* ByteWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ByteWeakArray_creator_toBytes_t(struct XYObject_t* user_XYObject);


#endif

// end of file byteweakarray.h
                                
                                

