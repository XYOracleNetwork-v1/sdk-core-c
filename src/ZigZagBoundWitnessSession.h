/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.h
 *
 * @XYO Core library source code.
 *
 * @brief bound witness session routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef ZIGZAGBOUNDWITNESSSESSION_H

/*
 * INCLUDES
 ****************************************************************************************
 */

//#include "network.h"                // includes <stdint.h>
#include "xyresult.h"
#include "XYObjects/XYObject.h"
#include "arrays.h"
#include "ByteArray.h"
#include "hash.h"
#include "crypto.h"
#include "include/objects/BoundWitness.h"
#include "include/objects/ZigZagBoundWitness.h"     // "crypto.h", "BoundWitness.h", "xyobject.h", "hash.h"
#include "relaynode.h"
#include "include/node/nodebase.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

//typedef struct ZigZagBoundWitnessSession ZigZagBoundWitnessSession_t;
//typedef struct NetworkPipe NetworkPipe_t;
//typedef struct NetworkPeer NetworkPeer_t;

/*
 * STRUCTURES
 ****************************************************************************************
 * /

struct ZigZagBoundWitnessSession {
  
}
/*
struct NetworkPipe{
  NetworkPeer_t* peer;
  proactiveNetworkProvider_t* Provider;   //TODO: wal, this doesn't seem to get used here?
  ByteArray_t* initializationData;
  XYResult_t*(*send)(ZigZagBoundWitnessSession_t* self,
              ByteArray_t* data,
              XYResult_t* (*callback)(ZigZagBoundWitnessSession_t* self,
              ByteArray_t* data));
  XYResult_t* (*close)(void);
};

struct NetworkPeer{
  XYResult_t* (*getRole)(NetworkPipe_t* pipe);
};
*/
/****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

//XYResult_t* receiverCallback(ZigZagBoundWitnessSession_t* self, ByteArray_t* data);


#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
