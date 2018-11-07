/**
 ****************************************************************************************
 *
 * @file crypto.c
 *
 * @XY4 project source code.
 *
 * @brief primary crypto routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "crypto.h"         // includes "xyobject.h", "hash.h"
#include "wc_rsa.h"
#include "xyo.h"
#include "trng.h"
#include "wc_ecc.h"
#include "wc_random.h"

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
 *      Provides a new instance of an XyoCryptoSigner.
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
 *      preallocated_return_result_ptr    [out]       XYResult_t*
 *                                                    // A XyoCryptoSigner that is compatible
 *                                                    // with getPublicKeyId() type and the
 *                                                    // getSignatureId() type.
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* newCryptoSignerInstance(ByteArray_t* privateKey) {

  Signer_t* signer = malloc(sizeof(Signer_t));          //TODO: wal, this should be
                                                        // freed somewhere
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!signer) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};  // couldn't create a signer!
                                                        // return the error now
  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!privateKey ||
     !privateKey->size ||
     !privateKey->payload) {

    // the supplied private key seems invalid.
    // so, generate a new one.

    preallocated_return_result_ptr = newPrivateKey();   // XYResult_t* from newPrivateKey

    if(preallocated_return_result_ptr->error) {RETURN_ERROR(ERR_NOKEYS)};        // return the error now

    signer->privateKey = *(ByteArray_t*)preallocated_return_result_ptr->result;  // store the new private key

  } else {

    // the supplied privateKey seems ok, so save it to the signer

    signer->privateKey = *privateKey;
  }

  preallocated_return_result_ptr = newPublicKey(signer);   // XYResult_t* from newPublicKey

  // make sure we got a new public key.

  if(preallocated_return_result_ptr->error) {RETURN_ERROR(ERR_NOKEYS)};   // return the error now

  signer->publicKey = *(ByteArray_t*)preallocated_return_result_ptr->result;

  signer->sign = &sign;                       // XYResult_t* (*sign)(Signer_t*, ByteArray_t*)
  signer->verify = &verify;                   // XYResult_t* (*verify)(Signer_t* signer,
                                              //                       ByteArray_t* signedData,
                                              //                       XYObject_t* signature,
                                              //                       XYObject_t* publicKey)
  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* newCryptoCreator(){

  /**************************************/
  /* allocate a new CryptoCreator first */
  /**************************************/

  CryptoCreator_t* aNewCryptoCreator = malloc(sizeof(CryptoCreator_t));     //TODO: wal, this should
                                                                            // be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!aNewCryptoCreator) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};           // couldn't create a new creator

  preallocated_return_result_ptr = newPrivateKey();                         // Generate a new private key.
/*
  if(preallocated_return_result_ptr->error) {RETURN_ERROR(ERR_NOKEYS)};     // couldn't create a new
                                                                            // private key.
                                                                            // return the error now
  ByteArray_t* newPrivateKeyPair = preallocated_return_result_ptr->result;

  preallocated_return_result_ptr = newCryptoSignerInstance(newPrivateKeyPair);  // a ByteArray_t*
                                                                                // to a privateKey
  if(preallocated_return_result_ptr->error ||
     !preallocated_return_result_ptr->result) {RETURN_ERROR(ERR_NOSIGNER)};     // no new signer instance!
                                                                                // return the error now
  memset(aNewCryptoCreator->id, 0x00, 2);                                       //TODO: wal, constants please

  aNewCryptoCreator->newCryptoSignerInstance = &newCryptoSignerInstance;
                                                            // store the new signer instance
                                                            // in the crypto creator
  aNewCryptoCreator->getId = &cryptoGetId;                  // char* cryptoGetId(CryptoCreator_t* object
  */
  return preallocated_return_result_ptr;
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
 *      object            [in]      CryptoCreator_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *      this routine is a 'getter' only. it does't generate a new ID.
 ****************************************************************************************
 */
XYResult_t* cryptoGetId(CryptoCreator_t* object){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!object || !object->getId) {RETURN_ERROR(ERR_BADDATA)};  // return the error now

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &object->id;

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr    [out]   XYResult_t*
 *                                                // The id (in form of a major in minor)
 *                                                // of a signature key type that the
 *                                                // XyoCryptoSigner (generated from newInstance())
 *                                                // will return when calling sign(data).
 *                                                // This is used when verifying signature types.
 *  NOTES
 *      this routine is a 'getter' only. it does't generate a new ID.
 ****************************************************************************************
 */
XYResult_t* getSignatureId(Signer_t* signer){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.size ||
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};  // return the error now

  static ByteArray_t signatureId[2];              // major/minor, wal, constant please

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = signatureId;

  return preallocated_return_result_ptr;          // return the public key
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
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *      this routine is a 'getter' only. it does't generate a new key.
 ****************************************************************************************
 */
XYResult_t* getPublicKey(Signer_t* signer){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.size ||
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};    // return the error now

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &(signer->publicKey);  // publicKey is a ByteArray_t*

  return preallocated_return_result_ptr;                          // return the public key
}

/**
 ****************************************************************************************
 *  NAME
 *      getPublicKeyId
 *
 *  DESCRIPTION
 *      The id (in the form of a major in minor) of a public key type that the XyoCryptoSigner
 *      (generated from newInstance()) will return when calling getPublicKey(). This is
 *      used when verifying signature types.
 *
 *  PARAMETERS
 *      signer      [in]      Signer_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]     XYResult_t*  // this holds an error code and the
 *                                                                // Public Key Id.
 *  NOTES
 *      this routine is a 'getter' only. it does't generate a new key.
 ****************************************************************************************
 */
XYResult_t* getPublicKeyId(Signer_t* signer){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.size ||
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};   // return the error now

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &signer->publicKey;  // return the public key ByteArray_t*

  return preallocated_return_result_ptr;    // return the public key from signer
}

/**
 ****************************************************************************************
 *  NAME
 *      newPublicKey
 *
 *  DESCRIPTION
 *      this routine returns a new public key
 *
 *  PARAMETERS
 *      signer            [in]      Signer_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *      //TODO: convert this routine from a simple 'getter' to a creator.
 ****************************************************************************************
 */
XYResult_t* newPublicKey(Signer_t* signer){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.size ||
     !signer->privateKey.payload) {RETURN_ERROR(ERR_BADDATA)};    // return the error now

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &(signer->publicKey);  // publicKey is a ByteArray_t*

  return preallocated_return_result_ptr;                          // return the public key
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
 *      preallocated_return_result_ptr  [out]   XYResult_t*
 *                                              // the private key of the supplied
 *                                              // XyoCryptoSigner.
 *                                              // This private key can be used to restore
 *                                              // the same keypair when creating a
 *                                              // XyoCryptoSigner with XyoCryptoProvider.
 *                                              ------------------------------
 *                                              preallocated_return_result_ptr.error = error code;
 *                                              preallocated_return_result_ptr.result = (ByteArray_t*)&signer->privateKey;
 *
 *  NOTES
 *      keep in mind that this is just a fetch call (getter), not a generate new key call.
 ****************************************************************************************
 */
XYResult_t* getPrivateKey(Signer_t* signer){

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.payload ||
     !signer->privateKey.size) {RETURN_ERROR(ERR_NOKEYS)}; // return the error now

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = (ByteArray_t*)&signer->privateKey;

  return preallocated_return_result_ptr;                  // return the private key
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
 *      preallocated_return_result_ptr      [out]     XYResult_t*     holds the error code
 *                                                    and a new private key ByteArray_t*
 *                                                    ------------------------
 *                                                    preallocated_return_result_ptr.error = error code
 *                                                    preallocated_return_result_ptr.result = aNewPrivateKey;
 *  NOTES
 *      will return a malloc error if malloc fails.
 *      wc_ = wolf crypto library routine or data type
 *      support SECP256k1
 ****************************************************************************************
 */
  int error = OK;
  uint8_t trng_bits[16];
  WC_RNG dialogRng;
  int keysize;

/************************************/
XYResult_t* newPrivateKey() {

static WC_RNG randomNumberGenerator;
// RsaKey rsaPrivateKey;
  ecc_key eccPrivateKey;

  trng_acquire(&trng_bits[0]);

  // ********************************/
  // * initialize stack structures  */
  // ********************************/

  XMEMSET(&randomNumberGenerator, 0, sizeof(randomNumberGenerator));
  //XMEMSET(&rsaPrivateKey, 0, sizeof(rsaPrivateKey));

  ByteArray_t* aNewPrivateKey = malloc(sizeof(keyPairStruct));    //TODO: wal, this should
                                                                  // be freed somewhere.
  // ********************************/
  // * guard against malloc errors  */
  // ********************************/

  if(!aNewPrivateKey) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};

  //long e = 65537;                             // standard value to use for exponent

  //error = wc_InitRng(&randomNumberGenerator); // BLOCKING call. the rng must be freed with wc_FreeRng
                                                // *** Internal return codes ***
                                                // #define DRBG_SUCCESS      0
                                                // #define DRBG_FAILURE      1
                                                // #define DRBG_NEED_RESEED  2
                                                // #define DRBG_CONT_FAILURE 3

  //if(error != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};

  //error = wc_InitRsaKey(&rsaPrivateKey, NULL);  // not using heap hint. No custom memory

  //if(error != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};

  // ****************************************
  // * generate a 2048 bit long private key *
  // ****************************************
  //int wc_MakeRsaKey(RsaKey* key, int size, long e, WC_RNG* rng)
  //error = wc_MakeRsaKey(&rsaPrivateKey, PRIVATE_KEY_LENGTH_1024, e, &randomNumberGenerator);

  //if(error != OK) {RETURN_ERROR(ERR_INTERNAL_ERROR)};

  // **********************************************
  // * store the new private key in a ByteArray_t *
  // * that will in turn be stored in             *
  // * preallocated_return_result_ptr->result     *
  // **********************************************

  //aNewPrivateKey->size = rsaPrivateKey.dataLen;
  //aNewPrivateKey->payload = (char *)rsaPrivateKey.data;

  //ref, wc_ecc_init (ecc_key *key)
  error = wc_ecc_init(&eccPrivateKey);    // preps the key supplied for future use

  //ref, wc_ecc_make_key(WC_RNG *rng, int keysize, ecc_key *key);
  //wc_ecc_make_key(&dialogRng, PRIVATE_KEY_LENGTH_1024, &eccPrivateKey);

  preallocated_return_result_ptr->error = error;
  preallocated_return_result_ptr->result = aNewPrivateKey;  // the ByteArray_t holding the new private key

  //error = wc_FreeRng(&randomNumberGenerator);               // required release

  return preallocated_return_result_ptr;                    // return the private key
}

/**
 ****************************************************************************************
 *  NAME
 *      newKeyPair
 *
 *  DESCRIPTION
 *      this routine returns a NEW pair of public and private keys
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *      will return a malloc error if malloc fails.
 *      this routine is not a simple getter. it actually creates a new key pair.
 ****************************************************************************************
 */
XYResult_t* newKeyPair(){

  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            // be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!keyPair) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};     // return the error now

  keyPair->publicKey = NULL;
  keyPair->privateKey = NULL;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = keyPair;         // keyPair is a ByteArray_t*

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr   [out]     XYResult_t*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t* sign(Signer_t* signer, ByteArray_t* dataToSign) {

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer || !dataToSign) {RETURN_ERROR(ERR_BADDATA)};   // return the error now

  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            // be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!keyPair) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};     // return the error now

  keyPair->publicKey = NULL;
  keyPair->privateKey = NULL;

  preallocated_return_result_ptr->error = 0;
  preallocated_return_result_ptr->result = &keyPair;

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr  [out]     XYResult_t*   // the result is in
 *
 *  NOTES
 *      will return a malloc error if malloc fails. otherwise, a bool result (of the verify)
 *      will be returned in preallocated_return_result_ptr->error
 ****************************************************************************************
 */
XYResult_t* verify(Signer_t* signer,
                   ByteArray_t* signedData,
                   XYObject_t* signature,
                   XYObject_t* publicKey) {

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signedData ||
     !signature ||
     !publicKey) {RETURN_ERROR(ERR_BADDATA)};               // return the error now

  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!keyPair) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};     // return the error now

  keyPair->publicKey = NULL;
  keyPair->privateKey = NULL;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = NULL;

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr   [out]    XYResult_t*
 *                                                // A promise with an XyoObject that contains
 *                                                // the data encrypted. This data can be
 *                                                // decrypted with the a XyoCryptoSigner
 *                                                // that supports the same cryptographic function.
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* xyencrypt(Signer_t* signer, ByteArray_t* unEncrypedData) {

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.payload ||
     !signer->privateKey.size ||
     !unEncrypedData->size ||
     !unEncrypedData->payload) {RETURN_ERROR(ERR_NOKEYS)};  // return the error now

  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!keyPair) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};     // return the error now

  keyPair->publicKey = NULL;
  keyPair->privateKey = NULL;

  preallocated_return_result_ptr->error = NULL;
  preallocated_return_result_ptr->result = &keyPair;

  return preallocated_return_result_ptr;
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
 *      preallocated_return_result_ptr   [out]     XYResult_t*
 *                                                // A promise with an XyoObject that contains
 *                                                // the data encrypted. This data can be
 *                                                // decrypted with the a XyoCryptoSigner
 *                                                // that supports the same cryptographic function.
 *  NOTES
 *      will return a malloc error if malloc fails.
 ****************************************************************************************
 */
XYResult_t* xydecrypt(Signer_t* signer, ByteArray_t* encrypedData) {

  /************************************/
  /* guard against bad input data     */
  /************************************/

  if(!signer ||
     !signer->privateKey.payload ||
     !signer->privateKey.size ||
     !encrypedData->size ||
     !encrypedData->payload) {RETURN_ERROR(ERR_NOKEYS)};    // return the error now

  keyPairStruct* keyPair = malloc(sizeof(keyPairStruct));   //TODO: wal, this should
                                                            //  be freed somewhere.
  /********************************/
  /* guard against malloc errors  */
  /********************************/

  if(!keyPair) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};     // return the error now

  keyPair->publicKey = NULL;
  keyPair->privateKey = NULL;

  preallocated_return_result_ptr->error = NULL;
  preallocated_return_result_ptr->result = &keyPair;

  return preallocated_return_result_ptr;
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

struct Signer{

  ByteArray_t publicKey;                              // Cryptographic Public Key
  ByteArray_t privateKey;                             // Cryptographic Private Key

  XYResult_t* (*getPublicKey)(Signer_t* signer);      // Returns public key
  XYResult_t* (*sign)(Signer_t*, ByteArray_t*);       // Returns a signed byte array

  / *
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   * /

  XYResult_t* (*verify)(Signer_t* signer,
                ByteArray_t* data,
                XYObject_t* signature,
                XYObject_t* publicKey);

  ByteArray_t* (*encrypt)(Signer_t*, ByteArray_t*);   // Encrypt the data to the key of
                                                      // this Signer object
  ByteArray_t* (*decrypt)(Signer_t*, ByteArray_t*);   // Decrypt the data with priv key
                                                      // of this Signer object.
  HashProvider_t* hashingProvider;
};

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

// end of file crypto.c
