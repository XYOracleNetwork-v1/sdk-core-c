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
#include "defines.h"

#define RETURN_ERROR(ERR) return_error(ERR, __FILE__, __LINE__);

#define return_error(ERR, __FILE__, __LINE__)               \
if(ERR == ERR_INSUFFICIENT_MEMORY){                         \
  printf(YEL "ERR_INSUFFICIENT_MEMORY @ %s:%d\n" RESET, __FILE__, __LINE__);  \
  preallocated_result->error = ERR_INSUFFICIENT_MEMORY;     \
  preallocated_result->result = 0;                          \
  return preallocated_result;                               \
} else {                                                    \
  XYResult* return_status = malloc(sizeof(XYResult));       \
    if(return_status != NULL){                              \
      if(ERR != OK && ERR != ERR_KEY_DOES_NOT_EXIST )        \
        printf(YEL "%s @ %s:%d\n" RESET, ErrorStrings[ERR], __FILE__, __LINE__);  \
      return_status->error = ERR;                           \
      return_status->result = 0;                            \
      return return_status;                                 \
    }                                                       \
    else {                                                  \
      printf(YEL "ERR_INSUFFICIENT_MEMORY @ %s:%d\n" RESET, __FILE__, __LINE__); \
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY; \
      preallocated_result->result = 0;                      \
      return preallocated_result;                           \
    }                                                       \
}

int littleEndian(void);

#define XYO_H
#endif
