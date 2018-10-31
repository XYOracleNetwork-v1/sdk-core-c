/**
 ****************************************************************************************
 *
 * @file nodebase.h
 *
 * @XYO Core library source code.
 *
 * @brief primary node base routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef NODEBASE_H
#define NODEBASE_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef NODEBASE_H

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
  HashProvider_t* hashingProvider;
  OriginChainNavigator* originChainNavigator;
  OriginChainState* originChainState;
  ZigZagBoundWitnessSession_t* session;
  uint8_t heuristicCount;
  uint8_t flag;
  BoundWitnessOption* boundWitnessOptions[BOUNDWITNESS_OPTIONS];
  NodeListener* listeners[MAX_ALLOCATED_LISTENERS];

  /**
   * Gets the choice of a catalog from another party.
   *
   * @param catalog The catalog of the other party.
   * @return The choice to preform in the bound witness.
   */
  uint8_t (*getChoice)(uint8_t* catalog);

  /*
  * Adds a heuristic to be used when creating bound witnesses.
  */
  uint8_t (*addHeuristic)(NodeBase* self, uint32_t key, XYObject_t* heuristic);
  /*
  * Removes a heuristic from the current heuristic pool.
  */
  uint8_t (*removeHeuristic)(NodeBase* self, uint32_t key);
  /*
  * Self signs an origin block to the devices origin chain.
  */
  uint8_t (*selfSignOriginChain)(NodeBase* self, uint32_t flag);
  /*
  * Gets all of the unsigned payloads for a given flag.
  */
  uint8_t (*getUnSignedPayloads)(NodeBase* self, uint32_t bitFlag);
  /*
  * Gets all of the signed payloads for a given flag.
  */
  uint8_t (*getSignedPayloads)(NodeBase* self, uint32_t bitFlag);
  /*
  * Call the listener for each block in a bound witness.
  */
  uint8_t (*notifyListeners)(NodeBase* self, ZigZagBoundWitness_t* boundWitness);
  /*
  * Get blocks to bridge
  */
  XYResult_t* (*getBridgedBlocks)(NodeBase* self);
  /*
  * Create bound witness, handle outcome, and store if needed
  */
  XYResult_t* (*doBoundWitness)(ByteArray_t* startingData, NetworkPipe* pipe);
  /*
  * Update the state of the origin chain.
  */
  XYResult_t* (*updateOriginState)(NodeBase* self);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  XYResult_t* (*makePayload)(NodeBase* self, uint32_t bitFlag);
};

#endif

// end of file nodebase.h


