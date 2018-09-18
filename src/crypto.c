#include "crypto.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


char* cryptoGetId(CryptoCreator* object){
  return object->id;
}

struct XYResult* getPublicKey(Signer* signer){
  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result->error != OK) return return_result;
  return_result->error = OK;
  return_result->result = signer->publicKey;
  return return_result;
}

keyPairStruct* generateKeypair(){
    keyPairStruct* keypair = malloc(sizeof(keyPairStruct));
    keypair->publicKey = NULL;
    keypair->privateKey = NULL;
    return keypair;
}

Signer* newInstance(ByteArray* user_PrivateKey){
  Signer* sig = malloc(sizeof(Signer));
  sig->publicKey = NULL;
  sig->privateKey = NULL; // Generate keypair and set these.
  sig->getPublicKey = &getPublicKey;
  sig->sign = NULL;
  sig->verify = NULL;
  return sig;
}

CryptoCreator* newCryptoCreator(){
  CryptoCreator* creator = malloc(sizeof(CryptoCreator));
  memset(creator->id, 0x00, 2);
  creator->newInstance = &newInstance;
  creator->getId = &cryptoGetId;
  return creator;
}
