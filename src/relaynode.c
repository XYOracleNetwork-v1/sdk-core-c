/**
 ****************************************************************************************
 *
 * @file NodeBase.c
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
#include "relaynode.h"

/**
 ****************************************************************************************
 *  NAME
 *      Relay_getChoice
 *
 *  DESCRIPTION
 *  Add a XYObject to the struct-hack
 *
 *  PARAMETERS
 *      object      [in]      CryptoCreator*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 uint8_t Relay_getChoice(uint8_t* theirCatalog){
   if(*theirCatalog - BOUND_WITNESS_OPTION != 0){
     return GIVE_ORIGIN_CHAIN_OPTION + BOUND_WITNESS_OPTION;
   } else {
     if(*theirCatalog & BOUND_WITNESS_OPTION){
       return BOUND_WITNESS_OPTION;
     } else {
       return 0;
     }
   }
   return 0;
}

#define typedFlags 176
#define untypedFlags 78
#define untypedFlagsNoIteration 190

char payloadBuffer[30] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, sizeof(payloadBuffer)-2, 176, 0x08, 
                              0x00, 0x00, 0x00, sizeof(payloadBuffer)-8, 78, 0x01, 0x00, sizeof(payloadBuffer)-10, 190, 0x03,
                              0x00, sizeof(payloadBuffer)-12, 0x00, 0x00, 0x00, 0x01, UNTYPED_ITERABLE, 0x01, 
                              0x00, 0x00, 0x00, 0x04};
char paydbg[] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, sizeof(payloadBuffer)-2, 176, 0x08, 
                              0x00, 0x00, 0x00, sizeof(payloadBuffer)-8, 78, 0x01, 0x00, sizeof(payloadBuffer)-10, 190, 0x03,
                              0x00, sizeof(payloadBuffer)-12, 0x00, 0x00, 0x00, 0x01, UNTYPED_ITERABLE, 0x01, 
                              0x00, 0x00, 0x00, 0x04};

char signatureBuffer[6+12+66] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, sizeof(signatureBuffer)-2, typedFlags, 0x08, 0x00, sizeof(signatureBuffer)-8, untypedFlags, 0x01, 0x00, 70, NONITERABLE_TWOBYTE, 0x04, 0x00, 68 };
char sigdbg[] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, sizeof(signatureBuffer)-2, typedFlags, 0x08, 0x00, sizeof(signatureBuffer)-8, untypedFlags, 0x01, 0x00, 70, NONITERABLE_TWOBYTE, 0x04, 0x00, 68 };

char pubkeyBuffer[6+8+66] = { TYPED_ITERABLE, 2, 0, 0, 0, sizeof(pubkeyBuffer)+sizeof(payloadBuffer)+sizeof(signatureBuffer), typedFlags, 0x01, 0x00, 68+4, untypedFlags, 0x0d, 0x00, 68 };
char pubdbg[] = { TYPED_ITERABLE, 2, 0, 0, 0, sizeof(pubkeyBuffer)+sizeof(payloadBuffer)+sizeof(signatureBuffer), typedFlags, 0x01, 0x00, 68+4, untypedFlags, 0x0d, 0x00, 68 };


/**
 ****************************************************************************************
 *  NAME
 *      doConnection
 *
 *  DESCRIPTION
 *  Add a XYObject to the struct-hack
 *
 *  PARAMETERS
 *      self        [in]      RelayNode_t*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 XYResult_t doConnection(RelayNode_t* self){

  DECLARE_RESULT();
  result = insertPublicKey(self);

  result = insertPayloads(self);

  result = insertSignature(self);
  to_uint32_be(globalBuffer, sizeof(pubkeyBuffer)+sizeof(payloadBuffer)+sizeof(signatureBuffer)+6);
  breakpoint();
  socket_send(&self->networkPipe, globalBuffer, sizeof(pubkeyBuffer)+sizeof(payloadBuffer)+sizeof(signatureBuffer)+6, 0);
  char funBuffer[500];
  int ret_code = socket_recv(&self->networkPipe, funBuffer, 65000).value.i;
  for(int i = 0; i < ret_code; i++){
    printf("%d", ret_code);
  }
  printf("\n");
  return result;
}

XYResult_t insertPublicKey(RelayNode_t* relay){
  DECLARE_RESULT();

  XYObject_t arrayObject;
  arrayObject.header = relay->networkPipe.scratchBuffer.payload;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  
  XYObject_t innerArrayObject;
  innerArrayObject.header = itr.header;
  innerArrayObject.payload = itr.indexPtr;
  //innerArrayObject = WeakArray_get(&itr, 0);
  

  XYArrayItr_t innerItr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, itr.indexPtr);
  
  memset(pubkeyBuffer + 8 + 6, 1, 66);

  uint32_t oldLength = XYObject_getFullLength(&arrayObject).value.ui;

  Iterator_insert(&innerArrayObject, 0, 67+5, oldLength, pubkeyBuffer+8);
  XYObject_t* self = &arrayObject;
  XYOBJ_INCREMENT(67+5);
  socket_send(&relay->networkPipe, pubkeyBuffer, sizeof(pubkeyBuffer), 1);
  return result;
}

XYResult_t insertPayloads(RelayNode_t* relay){
  DECLARE_RESULT();

  XYObject_t arrayObject;
  arrayObject.header = relay->networkPipe.scratchBuffer.payload;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  
  XYObject_t innerArrayObject =  IteratorNext(&itr);

  XYArrayItr_t innerItr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, itr.indexPtr);

  Iterator_insert(&innerArrayObject, 0, (sizeof(payloadBuffer)-6), XYObject_getFullLength(&arrayObject).value.ui, payloadBuffer+6);
  socket_send(&relay->networkPipe, &payloadBuffer, sizeof(payloadBuffer), 1);
  return result;
}


XYResult_t insertSignature(RelayNode_t* relay){
  DECLARE_RESULT();

  XYObject_t arrayObject;
  arrayObject.header = (XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator((XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  IteratorNext(&itr);
  
  XYObject_t innerArrayObject = IteratorNext(&itr);
  //XYObjectHeader_t innerArrayHeader;
  //innerArrayHeader.flags = typedFlags;
  //innerArrayHeader.type = TYPE_ARRAY;
  //innerArrayObject.header = &innerArrayHeader;
  //innerArrayObject.payload = itr.indexPtr;
  

  XYArrayItr_t innerItr = WeakArrayIterator((XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload, itr.indexPtr);

  //char writeBuffer[6+10+66] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, 80, *(uint8_t*)&typedFlags, 0x08, 0x00, 74, *(uint8_t*)&untypedFlags, 0x01, 0x00, 70, NONITERABLE_TWOBYTE, 0x04 };
  memset(signatureBuffer + 12 + 6, 3, 66);
  Iterator_insert(&innerArrayObject, 0, 67+5, XYObject_getFullLength(&arrayObject).value.ui, signatureBuffer+6);
  socket_send(&relay->networkPipe, &signatureBuffer, sizeof(signatureBuffer), 1);

  return result;
}
