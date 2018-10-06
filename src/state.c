/**
 ****************************************************************************************
 *
 * @file state.c
 *
 * @XY4 project source code.
 *
 * @brief primary state routines for the XY4 firmware.
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
XYResult_t* newOriginBlock(OriginChainState* self_OriginChainState, ByteArray_t* originBlockHash) {
  
  self_OriginChainState->latestHash = originBlockHash;
  self_OriginChainState->index = self_OriginChainState->index+1;
  self_OriginChainState->currentSigner = self_OriginChainState->nextSigner;
  self_OriginChainState->nextSigner = NULL;
  
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
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
XYResult_t* addSigner(OriginChainState* self_OriginChainState, struct Signer* user_Signer) {
  
  self_OriginChainState->nextSigner = user_Signer;
  
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

// end of file state.c

