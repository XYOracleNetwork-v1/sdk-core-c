#include "xyo.h"
#include <stdlib.h>
#include <string.h>

char* GetId(struct XYObject* object){
  return object->id;
}

void* GetPayload(struct XYObject* object){
  return object->payload;
}

XYResult* newObject(char id[2], void* payload){
  struct XYObject* new_object = malloc(sizeof(struct XYObject));
  if(new_object != NULL){
    if(!payload){
      new_object->payload = payload;
    }
    else{
      return NULL;
    }
    if(id[0] && id[1])
    {
      return NULL;
    }
    else
    {
      memcpy(new_object->id, id, 2);
    }
    new_object->GetId = &GetId;
    new_object->GetPayload = &GetPayload;
    preallocated_result->error = OK;
    preallocated_result->result = &new_object;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL:
  }

}

void initTable(){
  int x,y;
  for(x = 0; x < 255; x ++) {
      for(y = 0; y < 255; y ++) typeTable[x][y] = 0;
  }
}

void registerType(char id[2], void* creator){
  typeTable[id[0]][id[1]] = creator;
}

void* getType(char id[2]){
  return typeTable[id[0]][id[1]];
}

void ArrayIteratorNext(){

}

void* lookup(char id[2]){
  return typeTable[id[0]][id[1]];
}
