#include <stdio.h>
#include <stdlib.h>
#include "XYObject.h"
#include "../defines.h"

#define XY_DEBUGMODE
#ifdef XY_DEBUGMODE
#define XY_ASSERT_EQUAL(A, B) XY_ASSERT_EQUAL_ex(A, B, __FILE__, __LINE__)
#define XY_ASSERT_EQUAL_ex(A, B, file, line) if(A != B){ printf(RED "%s@%d Assert Failed\n" RESET, file, line); score = score + 1; }
#elif
#define XY_ASSERT_EQUAL(A, B) if(A != B){ score = score + 1; }
#endif

int main(){
  int score = 0;

  XYHeaderFlags_t signatureFlags;
  signatureFlags.typed = 0;
  signatureFlags.iteratable = 1;
  signatureFlags.lengthType = 1;

  XYObjectHeader_t signatureHeader;
  signatureHeader.flags = signatureFlags;
  signatureHeader.type = 0x12;

  XYObject_t signatureObject;
  signatureObject.header = &signatureHeader;
  signatureObject.payload = malloc(sizeof(char)*4);
  *(char*)&signatureObject.payload[0] = 1;
  *(char*)&signatureObject.payload[1] = 2;
  *(char*)&signatureObject.payload[2] = 3;
  *(char*)&signatureObject.payload[3] = 4;
  XYObject_t* self = &signatureObject;
  
  uint32_t size32 = XYOBJ_READ_UINT32();
  uint32_t size16 = XYOBJ_READ_UINT16();
  uint32_t size8 = XYOBJ_READ_UINT8();

  XY_ASSERT_EQUAL(size32, 16909060);
  XY_ASSERT_EQUAL(size16, 258);
  XY_ASSERT_EQUAL(size8, 1);
  return score;



  
}