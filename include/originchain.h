#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "storage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef ORIGINCHAIN_H
typedef struct OriginChainNavigator OriginChainNavigator;

#define ORIGINCHAIN_H
#endif

struct OriginChainNavigator {
  XYResult* (*getOriginBlockByPreviousHash)(OriginChainNavigator*, ByteArray* originBlockHash);
  XYResult* (*getOriginBlockByBlockHash)(OriginChainNavigator*, ByteArray* originBlockHash);
  XYResult* (*removeOriginBlock)(OriginChainNavigator*, ByteArray* originBlockHash);
  XYResult* (*addBoundWitness)(BoundWitness*);
  XYResult* (*getHash)(BoundWitness* user_BoundWitness, HashProvider* user_HashProvider);
  XYResult* (*findPreviousBlocks)(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  StorageProvider* Storage;
  HashProvider* Hash;
};
