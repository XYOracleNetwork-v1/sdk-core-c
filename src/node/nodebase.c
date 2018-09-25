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

XYResult* initNode(NodeBase* self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount){
  self = malloc(sizeof(NodeBase));
  if(self == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  self->blockRepository = repository;
  self->hashingProvider = hashingProvider;

  self->originChainNavigator = malloc(sizeof(OriginChainNavigator));
  if(self->originChainNavigator == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  self->originChainState = malloc(sizeof(OriginChainState));
  if(self->originChainState == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  self->session = NULL;
  self->heuristicCount = heuristicCount;

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
  XYObject* heuristicsArray = (XYObject*)self->heuristics;
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
  XYObject* heuristicsArray = (XYObject*)self->heuristics;
  heuristicsArray = heuristicsArray + key;
  heuristicsArray = NULL;
  /* TODO: Since "Heuritics" array is NULL bounded this is
   * going to wipe the rest of the array out. TO FIX.
   */
   return TRUE;;
}

/*
 * Called When a bound witness is added successfully.
 */
XYResult* onBoundWitnessEndSuccess(NodeBase* self, BoundWitness* boundWitness){
  if(boundWitness != NULL){
    XYResult* hash_result = boundWitness->getHash(boundWitness, self->hashingProvider);
    if(hash_result->error != OK){
      RETURN_ERROR(ERR_CRITICAL);
    } else {
      XYResult* contains_result = self->originChainNavigator->containsOriginBlock(self->originChainNavigator, boundWitness);
      if(contains_result->error != OK){
        //TODO: De-onion and sort bridge blocks
        self->originChainNavigator->addBoundWitness(self->originChainNavigator, boundWitness);
        notifyListeners(self, boundWitness);
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result){
          return_result->error = OK;
          return_result->result = 0;
          return return_result;
        } else {
          RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
        }
      }
      else{
        RETURN_ERROR(ERR_CRITICAL);
      }
    }

  } else {
    RETURN_ERROR(ERR_CRITICAL);
  }
}

/*
 * Called When a bound witness has failed to be added.
 */
void onBoundWitnessEndFailure(enum EXyoErrors error){
  return;
}


/*
* Self signs an origin block to the devices origin chain.
*/
uint8_t selfSignOriginChain(NodeBase* self, uint flag){
  ZigZagBoundWitness* boundWitness = malloc(sizeof(ZigZagBoundWitness) + (2*sizeof(XYObject*)));
  if(boundWitness){
    boundWitness->boundWitness = malloc(sizeof(BoundWitnessTransfer));
    if(boundWitness == NULL) { return FALSE;; }
    XYResult* signer_result = self->originChainState->getSigners(self->originChainState);
    if(signer_result->error!=OK) { return FALSE; }
    boundWitness->signer = signer_result->result;
    boundWitness->incomingData = incomingData;
    boundWitness->addTransfer = addTransfer;
    boundWitness->addIncomingKeys = addIncomingKeys;
    boundWitness->addIncomingPayload = addIncomingPayload;
    boundWitness->addIncomingSignatures = addIncomingSignatures;
    boundWitness->makeSelfKeySet = makeSelfKeySet;
    boundWitness->signForSelf = signForSelf;
    boundWitness->heuristicCount = 2;
    XYObject* objects = (XYObject*)boundWitness->payload;

    XYResult* newObject_result = newObject((char*)&Sha256_id, ((ByteArray*)self->originChainState->latestHash->payload));
    if(newObject_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      free(newObject_result);
      return FALSE;
    }
    objects[0] = *(XYObject*)newObject_result->result;

    free(newObject_result);
    newObject_result = newObject((char*)&Index_id, &(self->originChainState->index));
    if(newObject_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      free(newObject_result);
      return FALSE;
    }
    objects[1] = *(XYObject*)newObject_result->result;

    XYResult* incomingData_result = boundWitness->incomingData(boundWitness, NULL, TRUE);
    if(incomingData_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    char* transferBytes = incomingData_result->result;
    free(incomingData_result);
    XYResult* lookup_result = lookup((char*)BoundWitness_id);
    if(lookup_result->error != OK){
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    free(lookup_result);
    ObjectProvider* boundWitnessCreator = lookup_result->result;

    XYResult* fromBytes_result = boundWitnessCreator->fromBytes(transferBytes);
    if(fromBytes_result->error != OK ){
      free(fromBytes_result);
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    BoundWitness* finalBoundWitness = fromBytes_result->result;
    free(fromBytes_result);
    if(updateOriginState(self, finalBoundWitness)){
      XYResult* success_return = onBoundWitnessEndSuccess(self, finalBoundWitness);
      if(success_return->error == OK){
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
XYObject* getUnSignedPayloads(NodeBase* self, uint bitFlag){
  int count = 0;
  for(int i = 0; i < BOUNDWITNESS_OPTIONS; i++){
    if(self->boundWitnessOptions[i] != NULL){
      count = count + 1;
    } else {
      break;
    }
  }
  XYObject* unsignedPayloads = malloc(sizeof(XYObject*)*count);
  XYObject* unsignedPayloadItr = unsignedPayloads;
  for(int i = 0; i< count; i++){
    BoundWitnessOption* option = self->boundWitnessOptions[i];
    unsignedPayloadItr = option->getUnsignedPayload();
    unsignedPayloadItr = unsignedPayloadItr+1;
  }
  return unsignedPayloads;
}
/*
* Gets all of the signed payloads for a given flag.
*/
XYObject* getSignedPayloads(NodeBase* self, uint bitFlag){
  int count = 0;
  for(int i = 0; i < BOUNDWITNESS_OPTIONS; i++){
    if(self->boundWitnessOptions[i] != NULL){
      count = count + 1;
    } else {
      break;
    }
  }
  XYObject* SignedPayloads = malloc(sizeof(XYObject*)*count);
  XYObject* SignedPayloadItr = SignedPayloads;
  for(int i = 0; i< count; i++){
    BoundWitnessOption* option = self->boundWitnessOptions[i];
    SignedPayloadItr = option->getSignedPayload();
    SignedPayloadItr = SignedPayloadItr+1;
  }
  return SignedPayloads;
}
/*
* Call the listener for each block in a bound witness.
*/
void notifyListeners(NodeBase* self, BoundWitness* boundWitness){
  for(int i = 0; i < MAX_ALLOCATED_LISTENERS; i++){
    if(self->listeners[i] != NULL){
      self->listeners[i]->onBoundWitnessDiscovered(boundWitness);
    } else {
      break;
    }
  }
  return;
}

/*
* Get blocks to bridge
XYResult* getBridgedBlocks(NodeBase* self){
  return preallocated_result;
}
*/

/*
* Create bound witness, handle outcome, and store if needed
*/
void doBoundWitness(NodeBase* self, ByteArray* startingData, NetworkPipe* pipe){
  if(self->session != NULL){
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result == NULL){ return; }
    self->onBoundWitnessStart();
    XYResult* role_result = pipe->peer->getRole(pipe);
    if(role_result->error != OK) {
      free(return_result);
      return;
    }
    uint8_t choice = getChoice((uint8_t*)role_result->result, startingData == NULL);
    self->session = malloc(sizeof(ZigZagBoundWitnessSession));
    if(self->session == NULL){ return; }
    self->session->NetworkPipe = pipe;
    XYResult* payload_result = makePayload(self, choice);
    if(payload_result->error != OK){ return; }
    self->session->boundWitness->dynamicPayloads = payload_result->result;
    XYResult* getSigner_result = self->originChainState->getSigners(self->originChainState);
    if(getSigner_result->error != OK) { return; }
    self->session->boundWitness->signer = getSigner_result->result;
    self->session->choice = choice;

    self->session->completeBoundWitness = completeBoundWitness;
    XYResult* completeBoundWitness_result = self->session->completeBoundWitness(self->session, startingData);
    pipe->close();
    if(completeBoundWitness_result->error != OK){
      onBoundWitnessEndFailure(completeBoundWitness_result->error);
      self->session = NULL;
      return;
    } else {
      updateOriginState(self, completeBoundWitness_result->result);
      onBoundWitnessEndSuccess(self, completeBoundWitness_result->result);
      self->session = NULL;
      return;

    }
  } else {
    return;
  }
}
/*
* Update the state of the origin chain.
*/
uint8_t updateOriginState(NodeBase* self, BoundWitness* boundWitness){
  XYResult* hash_result = boundWitness->getHash(boundWitness, self->hashingProvider);
  if(hash_result->error != OK){ return FALSE; }
  ByteArray* hash = hash_result->result;
  self->originChainState->newOriginBlock(self->originChainState, hash);
  free(hash_result);
  return TRUE;
}

/*
* Make a Payload* which can be used in a bound witness.
*/
XYResult* makePayload(NodeBase* self, uint bitFlag){
  // Set up Payload Array
  XYResult* lookup_result = lookup((char*)&IntStrongArray_id);
  if(lookup_result->error!=OK){ RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);}
  ObjectProvider* ISA_Creator = lookup_result->result;
  XYResult* create_result = ISA_Creator->create((char*)&Payload_id, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* payload_object = create_result->result;
  IntStrongArray* payloads_raw = payload_object->payload;
  //free(create_result);
  lookup_result = lookup((char*)&IntWeakArray_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }

  ObjectProvider* IWA_Creator = lookup_result->result;
  create_result = IWA_Creator->create(NULL, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* user_signedHeuristics_object = create_result->result;
  IntWeakArray* user_signedHeuristics = user_signedHeuristics_object->payload;
  //free(create_result);

  create_result = IWA_Creator->create(NULL, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* user_unsignedHeuristics_object = create_result->result;
  IntWeakArray* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
  //free(create_result);

  Payload* user_payload = malloc(sizeof(Payload));
  if(user_payload){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  /* Here we collect the correct heuristics given the bitFlag */
  XYObject* signedPayloads = getSignedPayloads(self, bitFlag);
  XYObject* unsignedPayloads = getUnSignedPayloads(self, bitFlag);

  /* get the number of signed and unsigned heuristics */
  uint8_t bool1 = 0;
  uint8_t bool2 = 0;
  for(int i = 0; i<MAX_ALLOCATED_HEURISTICS; i++){
    lookup_result = lookup((char*)signedPayloads->id);
    if(lookup_result->error != OK && bool1 == 0){
      bool1 = i;
    }
    lookup_result = lookup((char*)unsignedPayloads->id);
    if(lookup_result->error != OK && bool2 == 0){
      bool2 = i;
    }
    if(bool1 && bool2){
      break;
    }
  }

  /* Add each heuristic to its corresponding array */
  for(uint8_t i = 0; i < bool1; i++){
    user_signedHeuristics->add(user_signedHeuristics, signedPayloads[i].GetPayload(&signedPayloads[i]));
  }
  for(uint8_t i = 0; i < bool1; i++){
    user_unsignedHeuristics->add(user_signedHeuristics, unsignedPayloads[i].GetPayload(&unsignedPayloads[i]));
  }


  user_payload->size = user_signedHeuristics->size + user_unsignedHeuristics->size + 4;
  user_payload->signedHeuristics = user_signedHeuristics;
  user_payload->unsignedHeuristics = user_unsignedHeuristics;

  XYResult* newObject_result = newObject((char*)&Payload_id, user_payload);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  XYResult* add_result = payloads_raw->add(payloads_raw, newObject_result->result);
  if(add_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = payloads_raw;
    return return_result;
  } else {
    free(user_payload);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
