/**
 ****************************************************************************************
 *
 * @file xyo.h
 *
 * @XY4 project source code.
 *
 * @brief primary xyo routines for the XY4 firmware.  note that there currently is no xyo.c
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
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
#include "xyobject.h"

/*
 * DEFINES
 ****************************************************************************************
 */

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

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

int littleEndian(void);

#endif

// end of file xyo.h

