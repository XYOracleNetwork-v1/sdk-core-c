/**
 ****************************************************************************************
 *
 * @file hash.h
 *
 * @XY4 project source code.
 *
 * @brief primary hash routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef HASH_H
#define HASH_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "objects.h"
#include "WolfSSL/wc_sha256.h"                      // wc_ = wolf crypto library routine (SHA-256)

extern XYResult_t preallocated_return_result;

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
  XYResult_t* (*getHashId)(HashProvider_t* hashProviderObject);   // Fetch the above id object
                                                                  // and return it.
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newHashProvider(void);
XYResult_t* hashGetId(HashProvider_t* hashProviderObject);
XYResult_t* createHash(ByteArray_t* dataToHash);
XYResult_t* verifyHash(ByteArray_t* dataHashed, XYObject_t* hash);

XYResult_t* Heuristic_sha256_Creator_create(const char id[2], void* sha256);
XYResult_t* Heuristic_sha256_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_sha256_Creator_toBytes(XYObject_t* user_XYObject);

#endif

// end of file hash.h
