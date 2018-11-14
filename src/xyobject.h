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

XYResult_t* newObject(const char id[2], void* payload);
XYResult_t* initTable(void);
XYResult_t* tableLookup(const char id[2]);

#endif
