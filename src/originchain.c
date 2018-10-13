/**
 ****************************************************************************************
 *
 * @file originchain.c
 *
 * @XY4 project source code.
 *
 * @brief primary origin chain routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "originchain.h"

/*----------------------------------------------------------------------------*
*  NAME
*      getOriginBlockByBlockHash
*
*  DESCRIPTION
*      Retrieves a Origin Block object given a block hash.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                             [out]      XYObject*   Returns XYObject 
*                                                         of Boound Witness type
*----------------------------------------------------------------------------*/
XYResult_t* getOriginBlockByBlockHash(OriginChainNavigator* self_OriginChainNavigator, 
                                    ByteArray_t* originBlockHash) {
  
  XYResult_t* read_return = self_OriginChainNavigator->Storage->read(originBlockHash);
  ByteArray_t* return_array = read_return->result;
  free(read_return);
  XYResult_t* lookup_result = tableLookup((char*)BoundWitness_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider_t* BoundWitness_creator = lookup_result->result;
  free(lookup_result);
  return BoundWitness_creator->fromBytes(return_array->payload);
}

/*----------------------------------------------------------------------------*
*  NAME
*      removeOriginBlock
*
*  DESCRIPTION
*      Retrieves a Origin Block object given a block hash.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      success   Returns boolean 0 if remove 
*                                                    succeeded.
*----------------------------------------------------------------------------*/
XYResult_t* removeOriginBlock(OriginChainNavigator* self_OriginChainNavigator, ByteArray_t* originBlockHash) {
  return self_OriginChainNavigator->Storage->delete(originBlockHash);
}

/*----------------------------------------------------------------------------*
*  NAME
*      getOriginBlockByPreviousHash
*
*  DESCRIPTION
*      Retrieves a Origin Block that has the given hash in one of it's Payloads 
*      as the previous hash.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                [out]      XYObject*   Returns XYObject of Boound 
*                                                      Witness type
*----------------------------------------------------------------------------*/
XYResult_t* getOriginBlockByPreviousHash(OriginChainNavigator* self_OriginChainNavigator, 
                                         ByteArray_t* originBlockHash) {
  
  ByteArray_t* formattedHash = malloc(sizeof(ByteArray_t));
  char* payload = malloc(originBlockHash->size + 1*sizeof(char));
  
  if(formattedHash && payload){
    
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), originBlockHash->payload, originBlockHash->size+(1*sizeof(char)));
    formattedHash->size = originBlockHash->size + 1;
    formattedHash->payload = payload;
    char nullBits[2] = { 0, 0 };
    memcpy(formattedHash->reserved, nullBits, sizeof(nullBits));
    XYResult_t* read_return = self_OriginChainNavigator->Storage->read(formattedHash);
    free(formattedHash);
    free(payload);
    
    if(read_return->error != OK){
      return read_return;
    }
    
    ByteArray_t* parentHash = read_return->result;
    free(read_return);
    
    return getOriginBlockByBlockHash(self_OriginChainNavigator, parentHash);
    
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*       addBoundWitness
*
*  DESCRIPTION
*       Will add a bound witness to the origin chain and update the OriginChainNavigator's 
*       currentHash.
*
*  PARAMETERS
*       *OriginChainNavigator     [in]       self_OriginChainNavigator*
*       *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*       XYResult_t*               [out]      XYObject*  Returns XYObject of 
*                                                       Boound Witness type
*----------------------------------------------------------------------------*/
XYResult_t* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, 
                            BoundWitness* user_BoundWitness) {
  
  XYResult_t* lookup_result = tableLookup((char*)BoundWitness_id);
  ObjectProvider_t* BoundWitness_creator = lookup_result->result;
  XYResult_t* toBytes_result = BoundWitness_creator->toBytes((XYObject_t*)user_BoundWitness);
  char* boundWitnessBytes = toBytes_result->result;
  free(lookup_result);
  free(toBytes_result);
  XYResult_t* blockHash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  ByteArray_t* blockHashValue = blockHash_result->result;
  XYResult_t* previousBlock_result = self_OriginChainNavigator->findPreviousBlocks(self_OriginChainNavigator, user_BoundWitness);
  
  if(previousBlock_result->error != OK){
    return previousBlock_result;
  }
  
  BoundWitness* previousBlock = previousBlock_result->result;
  XYResult_t* prevBlockHash_result = user_BoundWitness->getHash(previousBlock, self_OriginChainNavigator->Hash);
  ByteArray_t* prevBlockHashValue = prevBlockHash_result->result;
  char* payload = malloc(blockHashValue->size + 1*sizeof(char));
  
  ByteArray_t* write_ByteArray = malloc(sizeof(ByteArray_t));;
  
  if(write_ByteArray && payload){
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), prevBlockHashValue->payload, blockHashValue->size+(1*sizeof(char)));
    write_ByteArray->size = user_BoundWitness->size;
    write_ByteArray->payload = boundWitnessBytes;
    self_OriginChainNavigator->Storage->write(blockHashValue, write_ByteArray);
    write_ByteArray->size = prevBlockHashValue->size + 1*sizeof(char);
    write_ByteArray->payload = payload;
    self_OriginChainNavigator->Storage->write(write_ByteArray, blockHashValue);
    free(write_ByteArray);
    free(payload);
    XYResult_t* return_result = malloc(sizeof(XYResult_t));
    
    if(return_result){
      return_result->error = OK;
      return_result->result = 0;
      
      return return_result;
    } else {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      findPreviousBlocks
*
*  DESCRIPTION
*      Will returns a block that
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      XYObject*  Returns XYObject of Boound 
*                                                     Witness type
*----------------------------------------------------------------------------*/
XYResult_t* findPreviousBlocks(OriginChainNavigator* self_OriginChainNavigator, 
                               BoundWitness* user_BoundWitness){
  
  if(user_BoundWitness->payloads){
    
    IntStrongArray_t* user_payloads = user_BoundWitness->payloads;
    int stop = 0;
    for(int i = 0; stop == 0;i++){
      XYResult_t* get_result = user_payloads->get(user_payloads, i);
      if(get_result->error==ERR_KEY_DOES_NOT_EXIST){
       stop = 1;
     } else {
        Payload_t* potential_payload = get_result->result;
        int breakout = 0;
        for(int j = 0; breakout==0; ){
          XYResult_t* payloadGet_result = potential_payload->signedHeuristics->get(potential_payload->signedHeuristics, j);
          if(payloadGet_result->error!=ERR_KEY_DOES_NOT_EXIST){
            XYObject_t* payload_obj = payloadGet_result->result;
            if(payload_obj->id[0] == 0x02 && payload_obj->id[1] == 0x06){
              PreviousHash_t* previous_hash = payload_obj->payload;
              XYResult_t* lookup_result = tableLookup((char*)previous_hash->hash);
              if(lookup_result->error != OK){
                continue;
              }
              ObjectProvider_t* previousHash_creator = lookup_result->result;
              free(lookup_result);
              ByteArray_t* blockHash = malloc(sizeof(ByteArray_t));
              //TODO: wal, should check for any malloc errors
              blockHash->payload = previous_hash->hash;
              blockHash->size = previousHash_creator->defaultSize-2;
              XYResult_t* foundParent = self_OriginChainNavigator->getOriginBlockByBlockHash(self_OriginChainNavigator, blockHash);
              free(blockHash);
              if(foundParent->error!=OK){
                free(foundParent);
                continue;
              } else {
                return foundParent;
              }
            } else {
              continue;
            }
          } else {
            breakout = 1;
          }
        }
      }
    }
    RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST)
  } else {
    RETURN_ERROR(ERR_BADDATA);
  }
}

// end of file originchain.h

