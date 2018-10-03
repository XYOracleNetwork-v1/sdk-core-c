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
  NodeBase* node;
  NodeListener* listener;
  ProcedureCatalogue* procedureCatalogue;
  /*
  * Gets the choice of a catalog from another party.
  */
  uint8_t (*getChoice)(ProcedureCatalogue* ourCatalog, uint8_t theirCatalog);
  NetworkPipe* (*findSomeoneToTalkTo)( RelayNode* self );
  void (*doConnection)( RelayNode* self );

};

XYResult* initRelayNode(RelayNode* self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount);
/**
    * Gets the choice of a catalog from another party.
    *
    * @param catalog The catalog of the other party.
    * @return The choice to preform in the bound witness.
    */


uint8_t Relay_getChoice(ProcedureCatalogue* ourCatalog, uint8_t theirCatalog);



#define NODEBASE_H
#endif
