#include "xyobject.h"
#include <stdlib.h>
#include <string.h>
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include <arpa/inet.h>

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_create
*
*  DESCRIPTION
*      Create an empty Bound Witness Object
*
*  PARAMETERS
*     *id                    [in]       char*
*     *user_data             [in]       void*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYObject* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_create(char id[2], void* user_data){
  return newObject(id, user_data);
}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_fromBytes
*
*  DESCRIPTION
*      Create an Bound Witness object given a set of Bytes. Bytes must not
*      include major and minor of array.
*
*  PARAMETERS
*     *data                  [in]       char*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns XYResult* of the BoundWitness type.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_fromBytes(char* data){
  BoundWitness* return_BoundWitness = malloc(sizeof(BoundWitness));
  uint32_t BoundWitnessSize = to_uint32(data);
  return_BoundWitness->size = BoundWitnessSize;
  ShortStrongArray* publicKeysPtr;
  IntStrongArray* payloadPtr;
  ShortStrongArray* signaturesPtr;

  /*
   * We process each of the elements in a Bound Witness
   * in separate chunks here and assign all the pointers
   * at the end. First is publicKeys.
   */
  char ShortStrongArray_id[2] = { 0x01, 0x02 };
  uint32_t publicKeysSize = to_uint32(&data[4]);
  XYResult* lookup_result = lookup(ShortStrongArray_id);
  if(lookup_result->error == OK){
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      publicKeysPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
    return lookup_result;
  }

  // Payload
  uint32_t payloadSize = to_uint32((char*)&data[4+publicKeysSize*sizeof(char)]);
  char IntStrongArray_id[2] = { 0x01, 0x03 };
 //free(lookup_result);
  lookup_result = lookup(IntStrongArray_id);
  if(lookup_result->error == OK){
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize*sizeof(char)]);

    if(fromBytes_result->error == OK)
    {
      payloadPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
    return lookup_result;
  }

  // Signatures
 //free(lookup_result);
  lookup_result = lookup(ShortStrongArray_id);
  if(lookup_result->error == OK){
    Object_Creator* SWA_Creator = lookup_result->result;
    XYResult* fromBytes_result = SWA_Creator->fromBytes((char*)&data[4+publicKeysSize+payloadSize*sizeof(char)]);
    if(fromBytes_result->error == OK)
    {
      signaturesPtr = fromBytes_result->result;
     //free(fromBytes_result);
    } else {
      return fromBytes_result;
    }
  } else {
    return lookup_result;
  }
  return_BoundWitness->size = BoundWitnessSize;
  return_BoundWitness->publicKeys = publicKeysPtr;
  return_BoundWitness->payload = payloadPtr;
  return_BoundWitness->signatures = signaturesPtr;
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = return_BoundWitness;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}

/*----------------------------------------------------------------------------*
*  NAME
*      BoundWitness_creator_toBytes
*
*  DESCRIPTION
*      Given an XYObject of Bound Witness type this routine will serialize
*      the object and return a char* to the serialized bytes.
*
*  PARAMETERS
*    *user_XYObject         [in]       XYObject*
*
*  RETURNS
*      XYResult*            [out]      bool   Returns char* to serialized bytes.
*----------------------------------------------------------------------------*/
XYResult* BoundWitness_creator_toBytes(struct XYObject* user_XYObject){
  BoundWitness* user_BoundWitness = user_XYObject->payload;
  char ShortStrongArray_id[2] = { 0x01, 0x02 };
  char IntStrongArray_id[2] = { 0x01, 0x03 };
  XYResult* lookup_result = lookup(ShortStrongArray_id);
  Object_Creator* SSA_Creator = lookup_result->result;
 //free(lookup_result);
  lookup_result = lookup(IntStrongArray_id);
  Object_Creator* ISA_Creator = lookup_result->result;
  char* byteBuffer = malloc(user_BoundWitness->size*sizeof(char));
  if(littleEndian()){
    uint32_t encodedSize = to_uint32((char*)&user_BoundWitness->size);
    memcpy(byteBuffer, &encodedSize, sizeof(uint32_t));
  } else {
    memcpy(byteBuffer, &user_BoundWitness->size, sizeof(uint32_t));
  }

  ShortStrongArray* user_publicKeys = user_BoundWitness->publicKeys;
  uint16_t user_publicKeysSize = user_publicKeys->size;
  XYResult* newObject_result = newObject(ShortStrongArray_id, user_publicKeys);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
   //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t), toBytes_result->result, user_publicKeysSize);
     //free(toBytes_result);
    }
  }

  IntStrongArray* user_payload = user_BoundWitness->payload;
  uint32_t user_payloadSize = user_payload->size;
  newObject_result = newObject(IntStrongArray_id, user_payload);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize, toBytes_result->result, user_payloadSize);
     //free(toBytes_result);
    }
  }

  ShortStrongArray* user_signatures = user_BoundWitness->signatures;
  uint16_t user_signaturesSize = user_signatures->size;
  newObject_result = newObject(ShortStrongArray_id, user_signatures);
  if(newObject_result->error == OK){
    XYResult* toBytes_result = SSA_Creator->toBytes(newObject_result->result);
    //free(newObject_result);
    if(toBytes_result->error == OK){
      memcpy(byteBuffer+sizeof(uint32_t)+user_publicKeysSize+user_payloadSize, toBytes_result->result, user_signaturesSize);

     //free(toBytes_result);
    }
  }
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = byteBuffer;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

}
