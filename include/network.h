/**
 ****************************************************************************************
 *
 * @file network.h
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
#define NETWORK_H

#include <stdint.h>
#include "xyobject.h"

typedef struct NetworkProvider NetworkProvider;
typedef struct ProcedureCatalogue ProcedureCatalogue;

struct NetworkProvider {
  struct XYResult* (*find)(int flags);
};

struct ProcedureCatalogue {

};

#endif
