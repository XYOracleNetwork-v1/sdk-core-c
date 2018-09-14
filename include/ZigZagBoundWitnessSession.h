#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include <stdint.h>
#include "xyo.h"
#include "ZigZagBoundWitness.h"

typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession;

struct ZigZagBoundWitnessSession {
  struct XYResult* (*completeBoundWitness)(ZigZagBoundWitnessSession* userSession, ByteArray* bwData);
  struct NetworkPipe* NetworkPipe;
  struct ZigZagBoundWitness* BoundWitness;
  int cycles;
  ByteArray* choice;
};

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
