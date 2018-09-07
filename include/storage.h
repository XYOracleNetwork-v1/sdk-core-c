#include "xyobject.h"
#ifndef STORAGE_H
typedef struct StorageProvider StorageProvider;

#define STORAGE_H
#endif

struct StorageProvider{
  XYResult* (*write)(ByteArray*, ByteArray*); // Insert into dictionary
  XYResult* (*read)(ByteArray*); // Get from dictionary
  XYObject* (*GetAllKeys)(); // Get all keys in dictionary TODO
  XYResult* (*delete)(ByteArray*); // Remove element from hashmap TODO
  XYResult* (*contains)(ByteArray*, void (*f)(ByteArray*)); // Is element in hashmap TODO
} storageProvider;
