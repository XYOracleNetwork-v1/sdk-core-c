#ifndef BOUNDWITNESS_H
#include "xyobject.h"
#include "hash.h"

typedef struct BoundWitness BoundWitness;

struct BoundWitness {
  uint32_t size;
  struct ShortStrongArray* publicKeys;
  struct IntStrongArray* payloads;
  struct ShortStrongArray* signatures;
  struct XYResult*  (*getSigningData)(BoundWitness*);
  struct XYResult* (*getHash)(BoundWitness*, HashProvider*);
  struct XYResult* (*toTransfer)(BoundWitness*);
} ;

#define BOUNDWITNESS_H
#endif
