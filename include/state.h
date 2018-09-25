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
#include <stdint.h>
#include "crypto.h"
#include "xyobject.h"
typedef struct OriginChainState OriginChainState;

struct OriginChainState {
  struct XYResult* (*newOriginBlock)(OriginChainState* self_OriginChainState, ByteArray* newHash);
  struct XYResult* (*addSigner)(OriginChainState* self_OriginChainState, Signer* newSigner);
  struct XYResult* (*getSigners)(OriginChainState* self_OriginChainState);
  struct Signer* currentSigner;
  struct Signer* nextSigner;
  struct ByteArray* latestHash;
  uint32_t index;
};

#define STATE_H
#endif
