/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.h
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

#ifndef ZIGZAGBOUNDWITNESS_H
#include <stdint.h>
#include "xyo.h"
#include "xyobject.h"
#include "crypto.h"
#include "BoundWitness.h"
#include "XYOHeuristicsBuilder.h"

typedef struct ZigZagBoundWitness ZigZagBoundWitness;

struct ZigZagBoundWitness {
  BoundWitnessTransfer* boundWitness;
  ShortStrongArray* dynamicPublicKeys;
  IntStrongArray* dynamicPayloads;
  ShortStrongArray* dynamicSignatures;
  int hasSentKeysAndPayload;
  int heuristicCount;
  /**
   * Adds data to the bound witness and returns whats the party should send back.
   *
   * @param transfer The data from the other party.
   * @param endPoint If not already turned around, decide if what to send sign and send back.
   * @return A XyoBoundWitnessTransfer to send to the other party wrapped in a XYResult.
   */
   XYResult* (*incomingData)(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness, int endpoint);
   int (*addTransfer)(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness);
   int (*addIncomingKeys)(ZigZagBoundWitness* self, ShortStrongArray* incomingKeySets);
   int (*addIncomingPayload)(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads);
   int (*addIncomingSignatures)(ZigZagBoundWitness* self, ShortStrongArray* incomingSignatures);
   XYResult* (*makeSelfKeySet)(ZigZagBoundWitness* self);
   XYResult* (*signForSelf)(ZigZagBoundWitness* self);
   Signer* signer;
   XYObject* payload[0];
};

XYResult* incomingData(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness, int endpoint);
int addTransfer(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness);
int addIncomingKeys(ZigZagBoundWitness* self, ShortStrongArray* incomingKeySets);
int addIncomingPayload(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads);
int addIncomingSignatures(ZigZagBoundWitness* self, ShortStrongArray* incomingSignatures);
XYResult* makeSelfKeySet(ZigZagBoundWitness* self);
XYResult* signForSelf(ZigZagBoundWitness* self);

#define ZIGZAGBOUNDWITNES_H
#endif
