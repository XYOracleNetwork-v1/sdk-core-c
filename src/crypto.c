/**
 ****************************************************************************************
 *
 * @file crypto.c
 *
 * @XY4 project source code.
 *
 * @brief primary crypto routines for the XY4 firmware.
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

#include "crypto.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *  NAME
 *      newInstance
 *
 *  DESCRIPTION
 *      this routine returns a NEW Signer 
 *
 *  PARAMETERS
 *      user_PrivateKey     [in]      ByteArray*
 *      hashingProvider     [in]      HashProvider* 
 *
 *  RETURNS
 *      signer              [out]     Signer*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a signer
 ****************************************************************************************
 */
Signer* newInstance(ByteArray_t* user_PrivateKey, HashProvider* hashingProvider){
  
  Signer* signer = malloc(sizeof(Signer));    //TODO: wal, where is this freed?
  
  if (signer) {
    
    signer->publicKey = NULL;
    signer->privateKey = NULL;                // Generate keypair and set these.
    signer->getPublicKey = &getPublicKey;
    signer->sign = NULL;
    signer->verify = NULL;
  }
  
  return signer;
}

/**
 ****************************************************************************************
 *  NAME
 *      newCryptoCreator
 *
 *  DESCRIPTION
 *      this routine returns a NEW CryptoCreator 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      creator     [out]     CryptoCreator*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a creator
 ****************************************************************************************
 */
CryptoCreator* newCryptoCreator(){
  
  CryptoCreator* creator = malloc(sizeof(CryptoCreator));   //TODO: wal, where is this freed?
  
  if (creator) {
    
    memset(creator->id, 0x00, 2);
    creator->newInstance = &newInstance;
    creator->getId = &cryptoGetId;
  }
  
  return creator;
}

/**
 ****************************************************************************************
 *  NAME
 *      cryptoGetId
 *
 *  DESCRIPTION
 *      this routine returns the id of the object supplied
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
char* cryptoGetId(CryptoCreator* object){
  
  return object->id;
}

/**
 ****************************************************************************************
 *  NAME
 *      getPublicKey
 *
 *  DESCRIPTION
 *      this routine returns the public key for the supplied signer
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create an XYResult
 ****************************************************************************************
 */
XYResult_t* getPublicKey(Signer* signer){
  
  XYResult_t* return_result = malloc(sizeof(XYResult_t));   //TODO: wal, where is this freed?
  
  if(return_result->error != OK) return return_result;
  
  return_result->error = OK;
  return_result->result = signer->publicKey;
  
  return return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      generateKeypair
 *
 *  DESCRIPTION
 *      this routine returns a NEW pair of public and private keys 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      keypair     [out]     keyPairStruct*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a keypair
 ****************************************************************************************
 */
keyPairStruct* generateKeypair(){
  
  keyPairStruct* keypair = malloc(sizeof(keyPairStruct));   //TODO: wal, where is this freed?
  
  if (keypair) {
    
    keypair->publicKey = NULL;
    keypair->privateKey = NULL;
  }
  
  return keypair;
}

// end of file crypto.c

