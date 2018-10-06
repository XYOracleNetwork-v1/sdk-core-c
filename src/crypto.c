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

#include <stdlib.h>
#include "crypto.h"         // includes "xyobject.h", "hash.h"
#include "wc_rsa.h"         // C:\xycorp\xy4_firmware\src

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *  NAME
 *      newPrivateKey
 *
 *  DESCRIPTION
 *      this routine creates and returns a new private key 
 *
 *  PARAMETERS
 *      user_PrivateKey     [in]      ByteArray*
 *      hashingProvider     [in]      HashProvider* 
 *
 *  RETURNS
 *      err                 [out]     int
 *
 *  NOTES
 *      will return a malloc error if there is insufficient memory to create a signer
 ****************************************************************************************
 */
int newPrivateKey(void){
  
  RsaKey priv;
  WC_RNG randomNumberGenerator;
  
  int err = 0;
  long e = 65537;   // standard value to use for exponent
  
  wc_InitRsaKey(&priv, NULL); // not using heap hint. No custom memory
    
  wc_InitRng(&randomNumberGenerator);
    
  // generate a 2048 bit long private key
  err = wc_MakeRsaKey(&priv, 2048, e, &randomNumberGenerator);
    
  if(err != 0) {
    // error generating a private key
  }  

  return err;
}

/**
 ****************************************************************************************
 *  NAME
 *      newSignerInstance
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
 *      will return a malloc error if there is insufficient memory to create a signer
 ****************************************************************************************
 */
Signer* newSignerInstance(ByteArray_t* user_PrivateKey, HashProvider_t* hashingProvider){
  
  Signer* signer = malloc(sizeof(Signer));    //TODO: wal, where is this freed?
  
  if (signer) {
    
    int err = newPrivateKey();                // Generate a new private key.
  
    signer->publicKey = NULL;
    signer->privateKey = NULL;                
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
    creator->newSignerInstance = &newSignerInstance;
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
 *      will return a malloc error if there is insufficient memory to create a keypair.
 ****************************************************************************************
 */
XYResult_t* getPublicKey(Signer* signer){
  
  XYResult_t* return_result = malloc(sizeof(XYResult_t));   //TODO: wal, where are these freed?
  
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
 *      will return a malloc error if there is insufficient memory to create a keypair.
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

