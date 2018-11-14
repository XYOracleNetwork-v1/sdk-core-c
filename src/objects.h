/**
 ****************************************************************************************
 *
 * @file xyobject.h
 *
 * @XYO Core library source code.
 *
 * @brief primary xy object routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "xyresult.h"
#include "xyobject.h"
#include "arrays.h"
#include "ByteArray.h"
#include "include/objects/Payload.h"
#include "include/objects/Index.h"
#include "include/objects/PreviousHash.h"
#include "include/objects/NextPublicKey.h"
#include "include/objects/heuristics.h"
//#include "ZigZagBoundWitnessSession.h"


#ifndef OBJECTS_H
#define OBJECTS_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct NextPublicKey NextPublicKey;
typedef struct PreviousHash PreviousHash_t;
typedef struct Payload Payload_t;
typedef struct SignatureSet SignatureSet_t;
typedef struct NextPublicKey NextPublicKey_t;
typedef struct ECDSA_secp256k1_uncompressed ECDSA_secp256k1_uncompressed_t;
typedef struct secp256k1Signature secp256k1Signature_t;







typedef struct ShortWeakArray ShortWeakArray_t;
typedef struct IntWeakArray IntWeakArray_t;

struct KeySet{
  ShortWeakArray_t* keys;
} ;

struct SignatureSet{
  ShortWeakArray_t* signatures;
} ;

struct Payload{
  uint32_t size;
  IntWeakArray_t* signedHeuristics;
  IntWeakArray_t* unsignedHeuristics;
} ;

struct PreviousHash{
  char id[2];
  char* hash;
} ;

struct NextPublicKey{
  char id[2];
  char* publicKey;
} ;

struct ECDSA_secp256k1_uncompressed{
  char point_x[32];
  char point_y[32];
} ;

struct secp256k1Signature{
  uint8_t size;
  char* signature;
} ;

/*
 * STRUCTURES
 ****************************************************************************************
 */

/*
 * Our documentation uses the terminology of Multi or Single Element arrays.
 * A Multi element array is one which can contain many different types within it.
 * Each element in a Multi Element array has the following structure.
 *      char    id[2];
 *      void*   payload;
 * Each element in a sigle element array is only the payload. The major
 * and minor for the elements in the array are in the array object itself.
 * In this header file Multi-type element arrays will be referred to as
 * Weak arrays, and Single-type element array will be referred to as a
 * Strong array.
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
XYResult_t* registerType(char id[2], void* creator);

extern uint32_t to_uint32(unsigned char* data);
extern uint16_t to_uint16(unsigned char* data);

#endif

// end of file xyobject.h
