/**
 ****************************************************************************************
 *
 * @file repository.h
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

#include "xyobject.h"
#include "xyo.h"
#include "defines.h"
#ifndef REPOSITORY_H

typedef struct RepositoryProviderT{
  void* repository;
  uint16_t logicalEnd;
  XYResult* (*write)(struct RepositoryProviderT* self, ByteArray* value, uint16_t offset, uint16_t timeout);
  XYResult* (*read)(struct RepositoryProviderT* self, uint16_t offset, uint16_t timeout);
  XYResult* (*readRows)(struct RepositoryProviderT* self, uint16_t beginning, uint16_t end, uint16_t timeout);
  XYResult* (*delete)(struct RepositoryProviderT* self, uint16_t offset, uint16_t timeout);
  XYResult* (*deleteRows)(struct RepositoryProviderT* self, uint16_t beginning, uint16_t end, uint16_t timeout);
} RepositoryProvider;

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

} OriginChainProvider;

OriginChainProvider* initOriginChainProvider( void );
XYResult* append(OriginChainProvider* self, ByteArray* value, uint16_t timeout);
XYResult* getChain(OriginChainProvider* self);
XYResult* deleteChain(OriginChainProvider* self);

#define REPOSITORY_H
#endif
