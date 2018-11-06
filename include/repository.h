/**
 ****************************************************************************************
 *
 * @file repository.h
 *
 * @XYO Core library source code.
 *
 * @brief primary repository routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdint.h>
#include "objects.h"
#include "xyo.h"

#ifndef REPOSITORY_H
#define REPOSITORY_H

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
  XYResult_t* (*append)(struct OriginChainProviderT* self, ByteArray_t* value, uint16_t timeout);
  XYResult_t* (*getChain)(struct OriginChainProviderT* self);
  XYResult_t* (*deleteChain)(struct OriginChainProviderT* self);

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
  XYResult_t* (*append)(ByteArray_t* value, uint timeout);
  struct RepositoryProvider_t* repo;
};

XYResult_t* append(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout);
XYResult_t* getChain(OriginChainProvider_t* self);
XYResult_t* deleteChain(OriginChainProvider_t* self);
OriginChainProvider_t* initOriginChainProvider( void );

#define REPOSITORY_H
#endif

// end of file repository.h
