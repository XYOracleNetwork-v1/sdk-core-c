#include "xyo.h"
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
