/**
 ****************************************************************************************
 *
 * @file nodebase.h
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

#ifndef NODEBASE_H
#define NODEBASE_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "repository.h"
#include "hash.h"
#include "originchain.h"
#include "ZigZagBoundWitnessSession.h"
#include "state.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct NodeBase NodeBase;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct NodeBase {
  RepositoryProvider* blockRepository;
  HashProvider* hashingProvider;
  OriginChainNavigator* originChainNavigator;
  OriginChainState* state;
  ZigZagBoundWitnessSession* session;
  /*
  * Gets the choice of a catalog from another party.
  */
  uint (*getChoice)(NodeBase* self, uint catalog, uint8_t strict);
  /*
  * Adds a heuristic to be used when creating bound witnesses.
  */
  uint8_t (*addHeuristic)(NodeBase* self, uint key, XYObject_T* heuristic);
  /*
  * Removes a heuristic from the current heuristic pool.
  */
  uint8_t (*removeHeuristic)(NodeBase* self, uint key);
  /*
  * Self signs an origin block to the devices origin chain.
  */
  uint8_t (*selfSignOriginChain)(NodeBase* self, uint flag);
  /*
  * Gets all of the unsigned payloads for a given flag.
  */
  uint8_t (*getUnSignedPayloads)(NodeBase* self, uint bitFlag);
  /*
  * Gets all of the signed payloads for a given flag.
  */
  uint8_t (*getSignedPayloads)(NodeBase* self, uint bitFlag);
  /*
  * Call the listener for each block in a bound witness.
  */
  uint8_t (*notifyListeners)(NodeBase* self, ZigZagBoundWitness* boundWitness);
  /*
  * Get blocks to bridge
  */
  XYResult_t* (*getBridgedBlocks)(NodeBase* self);
  /*
  * Create bound witness, handle outcome, and store if needed
  */
  XYResult_t* (*doBoundWitness)(ByteArray_T* startingData, NetworkPipe* pipe);
  /*
  * Update the state of the origin chain.
  */
  XYResult_t* (*updateOriginState)(NodeBase* self);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  XYResult_t* (*makePayload)(NodeBase* self, uint bitFlag);

};

#endif

// end of file nodebase.h


