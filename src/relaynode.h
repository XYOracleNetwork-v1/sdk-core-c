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
#define ONE_NONITERABLE_UNTYPED 0 // Consider changing this constant
#define ONE_NONITERABLE_TYPED 16
#define ONE_ITERABLE_UNTYPED 32
#define ONE_ITERABLE_TYPED 48
#define TWO_NONITERABLE_UNTYPED 64
#define TWO_NONITERABLE_TYPED 80
#define FOUR_NONITERABLE_TYPED 144
#define TWO_ITERABLE_UNTYPED 96
#define TWO_ITERABLE_TYPED 112 // 0b01110000
#define FOUR_ITERABLE_TYPED 176
#define FOUR_ITERABLE_UNTYPED 160
#define FOUR_NONITERABLE_UNTYPED 128


uint8_t Relay_getChoice(const uint8_t* theirCatalog);
XYResult_t doConnection(RelayNode_t* self, uint32_t offset);
XYResult_t getInitBoundWitness( void );
extern XYResult_t socket_send(NetworkPipe_t* self, char* data, uint32_t count);
extern XYResult_t socket_recv(NetworkPipe_t* self, char* data, uint32_t recvBytes);
extern XYResult_t socket_close(NetworkPipe_t* self);
//extern NetworkPipe_t* findSomeoneToTalkTo( void );



//TODO: DEBUG extern
//extern uint8_t fetter[47];
#define RELAYNODE_H
#endif
