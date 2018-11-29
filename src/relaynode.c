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
 *      object      [in]      CryptoCreator*
 *
 *  RETURNS
 *      id          [out]     char*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
 XYResult_t doConnection(RelayNode_t* self){

  
  //if(connectionToOtherPartyFrom.scratchBuffer == NULL){
    /*
    XYResult_t whatTheOtherPartyWantsToDo_result = connectionToOtherPartyFrom->peer->getRole(connectionToOtherPartyFrom);
    if(whatTheOtherPartyWantsToDo_result->error != OK){
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE); //TODO: close the pipe?!?
    }
    ByteArray_t* whatTheOtherPartyWantsToDo = whatTheOtherPartyWantsToDo_result->result;
    if(canDo(whatTheOtherPartyWantsToDo)){
      return doBoundWitness(self->node, NULL, connectionToOtherPartyFrom);;
    } else {
      connectionToOtherPartyFrom->close(connectionToOtherPartyFrom);
      RETURN_ERROR(ERR_PEER_INCOMPATABLE);
    }
    */
  //} else {
  //return doBoundWitness(self->node, connectionToOtherPartyFrom.scratchBuffer, connectionToOtherPartyFrom);;
  //}
  //XYResult_t result = completeBoundWitness(&self->node, &self->networkPipe.scratchBuffer, self->node.NetworkPipe);
  DECLARE_RESULT();
  result = insertPublicKey(self);
  insertPayloads(self);
  
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
  char writeBuffer[8+66] = { *(uint8_t*)&typedFlags, 0x01, 0x00, 67+3, *(uint8_t*)&untypedFlags, 0x0d, 0x00, 67 };
  memset(writeBuffer + 7, 1, 66);

  uint32_t oldLength = XYObject_getFullLength(&arrayObject).value.ui;

  Iterator_insert(&innerArrayObject, 0, 67+5, oldLength, &writeBuffer);
  XYObject_t* self = &arrayObject;
  XYOBJ_INCREMENT(67+5);
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
  IteratorNext(&itr);
  XYObject_t innerArrayObject;
  XYObjectHeader_t innerArrayHeader;
  innerArrayHeader.flags = typedFlags;
  innerArrayHeader.type = TYPE_ARRAY;
  innerArrayObject.header = &innerArrayHeader;
  innerArrayObject.payload = itr.indexPtr;
  

  XYArrayItr_t innerItr = WeakArrayIterator(relay->networkPipe.scratchBuffer.payload, itr.indexPtr);
  char writeBuffer[8+66] = { *(uint8_t*)&typedFlags, 0x08, 0x00, 67+3, *(uint8_t*)&untypedFlags, 0x01, 0x00, 68, *(uint8_t*)&untypedFlagsNoIteration, 0x03,  };
  memset(writeBuffer + 7, 2, 66);
  Iterator_insert(&innerArrayObject, 0, 67+5, XYObject_getFullLength(&arrayObject).value.ui, &writeBuffer);
  breakpoint();
  return result;
}

