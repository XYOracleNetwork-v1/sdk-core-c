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

#include "nodebase.h"

typedef struct RelayNode RelayNode;

struct RelayNode {
  NodeBase_t* node;
  NodeListener* listener;
  ProcedureCatalogue* procedureCatalogue;
  NetworkProvider* networkProvider;
 /*
  * Gets the choice of a catalog from another party.
  */
  uint8_t (*getChoice)(ProcedureCatalogue* ourCatalog, uint8_t theirCatalog);
  NetworkPipe_t* (*findSomeoneToTalkTo)( void );
  XYResult_t* (*doConnection)( RelayNode* self );

};

XYResult_t* initRelayNode(RelayNode* self, OriginChainProvider_t* repository, HashProvider_t* hashingProvider, uint8_t heuristicCount);
  /**
    * Gets the choice of a catalog from another party.
    *
    * @param theirCatalog The catalog of the other party.
    * @return The choice to preform in the bound witness.
    */


uint8_t Relay_getChoice(uint8_t* theirCatalog);
XYResult_t* doConnection(RelayNode* self);
//extern NetworkPipe_t* findSomeoneToTalkTo( void );


#define NODEBASE_H
#endif
