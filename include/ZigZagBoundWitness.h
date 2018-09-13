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

  /**
   * Adds data to the bound witness and returns whats the party should send back.
   *
   * @param transfer The data from the other party.
   * @param endPoint If not already turned around, decide if what to send sign and send back.
   * @return A XyoBoundWitnessTransfer to send to the other party wrapped in a XYResult.
   */
   XYResult* (*incomingData)(ZigZagBoundWitness* self, BoundWitness* boundWitness, int endpoint);
   int (*addTransfer)(ZigZagBoundWitness* self, BoundWitness* boundWitness);
   int (*addIncomingKeys)(ZigZagBoundWitness* self, ShortStrongArray* incomingKeySets);
   int (*addIncomingPayload)(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads);
   int (*addIncomingSignatures)(ZigZagBoundWitness* self, ShortStrongArray* incomingSignatures);
   XYResult* (*makeSelfKeySet)(ZigZagBoundWitness* self);
   XYResult* (*signForSelf)(ZigZagBoundWitness* self);
   Signer* signer;
   XYObject* payload;
};

XYResult* incomingData(ZigZagBoundWitness* self, BoundWitness* boundWitness, int endpoint);
int addTransfer(ZigZagBoundWitness* self, BoundWitness* boundWitness);
int addIncomingKeys(ZigZagBoundWitness* self, ShortStrongArray* incomingKeySets);
int addIncomingPayload(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads);
int addIncomingSignatures(ZigZagBoundWitness* self, ShortStrongArray* incomingSignatures);
XYResult* makeSelfKeySet(ZigZagBoundWitness* self);

#define ZIGZAGBOUNDWITNES_H
#endif
