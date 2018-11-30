/**
 ****************************************************************************************
 *
 * @file NodeBase.c
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
#include "state.h"
#include "XYObjects/XYObject.h"
#include "ByteArray.h"
#include "network.h"
#include "defines.h"
#include "include/node/nodebase.h"


/**
 ****************************************************************************************
 *  NAME
 *      initNode
 *
 *  DESCRIPTION
 *    Initializes the Node Base class with a repository and hashing provider, and sets up
 *    the state and bound witness session.
 *
 *  PARAMETERS
 *      object      [in]      CryptoCreator*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */

XYResult_t initNode(NodeBase_t* self){
  INIT_SELF_UNKNOWN();

  self->originChainState.index = 0;
  self->choice = NODE_MODE;
  memset(self->originChainState.latestHash, 0, 32);
  
  return result;
}