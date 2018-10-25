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
 
/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"

// Just a sample doc template.
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
*----------------------------------------------------------------------------*/       
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

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_RSSI_Creator_create
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     id[]       [in]      char
*     rssi       [in]      void*
*
*  RETURNS
*     newObject(id, &rssi)     [out]     XYResult_t    
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_RSSI_Creator_create(char id[2], void* rssi){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!rssi) {RETURN_ERROR(ERR_BADDATA)};

  return newObject(id, &rssi);
}

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_RSSI_Creator_fromBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     heuristic_data          [in]      char*
*
*  RETURNS
*     newObject(id, &rssi)    [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!heuristic_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[2];
  
  return newObject(id, &rssi);
}

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_RSSI_Creator_toBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     user_XYObject     [in]       XYObject_t*
*
*  RETURNS
*     preallocated_return_result_ptr     [out]      XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_RSSI_Creator_toBytes(XYObject_t* user_XYObject){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  char encoded_bytes;
  char* rssi = (char*)user_XYObject->payload;
  encoded_bytes = rssi[0];

  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &encoded_bytes;
    
  return preallocated_return_result_ptr;
}

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_Text_Creator_create
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     id[]                    [in]      char
*     text                    [in]      void*
*
*  RETURNS
*     newObject(id, text)     [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_Text_Creator_create(char id[2], void* text){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!text) {RETURN_ERROR(ERR_BADDATA)};

  return newObject(id, text);
}

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_Text_Creator_fromBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     heuristic_data                    [in]      char*
*
*  RETURNS
*     newObject(id, payload_bytes)      [out]     XYResult_t*   
*
*  NOTES
*     
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_Text_Creator_fromBytes(char* heuristic_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!heuristic_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  memcpy(id, heuristic_data, 2);  //TODO: wal, constants please
  
  uint16_t size = to_uint16(&heuristic_data[2]);
  
  char* payload_bytes = malloc(size*sizeof(char));
  
  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(!payload_bytes) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};
    
  memcpy(payload_bytes, &heuristic_data[2], size);
  
  return newObject(id, payload_bytes);
}

/*----------------------------------------------------------------------------*
*  NAME
*     Heuristic_Text_Creator_toBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     user_XYObject     [in]      XYObject_t*
*
*  RETURNS
*     preallocated_return_result_ptr      [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* Heuristic_Text_Creator_toBytes(XYObject_t* user_XYObject){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  char* text = user_XYObject->payload;
  uint16_t size = to_uint16(text);
  uint16_t encodedSize = size;
  
  if(littleEndian()){
    encodedSize = to_uint16((char*)&encodedSize);
  }
  
  char* encoded_bytes = malloc(sizeof(char)*size);
  
  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(encoded_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};
  
  memcpy(encoded_bytes, &encodedSize, sizeof(uint16_t));
  memcpy(encoded_bytes+(2*sizeof(char)), 
                        text+(2*sizeof(char)), 
                        size-(2*sizeof(char)));

  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;
    
  return preallocated_return_result_ptr;
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Uncompressed_creator_create
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     id[]                  [in]      char
*     text                  [in]      void*
*
*  RETURNS
*     newObject(id, text)   [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Uncompressed_creator_create(char id[2], void* text){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!text) {RETURN_ERROR(ERR_BADDATA)};

  return newObject(id, text);
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Uncompressed_creator_fromBytes
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     key_data              [in]      char*
*
*  RETURNS
*     newObject(id, key)    [out]     XYResult_t*   
*
*  NOTES
*     
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Uncompressed_creator_fromBytes(char* key_data){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!key_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  
  memcpy(id, key_data, 2);    //TODO: wal, constant please
  
  ECDSA_secp256k1_uncompressed_t* key = malloc(sizeof(ECDSA_secp256k1_uncompressed_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(!key) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}
  
  memcpy(key->point_x, &key_data[2], 32*sizeof(char));    //TODO: wal, constant please
  memcpy(key->point_y, &key_data[2+32], 32*sizeof(char));
  
  return newObject(id, key);  //TODO: wal, check that we are returning any errors
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Uncompressed_creator_toBytes_t
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     user_XYObject     [in]      XYObject_t*
*
*  RETURNS
*     preallocated_return_result_ptr     [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Uncompressed_creator_toBytes(XYObject_t* user_XYObject){
  
  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  ECDSA_secp256k1_uncompressed_t* raw_key = user_XYObject->payload;
  
  char* encoded_bytes = malloc(64*sizeof(char));  //TODO: wal, constant please

  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(encoded_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(encoded_bytes, raw_key->point_x , 32*sizeof(char));
  memcpy(encoded_bytes+32*sizeof(char), raw_key->point_y , 32*sizeof(char));

  preallocated_return_result_ptr = &preallocated_return_result;
  
  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;
    
  return preallocated_return_result_ptr;
}

/*----------------------------------------------------------------------------*
*  NAME
*     ECDSA_secp256k1Sig_creator_create
*
*  DESCRIPTION
*     tbd
*
*  PARAMETERS
*     id[]                    [in]      char
*     text                    [in]      void*
*
*  RETURNS
*     newObject(id, text)     [out]     XYResult_t*   
*
*  NOTES
*
*----------------------------------------------------------------------------
*/
XYResult_t* ECDSA_secp256k1Sig_creator_create(char id[2], void* text){

  /********************************/
  /* guard against bad input data */
  /********************************/
  
  if(!text) {RETURN_ERROR(ERR_BADDATA)};

  return newObject(id, text);
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
  /********************************/
  
  if(!heuristic_data) {RETURN_ERROR(ERR_BADDATA)};

  char id[2];
  memcpy(id, heuristic_data, 2);
  uint8_t size = heuristic_data[3];
  
  char* payload_bytes = malloc(size-(1*sizeof(char)));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
    
  if(payload_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(payload_bytes, &heuristic_data[2], size);
  
  secp256k1Signature_t* return_signature = malloc(sizeof(secp256k1Signature_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
    
  if(return_signature == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  return_signature->size = size;
  return_signature->signature = payload_bytes;
  
  return newObject(id, return_signature);   //TODO: wal, check that errors are returned
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
  /********************************/
  
  if(!user_XYObject) {RETURN_ERROR(ERR_BADDATA)};

  secp256k1Signature_t* raw_signature = user_XYObject->payload;
  
  char* encoded_bytes = malloc(raw_signature->size);

  /********************************/
  /* guard against malloc errors  */
  /********************************/
    
  if(encoded_bytes == NULL) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);}

  memcpy(encoded_bytes, user_XYObject->payload, raw_signature->size-(1*sizeof(char)));

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = encoded_bytes;
    
  return preallocated_return_result_ptr;
}

// end of file xyoheuristicsbuilder.c

