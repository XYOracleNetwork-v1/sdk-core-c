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
 *      initRelayNode
 *
 *  DESCRIPTION
 *    Initializes the Node Base class with a repository and hashing provider, and sets up
 *    the state and bound witness session.
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

XYResult* initRelayNode(RelayNode* self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount){
  NodeBase* baseNode;
  initNode(&baseNode, repository, hashingProvider, heuristicCount);
  self->node = baseNode;
  self->node->getChoice = Relay_getChoice;
  //self->getChoice = Relay_getChoice;
  self->doConnection = doConnection;
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
 XYResult* doConnection(RelayNode* self){
  NetworkPipe* connectionToOtherPartyFrom = self->findSomeoneToTalkTo();
  if(connectionToOtherPartyFrom->initializationData == NULL){
    XYResult* whatTheOtherPartyWantsToDo_result = connectionToOtherPartyFrom->peer->getRole(connectionToOtherPartyFrom);
    if(whatTheOtherPartyWantsToDo_result->error != OK){
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE); //TODO: close the pipe?!?
    }
    ByteArray* whatTheOtherPartyWantsToDo = whatTheOtherPartyWantsToDo_result->result;
    if(self->procedureCatalogue->canDo(whatTheOtherPartyWantsToDo)){
      return self->node->doBoundWitness(self->node, NULL, connectionToOtherPartyFrom);;
    } else {
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE);
    }
  } else {
    return self->node->doBoundWitness(self->node, connectionToOtherPartyFrom->initializationData, connectionToOtherPartyFrom);;
  }
}
