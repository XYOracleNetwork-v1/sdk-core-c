#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <arpa/inet.h>

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the NextPublicKey type.
*----------------------------------------------------------------------------*/
XYResult* NextPublicKey_creator_create(char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_fromBytes
*
*  DESCRIPTION
*      Create an Next Public Key object given a set of Bytes. Bytes must be
*      payload of Next Public Key object.
*
*  PARAMETERS
*     *pubkey_data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the NextPublicKey type.
*----------------------------------------------------------------------------*/
XYResult* NextPublicKey_creator_fromBytes(char* pubkey_data){
  char id[2];
  memcpy(id, pubkey_data, 2);
  char pubkey_id[2];
  memcpy(pubkey_id, pubkey_data+(sizeof(char)*2), 2);
  XYResult* lookup_result = lookup(id);
  XYResult* lookup_result2 = lookup(pubkey_id);
  NextPublicKey* return_NPK = malloc(sizeof(NextPublicKey));
  if(lookup_result->error == OK && lookup_result2->error == OK && return_NPK){
    Object_Creator* NPK_creator = lookup_result2->result;
    uint32_t element_size = 0;
    uint8_t offset = 0;
    if(NPK_creator->defaultSize != 0){
      element_size = NPK_creator->defaultSize;
      return_NPK->publicKey = malloc(element_size*sizeof(char));
    }
    else if(NPK_creator->sizeIdentifierSize != 0)
    {
      switch(NPK_creator->sizeIdentifierSize){
        case 1:
          element_size = pubkey_data[4];
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
        case 2:
          element_size = to_uint16(&pubkey_data[4]);
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
        case 4:
          element_size = to_uint32(&pubkey_data[4]);
          return_NPK->publicKey = malloc(element_size*sizeof(char));
          break;
      }
    }
    else
    {
      RETURN_ERROR(ERR_CRITICAL);
    }
    if(return_NPK->publicKey){
      memcpy(return_NPK->id, &pubkey_id, 2);
      memcpy(return_NPK->publicKey, pubkey_data+(sizeof(char)*4), (2 * sizeof(char))+(element_size*sizeof(char)));
    }
    else
    {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
    XYResult* return_result = malloc(sizeof(XYResult));
    if(return_result){
      return_result->error = OK;
      return_result->result = return_NPK;
      return return_result;
    }
    else
    {
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
  }
  if(lookup_result->error == OK && lookup_result2->error == OK){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  else{
    RETURN_ERROR(ERR_BADDATA);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      NextPublicKey_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Next Public Key type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* NextPublicKey_creator_toBytes(struct XYObject* user_XYObject){
  if(user_XYObject->id[0] == 0x02 && user_XYObject->id[1] == 0x07){
    NextPublicKey* user_NPK = user_XYObject->GetPayload(user_XYObject);
    char id[2];
    memcpy(&id, user_NPK->id, 2);
    XYResult* lookup_result = lookup(id);
    if(lookup_result->error == OK){
      Object_Creator* NPK_creator = lookup_result->result;
      uint32_t element_size = 0;
      char* byteBuffer;

      if(NPK_creator->defaultSize != 0){
        element_size = NPK_creator->defaultSize;
        byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
        memcpy(byteBuffer, &id, 2);
        memcpy(byteBuffer+2, user_NPK->publicKey, 2*sizeof(char) + (element_size*sizeof(char)));
      }
      else if(NPK_creator->sizeIdentifierSize != 0)
      {
        char* casted_NPK = (char*)user_NPK->publicKey;
        switch(NPK_creator->sizeIdentifierSize){
          case 1:
            element_size = casted_NPK[0];
            byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+2, user_NPK->publicKey, 2*sizeof(char) + (element_size*sizeof(char)));
            break;
          case 2:
            element_size = to_uint16(&casted_NPK[0]);
            uint16_t encodedSize16 = element_size;
            int mallocNumber = 2*sizeof(char) + (element_size*sizeof(char));
            byteBuffer = malloc(mallocNumber);
            if(littleEndian()){
               encodedSize16 = to_uint16((char*)&element_size);
            }
            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize16, 2);
            memcpy(byteBuffer+(4*sizeof(char)), user_NPK->publicKey+2, (element_size-(sizeof(char)*2)));

            break;
          case 4:
            element_size = to_uint32(&casted_NPK[0]);
            uint32_t encodedSize32 = element_size;
            byteBuffer = malloc(2*sizeof(char) + (element_size*sizeof(char)));
            if(littleEndian()){
               encodedSize32 = to_uint32((char*)&element_size);
            }
            memcpy(byteBuffer, &id, 2);
            memcpy(byteBuffer+(2*sizeof(char)), &encodedSize32, 4);
            memcpy(byteBuffer+(6*sizeof(char)), user_NPK->publicKey+4, (element_size-(sizeof(char)*4)));
            break;
        }
      }
      else
      {
        RETURN_ERROR(ERR_CRITICAL);
      }
      XYResult* return_result = malloc(sizeof(XYResult));
      if(return_result){
        return_result->error = OK;
        return_result->result = byteBuffer;
        return return_result;
      }
      else
      {
        free(byteBuffer);
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
    }
    else
    {
      RETURN_ERROR(ERR_BADDATA);
    }
  }
  else
  {
    RETURN_ERROR(ERR_BADDATA);
  }
}
