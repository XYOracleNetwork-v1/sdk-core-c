/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: XYOHeuristicsBuilder.h
 * @Last modified by:   Nate Brune
 * @Last modified time: 09-Aug-2018
 * @Copyright: XY - The Findables Company
 */


#ifndef XYOHeuristicsBuilder_H
#include "xyo.h"
#include "hash.h"
#include "BoundWitness.h"
/* todo */
void breakpoint(void);

XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi);
XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObect);

XYResult* Heuristic_Text_Creator_create(char id[2], void* user_data);
XYResult* Heuristic_Text_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_Text_Creator_toBytes(struct XYObject* user_XYObect);

XYResult* NextPublicKey_creator_create(char id[2], void* user_data);
XYResult* NextPublicKey_creator_fromBytes(char* pubkey_data);
XYResult* NextPublicKey_creator_toBytes(struct XYObject* user_XYObect);

XYResult* ECDSA_secp256k1Uncompressed_creator_create(char id[2], void* user_data);
XYResult* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* heuristic_data);
XYResult* ECDSA_secp256k1Uncompressed_creator_toBytes(struct XYObject* user_XYObect);

XYResult* PreviousHash_creator_create(char id[2], void* user_data);
XYResult* PreviousHash_creator_fromBytes(char* hash_data);
XYResult* PreviousHash_creator_toBytes(struct XYObject* user_XYObect);

XYResult* Index_creator_create(char id[2], void* user_data);
XYResult* Index_creator_fromBytes(char* index_data);
XYResult* Index_creator_toBytes(struct XYObject* user_XYObect);

XYResult* Payload_creator_create(char id[2], void* user_data);
XYResult* Payload_creator_fromBytes(char* payload_data);
XYResult* Payload_creator_toBytes(struct XYObject* user_XYObect);

XYResult* ByteStrongArray_creator_create(char id[2], void* user_data);
XYResult* ByteStrongArray_creator_fromBytes(char* data);
XYResult* ByteStrongArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* ByteStrongArray_add(ByteStrongArray* self_ShortStrongArray, XYObject* user_XYObject);

XYResult* ShortStrongArray_creator_create(char id[2], void* user_data);
XYResult* ShortStrongArray_creator_fromBytes(char* data);
XYResult* ShortStrongArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* ShortStrongArray_add(ShortStrongArray* self_ShortStrongArray, XYObject* user_XYObject);

XYResult* IntStrongArray_creator_create(char id[2], void* user_data);
XYResult* IntStrongArray_creator_fromBytes(char* data);
XYResult* IntStrongArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* IntStrongArray_add(IntStrongArray* self_IntStrongArray, XYObject* user_XYObject);

XYResult* IntWeakArray_creator_create(char[2], void* user_data);
XYResult* IntWeakArray_creator_fromBytes(char* data);
XYResult* IntWeakArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* IntWeakArray_add(IntWeakArray* self_IntWeakArray, XYObject* user_XYObject);

XYResult* ShortWeakArray_creator_create(char[2], void* user_data);
XYResult* ShortWeakArray_creator_fromBytes(char* data);
XYResult* ShortWeakArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* ShortWeakArray_add(ShortWeakArray* self_ShortWeakArray, XYObject* user_XYObject);

XYResult* ByteWeakArray_creator_create(char[2], void* user_data);
XYResult* ByteWeakArray_creator_fromBytes(char* data);
XYResult* ByteWeakArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* ByteWeakArray_add(ByteWeakArray* self_ByteWeakArray, XYObject* user_XYObject);

#define XYOHeuristicsBuilder_H
#endif
