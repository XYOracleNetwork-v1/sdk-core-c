#include <stdint.h>

typedef struct XYObject XYObject, *xop;

struct XYObject{
  char id[2];
  void* payload;
  char* (*GetId)(XYObject*);
  void* (*GetPayload)(XYObject*);
};

typedef struct {
  uint16_t size;
  uint8_t major;
  uint8_t minor;
  uint16_t elements;
  void* payload;
} SZSNSArray; //0x01

typedef struct {
  uint8_t major;
  uint8_t minor;
  uint16_t elements;
  void* payload;
} NZSNSArray; //0x02

typedef struct {
  uint16_t size;
  uint16_t elements;
  void* payload;
} SZSNWArray; //0x03


typedef struct {
  uint8_t size;
  uint8_t major;
  uint8_t minor;
  uint8_t elements;
  void* payload;
} BZBNSArray; //0x04

typedef struct {
  uint8_t size;
  uint8_t elements;
  void* payload;
} BZBWSArray; //0x05

typedef struct {
  uint16_t size;
  uint8_t major;
  uint8_t minor;
  uint8_t elements;
  void* payload;
} SZBNSArray; //0x06

typedef struct {
  uint16_t size;
  uint8_t elements;
  void* payload;
} SZBNWArray; //0x07

typedef struct {
  uint32_t size;
  uint8_t major;
  uint8_t minor;
  uint16_t elements;
  void* payload;
} IZSNSArray; //0x08

typedef struct {
  uint32_t size;
  uint16_t elements;
  void* payload;
} IZSNWArray; //0x09

typedef struct {
  uint32_t size;
  uint8_t major;
  uint8_t minor;
  uint8_t elements;
  void* payload;
} IZBNSArray; //0x0a

typedef struct ArrayItr ArrayItr, *aip;

struct ArrayItr {
  void* start;
  void* end;
  uint16_t step;
  void* (*next)(struct ArrayItr*);
  void*(*previous)(struct ArrayItr*);
};

typedef struct {
  uint32_t size;
  ArrayItr keys; //SZBNSArray
  ArrayItr SignedPayload; //IZBNSArray
  ArrayItr UnsignedPayload; //IZBNSArray
  ArrayItr signatures; //SZBNSArray
} BoundWitness;
