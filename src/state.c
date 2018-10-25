/**
 ****************************************************************************************
 *
 * @file state.c
 *
 * @XY4 project source code.
 *
 * @brief primary state routines for the XY4 firmware.
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
XYResult_t* newOriginBlock(OriginChainState* self_OriginChainState, 
                           ByteArray_t* originBlockHash) {
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self_OriginChainState || !originBlockHash) {RETURN_ERROR(ERR_BADDATA)};

  self_OriginChainState->latestHash = originBlockHash;
  self_OriginChainState->index = self_OriginChainState->index+1;
  self_OriginChainState->currentSigner = self_OriginChainState->nextSigner;
  self_OriginChainState->nextSigner = NULL;
  
  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = 0;
  
  return preallocated_return_result_ptr;
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
XYResult_t* addSigner(OriginChainState* self_OriginChainState, Signer_t* user_Signer) {
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self_OriginChainState || !user_Signer) {RETURN_ERROR(ERR_BADDATA)};

  self_OriginChainState->nextSigner = user_Signer;
  
  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = 0;
  
  return preallocated_return_result_ptr;
}

// end of file state.c

