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
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char* hashGetId(HashProvider* object){
  return object->id;
}

int verify(ByteArray* hash, ByteArray* data){
  return TRUE;
}

XYResult* hash(ByteArray* user){
  ByteArray* return_array = malloc(sizeof(ByteArray));
  if(return_array){
    return_array->size = 32;
    return_array->payload = malloc(sizeof(char)*32);
    if(return_array->payload){
      srand(time(NULL));
      for(int i = 0; i<32; i++){
        memset(return_array->payload+i, ((uint8_t)rand())%255, 1);
      }

      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result){
        return newObject((char*)&Sha256_id, return_array);
      } else {
        free(return_array->payload);
        free(return_array);
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
    } else {
      free(return_array);
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

HashProvider* newHashProvider(){
  HashProvider* hasher = malloc(sizeof(HashProvider));
  if(hasher == NULL){ return NULL; }
  hasher->Hash = hash;
  hasher->VerifyHash = verify;
  hasher->GetId = &hashGetId;
  hasher->id[0] = 0x03;
  hasher->id[1] = 0x05;
  return hasher;
}
