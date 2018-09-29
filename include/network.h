/**
 ****************************************************************************************
 *
 * @file network.h
 *
 * @XY4 project source code.
 *
 * @brief primary crypto routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef NETWORK_H
#define NETWORK_H

/*
 * DEFINES
 ****************************************************************************************
 */

#include <stdint.h>
#include "xyobject.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct NetworkProvider NetworkProvider;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct NetworkProvider{
  XYResult_t* (*find)(int flags);
};

#endif

// end of file network.h

