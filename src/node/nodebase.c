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
#include "nodebase.h"

/**
 ****************************************************************************************
 *  NAME
 *      initNode
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

XYResult* initNode(NodeBase* self, RepositoryProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount){
  self->blockRepository = repository;
  self->hashingProvider = hashingProvider;

  self->originChainNavigator = malloc(sizeof(OriginChainNavigator));
  if(self->originChainNavigator == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  self->originChainState = malloc(sizeof(OriginChainState));
  if(self->originChainState == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  self->session = NULL;
  self->heuristicCount = heuristicCount;

  self->boundWitnessOptions = malloc(sizeof(void*) * MAX_ALLOCATED_OPTIONS); //TODO SIZEOF boundwitnessoption
  if(self->boundWitnessOptions == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  self->listeners = malloc(sizeof(void*) * MAX_ALLOCATED_LISTENERS);
  if(self->listeners == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = NULL;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/**
 ****************************************************************************************
 *  NAME
 *      addHeuristic
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

uint8_t addHeuristic(NodeBase* self, uint key, XYObject* heuristic){
  if(key > self->heuristicCount) { return FALSE;; }
  XYObject* heuristicsArray = self->heuristics;
  heuristicsArray = heuristicsArray + key;
  XYResult* lookup_result = lookup((char*)heuristicsArray);
  if(lookup_result->error != OK){
    free(lookup_result);
    heuristicsArray = heuristic;
    return TRUE;;
  } else {
    if(self->removeHeuristic(self, key)){
      heuristicsArray = heuristic;
      return TRUE;;
    } else {
      return FALSE;;
    }
  }
}
/*
* Removes a heuristic from the current heuristic pool.
*/
uint8_t removeHeuristic(NodeBase* self, uint key){
  if(key > self->heuristicCount) { return FALSE;; }
  XYObject* heuristicsArray = self->heuristics;
  heuristicsArray = heuristicsArray + key;
  heuristicsArray = NULL;
  /* TODO: Since "Heuritics" array is NULL bounded this is
   * going to wipe the rest of the array out. TO FIX.
   */
   return TRUE;;
}
/*
* Self signs an origin block to the devices origin chain.
*/
uint8_t selfSignOriginChain(NodeBase* self, uint flag){
  ZigZagBoundWitness* boundWitness = malloc(sizeof(ZigZagBoundWitness) + (self->heuristicCount*sizeof(XYObject*)));
  if(boundWitness){
    boundWitness->boundWitness = malloc(sizeof(BoundWitnessTransfer));
    if(boundWitness == NULL) { return FALSE;; }
    XYResult* signer_result = self->originChainState->getSigners(self->originChainState);
    if(signer_result->error!=OK) { return FALSE;; }
    boundWitness->signer = signer_result->result;
    boundWitness->incomingData = incomingData;
    boundWitness->addTransfer = addTransfer;
    boundWitness->addIncomingKeys = addIncomingKeys;
    boundWitness->addIncomingPayload = addIncomingPayload;
    boundWitness->addIncomingSignatures = addIncomingSignatures;
    boundWitness->makeSelfKeySet = makeSelfKeySet;
    boundWitness->signForSelf = signForSelf;
    XYResult* incomingData_result = boundWitness->incomingData(boundWitness, NULL, TRUE);
    if(incomingData_result->error != OK) {
      free(boundWitness);
      free(boundWitness->boundWitness);
      return FALSE;
    }
    char* transferBytes = incomingData_result->result;
    free(incomingData_result);
    XYResult* lookup_result = lookup((char*)BoundWitness_id);
    if(lookup_result->error != OK){
      free(boundWitness);
      free(boundWitness->boundWitness);
      return FALSE;
    }
    free(lookup_result);
    ObjectProvider* boundWitnessCreator = lookup_result->result;

    XYResult* fromBytes_result = boundWitnessCreator->fromBytes(transferBytes);
    if(fromBytes_result->error != OK ){
      free(fromBytes_result);
      free(boundWitness);
      free(boundWitness->boundWitness);
      return FALSE;
    }
    BoundWitness* finalBoundWitness = fromBytes_result->result;
    free(fromBytes_result);
    if(updateOriginState(self, finalBoundWitness)){
      XYResult* success_return = onBoundWitnessEndSuccess(self, finalBoundWitness);
      if(success_return->error != OK){
        free(success_return);
        return TRUE;
      } else {
        free(success_return);
        return FALSE;
      }
    } else {
      return FALSE;
    }
  } else {
    return FALSE;;
  }
}
/*
* Gets all of the unsigned payloads for a given flag.
*/
uint8_t getUnSignedPayloads(NodeBase* self, uint bitFlag){}
/*
* Gets all of the signed payloads for a given flag.
*/
uint8_t getSignedPayloads(NodeBase* self, uint bitFlag){}
/*
* Call the listener for each block in a bound witness.
*/
uint8_t notifyListeners(NodeBase* self, ZigZagBoundWitness* boundWitness){}
/*
* Get blocks to bridge
*/
XYResult* getBridgedBlocks(NodeBase* self){}
/*
* Create bound witness, handle outcome, and store if needed
*/
XYResult* doBoundWitness(ByteArray* startingData, NetworkPipe* pipe){}
/*
* Update the state of the origin chain.
*/
XYResult* updateOriginState(NodeBase* self, BoundWitness* boundWitness){}
/*
* Make a Payload* which can be used in a bound witness.
*/
XYResult* makePayload(NodeBase* self, uint bitFlag){}
