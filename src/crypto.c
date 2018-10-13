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
#include "xyo.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *  NAME
 *      newCryptoSignerInstance
 *
 *  DESCRIPTION
 *      Provides a new instance of a XyoCryptoSigner. 
 *
 *  PARAMETERS
 *      privateKey          [in]      ByteArray_t*    // A private key to initialize the 
 *                                                    // XyoCryptoSigner with a public 
 *                                                    // private keypair. To obtain a 
 *                                                    // private key, one must call 
 *                                                    // getPrivateKey() on a XyoCryptoSigner. 
 *                                                    // If this value is null, the XyoCryptoSigner 
 *                                                    // will automatically create a public 
 *                                                    // and private keypair.
 *  RETURNS
 *      XYResult_t*         [out]     return_result   // A XyoCryptoSigner that is compatible 
 *                                                    // with getPublicKeyId() type and the 
 *                                                    // getSignatureId() type.
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
struct Signer{
  
  ByteArray_t publicKey;                            // Cryptographic Public Key
  ByteArray_t privateKey;                           // Cryptographic Private Key
  
  XYResult_t* (*getPublicKey)(Signer_t* signer);    // Returns public key
  XYResult_t* (*sign)(Signer_t*, ByteArray_t*);     // Returns a signed byte array
    
  / *
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   * /
  
  XYResult_t* (*verify)(Signer_t* signer, 
                ByteArray_t* data, 
                XYObject_t* signature, 
                XYObject_t* publicKey);
  
  ByteArray_t* (*encrypt)(Signer_t*, ByteArray_t*);           // Encrypt the data to the key of 
                                                              // this Signer object
  ByteArray_t* (*decrypt)(Signer_t*, ByteArray_t*);           // Decrypt the data with priv key 
                                                              // of this Signer object.
  HashProvider_t* hashingProvider;
};
 */
XYResult_t* newCryptoSignerInstance(ByteArray_t* privateKey) {
  
  static XYResult_t return_result;
  
  XYResult_t* return_result_ptr;
    
  Signer_t* signer = malloc(sizeof(Signer_t));          //TODO: wal, this should be 
                                                        // freed somewhere

  if(!signer) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};  // couldn't create a signer!
                                                        // return the error now
  if(!privateKey || !privateKey->size) {
    
    // the supplied private key is invalid.
    // so, generate a new one.
    
    return_result_ptr = newPrivateKey();                // XYResult_t* from newPrivateKey
    
    if(return_result_ptr->error) {RETURN_ERROR(ERR_CRITICAL)};      // return the error now
    
    signer->privateKey = *(ByteArray_t*)return_result_ptr->result;  // store the new private key
    
  } else {
    
    // save the supplied privateKey
    
    signer->privateKey = *privateKey;             
  }
  
  return_result_ptr = newPublicKey(signer);     // XYResult_t* from newPublicKey

  signer->publicKey = *(ByteArray_t*)return_result_ptr->result;
  
  signer->sign = &sign;                         // XYResult_t* (*sign)(Signer_t*, ByteArray_t*) 
  signer->verify = &verify;                     // XYResult_t* (*verify)(Signer_t* signer, 
                                                //                       ByteArray_t* signedData, 
                                                //                       XYObject_t* signature, 
                                                //                       XYObject_t* publicKey)  
  return &return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      newCryptoCreator
 *
 *  DESCRIPTION
 *      this routine returns a new CryptoCreator 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      creator     [out]     CryptoCreator*
 *
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
struct CryptoCreator{
  
  char id[2];
  char* (*getId)(struct CryptoCreator*);                              // Fetch the above id 
                                                                      // object and return it.
  Signer_t* (*newCryptoSignerInstance)(ByteArray_t* user_PrivateKey); // Generate a new Signer
                                                                      // object which includes
                                                                      // generating a new 
                                                                      // keypair.
}; 
struct XYResult{
  int error;
  void* result;
};
 */
XYResult_t* newCryptoCreator(){
  
  static XYResult_t return_result;
  static XYResult_t* return_result_ptr;

  /**************************************/
  /* allocate a new CryptoCreator first */
  /**************************************/
  
  CryptoCreator_t* cryptoCreator = malloc(sizeof(CryptoCreator_t)); //TODO: wal, this should 
                                                                    // be freed somewhere.
  if (cryptoCreator) {
    
    XYResult_t* newPrivateKeyResult = newPrivateKey();              // Generate a new private key.
  
    if(newPrivateKeyResult->error) {RETURN_ERROR(ERR_CRITICAL)};    // couldn't create a new
                                                                    // private key. 
                                                                    // return the error now
    ByteArray_t* newPrivateKey = newPrivateKeyResult->result;
    
    XYResult_t* newSignerResult = newCryptoSignerInstance(newPrivateKey);  // a ByteArray_t* 
                                                                    // to a privateKey
    if(newSignerResult->error ||
       !newSignerResult->result) {RETURN_ERROR(ERR_CRITICAL)};      // no new signer instance!
                                                                    // return the error now
    memset(cryptoCreator->id, 0x00, 2);                             //TODO: wal, constants please
    
    return_result_ptr = newCryptoSignerInstance(newPrivateKey);     // create a new signer
       
    cryptoCreator->newCryptoSignerInstance = (Signer_t*)return_result_ptr->result;    
                                                                    // store the new signer instance   
                                                                    // in the crypto creator
    
    cryptoCreator->getId = &cryptoGetId;            // char* cryptoGetId(CryptoCreator_t* object
  }
  
  return &return_result;
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
char* cryptoGetId(CryptoCreator_t* object){
  
  return object->id;
}

/**
 ****************************************************************************************
 *  NAME
 *      getSignatureId
 *
 *  DESCRIPTION
 *      this routine returns the public key from the supplied signer
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*   // The id (in form of a major in minor) 
 *                                                // of a signature key type that the 
 *                                                // XyoCryptoSigner (generated from newInstance()) 
 *                                                // will return when calling sign(data). 
 *                                                // This is used when verifying signature types.
 *  NOTES
 *      
 ****************************************************************************************
 */
XYResult_t* getSignatureId(Signer_t* signer){
  
  static XYResult_t return_result;
  static ByteArray_t signatureId[2];              // major/minor, wal, constant please
  
  return_result.error = OK;
  return_result.result = signatureId;

  if(!signer) {RETURN_ERROR(ERR_BADDATA)};        // return the error now
  
  return &return_result;                          // return the public key
}

/**
 ****************************************************************************************
 *  NAME
 *      getPublicKey
 *
 *  DESCRIPTION
 *      this routine returns the public key from the supplied signer
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*
 *
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* getPublicKey(Signer_t* signer){
  
  static XYResult_t return_result;
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  
  if(!signer || 
     !signer->privateKey.size || 
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};  // return the error now
  
  if(return_result.error != OK) return &return_result;          // return the error now
  
  return_result.error = OK;
  return_result.result = &(signer->publicKey);                  // ByteArray_t*
  
  return &return_result;                                        // return the public key
}

/**
 ****************************************************************************************
 *  NAME
 *      getPublicKeyId
 *
 *  DESCRIPTION
 *      The id (in form of a major in minor) of a public key type that the XyoCryptoSigner 
 *      (generated from newInstance()) will return when calling getPublicKey(). This is 
 *      used when verifying signature types.
 *
 *  PARAMETERS
 *      signer            [in]      Signer_t*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*     // this holds an error code and the 
 *                                                  // Public Key Id.
 *
 *  NOTES
 *      this routine is a 'getter' only. it does't generate a new key.
 ****************************************************************************************
 */
XYResult_t* getPublicKeyId(Signer_t* signer){
  
  static XYResult_t return_result;
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  
  if(!signer || 
     !signer->privateKey.size || 
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};   // return the error now
  
  return_result.error = OK;
  return_result.result = &signer->publicKey;    // return the public key ByteArray_t*
  
  return &return_result;    // return the public key from signer
}

/**
 ****************************************************************************************
 *  NAME
 *      getPublicKey
 *
 *  DESCRIPTION
 *      this routine returns a new public key 
 *
 *  PARAMETERS
 *      signer            [in]      Signer_t*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*
 *
 *  NOTES
 *      
 ****************************************************************************************
 */
XYResult_t* newPublicKey(Signer_t* signer){
  
  static XYResult_t return_result;
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  
  if(!signer || 
     !signer->privateKey.size || 
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};  // return the error now
  
  //if(return_result.error != OK) return &return_result;          // return the error now
  
  return_result.error = OK;
  return_result.result = &(signer->publicKey);                  // ByteArray_t*
  
  return &return_result;                                        // return the public key
}

/**
 ****************************************************************************************
 *  NAME
 *      generateKeyPair
 *
 *  DESCRIPTION
 *      this routine returns a NEW pair of public and private keys 
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      keyPair     [out]     keyPairStruct*
 *
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
keyPairStruct* generateKeyPair(){
  
  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should 
                                                            //  be freed somewhere.
  if (keyPair) {
    
    keyPair->publicKey = NULL;
    keyPair->privateKey = NULL;
  }
  
  return keyPair;
}

/**
 ****************************************************************************************
 *  NAME
 *      sign
 *
 *  DESCRIPTION
 *      this routine performs a cryptographic signature of the data supplied, that was 
 *      created with the private key. 
 *
 *  PARAMETERS
 *      dataToSign      [in]      ByteArray_t*    // The data to cryptographically sign 
 *                                                // using the private key of the 
 *                                                // XyoCryptoSigner.
 *  RETURNS
 *      return_result   [out]     XYResult_t*
 *
 *  NOTES
 *      will return a malloc error in return_result->error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* sign(Signer_t* signer, ByteArray_t* dataToSign) {
  
  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  static XYResult_t return_result;

  if (keyPair) {
    
    keyPair->publicKey = NULL;
    keyPair->privateKey = NULL;
  }
  
  return_result.error = 0;
  return_result.result = &keyPair;
  
  return &return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      verify
 *
 *  DESCRIPTION
 *      Cryptographically verify a signature given data, a signature, 
 *      and a public key that the XyoCryptoSigner supports. If the signature is valid, 
 *      this routine will return true, if it is invalid it will return false.
 *
 *  PARAMETERS
 *      Signer_t*       [in]      signer          // The signer to use 
 *      signedData      [in]      ByteArray_t*    // The data that was signed using the 
 *                                                // cryptographic function that the 
 *                                                // XyoCryptoSigner supports.
 *      signature       [in]      XYObject_t*     // The signature that was created using 
 *                                                // the cryptographic function that the 
 *                                                // XyoCryptoSigner supports.
 *      publicKey       [in]      XYObject_t*     // The public key of the party that signed 
 *                                                // the data with the the cryptographic 
 *                                                // function that the XyoCryptoSigner supports.
 *  RETURNS
 *      return_result   [out]     XYResult_t*     // the result is in 
 *
 *  NOTES
 *      will return a malloc error if malloc fails. otherwise, a bool result (of the verify) 
 *      will be returned in return_result->error
 ****************************************************************************************
 */
XYResult_t* verify(Signer_t* signer, 
                   ByteArray_t* signedData, 
                   XYObject_t* signature, 
                   XYObject_t* publicKey) {
  
  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  static XYResult_t return_result;

  if (keyPair) {
    
    keyPair->publicKey = NULL;
    keyPair->privateKey = NULL;
  }
  
  return_result.error = OK;
  return_result.result = NULL;
  
  return &return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      encrypt
 *
 *  DESCRIPTION
 *      encrypts the supplied data.
 *
 *  PARAMETERS
 *      Signer_t*       [in]      signer          // The signer to use 
 *      unEncrypedData  [in]      ByteArray_t*    // The data that the XyoCryptoSigner will 
 *                                                // encrypt using its private key.
 *  RETURNS
 *      return_result   [out]     XYObject_t*     // A promise with an XyoObject that contains 
 *                                                // the data encrypted. This data can be 
 *                                                // decrypted with the a XyoCryptoSigner 
 *                                                // that supports the same cryptographic function.
 *  NOTES
 *      will return a malloc error if malloc fails. 
 ****************************************************************************************
 */
XYObject_t* encrypt(Signer_t* signer, ByteArray_t* unEncrypedData) {
  
  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  static XYObject_t return_object;

  if (keyPair) {
    
    keyPair->publicKey = NULL;
    keyPair->privateKey = NULL;
  }
  
  return_object.payload = NULL;
  
  return &return_object;
}

/**
 ****************************************************************************************
 *  NAME
 *      decrypt
 *
 *  DESCRIPTION
 *      A ByteArray that is the decrypted data.
 *
 *  PARAMETERS
 *      Signer_t*       [in]      signer          // The signer to use 
 *      encrypedData    [in]      ByteArray_t*    // The data that was encrypted using a 
 *                                                // cryptographic function that the 
 *                                                // XyoCryptoSigner supports.
 *  RETURNS
 *      return_result   [out]     XYObject_t*     // A promise with an XyoObject that contains 
 *                                                // the data encrypted. This data can be 
 *                                                // decrypted with the a XyoCryptoSigner 
 *                                                // that supports the same cryptographic function.
 *  NOTES
 *      will return a malloc error if malloc fails. 
 ****************************************************************************************
 */
XYObject_t* decrypt(Signer_t* signer, ByteArray_t* encrypedData) {
  
  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  static XYObject_t return_object;

  if (keyPair) {
    
    keyPair->publicKey = NULL;
    keyPair->privateKey = NULL;
  }
  
  return_object.payload = NULL;
  
  return &return_object;
}

/**
 ****************************************************************************************
 *  NAME
 *      getPrivateKey
 *
 *  DESCRIPTION
 *      this routine returns the private key from the supplied signer object
 *
 *  PARAMETERS
 *      signer            [in]      Signer*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*   // the private key of the supplied
 *                                                // XyoCryptoSigner. 
 *                                                // This private key can be used to restore 
 *                                                // the same keypair when creating a 
 *                                                // XyoCryptoSigner with XyoCryptoProvider.
 *  NOTES
 *      keep in mind that this is a fetch call (getter), not a generate call.
 ****************************************************************************************
 */
XYResult_t* getPrivateKey(Signer_t* signer){
  
  static XYResult_t return_result;
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  
  if(!signer) {RETURN_ERROR(ERR_BADDATA)};                    // return the error now
  if(!signer->privateKey.size) {RETURN_ERROR(ERR_BADDATA)};   // return the error now
  
  return_result.error = OK;
  return_result.result = (ByteArray_t*)&signer->privateKey;
  
  return &return_result;                                      // return the private key
}

/**
 ****************************************************************************************
 *  NAME
 *      newPrivateKey
 *
 *  DESCRIPTION
 *      this routine creates and returns a new private key 
 *
 *  PARAMETERS
 *      none 
 *
 *  RETURNS
 *      return_result       [out]     XYResult_t*     holds the error code 
 *                                                    and a new private key ByteArray_t*
 *  NOTES
 *      will return a malloc error if malloc fails. 
 *      wc_ = wolf crypto library routine or data type
 ****************************************************************************************
 */
XYResult_t* newPrivateKey() {
  
  static XYResult_t return_result;
  
  ByteArray_t* theNewPrivateKey = malloc(sizeof(keyPairStruct));    //TODO: wal, this should
                                                                    //  be freed somewhere.
  if(!theNewPrivateKey) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};
  
  static RsaKey rsaPrivateKey;
  WC_RNG randomNumberGenerator;
  
  int error = 0;
  long e = 65537;                         // standard value to use for exponent
  
  wc_InitRsaKey(&rsaPrivateKey, NULL);    // not using heap hint. No custom memory
    
  wc_InitRng(&randomNumberGenerator);
  
  /****************************************/
  /* generate a 2048 bit long private key */
  /****************************************/
  error = wc_MakeRsaKey(&rsaPrivateKey, 2048, e, &randomNumberGenerator);
  
  if(error != OK) {
    // an error occurred while generating a new private key
    
    RETURN_ERROR(ERR_NOKEYS);
  }  

  /**********************************************/
  /* store the new private key in a ByteArray_t */
  /* that will in turn be stored in the return  */
  /* result XYResult_t                          */
  /**********************************************/
  
  theNewPrivateKey->size = rsaPrivateKey.dataLen;
  theNewPrivateKey->payload = (char *)rsaPrivateKey.data;
  
  return_result.error = OK;
  return_result.result = theNewPrivateKey;  // the ByteArray_t holding the new private key
  
  return &return_result;                    // return the private key
}

/*
/////////////////////
// type references //
/////////////////////

struct ByteArray{
  uint32_t size;
  char reserved[2];
  char* payload;
}

struct XYResult{
  int error;
  void* result;
};

struct XYObject{
  char id[2];
  void* payload;
  char* (*GetXyobjectId)(XYObject_t*);      // Fetch the above id object and return it.
  void* (*GetPayload)(XYObject_t*);         // Fetch the above payload pointer object and return it.
} ;

struct RsaKey {
    mp_int n, e, d, p, q, dP, dQ, u;
    void* heap;                              // for user memory overrides 
    byte* data;                              // temp buffer for async RSA 
    int   type;                              // public or private 
    int   state;
    word32 dataLen;
*/

// end of file crypto.c

