#include "originchain.h"

XYResult* getOriginBlockByBlockHash(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  XYResult* read_return = self_OriginChainNavigator->Storage->read(originBlockHash);
  ByteArray* return_array = read_return->result;
  free(read_return);
  XYResult* lookup_result = lookup(BoundWitness_id);
  if(lookup_result->error != OK){
    return lookup_result;
  }
  Object_Creator* BoundWitness_creator = lookup_result->result;
  free(lookup_result);
  return BoundWitness_creator->fromBytes(return_array->payload);
}

XYResult* removeOriginBlock(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  return self_OriginChainNavigator->Storage->delete(originBlockHash);
}

XYResult* getOriginBlockByPreviousHash(OriginChainNavigator* self_OriginChainNavigator, ByteArray* originBlockHash) {
  ByteArray* formattedHash = malloc(sizeof(ByteArray));
  char* payload = malloc(originBlockHash->size + 1*sizeof(char));
  if(formattedHash){
    memset(payload, 0xff, sizeof(char));
    memcpy(payload+sizeof(char), originBlockHash->payload, originBlockHash->size+(1*sizeof(char)));
    formattedHash->size = originBlockHash->size + 1;
    formattedHash->payload = payload;
    char nullBits[2] = { 0, 0 };
    memcpy(formattedHash->reserved, nullBits, sizeof(nullBits));
    XYResult* read_return = self_OriginChainNavigator->Storage->read(formattedHash);
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

/*
XYResult* addBoundWitness(OriginChainNavigator* self_OriginChainNavigator, BoundWitness* user_BoundWitness) {
  return
}
*/
