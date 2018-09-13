#ifndef BOUNDWITNESS_H
#include "hash.h"
#include "xyo.h"

typedef struct BoundWitness BoundWitness;
typedef struct BoundWitnessTransfer BoundWitnessTransfer;

struct BoundWitness {
  uint32_t size;
  struct ShortStrongArray* publicKeys;
  struct IntStrongArray* payloads;
  struct ShortStrongArray* signatures;
  struct XYResult*  (*getSigningData)(BoundWitness*);
  struct XYResult* (*getHash)(BoundWitness*, HashProvider*);
} ;

struct BoundWitnessTransfer {
  uint32_t size;
  uint8_t  choice;
  struct ShortStrongArray* publicKeys;
  struct IntStrongArray* payloads;
  struct ShortStrongArray* signatures;
  struct XYResult*  (*getSigningData)(BoundWitnessTransfer*);
  struct XYResult* (*getHash)(BoundWitness*, HashProvider*);
} ;

XYResult* BoundWitnessTransfer_create(char id[2], void* user_data);
XYResult* BoundWitnessTransfer_fromBytes(char* user_Transfer);
XYResult* BoundWitnessTransfer_toBytes(XYObject* user_XYObect);

XYResult* BoundWitness_creator_create(char id[2], void* user_data);
XYResult* BoundWitness_creator_fromBytes(char* BoundWitness_data);
XYResult* BoundWitness_creator_toBytes(struct XYObject* user_XYObect);
XYResult* BoundWitness_getSigningData(BoundWitness* user_BoundWitness);
XYResult* BoundWitness_getHash(BoundWitness* user_BoundWitness, HashProvider*);

#define BOUNDWITNESS_H
#endif
