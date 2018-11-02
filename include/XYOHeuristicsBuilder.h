/**
 ****************************************************************************************
 *
 * @file XYOHeuristicsBuilder.h
 *
 * @XYO Core library source code.
 *
 * @brief primary XYOHeuristicsBuilder routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef XYOHeuristicsBuilder_H
#define XYOHeuristicsBuilder_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "xyobject.h"

extern XYResult_t preallocated_return_result;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

int littleEndian(void);

void breakpoint(void);

XYResult_t* Heuristic_RSSI_Creator_create(const char id[2], void* rssi);
XYResult_t* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObject);

XYResult_t* Heuristic_Text_Creator_create(const char id[2], void* user_data);
XYResult_t* Heuristic_Text_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_Text_Creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* NextPublicKey_creator_create(const char id[2], void* user_data);
XYResult_t* NextPublicKey_creator_fromBytes(char* pubkey_data);
XYResult_t* NextPublicKey_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ECDSA_secp256k1Uncompressed_creator_create(const char id[2], void* user_data);
XYResult_t* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* heuristic_data);
XYResult_t* ECDSA_secp256k1Uncompressed_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ECDSA_secp256k1Sig_creator_create(const char id[2], void* text);
XYResult_t* ECDSA_secp256k1Sig_creator_fromBytes(char* heuristic_data);
XYResult_t* ECDSA_secp256k1Sig_creator_toBytes(struct XYObject* user_XYObject);

XYResult_t* PreviousHash_creator_create(const char id[2], void* user_data);
XYResult_t* PreviousHash_creator_fromBytes(char* hash_data);
XYResult_t* PreviousHash_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* Index_creator_create(const char id[2], void* user_data);
XYResult_t* Index_creator_fromBytes(char* index_data);
XYResult_t* Index_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* Payload_creator_create(const char id[2], void* user_data);
XYResult_t* Payload_creator_fromBytes(char* payload_data);
XYResult_t* Payload_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* Heuristic_sha256_Creator_create(const char id[2], void* sha256);
XYResult_t* Heuristic_sha256_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_sha256_Creator_toBytes(struct XYObject* user_XYObject);

XYResult_t* ByteStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ByteStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ByteStrongArray_add(ByteStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);

XYResult_t* ShortStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortStrongArray_creator_fromBytes(char* data);
XYResult_t* ShortStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ShortStrongArray_add(ShortStrongArray_t* self_ShortStrongArray, XYObject_t* user_XYObject);

XYResult_t* IntStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntStrongArray_creator_fromBytes(char* data);
XYResult_t* IntStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* IntStrongArray_add(IntStrongArray_t* self_IntStrongArray, XYObject_t* user_XYObject);

XYResult_t* BlockSet_creator_toBytes(struct XYObject* user_XYObject);

XYResult_t* IntWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntWeakArray_creator_fromBytes(char* data);
XYResult_t* IntWeakArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* IntWeakArray_add(IntWeakArray_t* self_IntWeakArray, XYObject_t* user_XYObject);

XYResult_t* ShortWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ShortWeakArray_add(ShortWeakArray_t* self_ShortWeakArray, XYObject_t* user_XYObject);

XYResult_t* ByteWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ByteWeakArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ByteWeakArray_add(ByteWeakArray_t* self_ByteWeakArray, XYObject_t* user_XYObject);

#define XYOHeuristicsBuilder_H
#endif

// end of file XYOHeuristicsBuilder.h
