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
#include "XYResult.h"
#include "defines.h"
#include "XYObjects/XYObject.h"
#include "state.h"
#include "network.h"
#include "include/node/nodebase.h"
#include "XYObjects/Array/Iterator.h"
#include "XYObjects/XYObject.h"

typedef struct RelayNode RelayNode_t;

struct RelayNode {
  NodeBase_t node;
  NodeListener listener;
  NetworkPipe_t networkPipe;
};


uint8_t Relay_getChoice(uint8_t* theirCatalog);
XYResult_t doConnection(RelayNode_t* self);
XYResult_t insertPublicKey(RelayNode_t* relay);
XYResult_t insertPayloads(RelayNode_t* relay);
XYResult_t insertSignature(RelayNode_t* relay);
extern XYResult_t socket_send(NetworkPipe_t* self, char* data, uint32_t count, uint8_t debug);
extern XYResult_t socket_recv(NetworkPipe_t* self, char* data, uint32_t recvBytes);
//extern NetworkPipe_t* findSomeoneToTalkTo( void );

#define TWO_NONITERABLE_UNTYPED 64 //UNDEFINED BEHAVIOR
#define TWO_NONITERABLE_TYPED 80
#define FOUR_NONITERABLE_TYPED 144
#define TWO_ITERABLE_UNTYPED 96
#define TWO_ITERABLE_TYPED 112
#define FOUR_ITERABLE_TYPED 176
#define FOUR_ITERABLE_UNTYPED 160
#define FOUR_NONITERABLE_UNTYPED 128

//TODO: DEBUG extern
extern char globalBuffer[1024];
#define NODEBASE_H
#endif
