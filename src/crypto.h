/**
 * @file crypto.h
 * @author Arie Trouw (developer@xy.company)
 * @brief Primary crypto routines for the XY4 firmware.
 * @version 0.1
 * @date 2018-11-15
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

#ifndef XYCRYPTO
#define XYCRYPTO

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "hash.h"         // includes "xyobject.h"
#include "WolfSSL/wc_types.h"
#include "WolfSSL/wc_rsa.h"
#include "WolfSSL/trng.h"
#include "WolfSSL/wc_ecc.h"
#include "WolfSSL/wc_random.h"

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
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t newCryptoSignerInstance(ByteArray_t* privateKey);
XYResult_t newCryptoCreator(void);
XYResult_t getPublicKeyId(Signer_t* signer);
XYResult_t getSignatureId(Signer_t* signer);
XYResult_t XyoCryptoSigner (XYObject_t* privateKey);
XYResult_t getPublicKey(Signer_t* signer);
XYResult_t sign(Signer_t* signer, char* dest, ByteArray_t* src);
XYResult_t verify(Signer_t* signer, ByteArray_t* signedData, XYObject_t* signature, XYObject_t* publicKey);
XYResult_t xyencrypt(Signer_t* signer, ByteArray_t* unEncrypedData);
XYResult_t xydecrypt(Signer_t* signer, ByteArray_t* encrypedData);
XYResult_t getPrivateKey(Signer_t* signer);
XYResult_t newPrivateKey(void);

XYResult_t cryptoGetId(CryptoCreator_t* object);
XYResult_t generateNewKeyPair(void);
XYResult_t newKeyPair(void);
XYResult_t newPublicKey(Signer_t* signer);

#endif

// end of file crypto.h
