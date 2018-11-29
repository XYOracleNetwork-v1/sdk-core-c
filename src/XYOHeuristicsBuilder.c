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
 #include "XYObjects/XYObject.h"
 #include <stdlib.h>
 #include <string.h>
 #include "xyo.h"
 #include <stdio.h>

void breakpoint(void){};

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
*----------------------------------------------------------------------------* /
 uint32_t to_uint32(unsigned char* data) {
   return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
 }

 uint16_t to_uint16(unsigned char* data) {
   return (data[0] << 8) + data[1];
 }

/*----------------------------------------------------------------------------*
*  NAME
*      littleEndian
*
*  DESCRIPTION
*      Determines the endian of the device we are running on.
*
*  PARAMETERS
*      none
*
*  RETURNS
*      result      [out]     bool    returns TRUE if Little Endian endian, FALSE
*                                    if Big Endian.
*  NOTES
*
*----------------------------------------------------------------------------
*/
int littleEndian(void){

  volatile uint32_t i=0x01234567;
  // return 0 for big endian, 1 for little endian.
  return (*((uint8_t*)(&i))) == 0x67;
}
/*
XYResult_t* Heuristic_RSSI_Creator_create(const char id[2], void* rssi){
  return newObject(id, rssi);
}

XYResult_t* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[2];
  return newObject(id, &rssi);
}

XYResult_t* Heuristic_RSSI_Creator_toBytes(XYObject_t* user_XYObject){
  char encoded_bytes;
  char* rssi = (char*)user_XYObject->payload;
  encoded_bytes = rssi[0];

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &encoded_bytes;

  return preallocated_return_result_ptr;
}

XYResult_t* Heuristic_Text_Creator_create(const char id[2], void* text){
  return newObject(id, text);
}

XYResult_t* Heuristic_Text_Creator_fromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);  //TODO: wal, constants please

  uint16_t size = to_uint16(&heuristic_data[2]);

  char* payload_bytes = malloc(size*sizeof(char));

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(!payload_bytes) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(payload_bytes, &heuristic_data[2], size);

  return newObject(id, payload_bytes);
}

XYResult_t* Heuristic_Text_Creator_toBytes(XYObject_t* user_XYObject){
  char* text = user_XYObject->payload;
  uint16_t size = to_uint16(text);
  uint16_t encodedSize = size;

  if(littleEndian()){
    encodedSize = to_uint16((unsigned char*)&encodedSize);
  }

  char* encoded_bytes = malloc(sizeof(char)*size);

  /********************************/
  /* guard against malloc errors  */
  /******************************** /

  if(encoded_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(encoded_bytes, &encodedSize, sizeof(uint16_t));
  memcpy(encoded_bytes+(2*sizeof(char)),
                        text+(2*sizeof(char)),
                        size-(2*sizeof(char)));

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;

  return preallocated_return_result_ptr;
}
*/
// end of file xyoheuristicsbuilder.c
