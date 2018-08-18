#include "xyobject.h"

typedef struct {
  int (*put)(char*, char*);
  XYObject* (*get)(char*);
  XYObject* (*GetAllKeys)();
  int (*remove)(char*);
  void (*contains)(char*, void (*f)(char*));
} storageProvider;
