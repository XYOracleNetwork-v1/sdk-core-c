/**
 ****************************************************************************************
 *
 * @file originchain.c
 *
 * @XYO Core library source code.
 *
 * @brief primary origin chain routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
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
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      XYObject_t*   Returns XYObject
*                                          of Boound Witness type
*----------------------------------------------------------------------------*/
/*
XYResult_t* getOriginBlockByBlockHash(OriginChainNavigator* self_OriginChainNavigator,
                                      ByteArray_t* originBlockHash) {

  /********************************/
  /* guard against bad input data */
  /********************************//*

  if(!self_OriginChainNavigator || !originBlockHash) {RETURN_ERROR(ERR_BADDATA);}

  XYResult_t* read_return = self_OriginChainNavigator->Repository->read(originBlockHash);
  ByteArray_t* return_array = read_return->result;
  free(read_return);
  XYResult_t* lookup_result = tableLookup((char*)BoundWitness_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  ObjectProvider_t* BoundWitness_creator = lookup_result->result;
  //free(lookup_result);
  return BoundWitness_creator->fromBytes(return_array->payload);
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
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      success   Returns boolean 0 if remove
*                                                    succeeded.
*----------------------------------------------------------------------------*/
/*
XYResult_t* removeOriginBlock(OriginChainNavigator* self_OriginChainNavigator, ByteArray_t* originBlockHash) {

  /********************************/
  /* guard against bad input data */
  /********************************//*

  if(!self_OriginChainNavigator || !originBlockHash) {RETURN_ERROR(ERR_BADDATA);}

  return self_OriginChainNavigator->repository->delete(originBlockHash);
}
*/

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
*      XYResult_t*                [out]      XYObject_t*   Returns XYObject of Boound
*                                                      Witness type
*----------------------------------------------------------------------------*/
/*
XYResult_t* getOriginBlockByPreviousHash(OriginChainNavigator* self_OriginChainNavigator,
                                         ByteArray_t* originBlockHash) {

  /********************************/
  /* guard against bad input data */
  /********************************//*

  if(!self_OriginChainNavigator || !originBlockHash) {RETURN_ERROR(ERR_BADDATA);}

  ByteArray_t* formattedHash = malloc(sizeof(ByteArray_t));
  char* payload = malloc(originBlockHash->size + 1*sizeof(char));
  if(formattedHash && payload){
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), originBlockHash->payload, originBlockHash->size+(1*sizeof(char)));
    formattedHash->size = originBlockHash->size + 1;
    formattedHash->payload = payload;
    char nullBits[2] = { 0, 0 };
    memcpy(formattedHash->reserved, nullBits, sizeof(nullBits));
    XYResult_t* read_return = self_OriginChainNavigator->Repository>read(formattedHash);
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
*/

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
*       XYResult_t*               [out]      XYObject_t*  Returns XYObject of
*                                                       Boound Witness type
*----------------------------------------------------------------------------*/
XYResult_t* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator,
                            BoundWitness_t* user_BoundWitness) {

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!self_OriginChainNavigator || !user_BoundWitness) {RETURN_ERROR(ERR_BADDATA);}

  XYResult_t* lookup_result = tableLookup((char*)BoundWitness_id);
  ObjectProvider_t* BoundWitness_creator = lookup_result->result;
  XYResult_t* boundWitness_result = newObject((const char*)&BoundWitness_id, user_BoundWitness);
  XYObject_t* boundWitness_object = boundWitness_result->result;
  XYResult_t* toBytes_result = BoundWitness_creator->toBytes((XYObject_t*)user_BoundWitness);
  char* boundWitnessBytes = toBytes_result->result;
  ////free(lookup_result);
  //free(toBytes_result);
  XYResult_t* blockHash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  XYObject_t* blockHashObject = blockHash_result->result;
  ByteArray_t* blockHashValue = blockHashObject->payload;
  //XYResult_t* previousBlock_result = getMostRecentOriginBlock(self_OriginChainNavigator);
  //BoundWitness_t* previousBlock = previousBlock_result->result;
  //XYResult_t* prevBlockHash_result = user_BoundWitness->getHash(previousBlock, self_OriginChainNavigator->Hash);
  //ByteArray_t* prevBlockHashValue = prevBlockHash_result->result;
  //char* payload = malloc(blockHashValue->size + 1*sizeof(char));

  ByteArray_t* write_ByteArray = malloc(sizeof(ByteArray_t));;

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(write_ByteArray){
    //memset(payload, 0xff, sizeof(char));
    //memcpy(payload+sizeof(char), prevBlockHashValue->payload, blockHashValue->size+(1*sizeof(char)));
    write_ByteArray->size = user_BoundWitness->size;
    write_ByteArray->payload = boundWitnessBytes;
    self_OriginChainNavigator->originChainRepository->append(self_OriginChainNavigator->originChainRepository, write_ByteArray, DEFAULT_TIMEOUT);
    self_OriginChainNavigator->originChainRepository->logicalEnd = self_OriginChainNavigator->originChainRepository->logicalEnd+1;
    //free(write_ByteArray);
    //free(payload);

    /*
     * Here we add the hash of the block to our bridgeQueue
     */
     if(self_OriginChainNavigator->bridgeQueue == NULL){ RETURN_ERROR(ERR_CRITICAL); }

     uint32_t queueLen = self_OriginChainNavigator->queueLen+1;
     if(queueLen == 1){
       //printf("Realloc size: %lu\n", (sizeof(ByteArray_t**)*(queueLen+1)));
       self_OriginChainNavigator->bridgeQueue = malloc(sizeof(ByteArray_t**)*(queueLen+1));
       self_OriginChainNavigator->bridgeQueue[0] = blockHashValue;
       self_OriginChainNavigator->bridgeQueue[1] = NULL;
     } else {
       //printf("Realloc size: %lu\n", (sizeof(ByteArray_t**)*(queueLen)));
       self_OriginChainNavigator->bridgeQueue  = realloc(self_OriginChainNavigator->bridgeQueue, (sizeof(ByteArray_t**)*(queueLen+1)));
       self_OriginChainNavigator->bridgeQueue[queueLen-1] = blockHashValue;
       self_OriginChainNavigator->bridgeQueue[queueLen] = NULL;
     }


     self_OriginChainNavigator->queueLen = queueLen;
      /*
      for(uint32_t count = 0; count < self_OriginChainNavigator->queueLen; count++){

        if(queue[count]->size == 0){
          *queue[count] = *blockHashValue;
        } else {
          continue;
        }

      }
    }
    */

    preallocated_return_result_ptr = &preallocated_return_result;

    preallocated_return_result_ptr->error = OK;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;

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
*      XYResult_t*              [out]      XYObject_t*  Returns XYObject of Boound
*                                                     Witness type
*----------------------------------------------------------------------------*/
/*
XYResult_t* findPreviousBlocks(OriginChainNavigator* self_OriginChainNavigator,
                               BoundWitness_t* user_BoundWitness){

  /********************************/
  /* guard against bad input data */
  /********************************//*

  if(!self_OriginChainNavigator || !user_BoundWitness) {RETURN_ERROR(ERR_BADDATA);}

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
              //free(lookup_result);
              ByteArray_t* blockHash = malloc(sizeof(ByteArray_t));

              /********************************/
              /* guard against malloc errors  */
              /********************************//*

              if(!blockHash) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

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
*     *BoundWitness                            [in]       user_BoundWitness_t*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
XYResult_t* containsOriginBlock(OriginChainNavigator* self_OriginChainNavigator, BoundWitness_t* user_BoundWitness){
  XYResult_t* hash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  if(hash_result->error != OK){
    RETURN_ERROR(ERR_CRITICAL);
  }
  /*
  if(self_OriginChainNavigator->bridgeQueue == NULL){
    self_OriginChainNavigator->bridgeQueue = malloc(sizeof(ByteArray_t**) * 2);
    self_OriginChainNavigator->bridgeQueue[1] = NULL;
    self_OriginChainNavigator->queueLen = 1;
  }
  */
  if(self_OriginChainNavigator->queueLen == 0){
    RETURN_ERROR(ERR_KEY_DOES_NOT_EXIST);
  }
  for(size_t i = 0; i<self_OriginChainNavigator->queueLen-1; i++){
    if(self_OriginChainNavigator->bridgeQueue[i] != NULL && self_OriginChainNavigator->bridgeQueue[i]->size != (uint8_t)0){
      XYObject_t* hash = hash_result->result;
      if(strncmp( ((ByteArray_t*)hash->payload)->payload, self_OriginChainNavigator->bridgeQueue[i]->payload, 32) == 0 ){
        /* strcmp confirms the hashes match. */
        XYResult_t* return_result = malloc(sizeof(XYResult_t));
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
*     *BoundWitness                            [in]       user_BoundWitness_t*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
/*
OriginChainNavigator* initOriginChainProvider( OriginChainNavigator** self, char* bits){
  (*self) = malloc(sizeof(OriginChainNavigator));
  if(*self){
    (*self)->originChainRepository = malloc(sizeof(OriginChainProvider_t));

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

// end of file originchain.h
