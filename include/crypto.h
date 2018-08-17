typedef struct{
  char* publicKey;
  char* privateKey;
} keyPairStruct;

typedef struct tagSigner{
  char* publicKey;
  char* privateKey;
  char* (*GetPubKey)(struct tagSigner*);
  char* (*Sign)(struct tagSigner*, char*);
  int (*Verify)(struct tagSigner, char*, char*);
  char* (*Encrypt)(char*);
  char* (*Decrypt)(char*);
} Signer;

typedef struct tagCryptoCreator{
  char id[2];
  char* (*GetId)(struct tagCryptoCreator*);
  Signer* (*GetSigner)();
} CryptoCreator;
