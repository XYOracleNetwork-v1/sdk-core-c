/**
 ****************************************************************************************
 *
 * @file zigzagboundwitness.c
 *
 * @XYO Core library source code.
 *
 * @brief primary zig zag bound witness routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "ZigZagBoundWitnessSession.h"

/*----------------------------------------------------------------------------*
*  NAME
*      incomingData
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     self              [in]      ZigZagBoundWitness_t*
*     *boundWitness     [in]      BoundWitness_t*     The data from the other party.
*
*     *endpoint         [in]      int               If not already turned around,
*                                                   decide if what to send sign
*                                                   and send back.
*  RETURNS
*     toBytes_result    [out]     XYResult_t        Returns XYResult<ByteArray_t*> the data
*                                                   to send to the other party.
*----------------------------------------------------------------------------*/
int publicKeyCount = 0; //TODO: Are these values needed?
int signatureCount = 0;
XYResult_t incomingData(ZigZagBoundWitness_t* self,
                         //BoundWitnessTransfer_t* boundWitness,
                         int endpoint){
  /*
  static int result_int;
  self->publicKeyCount = 0;
  self->signatureCount = 0;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &result_int;

  if(boundWitness != NULL){
    preallocated_return_result_ptr = self->addTransfer(self, boundWitness);

    if( !(int*)(preallocated_return_result_ptr->result)) {RETURN_ERROR(ERR_BADDATA);}
    if(publicKeyCount == signatureCount && signatureCount > 0){
      publicKeyCount = 0;
      signatureCount = 0;
    }
  }
  XYObject_t* selfKeysetObject = NULL;

  if(!self->hasSentKeysAndPayload){
    XYResult_t* keyset_results = makeSelfKeySet(self);
    selfKeysetObject = keyset_results->result;
    self->hasSentKeysAndPayload = 1;
  }
  if(self->dynamicPublicKeys->size > 4){
    self->dynamicPublicKeys->add(self->dynamicPublicKeys, selfKeysetObject);
  }



  BoundWitnessTransfer_t* BoundWitness_raw = malloc(sizeof(BoundWitness_t)); //TODO: Investigate the behavior here why isn't this BoundWitnessTransfer_t

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(BoundWitness_raw == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  BoundWitness_raw->size = self->dynamicPublicKeys->size + self->dynamicPayloads->size + self->dynamicSignatures->size +(4*sizeof(char));
  BoundWitness_raw->publicKeys = self->dynamicPublicKeys;
  BoundWitness_raw->payloads = self->dynamicPayloads;
  BoundWitness_raw->signatures = self->dynamicSignatures;
  BoundWitness_raw->getSigningData = BoundWitness_getSigningData;
  BoundWitness_raw->getHash = BoundWitness_getHash;
  self->boundWitness = BoundWitness_raw;

  if(endpoint){
    XYResult_t* signForSelf_result = self->signForSelf(self);
    if(signForSelf_result->error != OK){
      return signForSelf_result;
    }
    free(signForSelf_result);
  }

  XYResult_t* lookup_result = tableLookup(BoundWitnessTransfer_id);
  if(lookup_result->error != OK) { return NULL; }
  ObjectProvider_t* BountWitnessTransfer_creator = lookup_result->result;
  XYResult_t* newObject_result = newObject(BoundWitnessTransfer_id, BoundWitness_raw);

  if(newObject_result->error != OK) { return newObject_result; }

  if(self->dynamicPublicKeys->size != self->dynamicSignatures->size){
    BoundWitnessTransfer_t* tempWitness_raw = malloc(sizeof(BoundWitness_t)); //TODO: Investigate the behavior here why isn't this BoundWitnessTransfer_t
    XYResult_t* lookup_result = tableLookup((const char*)&ShortStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }
    ObjectProvider_t* SSA_Creator = lookup_result->result;
    XYResult_t* create_result = SSA_Creator->create((const char*)&KeySet_id, NULL);
    XYObject_t* pubkeyArray_object = create_result->result;
    ShortStrongArray_t* pubkeyArray = pubkeyArray_object->payload;

    lookup_result = tableLookup((const char*)&IntStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }
    ObjectProvider_t* ISA_Creator = lookup_result->result;
    create_result = ISA_Creator->create((const char*)&IntStrongArray_id, NULL);
    XYObject_t* payloadArray_object = create_result->result;
    IntStrongArray_t* payloadArray = payloadArray_object->payload;

    lookup_result = tableLookup((const char*)&IntStrongArray_id);
    if(lookup_result->error != OK){
      return NULL;
    }

    if(selfKeysetObject != NULL){
      pubkeyArray->add(pubkeyArray, selfKeysetObject);
    }

    XYResult_t* payload_result = self->dynamicPayloads->get(self->dynamicPayloads, 0);
    XYObject_t* payload_object_two = payload_result->result;
    unsigned char* payloadPtr = payload_object_two->payload;
    lookup_result = tableLookup((const char*)&IntWeakArray_id);
    ObjectProvider_t* IWA_Creator = lookup_result->result;
    create_result = IWA_Creator->fromBytes((char*)payloadPtr);
    IntWeakArray_t* fromBytesArray = create_result->result;
    XYResult_t* newObject_result = newObject((const char*)&IntWeakArray_id, fromBytesArray);
    XYObject_t* arrayObject = newObject_result->result;
    payloadArray->add(payloadArray, arrayObject);
    free(newObject_result);

    /* TODO: In the future lets replace the following lines with one signForSelf call * /
    //XYResult_t* signForSelf_result = self->signForSelf(tempWitness_raw);
    //XYResult_t* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
    //ByteArray_t* signingData = signingData_result->result;
    //XYResult_t* sign_result = self->signer->sign(self->signer, signingData);
    //XYObject_t* signatureObject = sign_result->result;
    // Get signing data and sign it
    self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
    XYResult_t* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
    self->boundWitness->payloads->size = to_uint32((unsigned char*)&self->boundWitness->payloads->size);
    ByteArray_t* signingData = signingData_result->result;

    create_result = SSA_Creator->create((const char*)&SignatureSet_id, NULL);
    XYObject_t* signatureArray_object = create_result->result;
    ShortStrongArray_t* signatureArray = signatureArray_object->payload;

    XYResult_t* sign_result = self->signer->sign(self->signer, signingData);
    XYObject_t* signatureObject = sign_result->result;

    // Create SignatureSet Array
    XYResult_t* add_result = signatureArray->add(signatureArray, signatureObject);
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

    newObject_result = newObject(BoundWitnessTransfer_id, tempWitness_raw);
    XYResult_t* toBytes_result = BountWitnessTransfer_creator->toBytes(newObject_result->result);
    free(newObject_result);
    return toBytes_result;

    //BoundWitness_raw->size = self->
  } else {
    return NULL;
  }

  XYResult_t* toBytes_result = BountWitnessTransfer_creator->toBytes(newObject_result->result);
  free(newObject_result);

  return toBytes_result;
  */
}

/*----------------------------------------------------------------------------*
*  NAME
*      addTransfer
*
*  DESCRIPTION
*  Adds data from an in progress BoundWitness.
*
*  PARAMETERS
*     self            [in]      ZigZagBoundWitness_t*
*     boundWitness    [in]      BoundWitnessTransfer_t*   The semi-formed BoudnWitness
*
*  RETURNS
*      XYResult       [out]     bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
/* XYResult_t* addTransfer(ZigZagBoundWitness_t* self, BoundWitnessTransfer_t* boundWitness){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self || !boundWitness) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  //TODO: wal, if this routine returns a bool, why is it declared as an int?

  static int result_int;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &result_int;

  /********************************/
  /* guard against bad input data */
  /******************************** /

  if(!self || !boundWitness) {RETURN_ERROR(ERR_BADDATA);}

  if(!boundWitness){
    result_int = 1;       //TODO: wal, 1 what?  Perhaps a good place for a constant
    return preallocated_return_result_ptr;
  }

  if(boundWitness->publicKeys)
  {
    preallocated_return_result_ptr = addIncomingKeys(self, boundWitness->publicKeys);

    if( (int*)(preallocated_return_result_ptr->result) > 0) return preallocated_return_result_ptr;

    } else if (boundWitness->payloads) {

    //int result = addIncomingPayload(self, boundWitness->payloads);
    preallocated_return_result_ptr = addIncomingPayload(self, boundWitness->payloads);

    if( (int*)(preallocated_return_result_ptr->result) > 0) return preallocated_return_result_ptr;

    } else if (boundWitness->signatures) {

    //int result = addIncomingSignatures(self, boundWitness->signatures);
    preallocated_return_result_ptr = addIncomingSignatures(self, boundWitness->signatures);

    //if(result) return result;
    if( (int*)(preallocated_return_result_ptr->result) > 0) return preallocated_return_result_ptr;

    } else {

    result_int = 2;
    //return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
    return preallocated_return_result_ptr;
    }

    result_int = 0;
    //return 0;   //TODO: wal, why 0?  how 'bout a meaningful constant here?
    return preallocated_return_result_ptr;
    */
//}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingKeys
*
*  DESCRIPTION
*     Adds KeySet from an incoming payload
*
*  PARAMETERS
*     self                [in]    ZigZagBoundWitness_t*
*     *incomingKeySets    [in]    ShortStrongArray_t*   The keyset which will be added.
*
*  RETURNS
*      XYResult_t         [out]   bool        Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
/*
XYResult_t* addIncomingKeys(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingKeySets){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self || !incomingKeySets) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  static int result_int;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &result_int;

  for(int i = 0; ; i++){

    XYResult_t* get_result = incomingKeySets->get(incomingKeySets, i);
    if(get_result->error != OK){ break; }
    XYObject_t* xyobject = get_result->result;

    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){

      uint16_t encoded_size = to_uint16((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &encoded_size, 2);
      XYResult_t* array_creator_result = tableLookup((const char*)&KeySet_id);
      ObjectProvider_t* array_creator = array_creator_result->result;

      /* The first two bytes were in big endian, to_uint16 flips them to little endian * /
      uint16_t fixSize = to_uint16((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &fixSize, 2);
      //char* payloadptr = xyobject->payload;
      XYResult_t* create_result = array_creator->fromBytes(xyobject->payload);
      ShortWeakArray_t* payloadObject = create_result->result;
      preallocated_return_result_ptr = newObject((const char*)&KeySet_id, payloadObject);
      if(preallocated_return_result_ptr->error != OK){ return preallocated_return_result_ptr; }

      preallocated_return_result_ptr = self->dynamicPublicKeys->add(self->dynamicPublicKeys, preallocated_return_result_ptr->result);
      publicKeyCount += 1;
      if(preallocated_return_result_ptr->error != OK) return preallocated_return_result_ptr;
      result_int = 0;
      preallocated_return_result_ptr->error = OK;
      preallocated_return_result_ptr->result = &result_int;
      return preallocated_return_result_ptr;
    } else {

      result_int = 2;

      //return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
      return preallocated_return_result_ptr;
    }
  }
  */
//}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingPayload
*
*  DESCRIPTION
*     Adds KeySet from an incoming payload
*
*  PARAMETERS
*     self                [in]    ZigZagBoundWitness_t*
*     incomingPayloads    [in]    IntStrongArray_t*   The payload which will be added.
*
*  RETURNS
*      XYResult_t         [out]   bool        Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
/* XYResult_t* addIncomingPayload(ZigZagBoundWitness_t* self, IntStrongArray_t* incomingPayloads){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self || !incomingPayloads) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  static int result_int;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &result_int;

  for(int i = 0; i<2; i++){ //TODO: i<1 is hardwired. This logic is bugged.
    XYResult_t* get_result = incomingPayloads->get(incomingPayloads, i);
    XYObject_t* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x04){
      char* userPayloadBytes = xyobject->payload;
      if(littleEndian()){
        uint32_t correctedSize = to_uint32((unsigned char*)userPayloadBytes);
        memcpy(userPayloadBytes, &correctedSize, 4);
      }
      char IntWeakArrayID[2] = {0x01, 0x06};
      XYResult_t* lookup_result = tableLookup(IntWeakArrayID);
      ObjectProvider_t* weakArrayCreator = lookup_result->result;

      XYResult_t* fromBytes_result = weakArrayCreator->fromBytes(&userPayloadBytes[4]);
      if(fromBytes_result->error != OK) return 1;
      IntWeakArray_t* signedHeuristicsArray = fromBytes_result->result;

      free(fromBytes_result);
      fromBytes_result = weakArrayCreator->fromBytes(&userPayloadBytes[4+signedHeuristicsArray->size]);
      if(fromBytes_result->error != OK) return 1;
      IntWeakArray_t* unsignedHeuristicsArray = fromBytes_result->result;

      Payload_t* userPayload = (Payload_t*)userPayloadBytes;
      userPayload->signedHeuristics = signedHeuristicsArray;
      userPayload->unsignedHeuristics = unsignedHeuristicsArray;

      /*
      uint32_t encoded_size = to_uint32((unsigned char*)xyobject->payload);
      memcpy(xyobject->payload, &encoded_size, 4);
      * /
      XYResult_t* add_result = self->dynamicPayloads->add(self->dynamicPayloads, xyobject);
      if(add_result->error != OK) return 1;
    } else {

      result_int = 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?

      //return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
      return preallocated_return_result_ptr;
    }
  }
  */
//}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingSignatures
*
*  DESCRIPTION
*   Adds KeySet from an incoming payload
*
*  PARAMETERS
*     self                [in]    ZigZagBoundWitness_t*
*     incomingSignatures  [in]    ShortStrongArray_t*   The signatures which will be added.
*
*  RETURNS
*      XYResult_t         [out]   bool        Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
/* XYResult_t* addIncomingSignatures(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingSignatures){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self || !incomingSignatures) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  static int result_int;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &result_int;

  for(int i = 0; ; i++){

    XYResult_t* get_result = incomingSignatures->get(incomingSignatures, i);
    XYObject_t* xyobject = get_result->result;
    if(xyobject == NULL ){ free(get_result); return 0; }
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x03){
      //char* userSignatureBytes = xyobject->payload;
      /*
      f(littleEndian()){
        uint32_t correctedSize = to_uint32((unsigned char*)userSignatureBytes);
        memcpy(userSignatureBytes, &correctedSize, 4);
      }
      char ShortWeakArrayID[2] = {0x01, 0x05};
      XYResult_t* lookup_result = tableLookup(ShortWeakArrayID);
      ObjectProvider_t* weakArrayCreator = lookup_result->result;
      XYResult_t* fromBytes_result = weakArrayCreator->fromBytes(&userSignatureBytes[3]);
      if(fromBytes_result->error != OK) return 1;
      ShortWeakArray_t* signatureArray = fromBytes_result->result;

      XYResult_t* newObject_result = newObject((const char*)&SignatureSet_id, signatureArray);
      if(newObject_result->error != OK){  result_int = -1; preallocated_return_result_ptr;  }
      * /
      const char* incomingSignaturesAddr = incomingSignatures->payload;
      secp256k1Signature_t* signature = malloc(sizeof(secp256k1Signature_t));
      signature->size = *(incomingSignaturesAddr+6);

      signature->signature = malloc(sizeof(char)*signature->size);
      memcpy(signature->signature, incomingSignaturesAddr+7, signature->size-1);
      XYResult_t* newObject_result = newObject((const char*)incomingSignaturesAddr+4, signature);
      XYResult_t* add_result = self->dynamicSignatures->add(self->dynamicSignatures, newObject_result->result);
      if(add_result->error != OK){ result_int = 1; preallocated_return_result_ptr; };
      signatureCount += 1;
      //return 0; //TODO: Assure that this function behaves to spec.
    } else {

      result_int = 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?

      //return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
      return preallocated_return_result_ptr;
    }
  }
  */
//}

/*----------------------------------------------------------------------------*
*  NAME
*      makeSelfKeySet
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     self                [in]    ZigZagBoundWitness_t*
*
*  RETURNS
*     XYResult_t          [out]   bool        Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
/* XYResult_t* makeSelfKeySet(ZigZagBoundWitness_t* self){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  // Get a copy of the public key we are using currently
  XYResult_t* getPublicKey_result = self->signer->getPublicKey(self->signer);
  if(getPublicKey_result->error != 0){
    printf("Failed to get public key.\n");
    return getPublicKey_result;
  }
  XYObject_t* publicKey = getPublicKey_result->result;
  free(getPublicKey_result);

  // Create Keyset Array
  XYResult_t* lookup_result = tableLookup((char*)&KeySet_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider_t* KeySet_creator = lookup_result->result;
  XYResult_t* create_result = KeySet_creator->create((char*)&KeySet_id, NULL);
  XYObject_t* keysetObject = create_result->result;
  ShortWeakArray_t* keysetArray = keysetObject->payload;
  //free(lookup_result);

  // Add public key object to keyset array
  XYResult_t* add_result = keysetArray->add(keysetArray, publicKey);
  if(add_result->error != OK){
    return add_result;
  }
  free(add_result);

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = keysetObject;

  return preallocated_return_result_ptr;
  */
//}

/*----------------------------------------------------------------------------*
*  NAME
*      signForSelf
*
*  DESCRIPTION
*      Routine that retrieves all the signing data, signs it, and appeneds that
*      signature to the ZigZagBoundWitness
*
*  PARAMETERS
*     self          [in]       ZigZagBoundWitness_t*
*
*  RETURNS
*      _result*     [out]      XYResult_t*
*
*----------------------------------------------------------------------------*/
/* XYResult_t* signForSelf(ZigZagBoundWitness_t* self){

  /********************************/
  /* some guards against bad data */
  /******************************** /

  if(!self) {RETURN_ERROR(ERR_BADDATA);}  // return the error now

  // Get signing data and sign it
  XYResult_t* signingData_result = self->boundWitness->getSigningData(self->boundWitness);
  ByteArray_t* signingData = signingData_result->result;
  XYResult_t* signing_result = self->signer->sign(self->signer, signingData);
  XYObject_t* signatureObject = signing_result->result;

  // Create SignatureSet Array
  XYResult_t* lookup_result = tableLookup((char*)&SignatureSet_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider_t* SignatureSet_creator = lookup_result->result;
  XYResult_t* create_result = SignatureSet_creator->create((char*)&KeySet_id, NULL);
  if(create_result->result != OK){
    return create_result;
  }
  XYObject_t* signatureSetObject = create_result->result;
  ShortWeakArray_t* keysetArray = signatureSetObject->payload;
  XYResult_t* add_result = keysetArray->add(keysetArray, signatureObject);
  if(add_result->error != OK){
    RETURN_ERROR(ERR_BADSIG);
  } else {
    //free(lookup_result);
    free(signing_result);
    free(create_result);
    return self->dynamicSignatures->add(self->dynamicSignatures, add_result->result);
  }
  */
//}

// end of file zigzagboundwitness.c
