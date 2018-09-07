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
  XYResult* (*getOriginBlockByPreviousHash)(struct OriginChainNavigator*, char* originBlockHash);
  XYResult* (*getOriginBlockByBlockHash)(struct OriginChainNavigator*, char* originBlockHash);
  XYResult* (*removeOriginBlock)(OriginChainNavigator*, char* originBlockHash);
  XYResult* (*addBoundWitness)(BoundWitness*);
  StorageProvider* Storage;

};
