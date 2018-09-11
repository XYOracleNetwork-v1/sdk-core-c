#ifndef NETWORK_H
#include <stddef.h>

typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;
typedef struct NetworkProvider NetworkProvider;

struct NetworkPipe{
  NetworkPeer* peer;
  NetworkProvider* Provider;
  struct ByteArray* initializationData;
  struct XYResult* (*send)(NetworkPipe* self, ByteArray* data, void (*callback)(ByteArray* data));
  struct XYResult* (*close)();
};

struct NetworkPeer{
  struct XYResult* (*getRole)(NetworkPipe* pipe);
};

struct NetworkProvider{
  struct XYResult* (*find)(int flags);
  struct XYResult* (*send)(ByteArray* data, void (*callback)(ByteArray* data));
  struct XYResult* (*close)();
};

#define NETWORK_H
#endif
