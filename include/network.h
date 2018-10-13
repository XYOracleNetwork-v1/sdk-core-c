/**
 ****************************************************************************************
 *
 * @file network.h
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

#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include "xyo.h"
#include "defines.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

typedef struct NetworkProvider NetworkProvider;
typedef struct ProcedureCatalogue ProcedureCatalogue;
typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;

struct NetworkProvider {
  NetworkPipe* (*findPeer)(NetworkProvider* self, uint8_t flags);
  void (*addPeer)(NetworkProvider* self, char* ip, int port);

  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
   struct sockaddr_in* peers[MAX_PEERS];
   ProcedureCatalogue* procedureCatalogue;
   int port;
};

struct ProcedureCatalogue {
/**
 * Given a series of bytes. It will return if the device can support that operation.
 *
 * @param byteArray A series of bytes, that is the other parties catalog.
 * @return If the party can do the operation.
 */
  uint8_t (*canDo)(ByteArray* catalog);

};

struct NetworkPipe{
  NetworkPeer* peer;
  NetworkProvider* provider;
  ByteArray* initializationData;
  ByteArray* catalogBuffer;
  XYResult* (*send)(void* self, ByteArray* data, XYResult* (*callback)(void* self, ByteArray* data));
  XYResult* (*close)();
};

struct NetworkPeer{
  XYResult* (*getRole)(NetworkPipe* pipe);

  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
  int server_fd;
  int socket;
  char* elhoBuffer;
  struct sockaddr_in address;
};

struct TcpClient {
  NetworkPipe* (*findPeer)(struct TcpClient* self, uint8_t flags);
  NetworkProvider* networkProvider;
  struct sockaddr_in* peers[MAX_PEERS];
  ProcedureCatalogue* procedureCatalogue;
};


struct client_arguments{
  uint8_t flags;
  pthread_t* server;
  struct sockaddr_in* peers[MAX_PEERS];
};

struct server_arguments{
  uint8_t flags;
  pthread_t* client;
  int port;
};

void* serverThread(void* flag);
void* clientThread(void* args);
extern NetworkPipe* findPeer(NetworkProvider* self, uint8_t flags);
extern uint8_t canDo(ByteArray* catalog);
#endif
