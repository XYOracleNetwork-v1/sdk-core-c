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
#include <stdio.h>
//#include <sys/socket.h>
#include <stdlib.h>
//#include <netinet/in.h>
#include <string.h>
//#include <fcntl.h>
#include <limits.h>
#include "defines.h"
#include "ByteArray.h"
#include "XYResult.h"

typedef struct ProcedureCatalogue ProcedureCatalogue_t;
typedef struct NetworkPipe NetworkPipe_t;
typedef struct NetworkPeer NetworkPeer_t;

struct NetworkPeer{
  /*
   * Sentinel-known implementation specific private variables
   * They can be erased in later networking impelmentations,
   * the only file that uses these variables at the time
   * of writing is network.c for TCP.
   */
  int socket;
//  struct sockaddr_in address;
  uint16_t port;
};

struct NetworkPipe{
  NetworkPeer_t peer;
  uint8_t role;
  ByteArray_t scratchBuffer;
  char theirCatalog[CATALOG_BUFFER_SIZE+4];
};

void* serverThread(void* flag);
void* clientThread(void* args);
extern XYResult_t findPeer(NetworkPipe_t* pipe, char* buffer, uint32_t size, uint8_t flags);
extern uint8_t canDo(ByteArray_t* catalog);

#endif
