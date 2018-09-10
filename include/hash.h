#ifndef HASH_H

typedef struct HashProvider HashProvider;

struct HashProvider{
  char id[2];
  struct XYResult* (*Hash)(struct ByteArray*); // Given just a null terminated char* return a cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  int (*VerifyHash)(struct ByteArray* hash, struct ByteArray* data);
  char* (*GetId)(); // Fetch the above id object and return it.
};

#define HASH_H
#endif
