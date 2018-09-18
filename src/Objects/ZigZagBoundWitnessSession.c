#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include "ZigZagBoundWitnessSession.h"

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
XYResult* completeBoundWitness(ZigZagBoundWitnessSession* userSession, ByteArray* boundWitnessData){
  // Here we infer if the userSession bound witness is already completed or not.
  if(userSession->BoundWitness->dynamicPublicKeys->size == userSession->BoundWitness->dynamicSignatures->size && userSession->BoundWitness->dynamicPublicKeys->size != 0){
    RETURN_ERROR(ERR_INTERNAL_ERROR);
  }

  BoundWitnessTransfer* boundWitness = NULL;
  if(boundWitnessData != NULL){
      XYResult* lookup_result = lookup((char*)&BoundWitnessTransfer_id);
      if(lookup_result->error != OK) return lookup_result;
      ObjectProvider* BoundWitnessTransfer_creator = lookup_result->result;
      free(lookup_result);
      XYResult* fromBytes_result = BoundWitnessTransfer_creator->fromBytes(boundWitnessData->payload);
      if(fromBytes_result->error != OK) return fromBytes_result;
      boundWitness = fromBytes_result->result;
      free(fromBytes_result);
  }
  XYResult* incomingData_result = userSession->BoundWitness->incomingData(userSession->BoundWitness, boundWitness, userSession->cycles && boundWitnessData != NULL);
  if(incomingData_result->error != OK){
    return incomingData_result;
  }
  ByteArray* returnData = malloc(sizeof(ByteArray));
  if(returnData == NULL) {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  returnData->size = to_uint32((char*)&incomingData_result->result);
  if(littleEndian()){
    returnData->size = to_uint32((char*)&returnData->size);
  }

  if(userSession->cycles && boundWitnessData == NULL){
    /*
     * CATALOG_SIZE + 5 because the first bytes of a packet should be fixed length 5.
     * 4 bytes for size 1 byte for size of the catalog and then the actual catalog.
     * CATALOG_SIZE is measured in bytes. Catalog size 1 means the catalog is 8 bits long.
     * The first 4 bytes of a packet in our protocol should always be size of the whole
     * message in bytes in big endian. choice->size should return the same as CATALOG_SIZE+5
     * but this is explicit.
     */
    returnData->payload = malloc(sizeof(char)*(CATALOG_SIZE + 5) + returnData->size);
    if(returnData->payload){
      memcpy(returnData->payload, userSession->choice->payload, sizeof(char)*(CATALOG_SIZE + 5));
      memcpy(returnData->payload, incomingData_result->result, returnData->size);
      returnData->size += CATALOG_SIZE+5;
      return userSession->NetworkPipe->send(userSession, returnData, receiverCallback);
    } else { RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  } else {
    return userSession->NetworkPipe->send(userSession, returnData, receiverCallback);
  }
}

XYResult* receiverCallback(ZigZagBoundWitnessSession* self, ByteArray* data){
  if(data->size == 0 ) return 0;
  if(self->cycles == 0){
    XYResult* lookup_result = lookup((char*)&BoundWitnessTransfer_id);
    if(lookup_result->error != OK) return lookup_result;
    ObjectProvider* BWT_creator = lookup_result->result;
    XYResult* transfer_result = BWT_creator->fromBytes(data->payload);
    if(transfer_result->error != OK) return transfer_result;
    BoundWitnessTransfer* transfer = transfer_result->result;
    self->BoundWitness->incomingData(self->BoundWitness, transfer, 1);
    return 0;
  } else {
    self->cycles++;
    return self->completeBoundWitness(self, data);

  }
}

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
