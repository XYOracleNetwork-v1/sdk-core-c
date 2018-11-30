/**
 ****************************************************************************************
 *
 * @file zigzagboundwitnessession.c
 *
 * @XYO Core library source code.
 *
 * @brief primary zig zag bound witness session routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef ZIGZAGBOUNDWITNESSSESSION_H

#include <stdlib.h>
#include "xyo.h"
//#include "BoundWitness.h"
#include "network.h"
#include "ZigZagBoundWitnessSession.h"
//#include "XYOHeuristicsBuilder.h"

/*----------------------------------------------------------------------------*
*   NAME
*     completeBoundWitness
*
*   DESCRIPTION
*     Adds data to the bound witness and returns what the party should send back.
*
*   PARAMETERS
*     userSession         [in]      ZigZagBoundWitnessSession_t*    The data from the other party.
*     boundWitnessData    [in]      ByteArray_t*    If not already turned around,
*                                                   decide if what to send sign and
*                                                   send back.
*   RETURNS
*     XYResult_t         [out]     XYResult_t*     Returns XYResult<ByteArray_t*> the
*                                                   data to send to the other party.
*   NOTES
*
*----------------------------------------------------------------------------*/
XYResult_t* completeBoundWitness(ZigZagBoundWitnessSession_t* userSession,
                                 ByteArray_t* boundWitnessData){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!userSession || !boundWitnessData) {RETURN_ERROR(ERR_BADDATA);}

  // Here we infer if the userSession bound witness is already completed or not.
  /*
  if(userSession->boundWitness->dynamicPublicKeys->size == userSession->boundWitness->dynamicSignatures->size &&
     userSession->boundWitness->dynamicPublicKeys->size != 0){

    RETURN_ERROR(ERR_INTERNAL_ERROR);
  }
  */

  if(boundWitnessData->payload == NULL){
    RETURN_ERROR(ERR_PEER_INCOMPATABLE);
  }

  //printf("completeBoundWitness>> user cycles %d\n", userSession->cycles);

  BoundWitnessTransfer_t* boundWitness = NULL;

  if(boundWitnessData != NULL){
      XYResult_t* lookup_result = tableLookup((char*)&BoundWitnessTransfer_id);
      if(lookup_result->error != OK) return lookup_result;
      ObjectProvider_t* BoundWitnessTransfer_creator = lookup_result->result;
      //free(lookup_result);
      XYResult_t* fromBytes_result = BoundWitnessTransfer_creator->fromBytes(boundWitnessData->payload);
      if(fromBytes_result->error != OK) return fromBytes_result;
      boundWitness = fromBytes_result->result;
      free(fromBytes_result);
  }

  XYResult_t* incomingData_result = userSession->boundWitness->incomingData(userSession->boundWitness,
                                    boundWitness,
                                    userSession->cycles); //&& boundWitnessData != NULL);

 if(userSession->cycles>1){
    RETURN_ERROR(OK);
  }

  if(incomingData_result->error != OK){
    return incomingData_result;
  } else if(incomingData_result->error == OK && incomingData_result->result == (void*)1){
    RETURN_ERROR(OK);
  }

  ByteArray_t* returnData = malloc(sizeof(ByteArray_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(returnData == NULL) {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  returnData->size = to_uint32((unsigned char*)incomingData_result->result);

  if(littleEndian()){
    returnData->size = to_uint32((unsigned char*)&returnData->size);
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

      memcpy(returnData->payload, &userSession->choice, sizeof(char)*(CATALOG_SIZE + 5));
      memcpy(returnData->payload, incomingData_result->result, returnData->size);
      returnData->size += CATALOG_SIZE+5;

      return userSession->NetworkPipe->send(userSession, returnData, receiverCallback);

    } else {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)}
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
*     receiverCallback
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     *self         [in]      ZigZagBoundWitnessSession_t*  The data from the other party.
*     *data         [in]      ByteArray_t*                    If not already turned around, decide
*                                                           if what to send sign and send back.
*
*  RETURNS
*      XYResult_t   [out]     XYResult_t*                   Returns XYResult<ByteArray_t*> the
*                                                           data to send to the other party.
*----------------------------------------------------------------------------*/
XYResult_t* receiverCallback(void* self, ByteArray_t* data){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self || !data) {RETURN_ERROR(ERR_BADDATA);}

  if(data->size == 0 ) return 0;
  
  ZigZagBoundWitnessSession_t* selfBoundWitness = (ZigZagBoundWitnessSession_t*)self;

  if( (selfBoundWitness->cycles == 0) && data->size == 0 ){

    XYResult_t* lookup_result = tableLookup((char*)&BoundWitnessTransfer_id);

    if(lookup_result->error != OK) return lookup_result;

    ObjectProvider_t* BWT_creator = lookup_result->result;
    XYResult_t* transfer_result = BWT_creator->fromBytes(data->payload);

    if(transfer_result->error != OK) return transfer_result;
    BoundWitnessTransfer_t* transfer = transfer_result->result;
    selfBoundWitness->boundWitness->incomingData(selfBoundWitness->boundWitness, transfer, 1);

    RETURN_ERROR(OK);
  } else {
    selfBoundWitness->cycles++;

    return selfBoundWitness->completeBoundWitness(self, data);

  }
}

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
