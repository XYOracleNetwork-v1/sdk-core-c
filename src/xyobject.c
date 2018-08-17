#include "xyo.h"
#include <stdlib.h>
#include <string.h>

char* GetId(XYObject* object){
  return object->id;
}

void* GetPayload(XYObject* object){
  return object->payload;
}

XYObject* newObject(char id[2], void* payload){
  XYObject* obj = malloc(sizeof(XYObject));
  if(!payload){
    obj->payload = payload;
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
    memcpy(obj->id, id, 2);
  }
  obj->GetId = &GetId;
  obj->GetPayload = &GetPayload;
}
