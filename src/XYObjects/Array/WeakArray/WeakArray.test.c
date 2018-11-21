#include <stdio.h>
#include <stdlib.h>
#include "WeakArray.h"
//#include "../../XYObjectHeader.h"
#define XY_DEBUGMODE
#ifdef XY_DEBUGMODE
#define XY_ASSERT_EQUAL(A, B) XY_ASSERT_EQUAL_ex(A, B, __FILE__, __LINE__)
#define XY_ASSERT_EQUAL_ex(A, B, file, line) if(A != B){ printf(RED "%s@%d Assert Failed\n" RESET, file, line); score = score + 1; }
#elif
#define XY_ASSERT_EQUAL(A, B) if(A != B){ score = score + 1; }
#endif
int testAdd();
int testGap();

int main(){
    int score = 0;
    score += testAdd();
    score += testGap();
    return score;
}

int testAdd(){
    int score = 0;

    XYHeaderFlags_t signatureFlags;
    signatureFlags.typed = 0;
    signatureFlags.iteratable = 1;
    signatureFlags.lengthType = 1;

    XYObjectHeader_t signatureHeader;
    signatureHeader.flags = signatureFlags;
    signatureHeader.type = 0x12;

    XYObject_t signatureObject;
    signatureObject.header = &signatureHeader;

    XYHeaderFlags_t typedFlags;
    typedFlags.typed = 1;
    typedFlags.iteratable = 1;
    typedFlags.lengthType = (uint8_t)2;

    XYObjectHeader_t arrayHeader;
    arrayHeader.flags = typedFlags;
    arrayHeader.type = TYPE_ARRAY;

    XYObject_t arrayObject;
    arrayObject.header = &arrayHeader;

    arrayObject.payload = malloc(sizeof(char)*(67+67+4));
    *(char*)&arrayObject.payload[3] = 4; // Set Size of array

    char* endPtr;
    XYObject_t* self = &arrayObject;

    endPtr = arrayObject.payload + XYObject_getLength(self).value.ui;
    XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4);

    WeakArray_add(self, signatureHeader, 67);
    XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4+67);
    XY_ASSERT_EQUAL(*(uint8_t*)(((&arrayObject)->payload)+4), *(uint8_t*)&signatureFlags ); 
    
    WeakArray_add(self, signatureHeader, 67);
    XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4+67+67);
    XY_ASSERT_EQUAL(*(uint8_t*)(((&arrayObject)->payload)+4+67), *(uint8_t*)&signatureFlags ); 

    endPtr = arrayObject.payload + XYObject_getLength(self).value.ui;
    XY_ASSERT_EQUAL(*(endPtr-(sizeof(char)*67)), *(uint8_t*)&signatureFlags );

    return score;
}

int testGap(){
    int score = 0;

    XYHeaderFlags_t signatureFlags;
    signatureFlags.typed = 0;
    signatureFlags.iteratable = 1;
    signatureFlags.lengthType = 1;

    XYObjectHeader_t signatureHeader;
    signatureHeader.flags = signatureFlags;
    signatureHeader.type = 0x12;

    XYObject_t signatureObject;
    signatureObject.header = &signatureHeader;

    XYHeaderFlags_t typedFlags;
    typedFlags.typed = 1;
    typedFlags.iteratable = 1;
    typedFlags.lengthType = (uint8_t)2;

    XYObjectHeader_t arrayHeader;
    arrayHeader.flags = typedFlags;
    arrayHeader.type = TYPE_ARRAY;

    XYObject_t arrayObject;
    arrayObject.header = &arrayHeader;

    const uint8_t totalObjects = 5;
    const uint8_t overallocation = 10;

    arrayObject.payload = malloc(sizeof(char)*((((67)*totalObjects)+4)+overallocation));
    *(char*)&arrayObject.payload[3] = 4; // Set Size of array

    char* endPtr;
    XYObject_t* self = &arrayObject;
    XYArrayItr_t itr = WeakArrayIterator(self);
    WeakArray_add(self, signatureHeader, 6);
    memset(((char*)itr.indexPtr)+4, 3, 2);
    *(char*)&itr.indexPtr[2] = 0;
    *(char*)&itr.indexPtr[3] = 4;
    for(int i=0; i<totalObjects; i++){
        IteratorNext(&itr);
        memset(((char*)itr.indexPtr)+4, 3, 2);
        *(char*)&itr.indexPtr[2] = 0;
        *(char*)&itr.indexPtr[3] = 4;
        WeakArray_add(self, signatureHeader, 6);
        XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4+(6*(i+2)));
        XY_ASSERT_EQUAL(*(uint8_t*)(((&arrayObject)->payload)+(4+(6*(i+1)))), *(uint8_t*)&signatureFlags ); 
        
    }
   
    endPtr = arrayObject.payload + XYObject_getLength(self).value.ui;
    XY_ASSERT_EQUAL(*(endPtr-(sizeof(char)*6)), *(uint8_t*)&signatureFlags );

    WeakArray_gap(&arrayObject, 3, 12, 2);
    char* newEndPtr = arrayObject.payload + XYObject_getLength(self).value.ui;

    XY_ASSERT_EQUAL(*(newEndPtr-(sizeof(char)*6*2)), *(uint8_t*)&signatureFlags );
    XY_ASSERT_EQUAL(newEndPtr-2, endPtr );

    WeakArray_gap(&arrayObject, 2, 18, 5);
    newEndPtr = arrayObject.payload + XYObject_getLength(self).value.ui;

    XY_ASSERT_EQUAL(*(newEndPtr-(sizeof(char)*6*2)), *(uint8_t*)&signatureFlags );
    XY_ASSERT_EQUAL(newEndPtr-2-5, endPtr );

    return score;
}

int testAddArray(){
    int score = 0;
    return score;
}