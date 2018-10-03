/**
 ****************************************************************************************
 *
 * @file state.h
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

#ifndef STATE_H
#define STATE_H
#include <stdint.h>
#include "crypto.h"
#include "xyobject.h"
#include "repository.h"
typedef struct OriginChainState OriginChainState;

struct OriginChainState {
  XYResult* (*newOriginBlock)(OriginChainState* self_OriginChainState, ByteArray* newHash);
  XYResult* (*addSigner)(OriginChainState* self_OriginChainState, Signer* newSigner);
  XYResult* (*getSigners)(OriginChainState* self_OriginChainState);
  Signer* currentSigner;
  Signer* nextSigner;
  ByteArray* latestHash;
  uint32_t index;
};

#endif
