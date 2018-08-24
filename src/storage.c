#include <stdlib.h>
#include "storage.h"

storageProvider* newStorageProvider(){
  storageProvider* store = malloc(sizeof(storageProvider));
  store->put = NULL;
  store->get = NULL;
  store->GetAllKeys = NULL;
  store->remove = NULL;
  store->contains = NULL;
  return store;
}
