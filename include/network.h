/**
 ****************************************************************************************
 *
 * @file crypto.c
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

#ifndef NETWORK_H
#include <stdint.h>
typedef struct NetworkProvider NetworkProvider;

struct NetworkProvider{
  struct XYResult* (*find)(int flags);
};

#define NETWORK_H
#endif
