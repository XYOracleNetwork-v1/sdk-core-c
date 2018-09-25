/**
 ****************************************************************************************
 *
 * @file xyo.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef XYO_H
#include <stddef.h>
#include <stdint.h>
#include "xyobject.h"

#define RETURN_ERROR(ERR)                                   \
if(ERR == ERR_INSUFFICIENT_MEMORY){                         \
  preallocated_result->error = ERR_INSUFFICIENT_MEMORY;     \
  preallocated_result->result = 0;                          \
  return preallocated_result;                               \
} else {                                                    \
  XYResult* return_result = malloc(sizeof(XYResult));       \
    if(return_result != NULL){                              \
      return_result->error = ERR;                           \
      return_result->result = 0;                            \
      return return_result;                                 \
    }                                                       \
    else {                                                  \
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY; \
      preallocated_result->result = 0;                      \
      return preallocated_result;                           \
    }                                                       \
}

int littleEndian(void);

#define CATALOG_SIZE 1

#define XYO_H
#endif
