/**
 ****************************************************************************************
 *
 * @file zigzagboundwitness.c
 *
 * @XY4 project source code.
 *
 * @brief primary zig zag bound witness routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef ZIGZAGBOUNDWITNESS_H
#define ZIGZAGBOUNDWITNESS_H

#include <stdlib.h>
#include "xyo.h"
#include "ZigZagBoundWitness.h"

/*----------------------------------------------------------------------------*
*  NAME
*      incomingData
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     self              [in]      ZigZagBoundWitness_t*
*     *boundWitness     [in]      BoundWitness*     The data from the other party.
*     
*     *endpoint         [in]      int               If not already turned around, 
*                                                   decide if what to send sign 
*                                                   and send back.
*  RETURNS
*     toBytes_result    [out]     XYResult_t        Returns XYResult<ByteArray*> the data 
*                                                   to send to the other party.
*----------------------------------------------------------------------------*/
XYResult_t* incomingData(ZigZagBoundWitness_t* self, 
                         BoundWitnessTransfer* boundWitness, 
                         int endpoint){
  
  if(boundWitness != NULL){
    int success = self->addTransfer(self, boundWitness);
    
    if(!success){
      RETURN_ERROR(ERR_BADDATA);
    }
  }

  if(!self->hasSentKeysAndPayload){
    XYResult_t* keyset_results = makeSelfKeySet(self);
    self->dynamicPublicKeys->add(self->dynamicPublicKeys, keyset_results->result);
    self->dynamicPayloads->add(self->dynamicPayloads, self->payload);
    self->hasSentKeysAndPayload = 0;
  }

  if(endpoint){
    XYResult_t* signForSelf_result = self->signForSelf(self);
    if(signForSelf_result->error != OK){
      return signForSelf_result;
    }
    free(signForSelf_result);
  }
  BoundWitnessTransfer* BoundWitness_raw = malloc(sizeof(BoundWitness));
  if(BoundWitness_raw == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  BoundWitness_raw->size = self->dynamicPublicKeys->size + self->dynamicPayloads->size + self->dynamicSignatures->size +(4*sizeof(char));
  BoundWitness_raw->publicKeys = self->dynamicPublicKeys;
  BoundWitness_raw->payloads = self->dynamicPayloads;
  BoundWitness_raw->signatures = self->dynamicSignatures;
  BoundWitness_raw->getSigningData = &BoundWitness_getSigningData;
  
  self->boundWitness = BoundWitness_raw;

  XYResult_t* lookup_result = tableLookup((char*)&BoundWitnessTransfer_id);
  
  if(lookup_result->error != OK) return lookup_result;
  
  ObjectProvider_t* BountWitnessTransfer_creator = lookup_result->result;
  XYResult_t* newObject_result = newObject((char*)&BoundWitnessTransfer_id, BoundWitness_raw);
  
  if(newObject_result->result != OK) return newObject_result;
  
  XYResult_t* toBytes_result = BountWitnessTransfer_creator->toBytes(newObject_result->result);
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
*     self            [in]      ZigZagBoundWitness_t*   
*     boundWitness    [in]      BoundWitnessTransfer*   The semi-formed BoudnWitness
*
*  RETURNS
*      int            [out]     Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
int addTransfer(ZigZagBoundWitness_t* self, BoundWitnessTransfer* boundWitness){
  
 if(!boundWitness){
   return 1;          //TODO: wal, 1 what?  Perhaps a good place for a constant
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
   return 2;    //TODO: wal, why 2?  how 'bout a meaningful constant here?
 }
 return 0;      //TODO: wal, why 0?  how 'bout a meaningful constant here?

}

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
int addIncomingKeys(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingKeySets){
  
  for(int i = 0; ; i++){
    XYResult_t* get_result = incomingKeySets->get(incomingKeySets, i);
    XYObject_t* xyobject = get_result->result;
    
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){   //TODO: constants would be good here
      XYResult_t* add_result = self->boundWitness->publicKeys->add(self->boundWitness->publicKeys, xyobject);
      if(add_result->error != OK) return 1;
    } else {
      return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
    }
  }
}

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
int addIncomingPayload(ZigZagBoundWitness_t* self, IntStrongArray_t* incomingPayloads){
  
  for(int i = 0; ; i++){
    XYResult_t* get_result = incomingPayloads->get(incomingPayloads, i);
    XYObject_t* xyobject = get_result->result;
    
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){   //TODO: constants would be good here
      XYResult_t* add_result = self->boundWitness->payloads->add(self->boundWitness->payloads, xyobject);
      
      if(add_result->error != OK) return 1;   //TODO: wal, why 1?  how 'bout a meaningful constant here?
      
    } else {
      return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
    }
  }
}

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
int addIncomingSignatures(ZigZagBoundWitness_t* self, ShortStrongArray_t* incomingSignatures){
  
  for(int i = 0; ; i++){
    XYResult_t* get_result = incomingSignatures->get(incomingSignatures, i);
    XYObject_t* xyobject = get_result->result;
    if(xyobject->id[0] == 0x02 && xyobject->id[1] == 0x02){
      XYResult_t* add_result = self->boundWitness->signatures->add(self->boundWitness->signatures, xyobject);
      
      if(add_result->error != OK) return 1;   //TODO: wal, why 1?  how 'bout a meaningful constant here?
    } else {
      return 2;   //TODO: wal, why 2?  how 'bout a meaningful constant here?
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
*     self                [in]    ZigZagBoundWitness_t*   
*
*  RETURNS
*      XYResult_t         [out]   bool        Returns Bool True if it succeeded.
*----------------------------------------------------------------------------*/
XYResult_t* makeSelfKeySet(ZigZagBoundWitness_t* self){
  
  // Get a copy of the public key we are using currently
  XYResult_t* getPublicKey_result = self->signer->getPublicKey(self->signer);
  if(getPublicKey_result->error != 0){
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
  free(lookup_result);

  // Add public key object to keyset array
  XYResult_t* add_result = keysetArray->add(keysetArray, publicKey);
  if(add_result->error != OK){
    return add_result;
  }
  free(add_result);
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
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
XYResult_t* signForSelf(ZigZagBoundWitness_t* self){
  
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
  free(lookup_result);
  free(signing_result);
  free(create_result);

  return keysetArray->add(keysetArray, signatureObject);
}

#endif

// end of file zigzagboundwitness.c

