#include <stdint.h>
#include "WeakArray/WeakArray.h"
#include "../XYObject.h"
#include "../XYObjectTypes.h"

typedef struct arrayItr
{
  XYObject_t array;
  uint16_t index;
  void *indexPtr;
  char* gap;
} XYArrayItr_t;

XYArrayItr_t WeakArrayIterator(XYObject_t* self);
XYObject_t IteratorNext(XYArrayItr_t* itr);
XYObject_t IteratorGet(XYArrayItr_t* itr);

#define XYCHECK_ITRERATOR(_ITR_) if(( (XYArrayItr_t)_ITR_.header != NULL && (XYArrayItr_t)_ITR_.payload != NULL){ result.status = ERR_CRITICAL}

XYResult_t Iterator_bookmark(XYObject_t *self, uint32_t element, uint32_t bytesAfter, uint32_t offset, XYArrayItr_t* optional);