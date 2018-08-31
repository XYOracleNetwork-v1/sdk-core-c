#include <stdint.h>
#ifndef XYOBJECT_H

typedef struct XYObject XYObject;
typedef struct XYResult XYResult;
typedef struct BoundWitness BoundWitness;
typedef struct ByteStrongArray ByteStrongArray;
typedef struct ShortStrongArray ShortStrongArray;
typedef struct IntStrongArray IntStrongArray;
typedef struct IntWeakArray IntWeakArray;
typedef struct ShortWeakArray ShortWeakArray;
typedef struct ByteWeakArray ByteWeakArray;
typedef struct NextPublicKey NextPublicKey;
/*
 * Our documentation uses the terminology of Multi or Single Element arrays.
 * A Multi element array is one which can contain many different types within it.
 * Each element in a Multi Element array has the following structure.
 *      char    id[2];
 *      void*   payload;
 * Each element in a sigle element array is only the payload. The major
 * and minor for the elements in the array are in the array object itself.
 * In this header file Multi-type element arrays will be referred to as
 * Weak arrays, and Single-type element array will be referred to as a
 * Strong array.
 */

struct ByteStrongArray {
  uint8_t  size;
  char     id[2];
  XYResult* (*add)(struct ByteStrongArray* self_ByteStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ByteStrongArray* self_ByteStrongArray, int index);
  XYResult* (*get)(struct ByteStrongArray* self_ByteStrongArray, int index);
  char* payload;
} ; //0x01

struct ShortStrongArray {
  uint16_t size;
  char     id[2];
  XYResult* (*add)(struct ShortStrongArray* self_ShortStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ShortStrongArray* self_ShortStrongArray, int index);
  XYResult* (*get)(struct ShortStrongArray* self_ShortStrongArray, int index);
  void* payload;
} ; //0x02

struct IntStrongArray {
  uint32_t  size;
  char      id[2];
  XYResult* (*add)(struct IntStrongArray* self_IntStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct IntStrongArray* self_IntStrongArray, int index);
  XYResult* (*get)(struct IntStrongArray* self_IntStrongArray, int index);
  void* payload;
} ; //0x03

struct LongStrongArray {
  uint64_t  size;
  char      id[2];
  XYResult* (*add)(struct LongStrongArray* self_LongStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct LongStrongArray* self_LongStrongArray, int index);
  XYResult* (*get)(struct LongStrongArray* self_LongStrongArray, int index);
  void* payload;
} ; //0x04

struct ByteWeakArray {
  uint8_t  size;
  XYResult* (*add)(struct ByteWeakArray* self_ByteWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ByteWeakArray* self_ByteWeakArray, int index);
  XYResult* (*get)(struct ByteWeakArray* self_ByteWeakArray, int index);
  void* payload;
} ; //0x05

struct ShortWeakArray {
  uint16_t size;
  XYResult* (*add)(struct ShortWeakArray* self_ShortWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct ShortWeakArray* self_ShortWeakArray, int index);
  XYResult* (*get)(struct ShortWeakArray* self_ShortWeakArray, int index);
  void* payload;
} ; //0x06

struct IntWeakArray {
  uint32_t  size;
  XYResult* (*add)(struct IntWeakArray* self_IntWeakArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct IntWeakArray* self_IntWeakArray, int index);
  XYResult* (*get)(struct IntWeakArray* self_IntWeakArray, int index);
  void* payload;
} ; //0x07

struct LongWeakArray {
  uint64_t  size;
  XYResult* (*add)(struct LongStrongArray* self_LongStrongArray, XYObject* user_XYObject);
  XYResult* (*remove)(struct LongStrongArray* self_LongStrongArray, int index);
  XYResult* (*get)(struct LongStrongArray* self_LongStrongArray, int index);
  void* payload;
} ; //0x08

enum EXyoErrors{
  OK,
  ERR_CRITICAL, // Catastrophic failure.
  ERR_NOID, // Returned when the core can't get the ID.
  ERR_CANT_GET_PAYLOAD, // Returned when the payload in inaccesible.
  ERR_NOSIGNER, // Returned when the core can't create a signer.
  ERR_ADDRESS_UNAVAILABLE, // Could not bind to address provided.
  ERR_NETWORK_UNAVAILABLE, // Core network services are unavailable.
  ERR_RECEIVER_REFUSED_CONNECTION, // Returned when The receiver refused connection.
  ERR_BUSY, // Returned when a core service is busy.
  ERR_NOKEYS, // Returned by when no keypair has been generated.
  ERR_BADDATA, // Returned if data is malformed e.g. too big.
  ERR_BADPUBKEY, // Returned if the public key is invalid.
  ERR_BADSIG, // Returned if the signature encoding is improper.
  ERR_CORRUPTDATA, // Returned if data is improperly encrypted.
  ERR_KEY_ALREADY_EXISTS, // Returned if can't insert because key is already mapped.
  ERR_INSUFFICIENT_MEMORY, // Returned if there wasn't enough memory to store.
  ERR_INTERNAL_ERROR, // Returned if there was a hardware error.
  ERR_TIMEOUT, // Returned if the disk timed out on read/write.
  ERR_COULD_NOT_DELETE, // Returned if delete failed.
  ERR_PERMISSION, // Returned if permissions are improper.
  ERR_KEY_DOES_NOT_EXIST // Returned if key isn't found in map.
};

struct XYResult{
  enum EXyoErrors error;
  void* result;
};

struct BoundWitness{
  uint32_t size;
  struct ShortStrongArray* publicKeys;
  struct IntStrongArray* payload;
  struct ShortStrongArray* signatures;
};

struct KeySet{
  struct ShortWeakArray* keys;
};

struct SignatureSet{
  struct ShortWeakArray* signatures;
};

struct Payload{
  uint32_t size;
  struct IntWeakArray* signedHeuristics;
  struct IntWeakArray* unsignedHeuristics;
};

struct PreviousHash{
  char id[2];
  char* hash;
};

struct NextPublicKey{
  char id[2];
  char* publicKey;
};

struct XYObject{
  char id[2];
  void* payload;
  char* (*GetId)(struct XYObject*); // Fetch the above id object and return it.
  void* (*GetPayload)(struct XYObject*); // Fetch the above payload pointer object and return it.
};

struct XYResult* preallocated_result;
void* typeTable[16][16];

struct XYResult* newObject(char id[2], void* payload);
struct XYResult* lookup(char id[2]);
XYResult* initTable();

#define XYOBJECT_H
#endif
