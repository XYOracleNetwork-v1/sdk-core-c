/**
 ****************************************************************************************
 *
 * @file originchain.h
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

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "BoundWitness.h"
#include "repository.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef ORIGINCHAIN_H
typedef struct OriginChainNavigator OriginChainNavigator;

#define ORIGINCHAIN_H
#endif

struct OriginChainNavigator {
  //XYResult* (*getOriginBlockByPreviousHash)(OriginChainNavigator*, ByteArray* originBlockHash);
  //XYResult* (*getOriginBlockByBlockHash)(OriginChainNavigator*, ByteArray* originBlockHash);
  //XYResult* (*removeOriginBlock)(OriginChainNavigator*, ByteArray* originBlockHash);
  XYResult* (*addBoundWitness)(OriginChainNavigator* self, BoundWitness*);
  //XYResult* (*getHash)(BoundWitness* user_BoundWitness, HashProvider* user_HashProvider);
  //XYResult* (*findPreviousBlocks)(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  XYResult* (*containsOriginBlock)(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  OriginChainProvider* originChainRepository;
  HashProvider* Hash;
  ByteArray bridgeQueue[MIN_QUEUE];

};

XYResult* getMostRecentOriginBlock(OriginChainNavigator* self_OriginChainNavigator);
