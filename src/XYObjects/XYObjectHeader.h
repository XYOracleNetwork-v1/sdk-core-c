#include <stdint.h>

//bit fields for the flags in the first byte of the header
typedef struct XYHeaderFlags {
  uint8_t lengthType: 2;
  uint8_t iteratable: 1;
  uint8_t typed: 1;
  uint8_t reserved: 4;
} XYHeaderFlags_t;

// the type of the object
typedef struct XYHeader {
  XYHeaderFlags_t flags;
  uint8_t type;
} XYHeader_t;

#define XY_HEADER_OFFSET (0)
#define XY_HEADER_LENGTH (2)

// Masks for deternining the method of length discovery for a given major
#define XY_LENGTH_1BYTE      (0x0)
#define XY_LENGTH_2BYTE      (0x1)
#define XY_LENGTH_4BYTE      (0x2)
#define XY_LENGTH_8BYTE      (0x3)

#define XY_LENGTH_OFFSET (XY_HEADER_LENGTH)