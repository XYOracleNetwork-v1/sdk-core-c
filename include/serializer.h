#include "xyo.h"

void* typeTable[255][255] = { 0 };

struct ArrayItr* unpackArray(char* data);

union SizeUnion {
    int sizeIdentifierSize;
    int defaultSize;
}

struct SZSNSArrayCreator {
  char id[2];
  union SizeUnion sizeBytes;
  XYObject* (*create)(char[2]);
  XYObject* (*createFromBytes)(char*);
  char*     (*encode)(XYObject*)
};
