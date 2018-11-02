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
XYResult_t* newOriginBlock(OriginChainState_t* self_OriginChainState, 
                           ByteArray_t* originBlockHash) {
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self_OriginChainState || !originBlockHash) {RETURN_ERROR(ERR_BADDATA);}

  self_OriginChainState->latestHash = originBlockHash;
  self_OriginChainState->index = self_OriginChainState->index+1;
  if(self_OriginChainState->nextSigner != NULL){
    self_OriginChainState->currentSigner = self_OriginChainState->nextSigner;
    self_OriginChainState->nextSigner = NULL;
  }
  
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
XYResult_t* addSigner(OriginChainState_t* self_OriginChainState, Signer_t* user_Signer) {
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self_OriginChainState || !user_Signer) {RETURN_ERROR(ERR_BADDATA);}

  if(self_OriginChainState->currentSigner != NULL){
    self_OriginChainState->nextSigner = user_Signer;
  } else {
    self_OriginChainState->nextSigner = user_Signer;
  }
  
  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = 0;
  
  return preallocated_return_result_ptr;
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
 XYResult_t* getSigners(OriginChainState_t* self_OriginChainState){
   if(self_OriginChainState->currentSigner == NULL) { RETURN_ERROR(ERR_NOSIGNER);}
   XYResult_t* return_result = malloc(sizeof(XYResult_t));
   if(return_result){
     return_result->error = OK;
     return_result->result = self_OriginChainState->currentSigner;
     return return_result;
   } else {
     RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
   }
 }
// end of file state.c
