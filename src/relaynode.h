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

#ifndef RELAYNODE_H
#include <stdint.h>
#include "xyresult.h"
#include "defines.h"
#include "XYObjects/XYObject.h"
#include "arrays.h"

#include "objects.h"
#include "state.h"
//#include "include/state/originchain.h"
//#include "include/state/repository.h"
//#include "ZigZagBoundWitnessSession.h"
#include "network.h"
#include "include/node/nodebase.h"





#include "include/node/nodebase.h"
#include "XYObjects/Array/Iterator.h"
#include "XYObjects/XYObject.h"

typedef struct RelayNode RelayNode_t;

struct RelayNode {
  NodeBase_t node;
  NodeListener listener;
  NetworkPipe_t networkPipe;
};

//XYResult_t* initRelayNode(RelayNode_t* self, OriginChainProvider_t* repository, HashProvider_t* hashingProvider, uint8_t heuristicCount);
  /**
    * Gets the choice of a catalog from another party.
    *
    * @param theirCatalog The catalog of the other party.
    * @return The choice to preform in the bound witness.
    */


uint8_t Relay_getChoice(uint8_t* theirCatalog);
XYResult_t doConnection(RelayNode_t* self);
XYResult_t insertPublicKey(RelayNode_t* relay);
XYResult_t insertPayloads(RelayNode_t* relay);
XYResult_t insertSignature(RelayNode_t* relay);
extern XYResult_t socket_send(NetworkPipe_t* self, char* data, uint32_t count);
//extern NetworkPipe_t* findSomeoneToTalkTo( void );


#define NODEBASE_H
#endif
