/**
 ****************************************************************************************
 *
 * @file xyobject.c
 *
 * @XY4 project source code.
 *
 * @brief primary xy object routines for the XY4 firmware.
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

#include "xyobject.h"
#include "xyo.h"

/**
 ****************************************************************************************
 *  NAME
 *      getXyobjectId
 *
 *  DESCRIPTION
 *      this routine returns the id of the supplied xyo object 
 *
 *  PARAMETERS
 *      object    [in]      XYObject*
 *
 *  RETURNS
 *      id        [out]     char*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
char* getXyobjectId(XYObject_t* xyobject){
  return xyobject->id;
}

/**
 ****************************************************************************************
 *  NAME
 *      getPayload
 *
 *  DESCRIPTION
 *      this routine returns the payload of the supplied xyo object 
 *
 *  PARAMETERS
 *      object    [in]      XYObject*
 *
 *  RETURNS
 *      id        [out]     char*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
void* getPayload(XYObject_t* xyobject){
  return xyobject->payload;
}

/**
 ****************************************************************************************
 *  NAME
 *      newObject
 *
 *  DESCRIPTION
 *      this routine creates a new object 
 *
 *  PARAMETERS
 *      id[]              [in]      char
 *      payload           [in]      void*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
XYResult_t* newObject(char id[2], void* payload){
  
  XYObject_t* new_object = malloc(sizeof(XYObject_t));
  
  if(new_object != NULL){
    if(payload != 0){
      new_object->payload = payload;
    }
    else{
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
    strncpy(new_object->id, id, 2);
    new_object->GetXyobjectId = &getXyobjectId;
    new_object->GetPayload = &getPayload;
    XYResult_t* return_result = malloc(sizeof(XYResult_t));
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

//TODO: wal, should this be moved to where all the globals are defined?
void* typeTable[17][16];

/**
 ****************************************************************************************
 *  NAME
 *      initTable
 *
 *  DESCRIPTION
 *      this routine initializes specific arrays and sets them in typeTable[][]
 *
 *  PARAMETERS
 *      id[]                    [in]      char
 *      payload                 [in]      void*
 *
 *  RETURNS
 *      preallocated_result     [out]     XYResult*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
XYResult_t* initTable(){
  
  int x,y;
  
  for(x = 0; x < 17; x ++) {
      for(y = 0; y < 16; y ++) typeTable[x][y] = 0;
  }

  // Initialize Byte Strong Array Creator
  struct ObjectProvider* ByteStrongArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  struct ObjectProvider* ShortStrongArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  struct ObjectProvider* IntStrongArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* ByteWeakArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* ShortWeakArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* IntWeakArray_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* BoundWitness_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* KeySet_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* Payload_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* Index_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* PreviousHash_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* NPK_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* BWT_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* SHA256_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* secp256k1_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* NPKU_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* secp256k1sig_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* rssi_creator = malloc(sizeof(ObjectProvider_t));
  
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
  ObjectProvider_t* text_creator = malloc(sizeof(ObjectProvider_t));
  
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

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
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

/**
 ****************************************************************************************
 *  NAME
 *      registerType
 *
 *  DESCRIPTION
 *      this routine sets typeTable[id[0]][id[1]] to the supplied creator
 *
 *  PARAMETERS
 *      id[]                    [in]      char
 *      creator                 [in]      void*
 *
 *  RETURNS
 *      preallocated_result     [out]     XYResult*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
void registerType(char id[2], void* creator){
  
  typeTable[id[0]][id[1]] = creator;
}

/**
 ****************************************************************************************
 *  NAME
 *      ArrayIteratorNext
 *
 *  DESCRIPTION
 *      this routine needs to be filled in with something to do
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      nothing
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
void ArrayIteratorNext(){
  //TODO: wal, needs something here or we can just remove this method
}

/**
 ****************************************************************************************
 *  NAME
 *      lookup
 *
 *  DESCRIPTION
 *      this routine looks up the supplied id[] in typeTablel[][]
 *
 *  PARAMETERS
 *      id[]              [in]      char
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
XYResult_t* tableLookup(char id[2]){
  
  void* tableValue = typeTable[id[0]][id[1]];
  
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
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

// end of file xyobject.c

