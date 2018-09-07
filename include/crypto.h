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
  char* publicKey; // Cryptographic Public Key
  char* privateKey; // Cryptographic Private Key
  char* (*getPublicKey)(Signer*);  // Returns public key
  char* (*sign)(Signer*, char*); // Returns signed byte array
  /*
   * The method will take data and a cryptographic signature and a cryptographic public key
   * and determine if data was signed by the given public key correctly or if the signature
   * is malformed / invalid. Boolean return value.
   */
  int (*verify)(Signer*, char* data, char* sig, char* pubkey);
  char* (*encrypt)(struct Signer*, char*); // Encrypt the data to the key of this Signer object
  char* (*decrypt)(struct Signer*, char*); // Decrypt the data with the key of this Signer object.
};

struct CryptoCreator{
  char id[2];
  char* (*getId)(struct CryptoCreator*); // Fetch the above id object and return it.
  Signer* (*getSigner)(); // Generate a new Signer object which includes generating a new keypair.
};
