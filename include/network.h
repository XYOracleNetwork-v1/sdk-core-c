/**
 ****************************************************************************************
 *
 * @file network.h
 *
 * @XY4 project source code.
 *
 * @brief primary network routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
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

typedef struct proactiveNetworkProvider proactiveNetworkProvider_t;
typedef struct reactiveNetworkProvider reactiveNetworkProvider_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct proactiveNetworkProvider{
  
  // temporary placeholders
  uint32_t requestConnection;
  uint32_t sendData;
  uint32_t disconnect;
};

struct reactiveNetworkProvider{
  
  // temporary placeholders
  uint32_t listen;
  uint32_t disconnect;
};

XYResult_t* newProactiveNetworkProvider(void);
XYResult_t* newReactiveNetworkProvider(void);

#endif

// end of file network.h

