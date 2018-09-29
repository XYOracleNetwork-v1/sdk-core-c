/**
 ****************************************************************************************
 *
 * @file XYOHeuristicsBuilder.h
 *
 * @XY4 project source code.
 *
 * @brief primary XYOHeuristicsBuilder routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef XYOHeuristicsBuilder_H
#define XYOHeuristicsBuilder_H

/*
 * INCLUDES
 ****************************************************************************************
 */

//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include "xyobject.h"
//#include "xyo.h"
//#include "hash.h"
//#include "BoundWitness.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
 
uint32_t to_uint32(char* data);

//TODO: 

void breakpoint(void);

XYResult_t* Heuristic_RSSI_Creator_create(char id[2], void* rssi);
XYResult_t* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_RSSI_Creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* Heuristic_Text_Creator_create(char id[2], void* user_data);
XYResult_t* Heuristic_Text_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_Text_Creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* NextPublicKey_creator_create(char id[2], void* user_data);
XYResult_t* NextPublicKey_creator_fromBytes(char* pubkey_data);
XYResult_t* NextPublicKey_creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* ECDSA_secp256k1Uncompressed_creator_create(char id[2], void* user_data);
XYResult_t* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* heuristic_data);
XYResult_t* ECDSA_secp256k1Uncompressed_creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* PreviousHash_creator_create(char id[2], void* user_data);
XYResult_t* PreviousHash_creator_fromBytes(char* hash_data);
XYResult_t* PreviousHash_creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* Index_creator_create(char id[2], void* user_data);
XYResult_t* Index_creator_fromBytes(char* index_data);
XYResult_t* Index_creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* Payload_creator_create(char id[2], void* user_data);
XYResult_t* Payload_creator_fromBytes(char* payload_data);
XYResult_t* Payload_creator_toBytes(XYResult_t* user_XYObect);

XYResult_t* ByteStrongArray_creator_create(char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ByteStrongArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* ByteStrongArray_add(ByteStrongArray_t* self_ShortStrongArray, XYResult_t* user_XYObject);

XYResult_t* ShortStrongArray_creator_create(char id[2], void* user_data);
XYResult_t* ShortStrongArray_creator_fromBytes(char* data);
XYResult_t* ShortStrongArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* ShortStrongArray_add(ShortStrongArray_t* self_ShortStrongArray, XYResult_t* user_XYObject);

XYResult_t* IntStrongArray_creator_create(char id[2], void* user_data);
XYResult_t* IntStrongArray_creator_fromBytes(char* data);
XYResult_t* IntStrongArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* IntStrongArray_add(IntStrongArray_t* self_IntStrongArray, XYResult_t* user_XYObject);

XYResult_t* IntWeakArray_creator_create(char[2], void* user_data);
XYResult_t* IntWeakArray_creator_fromBytes(char* data);
XYResult_t* IntWeakArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* IntWeakArray_add(IntWeakArray_t* self_IntWeakArray, XYResult_t* user_XYObject);

XYResult_t* ShortWeakArray_creator_create(char[2], void* user_data);
XYResult_t* ShortWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* ShortWeakArray_add(ShortWeakArray_t* self_ShortWeakArray, XYResult_t* user_XYObject);

XYResult_t* ByteWeakArray_creator_create(char[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ByteWeakArray_creator_toBytes(XYResult_t* user_XYObect);
XYResult_t* ByteWeakArray_add(ByteWeakArray_t* self_ByteWeakArray, XYResult_t* user_XYObject);

#endif

// end of file XYOHeuristicsBuilder.h

