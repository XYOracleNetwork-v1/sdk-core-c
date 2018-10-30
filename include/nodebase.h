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
  uint8_t (*addHeuristic)(NodeBase* self, uint16_t key, XYObject* heuristic);
  /*
  * Removes a heuristic from the current heuristic pool.
  */
  uint8_t (*removeHeuristic)(NodeBase* self, uint16_t key);
  /*
  * Self signs an origin block to the devices origin chain.
  */
  uint8_t (*selfSignOriginChain)(NodeBase* self);
  /*
  * Gets all of the unsigned payloads for a given flag.
  */
  XYObject* (*getUnSignedPayloads)(NodeBase* self);
  /*
  * Gets all of the signed payloads for a given flag.
  */
  XYObject* (*getSignedPayloads)(NodeBase* self);
  /*
  * Call the listener for each block in a bound witness.
  */
  void (*notifyListeners)(NodeBase* self, BoundWitness* boundWitness);
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
  uint8_t (*updateOriginState)(NodeBase* self, BoundWitness* boundWitness);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  XYResult* (*makePayload)(NodeBase* self);
  /*
  * Make a Payload* which can be used in a bound witness.
  */
  void (*onBoundWitnessStart)(void);

  void (*onBoundWitnessEndSuccess)(NodeBase* self, BoundWitness* boundWitness);
  void (*onBoundWitnessEndFailure)( void );

  /*
   * Struct hack heuristics in
   */
  XYObject* heuristics[];
};

XYResult* initNode(NodeBase** self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount);
uint8_t addHeuristic(NodeBase* self, uint16_t key, XYObject* heuristic);
uint8_t removeHeuristic(NodeBase* self, uint16_t key);
uint8_t selfSignOriginChain(NodeBase* self);
XYObject* getUnSignedPayloads(NodeBase* self);
XYObject* getSignedPayloads(NodeBase* self);
void notifyListeners(NodeBase* self, BoundWitness* boundWitness);
XYResult* getBridgedBlocks(NodeBase* self);
XYResult* doBoundWitness(NodeBase* self, ByteArray* startingData, NetworkPipe* pipe);
uint8_t updateOriginState(NodeBase* self, BoundWitness* boundWitness);
void onBoundWitnessEndSuccess(NodeBase* self, BoundWitness* boundWitness);
void onBoundWitnessEndFailure( void );
void onBoundWitnessStart( void );
XYResult* makePayload(NodeBase* self);

//extern uint8_t (*getChoice)(uint8_t catalog);
XYObject* getSignedHash(void*);
XYObject* getUnSignedHash(void*);
XYObject* getSignedIndex(void*);
XYObject* getUnSignedIndex(void*);
XYObject* getSignedBridge(void*);
XYObject* getUnSignedBridge(void*);


/* Globals */
extern uint8_t gsignedHeuristicCount;
extern uint8_t gunsignedHeuristicCount;
extern OriginChainState* gState;

//ProcedureCatalogue* procedureCatalogue;

#define NODEBASE_H
#endif
