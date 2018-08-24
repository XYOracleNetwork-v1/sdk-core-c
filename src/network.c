#include <stdlib.h>
#include "network.h"

proactiveNetworkProvider* newProactiveNetworkProvider(){
  proactiveNetworkProvider* provider = malloc(sizeof(proactiveNetworkProvider));
  provider->requestConnection = NULL;
  provider->sendData = NULL;
  provider->disconnect = NULL;
  return provider;
}

reactiveNetworkProvider* newReactiveNetworkProvider(){
  reactiveNetworkProvider* provider = malloc(sizeof(reactiveNetworkProvider));
  provider->listen = NULL;
  provider->disconnect = NULL;
  return provider;
}
