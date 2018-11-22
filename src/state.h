/**
 ****************************************************************************************
 *
 * @file state.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef STATE_H
#include <stdint.h>
#include "XYObjects/XYObject.h"
#include "crypto.h"
#include "xyo.h"
//#include "ZigZagBoundWitnessSession.h"
//#include "include/state/originchain.h"
//#include "include/state/repository.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct OriginChainState {
  Signer_t* currentSigner;
  Signer_t* nextSigner;
  char latestHash[32];
  uint32_t index;
} OriginChainState_t;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newOriginBlock(OriginChainState_t* self_OriginChainState, ByteArray_t* originBlockHash);
XYResult_t* addSigner(OriginChainState_t* self_OriginChainState, Signer_t* user_Signer);
XYResult_t* getSigners(OriginChainState_t* self_OriginChainState);

#define STATE_H
#endif
