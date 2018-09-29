/**
 ****************************************************************************************
 *
 * @file storage.c
 *
 * @XY4 project source code.
 *
 * @brief primary storage routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
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
 *      store     [out]     StorageProvider*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a storage provider
 ****************************************************************************************
 */
 StorageProvider* newStorageProvider(){
  
  StorageProvider* store = malloc(sizeof(struct StorageProvider));
   
  //TODO: wal, where's the malloc test?
  store->write = NULL;
  store->read = NULL;
  store->GetAllKeys = NULL;
  store->delete = NULL;
  store->contains = NULL;
   
  return store;
}

// end of file storage.c

