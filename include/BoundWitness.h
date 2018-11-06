/**
 ****************************************************************************************
 *
 * @file boundwitness.h
 *
 * @XYO Core library source code.
 *
 * @brief primary bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
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
#include <sys/types.h>
#include <stddef.h>

//#include <xyo/relaynode.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "hash.h"
#include "xyobject.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct BoundWitness BoundWitness_t;
typedef struct BoundWitnessTransfer BoundWitnessTransfer_t;
typedef struct BoundWitnessHeaders BoundWitnessHeaders_t;
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
  XYResult_t* (*getHash)(BoundWitness_t*, HashProvider_t*);
} ;

struct BoundWitnessTransfer {
  uint32_t size;
  ShortStrongArray_t* publicKeys;
  IntStrongArray_t* payloads;
  ShortStrongArray_t* signatures;
  XYResult_t* (*getSigningData)(void* user_BoundWitness);
  XYResult_t* (*getHash)(BoundWitness_t*, HashProvider_t*);
  uint8_t  choice;
} ;

struct BoundWitnessHeaders {
  void* publicKeys;
  void* payloads;
  void* signatures;
} ;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* BoundWitness_getSigningData(void* user_BoundWitness);
XYResult_t* BoundWitness_getHash(BoundWitness_t* user_BoundWitness, HashProvider_t*);

#define GET_BW_PUBLICKEY(DATA, MAJOR, MINOR) BoundWitness_parse_expanded
#endif

// end of file boundwitness.h
