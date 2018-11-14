#ifndef BYTEARRAY_H
#define BYTEARRAY_H
typedef struct ByteArray{
  uint32_t size;
  char reserved[2];
  char* payload;
} ByteArray_t;
#endif
