/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitness.c
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

#include "ZigZagBoundWitness.h"

/*----------------------------------------------------------------------------*
*  NAME
*      incomingData
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     *boundWitness      [in]      BoundWitness*    The data from the other party.
*     *endpoint          [in]      ByteStrongArray* If not already turned around, decide if what to send sign and send back.
*
*  RETURNS
*      XYResult  [out]      bool       Returns XYResult<ByteArray*> the data to send to the other party.
*----------------------------------------------------------------------------*/
int publicKeyCount = 0;
int signatureCount = 0;
XYResult* incomingData(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness, int endpoint){
  self->publicKeyCount = 0;
  self->signatureCount = 0;
  if(boundWitness != NULL){
    int success = self->addTransfer(self, boundWitness);

    if(success){
      RETURN_ERROR(ERR_BADDATA);
    } else {
      if(publicKeyCount == signatureCount && signatureCount > 0){
        publicKeyCount = 0;
        signatureCount = 0;
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result){
          return_result->error = OK;
          return_result->result = (void*)1;
          return return_result;
        }
      }
    }
  }
  XYObject* selfKeysetObject = NULL;
  if(!self->hasSentKeysAndPayload){
    XYResult* keyset_results = makeSelfKeySet(self);
    selfKeysetObject = keyset_results->result;
    self->dynamicPublicKeys->add(self->dynamicPublicKeys, keyset_results->result);
    //for(int i = 0; i< self->heuristicCount; i++){
    //  XYObject add_object = self->payload[i];

    //  self->dynamicPayloads->add(self->dynamicPayloads, &add_object);
    //}
    self->hasSentKeysAndPayload = 0;
  }

  BoundWitnessTransfer* BoundWitness_raw = malloc(sizeof(BoundWitness));
  if(BoundWitness_raw == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  BoundWitness_raw->size = self->dynamicPublicKeys->size + self->dynamicPayloads->size + self->dynamicSignatures->size +(4*sizeof(char));
  BoundWitness_raw->publicKeys = self->dynamicPublicKeys;
  BoundWitness_raw->payloads = self->dynamicPayloads;
  BoundWitness_raw->signatures = self->dynamicSignatures;
  BoundWitness_raw->getSigningData = BoundWitness_getSigningData;
  BoundWitness_raw->getHash = BoundWitness_getHash;
  self->boundWitness = BoundWitness_raw;

  if(endpoint){
    XYResult* signForSelf_result = self->signForSelf(self);
    if(signForSelf_result->error != OK){
      return signForSelf_result;
    }
    free(signForSelf_result);
  }

  XYResult* lookup_result = lookup((char*)BoundWitnessTransfer_id);
  if(lookup_result->error != OK) {
    return NULL;
  }
  ObjectProvider* BountWitnessTransfer_creator = lookup_result->result;
  XYResult* newObject_result = newObject((char*)BoundWitnessTransfer_id, BoundWitness_raw);

  if(newObject_result->error != OK) { return newObject_result; }

  if(self->dynamicPublicKeys->size != self->dynamicSignatures->size){
    BoundWitnessTransfer* tempWitness_raw = malloc(sizeof(BoundWitness));
    XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }
    ObjectProvider* SSA_Creator = lookup_result->result;
    XYResult* create_result = SSA_Creator->create((char*)&KeySet_id, NULL);
    XYObject* pubkeyArray_object = create_result->result;
    ShortStrongArray* pubkeyArray = pubkeyArray_object->payload;

    lookup_result = lookup((char*)&IntStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }
    ObjectProvider* ISA_Creator = lookup_result->result;
    create_result = ISA_Creator->create((char*)&IntStrongArray_id, NULL);
    XYObject* payloadArray_object = create_result->result;
    IntStrongArray* payloadArray = payloadArray_object->payload;

    lookup_result = lookup((char*)&IntStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }

    pubkeyArray->add(pubkeyArray, selfKeysetObject);
    XYResult* payload_result = self->dynamicPayloads->get(self->dynamicPayloads, 0);
    XYObject* payload_object_two = payload_result->result;
    unsigned char* payloadPtr = payload_object_two->payload;
    lookup_result = lookup((char*)&IntWeakArray_id);
    ObjectProvider* IWA_Creator = lookup_result->result;
    create_result = IWA_Creator->fromBytes((char*)payloadPtr);
    IntWeakArray* fromBytesArray = create_result->result;
    XYResult* newObject_result = newObject((char*)&IntWeakArray_id, fromBytesArray);
    XYObject* arrayObject = newObject_result->result;
    payloadArray->add(payloadArray, arrayObject);
    free(newObject_result);

    /* TODO: In the future lets replace the following lines with one signForSelf call */
    //XYResult* signForSelf_result = self->signForSelf(tempWitness_raw);
    //XYResult* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
    //ByteArray* signingData = signingData_result->result;
    //XYResult* sign_result = self->signer->sign(self->signer, signingData);
    //XYObject* signatureObject = sign_result->result;
    // Get signing data and sign it
    self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
    XYResult* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
    self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
    ByteArray* signingData = signingData_result->result;

    create_result = SSA_Creator->create((char*)&SignatureSet_id, NULL);
    XYObject* signatureArray_object = create_result->result;
    ShortStrongArray* signatureArray = signatureArray_object->payload;

    XYResult* sign_result = self->signer->sign(self->signer, signingData);
    XYObject* signatureObject = sign_result->result;

    // Create SignatureSet Array
    XYResult* add_result = signatureArray->add(signatureArray, signatureObject);
    if(add_result->error != OK) { RETURN_ERROR(ERR_CORRUPTDATA); }
    free(add_result);
    add_result = self->dynamicSignatures->add(self->dynamicSignatures, signatureObject);
    if(add_result->error != OK) { RETURN_ERROR(ERR_CORRUPTDATA); }

    BoundWitness_raw->size = self->dynamicPublicKeys->size + self->dynamicPayloads->size + self->dynamicSignatures->size +(4*sizeof(char));

    tempWitness_raw->signatures = signatureArray;
    tempWitness_raw->publicKeys = pubkeyArray;
    tempWitness_raw->payloads = payloadArray;
    tempWitness_raw->getSigningData = BoundWitness_getSigningData;
    tempWitness_raw->getHash = BoundWitness_getHash;

    newObject_result = newObject((char*)BoundWitnessTransfer_id, tempWitness_raw);
    XYResult* toBytes_result = BountWitnessTransfer_creator->toBytes(newObject_result->result);
    free(newObject_result);
    return toBytes_result;

    //BoundWitness_raw->size = self->
  } else {
    return NULL;
  }

  XYResult* toBytes_result = BountWitnessTransfer_creator->toBytes(newObject_result->result);

  free(newObject_result);
  return toBytes_result;
}

/*----------------------------------------------------------------------------*
*  NAME
*      addTransfer
*
*  DESCRIPTION
*  Adds data from an in progress BoundWitness.
*
*  PARAMETERS
*     *boundWitness  [in]           BoundWitness*   The semi-formed BoudnWitness
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addTransfer(ZigZagBoundWitness* self, BoundWitnessTransfer* boundWitness){
 if(!boundWitness){
   return 1;
 }

 if(boundWitness->publicKeys)
 {
   int result = addIncomingKeys(self, boundWitness->publicKeys);
   if(result) return result;
 } if (boundWitness->payloads) {
   int result = addIncomingPayload(self, boundWitness->payloads);
   if(result) return result;
 } if (boundWitness->signatures) {
   int result = addIncomingSignatures(self, boundWitness->signatures);
   if(result) return result;
 } if(boundWitness->publicKeys == NULL && boundWitness->payloads == NULL && boundWitness->signatures == NULL) {
   return 2;
 }
 return 0;

}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingKeys
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     *incomingKeySets     [in]    XYObject*   The keyset which will be added.
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addIncomingKeys(ZigZagBoundWitness* self, ShortStrongArray* incomingKeySets){
  for(int i = 0;; i++){ //TODO: i<1 is hardwired. This logic is bugged.
    XYResult* get_result = incomingKeySets->get(incomingKeySets, i);
    if(get_result->error != OK){ break; }
    XYObject* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){
      uint16_t encoded_size = to_uint16((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &encoded_size, 2);
      XYResult* array_creator_result = lookup((char*)&KeySet_id);
      ObjectProvider* array_creator = array_creator_result->result;

      /* The first two bytes were in big endian, to_uint16 flips them to little endian */
      uint16_t fixSize = to_uint16((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &fixSize, 2);
      XYResult* create_result = array_creator->fromBytes(xyobject->payload);
      XYObject* payloadObject = create_result->result;


      XYResult* add_result = self->dynamicPublicKeys->add(self->dynamicPublicKeys, payloadObject);
      publicKeyCount += 1;
      if(add_result->error != OK) return 1;
      return 0;
    } else {
      return 2;
    }
  }
  return 1;
}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingPayload
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     *incomingPayloads     [in]    XYObject*   The payload which will be added.
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addIncomingPayload(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads){
  for(int i = 0; i<2; i++){ //TODO: i<1 is hardwired. This logic is bugged.
    XYResult* get_result = incomingPayloads->get(incomingPayloads, i);
    XYObject* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x04){
      char* userPayloadBytes = xyobject->payload;
      if(littleEndian()){
        uint32_t correctedSize = to_uint32((unsigned char*)userPayloadBytes);
        memcpy(userPayloadBytes, &correctedSize, 4);
      }
      char IntWeakArrayID[2] = {0x01, 0x06};
      XYResult* lookup_result = lookup(IntWeakArrayID);
      ObjectProvider* weakArrayCreator = lookup_result->result;

      XYResult* fromBytes_result = weakArrayCreator->fromBytes(&userPayloadBytes[4]);
      if(fromBytes_result->error != OK) return 1;
      IntWeakArray* signedHeuristicsArray = fromBytes_result->result;

      free(fromBytes_result);
      fromBytes_result = weakArrayCreator->fromBytes(&userPayloadBytes[4+signedHeuristicsArray->size]);
      if(fromBytes_result->error != OK) return 1;
      IntWeakArray* unsignedHeuristicsArray = fromBytes_result->result;

      Payload* userPayload = (Payload*)userPayloadBytes;
      userPayload->signedHeuristics = signedHeuristicsArray;
      userPayload->unsignedHeuristics = unsignedHeuristicsArray;

      /*
      uint32_t encoded_size = to_uint32((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &encoded_size, 4);
      */
      XYResult* add_result = self->dynamicPayloads->add(self->dynamicPayloads, xyobject);
      if(add_result->error != OK) return 1;
      return 0;
    } else {
      return 2;
    }
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingSignatures
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     *incomingKeySets     [in]    XYObject*   The signatures which will be added.
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addIncomingSignatures(ZigZagBoundWitness* self, ShortStrongArray* incomingSignatures){
  for(int i = 0; ; i++){
    XYResult* get_result = incomingSignatures->get(incomingSignatures, i);
    XYObject* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x03){
      char* userSignatureBytes = xyobject->payload;
      if(littleEndian()){
        uint32_t correctedSize = to_uint32((unsigned char*)userSignatureBytes);
        memcpy(userSignatureBytes, &correctedSize, 4);
      }
      char ShortWeakArrayID[2] = {0x01, 0x05};
      XYResult* lookup_result = lookup(ShortWeakArrayID);
      ObjectProvider* weakArrayCreator = lookup_result->result;
      XYResult* fromBytes_result = weakArrayCreator->fromBytes(&userSignatureBytes[3]);
      if(fromBytes_result->error != OK) return 1;
      XYObject* signatureArrayObject = fromBytes_result->result;
      free(fromBytes_result);
      XYResult* add_result = self->dynamicSignatures->add(self->dynamicSignatures, signatureArrayObject);
      if(add_result->error != OK) return 1;
      signatureCount += 1;
      return 0;
    } else {
      return 2;
    }
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      makeSelfKeySet
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     *incomingKeySets     [in]    XYObject*   The signatures which will be added.
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
XYResult* makeSelfKeySet(ZigZagBoundWitness* self){
  // Get a copy of the public key we are using currently
  XYResult* getPublicKey_result = self->signer->getPublicKey(self->signer);
  if(getPublicKey_result->error != 0){
    return getPublicKey_result;
  }
  XYObject* publicKey = getPublicKey_result->result;
  free(getPublicKey_result);

  // Create Keyset Array
  XYResult* lookup_result = lookup((char*)&KeySet_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider* KeySet_creator = lookup_result->result;
  XYResult* create_result = KeySet_creator->create((char*)KeySet_id, NULL);
  XYObject* keysetObject = create_result->result;
  ShortWeakArray* keysetArray = keysetObject->payload;
  free(lookup_result);

  // Add public key object to keyset array
  XYResult* add_result = keysetArray->add(keysetArray, publicKey);
  if(add_result->error != OK){
    return add_result;
  }
  free(add_result);
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = keysetObject;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_signForSelf
*
*  DESCRIPTION
*      Routine that retrieves all the signing data, signs it, and appeneds that
*      signature to the ZigZagBoundWitness
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult* signForSelf(ZigZagBoundWitness* self){
  // Get signing data and sign it
  self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
  XYResult* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
  self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
  ByteArray* signingData = signingData_result->result;
  XYResult* sign_result = self->signer->sign(self->signer, signingData);
  XYObject* signatureObject = sign_result->result;

  // Create SignatureSet Array

  XYResult* lookup_result = lookup((char*)SignatureSet_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider* SignatureSet_creator = lookup_result->result;
  XYResult* create_result = SignatureSet_creator->create((char*)SignatureSet_id, NULL);
  if(create_result->result != OK){
    return create_result;
  }
  XYObject* signatureSetObject = create_result->result;
  ShortWeakArray* keysetArray = signatureSetObject->payload;
  XYResult* add_result = keysetArray->add(keysetArray, signatureObject);
  if(add_result->error != OK){
    RETURN_ERROR(ERR_BADSIG);
  } else {
    return self->dynamicSignatures->add(self->dynamicSignatures, add_result->result);
  }

}
