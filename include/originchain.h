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
#define ORIGINCHAIN_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct OriginChainNavigator OriginChainNavigator;

typedef struct OriginChainNavigator {
  XYResult_t* (*addBoundWitness)(struct OriginChainNavigatorT* self, BoundWitness*);
  XYResult_t* (*containsOriginBlock)(struct OriginChainNavigatorT* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  OriginChainProvider* originChainRepository;
  HashProvider_t* Hash;
  ByteArray_t* *bridgeQueue;
  uint32_t queueLen;
  char lastHash[32];

};
#endif


//OriginChainNavigator* initOriginChainProvider( OriginChainNavigator** self, char* bits);
XYResult* getMostRecentOriginBlock(OriginChainNavigator* self_OriginChainNavigator);
XYResult* containsOriginBlock(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
XYResult* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
