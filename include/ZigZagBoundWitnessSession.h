#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include <stdint.h>
#include "xyo.h"
#include "ZigZagBoundWitness.h"
#include "network.h"

typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession;

struct ZigZagBoundWitnessSession {
  struct XYResult* (*completeBoundWitness)(ZigZagBoundWitnessSession* userSession, ByteArray* bwData);
  struct NetworkPipe* NetworkPipe;
  struct ZigZagBoundWitness* BoundWitness;
  uint8_t cycles;
  ByteArray* choice;
};

struct XYResult* receiverCallback(NetworkPipe* self, uint8_t cycles, XYResult* data);

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
