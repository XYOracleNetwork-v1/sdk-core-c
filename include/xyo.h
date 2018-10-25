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

/*
 * DEFINES
 ****************************************************************************************
 */

#define CATALOG_SIZE 1

#define RETURN_ERROR(ERR)                                 \
if(ERR == ERR_INSUFFICIENT_MEMORY){                       \
  preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;   \
  preallocated_return_result_ptr->result = 0;                        \
  return preallocated_return_result_ptr;                             \
} else {                                                  \
  preallocated_return_result_ptr->error = ERR;                         \
  preallocated_return_result_ptr->result = 0;                          \
  return preallocated_return_result_ptr;                               \
}                                                          \

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

int littleEndian(void);

#endif

// end of file xyo.h

