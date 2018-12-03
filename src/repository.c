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
#include "repository.h"

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
OriginChainProvider_t* initOriginChainProvider(){
  
  return NULL;
}

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
XYResult_t* append(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout){
  
  return NULL;
}

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
XYResult_t* getChain(OriginChainProvider_t* self){
  
  return NULL;
}

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
XYResult_t* deleteChain(OriginChainProvider_t* self){
  
  return NULL;
}

// end of file repository.c

