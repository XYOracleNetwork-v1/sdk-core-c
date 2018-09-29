/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.h
 *
 * @XY4 project source code.
 *
 * @brief bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "xyobject.h"

#ifndef REPOSITORY_H
#define REPOSITORY_H

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct RepositoryProvider RepositoryProvider;
typedef struct OriginChainRepository OriginChainRepository;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct RepositoryProvider{
  uint repository;
  uint logicalEnd;
  XYResult_t* (*write)(ByteArray_t* value, uint offset, uint timeout);
  XYResult_t* (*read)(uint offset, uint timeout);
  XYResult_t* (*readRows)(uint beginning, uint end, uint timeout);
  XYResult_t* (*delete)(uint offset, uint timeout);
  XYResult_t* (*deleteRows)(uint beginning, uint end, uint timeout);
};

struct OriginChainRepository {
  XYResult_t* (*append)(ByteArray_t* value, uint timeout);
  RepositoryProvider* repo;
};

#endif
