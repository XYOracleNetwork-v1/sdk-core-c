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

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef NODEBASE_H

//#include "repository.h"
//#include "hash.h"
//#include "originchain.h"
//#include "ZigZagBoundWitnessSession.h"
//#include "state.h"
//#include "defines.h"
//#include "xyobject.h"
//#include "boundwitnessoption.h"
#include <stdint.h>
#include "boundwitnessoption.h"
#include "nodelistener.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct NodeBase NodeBase_t;

/*
 * FORWARD DEFINITIONS
 ****************************************************************************************
 */
 /*
typedef struct RepositoryProvider RepositoryProvider_t;
typedef struct HashProvider HashProvider_t;
typedef struct OriginChainNavigator OriginChainNavigator_t;
typedef struct OriginChainState OriginChainState_t;
typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession_t;
typedef struct XYObject XYObject_t;
typedef struct XYResult XYResult_t;
typedef struct BoundWitnessOption BoundWitnessOption;
//typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession_t;
*/

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct NodeBase {
  RepositoryProvider_t* blockRepository;
  HashProvider_t* hashingProvider;
  OriginChainNavigator_t* originChainNavigator;
  OriginChainState_t* originChainState;
  ZigZagBoundWitnessSession_t* session;
  uint8_t heuristicCount;
  uint8_t flag;
  BoundWitnessOption_t* boundWitnessOptions[BOUNDWITNESS_OPTIONS];
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
  uint8_t (*addHeuristic)(NodeBase_t* self, uint32_t key, XYObject_t* heuristic);
  /*
  * Removes a heuristic from the current heuristic pool.
  */
  uint8_t (*removeHeuristic)(NodeBase_t* self, uint32_t key);
  /*
  * Self signs an origin block to the devices origin chain.
  */
  uint8_t (*selfSignOriginChain)(NodeBase_t* self, uint32_t flag);
  /*
  * Gets all of the unsigned payloads for a given flag.
  */
  uint8_t (*getUnSignedPayloads)(NodeBase_t* self, uint32_t bitFlag);
  /*
  * Gets all of the signed payloads for a given flag.
  */
  uint8_t (*getSignedPayloads)(NodeBase_t* self, uint32_t bitFlag);
  /*
  * Call the listener for each block in a bound witness.
  */
  uint8_t (*notifyListeners)(NodeBase_t* self, ZigZagBoundWitness_t* boundWitness);
  /*
  * Get blocks to bridge
  */
  XYResult_t* (*getBridgedBlocks)(NodeBase_t* self);
  /*
  * Create bound witness, handle outcome, and store if needed
  */
  XYResult_t* (*doBoundWitness)(NodeBase_t* self, ByteArray_t* startingData, NetworkPipe_t* pipe);
  /*
  * Update the state of the origin chain.
  */
  XYResult_t* (*updateOriginState)(NodeBase_t* self);
  /*
  * Make a Payload_t* which can be used in a bound witness.
  */
  XYResult_t* (*makePayload)(NodeBase_t* self);
  /*
   * Make a Payload* which can be used in a bound witness.
   */
  void (*onBoundWitnessStart)(void);

  void (*onBoundWitnessEndSuccess)(NodeBase_t* self, BoundWitness_t* boundWitness);
  void (*onBoundWitnessEndFailure)( void );

  /*
   * Struct hack heuristics in
   */
  XYObject_t* heuristics[];
};

XYResult_t* initNode(NodeBase_t** self, OriginChainProvider_t* repository, HashProvider_t* hashingProvider, uint8_t heuristicCount);
uint8_t addHeuristic(NodeBase_t* self, uint32_t key, XYObject_t* heuristic);
uint8_t removeHeuristic(NodeBase_t* self, uint32_t key);
uint8_t selfSignOriginChain(NodeBase_t* self);
XYObject_t* getUnSignedPayloads(NodeBase_t* self);
XYObject_t* getSignedPayloads(NodeBase_t* self);
void notifyListeners(NodeBase_t* self, BoundWitness_t* boundWitness);
XYResult_t* getBridgedBlocks(NodeBase_t* self);
XYResult_t* doBoundWitness(NodeBase_t* self, ByteArray_t* startingData, NetworkPipe_t* pipe);
uint8_t updateOriginState(NodeBase_t* self, BoundWitness_t* boundWitness);
void onBoundWitnessEndSuccess(NodeBase_t* self, BoundWitness_t* boundWitness);
void onBoundWitnessEndFailure( void );
void onBoundWitnessStart( void );
XYResult_t* makePayload(NodeBase_t* self);

//extern uint8_t (*getChoice)(uint8_t catalog);
XYObject_t* getSignedHash(void*);
XYObject_t* getUnSignedHash(void*);
XYObject_t* getSignedIndex(void*);
XYObject_t* getUnSignedIndex(void*);
XYObject_t* getSignedBridge(void*);
XYObject_t* getUnSignedBridge(void*);


/* Globals */
extern uint8_t gsignedHeuristicCount;
extern uint8_t gunsignedHeuristicCount;
extern OriginChainState_t* gState;


#endif
