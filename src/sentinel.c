/**
 ****************************************************************************************
 *
 * @file network.c
 *
 * @XYO Core library source code.
 *
 * @brief primary networking routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */
#include "sentinel.h"

Sentinel* initSentinel(NetworkProvider* socket, OriginChainProvider_t* store, HashProvider_t* hash){
  Sentinel* self = malloc(sizeof(Sentinel));
  if(self){
    self->networkProvider = socket;
    self->networkProvider->peers = malloc(sizeof(NetworkPeer_t) * DEFAULT_NUM_PEERS);
    self->relayNode = malloc(sizeof(RelayNode));
    if(self->relayNode){
      initRelayNode(self->relayNode, store, hash, 2);
      //findSomeoneToTalkTo_args.provider = socket;
      //findSomeoneToTalkTo_args.flags = GIVE_ORIGIN_CHAIN_OPTION+BOUND_WITNESS_OPTION;
      self->relayNode->findSomeoneToTalkTo = findSomeoneToTalkTo;
      self->relayNode->procedureCatalogue = malloc(sizeof(struct ProcedureCatalogue));
      if(self->relayNode->procedureCatalogue){
        self->relayNode->procedureCatalogue->canDo = canDo;
      } else {
        return NULL;
      }
      self->networkProvider->findPeer = findPeer;
      self->networkProvider->addPeer = addPeer;
      ByteArray_t* lHash = malloc(sizeof(ByteArray_t));
      if(hash){
        lHash->size = 32;
        lHash->payload = malloc(sizeof(char)*32);
      } else {
        return NULL;
      }
      self->relayNode->node->originChainState->latestHash = lHash;
      return self;
    } else {
      free(self);
      return NULL;
    }
  } else {
    return NULL;
  }
}

NetworkPipe_t* findSomeoneToTalkTo(){
  NetworkPipe_t* dummyPipe = malloc(sizeof(NetworkPipe_t));
  if(dummyPipe){
    dummyPipe->peer = malloc(sizeof(NetworkPeer_t));
    dummyPipe->provider = malloc(sizeof(NetworkProvider));
    dummyPipe->initializationData = NULL;
    dummyPipe->catalogBuffer = malloc(sizeof(ByteArray_t));
    dummyPipe->catalogBuffer->size = 1;
    dummyPipe->catalogBuffer->payload = malloc(sizeof(char));
    memset(dummyPipe->catalogBuffer->payload, BOUND_WITNESS_OPTION + GIVE_ORIGIN_CHAIN_OPTION, 1);
    dummyPipe->send = &dummySend;
    dummyPipe->close = &socket_close;
  } else {
    return NULL;
  }
  //return(findPeer(findSomeoneToTalkTo_args.provider, findSomeoneToTalkTo_args.flags));
  return NULL;
}



void* initTcpProvider(NetworkProvider** self, int port){
  *self = malloc(sizeof(NetworkProvider));
  if(*self){
    (*self)->findPeer = findPeer;
    (*self)->port = port;
    (*self)->peerCount = DEFAULT_NUM_PEERS;
    (*self)->peers = malloc(sizeof(struct sockaddr_in*)*(*self)->peerCount+1);
    (*self)->procedureCatalogue = malloc(sizeof(ProcedureCatalogue));
    if((*self)->procedureCatalogue){
      (*self)->procedureCatalogue->canDo = canDo;
      return (void*)TRUE;
    } else {
      free((*self));
      return NULL;
    }
  } else {
    return NULL;
  }
}

uint8_t canDo(ByteArray_t* catalog){
  /*
  uint8_t theirCatalog = catalog->payload[catalog->size-1];
  if(theirCatalog & BOUND_WITNESS_OPTION){
    return TRUE;
  } else if ( theirCatalog & TAKE_ORIGIN_CHAIN_OPTION){
    return TRUE;
  } else {
    return FALSE;
  }
  */
  return FALSE;
}

uint8_t client_done = FALSE;
uint8_t server_done = FALSE;

NetworkPipe_t* findPeer(NetworkProvider_t* self, uint8_t flags){
/*
  pthread_t server_thread, client_thread;
  struct client_arguments client_args;
  struct server_arguments server_args;
  NetworkPeer* client = NULL;
  NetworkPeer* server = NULL;

  client_args.flags = flags;
  client_args.server = &server_thread;
  client_args.peers = self->peers;
  client_args.peerCount = self->peerCount;
  / *
  for(int i = 0; i<MAX_PEERS; i++){
    if(self->peers[i] != NULL){
      struct sockaddr_in* peer = self->peers[i];
      client_args.peers[i] = peer;
    } else {
      break;
    }
  }
  * /
  //client_args.peers = self->peers;
  server_args.flags = flags;
  server_args.client = &client_thread;
  server_args.port = self->port;
  pthread_create(&client_thread, NULL, clientThread, (void*)&client_args );
  //pthread_create(&server_thread, NULL, serverThread, (void*)&server_args);
  while(1){
    sleep(1);
    if( client_done ){
      pthread_join(client_thread, (void*)&client);
      //pthread_cancel(server_thread);
      break;
    } else if ( server_done ) {
      pthread_join(server_thread, (void*)&server);
      //pthread_cancel(client_thread);
      break;
    } else {
      continue;
    }
  }
  NetworkPipe* return_pipe = malloc(sizeof(NetworkPipe));
  if(return_pipe){
    if(client_done){
      return_pipe->peer = client;
    } else if (server_done){
      return_pipe->peer = server;
    } else {
      free(return_pipe);
      return NULL;
    }
    return_pipe->peer->getRole = getRole;
    return_pipe->provider = self;
    return_pipe->catalogBuffer = malloc(sizeof(ByteArray));
    return_pipe->send = socket_send;
    return_pipe->close = socket_close;
    ((ByteArray*)return_pipe->catalogBuffer)->payload = malloc(sizeof(char)*CATALOG_SIZE+1);
    ((ByteArray*)return_pipe->catalogBuffer)->size = CATALOG_SIZE+1;
    if(client_done){
      uint32_t size = 0;
      memcpy(&size, client->elhoBuffer, 4);
      if(littleEndian()){
        size = to_uint32((unsigned char*)&size);
      }
      memcpy(((ByteArray*)return_pipe->catalogBuffer)->payload, client->elhoBuffer+4, CATALOG_SIZE+1);
      free(client->elhoBuffer);
      return_pipe->initializationData = malloc(sizeof(ByteArray));
      return_pipe->initializationData->size = size-CATALOG_BUFFER_SIZE;
      return_pipe->initializationData->payload = malloc(sizeof(char)*size-CATALOG_BUFFER_SIZE);
      read(client->socket, return_pipe->initializationData->payload, size-CATALOG_BUFFER_SIZE);
      //return_pipe->initializationData = NULL;
      return return_pipe;

    } else if (server_done){
      ByteArray* initializeData = malloc(sizeof(ByteArray));
      if(initializeData){
        char* shortBuffer = server->elhoBuffer;
        uint32_t total_size = to_uint32((unsigned char*)shortBuffer);
        char* initializeDataPayload = malloc(sizeof(char)*total_size);
        if(initializeDataPayload){
          initializeData->payload = initializeDataPayload;
          initializeData->size = total_size-CATALOG_BUFFER_SIZE;
          memcpy(initializeData->payload, server->elhoBuffer, total_size);
          return_pipe->peer = server;
          return_pipe->provider = self;
          return_pipe->initializationData = initializeData;
          return return_pipe;
        }

      } else {
        return NULL;
      }

    } else {

      return NULL;
    }

    if(client){
      return NULL;
    } else if(server){
      return NULL;
    } else {
      return NULL;
    }
  } else {
      return NULL;
  }
*/
  return NULL;
}

/* Server/Client code stolen from https://www.geeksforgeeks.org/socket-programming-cc/ */

void* clientThread(void* args){
  /*
  struct client_arguments* the_arguments = (struct client_arguments*)args;
  int sock = 2;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Failed to open socket err: %d", sock);
    return NULL;
  }
  //pthread_cleanup_push(cleanup_handler, &sock);

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);


  while(TRUE){
    for(int i=0; i<the_arguments->peerCount;i++){
      if(the_arguments->peers[i] != NULL){
        const struct sockaddr_in* address = the_arguments->peers[i];
        int val = connect(sock, (struct sockaddr*)address, sizeof(*address));
        if(val==0){
          if(CATALOG_BUFFER_SIZE < 255){
            char sendBuffer[CATALOG_BUFFER_SIZE] = { 0x00, 0x00, 0x00, CATALOG_BUFFER_SIZE, CATALOG_BUFFER_SIZE-5, 0x00, 0x00, 0x00, ((struct client_arguments*)args)->flags};
            send(sock, sendBuffer, CATALOG_BUFFER_SIZE , 0 );
          } else {
            continue;
          }

          char* recvBuffer = malloc(sizeof(char)*PARENT_RECV_BUFF_SIZE);
          if(recvBuffer == NULL){
            return recvBuffer;
          }

          NetworkPeer* return_peer = malloc(sizeof(NetworkPeer));
          if(return_peer==NULL){
            free(recvBuffer);
            return return_peer;
          }

          ssize_t ret_val = read(sock, recvBuffer, CATALOG_BUFFER_SIZE);
          if (ret_val < 0) {
              return NULL;
          }

          uint8_t theirCatalog = recvBuffer[CATALOG_BUFFER_SIZE-1];
          uint8_t ourCatalog = ((struct client_arguments*)args)->flags;

          if(theirCatalog & ourCatalog){
            if(return_peer){
              return_peer->server_fd = 0;
              return_peer->socket = sock;
              return_peer->elhoBuffer = recvBuffer;
              client_done = TRUE;
              pthread_exit(return_peer);
            } else { RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }

          } else if(ourCatalog==GIVE_ORIGIN_CHAIN_OPTION+BOUND_WITNESS_OPTION && theirCatalog==TAKE_ORIGIN_CHAIN_OPTION){
              return_peer->server_fd = 0;
              return_peer->socket = sock;
              return_peer->elhoBuffer = recvBuffer;
              client_done = TRUE;
              pthread_exit(return_peer);
          } else if(ourCatalog==TAKE_ORIGIN_CHAIN_OPTION+BOUND_WITNESS_OPTION && theirCatalog==GIVE_ORIGIN_CHAIN_OPTION){
            / *unimplemented: take bridge blocks * /
            shutdown(sock, SHUT_RDWR);
            continue;
          } else {
            #ifndef HEADLESS
            printf("Failed to connect because there catalog was: ");
            #endif
            for(int i = 0; i<CATALOG_BUFFER_SIZE+1; i++){
              printf("%d ", recvBuffer[i]);
            }
            printf("\nlen: %zd\n", ret_val);
            shutdown(sock, SHUT_RDWR);
            continue;
          }
        } else {
          int flag = 1;
          setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(int));
          if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
          {
            printf("Failed to open socket err: %d", sock);
            return NULL;
          }
          #ifndef HEADLESS
          //printf("Failed to connect %d \n", connect(sock, (struct sockaddr*)&address, sizeof(address)));
          #endif
        }
      } else {
        break;
      }
      sleep(1);
    }
  }
  //pthread_cleanup_pop(1);
  */
  return NULL;
}

void* serverThread(void* args){
  /*
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);


  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
      return NULL; // Socket Failed
  }
  //pthread_cleanup_push(cleanup_handler, &server_fd);

  //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
  {
      return NULL; // setsockopt failed
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(((struct server_arguments*)args)->port);

  if (bind(server_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
    return NULL;
  }
  if (listen(server_fd, 3) < 0)
  {
      return NULL;
  }
  while((new_socket = accept(server_fd, (struct sockaddr *)&address,
                     (socklen_t*)&addrlen)) < INT_MAX)
  {
    if (new_socket<0)
    {
      return NULL;
    }

    char recvBuffer[PARENT_RECV_BUFF_SIZE] = {0};

    ssize_t ret_val = read(new_socket, recvBuffer, PARENT_RECV_BUFF_SIZE);
    if (ret_val < 0) {
        return NULL;
    }
    uint8_t theirCatalog = recvBuffer[CATALOG_BUFFER_SIZE-1];
    uint8_t ourCatalog = ((struct client_arguments*)args)->flags;
    if(ourCatalog & theirCatalog){
      NetworkPeer* return_peer = malloc(sizeof(NetworkPeer));
      if(return_peer){
        return_peer->server_fd = server_fd;
        return_peer->socket = new_socket;
        return_peer->elhoBuffer = recvBuffer;
        server_done = TRUE;
        pthread_exit(return_peer);
      } else {
        return NULL;
      }
    } else {
      //TODO: Disconnect

    }
  }
  //pthread_cleanup_pop(1);
  */
  return NULL;
}

void cleanup_handler(void *arg )
{
  /*
   int sock = *(int*)arg;
   int flag = 1;
   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(int));
   // close your socket
  */
}

void addPeer(NetworkProvider* self, char* ip, int port){
    /*
    struct sockaddr_in* serv_addr = malloc(sizeof(struct sockaddr_in));
    memset(serv_addr, '0', sizeof(struct sockaddr_in));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(port);
    if(!inet_pton(AF_INET, ip, &serv_addr->sin_addr)){
      printf("[Warning] Adding peer %s %d failed. ERR %d\n", ip, port, inet_pton(AF_INET, ip, &serv_addr->sin_addr));
      return;
    }
    if((*self).peers[self->peerCount-1] == NULL){
      self->peers[self->peerCount-1] = serv_addr;
      self->peers[self->peerCount] = NULL;
    } else{
      self->peerCount+=1;
      self->peers = realloc(self->peers, (self->peerCount+1)*(sizeof(struct sockaddr_in*)));
      self->peers[self->peerCount-1] = serv_addr;
      self->peers[self->peerCount] = NULL;
    }
    return;
    */
}

XYResult_t* dummySend( void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data)) {
  
uint8_t boundTransfer1[] = {\
  0x00,0x00,0x00,0x65,0x01,0x00,0x48,0x02,
  0x02,0x00,0x44,0x04,0x01,0x7F,0xA3,0xE0,
  0x42,0x23,0xC5,0xF3,0xB1,0xFC,0x0D,0xFF,
  0xAA,0x4E,0x50,0x7C,0x18,0xFF,0x00,0x38,
  0x0B,0x0F,0x36,0xCC,0x93,0x22,0xDB,0xBB,
  0x21,0xFF,0xF3,0x0D,0x3A,0x37,0x06,0xEA,
  0xB1,0xB7,0xA4,0xDF,0xD6,0xCF,0x00,0x23,
  0x66,0xF9,0x35,0x00,0xB6,0xDC,0x8F,0xE4,
  0x8A,0x0D,0xEB,0xCA,0x00,0xF3,0xBC,0xB7,
  0xC7,0x95,0x6F,0x2B,0xB7,0x00,0x00,0x00,
  0x18,0x02,0x04,0x00,0x00,0x00,0x12,0x00,
  0x00,0x00,0x07,0x08,0x01,0x05,0x00,0x00,
  0x00,0x07,0x08,0x01,0x05 };
  ByteArray_t* returnArray = malloc(sizeof(ByteArray_t));
  returnArray->size = 65;
  returnArray->payload = malloc(sizeof(char)*65);
  memcpy(returnArray->payload, &boundTransfer1, 65); 
  callback(self, returnArray);
  
  return preallocated_return_result_ptr;
}

//XYResult_t* socket_send(void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data)) {
  /*
  uint32_t packetSize = data->size + 4;
  packetSize = to_uint32((unsigned char*)(uintptr_t)&packetSize);

  send(((ZigZagBoundWitnessSession*)self)->NetworkPipe->peer->socket, &packetSize, 4 , 0 );
  send(((ZigZagBoundWitnessSession*)self)->NetworkPipe->peer->socket, data->payload, data->size , 0 );
  char recvBuffer[PARENT_RECV_BUFF_SIZE] = {0};
  ssize_t ret_val = read(((ZigZagBoundWitnessSession*)self)->NetworkPipe->peer->socket, recvBuffer, PARENT_RECV_BUFF_SIZE);
  / *
  printf("Socket Recieved: ");
  for(int i = 0; i<ret_val; i++){
    printf("%d", recvBuffer[i]);
  }
  printf("\n");
  * /
  ByteArray* recieved = malloc(sizeof(ByteArray));
  recieved->size = ret_val;
  if(ret_val>0){
    recieved->payload = malloc(sizeof(char)*(uint32_t)ret_val);
    memcpy(recieved->payload, recvBuffer+4, sizeof(char)*(uint32_t)ret_val-4);
    return callback(self, recieved);
  } else if( ret_val == 0) {
    //XYResult* shutdown_res = socket_close(self);
    //free(shutdown_res);
    recieved->payload = NULL;
    return callback(self, recieved);
  } else {
    RETURN_ERROR(ERR_BOUNDWITNESS_FAILED);
  }
  */
//}

XYResult_t* socket_close(void* self) {
  RETURN_ERROR(OK);
}
