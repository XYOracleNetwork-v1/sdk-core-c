#include "xyo.h"

struct ArrayItr* unpackArray(char* data);
/*
struct ByteStrongArrayCreator {
  char id[2];
  XYObject* (*create)(char[2]);
  XYObject* (*createFromBytes)(char*);
  char*     (*encode)(XYObject*);
};
*/
#define RETURN_ERROR(ERR)                                   \
if(ERR == ERR_INSUFFICIENT_MEMORY){                         \
  preallocated_result->error = ERR_INSUFFICIENT_MEMORY;     \
  preallocated_result->result = 0;                          \
  return preallocated_result;                               \
} else {                                                    \
  XYResult* return_result = malloc(sizeof(XYResult));       \
    if(return_result != NULL){                              \
      return_result->error = ERR;                           \
      return_result->result = 0;                            \
      return return_result;                                 \
    }                                                       \
    else {                                                  \
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY; \
      preallocated_result->result = 0;                      \
      return preallocated_result;                           \
    }                                                       \
}
