#include <stdlib.h>
#include "network.h"

proactiveNetworkProvider* newProactiveNetworkProvider(){
  proactiveNetworkProvider* provider = malloc(sizeof(proactiveNetworkProvider));
  provider->RequestConnection = NULL;
  provider->SendData = NULL;
  provider->Disconnect = NULL;
  return provider;
}

reactiveNetworkProvider* newReactiveNetworkProvider(){
  reactiveNetworkProvider* provider = malloc(sizeof(reactiveNetworkProvider));
  provider->Listen = NULL;
  provider->Disconnect = NULL;
  return provider;
}
