/**
 * @Author: Nate Brune
 * @Date:   10-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: main.c
 * @Last modified by:   Nate Brune
 * @Last modified time: 10-Aug-2018
 * @Copyright: XY - The Findables Company
 */
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "serializer.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

int main(){
  preallocated_result = malloc(sizeof(struct XYResult));
  if(preallocated_result){
      XYResult* result = initTable();
      if(result->error == OK){
      char testArray[8] = { 0x01, 0x01, 0x06, 0x09, 0x01, 0x09, 0x09, 0x01 };
      uint8_t id[2];
      id[0] = 0x01;
      id[1] = 0x01;
      result = lookup(id);
      Object_Creator* arrayCreator = result->result;
      result = arrayCreator->fromBytes(&testArray);
      if(result->error == OK){
        ByteStrongArray* array = result->result;
        printf("Array id: %c %c\n", array->id[0], array->id[1]);
        printf("Array size: %d\n", array->size);
        printf("End Meme.\n");
      }
      else {
        return -1;
      }
    }
    else{
      return -1;
    }
  }
  else{
    return -1;
  }
  return 0;
}
