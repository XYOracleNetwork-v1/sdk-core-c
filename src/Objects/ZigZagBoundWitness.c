#ifndef ORIGINCHAIN_H
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
XYResult* incomingData(ZigZagBoundWitness* self, BoundWitness* boundWitness, int endpoint){

  /*
  * get size of signature array
  * to send and appened in progress boundWitness to local
  * BoundWitness object
  */
  int signatureRecievedSize = 0;

  if(boundWitness != NULL){
    int success = self->addTransfer(self, boundWitness);
    signatureRecievedSize = boundWitness->signatures->size;
    if(!success){
      RETURN_ERROR(ERR_BADDATA);
    }
  }

  if(!self->hasSentKeysAndPayload){
   self->dynamicPublicKeys->add(self->dynamicPublicKeys, makeSelfKeyset());
   self->dynamicPayloads->add(self->dynamicPayloads, self->payload);
   self->hasSentKeysAndPayload = 0;
  }

  BoundWitness* BoundWitness_raw = malloc(sizeof(BoundWitness));

  if(endpoint){
    XYResult* signForSelf_result = self->signForSelf(self);
    if(signForSelf_result->error != OK)
      return signForSelf_result;
    free(signForSelf_result);
  }

  BoundWitness_raw->size = self->dynamicPublicKeys->size + self->dynamicPayloads->size + self->dynamicSignatures->size +(4*sizeof(char));
  BoundWitness_raw->publicKeys = self->dynamicPublicKeys;
  BoundWitness_raw->payloads = self->dynamicPayloads;
  BoundWitness_raw->signatures = self->dynamicSignatures;
  BoundWitness_raw->getSigningData = &BoundWitness_getSigningData;
  BoundWitness_raw->toTransfer = &BoundWitness_toTransfer;
  self->boundWitness = BoundWitness_raw;

  return BoundWitness_raw->toTransfer(BoundWitness_raw);
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
int addTransfer(ZigZagBoundWitness* self, BoundWitness* boundWitness){
 if(!boundWitness){
   return 1;
 }

 if(boundWitness->publicKeys)
 {
   int result = addIncomingKeys(self, boundWitness->publicKeys);
   if(result) return result;
 } else if (boundWitness->payloads) {
   int result = addIncomingPayload(self, boundWitness->payloads);
   if(result) return result;
 } else if (boundWitness->signatures) {
   int result = addIncomingSignatures(self, boundWitness->signatures);
   if(result) return result;
 } else {
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
  for(int i = 0; ; i++){
    XYResult* get_result = incomingKeySets->get(incomingKeySets, i);
    XYObject* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){
      XYResult* add_result = self->boundWitness->publicKeys->add(self->boundWitness->publicKeys, xyobject);
      if(add_result->error != OK) return 1;
    } else {
      return 2;
    }
  }
  return 0;
}

/*----------------------------------------------------------------------------*
*  NAME
*      addIncomingPayload
*
*  DESCRIPTION
*  Adds KeySet from an incoming payload
*
*  PARAMETERS
*     *incomingKeySets     [in]    XYObject*   The payload which will be added.
*
*  RETURNS
*      XYResult  [out]      bool       Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addIncomingPayload(ZigZagBoundWitness* self, IntStrongArray* incomingPayloads){
  for(int i = 0; ; i++){
    XYResult* get_result = incomingPayloads->get(incomingPayloads, i);
    XYObject* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){
      XYResult* add_result = self->boundWitness->payloads->add(self->boundWitness->payloads, xyobject);
      if(add_result->error != OK) return 1;
    } else {
      return 2;
    }
  }
  return 0;
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
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){
      XYResult* add_result = self->boundWitness->signatures->add(self->boundWitness->signatures, xyobject);
      if(add_result->error != OK) return 1;
    } else {
      return 2;
    }
  }
  return 0;
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
  Object_Creator* KeySet_creator = lookup_result->result;
  XYResult* create_result = KeySet_creator->create((char*)&KeySet_id, NULL);
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
*      Method somewhat similar to BoundWitness_creator_toBytes that creates a
*      Byte array with the XYO Bound Witness Transfer object type bytes.
*
*  PARAMETERS
*     *user_BoundWitness                    [in]       BoundWitness*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* with a byteBuffer as the result.
*----------------------------------------------------------------------------*/
XYResult* signForSelf(ZigZagBoundWitness* self){
  XYResult* signingData_result = self->boundWitness->getSigningData(self);
  ByteArray* signingData = signingData_result->result;
  XYResult* sign_result = self->signer->sign(self->signer, signingData);
  ByteArray* signature = sign_result->result;

}

#define ORIGINCHAIN_H
#endif
