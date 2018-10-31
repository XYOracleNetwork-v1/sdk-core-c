/**
 ****************************************************************************************
 *
 * @file crypto.h
 *
 * @XY4 project source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef XYCRYPTO
#define XYCRYPTO

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "hash.h"   // includes "xyobject.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define PRIVATE_KEY_LENGTH_2048 2048
#define PRIVATE_KEY_LENGTH_1024 1024

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct Signer Signer_t;
typedef struct CryptoCreator CryptoCreator_t;

typedef struct{
  char* publicKey;
  char* privateKey;
} keyPairStruct;

extern XYResult_t preallocated_return_result;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct Signer{
  
  ByteArray_t publicKey;                            // Cryptographic Public Key
  ByteArray_t privateKey;                           // Cryptographic Private Key
  
  XYResult_t* (*getPublicKey)(Signer_t*);           // Returns public key
  XYResult_t* (*sign)(Signer_t*, ByteArray_t*);     // Returns signed byte array
    
  /*
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   */
  
  XYResult_t* (*verify)(Signer_t* signer, 
                ByteArray_t* signedData, 
                XYObject_t* signature, 
                XYObject_t* publicKey);
  
  ByteArray_t* (*encrypt)(Signer_t*, ByteArray_t*);     // Encrypt the data to the key of 
                                                        // this Signer object
  ByteArray_t* (*decrypt)(Signer_t*, ByteArray_t*);     // Decrypt the data with the key 
                                                        // of this Signer object.
  HashProvider_t* hashingProvider;
};

/*************************************/

struct CryptoCreator{
  
  char id[2];                                                       //TODO: wal, constants please
  XYResult_t* (*getId)(CryptoCreator_t*);                           // Fetch the id object above
                                                                    // and return it.
  XYResult_t* (*newCryptoSignerInstance)(ByteArray_t* privateKey);  // Generate a new XyoCryptoSigner 
                                                                    // object, which includes
                                                                    // generating a new keypair.
}; 

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newCryptoSignerInstance(ByteArray_t* privateKey);
XYResult_t* newCryptoCreator(void);
XYResult_t* getPublicKeyId(Signer_t* signer);
XYResult_t* getSignatureId(Signer_t* signer);
XYResult_t* XyoCryptoSigner (XYObject_t* privateKey);
XYResult_t* getPublicKey(Signer_t* signer);
XYResult_t* sign(Signer_t* signer, ByteArray_t* dataToSign);
XYResult_t* verify(Signer_t* signer, ByteArray_t* signedData, XYObject_t* signature, XYObject_t* publicKey);
XYResult_t* encrypt(Signer_t* signer, ByteArray_t* unEncrypedData);
XYResult_t* decrypt(Signer_t* signer, ByteArray_t* encrypedData);
XYResult_t* getPrivateKey(Signer_t* signer);
XYResult_t* newPrivateKey(void);

XYResult_t* cryptoGetId(CryptoCreator_t* object);
XYResult_t* generateNewKeyPair(void);
XYResult_t* newKeyPair(void);
XYResult_t* newPublicKey(Signer_t* signer);

#endif

// end of file crypto.h


