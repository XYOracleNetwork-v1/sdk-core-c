/**
 ****************************************************************************************
 *
 * @file storage.c
 *
 * @XY4 project source code.
 *
 * @brief primary storage routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "storage.h"

/**
 ****************************************************************************************
 *  NAME
 *      newStorageProvider
 *
 *  DESCRIPTION
 *      this routine returns a NEW storage provider 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]      XYResult_t*
 *                                                    -----------------------
 *              preallocated_return_result_ptr->error = OK (error code)
 *              preallocated_return_result_ptr->result = aNewStorageProvider (StorageProvider_t*)
 *  NOTES
 *      will return a malloc error if there is insufficient memory to create a storage provider
 ****************************************************************************************
 */
 XYResult_t* newStorageProvider(){
  
  StorageProvider_t* aNewStorageProvider = malloc(sizeof(StorageProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(!aNewStorageProvider) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};
    
  aNewStorageProvider->write = NULL;
  aNewStorageProvider->read = NULL;
  aNewStorageProvider->GetAllKeys = NULL;
  aNewStorageProvider->delete = NULL;
  aNewStorageProvider->contains = NULL;
   
  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = aNewStorageProvider;   // a StorageProvider_t* 
  
  return preallocated_return_result_ptr;                          
}

// end of file storage.c

