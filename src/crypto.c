#include "crypto.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


char* cryptoGetId(CryptoCreator* object){
  return object->id;
}

char* GetPubKey(Signer* signer){
  return signer->publicKey;
}

keyPairStruct* generateKeypair(){
    keyPairStruct* keypair = malloc(sizeof(keyPairStruct));
    keypair->publicKey = NULL;
    keypair->privateKey = NULL;
    return keypair;
}

Signer* GetSigner(){
  Signer* sig = malloc(sizeof(Signer));
  keyPairStruct* keys = generateKeypair();
  sig->publicKey = keys->publicKey;
  sig->privateKey = keys->privateKey;
  sig->GetPubKey = &GetPubKey;
  sig->Sign = NULL;
  sig->Verify = NULL;
  sig->Verify = NULL;
  return sig;
}

CryptoCreator* newCryptoCreator(){
  CryptoCreator* creator = malloc(sizeof(CryptoCreator));
  memset(creator->id, 0x00, 2);
  creator->GetSigner = &GetSigner;
  creator->GetId = &cryptoGetId;
  return creator;
}
