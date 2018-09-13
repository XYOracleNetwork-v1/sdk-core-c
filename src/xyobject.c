#include "XYOHeuristicsBuilder.h"
#include <stdlib.h>
#include <string.h>

char* GetId(struct XYObject* object){
  return object->id;
}

void* GetPayload(struct XYObject* object){
  return object->payload;
}

XYResult* newObject(char id[2], void* payload){
  struct XYObject* new_object = malloc(sizeof(XYObject));
  if(new_object != NULL){
    if(payload != 0){
      new_object->payload = payload;
    }
    else{
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
    strncpy(new_object->id, id, 2);
    new_object->GetId = &GetId;
    new_object->GetPayload = &GetPayload;
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result){
      return_result->error = OK;
      return_result->result = new_object;
      return return_result;
    } else {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

}

XYResult* initTable(){
  int x,y;
  for(x = 0; x < 16; x ++) {
      for(y = 0; y < 16; y ++) typeTable[x][y] = 0;
  }

  // Initialize Byte Strong Array Creator
  struct Object_Creator* ByteStrongArray_creator = malloc(sizeof(Object_Creator));
  if(ByteStrongArray_creator != NULL){
    ByteStrongArray_creator->sizeIdentifierSize = 1;
    ByteStrongArray_creator->defaultSize = 0;
    ByteStrongArray_creator->create = &ByteStrongArray_creator_create;
    ByteStrongArray_creator->fromBytes = &ByteStrongArray_creator_fromBytes;
    ByteStrongArray_creator->toBytes = &ByteStrongArray_creator_toBytes;
    typeTable[0x01][0x01] = ByteStrongArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Short Strong Array Creator
  struct Object_Creator* ShortStrongArray_creator = malloc(sizeof(Object_Creator));
  if(ShortStrongArray_creator != NULL){
    ShortStrongArray_creator->sizeIdentifierSize = 2;
    ShortStrongArray_creator->defaultSize = 0;
    ShortStrongArray_creator->create = &ShortStrongArray_creator_create;
    ShortStrongArray_creator->fromBytes = &ShortStrongArray_creator_fromBytes;
    ShortStrongArray_creator->toBytes = &ShortStrongArray_creator_toBytes;
    typeTable[0x01][0x02] = ShortStrongArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Int Strong Array Creator
  struct Object_Creator* IntStrongArray_creator = malloc(sizeof(Object_Creator));
  if(IntStrongArray_creator != NULL){
    IntStrongArray_creator->sizeIdentifierSize = 4;
    IntStrongArray_creator->defaultSize = 0;
    IntStrongArray_creator->create = &IntStrongArray_creator_create;
    IntStrongArray_creator->fromBytes = &IntStrongArray_creator_fromBytes;
    IntStrongArray_creator->toBytes = &IntStrongArray_creator_toBytes;
    typeTable[0x01][0x03] = IntStrongArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Byte Weak Array Creator
  struct Object_Creator* ByteWeakArray_creator = malloc(sizeof(Object_Creator));
  if(ByteWeakArray_creator != NULL){
    ByteWeakArray_creator->sizeIdentifierSize = 1;
    ByteWeakArray_creator->defaultSize = 0;
    ByteWeakArray_creator->create = &ByteWeakArray_creator_create;
    ByteWeakArray_creator->fromBytes = &ByteWeakArray_creator_fromBytes;
    ByteWeakArray_creator->toBytes = &ByteWeakArray_creator_toBytes;
    typeTable[0x01][0x04] = ByteWeakArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Short Weak Array Creator
  struct Object_Creator* ShortWeakArray_creator = malloc(sizeof(Object_Creator));
  if(ShortWeakArray_creator != NULL){
    ShortWeakArray_creator->sizeIdentifierSize = 2;
    ShortWeakArray_creator->defaultSize = 0;
    ShortWeakArray_creator->create = &ShortWeakArray_creator_create;
    ShortWeakArray_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    ShortWeakArray_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[0x01][0x05] = ShortWeakArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Int Weak Array Creator
  struct Object_Creator* IntWeakArray_creator = malloc(sizeof(Object_Creator));
  if(IntWeakArray_creator != NULL){
    IntWeakArray_creator->sizeIdentifierSize = 4;
    IntWeakArray_creator->defaultSize = 0;
    IntWeakArray_creator->create = &IntWeakArray_creator_create;
    IntWeakArray_creator->fromBytes = &IntWeakArray_creator_fromBytes;
    IntWeakArray_creator->toBytes = &IntWeakArray_creator_toBytes;
    typeTable[0x01][0x06] = IntWeakArray_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Bound Witness Creator
  struct Object_Creator* BoundWitness_creator = malloc(sizeof(Object_Creator));
  if(BoundWitness_creator != NULL){
    BoundWitness_creator->sizeIdentifierSize = 4;
    BoundWitness_creator->defaultSize = 0;
    BoundWitness_creator->create = &BoundWitness_creator_create;
    BoundWitness_creator->fromBytes = &BoundWitness_creator_fromBytes;
    BoundWitness_creator->toBytes = &BoundWitness_creator_toBytes;
    typeTable[0x02][0x01] = BoundWitness_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Keyset
  struct Object_Creator* KeySet_creator = malloc(sizeof(Object_Creator));
  if(KeySet_creator != NULL){
    KeySet_creator->sizeIdentifierSize = 2;
    KeySet_creator->defaultSize = 0;
    KeySet_creator->create = &ShortWeakArray_creator_create;
    KeySet_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    KeySet_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[0x02][0x02] = KeySet_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Payload Creator
  struct Object_Creator* Payload_creator = malloc(sizeof(Object_Creator));
  if(Payload_creator != NULL){
    Payload_creator->sizeIdentifierSize = 4;
    Payload_creator->defaultSize = 0;
    Payload_creator->create = &Payload_creator_create;
    Payload_creator->fromBytes = &Payload_creator_fromBytes;
    Payload_creator->toBytes = &Payload_creator_toBytes;
    typeTable[0x02][0x04] = Payload_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Index Creator
  struct Object_Creator* Index_creator = malloc(sizeof(Object_Creator));
  if(Index_creator != NULL){
    Index_creator->sizeIdentifierSize = 0;
    Index_creator->defaultSize = 4;
    Index_creator->create = &Index_creator_create;
    Index_creator->fromBytes = &Index_creator_fromBytes;
    Index_creator->toBytes = &Index_creator_toBytes;
    typeTable[0x02][0x05] = Index_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Previous Hash Creator
  struct Object_Creator* PreviousHash_creator = malloc(sizeof(Object_Creator));
  if(PreviousHash_creator != NULL){
    PreviousHash_creator->sizeIdentifierSize = 0;
    PreviousHash_creator->defaultSize = 34;
    PreviousHash_creator->create = PreviousHash_creator_create;
    PreviousHash_creator->fromBytes = PreviousHash_creator_fromBytes;
    PreviousHash_creator->toBytes = PreviousHash_creator_toBytes;
    typeTable[0x02][0x06] = PreviousHash_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Next Public Key Creator
  struct Object_Creator* NPK_creator = malloc(sizeof(Object_Creator));
  if(NPK_creator != NULL){
    NPK_creator->sizeIdentifierSize = 0;
    NPK_creator->defaultSize = 0;
    NPK_creator->create = &NextPublicKey_creator_create;
    NPK_creator->fromBytes = &NextPublicKey_creator_fromBytes;
    NPK_creator->toBytes = &NextPublicKey_creator_toBytes;
    typeTable[0x02][0x07] = NPK_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Bound Witness Transfer
  struct Object_Creator* BWT_creator = malloc(sizeof(Object_Creator));
  if(BWT_creator != NULL){
    BWT_creator->sizeIdentifierSize = 0;
    BWT_creator->defaultSize = 0;
    BWT_creator->create = &BoundWitness_creator_create;
    BWT_creator->fromBytes = &BoundWitnessTransfer_fromBytes;
    BWT_creator->toBytes = &BoundWitnessTransfer_toBytes;
    typeTable[0x02][0x08] = BWT_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize SHA256 Hash Creator
  struct Object_Creator* SHA256_creator = malloc(sizeof(Object_Creator));
  if(SHA256_creator != NULL){
    SHA256_creator->sizeIdentifierSize = 0;
    SHA256_creator->defaultSize = 32;
    SHA256_creator->create = NULL;
    SHA256_creator->fromBytes = NULL;
    SHA256_creator->toBytes = NULL;
    typeTable[0x03][0x05] = SHA256_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize ECDSA Secp256k1 Uncompressed Key TODO
  struct Object_Creator* secp256k1_creator = malloc(sizeof(Object_Creator));
  if(secp256k1_creator != NULL){
    secp256k1_creator->sizeIdentifierSize = 0;
    secp256k1_creator->defaultSize = 64;
    secp256k1_creator->create = &ECDSA_secp256k1Uncompressed_creator_create;
    secp256k1_creator->fromBytes = &ECDSA_secp256k1Uncompressed_creator_fromBytes;
    secp256k1_creator->toBytes = &ECDSA_secp256k1Uncompressed_creator_toBytes;
    typeTable[0x04][0x01] = secp256k1_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Custom Next Public Key Creator
  struct Object_Creator* NPKU_creator = malloc(sizeof(Object_Creator));
  if(NPKU_creator != NULL){
    NPKU_creator->sizeIdentifierSize = 0;
    NPKU_creator->defaultSize = 6;
    NPKU_creator->create = NULL;
    NPKU_creator->fromBytes = NULL;
    NPKU_creator->toBytes = NULL;
    typeTable[0x04][0x03] = NPKU_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize SECP256k1 Signature type
  struct Object_Creator* secp256k1sig_creator = malloc(sizeof(Object_Creator));
  if(secp256k1sig_creator != NULL){
    secp256k1sig_creator->sizeIdentifierSize = 1;
    secp256k1sig_creator->defaultSize = 0;
    secp256k1sig_creator->create = NULL;
    secp256k1sig_creator->fromBytes = NULL;
    secp256k1sig_creator->toBytes = NULL;
    typeTable[0x04][0x03] = secp256k1sig_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize RSSI Heuristic Creator
  struct Object_Creator* rssi_creator = malloc(sizeof(Object_Creator));
  if(rssi_creator != NULL){
    rssi_creator->sizeIdentifierSize = 0;
    rssi_creator->defaultSize = 1;
    rssi_creator->create = &Heuristic_RSSI_Creator_create;
    rssi_creator->fromBytes = &Heuristic_RSSI_Creator_fromBytes;
    rssi_creator->toBytes = &Heuristic_RSSI_Creator_toBytes;
    typeTable[0x08][0x01] = rssi_creator; //TODO: Change major and minor for RSSI when it's standardized.
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Text Heuristic Creator
  struct Object_Creator* text_creator = malloc(sizeof(Object_Creator));
  if(text_creator != NULL){
    text_creator->sizeIdentifierSize = 2;
    text_creator->defaultSize = 0;
    text_creator->create = &Heuristic_Text_Creator_create;
    text_creator->fromBytes = &Heuristic_Text_Creator_fromBytes;
    text_creator->toBytes = &Heuristic_Text_Creator_toBytes;
    typeTable[0x10][0x01] = text_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }
}

void registerType(char id[2], void* creator){
  typeTable[id[0]][id[1]] = creator;
}

void ArrayIteratorNext(){

}

XYResult* lookup(char id[2]){
  void* tableValue = typeTable[id[0]][id[1]];
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result != NULL){
    if(tableValue != 0){
      return_result->error = OK;
      return_result->result = tableValue;
      return return_result;
    }
    else {
      return_result->error = ERR_KEY_DOES_NOT_EXIST;
      return_result->result = 0;
      return return_result;
    }
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }
}
