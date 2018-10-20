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
OriginChainState* gState = NULL;

XYResult* initNode(NodeBase** self, OriginChainProvider* repository, HashProvider* hashingProvider, uint8_t heuristicCount){
  *self = malloc(sizeof(NodeBase));
  if(self == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  (*self)->blockRepository = repository;
  (*self)->hashingProvider = hashingProvider;

  (*self)->originChainNavigator = malloc(sizeof(OriginChainNavigator));
  (*self)->originChainNavigator->containsOriginBlock = containsOriginBlock;
  (*self)->originChainNavigator->Hash = hashingProvider;
  (*self)->originChainNavigator->addBoundWitness = addBoundWitness;
  XYResult* result = IntStrongArray_creator_create((char*)&BoundWitness_id, NULL);
  XYObject* repoArray_object = result->result;
  IntStrongArray* repoArray = repoArray_object->payload;
  (*self)->originChainNavigator->originChainRepository = malloc(sizeof(OriginChainProvider));
  (*self)->originChainNavigator->originChainRepository->repository = repoArray;
  if((*self)->originChainNavigator->originChainRepository == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  if((*self)->originChainNavigator->originChainRepository->repository == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  (*self)->originChainNavigator->originChainRepository->append = append;
  (*self)->originChainNavigator->originChainRepository->getChain = getChain;
  (*self)->originChainNavigator->originChainRepository->deleteChain = deleteChain;


  if((*self)->originChainNavigator == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  (*self)->originChainState = malloc(sizeof(OriginChainState));
  gState = (*self)->originChainState;
  (*self)->originChainState->addSigner = addSigner;
  (*self)->originChainState->newOriginBlock = newOriginBlock;
  (*self)->originChainState->getSigners = getSigners;
  if((*self)->originChainState == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  (*self)->session = NULL;
  (*self)->heuristicCount = heuristicCount;

  (*self)->addHeuristic = addHeuristic;
  (*self)->removeHeuristic = removeHeuristic;
  (*self)->selfSignOriginChain = selfSignOriginChain;
  (*self)->getUnSignedPayloads = getUnSignedPayloads;
  (*self)->getSignedPayloads = getSignedPayloads;
  (*self)->notifyListeners = notifyListeners;
  (*self)->getBridgedBlocks = getBridgedBlocks;
  (*self)->doBoundWitness = doBoundWitness;
  (*self)->updateOriginState = updateOriginState;
  (*self)->makePayload = makePayload;
  (*self)->onBoundWitnessStart = onBoundWitnessStart;
  (*self)->onBoundWitnessEndSuccess = onBoundWitnessEndSuccess;
  (*self)->onBoundWitnessEndFailure = onBoundWitnessEndFailure;

  BoundWitnessOption* option1 = malloc(sizeof(BoundWitnessOption));
  option1->flag = BOUND_WITNESS_OPTION; //TODO: hard wired
  option1->getSignedPayload = getSignedIndex;
  option1->getUnsignedPayload = getUnSignedIndex;

  BoundWitnessOption* option2 = malloc(sizeof(BoundWitnessOption));
  option2->flag = BOUND_WITNESS_OPTION; //TODO: hard wired
  option2->getSignedPayload = getSignedHash;
  option2->getUnsignedPayload = getUnSignedHash;

  (*self)->boundWitnessOptions[0] = option1;
  (*self)->boundWitnessOptions[1] = option2;


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
void onBoundWitnessEndSuccess(NodeBase* self, BoundWitness* boundWitness){
  if(boundWitness != NULL){
    XYResult* hash_result = boundWitness->getHash(boundWitness, self->hashingProvider);
    if(hash_result->error != OK){
      return;
    } else {
      XYResult* contains_result = self->originChainNavigator->containsOriginBlock(self->originChainNavigator, boundWitness);
      if(contains_result->error != OK){
        //TODO: De-onion and sort bridge blocks
        self->originChainNavigator->addBoundWitness(self->originChainNavigator, boundWitness);
        notifyListeners(self, boundWitness);
        return;
      }
      else{
        return;
      }
    }

  } else {
    return;
  }
}

/*
 * Called When a bound witness has failed to be added.
 */
void onBoundWitnessEndFailure(enum EXyoErrors error){
  return;
}

/*
 * Called when a bound witness is about to start.
 */
void onBoundWitnessStart(){
  return;
}

XYObject* getSignedHash(){
  PreviousHash* return_PH = malloc(sizeof(PreviousHash));
  if(return_PH){
    return_PH->hash = malloc(sizeof(char)*34);
    strncpy(return_PH->id, (char*)&Sha256_id, 2);
    memset(return_PH->hash, 2, 32);
    XYResult* newObject_result = newObject((char*)&PreviousHash_id, return_PH);
    if(newObject_result->error != OK){
      if(return_PH) free(return_PH);
      free(newObject_result);
      return NULL;
    }
    return newObject_result->result;
  } else {
    return NULL;
  }
}

XYObject* getUnSignedHash(){
  PreviousHash* return_PH = malloc(sizeof(PreviousHash));
  if(return_PH){
    return_PH->hash = malloc(sizeof(char)*34);
    strncpy(return_PH->id, (char*)&Sha256_id, 2);
    memset(return_PH->hash, 9, 32);
    XYResult* newObject_result = newObject((char*)&PreviousHash_id, return_PH);
    if(newObject_result->error != OK){
      if(return_PH) free(return_PH);
      free(newObject_result);
      return NULL;
    }
    return newObject_result->result;
  } else {
    return NULL;
  }
}

XYObject* getSignedIndex(){
  XYResult* newObject_result = newObject((char*)&Index_id, &gState->index);
  XYObject* return_object = newObject_result->result;
  free(newObject_result);
  return return_object;
}

XYObject* getUnSignedIndex(){
  XYResult* newObject_result = newObject((char*)&Index_id, &gState->index);
  XYObject* return_object = newObject_result->result;
  free(newObject_result);
  return return_object;
}

/*
* Self signs an origin block to the devices origin chain.
*/
uint8_t selfSignOriginChain(NodeBase* self, uint flag){
  ZigZagBoundWitness* boundWitness = malloc(sizeof(ZigZagBoundWitness) + (2*sizeof(XYObject)));
  if(boundWitness){
    boundWitness->boundWitness = malloc(sizeof(BoundWitnessTransfer));
    if(boundWitness->boundWitness == NULL) { return FALSE; }
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

    XYResult* lookup_result = lookup((char*)ShortStrongArray_id);
    ObjectProvider* SSA_Creator = lookup_result->result;
    free(lookup_result);
    lookup_result = lookup((char*)IntStrongArray_id);
    ObjectProvider* ISA_Creator = lookup_result->result;
    free(lookup_result);

    XYResult* SSA_result = SSA_Creator->create((char*)&ECDSASecp256k1_id, NULL);
    XYObject* pubkeyArray_object = SSA_result->result;
    ShortStrongArray* pubkeyArray = pubkeyArray_object->payload;
    free(SSA_result);
    SSA_result = SSA_Creator->create((char*)SignatureSet_id, NULL);
    XYObject* signatureArray_object = SSA_result->result;
    ShortStrongArray* signatureArray = signatureArray_object->payload;
    free(SSA_result);
    XYResult* ISA_result = ISA_Creator->create((char*)Payload_id, NULL);
    XYObject* payloadsArray_object = ISA_result->result;
    IntStrongArray* payloadsArray = payloadsArray_object->payload;
    boundWitness->dynamicPublicKeys = pubkeyArray;
    boundWitness->dynamicPayloads = payloadsArray;
    boundWitness->dynamicSignatures = signatureArray;

    XYObject* objects = (XYObject*)&boundWitness->payload;

    XYResult* newObject_result = newObject((char*)Sha256_id, ((ByteArray*)self->originChainState->latestHash->payload));
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
    lookup_result = lookup((char*)BoundWitness_id);
    if(lookup_result->error != OK){
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
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
      self->onBoundWitnessEndSuccess(self, finalBoundWitness);
      return TRUE;
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
  XYObject* unsignedPayloads = malloc(sizeof(XYObject)*count);
  XYObject* unsignedPayloadItr = unsignedPayloads;
  for(int i = 0; i< count; i++){
    BoundWitnessOption* option = self->boundWitnessOptions[i];
    XYObject* result = option->getUnsignedPayload();
    if(result){
      *unsignedPayloadItr = *result;
      unsignedPayloadItr = unsignedPayloadItr+1;
    } else {
      continue;
    }
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
  XYObject* SignedPayloads = malloc(sizeof(XYObject)*count);
  XYObject* SignedPayloadItr = SignedPayloads;
  for(int i = 0; i< count; i++){
    BoundWitnessOption* option = self->boundWitnessOptions[i];
    XYObject* result = option->getSignedPayload();
    if(result){
      *SignedPayloadItr = *result;
      SignedPayloadItr = SignedPayloadItr+1;
    } else {
      continue;
    }
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
 */
XYResult* getBridgedBlocks(NodeBase* self){
  return NULL;
}

/*
* Create bound witness, handle outcome, and store if needed
*/
XYResult* doBoundWitness(NodeBase* self, ByteArray* startingData, NetworkPipe* pipe){
  breakpoint();
  if(self->session == NULL){
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->onBoundWitnessStart();
    XYResult* role_result = pipe->peer->getRole(pipe);
    if(role_result->error != OK) {
      free(return_result);
      RETURN_ERROR(ERR_CRITICAL);
    }
    uint8_t choice = self->getChoice((uint8_t*)role_result->result);
    self->session = malloc(sizeof(ZigZagBoundWitnessSession));
    if(self->session == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->session->boundWitness = malloc(sizeof(ZigZagBoundWitness));
    if(self->session->boundWitness == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->session->NetworkPipe = pipe;
    XYResult* payload_result = self->makePayload(self, self->flag);
    if(payload_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }
    self->session->boundWitness->dynamicPayloads = payload_result->result;
    XYResult* getSigner_result = self->originChainState->getSigners(self->originChainState);
    if(getSigner_result->error != OK) { RETURN_ERROR(ERR_NOSIGNER); }
    self->session->boundWitness->signer = getSigner_result->result;
    self->session->choice = choice;

    self->session->completeBoundWitness = completeBoundWitness;
    self->session->boundWitness->incomingData = incomingData;
    self->session->boundWitness->addTransfer = addTransfer;
    self->session->boundWitness->addIncomingKeys = addIncomingKeys;
    self->session->boundWitness->addIncomingPayload = addIncomingPayload;
    self->session->boundWitness->addIncomingSignatures = addIncomingSignatures;
    self->session->boundWitness->makeSelfKeySet = makeSelfKeySet;
    self->session->boundWitness->signForSelf = signForSelf;

    XYResult* lookup_result = lookup((char*)ShortStrongArray_id);
    ObjectProvider* SSA_Creator = lookup_result->result;
    free(lookup_result);

    XYResult* SSA_result = SSA_Creator->create((char*)&KeySet_id, NULL);
    XYObject* pubkeyArray_object = SSA_result->result;
    ShortStrongArray* pubkeyArray = pubkeyArray_object->payload;
    free(SSA_result);
    SSA_result = SSA_Creator->create((char*)SignatureSet_id, NULL);
    XYObject* signatureArray_object = SSA_result->result;
    ShortStrongArray* signatureArray = signatureArray_object->payload;
    free(SSA_result);

    self->session->boundWitness->dynamicPublicKeys = pubkeyArray;
    self->session->boundWitness->dynamicSignatures = signatureArray;

    XYResult* completeBoundWitness_result = self->session->completeBoundWitness(self->session, startingData);
    pipe->close(self->session);
    if(completeBoundWitness_result->error != OK){
      self->onBoundWitnessEndFailure(completeBoundWitness_result->error);
      self->session = NULL;
      RETURN_ERROR(ERR_BOUNDWITNESS_FAILED);
    } else {
      self->updateOriginState(self, (BoundWitness*)self->session->boundWitness->boundWitness);
      self->onBoundWitnessEndSuccess(self, (BoundWitness*)self->session->boundWitness->boundWitness);
      self->session = NULL;
      RETURN_ERROR(OK);

    }
  } else {
    RETURN_ERROR(ERR_CRITICAL);
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
  free(lookup_result);
  XYResult* create_result = ISA_Creator->create((char*)&IntStrongArray_id, NULL); // payload_id?
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* payloads_object = create_result->result;
  IntStrongArray* payloads_raw = payloads_object->payload;

  lookup_result = lookup((char*)&Payload_id);
  if(lookup_result->error!=OK){ RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);}
  ObjectProvider* Payload_Creator = lookup_result->result;
  free(lookup_result);
  create_result = Payload_Creator->create((char*)&Payload_id, NULL); // payload_id?
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* payload_object = create_result->result;
  Payload* payload_raw = payload_object->payload;
  /*
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
  free(create_result);

  create_result = IWA_Creator->create(NULL, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject* user_unsignedHeuristics_object = create_result->result;
  IntWeakArray* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
  free(create_result);

  Payload* user_payload = malloc(sizeof(Payload));
  if(user_payload == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  /* Here we collect the correct heuristics given the bitFlag */
  XYObject* signedPayloads = getSignedPayloads(self, bitFlag);
  XYObject* unsignedPayloads = getUnSignedPayloads(self, bitFlag);

  /* get the number of signed and unsigned heuristics */
  uint8_t bool1 = 2; //TODO HARD CODED
  uint8_t bool2 = 2;
  /*
  for(int i = 0; i<MAX_ALLOCATED_HEURISTICS; i++){
    lookup_result = lookup((char*)&signedPayloads[i].id);
    if(lookup_result->error != OK && bool1 == 255){
      bool1 = i;
    }
    lookup_result = lookup((char*)&unsignedPayloads[i].id);
    if(lookup_result->error != OK && bool2 == 255){
      bool2 = i;
    }
    if( (bool1 == i && bool2 >=0) || (bool2 == i && bool1 >=0) ){
      break;
    }
  }
  */
  /* Add each heuristic to its corresponding array */
  for(uint8_t i = 0; i < bool1; i++){
    payload_raw->signedHeuristics->add(payload_raw->signedHeuristics, &signedPayloads[i]);

  }
  for(uint8_t i = 0; i < bool2; i++){
    payload_raw->unsignedHeuristics->add(payload_raw->unsignedHeuristics, &unsignedPayloads[i]);
  }


  payload_raw->size = payload_raw->signedHeuristics->size + payload_raw->unsignedHeuristics->size + 4;
  XYResult* newObject_result = newObject((char*)&Payload_id, payload_raw);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  XYResult* add_result = payloads_raw->add(payloads_raw, newObject_result->result);
  if(add_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = payloads_raw;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
