/**
 ****************************************************************************************
 *
 * @file NodeBase.c
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Persistent Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include "relaynode.h"

// Arie's Mom

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
uint8_t fragment[]= { 4, 0, 0, 0, 1, // Catalog
TWO_ITERABLE_UNTYPED, 0x1b, 0x00, 47, // Fragment
ONE_ITERABLE_UNTYPED, 0x15, 29, // Fetter
ONE_ITERABLE_UNTYPED, 0x19, 5, // KeySet
ONE_NONITERABLE_UNTYPED, 0x0E, 2, 0, // Stub Public Key
ONE_ITERABLE_UNTYPED, 0x08, 12, ONE_NONITERABLE_UNTYPED, 0x0F, 9, 0, 0, 0, 0, 0, 0, 0, 0, // Stub Hash
ONE_NONITERABLE_UNTYPED, 0x03, 5, 0, 0, 0, 0,
ONE_ITERABLE_UNTYPED, 0x17, 12, // Witness
ONE_ITERABLE_UNTYPED, 0x1A, 5, // Signature Set
ONE_NONITERABLE_UNTYPED, 0x0B, 2, 255, // Stub Signature
ONE_NONITERABLE_UNTYPED, 0x13, 2, 1}; // Stub rssi

uint8_t fetter[] = {
4, 0, 0, 0, 1,
ONE_ITERABLE_UNTYPED, 0x16, 32, // FetterSet
ONE_ITERABLE_UNTYPED, 0x15, 29, // Fetter
ONE_ITERABLE_UNTYPED, 0x19, 5, // KeySet
ONE_NONITERABLE_UNTYPED, 0x0E, 2, 0, // Stub Public Key
ONE_ITERABLE_UNTYPED, 0x08, 12, ONE_NONITERABLE_UNTYPED, 0x0F, 9, 0, 0, 0, 0, 0, 0, 0, 0,
ONE_NONITERABLE_UNTYPED, 0x03, 5, 0, 0, 0, 0};

uint8_t witness[] = {
TWO_ITERABLE_UNTYPED, 0x18, 0x00, 16, // WitnesSet
ONE_ITERABLE_UNTYPED, 0x17, 12, // Witness
ONE_ITERABLE_UNTYPED, 0x1A, 5, // Signature Set
ONE_NONITERABLE_UNTYPED, 0x0B, 2, 255, // Stub Signature
ONE_NONITERABLE_UNTYPED, 0x13, 2, 1};

 XYResult_t doConnection(RelayNode_t* self, uint32_t offset){
  DECLARE_RESULT()

  if(offset>0){ // client
    /* 
     * The server has sent up a fragment containing one fetter
     * We appened our Fetter to the partially formed fragment below. 
     */
    XYObject_t fetterSet = { (XYObjectHeader_t*)self->networkPipe.scratchBuffer.payload, self->networkPipe.scratchBuffer.payload+2 };
    XYObject_t fetterObject = {(XYObjectHeader_t*)&fragment[9], (XYObjectHeader_t*)&fragment[11]};
    uint32_t length = XYObject_getFullLength(&fetterObject).value.ui;
    XYResult_t add_result = WeakArray_add(&fetterSet, &fetterObject , length);
    CHECK_RESULT(add_result);

    /* Directly after we appened our Witness */
    XYObject_t boundWitnessFragment = { (XYObjectHeader_t*)self->networkPipe.scratchBuffer.payload, self->networkPipe.scratchBuffer.payload+2 };
    fetterObject = (XYObject_t){(XYObjectHeader_t*)&fragment[40], (XYObjectHeader_t*)&fragment[42]};
    length = XYObject_getFullLength(&fetterObject).value.ui;
    add_result = WeakArray_add(&boundWitnessFragment, &fetterObject , length);
    CHECK_RESULT(add_result);

    /* send and check result */
    XYResult_t send_result = socket_send(&self->networkPipe, (char*)&fragment[5], sizeof(fragment)-5);
    CHECK_RESULT(send_result);

    /* Waiting on signatures */
    //char recvBuffer[1024];
    //socket_recv(&self->networkPipe, recvBuffer, 1024);
    printf("New Bound Witness!\n");
    socket_close(&self->networkPipe);

  } else { // server
    /*
     * This memcpy is out of place. We are trying to move our fetter
     * into the slot 0 in the (Afetter, Bfetter, Bwitness, Awitness)
     * configuration. To do this we must dynamically choose which 
     * buffer to use earlier in execution. For simplicity sake I 
     * will keep this as a memcpy until we are more stable. 
     */ 

    uint8_t fragment_header[] = { TWO_ITERABLE_UNTYPED, 0x01b, 0x00, 0x02 };
    memcpy(self->networkPipe.scratchBuffer.payload, fragment_header, 4);

    /* Send Our fetter over to the client */
    XYObject_t fetterObject = (XYObject_t){(XYObjectHeader_t*)&fetter[8], (XYObjectHeader_t*)&fetter[10]};
    XYObject_t fragmentObject = (XYObject_t){(XYObjectHeader_t*)self->networkPipe.scratchBuffer.payload, (char*)self->networkPipe.scratchBuffer.payload+2};
    uint32_t ourFetterLength = XYObject_getFullLength(&fetterObject).value.ui;
    XYResult_t send_result = socket_send(&self->networkPipe, (char*)fetter, sizeof(fetter));
    CHECK_RESULT(send_result);

    /* 
     * Begin parsing the client response.
     * 8 is max length of size, and we will need to two Object Headers.
     * Considering we are the server, at this point in the execution flow
     * it is assumed that have only sent the client our catalog buffer at
     * this point. We should expect to see a client Fetter sitting in the
     * recv buffer.  
     * (Fragment Header + Fetter Set Header)
     */
    uint8_t trashCanBuffer[TCP_WRAPPER_SIZE+(sizeof(XYObjectHeader_t)+8)*2]; 
    send_result = socket_recv(&self->networkPipe, (char*)&trashCanBuffer, TCP_WRAPPER_SIZE+sizeof(XYObjectHeader_t)); // Fragment Header
    CHECK_RESULT(send_result);

    /* Get Fetter set+size and not a byte more */
    XYObjectHeader_t* tempHeader;
    tempHeader = (XYObjectHeader_t*)((char*)trashCanBuffer+TCP_WRAPPER_SIZE);
    uint32_t fetterSetSizeLength = lengthTypeToLength(tempHeader->flags.lengthType);
    socket_recv(&self->networkPipe, (char*)&trashCanBuffer+TCP_WRAPPER_SIZE+sizeof(XYObjectHeader_t), fetterSetSizeLength); //


    /*
     * Set up object to help us get the size of our fetter so 
     * we can ultimately add it to the in progress fragment.
     * TODO: investigate using sizeof
     */
    void* headerPtr = (char*)trashCanBuffer+TCP_WRAPPER_SIZE;
    void* bodyPtr = (char*)trashCanBuffer+TCP_WRAPPER_SIZE+2;
    XYObject_t trashedObject = (XYObject_t){(XYObjectHeader_t*)headerPtr, (char*)bodyPtr};
    WeakArray_add(&fragmentObject, &fetterObject , ourFetterLength); // Add our fetter to the bound witness.

    /* Redundant: Get Fetter length, appened fetter, increment fragment size */
    unsigned char* endPtr = self->networkPipe.scratchBuffer.payload + XYObject_getFullLength(&fragmentObject).value.ui;
    socket_recv(&self->networkPipe, (char*)endPtr, 2);

    tempHeader = (void*)endPtr;
    uint32_t fetterSizeLength = lengthTypeToLength(tempHeader->flags.lengthType);
    socket_recv(&self->networkPipe, ((char*)endPtr)+2, fetterSizeLength);

    headerPtr = endPtr;
    bodyPtr = endPtr+2;
    trashedObject = (XYObject_t){(XYObjectHeader_t*)headerPtr, (char*)bodyPtr};
    
    XYResult_t fetterLength_result = XYObject_getLength(&trashedObject);
    CHECK_RESULT(fetterLength_result);

    XYResult_t recv_result = socket_recv(&self->networkPipe, (char*)endPtr+(sizeof(char)*2)+fetterSizeLength, fetterLength_result.value.ui-fetterSizeLength);
    CHECK_RESULT(recv_result);
    XYARRAY_INCREMENT(&fragmentObject, (fetterLength_result.value.ui+sizeof(XYObjectHeader_t)));


    /* Redundant: Get Witness length, appened Witness, increment fragment size */
    endPtr = (unsigned char*)endPtr+(sizeof(char)*2)+fetterSizeLength+fetterLength_result.value.ui - fetterSizeLength + 1;
    socket_recv(&self->networkPipe, (char*)endPtr, 2);
    
    tempHeader = (void*)endPtr;
    uint32_t witnessSizeLength = lengthTypeToLength(tempHeader->flags.lengthType);
    socket_recv(&self->networkPipe, ((char*)endPtr)+2, witnessSizeLength);

    headerPtr = endPtr;
    bodyPtr = endPtr+2;
    trashedObject = (XYObject_t){(XYObjectHeader_t*)headerPtr, (char*)bodyPtr};
    
    XYResult_t witnessLength_result = XYObject_getLength(&trashedObject);
    CHECK_RESULT(witnessLength_result);

    recv_result = socket_recv(&self->networkPipe, (char*)endPtr+(sizeof(char)*2)+witnessSizeLength, witnessLength_result.value.ui-witnessSizeLength);
    CHECK_RESULT(recv_result);
    XYARRAY_INCREMENT(&fragmentObject, (witnessLength_result.value.ui+sizeof(XYObjectHeader_t)));
    /* Create and Append signature */
    Signer_t* stubSigner = NULL;
    ByteArray_t signBuffer;
    signBuffer.size = XYObject_getFullLength(&fragmentObject).value.ui - sizeof(XYObjectHeader_t) - lengthTypeToLength(fragmentObject.header->flags.lengthType);
    signBuffer.payload = (unsigned char*)self->networkPipe.scratchBuffer.payload+sizeof(XYObjectHeader_t) + lengthTypeToLength(fragmentObject.header->flags.lengthType);
    sign(stubSigner, ((char*)witness)+13, &signBuffer);
    socket_send(&self->networkPipe, (char*)witness, sizeof(witness));
    trashedObject = (XYObject_t){(XYObjectHeader_t*)((char*)witness+4), (char*)witness+6};
    WeakArray_add(&fragmentObject, &trashedObject,  XYObject_getFullLength(&trashedObject).value.ui); // Minus 2 char for TWO_ITERABLE_UNTYPED

    printf("New Bound Witness!\n");
    socket_close(&self->networkPipe);
  }

  result.status = XY_STATUS_OK;
  result.value.ui = 0;
  return result;
}
