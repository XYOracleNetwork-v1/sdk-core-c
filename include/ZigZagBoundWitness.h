#ifndef ZIGZAGBOUNDWITNESS_H
#include <stdint.h>
#include "xyo.h"
#include "xyobject.h"
#include "BoundWitness.h"

typedef struct ZigZagBoundWitness ZigZagBoundWitness;

struct ZigZagBoundWitness {
  BoundWitness* boundWitness;
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
   struct XYResult* (*incomingData)(ZigZagBoundWitness* self, BoundWitness* boundWitness, int endpoint);
   int (*addTransfer)(ZigZagBoundWitness* self, BoundWitness* boundWitness);
   int (*addIncomingKeys)(ZigZagBoundWitness* self, XYObject* incomingKeySets);
   int (*addIncomingPayload)(ZigZagBoundWitness* self, XYObject* incomingPayloads);
   int (*addIncomingSignatures)(ZigZagBoundWitness* self, XYObject* incomingSignatures);
   struct XYObject* (*makeSelfKeySet)(ZigZagBoundWitness* self);
   struct XYResult* (*signBoundWitness)(ZigZagBoundWitness* self);
   Signer* signer;
   XYObject* payload;
};

#define ZIGZAGBOUNDWITNES_H
#endif
