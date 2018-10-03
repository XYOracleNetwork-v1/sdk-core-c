/**
 ****************************************************************************************
 *
 * @file crypto.c
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
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
/*
XYResult* getOriginBlockByBlockHash(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  XYResult* read_return = self_OriginChainNavigator->Repository->read(originBlockHash);

  ByteArray* return_array = read_return->result;
  free(read_return);
  XYResult* lookup_result = lookup((char*)BoundWitness_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider* BoundWitness_creator = lookup_result->result;
  free(lookup_result);
  return BoundWitness_creator->fromBytes(return_array->payload);

  //TODO Implement getOriginBlockByBlockHash;
}
*/
/*----------------------------------------------------------------------------*
*  NAME
*      removeOriginBlock
*
*  DESCRIPTION
*      Retrieves a Origin Block object given a block hash.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                               [out]      success   Returns boolean 0 if remove succeeded.
*----------------------------------------------------------------------------*/
/*
XYResult* removeOriginBlock(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  return self_OriginChainNavigator->repository->delete(self_OriginChainNavigator, originBlockHash);
}
TODO: Implement removeOriginBlock
*/

/*----------------------------------------------------------------------------*
*  NAME
*      getOriginBlockByPreviousHash
*
*  DESCRIPTION
*      Retrieves a Origin Block that has the given hash in one of it's Payloads as the previous hash.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
/*
XYResult* getOriginBlockByPreviousHash(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  ByteArray* formattedHash = malloc(sizeof(ByteArray));
  char* payload = malloc(originBlockHash->size + 1*sizeof(char));
  if(formattedHash && payload){
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), originBlockHash->payload, originBlockHash->size+(1*sizeof(char)));
    formattedHash->size = originBlockHash->size + 1;
    formattedHash->payload = payload;
    char nullBits[2] = { 0, 0 };
    memcpy(formattedHash->reserved, nullBits, sizeof(nullBits));

    XYResult* read_return = self_OriginChainNavigator->Repository->read(formattedHash);
    free(formattedHash);
    free(payload);
    if(read_return->error != OK){
      return read_return;
    }
    ByteArray* parentHash = read_return->result;
    free(read_return);
    return getOriginBlockByBlockHash(self_OriginChainNavigator, parentHash);
    return preallocated_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
TODO: Implement getOriginBlockByPreviousHash
*/

/*----------------------------------------------------------------------------*
*  NAME
*      addBoundWitness
*
*  DESCRIPTION
*      Will add a bound witness to the origin chain and update the OriginChainNavigator's currentHash.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
XYResult* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness) {
  XYResult* lookup_result = lookup((char*)BoundWitness_id);
  ObjectProvider* BoundWitness_creator = lookup_result->result;
  XYResult* toBytes_result = BoundWitness_creator->toBytes((XYObject*)user_BoundWitness);
  char* boundWitnessBytes = toBytes_result->result;
  free(lookup_result);
  free(toBytes_result);
  XYResult* blockHash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  ByteArray* blockHashValue = blockHash_result->result;

  XYResult* previousBlock_result = getMostRecentOriginBlock(self_OriginChainNavigator);

  if(previousBlock_result->error != OK){
    return previousBlock_result;
  }
  BoundWitness* previousBlock = previousBlock_result->result;
  XYResult* prevBlockHash_result = user_BoundWitness->getHash(previousBlock, self_OriginChainNavigator->Hash);
  ByteArray* prevBlockHashValue = prevBlockHash_result->result;
  char* payload = malloc(blockHashValue->size + 1*sizeof(char));
  ByteArray* write_ByteArray = malloc(sizeof(ByteArray));;
  if(write_ByteArray && payload){
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), prevBlockHashValue->payload, blockHashValue->size+(1*sizeof(char)));
    write_ByteArray->size = user_BoundWitness->size;
    write_ByteArray->payload = boundWitnessBytes;
    self_OriginChainNavigator->originChainRepository->append(self_OriginChainNavigator->originChainRepository, write_ByteArray, DEFAULT_TIMEOUT);
    self_OriginChainNavigator->originChainRepository->logicalEnd = self_OriginChainNavigator->originChainRepository->logicalEnd+1;
    free(write_ByteArray);
    free(payload);

    /*
     * Here we add the hash of the block to our bridgeQueue
     */
    ByteArray* queue = self_OriginChainNavigator->bridgeQueue;
    if(queue == NULL) { RETURN_ERROR(ERR_CRITICAL);}

    if(queue[0].size == (uint32_t)0){
      queue[0] = *blockHashValue;
    } else {

      for(uint32_t count = 0; count < MAX_QUEUE; count++){

        if(queue[count].size == 0){
          queue[count] = *blockHashValue;
        } else {
          continue;
        }

      }
    }

    XYResult* return_result = malloc(sizeof(XYResult));
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
*      getMostRecentOriginBlock
*
*  DESCRIPTION
*     Will return the most recently appended block to the origin chain.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
XYResult* getMostRecentOriginBlock(OriginChainNavigator* self_OriginChainNavigator) {
  return ((IntStrongArray*)self_OriginChainNavigator->originChainRepository->repository)->get(self_OriginChainNavigator->originChainRepository->repository, self_OriginChainNavigator->originChainRepository->logicalEnd-1);
}

/*----------------------------------------------------------------------------*
*  NAME
*      findPreviousBlocks
*
*  DESCRIPTION
*      Will returns a parent block to a given bound witness
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
/*
XYResult* findPreviousBlocks(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness){
  if(user_BoundWitness->payloads){
    IntStrongArray* user_payloads = user_BoundWitness->payloads;
    int stop = 0;
    for(int i = 0; stop == 0;i++){
      XYResult* get_result = user_payloads->get(user_payloads, i);
      if(get_result->error==ERR_KEY_DOES_NOT_EXIST){
       stop = 1;
     } else {
        Payload* potential_payload = get_result->result;
        int breakout = 0;
        for(int j = 0; breakout==0; ){
          XYResult* payloadGet_result = potential_payload->signedHeuristics->get(potential_payload->signedHeuristics, j);
          if(payloadGet_result->error!=ERR_KEY_DOES_NOT_EXIST){
            XYObject* payload_obj = payloadGet_result->result;
            if(payload_obj->id[0] == 0x02 && payload_obj->id[1] == 0x06){
              PreviousHash* previous_hash = payload_obj->payload;
              XYResult* lookup_result = lookup((char*)previous_hash->hash);
              if(lookup_result->error != OK){
                continue;
              }
              ObjectProvider* previousHash_creator = lookup_result->result;
              free(lookup_result);
              ByteArray* blockHash = malloc(sizeof(ByteArray));
              blockHash->payload = previous_hash->hash;
              blockHash->size = previousHash_creator->defaultSize-2;
              XYResult* foundParent = self_OriginChainNavigator->getOriginBlockByBlockHash(self_OriginChainNavigator, blockHash);
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
*/

/*----------------------------------------------------------------------------*
*  NAME
*      containsOriginBlock
*
*  DESCRIPTION
*     Will return OK if a boundwitness exists on disk or in ram and ERR otherwise
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *BoundWitness                            [in]       user_BoundWitness*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
XYResult* containsOriginBlock(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness){
  XYResult* hash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  if(hash_result->error != OK){
    RETURN_ERROR(ERR_CRITICAL);
  }

  for(size_t i = 0; i<MAX_QUEUE; i++){
    if(self_OriginChainNavigator->bridgeQueue[i].size != (uint8_t)0){
      if(strcmp( ((ByteArray*)hash_result->result)->payload, self_OriginChainNavigator->bridgeQueue[i].payload) == 0 ){
        /* strcmp confirms the hashes match. */
        XYResult* return_result = malloc(sizeof(XYResult));
        if(return_result){
          return_result->error = OK;
          return_result->result = (void*)i;
          return return_result;
        } else {
          free(hash_result);
          RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
        }
      } else {
        continue;
      }
    } else {
      RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
    }
  }
  RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
}

/*----------------------------------------------------------------------------*
*  NAME
*      initOriginChainProvider
*
*  DESCRIPTION
*     Will return OK if a boundwitness exists on disk or in ram and ERR otherwise
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *BoundWitness                            [in]       user_BoundWitness*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
/*
OriginChainNavigator* initOriginChainProvider( OriginChainNavigator** self, char* bits){
  (*self) = malloc(sizeof(OriginChainNavigator));
  if(*self){
    (*self)->originChainRepository = malloc(sizeof(OriginChainProvider));

    if((*self)->originChainRepository == NULL) {
      free(*self);
      return NULL;
    }
  } else {
    return NULL;
  }
  return (*self);
}
*/
