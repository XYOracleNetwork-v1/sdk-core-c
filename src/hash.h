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
#include "XYResult.h"
#include "ByteArray.h"
#include "XYObjects/XYObject.h"
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
typedef unsigned char  byte;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t newHashProvider(void);
XYResult_t getHashId( void );
XYResult_t createHash(ByteArray_t* dataToHash);
XYResult_t verifyHash(ByteArray_t* dataHashed, XYObject_t* hash);
extern void trng_acquire( uint8_t* );

#endif

// end of file hash.h
