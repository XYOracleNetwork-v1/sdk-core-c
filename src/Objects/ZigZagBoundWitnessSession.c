/**
 ****************************************************************************************
 *
 * @file zigzagboundwitnessession.c
 *
 * @XY4 project source code.
 *
 * @brief primary zig zag bound witness session routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef ZIGZAGBOUNDWITNESSSESSION_H

#include <stdlib.h>
#include "xyo.h"
#include "boundwitness.h"
#include "ZigZagBoundWitnessSession.h"
#include "XYOHeuristicsBuilder.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* receiverCallback(ZigZagBoundWitnessSession_t* self, ByteArray_t* data);

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
*     XYResult_t         [out]     XYResult_t*     Returns XYResult<ByteArray*> the 
*                                                   data to send to the other party.
*   NOTES
*   
*----------------------------------------------------------------------------*/
XYResult_t* completeBoundWitness(ZigZagBoundWitnessSession_t* userSession, 
                                 ByteArray_t* boundWitnessData){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!userSession || !boundWitnessData) {RETURN_ERROR(ERR_BADDATA)};

  // Here we infer if the userSession bound witness is already completed or not.
  
  if(userSession->BoundWitness->dynamicPublicKeys->size == userSession->BoundWitness->dynamicSignatures->size && 
     userSession->BoundWitness->dynamicPublicKeys->size != 0){
       
    RETURN_ERROR(ERR_INTERNAL_ERROR);
  }

  BoundWitnessTransfer* boundWitness = NULL;
  
  if(boundWitnessData != NULL){
      XYResult_t* lookup_result = tableLookup((char*)&BoundWitnessTransfer_id);
      if(lookup_result->error != OK) return lookup_result;
      ObjectProvider_t* BoundWitnessTransfer_creator = lookup_result->result;
      free(lookup_result);
      XYResult_t* fromBytes_result = BoundWitnessTransfer_creator->fromBytes(boundWitnessData->payload);
      if(fromBytes_result->error != OK) return fromBytes_result;
      boundWitness = fromBytes_result->result;
      free(fromBytes_result);
  }
  
  XYResult_t* incomingData_result = userSession->BoundWitness->incomingData(userSession->BoundWitness, 
                                    boundWitness, 
                                    userSession->cycles && boundWitnessData != NULL);
  
  if(incomingData_result->error != OK){
    return incomingData_result;
  }
  
  ByteArray_t* returnData = malloc(sizeof(ByteArray_t));
  
  /********************************/
  /* guard against malloc errors  */
  /********************************/
    
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
      
    } else {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)}
  } else {
    return userSession->NetworkPipe->send(userSession, returnData, receiverCallback);
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
*     *data         [in]      ByteArray*                    If not already turned around, decide 
*                                                           if what to send sign and send back.
*
*  RETURNS
*      XYResult_t   [out]     XYResult_t*                   Returns XYResult<ByteArray*> the 
*                                                           data to send to the other party.
*----------------------------------------------------------------------------*/
XYResult_t* receiverCallback(ZigZagBoundWitnessSession_t* self, ByteArray_t* data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!self || !data) {RETURN_ERROR(ERR_BADDATA)};

  if(data->size == 0 ) return 0;
  
  if(self->cycles == 0){
    
    XYResult_t* lookup_result = tableLookup((char*)&BoundWitnessTransfer_id);
    
    if(lookup_result->error != OK) return lookup_result;
    
    ObjectProvider_t* BWT_creator = lookup_result->result;
    XYResult_t* transfer_result = BWT_creator->fromBytes(data->payload);
    
    if(transfer_result->error != OK) return transfer_result;
    
    BoundWitnessTransfer* transfer = transfer_result->result;
    self->BoundWitness->incomingData(self->BoundWitness, transfer, 1);
    
    return 0;   //TODO: wal, booo, hisss.  this method has a XYResult_t* type
                // returning a 0 is too crude. please return a proper XYResult_t*
  } else {
    
    self->cycles++;
    
    return self->completeBoundWitness(self, data);
  }
}

#endif

// end of file zigzagboundwitnessession.c

