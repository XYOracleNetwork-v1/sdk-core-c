/**
 ****************************************************************************************
 *
 * @file hash.h
 *
 * @XY4 project source code.
 *
 * @brief primary crypto routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef HASH_H
#define HASH_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "xyobject.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct HashProvider HashProvider;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct HashProvider{
  
  char id[2];                         // 2 bytes representing major and minor
  XYResult_t* (*hash)(ByteArray_t*);  // Given just a null terminated char* return a 
                                      // cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  
  int (*verifyHash)(ByteArray_t* hash, ByteArray_t* data);
  char* (*getHashId)(HashProvider* hashProviderObject);   // Fetch the above id object 
                                                          // and return it.
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

char* hashGetId(HashProvider* hashProviderObject);

#endif

// end of file hash.h

