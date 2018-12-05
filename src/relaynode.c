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
}


/**
 ****************************************************************************************
 *  NAME
 *      getInitBoundWitness
 *
 *  DESCRIPTION
 *  Returns valid Bound Witnessness initialization data
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

 XYResult_t getInitBoundWitness( void ){
  DECLARE_RESULT();
  result.status = XY_STATUS_OK;
  result.value.ptr = fetter;
  return result;
   
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
uint8_t fetter[]= { 4, 0, 0, 0, 1, // Catalog
TWO_ITERABLE_UNTYPED, 0x2, 0x00, 42, //BW 27 + 13
ONE_ITERABLE_UNTYPED, 0x16, 25,  // Fetter Set 24 bytes
ONE_ITERABLE_UNTYPED, 0x15, 22, // Fetter
ONE_ITERABLE_UNTYPED, 0x19, 5, // KeySet
ONE_NONITERABLE_UNTYPED, 0x0E, 2, 0, // Stub Public Key
ONE_ITERABLE_UNTYPED, 0x08, 12, ONE_NONITERABLE_UNTYPED, 0x0F, 9, 0, 0, 0, 0, 0, 0, 0, 0, // Stub Hash
ONE_ITERABLE_UNTYPED, 0x18, 11, //Witness Set
ONE_ITERABLE_UNTYPED, 0x17, 8, // Witness
ONE_ITERABLE_UNTYPED, 0x1A, 5, // Signature Set
ONE_NONITERABLE_UNTYPED, 0x0B, 2, 0 }; // Stub Signature

 XYResult_t doConnection(RelayNode_t* self, uint32_t offset){
  DECLARE_RESULT()

  if(offset>0){ // client
    XYObject_t fetterSet = { (XYObjectHeader_t*)self->networkPipe.scratchBuffer.payload, self->networkPipe.scratchBuffer.payload+2 };
    XYObject_t fetterObject = {(XYObjectHeader_t*)&fetter[5], (XYObjectHeader_t*)&fetter[7]};
    //uint32_t length = XYObject_getLength(&fetterSet).value.ui;
    WeakArray_add(&fetterSet, &fetterObject , sizeof(fetter)-7);

    socket_send(&self->networkPipe, (char*)&fetter[5], sizeof(fetter)-5, 1);
  } else { // server
    socket_send(&self->networkPipe, (char*)fetter, sizeof(fetter), 1);
    char recvBuffer[1024];
    socket_recv(&self->networkPipe, recvBuffer, 4);
    
    socket_recv(&self->networkPipe, recvBuffer+4, to_uint32((unsigned char*)recvBuffer) );
  }
  XYObject_t globalBuffer = { (XYObjectHeader_t*)self->networkPipe.scratchBuffer.payload, self->networkPipe.scratchBuffer.payload+2 };
  uint32_t newOffset = XYObject_getLength(&globalBuffer).value.ui;
  socket_recv(&self->networkPipe, self->networkPipe.scratchBuffer.payload+newOffset, 4);
  uint32_t packetSize = to_uint32(((unsigned char*)self->networkPipe.scratchBuffer.payload)+newOffset);
  
  socket_recv(&self->networkPipe, self->networkPipe.scratchBuffer.payload+newOffset, packetSize);
  return result;
}
