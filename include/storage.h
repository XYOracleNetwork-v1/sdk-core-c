#include "xyobject.h"

typedef struct {
  int (*put)(char*, char*); // Insert into hashmap
  struct XYObject* (*get)(char*); // Get from hashmap
  struct XYObject* (*GetAllKeys)(); // Get all keys in hashmap
  int (*remove)(char*); // Remove element from hashmap
  void (*contains)(char*, void (*f)(char*)); // Is element in hashmap
} storageProvider;
