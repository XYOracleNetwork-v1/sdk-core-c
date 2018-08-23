#include <stdlib.h>
#include <string.h>
#include "serializer.h"
#include "xyo.h"



uint32_t to_uint32(char* data) {
  return data[0] + 256U*data[1] + 65536U*data[2] + 16777216U*data[3];
}

uint16_t to_uint16(char* data) {
  return data[0] + 256U*data[1];
}

void SZSNSArray_add(XYObject* self_SZSNSArray, XYObject* user_XYObect){
  XYResult* lookup_result = lookup(self_SZSNSArray->id);
  if(lookup_result.error != NULL){
    SZSNSArrayCreator* self_SZSNSArrayCreator = lookup_result->result;
    if(self_SZSNSArrayCreator->sizeBytes->defaultSize != NULL){
      self_SZSNSArrayCreator->sizeBytes->sizeIdentifierSize;

      uint16_t newSize = self_SZSNSArray->size + user_XYObject
      self_SZSNSArray->payload = realloc(self_SZSNSArray->payload ;
    }
    else if(self_SZSNSArrayCreator->){

    }
  }
}

XYResult* SZSNSArray_create(char id[2]){
  SZSNSArray* SZSNSArrayObject = malloc(sizeof(SZSNSArray));
  if(SZSNSArrayObject){
    SZSNSArrayObject->id[0] = id[0];
    SZSNSArrayObject->id[1] = id[1];
    SZSNSArrayObject->size = sizeof(SZSNSArray);
    SZSNSArrayObject->elements = 0;
    SZSNSArrayObject->payload = NULL:
    preallocated_result->error = NULL;
    preallocated_result->result = SZSNSArray;
  }
  else {
    preallocated_result->error = ERR_INSUFFICIENT_MEMORY;
    preallocated_result->result = NULL;
    return preallocated_result;
  }

}

/* todo */
XYResult* SZSNSArray_createFromBytes(char* data){
  SZSNSArray array = malloc(sizeof(SZSNSArray))
  array->id[0] = data[0];
  array->id[1] = data[1];
  char sizeAndElements[2];
  sizeAndElements[0] = data[2];
  sizeAndElements[1] = data[3];
  array->size = to_uint16(sizeAndElements);
  sizeAndElements[0] = data[4];
  sizeAndElements[1] = data[5];
  array->elements = to_uint16(sizeAndElements);
  array->add = &SZSNSArray_add;


}
