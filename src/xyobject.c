#include "XYOHeuristicsBuilder.h"
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
    return preallocated_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

}

XYResult* initTable(){
  int x,y;
  for(x = 0; x < 16; x ++) {
      for(y = 0; y < 16; y ++) typeTable[x][y] = 0;
  }

  struct Object_Creator* rssi_creator = malloc(sizeof(Object_Creator));
  if(rssi_creator != NULL){
    rssi_creator->sizeIdentifierSize = 0;
    rssi_creator->defaultSize = 1;
    rssi_creator->create = &Heuristic_RSSI_Creator_create;
    rssi_creator->fromBytes = &Heuristic_RSSI_Creator_fromBytes;
    rssi_creator->toBytes = &Heuristic_RSSI_Creator_toBytes;
    typeTable[9][1] = rssi_creator; //TODO: Change major and minor for RSSI when it's standardized.

    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = OK;
      return_result->result = 0;
      return return_result;
    }
    else {
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = 0;
      return preallocated_result;
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Byte Strong Array Creator
  struct Object_Creator* ByteStrongArray_creator = malloc(sizeof(Object_Creator));
  if(ByteStrongArray_creator != NULL){
    ByteStrongArray_creator->sizeIdentifierSize = 1;
    ByteStrongArray_creator->defaultSize = 0;
    ByteStrongArray_creator->create = &ByteStrongArray_creator_create;
    ByteStrongArray_creator->fromBytes = &ByteStrongArray_creator_fromBytes;
    ByteStrongArray_creator->toBytes = 0;
    typeTable[9][1] = rssi_creator; //TODO: Change major and minor for RSSI when it's standardized.

    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = OK;
      return_result->result = 0;
      return return_result;
    }
    else {
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = 0;
      return preallocated_result;
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
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

XYResult* lookup(char id[2]){
  void* tableValue = typeTable[id[0]][id[1]];
  if(tableValue != 0){

  }
  else {
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result != NULL){
      return_result->error = ERR_KEY_DOES_NOT_EXIST;
      return_result->result = 0;
      return return_result;
    }
    else {
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
      preallocated_result->result = 0;
      return preallocated_result;
    }
  }
  return typeTable[id[0]][id[1]];
}
