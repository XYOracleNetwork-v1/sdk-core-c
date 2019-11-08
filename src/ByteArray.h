/**
 * @file ByteArray.h
 * @author Arie Trouw (developer@xy.company)
 * @brief 
 * @version 0.1
 * @date 2018-11-15
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <stdint.h>

typedef struct ByteArray{
  uint32_t size;
  char reserved[2];
  char* payload;
} ByteArray_t;
#endif
