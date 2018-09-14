#include <stdlib.h>
#include "storage.h"

StorageProvider* newStorageProvider(){
  StorageProvider* store = malloc(sizeof(struct StorageProvider));
  store->write = NULL;
  store->read = NULL;
  store->GetAllKeys = NULL;
  store->delete = NULL;
  store->contains = NULL;
  return store;
}
