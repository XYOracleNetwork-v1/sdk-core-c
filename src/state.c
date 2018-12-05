/**
 ****************************************************************************************
 *
 * @file state.c
 *
 * @XYO Core library source code.
 *
 * @brief primary state routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "state.h"

/*----------------------------------------------------------------------------*
*  NAME
*      newOriginBlock
*
*  DESCRIPTION
*      Append block hash to xyo network state object and update index/signer.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult_t newOriginBlock(OriginChainState_t* self_OriginChainState,
                           ByteArray_t* originBlockHash) {

  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
}

/*----------------------------------------------------------------------------*
*  NAME
*      addSigner
*
*  DESCRIPTION
*      Add a signer object to the state.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       user_Signer*
*
*  RETURNS
*      XYResult_t*                             [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult_t addSigner(OriginChainState_t* self_OriginChainState, Signer_t* user_Signer) {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
}

/**
 ****************************************************************************************
 *  NAME
 *      getSigners
 *
 *  DESCRIPTION
 *  Gets the current Signer object in use.
 *
 *  PARAMETERS
 *      self      [in]      OriginChainState_t*
 *
 *  RETURNS
 *      Signer          [out]     XYResult<Signer*>
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 XYResult_t getSigners(OriginChainState_t* self_OriginChainState){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
 }
// end of file state.c
