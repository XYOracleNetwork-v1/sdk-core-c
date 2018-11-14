/**
 ****************************************************************************************
 *
 * @file bytestrongarray.h
 *
 * @XY4 project source code.
 *
 * @brief primary bytestrongarray routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 ****************************************************************************************
 */

#ifndef BYTESTRONGARRAY_H
#define BYTESTRONGARRAY_H

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
typedef struct ByteStrongArray ByteStrongArray_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 struct ByteStrongArray{
   uint8_t  size;
   char     id[2];
   XYResult_t* (*add)(ByteStrongArray_t* self_ByteStrongArray, XYObject_t* user_XYObject);
   XYResult_t* (*remove)(ByteStrongArray_t* self_ByteStrongArray, int index);
   XYResult_t* (*get)(ByteStrongArray_t* self_ByteStrongArray, int index);
   char* payload;
 } ; //0x01

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
XYResult_t* ByteStrongArray_add(ByteStrongArray_t* self_ByteStrongArray,
                                  XYObject_t* user_XYObject); //TODO: consider changing self to XYObject
XYResult_t* ByteStrongArray_get(ByteStrongArray_t* self_ByteStrongArray, int index);
XYResult_t* ByteStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ByteStrongArray_creator_toBytes_t(XYObject_t* user_XYObject);
*/
XYResult_t* ByteStrongArray_add(ByteStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);
XYResult_t* ByteStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ByteStrongArray_creator_toBytes(XYObject_t* user_XYObject);


#endif

// end of file bytestrongarray.h
