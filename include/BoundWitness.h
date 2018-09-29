/**
 ****************************************************************************************
 *
 * @file boundwitness.h
 *
 * @XY4 project source code.
 *
 * @brief primary bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef BOUNDWITNESS_H
#define BOUNDWITNESS_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "hash.h"
#include "xyo.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct BoundWitness BoundWitness;
typedef struct BoundWitnessTransfer BoundWitnessTransfer;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct BoundWitness {
  uint32_t size;
  ShortStrongArray_t* publicKeys;
  IntStrongArray_t* payloads;
  ShortStrongArray_t* signatures;
  XYResult_t* (*getSigningData)(void* user_BoundWitness);
  XYResult_t* (*getHash)(BoundWitness*, HashProvider*);
} ;

struct BoundWitnessTransfer {
  uint32_t size;
  ShortStrongArray_t* publicKeys;
  IntStrongArray_t* payloads;
  ShortStrongArray_t* signatures;
  XYResult_t* (*getSigningData)(void* user_BoundWitness);
  XYResult_t* (*getHash)(BoundWitness*, HashProvider*);
  uint8_t  choice;
} ;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* BoundWitnessTransfer_create(char id[2], void* user_data);
XYResult_t* BoundWitnessTransfer_fromBytes(char* user_Transfer);
XYResult_t* BoundWitnessTransfer_toBytes(XYObject_t* user_XYObect);

XYResult_t* BoundWitness_creator_create(char id[2], void* user_data);
XYResult_t* BoundWitness_creator_fromBytes(char* BoundWitness_data);
XYResult_t* BoundWitness_creator_toBytes(struct XYObject* user_XYObect);
XYResult_t* BoundWitness_getSigningData(void* user_BoundWitness);
XYResult_t* BoundWitness_getHash(BoundWitness* user_BoundWitness, HashProvider*);

#endif

// end of file boundwitness.h

