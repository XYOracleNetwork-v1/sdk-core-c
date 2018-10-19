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
//typedef struct OriginChainNavigator OriginChainNavigator;
//typedef struct OriginChainProvider  OriginChainProvider;


typedef struct OriginChainNavigatorT {
  XYResult* (*addBoundWitness)(struct OriginChainNavigatorT* self, BoundWitness*);
  XYResult* (*containsOriginBlock)(struct OriginChainNavigatorT* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  OriginChainProvider* originChainRepository;
  HashProvider* Hash;
  ByteArray bridgeQueue[MIN_QUEUE];

} OriginChainNavigator;
#endif


//OriginChainNavigator* initOriginChainProvider( OriginChainNavigator** self, char* bits);
XYResult* getMostRecentOriginBlock(OriginChainNavigator* self_OriginChainNavigator);
XYResult* containsOriginBlock(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
XYResult* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
