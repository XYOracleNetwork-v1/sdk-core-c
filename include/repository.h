/**
 ****************************************************************************************
 *
 * @file repository.h
 *
 * @XY4 project source code.
 *
 * @brief bound repository routines for the XY4 firmware.
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

typedef struct RepositoryProvider RepositoryProvider;
typedef struct OriginChainRepository OriginChainRepository;

/*
 * STRUCTURES
 ****************************************************************************************
 */

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
  XYResult_t* (*append)(ByteArray_t* value, uint32_t timeout);
  RepositoryProvider* repo;
};

#endif

// end of file repository.h

