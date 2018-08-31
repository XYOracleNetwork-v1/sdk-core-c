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
 #include "xyo.h"
 #include "XYOHeuristicsBuilder.h"
 #include <stdio.h>
 #include <arpa/inet.h>

void breakpoint(){};

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
 uint32_t to_uint32(char* data) {
   return 16777216U*data[0] + 65536U*data[1] + 256U*data[2] + data[3];
 }

 uint16_t to_uint16(char* data) {
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
 *----------------------------------------------------------------------------*/

int littleEndian(){
  volatile uint32_t i=0x01234567;
  // return 0 for big endian, 1 for little endian.
  return (*((uint8_t*)(&i))) == 0x67;
}

XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi){
  return newObject(id, rssi);
}

XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[2];
  return newObject(id, &rssi);
}

XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObect){
  char encoded_bytes;
  char* rssi = (char*)user_XYObect->payload;
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

XYResult* Heuristic_Text_Creator_create(char id[2], void* text){
  return newObject(id, text);
}

XYResult* Heuristic_Text_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  uint16_t size = to_uint16(&heuristic_data[2]);
  char* payload_bytes = malloc(size*sizeof(char));
  memcpy(payload_bytes, &heuristic_data[2], size);
  return newObject(id, payload_bytes);
}

XYResult* Heuristic_Text_Creator_toBytes(struct XYObject* user_XYObect){
  char* text = user_XYObect->payload;
  uint16_t size = to_uint16(text);
  uint16_t encodedSize = size;
  if(littleEndian()){
    uint16_t encodedSize = to_uint16((char*)&encodedSize);
  }
  char* encoded_bytes = malloc(sizeof(char)*size);

  memcpy(encoded_bytes, &encodedSize, sizeof(uint16_t));
  memcpy(encoded_bytes+(2*sizeof(char)), text+(2*sizeof(char)), size-(2*sizeof(char)));

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
