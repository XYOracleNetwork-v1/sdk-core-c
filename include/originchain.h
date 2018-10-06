/**
 ****************************************************************************************
 *
 * @file originchain.h
 *
 * @XY4 project source code.
 *
 * @brief primary node base routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "BoundWitness.h"   // includes "xyobject.h", "hash.h"
#include "storage.h"
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

#endif

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct OriginChainNavigator {
  XYResult_t* (*getOriginBlockByPreviousHash)(OriginChainNavigator*, ByteArray_t* originBlockHash);
  XYResult_t* (*getOriginBlockByBlockHash)(OriginChainNavigator*, ByteArray_t* originBlockHash);
  XYResult_t* (*removeOriginBlock)(OriginChainNavigator*, ByteArray_t* originBlockHash);
  XYResult_t* (*addBoundWitness)(BoundWitness*);
  XYResult_t* (*getHash)(BoundWitness* user_BoundWitness, HashProvider_t* user_HashProvider);
  XYResult_t* (*findPreviousBlocks)(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness);
  StorageProvider* Storage;
  HashProvider_t* Hash;
};

// end of file originchain.h

