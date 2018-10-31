/**
 ****************************************************************************************
 *
 * @file boundwitness.h
 *
 * @XYO Core library source code.
 *
 * @brief primary bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

#ifndef BOUNDWITNESS_H
#define BOUNDWITNESS_H

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdint.h>
#include "hash.h"   
#include "xyobject.h"   

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
  XYResult_t* (*getHash)(BoundWitness*, HashProvider_t*);
} ;

struct BoundWitnessTransfer {
  uint32_t size;
  ShortStrongArray_t* publicKeys;
  IntStrongArray_t* payloads;
  ShortStrongArray_t* signatures;
  XYResult_t* (*getSigningData)(void* user_BoundWitness);
  XYResult_t* (*getHash)(BoundWitness*, HashProvider_t*);
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
XYResult_t* BoundWitness_getHash(BoundWitness* user_BoundWitness, HashProvider_t*);

#endif

// end of file boundwitness.h

