/**
 ****************************************************************************************
 *
 * @file nodebase.h
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

#ifndef NODEBASE_H

#include "repository.h"
#include "hash.h"
#include "originchain.h"
#include "ZigZagBoundWitnessSession.h"
#include "state.h"
#include "boundwitnessoption.h"
#include "nodelistener.h"
#include "xyobject.h"

typedef struct NodeBase NodeBase;

struct NodeBase {
  OriginChainProvider* blockRepository;
  HashProvider* hashingProvider;
  OriginChainNavigator* originChainNavigator;
  OriginChainState* originChainState;
  ZigZagBoundWitnessSession* session;
  uint8_t heuristicCount;
  uint8_t flag;
  BoundWitnessOption* boundWitnessOptions[BOUNDWITNESS_OPTIONS];
  NodeListener* listeners[MAX_ALLOCATED_LISTENERS];

  /*
  * Initializes the Node.
  */
  XYResult* (*initNode)(NodeBase* self, OriginChainProvider* repository, HashProvider* hashingProvider);
  /*
  * Gets the choice of a catalog from another party.
  */
  uint8_t (*getChoice)(NodeBase* self, uint catalog, uint8_t strict);
  /*
  * Adds a heuristic to be used when creating bound witnesses.
  */
  uint8_t (*addHeuristic)(NodeBase* self, uint key, XYObject* heuristic);
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
  XYResult* (*getBridgedBlocks)(NodeBase* self);
  /*
  * Create bound witness, handle outcome, and store if needed
  */
  XYResult* (*doBoundWitness)(NodeBase* self, ByteArray* startingData, NetworkPipe* pipe);
  /*
  * Update the state of the origin chain.
  */
  XYResult* (*updateOriginState)(NodeBase* self, ZigZagBoundWitness* boundWitness);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  XYResult* (*makePayload)(NodeBase* self, uint bitFlag);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  XYResult* (*onBoundWitnessStart)(void);
  /*
   * Struct hack heuristics in
   */
  XYObject* heuristics[0];
};

XYResult* initNode(NodeBase* self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount);
uint8_t addHeuristic(NodeBase* self, uint key, XYObject* heuristic);
uint8_t removeHeuristic(NodeBase* self, uint key);
uint8_t selfSignOriginChain(NodeBase* self, uint flag);
XYObject* getUnSignedPayloads(NodeBase* self, uint bitFlag);
XYObject* getSignedPayloads(NodeBase* self, uint bitFlag);
void notifyListeners(NodeBase* self, BoundWitness* boundWitness);
XYResult* getBridgedBlocks(NodeBase* self);
void doBoundWitness(NodeBase* self, ByteArray* startingData, NetworkPipe* pipe);
uint8_t updateOriginState(NodeBase* self, BoundWitness* boundWitness);
XYResult* onBoundWitnessEndSuccess(NodeBase* self, BoundWitness* boundWitness);
void onBoundWitnessEndFailure(enum EXyoErrors error);
XYResult* makePayload(NodeBase* self, uint bitFlag);
/**
    * Gets the choice of a catalog from another party.
    *
    * @param catalog The catalog of the other party.
    * @return The choice to preform in the bound witness.
    */
uint8_t (*getChoice)(uint8_t* catalog, uint8_t boolean);

#define NODEBASE_H
#endif
