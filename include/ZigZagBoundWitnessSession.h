/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.h
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

#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include <stdint.h>
#include "xyo.h"
#include "ZigZagBoundWitness.h"
#include "network.h"

typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession;
typedef struct NetworkPipe NetworkPipe;
typedef struct NetworkPeer NetworkPeer;

struct ZigZagBoundWitnessSession {
  XYResult* (*completeBoundWitness)(ZigZagBoundWitnessSession* userSession, ByteArray* bwData);
  NetworkPipe* NetworkPipe;
  ZigZagBoundWitness* boundWitness;
  uint8_t cycles;
  uint8_t choice;
};

struct XYResult* receiverCallback(void* self, ByteArray* data);
struct XYResult* completeBoundWitness(ZigZagBoundWitnessSession* userSession, ByteArray* boundWitnessData);

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
