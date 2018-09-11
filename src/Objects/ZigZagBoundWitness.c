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

  if(boundWitness == NULL){
    RETURN_ERROR(ERR_BADDATA);
  }

  /*
   * First we generate three arrays.
   * publicKeys, payloads, and signatures to send
   */
  XYResult* lookup_result = lookup((char*)&ShortStrongArray_id);
  if(lookup_result->error != OK){
    RETURN_ERROR(ERR_CRITICAL);
  }
  Object_Creator* SSA_Creator = lookup_result->result;
  free(lookup_result);

  lookup_result = lookup((char*)&IntStrongArray_id);
  if(lookup_result->error != OK){
    RETURN_ERROR(ERR_CRITICAL);
  }
  Object_Creator* ISA_Creator = lookup_result->result;
  free(lookup_result);

  // Generate KeySet Array
  XYResult* create_result = SSA_Creator->create((char*)&KeySet_id, NULL);
  if(create_result->error != OK){
    return create_result->error;
  }
  XYObject*  publicKeys_object = create_result->result;
  ShortStrongArray* keysToSend = publicKeys_object->payload;
  free(create_result);

  // Generate Payloads Array
  create_result = ISA_Creator->create((char*)&Payload_id, NULL);
  if(create_result->error != OK){
    return create_result->error;
  }
  XYObject*  payloads_object = create_result->result;
  IntStrongArray* payloadsToSend = payloads_object->payload;
  free(create_result);

  // Generate Signature Array
  create_result = SSA_Creator->create((char*)&SignatureSet_id, NULL);
  if(create_result->error != OK){
    return create_result->error;
  }
  XYObject*  signatures_object = create_result->result;
  ShortStrongArray* signaturesToSend = signatures_object->payload;
  free(create_result);

  /*
   * get size of signature to send and
   * add it to the ZZBW object
   */
   int signatureRecievedSize = 0;
   signatureRecievedSize = boundWitness->signatures->size;
   self->addTransfer(self, self->boundWitness);

   if(!self->hasSentKeysAndPayload){
     keysToSend->add(keysToSend, makeSelfKeyset());
     payloadsToSend->add(payloadsToSend, self->payload);
     self->hasSentKeysAndPayload = 0;
   }

   if(signaturesToSend->size != keysToSend->size){
     if(signaturesToSend->size == 0 && !endPoint){
       keysToSend.add()
     }
   }

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
 int addTransfer(BoundWitness* boundWitness){

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
 int addIncomingKeys(XYObject* incomingKeySets){

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
 int addIncomingPayload(XYObject* incomingPayloads){

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
 int addIncomingSignatures(XYObject* incomingSignatures){

 }

#define ORIGINCHAIN_H
#endif
