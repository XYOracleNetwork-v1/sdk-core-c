/**
 ****************************************************************************************
 *
 * @file XYOHeuristicsBuilder.h
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


#ifndef XYOHeuristicsBuilder_H
#include "xyo.h"
#include "hash.h"
#include "BoundWitness.h"
#include "defines.h"
#include <stdlib.h>
#include <string.h>

/* todo */
void breakpoint(void);

XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi);
XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObject);

XYResult* Heuristic_Text_Creator_create(char id[2], void* user_data);
XYResult* Heuristic_Text_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_Text_Creator_toBytes(struct XYObject* user_XYObject);

XYResult* NextPublicKey_creator_create(char id[2], void* user_data);
XYResult* NextPublicKey_creator_fromBytes(char* pubkey_data);
XYResult* NextPublicKey_creator_toBytes(struct XYObject* user_XYObject);

XYResult* ECDSA_secp256k1Uncompressed_creator_create(char id[2], void* user_data);
XYResult* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* heuristic_data);
XYResult* ECDSA_secp256k1Uncompressed_creator_toBytes(struct XYObject* user_XYObject);

XYResult* ECDSA_secp256k1Sig_creator_create(char id[2], void* text);
XYResult* ECDSA_secp256k1Sig_creator_fromBytes(char* heuristic_data);
XYResult* ECDSA_secp256k1Sig_creator_toBytes(struct XYObject* user_XYObject);

XYResult* PreviousHash_creator_create(char id[2], void* user_data);
XYResult* PreviousHash_creator_fromBytes(char* hash_data);
XYResult* PreviousHash_creator_toBytes(struct XYObject* user_XYObject);

XYResult* Index_creator_create(char id[2], void* user_data);
XYResult* Index_creator_fromBytes(char* index_data);
XYResult* Index_creator_toBytes(struct XYObject* user_XYObject);

XYResult* Payload_creator_create(char id[2], void* user_data);
XYResult* Payload_creator_fromBytes(char* payload_data);
XYResult* Payload_creator_toBytes(struct XYObject* user_XYObject);

XYResult* Heuristic_sha256_Creator_create(char id[2], void* sha256);
XYResult* Heuristic_sha256_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_sha256_Creator_toBytes(struct XYObject* user_XYObject);


XYResult* ByteStrongArray_creator_create(char id[2], void* user_data);
XYResult* ByteStrongArray_creator_fromBytes(char* data);
XYResult* ByteStrongArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* ByteStrongArray_add(ByteStrongArray* self_ShortStrongArray, XYObject* user_XYObject);

XYResult* ShortStrongArray_creator_create(char id[2], void* user_data);
XYResult* ShortStrongArray_creator_fromBytes(char* data);
XYResult* ShortStrongArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* ShortStrongArray_add(ShortStrongArray* self_ShortStrongArray, XYObject* user_XYObject);

XYResult* IntStrongArray_creator_create(char id[2], void* user_data);
XYResult* IntStrongArray_creator_fromBytes(char* data);
XYResult* IntStrongArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* IntStrongArray_add(IntStrongArray* self_IntStrongArray, XYObject* user_XYObject);

XYResult* IntWeakArray_creator_create(char[2], void* user_data);
XYResult* IntWeakArray_creator_fromBytes(char* data);
XYResult* IntWeakArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* IntWeakArray_add(IntWeakArray* self_IntWeakArray, XYObject* user_XYObject);

XYResult* ShortWeakArray_creator_create(char[2], void* user_data);
XYResult* ShortWeakArray_creator_fromBytes(char* data);
XYResult* ShortWeakArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* ShortWeakArray_add(ShortWeakArray* self_ShortWeakArray, XYObject* user_XYObject);

XYResult* ByteWeakArray_creator_create(char[2], void* user_data);
XYResult* ByteWeakArray_creator_fromBytes(char* data);
XYResult* ByteWeakArray_creator_toBytes(struct XYObject* user_XYObject);
XYResult* ByteWeakArray_add(ByteWeakArray* self_ByteWeakArray, XYObject* user_XYObject);

#define XYOHeuristicsBuilder_H
#endif
