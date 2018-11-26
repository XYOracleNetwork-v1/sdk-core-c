#include "objects.h"

XYResult_t* ECDSA_secp256k1Uncompressed_creator_create(const char id[2], void* text){
  return NULL;//newObject(id, text);
}

XYResult_t* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* key_data){
  /*
  char id[2];
  memcpy(id, key_data, 2);
  ECDSA_secp256k1_uncompressed_t* key = malloc(sizeof(ECDSA_secp256k1_uncompressed_t));
  memcpy(key->point_x, &key_data[2], 32*sizeof(char));
  memcpy(key->point_y, &key_data[2+32], 32*sizeof(char));
  */
  return NULL;//ewObject(id, key);
}

XYResult_t* ECDSA_secp256k1Uncompressed_creator_toBytes(XYObject_t* user_XYObject){
  /*
  ECDSA_secp256k1_uncompressed_t* raw_key = user_XYObject->payload;
  char* encoded_bytes = malloc(64*sizeof(char));

  if(encoded_bytes == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  memcpy(encoded_bytes, raw_key->point_x , 32*sizeof(char));
  memcpy(encoded_bytes+32*sizeof(char), raw_key->point_y , 32*sizeof(char));

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;
  */
  return NULL;//preallocated_return_result_ptr;
}

XYResult_t* ECDSA_secp256k1Sig_creator_create(const char id[2], void* text){
  return NULL;//newObject(id, text);
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Sig_creator_fromBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     heuristic_data          [in]      char*
*
*  RETURNS
*     newObject(id, return_signature)   [out]     XYResult_t*
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Sig_creator_fromBytes(char* heuristic_data){

  /********************************/
  /* guard against bad input data */
  /******************************** /

  if(!heuristic_data) {RETURN_ERROR(ERR_BADDATA);}

  char id[2];
  memcpy(id, heuristic_data, 2);
  uint8_t size = (uint8_t)heuristic_data[3];

  char* payload_bytes = malloc(size-(1*sizeof(char)));

  /********************************/
  /* guard against malloc errors  */
  /******************************** 

  if(payload_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(payload_bytes, &heuristic_data[2], size);

  secp256k1Signature_t* return_signature = malloc(sizeof(secp256k1Signature_t));

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(return_signature == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  return_signature->size = size;
  return_signature->signature = payload_bytes;
  */
  return NULL; //newObject(id, return_signature);   //TODO: wal, check that errors are returned
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Sig_creator_toBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     user_XYObject       [in]      XYObject_t*
*
*  RETURNS
*     preallocated_return_result_ptr       [out]     XYResult_t*
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Sig_creator_toBytes(XYObject_t* user_XYObject){

  /********************************/
  /* guard against bad input data */
  /******************************** /

  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA);}

  secp256k1Signature_t* raw_signature = user_XYObject->payload;

  char* encoded_bytes = malloc(raw_signature->size);

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(encoded_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  secp256k1Signature_t* user_sig = user_XYObject->payload;

  memcpy(encoded_bytes+2, user_sig->signature, (sizeof(char)*user_sig->size-2));
  encoded_bytes[2] = 0x05;
  encoded_bytes[3] = 0x01;
  encoded_bytes[4] = user_sig->size-4;

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;
  */
  return NULL;//preallocated_return_result_ptr;
}
