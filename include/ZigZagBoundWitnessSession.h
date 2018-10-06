/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.h
 *
 * @XY4 project source code.
 *
 * @brief bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#define ZIGZAGBOUNDWITNESSSESSION_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "network.h"                // includes <stdint.h>
#include "ZigZagBoundWitness.h"     // "crypto.h", "BoundWitness.h", "xyobject.h", "hash.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession_t;
typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct ZigZagBoundWitnessSession {
  XYResult_t*(*completeBoundWitness)(ZigZagBoundWitnessSession_t* userSession, 
              ByteArray_t* bwData);
  NetworkPipe* NetworkPipe;
  ZigZagBoundWitness_t* BoundWitness;
  uint8_t cycles;
  ByteArray_t* choice;
};

struct NetworkPipe{
  NetworkPeer* peer;
  proactiveNetworkProvider_t* Provider;   //TODO: wal, this doesn't seem to get used here?
  ByteArray_t* initializationData;
  XYResult_t*(*send)(ZigZagBoundWitnessSession_t* self, 
              ByteArray_t* data, 
              XYResult_t* (*callback)(ZigZagBoundWitnessSession_t* self, 
              ByteArray_t* data));
  XYResult_t* (*close)();
};

struct NetworkPeer{
  XYResult_t* (*getRole)(NetworkPipe* pipe);
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* receiverCallback(ZigZagBoundWitnessSession_t* self, ByteArray_t* data);
XYResult_t* completeBoundWitness(ZigZagBoundWitnessSession_t* userSession, 
                                 ByteArray_t* boundWitnessData);

#endif

// end of file ZigZagBoundWitnessSession.h

