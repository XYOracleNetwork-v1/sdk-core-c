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

#ifndef OBJECTS_H
#define OBJECTS_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct XYObject XYObject_t;
typedef struct ByteStrongArray ByteStrongArray_t;
typedef struct ShortStrongArray ShortStrongArray_t;

struct ByteStrongArray{
  uint8_t  size;
  char     id[2];
  XYResult_t* (*add)(ByteStrongArray_t* self_ByteStrongArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(ByteStrongArray_t* self_ByteStrongArray, int index);
  XYResult_t* (*get)(ByteStrongArray_t* self_ByteStrongArray, int index);
  char* payload;
} ; //0x01

struct ShortStrongArray {
  uint16_t size;
  char     id[2];
  XYResult_t* (*add)(ShortStrongArray_t*  self_ShortStrongArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(ShortStrongArray_t*  self_ShortStrongArray, int index);
  XYResult_t* (*get)(ShortStrongArray_t*  self_ShortStrongArray, int index);
  void* payload;
} ; //0x02

typedef struct IntStrongArray IntStrongArray_t;
typedef struct ByteWeakArray ByteWeakArray_t;
typedef struct ShortWeakArray ShortWeakArray_t;
typedef struct IntWeakArray IntWeakArray_t;
typedef struct ByteArray ByteArray_t;
typedef struct NextPublicKey NextPublicKey;
typedef struct PreviousHash PreviousHash_t;
typedef struct Payload Payload_t;
typedef struct SignatureSet SignatureSet_t;
typedef struct NextPublicKey NextPublicKey_t;
typedef struct ECDSA_secp256k1_uncompressed ECDSA_secp256k1_uncompressed_t;
typedef struct secp256k1Signature secp256k1Signature_t;

struct IntStrongArray {
  uint32_t  size;
  char      id[2];
  XYResult_t* (*add)(IntStrongArray_t*  self_IntStrongArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(IntStrongArray_t*  self_IntStrongArray, int index);
  XYResult_t* (*get)(IntStrongArray_t*  self_IntStrongArray, int index);
  void* payload;
} ; //0x03

struct ByteWeakArray {
  uint8_t  size;
  XYResult_t* (*add)(ByteWeakArray_t* self_ByteWeakArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(ByteWeakArray_t* self_ByteWeakArray, int index);
  XYResult_t* (*get)(ByteWeakArray_t* self_ByteWeakArray, int index);
  void* payload;
} ; //0x04

struct ShortWeakArray {
  uint16_t size;
  XYResult_t* (*add)(ShortWeakArray_t* self_ShortWeakArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(ShortWeakArray_t* self_ShortWeakArray, int index);
  XYResult_t* (*get)(ShortWeakArray_t* self_ShortWeakArray, int index);
  void* payload;
} ; //0x05

struct IntWeakArray {
  uint32_t  size;
  XYResult_t* (*add)(IntWeakArray_t* self_IntWeakArray, XYObject_t* user_XYObject);
  XYResult_t* (*remove)(IntWeakArray_t* self_IntWeakArray, int index);
  XYResult_t* (*get)(IntWeakArray_t* self_IntWeakArray, int index);
  void* payload;
} ; //0x06

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

struct ByteArray{
  uint32_t size;
  char reserved[2];
  char* payload;
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


XYResult_t* ByteStrongArray_add(ByteStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);
XYResult_t* ByteWeakArray_add(ByteWeakArray_t* self_ByteWeakArray, XYObject_t* user_XYObject);
XYResult_t* ShortStrongArray_add(ShortStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);
XYResult_t* ShortWeakArray_add(ShortWeakArray_t* self_ShortWeakArray, XYObject_t* user_XYObject);
XYResult_t* IntStrongArray_add(IntStrongArray_t* self_IntStrongArray, XYObject_t* user_XYObject);
XYResult_t* IntWeakArray_add(IntWeakArray_t* self_IntWeakArray, XYObject_t* user_XYObject);


extern uint32_t to_uint32(unsigned char* data);
extern uint16_t to_uint16(unsigned char* data);

/* Standard Object IDs */
static const char ByteStrongArray_id[2]       = { 0x01, 0x01 };
static const char ShortStrongArray_id[2]      = { 0x01, 0x02 };
static const char IntStrongArray_id[2]        = { 0x01, 0x03 };
static const char ByteWeakArray_id[2]         = { 0x01, 0x04 };
static const char ShortWeakArray_id[2]        = { 0x01, 0x05 };
static const char IntWeakArray_id[2]          = { 0x01, 0x06 };

static const char BoundWitness_id[2]          = { 0x02, 0x01 };
static const char KeySet_id[2]                = { 0x02, 0x02 };
static const char SignatureSet_id[2]          = { 0x02, 0x03 };
static const char Payload_id[2]               = { 0x02, 0x04 };
static const char Index_id[2]                 = { 0x02, 0x05 };
static const char PreviousHash_id[2]          = { 0x02, 0x06 };
static const char NextPublicKey_id[2]         = { 0x02, 0x07 };
static const char BridgeHashSet_id[2]         = { 0x02, 0x08 };
static const char BridgeBlockSet_id[2]        = { 0x02, 0x09 };
static const char BoundWitnessTransfer_id[2]  = { 0x02, 0x0a };

static const char Sha256_id[2]                = { 0x03, 0x05 };
static const char ECDSASecp256k1_id[2]        = { 0x04, 0x01 };
static const char ECDSASecp256k1Sig_id[2]     = { 0x05, 0x01 };
static const char Rssi_id[2]                  = { 0x08, 0x01 };

#endif

// end of file xyobject.h
