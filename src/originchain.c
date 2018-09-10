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
XYResult* getOriginBlockByBlockHash(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  XYResult* read_return = self_OriginChainNavigator->Storage->read(originBlockHash);
  ByteArray* return_array = read_return->result;
  free(read_return);
  XYResult* lookup_result = lookup((char*)BoundWitness_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  Object_Creator* BoundWitness_creator = lookup_result->result;
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
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                               [out]      success   Returns boolean 0 if remove succeeded.
*----------------------------------------------------------------------------*/
XYResult* removeOriginBlock(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  return self_OriginChainNavigator->Storage->delete(originBlockHash);
}

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
    XYResult* read_return = self_OriginChainNavigator->Storage->read(formattedHash);
    free(formattedHash);
    free(payload);
    if(read_return->error != OK){
      return read_return;
    }
    ByteArray* parentHash = read_return->result;
    free(read_return);
    return getOriginBlockByBlockHash(self_OriginChainNavigator, parentHash);
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

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
  Object_Creator* BoundWitness_creator = lookup_result->result;
  XYResult* toBytes_result = BoundWitness_creator->toBytes((XYObject*)user_BoundWitness);
  char* boundWitnessBytes = toBytes_result->result;
  free(lookup_result);
  free(toBytes_result);
  XYResult* blockHash_result = user_BoundWitness->getHash(user_BoundWitness, self_OriginChainNavigator->Hash);
  ByteArray* blockHashValue = blockHash_result->result;
  XYResult* previousBlock_result = self_OriginChainNavigator->findPreviousBlocks(self_OriginChainNavigator, user_BoundWitness);
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
    self_OriginChainNavigator->Storage->write(blockHashValue, write_ByteArray);
    write_ByteArray->size = prevBlockHashValue->size + 1*sizeof(char);
    write_ByteArray->payload = payload;
    self_OriginChainNavigator->Storage->write(write_ByteArray, blockHashValue);
    free(write_ByteArray);
    free(payload);
    free(write_ByteArray);
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
*      findPreviousBlocks
*
*  DESCRIPTION
*      Will returns a block that
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   Returns XYObject of Boound Witness type
*----------------------------------------------------------------------------*/
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
              Object_Creator* previousHash_creator = lookup_result->result;
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
