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
//#include <pthread.h>
//#include <unistd.h>
#include <stdio.h>
//#include <sys/socket.h>
#include <stdlib.h>
//#include <netinet/in.h>
#include <string.h>
//#include <fcntl.h>
#include <limits.h>
#include "xyo.h"
#include "defines.h"
#include "ByteArray.h"
#include "xyresult.h"

typedef struct NetworkProvider NetworkProvider;
typedef struct ProcedureCatalogue ProcedureCatalogue;
//typedef struct NetworkPipe NetworkPipe_t;
typedef struct NetworkPeer NetworkPeer_t;

struct ProcedureCatalogue {
/**
 * Given a series of bytes. It will return if the device can support that operation.
 *
 * @param byteArray A series of bytes, that is the other parties catalog.
 * @return If the party can do the operation.
 */
  uint8_t (*canDo)(ByteArray_t* catalog);

};
/*
typedef struct NetworkPipe{
  NetworkPeer_t* peer;
  NetworkProvider* provider;
  ByteArray_t* initializationData;
  ByteArray_t* catalogBuffer;
  XYResult_t* (*send)(void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data));
  XYResult_t* (*close)(void* self);
} NetworkPipe_t;
*/
/*
typedef struct NetworkProvider {
  NetworkPipe_t* (*findPeer)(NetworkPipe_t* self, uint8_t flags);
  void (*addPeer)(NetworkProvider* self, char* ip, int port);

  / *
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   * /
   struct sockaddr_in* *peers;
   ProcedureCatalogue* procedureCatalogue;
   uint16_t port;
   uint8_t peerCount;
} NetworkProvider_t;
*/

typedef struct NetworkPipe{
  NetworkPeer_t* peer;
  NetworkProvider* provider;
  ByteArray_t* initializationData;
  ByteArray_t* catalogBuffer;
  XYResult_t* (*send)(void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data));
  XYResult_t* (*close)(void* self);
} NetworkPipe_t;

typedef struct NetworkProvider {
  NetworkPipe_t* (*findPeer)(NetworkProvider* self, uint8_t flags);
  void (*addPeer)(NetworkProvider* self, char* ip, int port);

  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
   struct sockaddr_in* *peers;
   ProcedureCatalogue* procedureCatalogue;
   uint16_t port;
   uint8_t peerCount;
} NetworkProvider_t;


struct NetworkPeer{
  XYResult_t* (*getRole)(NetworkPipe_t* pipe);

  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
  int server_fd;
  int socket;
  char* elhoBuffer;
  //struct sockaddr_in address;
};
/*
struct TcpClient {
  NetworkPipe_t* (*findPeer)(struct TcpClient* self, uint8_t flags);
  NetworkProvider* networkProvider;
  struct sockaddr_in* peers[MAX_PEERS];
  ProcedureCatalogue* procedureCatalogue;
};
*/

struct client_arguments{
  uint8_t flags;
  //pthread_t* server;
  struct sockaddr_in* *peers;
  uint8_t peerCount;
};

struct server_arguments{
  uint8_t flags;
  //pthread_t* client;
  int port;
};

void* serverThread(void* flag);
void* clientThread(void* args);
extern NetworkPipe_t* findPeer(NetworkProvider* self, uint8_t flags);
extern uint8_t canDo(ByteArray_t* catalog);

#endif
