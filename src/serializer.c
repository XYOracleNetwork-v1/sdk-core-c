#include "serializer.h"

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
