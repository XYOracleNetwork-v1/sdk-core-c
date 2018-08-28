/**
 * @Author: Nate Brune
 * @Date:   10-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: main.c
 * @Last modified by:   Nate Brune
 * @Last modified time: 10-Aug-2018
 * @Copyright: XY - The Findables Company
 */

#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "serializer.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>


int main(){
  preallocated_result = malloc(sizeof(struct XYResult));
  if(preallocated_result){
      XYResult* result = initTable();
      if(result->error == OK){
      char testArray[9] = { 0x00, 0x00, 0x00, 0x09, 0x09, 0x01, 0x09, 0x09, 0x01 };
      char id[2];
      id[0] = 0x01;
      id[1] = 0x03;
      result = lookup(id);
      Object_Creator* arrayCreator = result->result;
      result = arrayCreator->fromBytes(&testArray);
      if(result->error == OK){
        IntStrongArray* array = (IntStrongArray*)result->result;
        printf("Array id: %d %d\n", array->id[0], array->id[1]);
        printf("Array size: %d\n", array->size);
        printf("End fromBytes.\n");

        char array_id[2];
        array_id[0] = 0x01;
        array_id[1] = 0x06;
        result = newObject(array_id, array);
        XYObject* object = result->result;
        if(result->error == OK){
          result = arrayCreator->toBytes(result->result);
          IntStrongArray* datapointer = result->result;
          printf("Array id: %d %d\n", array->id[0], array->id[1]);
          printf("Array size: %d\n", array->size);
          printf("End toBytes.\n");
        }
        char rssi_id[2];
        rssi_id[0] = 0x09;
        rssi_id[1] = 0x01;
        result = arrayCreator->create(rssi_id, NULL);
        if(result->error == OK){
          XYObject* arrayObject = result->result;
          IntStrongArray* arrayPointer = arrayObject->payload;
          uint8_t rssi1 = 0x11;
          uint8_t rssi2 = 0x12;
          uint8_t rssi3 = 0x13;
          result = lookup(rssi_id);
          if(result->error == OK){
            Object_Creator* rssiCreator = result->result;
            XYResult* result1 = rssiCreator->create(rssi_id, &rssi1);
            XYResult* result4 = arrayPointer->add(arrayPointer, result1->result);

            XYResult* result2 = rssiCreator->create(rssi_id, &rssi2);
            XYResult* result5 = arrayPointer->add(arrayPointer, result2->result);

            XYResult* result3 = rssiCreator->create(rssi_id, &rssi3);
            XYResult* result6 = arrayPointer->add(arrayPointer, result3->result);
            XYResult* result7 = arrayPointer->get(arrayPointer, 0);
            XYObject* rssi_object7 = result7->result;
            XYResult* result8 = arrayPointer->get(arrayPointer, 1);
            XYObject* rssi_object8 = result7->result;
            XYResult* result9 = arrayPointer->get(arrayPointer, 2);
            XYObject* rssi_object9 = result7->result;
            printf("End create Bytes.\n");
          }

        }
      }
      else {
        return -1;
      }
    }
    else{
      return -1;
    }
  }
  else{
    return -1;
  }
  return 0;
}
