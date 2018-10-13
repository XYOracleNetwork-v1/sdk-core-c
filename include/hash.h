/**
 ****************************************************************************************
 *
 * @file hash.h
 *
 * @XY4 project source code.
 *
 * @brief primary hash routines for the XY4 firmware.
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
#include "wc_sha256.h"                // wc_ = wolf crypto library routine (SHA-256)

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct HashProvider HashProvider_t;
typedef unsigned char  byte;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct HashProvider{
  
  char id[2];                               // 2 bytes representing major and minor
  XYResult_t* (*createHash)(ByteArray_t*);  // Given just a null terminated char* return a 
                                            // cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  
  XYResult_t* (*verifyHash)(ByteArray_t* dataHashed, XYObject_t* hash);
  char* (*getHashId)(HashProvider_t* hashProviderObject);   // Fetch the above id object 
                                                            // and return it.
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

HashProvider_t* newHashProvider(void);
char* hashGetId(HashProvider_t* hashProviderObject);
XYResult_t* createHash(ByteArray_t* dataToHash);
XYResult_t* verifyHash(ByteArray_t* dataHashed, XYObject_t* hash);

#endif

// end of file hash.h

