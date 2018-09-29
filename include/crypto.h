/**
 ****************************************************************************************
 *
 * @file crypto.h
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

#ifndef XYCRYPTO
#define XYCRYPTO

/*
 * INCLUDES
 ****************************************************************************************
 */

//#include <stdlib.h>
//#include <stddef.h>
//#include <string.h>
//#include "xyobject.h"
#include "hash.h"

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct Signer Signer;
typedef struct CryptoCreator CryptoCreator;

typedef struct{
  char* publicKey;
  char* privateKey;
} keyPairStruct;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct Signer{
  
  struct ByteArray* publicKey;                    // Cryptographic Public Key
  struct ByteArray* privateKey;                   // Cryptographic Private Key
  XYResult_t* (*getPublicKey)(Signer*);             // Returns public key
  XYResult_t* (*sign)(Signer*, struct ByteArray*);  // Returns signed byte array
  
  /*
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   */
  
  int (*verify)(Signer*, struct ByteArray* data, struct ByteArray* sig, struct ByteArray* pubkey);
  
  ByteArray_t* (*encrypt)(struct Signer*, ByteArray_t*); // Encrypt the data to the key of this Signer object
  ByteArray_t* (*decrypt)(struct Signer*, ByteArray_t*); // Decrypt the data with the key of this Signer object.
  
  HashProvider* hashingProvider;
};

struct CryptoCreator{
  
  char id[2];
  char* (*getId)(struct CryptoCreator*);  // Fetch the above id object and return it.
  
  Signer* (*newInstance)(struct ByteArray* user_PrivateKey, HashProvider* hashingProvider); // Generate a 
                                                                                            // new Signer object which 
                                                                                            // includes generating a new 
                                                                                            // keypair.
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

char* cryptoGetId(CryptoCreator* object);
XYResult_t* getPublicKey(Signer* signer);
Signer* newInstance(ByteArray_t* user_PrivateKey, HashProvider* hashingProvider);
CryptoCreator* newCryptoCreator(void);

#endif

// end of file crypto.h


