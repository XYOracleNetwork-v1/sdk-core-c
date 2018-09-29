/**
 ****************************************************************************************
 *
 * @file hash.c
 *
 * @XY4 project source code.
 *
 * @brief primary hashing routines for the XY4 firmware.
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

#include "hash.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *  NAME
 *      getHashId
 *
 *  DESCRIPTION
 *      this routine returns the id of the supplied HashProvider object  
 *
 *  PARAMETERS
 *      hashProviderObject  [in]      HashProvider*
 *
 *  RETURNS
 *      id                  [out]     char*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
char* getHashId(HashProvider* hashProviderObject){
  
  return hashProviderObject->id;
}

/**
 ****************************************************************************************
 *  NAME
 *      newHashProvider
 *
 *  DESCRIPTION
 *      this routine returns the id of the object supplied 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      hasher      [out]     char*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a hasher
 ****************************************************************************************
 */
HashProvider* newHashProvider(){
  
  HashProvider* hasher = malloc(sizeof(HashProvider));    //TODO: wal, where is this freed?
  
  if (hasher) {
    
    hasher->hash = NULL;
    hasher->verifyHash = NULL;
    hasher->getHashId = &getHashId;
    hasher->id[0] = 0x00;
    hasher->id[1] = 0x00;
  }
  
  return hasher;
}

