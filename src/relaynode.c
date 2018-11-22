/**
 ****************************************************************************************
 *
 * @file NodeBase.c
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
#include "relaynode.h"

/**
 ****************************************************************************************
 *  NAME
 *      Relay_getChoice
 *
 *  DESCRIPTION
 *  Add a XYObject to the struct-hack
 *
 *  PARAMETERS
 *      object      [in]      CryptoCreator*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 uint8_t Relay_getChoice(uint8_t* theirCatalog){
   if(*theirCatalog - BOUND_WITNESS_OPTION != 0){
     return GIVE_ORIGIN_CHAIN_OPTION + BOUND_WITNESS_OPTION;
   } else {
     if(*theirCatalog & BOUND_WITNESS_OPTION){
       return BOUND_WITNESS_OPTION;
     } else {
       return 0;
     }
   }
   return 0;
   /*
     if((*theirCatalog - BOUND_WITNESS_OPTION) & TAKE_ORIGIN_CHAIN_OPTION){
       return GIVE_ORIGIN_CHAIN_OPTION + BOUND_WITNESS_OPTION;
     }
   } else {
     if(*theirCatalog & BOUND_WITNESS_OPTION){
       return BOUND_WITNESS_OPTION;
     } else {
       return 0;
     }
   }
   return 0;
   */
}

/**
 ****************************************************************************************
 *  NAME
 *      doConnection
 *
 *  DESCRIPTION
 *  Add a XYObject to the struct-hack
 *
 *  PARAMETERS
 *      object      [in]      CryptoCreator*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 XYResult_t doConnection(RelayNode_t* self){
  NetworkPipe_t connectionToOtherPartyFrom;
  XYResult_t findPeer_result = findPeer(&self->networkProvider, &connectionToOtherPartyFrom, self->node.choice);
  
  //if(connectionToOtherPartyFrom.scratchBuffer == NULL){
    /*
    XYResult_t whatTheOtherPartyWantsToDo_result = connectionToOtherPartyFrom->peer->getRole(connectionToOtherPartyFrom);
    if(whatTheOtherPartyWantsToDo_result->error != OK){
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE); //TODO: close the pipe?!?
    }
    ByteArray_t* whatTheOtherPartyWantsToDo = whatTheOtherPartyWantsToDo_result->result;
    if(canDo(whatTheOtherPartyWantsToDo)){
      return doBoundWitness(self->node, NULL, connectionToOtherPartyFrom);;
    } else {
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE);
    }
    */
  //} else {
  //return doBoundWitness(self->node, connectionToOtherPartyFrom.scratchBuffer, connectionToOtherPartyFrom);;
  //}
  XYResult_t result = completeBoundWitness(self->node, self->node.NetworkPipe.scratchBuffer);
}
