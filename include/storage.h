/**
 ****************************************************************************************
 *
 * @file storage.h
 *
 * @XY4 project source code.
 *
 * @brief primary storage routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "xyobject.h"
#include "xyo.h"

#ifndef STORAGE_H
#define STORAGE_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct StorageProvider StorageProvider_t;

extern XYResult_t preallocated_return_result;
extern XYResult_t* preallocated_return_result_ptr;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct StorageProvider{
  XYResult_t* (*write)(ByteArray_t*, ByteArray_t*);     // Insert into dictionary
  XYResult_t* (*read)(ByteArray_t*);                    // Get from dictionary
  XYObject_t* (*GetAllKeys)();                          // Get all keys in dictionary TODO
  XYResult_t* (*delete)(ByteArray_t*);                  // Remove element from hashmap TODO
  XYResult_t* (*contains)(ByteArray_t*, void (*f)(ByteArray_t*));   // Is element in hashmap TODO
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newStorageProvider(void);

#endif

// end of file storage.h


