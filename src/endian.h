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
 * @brief Convert 64-bit Value from Big Endian to Platform Endian
 * 
 * @param data Pointer to the uintxx in question
 * @return uint32_t Data pointer read in big endian
 */
uint64_t to_uint64(unsigned char* data);

/**
 * @brief Convert 64-bit Value from Big Endian to Platform Endian
 * 
 * @param data Pointer to the uintxx in question
 * @return uint32_t Data pointer read in big endian
 */
uint32_t to_uint32(unsigned char* data);

/**
 * @brief Convert 64-bit Value from Big Endian to Platform Endian
 * 
 * @param data Pointer to the uintxx in question
 * @return uint32_t Data pointer read in big endian
 */
uint16_t to_uint16(unsigned char* data);

/**
 * @brief Convert 64-bit Value from Platform Endian to Big Endian and write to Dest
 * 
 * @param dest Destination pointer where to be written
 * @param value Platform Endian number to be read
 */
void to_uint64_be(unsigned char* dest, uint64_t value);

/**
 * @brief Convert 32-bit Value from Platform Endian to Big Endian and write to Dest
 * 
 * @param dest Destination pointer where to be written
 * @param value Platform Endian number to be read
 */
void to_uint32_be(unsigned char* dest, uint32_t value);

/**
 * @brief Convert 16-bit Value from Platform Endian to Big Endian and write to Dest
 * 
 * @param dest destination pointer where to be written
 * @param value Platform Endian number to be read
 */
void to_uint16_be(unsigned char* dest, uint16_t value);

/**
 * @brief Detect if the machine running is littleEndian.
 * 
 * @return int endianess 
 */
int littleEndian(void);

#endif
