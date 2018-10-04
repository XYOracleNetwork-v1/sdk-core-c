/**
 ****************************************************************************************
 *
 * @file crypto.c
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

#include "crypto.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

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
 *      return_result     [out]     XYResult*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create an XYResult
 ****************************************************************************************
 */
XYResult* getPublicKey(Signer* signer){

  XYResult* return_result = malloc(sizeof(XYResult));

  if(return_result->error != OK) return return_result;

  return_result->error = OK;
  return_result->result = signer->publicKey;

  return return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      verify
 *
 *  DESCRIPTION
 *      this routine returns EExyoError::OK if the signature is valid to sign the
 *      signedData field. Optionally, the publicKey field can be used to verify the
 *      signature matches the public key.
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create an XYResult
 ****************************************************************************************
 */
uint8_t verify_sig(Signer* signer, ByteArray* signedData, ByteArray* signature, ByteArray* publicKey){
  return 0;
}

/**
 ****************************************************************************************
 *  NAME
 *      sign
 *
 *  DESCRIPTION
 *      this routine returns the public key for the supplied signer
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create an XYResult
 ****************************************************************************************
 */
XYResult* sign(Signer* signer, ByteArray* givenArray){

  secp256k1Signature* return_signature = malloc(sizeof(secp256k1Signature));
  if(return_signature == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  XYObject* return_object = malloc(sizeof(XYObject));
  if(return_object == NULL){
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }

  return_object->id[0] = SignatureSet_id[0];
  return_object->id[1] = SignatureSet_id[1];
  return_object->payload = return_signature;
  return_signature->size = 66;
  return_signature->signature = malloc(sizeof(char)*66);
  return_signature->signature[0] = 0x13;
  return_signature->signature[1] = 0x37;

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = return_object;
    return return_result;
  } else {
    free(return_signature->signature);
    free(return_signature);
    free(return_object);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

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
 *
 *  RETURNS
 *      signer              [out]     Signer*
 *
 *  NOTES
 *      will return NULL if there is insufficient memory to create a signer
 ****************************************************************************************
 */
Signer* newInstance(ByteArray* user_PrivateKey){

  Signer* signer = malloc(sizeof(Signer));
  if(signer == NULL){
    return NULL;
  }
  ByteArray* placeholder = malloc(sizeof(ByteArray));
  if(placeholder == NULL ){
    free(signer);
    return NULL;
  }
  placeholder->payload = malloc(sizeof(char)*64);
  if(placeholder->payload == NULL){
    free(signer);
    free(placeholder);
    return NULL;
  }

  placeholder->size = 64;
  placeholder->payload[0] = 0x13;
  placeholder->payload[1] = 0x37;

  signer->publicKey = placeholder;
  signer->privateKey = placeholder;            // Generate keypair and set these.
  signer->getPublicKey = &getPublicKey;
  signer->sign = sign;
  signer->verify = verify_sig;

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

  CryptoCreator* creator = malloc(sizeof(CryptoCreator));

  if (creator) {

    memset(creator->id, 0x00, 2);
    creator->newInstance = &newInstance;
    creator->getId = &cryptoGetId;
  }

  return creator;
}

// end of file crypto.c
