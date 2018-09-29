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

#include <stdint.h>
#include "xyo.h"
#include "ZigZagBoundWitness.h"
#include "network.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession;
typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;

struct ZigZagBoundWitnessSession {
  XYResult_t* (*completeBoundWitness)(ZigZagBoundWitnessSession* userSession, ByteArray_t* bwData);
  NetworkPipe* NetworkPipe;
  ZigZagBoundWitness* BoundWitness;
  uint8_t cycles;
  ByteArray_t* choice;
};

struct NetworkPipe{
  NetworkPeer* peer;
  NetworkProvider* Provider;
  ByteArray_t* initializationData;
  XYResult_t* (*send)(ZigZagBoundWitnessSession* self, ByteArray_t* data, XYResult_t* (*callback)(ZigZagBoundWitnessSession* self, ByteArray_t* data));
  XYResult_t* (*close)();
};

struct NetworkPeer{
  XYResult_t* (*getRole)(NetworkPipe* pipe);
};

XYResult_t* receiverCallback(ZigZagBoundWitnessSession* self, ByteArray_t* data);

#endif
