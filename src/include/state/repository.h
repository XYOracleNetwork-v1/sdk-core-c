/**
 ****************************************************************************************
 *
 * @file repository.h
 *
 * @XYO Core library source code.
 *
 * @brief primary XYO block management routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <stdint.h>
#include "objects.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * STRUCTURES
 ****************************************************************************************
 */

typedef struct OriginChainProviderT {
  void* repository;
  XYResult_t* (*appendToOriginChainInFlash)(struct OriginChainProviderT* self, ByteArray_t* value, uint16_t timeout);
  XYResult_t* (*getTheOriginChainFromFlash)(struct OriginChainProviderT* self);
  XYResult_t* (*deleteTheOriginChainFromFlash)(struct OriginChainProviderT* self);

  /* Externally implemented methods. These methods are
   * called into from outside the library.
   */
  XYResult_t* (*saveChain)(struct OriginChainProviderT* self);
  XYResult_t* (*reconstructChain)(struct OriginChainProviderT* self);
  char optionalBits[ORIGINCHAIN_EXTRA_BITS];
  uint16_t logicalEnd;
} OriginChainProvider_t;

typedef struct RepositoryProviderT {
  uint32_t repository;
  uint32_t logicalEnd;
  XYResult_t* (*write)(ByteArray_t* value, uint32_t offset, uint32_t timeout);
  XYResult_t* (*read)(uint32_t offset, uint32_t timeout);
  XYResult_t* (*readRows)(uint32_t beginning, uint32_t end, uint32_t timeout);
  XYResult_t* (*delete)(uint32_t offset, uint32_t timeout);
  XYResult_t* (*deleteRows)(uint32_t beginning, uint32_t end, uint32_t timeout);
} RepositoryProvider_t;

struct OriginChainRepository {
  XYResult_t* (*append)(ByteArray_t* value, uint16_t timeout);
  struct RepositoryProvider_t* repo;
};

/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */

XYResult_t* appendToOriginChainInFlash(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout);
XYResult_t* getTheOriginChainFromFlash(OriginChainProvider_t* self);
XYResult_t* deleteTheOriginChainFromFlash(OriginChainProvider_t* self);
OriginChainProvider_t* initOriginChainProvider( void );
XYResult_t* readXyoBlockFromFlash(uint32_t blockNumber,
                                  uint32_t NumberOfBlocksToRead, 
                                  uint8_t* destinationReadBuffer);

#endif

// end of file repository.h

