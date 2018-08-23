#include <stdint.h>

#ifdef XYOBJECT_H
typedef struct XYObject XYObject, *xop;
typedef struct XYResult XYResult, *xrp;
typedef struct ArrayItr ArrayItr, *aip;
typedef struct BoundWitness BoundWitness, *bw;

struct SZSNSArray {
  char id[2];
  uint16_t  size;
  uint16_t  elements;
  void (*add)(SZSNSArray* self_SZSNSArray, XYObject* user_XYObject);
  void (*remove)(int index);
  XYObject* (*get)(int index);
  void* payload;
} ; //0x01

typedef struct {
  char id[2];
  uint16_t elements;
  void* payload;
} NZSNSArray; //0x02

typedef struct {
  uint16_t size;
  uint16_t elements;
  void* payload;
} SZSNWArray; //0x03


typedef struct {
  char id[2];
  uint8_t size;
  uint8_t elements;
  void* payload;
} BZBNSArray; //0x04

typedef struct {
  uint8_t size;
  uint8_t elements;
  void* payload;
} BZBWSArray; //0x05

typedef struct {
  char id[2];
  uint16_t size;
  uint8_t elements;
  void* payload;
} SZBNSArray; //0x06

typedef struct {
  uint16_t size;
  uint8_t elements;
  void* payload;
} SZBNWArray; //0x07

typedef struct {
  char id[2];
  uint32_t size;
  uint16_t elements;
  void* payload;
} IZSNSArray; //0x08

typedef struct {
  uint32_t size;
  uint16_t elements;
  void* payload;
} IZSNWArray; //0x09

typedef struct {
  char id[2];
  uint32_t size;
  uint8_t elements;
  void* payload;
} IZBNSArray; //0x0a

struct XYResult{
  enum XY_ERROR error;
  void* result;
};

#define XYObjects
#endif

struct XYResult* newObject(char id[2], void* payload);

struct ArrayItr {
  void* start;
  void* end;
  uint16_t step;
  void* (*next)(struct ArrayItr*);
  void*(*previous)(struct ArrayItr*);
};

struct BoundWitness{
  uint32_t size;
  struct ArrayItr keys; //SZBNSArray
  struct ArrayItr SignedPayload; //IZBNSArray
  struct ArrayItr UnsignedPayload; //IZBNSArray
  struct ArrayItr signatures; //SZBNSArray
};

struct XYObject{
  char id[2];
  void* payload;
  char* (*GetId)(struct XYObject*); // Fetch the above id object and return it.
  void* (*GetPayload)(struct XYObject*); // Fetch the above payload pointer object and return it.
};

enum XY_ERROR{
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

struct XYResult* preallocated_result;
