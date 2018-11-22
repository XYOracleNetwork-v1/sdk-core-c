#include "xyresult.h"
#include "XYObjects/XYObject.h"
#include "hash.h"

XYResult_t* Heuristic_sha256_Creator_create(const char id[2], void* sha256){
  return newObject(id, sha256);
}

XYResult_t* Heuristic_sha256_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  char* hash = malloc(sizeof(char)*32);
  memcpy(hash, heuristic_data+2, 32);
  return newObject(id, &hash);
}

XYResult_t* Heuristic_sha256_Creator_toBytes(XYObject_t* user_XYObject){
  XYResult_t* return_result = malloc(sizeof(XYResult_t));
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
