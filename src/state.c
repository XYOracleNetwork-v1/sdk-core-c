/**
 ****************************************************************************************
 *
 * @file state.c
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
#include "state.h"

/*----------------------------------------------------------------------------*
*  NAME
*      newOriginBlock
*
*  DESCRIPTION
*      Append block hash to xyo network state object and update index/signer.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult* newOriginBlock(OriginChainState* self_OriginChainState, ByteArray* originBlockHash) {
  self_OriginChainState->latestHash = originBlockHash;
  self_OriginChainState->index = self_OriginChainState->index+1;
  self_OriginChainState->currentSigner = self_OriginChainState->nextSigner;
  self_OriginChainState->nextSigner = NULL;
  XYResult* return_result = malloc(sizeof(XYResult));
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
*      XYResult*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult* addSigner(OriginChainState* self_OriginChainState, struct Signer* user_Signer) {
  self_OriginChainState->nextSigner = user_Signer;
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
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
 *      self      [in]      OriginChainState*
 *
 *  RETURNS
 *      Signer          [out]     XYResult<Signer*>
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 XYResult* getSigners(OriginChainState* self_OriginChainState){
   if(self_OriginChainState->currentSigner == NULL) { RETURN_ERROR(ERR_NOSIGNER);}
   XYResult* return_result = malloc(sizeof(XYResult));
   if(return_result){
     return_result->error = OK;
     return_result->result = self_OriginChainState->currentSigner;
     return return_result;
   } else {
     RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
   }
 }
