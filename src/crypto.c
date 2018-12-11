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
XYResult_t newCryptoSignerInstance(ByteArray_t* privateKey) {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t newCryptoCreator(){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t cryptoGetId(CryptoCreator_t* object){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t getSignatureId(Signer_t* signer){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t getPublicKey(Signer_t* signer){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t getPublicKeyId(Signer_t* signer){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t newPublicKey(Signer_t* signer){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t getPrivateKey(Signer_t* signer){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
/*
int error = OK;
uint8_t trng_bits[16];
WC_RNG dialogRng;
int keysize;
*/
XYResult_t newPrivateKey() {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t newKeyPair(){
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
 *      XYResult_t      [out]     Uint32_t        // Bytes copied to dest stored in result.value.ui         
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t sign(Signer_t* signer, char* dest, ByteArray_t* src) {
  DECLARE_RESULT();
  if(!dest && !src->payload && !src->size){
    XYERROR(XY_STATUS_ERROR);
  }

  memset(dest, 3, 1);
  result.status = XY_STATUS_OK;
  result.value.ui = 1;
  return result;
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
XYResult_t verify(Signer_t* signer,
                   ByteArray_t* signedData,
                   XYObject_t* signature,
                   XYObject_t* publicKey) {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t xyencrypt(Signer_t* signer, ByteArray_t* unEncrypedData) {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
XYResult_t xydecrypt(Signer_t* signer, ByteArray_t* encrypedData) {
  DECLARE_RESULT();
  XYSTATUS(XY_STATUS_OK);
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
