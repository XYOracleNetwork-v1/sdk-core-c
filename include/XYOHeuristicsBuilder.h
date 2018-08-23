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

 union SizeUnion {
     int sizeIdentifierSize;
     int defaultSize;
 }

 struct Heuristic_RSSI_Creator {
   union SizeUnion sizeBytes;
   XYResult* (*create)(char[2], int);
   XYResult* (*createFromBytes)(char*);
   XYResult* (*encode)(XYObject*)
 };
