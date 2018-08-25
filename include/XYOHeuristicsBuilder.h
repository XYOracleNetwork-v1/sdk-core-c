/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: XYOHeuristicsBuilder.h
 * @Last modified by:   Nate Brune
 * @Last modified time: 09-Aug-2018
 * @Copyright: XY - The Findables Company
 */
#ifndef XYOHeuristicsBuilder_H
#include "xyo.h"

typedef struct Object_Creator Object_Creator;

 struct Object_Creator {
   int        sizeIdentifierSize;
   int        defaultSize;
   struct XYResult*  (*create)(char[2], void*);
   struct XYResult*  (*fromBytes)(char*);
   struct XYResult*  (*toBytes)(struct XYObject*);
 };

XYResult* Heuristic_RSSI_Creator_create(char id[2], void* rssi);
XYResult* Heuristic_RSSI_Creator_fromBytes(char* heuristic_data);
XYResult* Heuristic_RSSI_Creator_toBytes(struct XYObject* user_XYObect);

XYResult* ByteStrongArray_creator_create(char id[2], void* user_data);
XYResult* ByteStrongArray_creator_fromBytes(char* data);
XYResult* ByteStrongArray_creator_toBytes(struct XYObject* user_XYObect);
XYResult* ByteStrongArray_add(ByteStrongArray* self_ByteStrongArray, XYObject* user_XYObject);
#define XYOHeuristicsBuilder_H
#endif
