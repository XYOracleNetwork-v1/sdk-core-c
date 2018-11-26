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
//#include "defines.h"
//#include "XYObjects/XYObject.h"
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

typedef struct NodeBase {
  OriginChainState_t originChainState;
  NetworkPipe_t* NetworkPipe;
  uint8_t choice;
} NodeBase_t;

XYResult_t initNode(NodeBase_t* self);
uint8_t addHeuristic(NodeBase_t* self, uint32_t key, XYObject_t* heuristic);
uint8_t removeHeuristic(NodeBase_t* self, uint32_t key);
uint8_t selfSignOriginChain(NodeBase_t* self);
XYObject_t* getUnSignedPayloads(NodeBase_t* self);
XYObject_t* getSignedPayloads(NodeBase_t* self);
void notifyListeners(NodeBase_t* self, BoundWitness_t* boundWitness);
XYResult_t* getBridgedBlocks(NodeBase_t* self);
//XYResult_t doBoundWitness(NodeBase_t* self, ByteArray_t* startingData, struct NetworkPipe_t* pipe);
XYResult_t completeBoundWitness(NodeBase_t* userSession,
                                  ByteArray_t* boundWitnessData, NetworkPipe_t* pipe);
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
//extern OriginChainState_t* gState;

#define NODEBASE_H
#endif
