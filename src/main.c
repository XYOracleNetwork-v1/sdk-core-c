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
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

int main(){
  preallocated_result = malloc(sizeof(struct XYResult));
  if(preallocated_result){
    initTable();
    char* testArray = { 0x01 0x01 0x06 0x0a 0x0b 0x0c}
    char id[2] = { 0x01 0x01 }
    XYResult* result = lookup(id);
    ByteStrongArray_creator* arrayCreator = result.result;
    XYResult* result = arrayCreator.fromBytes(testArray);
    if(result.error == OK){
      ByteStrongArray* array = result.result;
    }
    printf("%p", array);
    else {
      RETURN_ERROR(ERR_CRITICAL)
    }
  }
  else{
    return -1;
  }
  return 0;
}
