#include <stdlib.h>
#include <string.h>
#include "serializer.h"
#include "xyobject.h"

void initTable(){
  int x,y;
  for(x = 0; x < 255; x ++) {
      for(y = 0; y < 255; y ++) typeTable[x][y] = 0;
  }
}

void registerType(char id[2], char*(*f)()){
  typeTable[id[0]][id[1]] = f;
}

void* getType(char id[2]){
  return typeTable[id[0]][id[1]];
}

void ArrayItrNext(){

}

uint32_t to_uint32(char* data) {
  return data[0] + 256U*data[1] + 65536U*data[2] + 16777216U*data[3];
}

uint16_t to_uint16(char* data) {
  return data[0] + 256U*data[1];
}

BoundWitness* boundWitnessUnpacker(char* data){
  BoundWitness* bw = malloc(sizeof(BoundWitness));
  bw->size = to_uint32(data);
  //bw->keys = unpack(data+(sizeof(uint32_t)));

}
