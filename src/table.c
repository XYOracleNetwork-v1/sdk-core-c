#include "table.h"
//TODO: wal, should this be moved to where other globals are defined?
void* typeTable[TYPE_TABLE_MAJOR_MAX][TYPE_TABLE_MINOR_MAX];

/**
 ****************************************************************************************
 *  NAME
 *      initTable
 *
 *  DESCRIPTION
 *      this routine initializes specific arrays and sets them in typeTable[][]
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      preallocated_result     [out]     XYResult_t*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t* initTable(){
  preallocated_return_result_ptr = preallocated_result;
  int x,y;
  for(x = 0; x < TYPE_TABLE_MAJOR_MAX; x ++) {
      for(y = 0; y < TYPE_TABLE_MINOR_MAX; y ++) typeTable[x][y] = 0;
  }
  /*
  // Initialize Byte Strong Array Creator
  ObjectProvider_t* ByteStrongArray_creator = malloc(sizeof(ObjectProvider_t));
  
  // ******************************** /
  // * guard against malloc errors  * /
  // ******************************** /

  if(ByteStrongArray_creator != NULL){
    ByteStrongArray_creator->sizeIdentifierSize = 1;
    ByteStrongArray_creator->defaultSize = 0;
    ByteStrongArray_creator->create = &ByteStrongArray_creator_create;
    ByteStrongArray_creator->fromBytes = &ByteStrongArray_creator_fromBytes;
    ByteStrongArray_creator->toBytes = &ByteStrongArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_BYTE_SINGLE_TYPE] = ByteStrongArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
  */

  // Initialize Short Strong Array Creator
  ObjectProvider_t* ShortStrongArray_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(ShortStrongArray_creator != NULL){
    ShortStrongArray_creator->sizeIdentifierSize = 2;
    ShortStrongArray_creator->defaultSize = 0;
    ShortStrongArray_creator->create = &ShortStrongArray_creator_create;
    ShortStrongArray_creator->fromBytes = &ShortStrongArray_creator_fromBytes;
    ShortStrongArray_creator->toBytes = &ShortStrongArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_SHORT_SINGLE_TYPE] = ShortStrongArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Int Strong Array Creator
  ObjectProvider_t* IntStrongArray_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(IntStrongArray_creator != NULL){
    IntStrongArray_creator->sizeIdentifierSize = 4;
    IntStrongArray_creator->defaultSize = 0;
    IntStrongArray_creator->create = &IntStrongArray_creator_create;
    IntStrongArray_creator->fromBytes = &IntStrongArray_creator_fromBytes;
    IntStrongArray_creator->toBytes = &IntStrongArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_INT_SINGLE_TYPE] = IntStrongArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
  /*
  // Initialize Byte Weak Array Creator
  ObjectProvider_t* ByteWeakArray_creator = malloc(sizeof(ObjectProvider_t));

  // ******************************** /
  // * guard against malloc errors  * /
  // ******************************** /

  if(ByteWeakArray_creator != NULL){
    ByteWeakArray_creator->sizeIdentifierSize = 1;
    ByteWeakArray_creator->defaultSize = 0;
    ByteWeakArray_creator->create = &ByteWeakArray_creator_create;
    ByteWeakArray_creator->fromBytes = &ByteWeakArray_creator_fromBytes;
    ByteWeakArray_creator->toBytes = &ByteWeakArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_BYTE_MULTI_TYPE] = ByteWeakArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
  */
  // Initialize Short Weak Array Creator
  ObjectProvider_t* ShortWeakArray_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(ShortWeakArray_creator != NULL){
    ShortWeakArray_creator->sizeIdentifierSize = 2;
    ShortWeakArray_creator->defaultSize = 0;
    ShortWeakArray_creator->create = &ShortWeakArray_creator_create;
    ShortWeakArray_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    ShortWeakArray_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_SHORT_MULTI_TYPE] = ShortWeakArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Int Weak Array Creator
  ObjectProvider_t* IntWeakArray_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(IntWeakArray_creator != NULL){
    IntWeakArray_creator->sizeIdentifierSize = 4;
    IntWeakArray_creator->defaultSize = 0;
    IntWeakArray_creator->create = &IntWeakArray_creator_create;
    IntWeakArray_creator->fromBytes = &IntWeakArray_creator_fromBytes;
    IntWeakArray_creator->toBytes = &IntWeakArray_creator_toBytes;
    typeTable[MAJOR_ARRAY][MINOR_INT_MULTI_TYPE] = IntWeakArray_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Bound Witness Creator
  ObjectProvider_t* BoundWitness_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(BoundWitness_creator != NULL){
    BoundWitness_creator->sizeIdentifierSize = 4;
    BoundWitness_creator->defaultSize = 0;
    BoundWitness_creator->create = &BoundWitness_creator_create;
    BoundWitness_creator->fromBytes = &BoundWitness_creator_fromBytes;
    BoundWitness_creator->toBytes = &BoundWitness_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_BOUND_WITNESS] = BoundWitness_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Keyset
  ObjectProvider_t* KeySet_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(KeySet_creator != NULL){
    KeySet_creator->sizeIdentifierSize = 2;
    KeySet_creator->defaultSize = 0;
    KeySet_creator->create = &ShortWeakArray_creator_create;
    KeySet_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    KeySet_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_KEY_SET] = KeySet_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Signature Set Creator
  ObjectProvider_t* SS_creator = malloc(sizeof(ObjectProvider_t));
  if(SS_creator != NULL){
    SS_creator->sizeIdentifierSize = 2;
    SS_creator->defaultSize = 0;
    SS_creator->create = &ShortWeakArray_creator_create;
    SS_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    SS_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_SIGNATURE_SET] = SS_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Payload Creator
  ObjectProvider_t* Payload_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(Payload_creator != NULL){
    Payload_creator->sizeIdentifierSize = 4;
    Payload_creator->defaultSize = 0;
    Payload_creator->create = &Payload_creator_create;
    Payload_creator->fromBytes = &Payload_creator_fromBytes;
    Payload_creator->toBytes = &Payload_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_PAYLOAD] = Payload_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Index Creator
  /*
  ObjectProvider_t* Index_creator = malloc(sizeof(ObjectProvider_t));

  // ******************************** /
  // * guard against malloc errors  * /
  // ******************************** /

  if(Index_creator != NULL){
    Index_creator->sizeIdentifierSize = 0;
    Index_creator->defaultSize = 4;
    Index_creator->create = &Index_creator_create;
    Index_creator->fromBytes = &Index_creator_fromBytes;
    Index_creator->toBytes = &Index_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_INDEX] = Index_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
  */

  // Initialize Previous Hash Creator
  //ObjectProvider_t* PreviousHash_creator = malloc(sizeof(ObjectProvider_t));

  // ******************************** /
  // * guard against malloc errors  * /
  // ******************************** /
/*
  if(PreviousHash_creator != NULL){
    PreviousHash_creator->sizeIdentifierSize = 0;
    PreviousHash_creator->defaultSize = 34;
    PreviousHash_creator->create = PreviousHash_creator_create;
    PreviousHash_creator->fromBytes = PreviousHash_creator_fromBytes;
    PreviousHash_creator->toBytes = PreviousHash_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_PREVIOUS_HASH] = PreviousHash_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
*/
  // Initialize Next Public Key Creator
  //ObjectProvider_t* NPK_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
/*
  if(NPK_creator != NULL){
    NPK_creator->sizeIdentifierSize = 0;
    NPK_creator->defaultSize = 0;
    NPK_creator->create = &NextPublicKey_creator_create;
    NPK_creator->fromBytes = &NextPublicKey_creator_fromBytes;
    NPK_creator->toBytes = &NextPublicKey_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_NEXT_PUBLIC_KEY] = NPK_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
*/
  // Initialize Bound Witness Transfer
  ObjectProvider_t* BWT_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(BWT_creator != NULL){
    BWT_creator->sizeIdentifierSize = 0;
    BWT_creator->defaultSize = 0;
    BWT_creator->create = &BoundWitness_creator_create;
    BWT_creator->fromBytes = &BoundWitnessTransfer_fromBytes;
    BWT_creator->toBytes = &BoundWitnessTransfer_toBytes;
    typeTable[MAJOR_CORE][MINOR_BOUND_WITNESS_TRANSFER] = BWT_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize Bridge Hash Set Creator
  /*
  ObjectProvider_t* HASHSET_creator = malloc(sizeof(ObjectProvider_t));
  if(NPK_creator != NULL){
    HASHSET_creator->sizeIdentifierSize = 2;
    HASHSET_creator->defaultSize = 0;
    HASHSET_creator->create = &ShortWeakArray_creator_create;
    HASHSET_creator->fromBytes = &ShortWeakArray_creator_fromBytes;
    HASHSET_creator->toBytes = &ShortWeakArray_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_OC_HASH_SET] = HASHSET_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }
  */

  // Initialize Bridge Block Set
  ObjectProvider_t* BBS_creator = malloc(sizeof(ObjectProvider_t));
  if(BBS_creator != NULL){
    BBS_creator->sizeIdentifierSize = 4;
    BBS_creator->defaultSize = 0;
    BBS_creator->create = &IntStrongArray_creator_create;
    BBS_creator->fromBytes = &IntStrongArray_creator_fromBytes;
    BBS_creator->toBytes = &BlockSet_creator_toBytes;
    typeTable[MAJOR_CORE][MINOR_OC_SET] = BBS_creator;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize SHA256 Hash Creator
  //ObjectProvider_t* SHA256_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
/*
  if(SHA256_creator != NULL){
    SHA256_creator->sizeIdentifierSize = 0;
    SHA256_creator->defaultSize = 32;
    SHA256_creator->create = Heuristic_sha256_Creator_create;
    SHA256_creator->fromBytes = Heuristic_sha256_Creator_fromBytes;
    SHA256_creator->toBytes = Heuristic_sha256_Creator_toBytes;
    typeTable[MAJOR_HASH][MINOR_SHA256] = SHA256_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
*/
  // Initialize ECDSA Secp256k1 Uncompressed Key TODO
  ObjectProvider_t* secp256k1_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(secp256k1_creator != NULL){
    secp256k1_creator->sizeIdentifierSize = 0;
    secp256k1_creator->defaultSize = 64;
    secp256k1_creator->create = &ECDSA_secp256k1Uncompressed_creator_create;
    secp256k1_creator->fromBytes = &ECDSA_secp256k1Uncompressed_creator_fromBytes;
    secp256k1_creator->toBytes = &ECDSA_secp256k1Uncompressed_creator_toBytes;

    typeTable[0x04][0x01] = secp256k1_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize SECP256k1 Signature type
  ObjectProvider_t* secp256k1sig_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(secp256k1sig_creator != NULL){
    secp256k1sig_creator->sizeIdentifierSize = 1;
    secp256k1sig_creator->defaultSize = 0;
    secp256k1sig_creator->create = &ECDSA_secp256k1Sig_creator_create;
    secp256k1sig_creator->fromBytes = &ECDSA_secp256k1Sig_creator_fromBytes;
    secp256k1sig_creator->toBytes = &ECDSA_secp256k1Sig_creator_toBytes;
    typeTable[MAJOR_SIGNATURES][MINOR_ECDSA_SECP256K1_SHA256_SIGNATURE] = secp256k1sig_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize SECP256k1 Signature type
  ObjectProvider_t* secp256k1sha1sig_creator = malloc(sizeof(ObjectProvider_t));
  if(secp256k1sha1sig_creator != NULL){
    secp256k1sha1sig_creator->sizeIdentifierSize = 0;
    secp256k1sha1sig_creator->defaultSize = 69;
    secp256k1sha1sig_creator->create = &ECDSA_secp256k1Sig_creator_create;
    secp256k1sha1sig_creator->fromBytes = &ECDSA_secp256k1Sig_creator_fromBytes;
    secp256k1sha1sig_creator->toBytes = &ECDSA_secp256k1Sig_creator_toBytes;
    typeTable[MAJOR_SIGNATURES][MINOR_ECDSA_SECP256K1_SHA1_SIGNATURE] = secp256k1sha1sig_creator;
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }

  // Initialize RSSI Heuristic Creator
  ObjectProvider_t* rssi_creator = malloc(sizeof(ObjectProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(rssi_creator != NULL){
    rssi_creator->sizeIdentifierSize = 0;
    rssi_creator->defaultSize = 1;
    rssi_creator->create = &Heuristic_RSSI_Creator_create;
    rssi_creator->fromBytes = &Heuristic_RSSI_Creator_fromBytes;
    rssi_creator->toBytes = &Heuristic_RSSI_Creator_toBytes;
    typeTable[MAJOR_HEURISTICS][MINOR_RSSI] = rssi_creator; //TODO: Change major and minor for RSSI when it's standardized.
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = 0;
    return preallocated_result;
  }

  // Initialize Text Heuristic Creator
  /*
  ObjectProvider_t* text_creator = malloc(sizeof(ObjectProvider_t));
  if(text_creator != NULL){
    text_creator->sizeIdentifierSize = 2;
    text_creator->defaultSize = 0;
    text_creator->create = &Heuristic_Text_Creator_create;
    text_creator->fromBytes = &Heuristic_Text_Creator_fromBytes;
    text_creator->toBytes = &Heuristic_Text_Creator_toBytes;
    //typeTable[MAJOR_CUSTOM][MINOR_TEXT] = text_creator;   // wal, compiler warning, subscript out of range
  }
  else {
    preallocated_return_result_ptr->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
  */

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = 0;

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t* tableLookup(const char id[2]){

  void* tableValue = typeTable[id[0]][id[1]];

  preallocated_return_result_ptr = &preallocated_return_result;

  if(tableValue != 0){

    preallocated_return_result_ptr->error = OK;
    preallocated_return_result_ptr->result = tableValue;

      return preallocated_return_result_ptr;
  }
  else {
    preallocated_return_result_ptr->error = ERR_KEY_DOES_NOT_EXIST;
    preallocated_return_result_ptr->result = 0;

    return preallocated_return_result_ptr;
  }
}
