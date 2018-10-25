/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitness.h
 *
 * @XY4 project source code.
 *
 * @brief primary ZigZagBoundWitness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "crypto.h"
#include "BoundWitness.h"   // includes "xyobject.h", "hash.h"

//#ifndef ZIGZAGBOUNDWITNESS_H
//#define ZIGZAGBOUNDWITNESS_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct ZigZagBoundWitness ZigZagBoundWitness_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct ZigZagBoundWitness{
  BoundWitnessTransfer* boundWitness;
  ShortStrongArray_t* dynamicPublicKeys;
  IntStrongArray_t* dynamicPayloads;
  ShortStrongArray_t* dynamicSignatures;
  int hasSentKeysAndPayload;

  /**
   * Adds data to the bound witness and returns what the party should send back.
   *
   * @param transfer The data from the other party.
   * @param endPoint If not already turned around, decide if what to send sign and send back.
   * @return A XyoBoundWitnessTransfer to send to the other party wrapped in a XYResult.
   */
  
   XYResult_t* (*incomingData)(ZigZagBoundWitness_t* self, BoundWitnessTransfer* boundWitness, int endpoint);
   XYResult_t*/*int*/ (*addTransfer)(ZigZagBoundWitness_t* self, BoundWitnessTransfer* boundWitness);
   XYResult_t*/*int*/ (*addIncomingKeys)(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingKeySets);
   XYResult_t*/*int*/ (*addIncomingPayload)(ZigZagBoundWitness_t* self, IntStrongArray_t* incomingPayloads);
   XYResult_t*/*int*/ (*addIncomingSignatures)(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingSignatures);
   XYResult_t* (*makeSelfKeySet)(ZigZagBoundWitness_t* self);
   XYResult_t* (*signForSelf)(ZigZagBoundWitness_t* self);
   Signer_t* signer;
   XYObject_t* payload;
} ;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* incomingData(ZigZagBoundWitness_t* self, 
                         BoundWitnessTransfer* boundWitness, 
                         int endpoint);
XYResult_t*/*int*/ addTransfer(ZigZagBoundWitness_t* self, BoundWitnessTransfer* boundWitness);
XYResult_t*/*int*/ addIncomingKeys(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingKeySets);
XYResult_t*/*int*/ addIncomingPayload(ZigZagBoundWitness_t* self, IntStrongArray_t* incomingPayloads);
XYResult_t*/*int*/ addIncomingSignatures(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingSignatures);
XYResult_t* makeSelfKeySet(ZigZagBoundWitness_t* self);

//#endif

// end of file ZigZagBoundWitness.h

