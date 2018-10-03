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
  uint logicalEnd;
  XYResult* (*write)(struct RepositoryProviderT* self, ByteArray* value, uint offset, uint timeout);
  XYResult* (*read)(struct RepositoryProviderT* self, uint offset, uint timeout);
  XYResult* (*readRows)(struct RepositoryProviderT* self, uint beginning, uint end, uint timeout);
  XYResult* (*delete)(struct RepositoryProviderT* self, uint offset, uint timeout);
  XYResult* (*deleteRows)(struct RepositoryProviderT* self, uint beginning, uint end, uint timeout);
} RepositoryProvider;

typedef struct OriginChainProviderT {
  void* repository;
  XYResult* (*append)(struct OriginChainProviderT* self, ByteArray* value, uint timeout);
  XYResult* (*getChain)(struct OriginChainProviderT* self);
  XYResult* (*deleteChain)(struct OriginChainProviderT* self);

  /* Externally implemented methods. These methods are
   * called into from outside the library.
   */
  XYResult* (*saveChain)(struct OriginChainProviderT* self);
  XYResult* (*reconstructChain)(struct OriginChainProviderT* self);
  char optionalBits[ORIGINCHAIN_EXTRA_BITS];
  uint logicalEnd;

} OriginChainProvider;

OriginChainProvider* initOriginChainProvider();
XYResult* append(OriginChainProvider* self, ByteArray* value, uint timeout);
XYResult* getChain(OriginChainProvider* self);
XYResult* deleteChain(OriginChainProvider* self);

#define REPOSITORY_H
#endif
