/**
 ****************************************************************************************
 *
 * @file xyo.h
 *
 * @XY4 project source code.
 *
 * @brief primary xyo routines for the XY4 firmware.  note that there currently is no xyo.c
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef XYO_H
#define XYO_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stddef.h>
#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define CATALOG_SIZE 1

#define RETURN_ERROR(ERR)                                 \
if(ERR == ERR_INSUFFICIENT_MEMORY){                       \
  preallocated_result->error = ERR_INSUFFICIENT_MEMORY;   \
  preallocated_result->result = 0;                        \
  return preallocated_result;                             \
} else {                                                  \
  XYResult_t* return_result = malloc(sizeof(XYResult_t));     \
    if(return_result != NULL){                            \
      return_result->error = ERR;                         \
      return_result->result = 0;                          \
      return return_result;                               \
    }                                                     \
    else {                                                \
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY;\
      preallocated_result->result = 0;                     \
      return preallocated_result;                          \
    }                                                      \
}                                                          \

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

int littleEndian(void);

#endif

// end of file xyo.h

