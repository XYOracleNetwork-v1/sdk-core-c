#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include <stdint.h>
#include "xyo.h"
typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession;

struct ZigZagBoundWitnessSession {
  struct XYResult* (*completeBoundWitness)(ZigZagBoundWitnessSession* userSession, ByteArray* bwData);
  struct NetworkProvider* NetworkProvider;
  struct ZigZagBoundWitness* BoundWitness;
  int choice;
  int cycles;

};

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
