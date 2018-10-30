/**
 ****************************************************************************************
 *
 * @file crypto.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

#ifndef XYCrypto

/*
 * DEFINES
 ****************************************************************************************
 */

#define XYCrypto

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "xyo.h"
#include "hash.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct Signer Signer;
typedef struct CryptoCreator CryptoCreator;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct Signer{

  ByteArray* publicKey; // Cryptographic Public Key
  ByteArray* privateKey; // Cryptographic Private Key
  XYResult* (*getPublicKey)(Signer*);  // Returns public key
  XYResult* (*sign)(Signer*, struct ByteArray*); // Returns signed byte array

  /*
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   */

  uint8_t (*verify)(Signer*, ByteArray* data, ByteArray* sig, ByteArray* pubkey);
  ByteArray* (*encrypt)(struct Signer*, struct ByteArray*); // Encrypt the data to the key of this Signer object
  ByteArray* (*decrypt)(struct Signer*, struct ByteArray*); // Decrypt the data with the key of this Signer object.

  HashProvider* hashingProvider;
};

struct CryptoCreator{

  char id[2];
  char* (*getId)(struct CryptoCreator*); // Fetch the above id object and return it.
  Signer* (*newInstance)(struct ByteArray* user_PrivateKey); // Generate a new Signer object which includes generating a new keypair.
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

char* cryptoGetId(CryptoCreator* object);
struct XYResult* getPublicKey(Signer* signer);
Signer* newInstance(ByteArray* user_PrivateKey);
CryptoCreator* newCryptoCreator(void);
XYResult* sign(Signer* signer, ByteArray* givenArray);
uint8_t verify_sig(Signer* signer, ByteArray* signedData, ByteArray* signature, ByteArray* publicKey);


#endif

// end of file crypto.h
