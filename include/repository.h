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
#include "xyobject.h"

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
  XYResult* (*append)(struct OriginChainProviderT* self, ByteArray* value, uint16_t timeout);
  XYResult* (*getChain)(struct OriginChainProviderT* self);
  XYResult* (*deleteChain)(struct OriginChainProviderT* self);

  /* Externally implemented methods. These methods are
   * called into from outside the library.
   */
  XYResult* (*saveChain)(struct OriginChainProviderT* self);
  XYResult* (*reconstructChain)(struct OriginChainProviderT* self);
  char optionalBits[ORIGINCHAIN_EXTRA_BITS];
  uint16_t logicalEnd;

struct RepositoryProvider{
  uint32_t repository;
  uint32_t logicalEnd;
  XYResult_t* (*write)(ByteArray_t* value, uint32_t offset, uint32_t timeout);
  XYResult_t* (*read)(uint32_t offset, uint32_t timeout);
  XYResult_t* (*readRows)(uint32_t beginning, uint32_t end, uint32_t timeout);
  XYResult_t* (*delete)(uint32_t offset, uint32_t timeout);
  XYResult_t* (*deleteRows)(uint32_t beginning, uint32_t end, uint32_t timeout);
};

struct OriginChainRepository {
  XYResult* (*append)(ByteArray* value, uint timeout);
  RepositoryProvider* repo;
};

#define REPOSITORY_H
#endif

// end of file repository.h

