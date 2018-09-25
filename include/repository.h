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
typedef struct RepositoryProvider RepositoryProvider;
typedef struct OriginChainProvider OriginChainProvider;

struct RepositoryProvider{
  void* repository;
  uint logicalEnd;
  XYResult* (*write)(RepositoryProvider* self, ByteArray* value, uint offset, uint timeout);
  XYResult* (*read)(RepositoryProvider* self, uint offset, uint timeout);
  XYResult* (*readRows)(RepositoryProvider* self, uint beginning, uint end, uint timeout);
  XYResult* (*delete)(RepositoryProvider* self, uint offset, uint timeout);
  XYResult* (*deleteRows)(RepositoryProvider* self, uint beginning, uint end, uint timeout);
};

struct OriginChainProvider{
  RepositoryProvider* repository;
  XYResult* (*append)(OriginChainProvider* self, ByteArray* value, uint timeout);
  XYResult* (*getChain)(OriginChainProvider* self);
  XYResult* (*deleteChain)(OriginChainProvider* self);

};

#define REPOSITORY_H
#endif
