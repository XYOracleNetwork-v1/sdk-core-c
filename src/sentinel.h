#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "relaynode.h"
//#include <arpa/inet.h>

void* initTcpProvider(NetworkProvider** self, int port);

typedef struct Sentinel_Tag {
  NetworkProvider* networkProvider;
  RelayNode* relayNode;
} Sentinel;

Sentinel* initSentinel(NetworkProvider* self, OriginChainProvider_t* originChain, HashProvider_t* hash);
void addPeer(NetworkProvider* self, char* ip, int port);
NetworkPipe_t* findSomeoneToTalkTo( void );
XYResult_t* dummySend( void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data));
XYResult_t* getRole(NetworkPipe_t* pipe);
XYResult_t* socket_send(void* self, ByteArray_t* data, XYResult_t* (*callback)(void* self, ByteArray_t* data));
XYResult_t* socket_close(void* self);

