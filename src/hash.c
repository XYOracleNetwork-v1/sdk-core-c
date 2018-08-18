#include "hash.h"
#include <stdlib.h>

char* hashGetId(HashProvider* object){
  return object->id;
}

HashProvider* newHashProvider(){
  HashProvider* hasher = malloc(sizeof(HashProvider));
  hasher->Hash = NULL;
  hasher->VerifyHash = NULL;
  hasher->GetId = &hashGetId;
  hasher->id[0] = 0x00;
  hasher->id[1] = 0x00;
}
