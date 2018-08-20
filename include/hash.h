typedef struct {
  char id[2];
  char* (*Hash)(char*); // Given just a null terminated char* return a cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  int (*VerifyHash)(char* hash, char* data);
  char* (*GetId)(); // Fetch the above id object and return it.
} HashProvider;
