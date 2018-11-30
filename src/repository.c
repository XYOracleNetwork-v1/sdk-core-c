/**
 ****************************************************************************************
 *
 * @file repository.c
 *
 * @XYO Core library source code.
 *
 * @brief primary XYO block i/o and storage management routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
 
#include "C:\xycorp\xy4_firmware\sdk\sdk-core-c\src\include\state\repository.h"
#include "state.h"
#include "ZigZagBoundWitnessSession.h"
#include "spi_flash_generic.h"
#include <stdint.h>
#include "user_xy_flash.h"
#include "spi_flash.h"

#define XYO_BLOCK_SIZE_IN_BYTES_1024 1024
#define XYO_FLASH_STARTING_BLOCK_OFFSET_ADDRESS 0x000400 + 0X008000 + 0X018000  // 1024 + (32*1024) + (96*1024)

/*----------------------------------------------------------------------------*
* NAME
*     initOriginChainProvider
*
* DESCRIPTION
*     Initializes an origin chain provider as an Int Strong Array
*
*  PARAMETERS
*     none
*
* RETURNS
*     provider     [out]      OriginChainProvider_t*
*
* NOTES:
*     
*----------------------------------------------------------------------------*/
OriginChainProvider_t* initOriginChainProvider() {
  
  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  
  if(lookup_result->error != OK) { return NULL; }
  
  ObjectProvider_t* ISA_Creator = lookup_result->result;
  //free(lookup_result);

  XYResult_t* create_result = ISA_Creator->create((char*)IntStrongArray_id, NULL);
  
  if(create_result->error != OK){ return NULL; }

  OriginChainProvider_t* provider = malloc(sizeof(OriginChainProvider_t));
  
  if(provider){
    
    provider->repository = create_result->result;
    //free(create_result);
    provider->logicalEnd = 0;
    provider->appendToOriginChainInFlash = appendToOriginChainInFlash;
    provider->getTheOriginChainFromFlash = getTheOriginChainFromFlash;
    provider->deleteTheOriginChainFromFlash = deleteTheOriginChainFromFlash;
    
    return provider;
    
  } else {
    return NULL;
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      readXyoBlockFromFlash
*
*  DESCRIPTION
*      read XYO block(s) from flash.
*
*  PARAMETERS
*     blockNumber                       [in]    uint32_t    block # to read from flash.
*     NumberOfBlocksToRead              [in]    uint32_t    when multiple blocks are read, 
*                                                           they are read sequentially 
*                                                           after the blockNumber requested.
*     dataBuffer                        [in]    uint8_t*    destination buffer for the XYO block(s) read.
*
*  RETURNS
*      preallocated_return_result_ptr   [out]   XYResult_t*
*
*  NOTES:
*
*
 ****************************************************************************************
 * @brief Read data from a given starting address (up to the end of the flash).
 * @param[in] *rd_data_ptr  Points to the position the read data will be stored
 * @param[in] address       Starting flash address of data to be read
 * @param[in] size          Size of the data to be read
 * @return Number of read bytes or error code
 ****************************************************************************************
 *
//ref, int32_t spi_flash_read_data(uint8_t *rd_data_ptr, uint32_t address, uint32_t size);
*----------------------------------------------------------------------------*/
XYResult_t* readXyoBlockFromFlash(uint32_t blockNumber,
                                  uint32_t NumberOfBlocksToRead, 
                                  uint8_t* destinationReadBuffer) {
  int32_t error32 = OK;
                                    
  error32 = spi_flash_read_data(destinationReadBuffer, 
                                FLASH_USER_BASE_ADDRESS + FLASH_BLOCK_0_OFFSET_ADDRESS + 
                                    (blockNumber * XYO_BLOCK_SIZE_IN_BYTES_1024),       // flash read address
                                NumberOfBlocksToRead * XYO_BLOCK_SIZE_IN_BYTES_1024);

  if (error32 != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};    

  preallocated_return_result_ptr->error = error32;
  preallocated_return_result_ptr->result = 0;

  return preallocated_return_result_ptr;
}

/*----------------------------------------------------------------------------*
*  NAME
*      writeXyoBlockToFlash
*
*  DESCRIPTION
*      write XYO block(s) to flash. use XYO_BLOCK_SIZE_IN_BYTES_1024 to calc data length.
*
*  PARAMETERS
*     blockNumber                       [in]    uint32_t    block # to save to flash.
*     NumberOfBlocksToWrite             [in]    uint32_t    these are written sequentially
*                                                           after the starting block.
*     dataBuffer                        [in]    uint8_t*    data source of the 
*                                                           XYO block(s) to store.
*  RETURNS
*      preallocated_return_result_ptr   [out]   XYResult_t* 
*
*  NOTES:
*
//ref, int8_t spi_flash_block_erase(uint32_t address, SPI_erase_module_t spiEraseModule)
//ref, int32_t spi_flash_page_program(uint8_t *wr_data_ptr, uint32_t address, uint16_t size)
*----------------------------------------------------------------------------*/
XYResult_t* writeXyoBlockToFlash( uint32_t blockNumber, 
                                  uint32_t NumberOfBlocksToWrite, 
                                  uint8_t* dataBuffer) {
  int8_t    error8 = OK;
  int32_t   error32 = OK;
  
  error8 = spi_flash_block_erase(FLASH_USER_BASE_ADDRESS + XYO_FLASH_STARTING_BLOCK_OFFSET_ADDRESS, 
                                 SECTOR_ERASE);

  if (error8 != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};    

  error32 = spi_flash_page_program(dataBuffer, 
                                   FLASH_USER_BASE_ADDRESS + FLASH_BLOCK_0_OFFSET_ADDRESS + 
                                      (blockNumber * XYO_BLOCK_SIZE_IN_BYTES_1024), 
                                   (NumberOfBlocksToWrite * XYO_BLOCK_SIZE_IN_BYTES_1024));

  if (error32 != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};    

  error8 = spi_flash_set_write_disable();   // prevent writing to flash

  if (error8 != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};    

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = 0;

  return preallocated_return_result_ptr;
}

/*----------------------------------------------------------------------------*
*  NAME
*      write
*
*  DESCRIPTION
*      Write block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
/*
XYResult_t* write(OriginChainProvider_t* self, ByteArray_t* value, uint16_t offset, uint16_t timeout) {

  // First we check if Storage Provider has been initialized
  if( self->repository == NULL ){
  
    XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
    if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    ObjectProvider_t* ISA_Creator = lookup_result->result;

    XYResult_t* create_result = ISA_Creator->create(IntStrongArray_id, NULL);
    if(create_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

    IntStrongArray_t* repo = create_result->result;
    //free(lookup_result);
    lookup_result = tableLookup(BoundWitness_id);
    if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL);}
    char* boundWitnessBytes = value->payload;
    ObjectProvider_t* boundWitness_Creator = lookup_result->result;
    XYResult_t* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
    if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

    BoundWitness_t* boundWitness_raw = fromBytes_result->result;
    XYResult_t* newObject_result = newObject(BoundWitness_id, boundWitness_raw);
    if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    self->logicalEnd = 1;
    
    return repo->add(repo, newObject_result->result);
    
  } else {
    for(int i = 0; i<=offset; i++){
      XYResult_t* lookup_result = tableLookup((const char*)&self->repository[i]);
      if(lookup_result->error == OK){
        continue;
      } else {
        lookup_result = tableLookup(BoundWitness_id);

        if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

        char* boundWitnessBytes = value->payload;
        ObjectProvider_t* boundWitness_Creator = lookup_result->result;

        XYResult_t* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
        if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

        BoundWitness_t* boundWitness_raw = fromBytes_result->result;

        XYResult_t* newObject_result = newObject(BoundWitness_id, boundWitness_raw);
        if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
        self->logicalEnd = i+1;
        
        return ((IntStrongArray_t*)self->repository)->add((IntStrongArray_t*)self->repository, 
                newObject_result->result);
      }
    }
    RETURN_ERROR(ERR_CRITICAL);
  }
}
*/

/*----------------------------------------------------------------------------*
*  NAME
*      appendToOriginChainInFlash
*
*  DESCRIPTION
*      Append the supplied block to the origin chain in flash
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self_OriginChainNavigator*
*     *ByteArray                [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult_t* appendToOriginChainInFlash(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout){
  
  if(timeout){ /* unimplemented */ }
  
  XYResult_t* lookup_result = tableLookup(BoundWitness_id);

  ObjectProvider_t* BoundWitness_Creator = lookup_result->result;
  //free(lookup_result);

  XYResult_t* fromBytes_result = BoundWitness_Creator->fromBytes(value->payload);
  
  if(fromBytes_result->error != OK) {return fromBytes_result;}
  
  BoundWitness_t* boundWitness = fromBytes_result->result;
  //XYObject_t* boundWitnessObject = fromBytes_result->result;
  XYResult_t* newObject_result = newObject((const char*)&BoundWitness_id, boundWitness);
  XYObject_t* boundWitnessObject = newObject_result->result;
  //boundWitness->size = to_uint32((unsigned char*)(uintptr_t)&boundWitness->size);
  self->logicalEnd = self->logicalEnd +1;
  
  return ((IntStrongArray_t*)self->repository)->add(self->repository, boundWitnessObject);
}

/*----------------------------------------------------------------------------*
*  NAME
*      getTheOriginChainFromFlash
*
*  DESCRIPTION
*      Get the location of the origin chain on disk and return the array object.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self*
*
*  RETURNS
*      XYResult_t*              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* getTheOriginChainFromFlash(OriginChainProvider_t* self){
  
  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  
  ObjectProvider_t* ISA_Creator = lookup_result->result;

  XYResult_t* newObject_result = newObject(IntStrongArray_id, self->repository);
  
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  XYResult_t* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
  
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  //free(lookup_result);
  free(newObject_result);

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
  if(return_result){
    
    return_result->error = OK;
    return_result->result = toBytes_result->result;
    free(toBytes_result);
    
    return return_result;
    
  } else {
    free(toBytes_result);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      deleteTheOriginChainFromFlash
*
*  DESCRIPTION
*      Nullify and erase the origin chain.
*
*  PARAMETERS
*     *OriginChainNavigator     [in]       self*
*
*  RETURNS
*      XYResult_t*              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* deleteTheOriginChainFromFlash(OriginChainProvider_t* self) {
  /* format old chain */

  /* KNOWN MEMORY LEAK. TODO: IMPLEMENT ARRAY DECONSTRUCTOR */
  free(((IntStrongArray_t*)self->repository)->payload);

  /* Create new IntStrongArray */

  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  ObjectProvider_t* ISA_Creator = lookup_result->result;
  XYResult_t* create_result = ISA_Creator->create((char*)IntStrongArray_id, NULL);
  
  if(create_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  
  IntStrongArray_t* repo = create_result->result;
  self->repository = repo;
  self->logicalEnd = 0;

  //free(lookup_result);
  free(create_result);

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    
    return return_result;
    
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

// end of file repository.c

