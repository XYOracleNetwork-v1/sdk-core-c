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
   /*
     if((*theirCatalog - BOUND_WITNESS_OPTION) & TAKE_ORIGIN_CHAIN_OPTION){
       return GIVE_ORIGIN_CHAIN_OPTION + BOUND_WITNESS_OPTION;
     }
   } else {
     if(*theirCatalog & BOUND_WITNESS_OPTION){
       return BOUND_WITNESS_OPTION;
     } else {
       return 0;
     }
   }
   return 0;
   */
}

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
  
  return result;
}

XYResult_t insertPublicKey(RelayNode_t* relay){
  DECLARE_RESULT();
  XYHeaderFlags_t typedFlags;
  typedFlags.lengthType = 2;
  typedFlags.iteratable = 1;
  typedFlags.typed = 1;

  XYHeaderFlags_t untypedFlags;
  untypedFlags.lengthType = 1;
  untypedFlags.iteratable = 1;
  untypedFlags.typed = 0;

  XYObject_t arrayObject;
  XYObjectHeader_t arrayHeader;
  arrayHeader.flags = typedFlags;
  arrayHeader.type = TYPE_ARRAY;
  arrayObject.header = &arrayHeader;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  
  XYObject_t innerArrayObject;
  XYObjectHeader_t innerArrayHeader;
  innerArrayHeader.flags = typedFlags;
  innerArrayHeader.type = TYPE_ARRAY;
  innerArrayObject.header = &innerArrayHeader;
  innerArrayObject.payload = itr.indexPtr;
  

  XYArrayItr_t innerItr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, itr.indexPtr);
  char writeBuffer[6+8+66] = { TYPED_ITERABLE, 2, 0, 0, 0, 0, *(uint8_t*)&typedFlags, 0x01, 0x00, 67+4, *(uint8_t*)&untypedFlags, 0x0d, 0x00, 67 };
  memset(writeBuffer + 8 + 6, 1, 66);

  uint32_t oldLength = XYObject_getFullLength(&arrayObject).value.ui;

  Iterator_insert(&innerArrayObject, 0, 67+5, oldLength, writeBuffer+8);
  XYObject_t* self = &arrayObject;
  XYOBJ_INCREMENT(67+5);
  socket_send(&relay->networkPipe, writeBuffer, sizeof(writeBuffer));
  breakpoint();
  return result;
}

XYResult_t insertPayloads(RelayNode_t* relay){
  DECLARE_RESULT();
  XYHeaderFlags_t typedFlags;
  typedFlags.lengthType = 2;
  typedFlags.iteratable = 1;
  typedFlags.typed = 1;

  XYHeaderFlags_t untypedFlags;
  untypedFlags.lengthType = 1;
  untypedFlags.iteratable = 1;
  untypedFlags.typed = 0;

  XYHeaderFlags_t untypedFlagsNoIteration;
  untypedFlags.lengthType = 1;
  untypedFlags.iteratable = 0;
  untypedFlags.typed = 0;

  XYObject_t arrayObject;
  XYObjectHeader_t arrayHeader;
  arrayHeader.flags = typedFlags;
  arrayHeader.type = TYPE_ARRAY;
  arrayObject.header = &arrayHeader;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  
  XYObject_t innerArrayObject =  IteratorNext(&itr);
  /*
  XYObjectHeader_t innerArrayHeader;
  innerArrayHeader.flags = typedFlags;
  innerArrayHeader.type = TYPE_ARRAY;
  innerArrayObject.header = &innerArrayHeader;
  innerArrayObject.payload = itr.indexPtr;
  */

  XYArrayItr_t innerItr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, itr.indexPtr);
  char writeBuffer[] = { TYPED_ITERABLE, 0x01, 0x00, 0x00, 0x00, 0x00, *(uint8_t*)&typedFlags, 0x08, 
                                0x00, 67+3, *(uint8_t*)&untypedFlags, 0x01, 0x00, 68, *(uint8_t*)&untypedFlagsNoIteration, 0x03,
                                0x00, 0x08, 0x00, 0x00, 0x00, 0x01, UNTYPED_ITERABLE, 0x01, 
                                0x00, 0x00, 0x00, 0x00};
  //memset(writeBuffer + 10 + 6, 2, (sizeof(writeBuffer)-16));
  breakpoint();
  Iterator_insert(&innerArrayObject, 0, (sizeof(writeBuffer)-6), XYObject_getFullLength(&arrayObject).value.ui, &writeBuffer+6);
  socket_send(&relay->networkPipe, writeBuffer, sizeof(writeBuffer));
  return result;
}

XYResult_t insertSignature(RelayNode_t* relay){
  DECLARE_RESULT();
  XYHeaderFlags_t typedFlags;
  typedFlags.lengthType = 2;
  typedFlags.iteratable = 1;
  typedFlags.typed = 1;

  XYHeaderFlags_t untypedFlags;
  untypedFlags.lengthType = 1;
  untypedFlags.iteratable = 1;
  untypedFlags.typed = 0;

  XYHeaderFlags_t untypedFlagsNoIteration;
  untypedFlags.lengthType = 1;
  untypedFlags.iteratable = 0;
  untypedFlags.typed = 0;

  XYObject_t arrayObject;
  arrayObject.header = (XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload;
  arrayObject.payload = relay->networkPipe.scratchBuffer.payload+2;

  void* endPtr;

  XYArrayItr_t itr = WeakArrayIterator((XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload, relay->networkPipe.scratchBuffer.payload+2);
  IteratorNext(&itr);
  IteratorNext(&itr);
  XYObject_t innerArrayObject;
  XYObjectHeader_t innerArrayHeader;
  innerArrayHeader.flags = typedFlags;
  innerArrayHeader.type = TYPE_ARRAY;
  innerArrayObject.header = &innerArrayHeader;
  innerArrayObject.payload = itr.indexPtr;
  

  XYArrayItr_t innerItr = WeakArrayIterator((XYObjectHeader_t*)relay->networkPipe.scratchBuffer.payload, itr.indexPtr);

  char writeBuffer[10+66] = { *(uint8_t*)&typedFlags, 0x08, 0x00, 67+3, *(uint8_t*)&untypedFlags, 0x01, 0x00, 68, NONITERABLE_TWOBYTE, 0x04 };
  memset(writeBuffer + 10, 3, 66);
  Iterator_insert(&innerArrayObject, 0, 67+5, XYObject_getFullLength(&arrayObject).value.ui, &writeBuffer);

  return result;
}
