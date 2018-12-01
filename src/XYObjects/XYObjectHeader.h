#include <stdint.h>

//bit fields for the flags in the first byte of the header
typedef struct XYHeaderFlags {
  uint8_t reserved: 4;      /*----xxxx*/
  uint8_t typed: 1;         /*---x----*/
  uint8_t iteratable: 1;    /*--x-----*/
  uint8_t lengthType: 2;    /*xx------*/
} XYHeaderFlags_t;

// the type of the object
typedef struct XYObjectHeader {
  XYHeaderFlags_t flags;
  uint8_t type;
} XYObjectHeader_t;

#define XY_HEADER_OFFSET (0)
#define XY_HEADER_LENGTH (sizeof(XYObjectHeader_t))

// Masks for deternining the method of length discovery for a given major
#define XY_LENGTH_1BYTE      (0x00)
#define XY_LENGTH_2BYTE      (0x01)
#define XY_LENGTH_4BYTE      (0x02)
#define XY_LENGTH_8BYTE      (0x03)

#define XY_LENGTH_OFFSET (XY_HEADER_LENGTH)
