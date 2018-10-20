/**
 ****************************************************************************************
 *
 * @file ZigZagBoundWitnessSession.c
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

#include "ZigZagBoundWitnessSession.h"

/*----------------------------------------------------------------------------*
*  NAME
*      completeBoundWitness
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
  if(userSession->boundWitness->dynamicPublicKeys->size == userSession->boundWitness->dynamicSignatures->size && userSession->boundWitness->dynamicPublicKeys->size != 4){
      RETURN_ERROR(ERR_INTERNAL_ERROR);
  }

  BoundWitnessTransfer* boundWitness = NULL;
  if(boundWitnessData != NULL){
      XYResult* lookup_result = lookup((char*)&BoundWitnessTransfer_id);
      if(lookup_result->error != OK) return lookup_result;
      ObjectProvider* BoundWitnessTransfer_creator = lookup_result->result;
      //free(lookup_result);
      XYResult* fromBytes_result = BoundWitnessTransfer_creator->fromBytes(boundWitnessData->payload);
      if(fromBytes_result->error != OK) return fromBytes_result;
      boundWitness = fromBytes_result->result;
      //free(fromBytes_result);
  }

  XYResult* incomingData_result = userSession->boundWitness->incomingData(userSession->boundWitness, boundWitness, boundWitnessData != NULL);

  if(incomingData_result->error != OK){
    return incomingData_result;
  } else if(incomingData_result->error == OK && incomingData_result->result == (void*)1){
    RETURN_ERROR(OK);
  }
  ByteArray* returnData = malloc(sizeof(ByteArray));
  if(returnData == NULL) {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  returnData->size = to_uint32((unsigned char*)incomingData_result->result);
  /*
  if(littleEndian()){
    returnData->size = to_uint32((char*)&returnData->size);
  }
  */
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
      memcpy(returnData->payload, &userSession->choice, sizeof(char)*(CATALOG_SIZE + 5));
      memcpy(returnData->payload, incomingData_result->result, returnData->size);
      returnData->size += CATALOG_SIZE+5;
      userSession->cycles += 1;
      return userSession->NetworkPipe->send((void*)userSession, returnData, receiverCallback);
    } else { RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  } else {
    returnData->payload = malloc(sizeof(char) * returnData->size);
    if(returnData->payload){
      char* dat = ((char*)incomingData_result->result);
      uint32_t boundWitnessSize = to_uint32((unsigned char*)dat);
      memcpy(returnData->payload, incomingData_result->result, boundWitnessSize);
      userSession->cycles += 1;
      return userSession->NetworkPipe->send((void*)userSession, returnData, receiverCallback);
    } else { RETURN_ERROR(ERR_INSUFFICIENT_MEMORY); }
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      receiverCallback
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     *self          [in]      ZigZagBoundWitnessSession*    The data from the other party.
*     *data          [in]      ByteArray*       If not already turned around, decide if what to send sign and send back.
*
*  RETURNS
*      XYResult  [out]      bool       Returns XYResult<ByteArray*> the data to send to the other party.
*----------------------------------------------------------------------------*/
XYResult* receiverCallback(void* self, ByteArray* data){
  if(data->size == 0 ) { free(data); RETURN_ERROR(OK); };
  if(((ZigZagBoundWitnessSession*)self)->cycles == 0){
    XYResult* lookup_result = lookup((char*)&BoundWitnessTransfer_id);
    if(lookup_result->error != OK) return lookup_result;
    ObjectProvider* BWT_creator = lookup_result->result;
    XYResult* transfer_result = BWT_creator->fromBytes(data->payload);
    if(transfer_result->error != OK) return transfer_result;
    BoundWitnessTransfer* transfer = transfer_result->result;
    ((ZigZagBoundWitnessSession*)self)->boundWitness->incomingData(((ZigZagBoundWitnessSession*)self)->boundWitness, transfer, 1);
    RETURN_ERROR(OK);
  } else {
    ((ZigZagBoundWitnessSession*)self)->cycles++;
    return ((ZigZagBoundWitnessSession*)self)->completeBoundWitness(self, data);

  }
}
