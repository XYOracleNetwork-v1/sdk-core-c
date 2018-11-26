/**
 ****************************************************************************************
 *
 * @file network.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 * INCLUDES
 ****************************************************************************************
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include "xyo.h"
#include "defines.h"
#include "ByteArray.h"
#include "xyresult.h"

typedef struct NetworkProvider NetworkProvider_t;
typedef struct ProcedureCatalogue ProcedureCatalogue_t;
typedef struct NetworkPipe NetworkPipe_t;
typedef struct NetworkPeer NetworkPeer_t;

struct NetworkProvider {
  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
   struct sockaddr_in peer;
   uint16_t port;
   uint8_t peerCount;
};

struct NetworkPeer{
  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
  int socket;
  struct sockaddr_in address;
};

struct NetworkPipe{
  NetworkPeer_t peer;
  NetworkProvider_t* provider;
  uint8_t role;
  ByteArray_t scratchBuffer;
};

struct TcpClient {
  NetworkProvider_t* networkProvider;
  struct sockaddr_in* peers[MAX_PEERS];
};


struct client_arguments{
  uint8_t flags;
  pthread_t* server;
  struct sockaddr_in* *peers;
  uint8_t peerCount;
};

struct server_arguments{
  uint8_t flags;
  pthread_t* client;
  int port;
};

void* serverThread(void* flag);
void* clientThread(void* args);
extern XYResult_t findPeer(NetworkProvider_t* self, NetworkPipe_t* pipe, uint8_t flags);
extern uint8_t canDo(ByteArray_t* catalog);

#endif
