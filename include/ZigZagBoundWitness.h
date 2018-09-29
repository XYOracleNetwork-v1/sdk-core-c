/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitness.h
 *
 * @XY4 project source code.
 *
 * @brief primary storage routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

//#include <stdint.h>
//#include "xyo.h"
//#include "XYOHeuristicsBuilder.h"
//#include "xyobject.h"
#include "crypto.h"
#include "BoundWitness.h"

#ifndef ZIGZAGBOUNDWITNESS_H
#define ZIGZAGBOUNDWITNESS_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct ZigZagBoundWitness ZigZagBoundWitness_2;

/*
 * STRUCTURES
 ****************************************************************************************
 */

typedef struct{
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
  
   XYResult_t* (*incomingData)(ZigZagBoundWitness_2* self, BoundWitnessTransfer* boundWitness, int endpoint);
   int (*addTransfer)(ZigZagBoundWitness_2* self, BoundWitnessTransfer* boundWitness);
   int (*addIncomingKeys)(ZigZagBoundWitness_2* self, ShortStrongArray_t* incomingKeySets);
   int (*addIncomingPayload)(ZigZagBoundWitness_2* self, IntStrongArray_t* incomingPayloads);
   int (*addIncomingSignatures)(ZigZagBoundWitness_2* self, ShortStrongArray_t* incomingSignatures);
   XYResult_t* (*makeSelfKeySet)(ZigZagBoundWitness_2* self);
   XYResult_t* (*signForSelf)(ZigZagBoundWitness_2* self);
   Signer* signer;
   XYObject_t* payload;
} ZigZagBoundWitness;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* incomingData(ZigZagBoundWitness* self, 
                         BoundWitnessTransfer* boundWitness, 
                         int endpoint);
int addTransfer(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness);
int addIncomingKeys(ZigZagBoundWitness* self, ShortStrongArray_t* incomingKeySets);
int addIncomingPayload(ZigZagBoundWitness* self, IntStrongArray_t* incomingPayloads);
int addIncomingSignatures(ZigZagBoundWitness* self, ShortStrongArray_t* incomingSignatures);
XYResult_t* makeSelfKeySet(ZigZagBoundWitness* self);

#endif

// end of file ZigZagBoundWitness.h

