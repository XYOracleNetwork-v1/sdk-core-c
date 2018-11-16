/**
 * @file endian.c
 * @author Arie Trouw (developer@xy.company)
 * @brief Convert 16 and 32 but numebrs between endians
 * @version 0.1
 * @date 2018-11-16
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */
#include "endian.h"

uint64_t to_uint64(unsigned char *data)
{
  return ((uint64_t)data[0] << 56) + ((uint64_t)data[1] << 48) + ((uint64_t)data[2] << 40) + ((uint64_t)data[3] << 32) + ((uint64_t)data[4] << 24) + ((uint64_t)data[5] << 16) + ((uint64_t)data[6] << 8) + (uint64_t)data[7];
}

uint32_t to_uint32(unsigned char *data)
{
  return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
}

uint16_t to_uint16(unsigned char *data)
{
  return (data[0] << 8) + data[1];
}

void to_uint64_be(unsigned char *dest, uint64_t value)
{
  dest[0] = (unsigned char)((value & 0xff00000000000000) >> 56);
  dest[1] = (unsigned char)((value & 0xff000000000000) >> 48);
  dest[2] = (unsigned char)((value & 0xff0000000000) >> 40);
  dest[3] = (unsigned char)((value & 0xff00000000) >> 32);
  dest[4] = (unsigned char)((value & 0xff000000) >> 24);
  dest[5] = (unsigned char)((value & 0xff0000) >> 16);
  dest[6] = (unsigned char)((value & 0xff00) >> 8);
  dest[7] = (unsigned char)(value & 0xff);
}

void to_uint32_be(unsigned char *dest, uint32_t value)
{
  dest[0] = (unsigned char)((value & 0xff000000) >> 24);
  dest[1] = (unsigned char)((value & 0xff0000) >> 16);
  dest[2] = (unsigned char)((value & 0xff00) >> 8);
  dest[3] = (unsigned char)(value & 0xff);
}

void to_uint16_be(unsigned char *dest, uint16_t value)
{
  dest[0] = (unsigned char)((value & 0xff00) >> 8);
  dest[1] = (unsigned char)(value & 0xff);
}