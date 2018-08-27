/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: XYOHeuristicsBuilder.c
 * @Last modified by:   Nate Brune
 * @Last modified time: 09-Aug-2018
 * @Copyright: XY - The Findables Company
 */
 #include "xyobject.h"
 #include <stdlib.h>
 #include <string.h>
 #include "serializer.h"
 #include "xyo.h"
 #include "XYOHeuristicsBuilder.h"
 #include <stdio.h>

XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi){
  return newObject(id, rssi);
}

XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[3];
  return newObject(id, &rssi);
}

XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObect){
  char encoded_bytes[3];
  encoded_bytes[0] = 0x09;
  encoded_bytes[1] = 0x01;
  char* rssi = (char*)user_XYObect->payload;
  encoded_bytes[2] = rssi[2];
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
