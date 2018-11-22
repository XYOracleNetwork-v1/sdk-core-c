//#include "xyresult.h"
//#include "XYObjects/XYObject.h"

XYResult_t* Payload_creator_create(const char id[2], void* user_data);
XYResult_t* Payload_creator_fromBytes(char* payload_data);
XYResult_t* Payload_creator_toBytes(XYObject_t* user_XYObject);
