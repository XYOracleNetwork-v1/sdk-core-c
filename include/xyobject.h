/**
 ****************************************************************************************
 *
 * @file xyobject.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef XYOBJECT_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

typedef struct XYObject XYObject;
typedef struct XYResult XYResult;
typedef struct ByteStrongArray ByteStrongArray;
typedef struct ShortStrongArray ShortStrongArray;
typedef struct IntStrongArray IntStrongArray;
typedef struct IntWeakArray IntWeakArray;
typedef struct ShortWeakArray ShortWeakArray;
typedef struct ByteWeakArray ByteWeakArray;
typedef struct NextPublicKey NextPublicKey;
typedef struct PreviousHash PreviousHash;
typedef struct Payload Payload;
typedef struct SignatureSet SignatureSet;
typedef struct ECDSA_secp256k1_uncompressed ECDSA_secp256k1_uncompressed;
typedef struct secp256k1Signature secp256k1Signature;
typedef struct ByteArray ByteArray;
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

struct ByteStrongArray {
  uint8_t  size;
  char     id[2];
  XYResult* (*add)(struct ByteStrongArray* self_ByteStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ByteStrongArray* self_ByteStrongArray, int index);
  XYResult* (*get)(struct ByteStrongArray* self_ByteStrongArray, int index);
  char* payload;
} ; //0x01

struct ShortStrongArray {
  uint16_t size;
  char     id[2];
  XYResult* (*add)(struct ShortStrongArray* self_ShortStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ShortStrongArray* self_ShortStrongArray, int index);
  XYResult* (*get)(struct ShortStrongArray* self_ShortStrongArray, int index);
  void* payload;
} ; //0x02

struct IntStrongArray {
  uint32_t  size;
  char      id[2];
  XYResult* (*add)(struct IntStrongArray* self_IntStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct IntStrongArray* self_IntStrongArray, int index);
  XYResult* (*get)(struct IntStrongArray* self_IntStrongArray, int index);
  void* payload;
} ; //0x03

struct ByteWeakArray {
  uint8_t  size;
  XYResult* (*add)(struct ByteWeakArray* self_ByteWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ByteWeakArray* self_ByteWeakArray, int index);
  XYResult* (*get)(struct ByteWeakArray* self_ByteWeakArray, int index);
  void* payload;
} ; //0x04

struct ShortWeakArray {
  uint16_t size;
  XYResult* (*add)(struct ShortWeakArray* self_ShortWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ShortWeakArray* self_ShortWeakArray, int index);
  XYResult* (*get)(struct ShortWeakArray* self_ShortWeakArray, int index);
  void* payload;
} ; //0x05

struct IntWeakArray {
  uint32_t  size;
  XYResult* (*add)(struct IntWeakArray* self_IntWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct IntWeakArray* self_IntWeakArray, int index);
  XYResult* (*get)(struct IntWeakArray* self_IntWeakArray, int index);
  void* payload;
} ; //0x06

enum EXyoErrors{
  OK,
  ERR_CRITICAL, // Catastrophic failure.
  ERR_NOID, // Returned when the core can't get the ID.
  ERR_CANT_GET_PAYLOAD, // Returned when the payload in inaccesible.
  ERR_NOSIGNER, // Returned when the core can't create a signer.
  ERR_ADDRESS_UNAVAILABLE, // Could not bind to address provided.
  ERR_NETWORK_UNAVAILABLE, // Core network services are unavailable.
  ERR_RECEIVER_REFUSED_CONNECTION, // Returned when The receiver refused connection.
  ERR_BUSY, // Returned when a core service is busy.
  ERR_NOKEYS, // Returned by when no keypair has been generated.
  ERR_BADDATA, // Returned if data is malformed e.g. too big.
  ERR_BADPUBKEY, // Returned if the public key is invalid.
  ERR_BADSIG, // Returned if the signature encoding is improper.
  ERR_CORRUPTDATA, // Returned if data is improperly encrypdefaultSize = ted.
  ERR_KEY_ALREADY_EXISTS, // Returned if can't insert because key is already mapped.
  ERR_INSUFFICIENT_MEMORY, // Returned if there wasn't enough memory to store.
  ERR_INTERNAL_ERROR, // Returned if there was a hardware error.
  ERR_TIMEOUT, // Returned if the disk timed out on read/write.
  ERR_COULD_NOT_DELETE, // Returned if delete failed.
  ERR_PERMISSION, // Returned if permissions are improper.
  ERR_KEY_DOES_NOT_EXIST // Returned if key isn't found in map.
};

struct XYResult{
  enum EXyoErrors error;
  void* result;
};

struct KeySet{
  struct ShortWeakArray* keys;
};

struct SignatureSet{
  struct ShortWeakArray* signatures;
};

struct Payload{
  uint32_t size;
  struct IntWeakArray* signedHeuristics;
  struct IntWeakArray* unsignedHeuristics;
};

struct PreviousHash{
  char id[2];
  char* hash;
};

struct NextPublicKey{
  char id[2];
  char* publicKey;
};

struct ECDSA_secp256k1_uncompressed{
  char point_x[32];
  char point_y[32];
};

struct secp256k1Signature{
  uint8_t size;
  char* signature;
};

struct ByteArray{
  uint32_t size;
  char reserved[2];
  char* payload;
};


struct XYObject{
  char id[2];
  void* payload;
  char* (*GetId)(struct XYObject*); // Fetch the above id object and return it.
  void* (*GetPayload)(struct XYObject*); // Fetch the above payload pointer object and return it.
};

extern struct XYResult* preallocated_result;
extern void* typeTable[TYPE_TABLE_MAJOR_MAX][TYPE_TABLE_MINOR_MAX];

struct XYResult* newObject(char id[2], void* payload);
struct XYResult* lookup(char id[2]);
struct XYResult* initTable(void);
uint16_t to_uint16(char* data);
uint32_t to_uint32(char* data);

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
static const char BoundWitnessTransfer_id[2]  = { 0x02, 0x01 };

static const char Sha256_id[2]                = { 0x03, 0x05 };
static const char ECDSASecp256k1_id[2]        = { 0x04, 0x01 };
static const char ECDSASecp256k1Sig_id[2]     = { 0x05, 0x01 };
static const char Rssi_id[2]                  = { 0x08, 0x01 };

typedef struct ObjectProvider ObjectProvider;

 struct ObjectProvider {
   int        sizeIdentifierSize;
   int        defaultSize;
   struct XYResult*  (*create)(char[2], void*);
   struct XYResult*  (*fromBytes)(char*);
   struct XYResult*  (*toBytes)(struct XYObject*);
 };

#define XYOBJECT_H
#endif
