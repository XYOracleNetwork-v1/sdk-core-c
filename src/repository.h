/**
 ****************************************************************************************
 *
 * @file repository.c
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
#include <stdint.h>
#include "state.h"
#include "ZigZagBoundWitnessSession.h"

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
  XYResult_t* (*append)(ByteArray_t* value, uint16_t timeout);
  struct RepositoryProvider_t* repo;
};

/*----------------------------------------------------------------------------*
*  NAME
*      initOriginChainProvider
*
*  DESCRIPTION
*     Initializes an origin chain provider as a Int Strong Array
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
OriginChainProvider_t* initOriginChainProvider( void );

/*----------------------------------------------------------------------------*
*  NAME
*      write
*
*  DESCRIPTION
*      Write block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
/*
XYResult_t* write(OriginChainProvider_t* self, ByteArray_t* value, uint16_t offset, uint16_t timeout);
*/

/*----------------------------------------------------------------------------*
*  NAME
*      read
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
/*
XYResult_t* read(RepositoryProvider_t* self, uint16_t offset, uint16_t timeout);
*/

/*----------------------------------------------------------------------------*
*  NAME
*      append
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult_t* append(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout);

/*----------------------------------------------------------------------------*
*  NAME
*      getChain
*
*  DESCRIPTION
*      Get the location of the origin chain on disk and return the array object.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* getChain(OriginChainProvider_t* self);

/*----------------------------------------------------------------------------*
*  NAME
*      deleteChain
*
*  DESCRIPTION
*      Nullify and clear the origin chain.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* deleteChain(OriginChainProvider_t* self);



#define REPOSITORY_H
#endif

// end of file repository.h
