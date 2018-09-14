#ifndef NETWORK_H
#include <stddef.h>

typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;
typedef struct NetworkProvider NetworkProvider;

struct NetworkPipe{
  NetworkPeer* peer;
  NetworkProvider* Provider;
  struct ByteArray* initializationData;
  struct XYResult* (*send)(NetworkPipe* self, ByteArray* data, struct ZigZagBoundWitness* session, void (*callback)(struct ZigZagBoundWitnessSession* session, XYResult* data));
  struct XYResult* (*close)();
};

struct NetworkPeer{
  struct XYResult* (*getRole)(NetworkPipe* pipe);
};

struct NetworkProvider{
  struct XYResult* (*find)(int flags);
};

#define NETWORK_H
#endif
