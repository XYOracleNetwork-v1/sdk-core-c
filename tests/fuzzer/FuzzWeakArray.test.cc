#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


extern "C" {
#include "../../src/XYObjects/Array/WeakArray/WeakArray.h"
#include "../../src/XYObjects/Array/Iterator.h"
/*    XYResult_t WeakArray_add(XYObject_t *self,
                         XYObject_t* newItemHeader,
                         uint32_t newItemLength); 
*/
}

extern "C" int LLVMFuzzerTestOneInput(XYObject_t *self,
                              XYObject_t* newItem,
                              uint32_t newItemLength) {\
  WeakArray_add(self, newItem, newItemLength);
  return 0;  // Non-zero return values are reserved for future use.
}