#ifndef NETWORK_H
#include <stdint.h>
typedef struct NetworkProvider NetworkProvider;

struct NetworkProvider{
  struct XYResult* (*find)(int flags);
};

#define NETWORK_H
#endif
