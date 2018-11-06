#include "xyresult.h"
#include "defines.h"

#ifndef XYOBJECTS_H
#define XYOBJECTS_H
extern XYResult_t preallocated_return_result;

/*
 * STRUCTURES
 ****************************************************************************************
 */

typedef struct XYObject{
 char id[2];
 void* payload;
 XYResult_t* (*GetXyobjectId)(struct XYObject*);  // Fetch the above id object and return it.
 XYResult_t* (*GetPayload)(struct XYObject*);     // Fetch the above payload pointer object and return it.
} XYObject_t ;

typedef struct ObjectProvider {
   int        sizeIdentifierSize;
   int        defaultSize;
   XYResult_t*  (*create)(char[2], void*);
   XYResult_t*  (*fromBytes)(char*);
   XYResult_t*  (*toBytes)(XYObject_t*);
} ObjectProvider_t;

extern void* typeTable[TYPE_TABLE_MAJOR_MAX][TYPE_TABLE_MINOR_MAX];



XYResult_t* getXyobjectId(XYObject_t* xyobject);
XYResult_t* getPayload(XYObject_t* xyobject);
/*
XYResult_t* ByteStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ShortStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortStrongArray_creator_fromBytes(char* data);
XYResult_t* ShortStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* IntStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntStrongArray_creator_fromBytes(char* data);
XYResult_t* IntStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ByteWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ByteStrongArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* ByteWeakArray_creator_toBytes(XYObject_t* user_XYObject);
XYResult_t* IntWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntWeakArray_creator_fromBytes(char* data);
XYResult_t* IntWeakArray_creator_toBytes(XYObject_t* user_XYObject);
*/
XYResult_t* Heuristic_RSSI_Creator_create(const char id[2], void* rssi);
XYResult_t* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult_t* Heuristic_RSSI_Creator_toBytes(XYObject_t* user_XYObject);

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
XYResult_t* ECDSA_secp256k1Sig_creator_toBytes(XYObject_t* user_XYObject);

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
XYResult_t* Heuristic_sha256_Creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ByteStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteStrongArray_creator_fromBytes(char* data);
XYResult_t* ByteStrongArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ShortStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortStrongArray_creator_fromBytes(char* data);
XYResult_t* ShortStrongArray_creator_toBytes(XYObject_t* user_XYObject);


XYResult_t* IntStrongArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntStrongArray_creator_fromBytes(char* data);
XYResult_t* IntStrongArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* BlockSet_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* IntWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* IntWeakArray_creator_fromBytes(char* data);
XYResult_t* IntWeakArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ShortWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ShortWeakArray_creator_fromBytes(char* data);
XYResult_t* ShortWeakArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* ByteWeakArray_creator_create(const char id[2], void* user_data);
XYResult_t* ByteWeakArray_creator_fromBytes(char* data);
XYResult_t* ByteWeakArray_creator_toBytes(XYObject_t* user_XYObject);

XYResult_t* BoundWitnessTransfer_create(const char id[2], void* user_data);
XYResult_t* BoundWitnessTransfer_fromBytes(char* user_Transfer);
XYResult_t* BoundWitnessTransfer_toBytes(XYObject_t* user_XYObect);

XYResult_t* BoundWitness_creator_create(const char id[2], void* user_data);
XYResult_t* BoundWitness_creator_fromBytes(char* BoundWitness_data);
XYResult_t* BoundWitness_creator_toBytes(XYObject_t* user_XYObect);

XYResult_t* newObject(const char id[2], void* payload);
XYResult_t* initTable(void);
XYResult_t* tableLookup(const char id[2]);

#endif
