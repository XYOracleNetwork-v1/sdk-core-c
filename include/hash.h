#ifndef HASH_H
typedef struct HashProvider HashProvider;

#define HASH_H

struct HashProvider{
  char id[2];
  struct XYObject* (*Hash)(char*); // Given just a null terminated char* return a cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  int (*VerifyHash)(char* hash, char* data);
  char* (*GetId)(); // Fetch the above id object and return it.
};
#endif
