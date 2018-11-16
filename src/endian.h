/**
 * @file endian.h
 * @author Arie Trouw (developer@xy.company)
 * @brief Convert 16 and 32 but numebrs between endians
 * @version 0.1
 * @date 2018-11-16
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

/*
 * FUNCTIONS
 ****************************************************************************************
 */

/**
 * @brief Convert 32-bit Value from Big Endian to Platform Endian
 * 
 * @param char 
 * @return uint32_t 
 */
uint32_t to_uint32(unsigned char* data);

/**
 * @brief Convert 16-bit Value from Big Endian to Platform Endian
 * 
 * @param char 
 * @return uint16_t 
 */
uint16_t to_uint16(unsigned char* data);

/**
 * @brief Write 32-Bit Big Endian value to a buffer from a Platform Endian source
 * 
 * @param char 
 * @param value 
 */
void to_uint32_be(unsigned char* dest, uint32_t value);

/**
 * @brief Write 16-Bit Big Endian value to a buffer from a Platform Endian source
 * 
 * @param char 
 * @param value 
 */
void to_uint16_be(unsigned char* dest, uint16_t value);

#endif