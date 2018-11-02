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
  XYResult_t* (*addBoundWitness)(struct OriginChainNavigatorT* self, BoundWitness_t*);
  XYResult_t* (*containsOriginBlock)(struct OriginChainNavigatorT* self_OriginChainNavigator, BoundWitness_t* user_BoundWitness);
  OriginChainProvider_t* originChainRepository;
  HashProvider_t* Hash;
  ByteArray_t* *bridgeQueue;
  uint32_t queueLen;
  char lastHash[32];

} OriginChainNavigator_t;
#endif


//OriginChainNavigator* initOriginChainProvider( OriginChainNavigator** self, char* bits);
XYResult_t* getMostRecentOriginBlock(OriginChainNavigator* self_OriginChainNavigator);
XYResult_t* containsOriginBlock(OriginChainNavigator* self_OriginChainNavigator, BoundWitness_t* user_BoundWitness);
XYResult_t* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, BoundWitness_t* user_BoundWitness);
