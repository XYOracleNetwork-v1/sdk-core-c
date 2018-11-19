#include <stdio.h>
#include <stdlib.h>
#include "WeakArray.h"
//#include "../../XYObjectHeader.h"

#define XY_ASSERT_EQUAL(A, B) if(A != B){ score = score + 1;};

int main(){
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
    signatureObject.payload = malloc(sizeof(char)*67);
    memset(signatureObject.payload, 0x03, 67);
    *(char*)&signatureObject.payload[0] = 67;

    XYObject_t arrayObject;
    XYObject_t* self = &arrayObject;
    XYHeaderFlags_t untypedFlags;
    untypedFlags.typed = 0;
    untypedFlags.iteratable = 1;
    untypedFlags.lengthType = (uint8_t)2; //TODO: Investigate linter warns

    XYObjectHeader_t arrayHeader;
    arrayHeader.flags = untypedFlags;
    arrayHeader.type = TYPE_ARRAY_WEAK;

    arrayObject.header = &arrayHeader;
    arrayObject.payload = malloc(sizeof(char)*(67+4));
    *(char*)&arrayObject.payload[3] = 4;

    XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4);
    WeakArray_add(self, signatureHeader, 67);
    XY_ASSERT_EQUAL(XYObject_getLength(self).value.ui, 4+67);
    XY_ASSERT_EQUAL(*(uint8_t*)(((&arrayObject)->payload)+4), *(uint8_t*)&signatureFlags )
    return score;

    


}