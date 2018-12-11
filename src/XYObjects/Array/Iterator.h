#include <stdint.h>
#include "WeakArray/WeakArray.h"
#include "../XYObject.h"
#include "../XYObjectTypes.h"
#include "../../ByteArray.h"

#ifdef BUILD_ITERATORS
typedef struct arrayItr
{
  XYObjectHeader_t* header;
  char* base;
  XYObjectHeader_t* innerHeader;
  char *indexPtr;
} XYArrayItr_t;

XYArrayItr_t WeakArrayIterator(XYObjectHeader_t* header, char* buffer);
XYObject_t IteratorNext(XYArrayItr_t* itr);
XYObject_t IteratorGet(XYArrayItr_t* itr);

#define XYCHECK_ITRERATOR(_ITR_) if(( (XYArrayItr_t)_ITR_.header != NULL && (XYArrayItr_t)_ITR_.payload != NULL){ result.status = ERR_CRITICAL}
#define Iterator_gap(_self, _element, _bytesAfter, _offset) Iterator_bookmark(_self, _element, _bytesAfter, _offset, NULL)

XYResult_t Iterator_bookmark(XYObject_t *self, uint32_t element, uint32_t bytesAfter, uint32_t offset, XYArrayItr_t* optional);
XYResult_t Iterator_insert(XYObject_t* self, uint32_t element, uint32_t offset, uint32_t totalBytes, char* bytes);
#endif
