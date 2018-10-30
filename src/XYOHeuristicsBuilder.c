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
 #include "xyobject.h"
 #include <stdlib.h>
 #include <string.h>
 #include "xyo.h"
 #include "XYOHeuristicsBuilder.h"
 #include <stdio.h>

void breakpoint(void){}

/*----------------------------------------------------------------------------*
*  NAME
*      to_uintxx(char* data)
*
*  DESCRIPTION
*      Interpretes the supplied data pointer as a unsigned integer in Big Endian.
*      The Author may use these routines to flip endian of a number but it should
*      be discouraged
*
*  PARAMETERS
     *data            [in]        char*
*
*  RETURNS
*      found          [out]      uintxx   returns unsigned integer representing the data given
*----------------------------------------------------------------------------*/       // Just a sample doc template.
 uint32_t to_uint32(unsigned char* data) {
   return 16777216U*data[0] + 65536U*data[1] + 256U*data[2] + data[3];
 }

 uint16_t to_uint16(unsigned char* data) {
   return 256U*data[0] + data[1];
 }

 /*----------------------------------------------------------------------------*
 *  NAME
 *      littleEndian
 *
 *  DESCRIPTION
 *      Determines the endian of the device we are running on.
 *
 *  PARAMETERS
 *
 *  RETURNS
 *      result  [out]      bool         returns TRUE if Little Endian endian, FALSE if Big Endian.
 *----------------------------------------------------------------------------
 */
int littleEndian(void){
  volatile uint32_t i=0x01234567;
  // return 0 for big endian, 1 for little endian.
  return (*((uint8_t*)(&i))) == 0x67;
}

XYResult* Heuristic_RSSI_Creator_create(const char id[2], void* rssi){
  return newObject(id, rssi);
}

XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[2];
  return newObject(id, &rssi);
}

XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObject){
  char encoded_bytes;
  char* rssi = (char*)user_XYObject->payload;
  encoded_bytes = rssi[0];

  struct XYResult* return_result = malloc(sizeof(struct XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = &encoded_bytes;
    return return_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }
}

XYResult* Heuristic_Text_Creator_create(const char id[2], void* text){
  return newObject(id, text);
}

XYResult* Heuristic_Text_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  uint16_t size = to_uint16((unsigned char*)&heuristic_data[2]);
  char* payload_bytes = malloc(size*sizeof(char));
  memcpy(payload_bytes, &heuristic_data[2], size);
  return newObject(id, payload_bytes);
}

XYResult* Heuristic_Text_Creator_toBytes(struct XYObject* user_XYObject){
  char* text = user_XYObject->payload;
  uint16_t size = to_uint16((unsigned char*)text);
  uint16_t encodedSize = size;
  if(littleEndian()){
    encodedSize = to_uint16((unsigned char*)&encodedSize);
  }
  char* encoded_bytes = malloc(sizeof(char)*size);
  if(encoded_bytes == NULL) {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  memcpy(encoded_bytes, &encodedSize, sizeof(uint16_t));
  memcpy(encoded_bytes+(2*sizeof(char)), text+(2*sizeof(char)), size-(2*sizeof(char)));

  struct XYResult* return_result = malloc(sizeof(struct XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = encoded_bytes;
    return return_result;
  }
  else {
    if(encoded_bytes) free(encoded_bytes);
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }
}

XYResult* ECDSA_secp256k1Uncompressed_creator_create(const char id[2], void* text){
  return newObject(id, text);
}

XYResult* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* key_data){
  char id[2];
  memcpy(id, key_data, 2);
  ECDSA_secp256k1_uncompressed* key = malloc(sizeof(ECDSA_secp256k1_uncompressed));
  memcpy(key->point_x, &key_data[2], 32*sizeof(char));
  memcpy(key->point_y, &key_data[2+32], 32*sizeof(char));
  return newObject(id, key);
}

XYResult* ECDSA_secp256k1Uncompressed_creator_toBytes(struct XYObject* user_XYObject){
  ECDSA_secp256k1_uncompressed* raw_key = user_XYObject->payload;
  char* encoded_bytes = malloc(64*sizeof(char));

  if(encoded_bytes == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  memcpy(encoded_bytes, raw_key->point_x , 32*sizeof(char));
  memcpy(encoded_bytes+32*sizeof(char), raw_key->point_y , 32*sizeof(char));

  struct XYResult* return_result = malloc(sizeof(struct XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = encoded_bytes;
    return return_result;
  }
  else {
    if(encoded_bytes) free(encoded_bytes);
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }
}

XYResult* ECDSA_secp256k1Sig_creator_create(const char id[2], void* text){
  return newObject(id, text);
}

XYResult* ECDSA_secp256k1Sig_creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  uint8_t size = (uint8_t)heuristic_data[3];
  char* payload_bytes = malloc(size-(1*sizeof(char)));
  memcpy(payload_bytes, &heuristic_data[2], size);
  secp256k1Signature* return_signature = malloc(sizeof(secp256k1Signature));
  return_signature->size = size;
  return_signature->signature = payload_bytes;
  return newObject(id, return_signature);
}

XYResult* ECDSA_secp256k1Sig_creator_toBytes(struct XYObject* user_XYObject){
  secp256k1Signature* raw_signature = user_XYObject->payload;
  char* encoded_bytes = malloc(sizeof(char)*raw_signature->size);

  if(encoded_bytes == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  secp256k1Signature* user_sig = user_XYObject->payload;
  uint16_t size = user_sig->size;
  size = to_uint16((unsigned char*)&size);
  memcpy(encoded_bytes, &size, 2);

  memcpy(encoded_bytes+2, user_sig->signature, (sizeof(char)*user_sig->size-2));
  encoded_bytes[2] = 0x05;
  encoded_bytes[3] = 0x01;
  encoded_bytes[4] = user_sig->size-4;

  struct XYResult* return_result = malloc(sizeof(struct XYResult));
  if(return_result != NULL){
    return_result->error = OK;
    return_result->result = encoded_bytes;
    return return_result;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }
}

XYResult* Heuristic_sha256_Creator_create(const char id[2], void* sha256){
  return newObject(id, sha256);
}

XYResult* Heuristic_sha256_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  char* hash = malloc(sizeof(char)*32);
  memcpy(hash, heuristic_data+2, 32);
  return newObject(id, &hash);
}

XYResult* Heuristic_sha256_Creator_toBytes(struct XYObject* user_XYObject){
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    char* return_buffer = malloc(sizeof(char)*32);
    //memcpy(return_buffer, user_XYObject->id, 2);
    memcpy(return_buffer, user_XYObject->payload, 32);
    return_result->error = OK;
    return_result->result = return_buffer;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
