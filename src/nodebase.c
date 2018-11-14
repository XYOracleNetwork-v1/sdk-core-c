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
OriginChainState_t* gState = NULL;

XYResult_t* initNode(NodeBase_t** self, OriginChainProvider_t* repository, HashProvider_t* hashingProvider, uint8_t heuristicCount){
  *self = malloc(sizeof(NodeBase_t));
  if(self == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  (*self)->blockRepository = repository;
  (*self)->hashingProvider = hashingProvider;

  (*self)->originChainNavigator = malloc(sizeof(OriginChainNavigator_t));
  (*self)->originChainNavigator->containsOriginBlock = containsOriginBlock;
  (*self)->originChainNavigator->Hash = hashingProvider;
  (*self)->originChainNavigator->addBoundWitness = addBoundWitness;
  (*self)->originChainNavigator->queueLen = 0;
  //(*self)->originChainNavigator->bridgeQueue = //malloc(sizeof(ByteArray_t*)*MIN_QUEUE);
  //(*self)->originChainNavigator->bridgeQueue[0] = NULL;
  //(*self)->originChainNavigator->bridgeQueue[1] = NULL;

  XYResult_t* result = IntStrongArray_creator_create((const char*)&BoundWitness_id, NULL);
  XYObject_t* repoArray_object = result->result;
  IntStrongArray_t* repoArray = repoArray_object->payload;
  (*self)->originChainNavigator->originChainRepository = malloc(sizeof(OriginChainProvider_t));
  (*self)->originChainNavigator->originChainRepository->repository = repoArray;
  if((*self)->originChainNavigator->originChainRepository == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  if((*self)->originChainNavigator->originChainRepository->repository == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  (*self)->originChainNavigator->originChainRepository->append = append;
  (*self)->originChainNavigator->originChainRepository->getChain = getChain;
  (*self)->originChainNavigator->originChainRepository->deleteChain = deleteChain;


  if((*self)->originChainNavigator == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  (*self)->originChainState = malloc(sizeof(OriginChainState_t));
  gState = (*self)->originChainState;
  (*self)->originChainState->addSigner = addSigner;
  (*self)->originChainState->newOriginBlock = newOriginBlock;
  (*self)->originChainState->getSigners = getSigners;
  (*self)->originChainState->index = 0;
  if((*self)->originChainState == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY) }

  (*self)->session = NULL;
  (*self)->heuristicCount = heuristicCount;

  (*self)->addHeuristic = addHeuristic;
  (*self)->removeHeuristic = removeHeuristic;
  (*self)->selfSignOriginChain = selfSignOriginChain;
  (*self)->getUnSignedPayloads = getUnSignedPayloads;
  (*self)->getSignedPayloads = getSignedPayloads;
  (*self)->notifyListeners = notifyListeners;
  //(*self)->getBridgedBlocks = getBridgedBlocks;
  (*self)->doBoundWitness = doBoundWitness;
  (*self)->updateOriginState = updateOriginState;
  (*self)->makePayload = makePayload;
  (*self)->onBoundWitnessStart = onBoundWitnessStart;
  (*self)->onBoundWitnessEndSuccess = onBoundWitnessEndSuccess;
  (*self)->onBoundWitnessEndFailure = onBoundWitnessEndFailure;
  (*self)->flag = NODE_MODE;


  BoundWitnessOption_t* option1 = malloc(sizeof(BoundWitnessOption_t));
  option1->flag = BOUND_WITNESS_OPTION; //TODO: hard wired
  option1->getSignedPayload = getSignedIndex;
  option1->getUnsignedPayload = getUnSignedIndex;

  BoundWitnessOption_t* option2 = malloc(sizeof(BoundWitnessOption_t));
  option2->flag = BOUND_WITNESS_OPTION; //TODO: hard wired
  option2->getSignedPayload = getSignedHash;
  option2->getUnsignedPayload = getUnSignedHash;

  BoundWitnessOption_t* option3 = malloc(sizeof(BoundWitnessOption_t));
  option3->flag = GIVE_ORIGIN_CHAIN_OPTION; //TODO: hard wired
  option3->getSignedPayload = getSignedBridge;
  option3->getUnsignedPayload = getUnSignedBridge;

  (*self)->boundWitnessOptions[0] = option1;
  (*self)->boundWitnessOptions[1] = option2;
  (*self)->boundWitnessOptions[2] = option3;


  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  if(return_result){
    return_result->error = OK;
    return_result->result = NULL;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

XYObject_t* getSignedHash(void* self){
  PreviousHash_t* return_PH = malloc(sizeof(PreviousHash_t));
  if(return_PH){
    return_PH->hash = malloc(sizeof(char)*34);
    strncpy(return_PH->id, (const char*)&Sha256_id, 2);
    NodeBase_t* nodebase = (NodeBase_t*)self;
    if(nodebase->originChainNavigator->queueLen < 1){
        memset(return_PH->hash, 0x00, 32);
        XYResult_t* newObject_result = newObject((const char*)&PreviousHash_id, return_PH);
        if(newObject_result->error != OK){
          if(return_PH) free(return_PH);
          free(newObject_result);
          return NULL;
        }
        //printf("Linking to: NULL\n");
        return newObject_result->result;
    }

    ByteArray_t* lastHash = nodebase->originChainNavigator->bridgeQueue[nodebase->originChainNavigator->queueLen-1];

    memcpy(return_PH->hash, lastHash->payload, 32);
    XYResult_t* newObject_result = newObject((const char*)&PreviousHash_id, return_PH);
    if(newObject_result->error != OK){
      if(return_PH) free(return_PH);
      free(newObject_result);
      return NULL;
    }
    /*
    printf("Bridge Queue: [");
    for(int j = 0; j< nodebase->originChainNavigator->queueLen-2; j++){
      ByteArray_t* lHash = nodebase->originChainNavigator->bridgeQueue[j];
      for(int i = 0; i<32; i++){
        printf("%1x", (unsigned)(unsigned char)lHash->payload[i]);
      }
      printf(", ");
    }
    printf("]\n");
    */
    /*
    printf("Linking to block: ");
    for(int i = 0; i<32; i++){
      printf("%1x", (unsigned)(unsigned char)lastHash->payload[i]);
    }
    printf("\n");
    */

    return newObject_result->result;
  } else {
    return NULL;
  }
}

XYObject_t* getUnSignedHash(void* self){
  return NULL;
}

XYObject_t* getSignedIndex(void* self){
  XYResult_t* newObject_result = newObject((const char*)&Index_id, &((NodeBase_t*)self)->originChainState->index);
  XYObject_t* return_object = newObject_result->result;
  //free(newObject_result);
  return return_object;
}

XYObject_t* getUnSignedIndex(void* self){
  return NULL;
}

XYObject_t* getSignedBridge(void* self){
  XYResult_t* arrayResult = tableLookup((const char*)&ShortWeakArray_id);
  if(arrayResult->error != OK){
    return NULL;
  }
  ObjectProvider_t* arrayCreator = arrayResult->result;
  XYResult_t* create_result = arrayCreator->create((const char*)&BridgeHashSet_id, NULL);
  XYObject_t* arrayObject = create_result->result;
  ShortWeakArray_t* arrayRaw = arrayObject->payload;
  if(((NodeBase_t*)self)->originChainNavigator->queueLen<1){
    return NULL;
  }
  for(uint32_t i=0; i<=((NodeBase_t*)self)->originChainNavigator->queueLen-1; i++){
    ByteArray_t* bridgeHash = ((NodeBase_t*)self)->originChainNavigator->bridgeQueue[i];
    XYResult_t* freeme = newObject((const char*)&Sha256_id, bridgeHash->payload);
    XYObject_t* hashObj = freeme->result;
    XYResult_t* addResult = arrayRaw->add(arrayRaw, hashObj);
    if(addResult->error != OK){
      printf("Couldn't add hash to bridge hash set. \n");
      free(freeme);
      free(addResult);
      free(hashObj);
      return NULL;
    }
    free(freeme);
  }
  free(arrayResult);
  return arrayObject;

  return NULL;
}

XYObject_t* getUnSignedBridge(void* self){
  if(((NodeBase_t*)self)->originChainNavigator->queueLen<1){
    return NULL;
  }
  NodeBase_t* node = (NodeBase_t*)self;
  OriginChainProvider_t* provider = node->originChainNavigator->originChainRepository;
  XYResult_t* newObject_result = newObject((const char*)&BridgeBlockSet_id, provider->repository);
  if(newObject_result->error != OK) { free(newObject_result); return NULL; }
  return newObject_result->result;

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

uint8_t addHeuristic(NodeBase_t* self, uint32_t key, XYObject_t* heuristic){
  if(key > self->heuristicCount) { return FALSE; }
  XYObject_t* heuristicsArray = (XYObject_t*)self->heuristics;
  heuristicsArray = heuristicsArray + key;
  XYResult_t* lookup_result = tableLookup((const char*)heuristicsArray->id);
  if(lookup_result->error != OK){
    //free(lookup_result);
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
uint8_t removeHeuristic(NodeBase_t* self, uint32_t key){
  if(key > self->heuristicCount) { return FALSE;; }
  XYObject_t* heuristicsArray = (XYObject_t*)self->heuristics;
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
void onBoundWitnessEndSuccess(NodeBase_t* self, BoundWitness_t* boundWitness){
  if(boundWitness != NULL){
    XYResult_t* hash_result = boundWitness->getHash(boundWitness, self->hashingProvider);
    if(hash_result->error != OK){
      return;
    } else {
      #ifndef HEADLESS
      XYObject_t* hash_object = hash_result->result;
      ByteArray_t* hash = hash_object->payload;
      printf(GRN "New block found:  " RESET);
      for(uint32_t i = 0; i<hash->size; i++){
        printf("%1x", (unsigned)(unsigned char)hash->payload[i]);
      }
      printf("\n");

      #endif

      XYResult_t* contains_result = self->originChainNavigator->containsOriginBlock(self->originChainNavigator, boundWitness);
      if(contains_result->error != OK ){
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
void onBoundWitnessEndFailure( void ){
  printf(RED "Bound witness failed!\n" RESET);
}

/*
 * Called when a bound witness is about to start.
 */
void onBoundWitnessStart( void ){
  return;
}

/*
* Self signs an origin block to the devices origin chain.
*/
uint8_t selfSignOriginChain(NodeBase_t* self){
  ZigZagBoundWitness_t* boundWitness = malloc(sizeof(ZigZagBoundWitness_t) + (2*sizeof(XYObject_t)));
  if(boundWitness){
    boundWitness->boundWitness = malloc(sizeof(BoundWitnessTransfer_t));
    if(boundWitness->boundWitness == NULL) { return FALSE; }
    XYResult_t* signer_result = self->originChainState->getSigners(self->originChainState);
    if(signer_result->error!=OK) { return FALSE; }
    boundWitness->signer = signer_result->result;
    boundWitness->incomingData = incomingData;
    boundWitness->hasSentKeysAndPayload = 0;
    boundWitness->addTransfer = addTransfer;
    boundWitness->addIncomingKeys = addIncomingKeys;
    boundWitness->addIncomingPayload = addIncomingPayload;
    boundWitness->addIncomingSignatures = addIncomingSignatures;
    boundWitness->makeSelfKeySet = makeSelfKeySet;
    boundWitness->signForSelf = signForSelf;
    boundWitness->heuristicCount = 3;

    XYResult_t* lookup_result = tableLookup(ShortStrongArray_id);
    ObjectProvider_t* SSA_Creator = lookup_result->result;
    //free(lookup_result);
    lookup_result = tableLookup(IntStrongArray_id);
    ObjectProvider_t* ISA_Creator = lookup_result->result;
    //free(lookup_result);

    XYResult_t* SSA_result = SSA_Creator->create((const char*)&ECDSASecp256k1_id, NULL);
    XYObject_t* pubkeyArray_object = SSA_result->result;
    ShortStrongArray_t* pubkeyArray = pubkeyArray_object->payload;
    free(SSA_result);
    SSA_result = SSA_Creator->create(SignatureSet_id, NULL);
    XYObject_t* signatureArray_object = SSA_result->result;
    ShortStrongArray_t* signatureArray = signatureArray_object->payload;
    free(SSA_result);
    XYResult_t* ISA_result = ISA_Creator->create(Payload_id, NULL);
    XYObject_t* payloadsArray_object = ISA_result->result;
    IntStrongArray_t* payloadsArray = payloadsArray_object->payload;
    boundWitness->dynamicPublicKeys = pubkeyArray;
    boundWitness->dynamicPayloads = payloadsArray;
    boundWitness->dynamicSignatures = signatureArray;

    XYObject_t* objects = (XYObject_t*)&boundWitness->payload;

    XYResult_t* newObject_result = newObject(Sha256_id, (self->originChainState->latestHash->payload));
    if(newObject_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      free(newObject_result);
      return FALSE;
    }
    objects[0] = *(XYObject_t*)newObject_result->result;

    free(newObject_result);
    newObject_result = newObject((const char*)&Index_id, &(self->originChainState->index));
    if(newObject_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      free(newObject_result);
      return FALSE;
    }
    objects[1] = *(XYObject_t*)newObject_result->result;
    XYResult_t* incomingData_result = boundWitness->incomingData(boundWitness, NULL, TRUE);
    if(incomingData_result->error != OK) {
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    char* transferBytes = incomingData_result->result;
    free(incomingData_result);
    lookup_result = tableLookup(BoundWitness_id);
    if(lookup_result->error != OK){
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    ObjectProvider_t* boundWitnessCreator = lookup_result->result;
    XYResult_t* fromBytes_result = boundWitnessCreator->fromBytes(transferBytes);

    if(fromBytes_result->error != OK ){
      free(fromBytes_result);
      free(boundWitness->boundWitness);
      free(boundWitness);
      return FALSE;
    }
    BoundWitness_t* finalBoundWitness = fromBytes_result->result;
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
uint8_t gunsignedHeuristicCount = 0;
XYObject_t* getUnSignedPayloads(NodeBase_t* self){
  uint32_t count = 0;
  for(uint32_t i = 0; i < BOUNDWITNESS_OPTIONS; i++){
    if(self->boundWitnessOptions[i] != NULL){
      count = count + 1;
    } else {
      break;
    }
  }
  XYObject_t* unsignedPayloads = malloc(sizeof(XYObject_t)*count);
  XYObject_t* unsignedPayloadItr = unsignedPayloads;
  for(uint32_t i = 0; i< count; i++){
    BoundWitnessOption_t* option = self->boundWitnessOptions[i];
    if(option->flag & self->flag){
      XYObject_t* result = option->getUnsignedPayload(self);
      if(result){
        *unsignedPayloadItr = *result;
        unsignedPayloadItr = unsignedPayloadItr+1;
        gunsignedHeuristicCount += 1;
      } else {
        continue;
      }
    } else {
      continue;
    }
  }
  return unsignedPayloads;
}
/*
* Gets all of the signed payloads for a given flag.
*/
uint8_t gsignedHeuristicCount = 0;
XYObject_t* getSignedPayloads(NodeBase_t* self){
  uint32_t count = 0;
  for(uint32_t i = 0; i < BOUNDWITNESS_OPTIONS; i++){
    if(self->boundWitnessOptions[i] != NULL){
      count = count + 1;
    } else {
      break;
    }
  }
  XYObject_t* SignedPayloads = malloc(sizeof(XYObject_t)*count);
  XYObject_t* SignedPayloadItr = SignedPayloads;
  for(uint32_t i = 0; i< count; i++){
    BoundWitnessOption_t* option = self->boundWitnessOptions[i];
    if(option->flag & self->flag){
      XYObject_t* result = option->getSignedPayload((void*)self);
      if(result){
        *SignedPayloadItr = *result;
        SignedPayloadItr = SignedPayloadItr+1;
        gsignedHeuristicCount += 1;
      } else {
        continue;
      }
    } else {
      continue;
    }

  }
  return SignedPayloads;
}

/*
* Call the listener for each block in a bound witness.
*/
void notifyListeners(NodeBase_t* self, BoundWitness_t* boundWitness){
  for(uint32_t i = 0; i < MAX_ALLOCATED_LISTENERS; i++){
    if(self->listeners[i] != NULL){
      self->listeners[i]->onBoundWitnessDiscovered((void*)boundWitness);
    } else {
      break;
    }
  }
  return;
}

/*
* Create bound witness, handle outcome, and store if needed
*/
XYResult_t* doBoundWitness(NodeBase_t* self, ByteArray_t* startingData, NetworkPipe_t* pipe){
  if(self->session == NULL){
    XYResult_t* return_result = malloc(sizeof(XYResult_t));
    if(return_result == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->onBoundWitnessStart();
    XYResult_t* role_result = pipe->peer->getRole(pipe);
    if(role_result->error != OK) {
      free(return_result);
      RETURN_ERROR(ERR_CRITICAL);
    }
    uint8_t choice = self->getChoice((uint8_t*)role_result->result);
    self->flag = choice;
    self->session = malloc(sizeof(ZigZagBoundWitnessSession_t));
    if(self->session == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->session->boundWitness = malloc(sizeof(ZigZagBoundWitness_t));
    if(self->session->boundWitness == NULL){ RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
    self->session->NetworkPipe = pipe;
    XYResult_t* payload_result = self->makePayload(self);
    if(payload_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }
    self->session->boundWitness->dynamicPayloads = payload_result->result;
    XYResult_t* getSigner_result = self->originChainState->getSigners(self->originChainState);
    if(getSigner_result->error != OK) { RETURN_ERROR(ERR_NOSIGNER); }
    self->session->boundWitness->signer = getSigner_result->result;
    self->session->choice = choice;
    self->session->boundWitness->hasSentKeysAndPayload = 0;

    self->session->completeBoundWitness = completeBoundWitness;
    self->session->boundWitness->incomingData = incomingData;
    self->session->boundWitness->addTransfer = addTransfer;
    self->session->boundWitness->addIncomingKeys = addIncomingKeys;
    self->session->boundWitness->addIncomingPayload = addIncomingPayload;
    self->session->boundWitness->addIncomingSignatures = addIncomingSignatures;
    self->session->boundWitness->makeSelfKeySet = makeSelfKeySet;
    self->session->boundWitness->signForSelf = signForSelf;

    XYResult_t* lookup_result = tableLookup(ShortStrongArray_id);
    ObjectProvider_t* SSA_Creator = lookup_result->result;
    //free(lookup_result);

    XYResult_t* SSA_result = SSA_Creator->create((const char*)&KeySet_id, NULL);
    XYObject_t* pubkeyArray_object = SSA_result->result;
    ShortStrongArray_t* pubkeyArray = pubkeyArray_object->payload;
    //free(SSA_result);
    SSA_result = SSA_Creator->create(SignatureSet_id, NULL);
    XYObject_t* signatureArray_object = SSA_result->result;
    ShortStrongArray_t* signatureArray = signatureArray_object->payload;
    //free(SSA_result);

    self->session->boundWitness->dynamicPublicKeys = pubkeyArray;
    self->session->boundWitness->dynamicSignatures = signatureArray;
    self->session->cycles = 0;
    self->session->boundWitness->hasSentKeysAndPayload = 0;
    XYResult_t* completeBoundWitness_result = self->session->completeBoundWitness(self->session, startingData);
    self->session->cycles = 0;
    pipe->close(self->session);
    if(completeBoundWitness_result->error != OK){
      self->onBoundWitnessEndFailure();
      self->session = NULL;
      RETURN_ERROR(ERR_BOUNDWITNESS_FAILED);
    } else {
      self->updateOriginState(self);
      self->onBoundWitnessEndSuccess(self, (BoundWitness_t*)self->session->boundWitness->boundWitness);
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
uint8_t updateOriginState(NodeBase_t* self, BoundWitness_t* boundWitness){
  XYResult_t* hash_result = boundWitness->getHash(boundWitness, self->hashingProvider);
  if(hash_result->error != OK){ return FALSE; }
  ByteArray_t* hash = hash_result->result;
  self->originChainState->newOriginBlock(self->originChainState, hash);
  free(hash_result);
  return TRUE;
}

/*
* Make a Payload_t* which can be used in a bound witness.
*/
XYResult_t* makePayload(NodeBase_t* self){
  // Set up Payload Array
  XYResult_t* lookup_result = tableLookup((const char*)&IntStrongArray_id);
  if(lookup_result->error!=OK){ RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);}
  ObjectProvider_t* ISA_Creator = lookup_result->result;
  //free(lookup_result);
  XYResult_t* create_result = ISA_Creator->create((const char*)&IntStrongArray_id, NULL); // payload_id?
  if(create_result->error != OK){
    return create_result;
  }
  XYObject_t* payloads_object = create_result->result;
  IntStrongArray_t* payloads_raw = payloads_object->payload;

  lookup_result = tableLookup((const char*)&Payload_id);
  if(lookup_result->error!=OK){ RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);}
  ObjectProvider_t* Payload_Creator = lookup_result->result;
  //free(lookup_result);
  create_result = Payload_Creator->create((const char*)&Payload_id, NULL); // payload_id?
  if(create_result->error != OK){
    return create_result;
  }
  XYObject_t* payload_object = create_result->result;
  Payload_t* payload_raw = payload_object->payload;
  /*
  //free(create_result);
  lookup_result = tableLookup((const char*)&IntWeakArray_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }

  ObjectProvider_t* IWA_Creator = lookup_result->result;
  create_result = IWA_Creator->create(NULL, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject_t* user_signedHeuristics_object = create_result->result;
  IntWeakArray_t* user_signedHeuristics = user_signedHeuristics_object->payload;
  free(create_result);

  create_result = IWA_Creator->create(NULL, NULL);
  if(create_result->error != OK){
    return create_result;
  }
  XYObject_t* user_unsignedHeuristics_object = create_result->result;
  IntWeakArray_t* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
  free(create_result);

  Payload_t* user_payload = malloc(sizeof(Payload_t));
  if(user_payload == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  */

  /*
    Here we collect the correct heuristics given the bitFlag
  */
  XYObject_t* signedPayloads = getSignedPayloads(self);
  XYObject_t* unsignedPayloads = getUnSignedPayloads(self);

  /* Add each heuristic to its corresponding array */
  for(uint8_t i = 0; i < gsignedHeuristicCount; i++){
     XYResult_t* inner_lookup_result = tableLookup((const char*)&signedPayloads[i]);
     if(inner_lookup_result->error == OK){
       payload_raw->signedHeuristics->add(payload_raw->signedHeuristics, &signedPayloads[i]);

     }
     //free(inner_lookup_result);
   }
   for(uint8_t i = 0; i < gunsignedHeuristicCount; i++){

     XYResult_t* inner_lookup_result = tableLookup((const char*)&unsignedPayloads[i]);
     if(inner_lookup_result->error == OK){
       payload_raw->unsignedHeuristics->add(payload_raw->unsignedHeuristics, &unsignedPayloads[i]);
     }
     free(inner_lookup_result);
   }
   //printf("unsignedHueristics size: %d\n", payload_raw->unsignedHeuristics->size);
   gunsignedHeuristicCount = 0;
   gsignedHeuristicCount = 0;


  payload_raw->size = payload_raw->signedHeuristics->size + payload_raw->unsignedHeuristics->size + 4;
  XYResult_t* newObject_result = newObject((const char*)&Payload_id, payload_raw);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  breakpoint();
  XYResult_t* add_result = payloads_raw->add(payloads_raw, newObject_result->result);
  if(add_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  if(return_result){
    return_result->error = OK;
    return_result->result = payloads_raw;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
