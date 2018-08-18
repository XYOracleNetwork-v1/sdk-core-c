#include <stddef.h>

typedef struct {
  int (*RequestConnection)(char*);
  char* (*SendData)(int);
  void (*Disconnect)(int);
} proactiveNetworkProvider;

typedef struct {
  int (*Listen)(void*);
  int (*Disconnect)(int);
} reactiveNetworkProvider;
