/**
 ****************************************************************************************
 *
 * @file state.h
 *
 * @XY4 project source code.
 *
 * @brief primary state routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef STATE_H
#define STATE_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdint.h>
#include "xyobject.h"
#include "crypto.h"
#include "xyo.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct OriginChainState OriginChainState;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 
struct OriginChainState {
  XYResult_t* (*newOriginBlock)(OriginChainState* self_OriginChainState, ByteArray_t* newHash);
  XYResult_t* (*addSigner)(OriginChainState* self_OriginChainState, Signer_t* newSigner);
  Signer_t* currentSigner;
  Signer_t* nextSigner;
  ByteArray_t* latestHash;
  uint32_t index;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newOriginBlock(OriginChainState* self_OriginChainState, ByteArray_t* originBlockHash);
XYResult_t* addSigner(OriginChainState* self_OriginChainState, Signer_t* user_Signer);

#endif

// end of file state.h

