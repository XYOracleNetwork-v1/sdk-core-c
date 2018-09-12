#ifndef XYCrypto
typedef struct Signer Signer;
typedef struct CryptoCreator CryptoCreator;
typedef struct{
  char* publicKey;
  char* privateKey;
} keyPairStruct;
#define XYCrypto
#endif

struct Signer{
  struct ByteArray* publicKey; // Cryptographic Public Key
  struct ByteArray* privateKey; // Cryptographic Private Key
  struct XYResult* (*getPublicKey)(Signer*);  // Returns public key
  struct XYResult* (*sign)(Signer*, ByteArray*); // Returns signed byte array
  /*
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   */
  int               (*verify)(Signer*, ByteArray* data, ByteArray* sig, ByteArray* pubkey);
  struct ByteArray* (*encrypt)(struct Signer*, ByteArray*); // Encrypt the data to the key of this Signer object
  struct ByteArray* (*decrypt)(struct Signer*, ByteArray*); // Decrypt the data with the key of this Signer object.
};

struct CryptoCreator{
  char id[2];
  char* (*getId)(struct CryptoCreator*); // Fetch the above id object and return it.
  Signer* (*getSigner)(); // Generate a new Signer object which includes generating a new keypair.
};
