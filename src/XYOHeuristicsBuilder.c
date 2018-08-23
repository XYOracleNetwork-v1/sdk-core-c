/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: XYOHeuristicsBuilder.c
 * @Last modified by:   Nate Brune
 * @Last modified time: 09-Aug-2018
 * @Copyright: XY - The Findables Company
 */
 #include "xyobject.h"
 #include <string.h>

struct XYResult* Heuristic_RSSI_Creator_create(char id[2], int* rssi){
  new_object(id, rssi);
}
struct XYResult* Heuristic_RSSI_Creator_createFromBytes(char* heuristic_data){
  char id[2];
  memcpy(id, heuristic_data, 2);
  int rssi = heuristic_data[3];
  new_object(id, &rssi);
}
struct XYResult* Heuristic_RSSI_Creator_encode(XYObject* user_XYObect){
  char encoded_bytes[3];
  encoded_bytes[0] = 0x02;
  encoded_bytes[1] = 0x03;
  char* rssi = (char*)user_XYObect->payload;
  encoded_bytes[2] = rssi[2];
}
