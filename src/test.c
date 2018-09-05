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
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>


int test(){
  preallocated_result = malloc(sizeof(struct XYResult));
  if(preallocated_result){
      XYResult* result = initTable();
      char id[2];
      id[0] = 0x01;
      id[1] = 0x06;
      XYResult* lookup_result = lookup(id);
      Object_Creator* IntWeakArray_creator = lookup_result->result;
      XYResult* create_result = IntWeakArray_creator->create(NULL, NULL);
      XYObject* IntWeakArray_object = create_result->result;
      IntWeakArray* IntWeakArray_raw = IntWeakArray_object->payload;

      id[0] = 0x10;
      id[1] = 0x01;
      lookup_result = lookup(id);
      Object_Creator* textHeuristic_creator = lookup_result->result;
      char* testText1 = "1";
      char buffer1[5] = { 0x10, 0x01, 3, 0, 0 };
      strcpy(&buffer1[4], testText1);
      create_result = textHeuristic_creator->fromBytes(buffer1);
      XYObject* testObject = create_result->result;
      IntWeakArray_raw->add(IntWeakArray_raw, create_result->result);

      id[0] = 0x10;
      id[1] = 0x01;
      char* testText2 = "22";
      char buffer2[6] = { 0x10, 0x01, 4, 0, 0, 0 };
      strcpy(&buffer2[4], testText2);
      create_result = textHeuristic_creator->fromBytes(buffer2);
      IntWeakArray_raw->add(IntWeakArray_raw, create_result->result);

      uint8_t rssi = 0x05;
      char rssi_id[2];
      rssi_id[0] = 0x08;
      rssi_id[1] = 0x01;
      result = lookup(rssi_id);
      Object_Creator* rssi_creator = result->result;
      create_result = rssi_creator->create(rssi_id, &rssi);
      IntWeakArray_raw->add(IntWeakArray_raw, create_result->result);

      char* publicKey = "4444";
      char bufferPubKey[10] = { 0x02, 0x07, 0x04, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00 };
      char public_key_id[2];
      public_key_id[0] = 0x02;
      public_key_id[1] = 0x07;
      result = lookup(public_key_id);
      Object_Creator* NPK_creator = result->result;
      strcpy(&bufferPubKey[6], publicKey);
      create_result = NPK_creator->fromBytes(&bufferPubKey);
      XYObject* NextPublicKey_object = create_result->result;
      XYResult* new_object_result = newObject(public_key_id, NextPublicKey_object);
      IntWeakArray_raw->add(IntWeakArray_raw, new_object_result->result);

      char* hashBytes = "SHA256 is the best SHA123456789K";
      char bufferHash[36] = { 0x02, 0x06, 0x03, 0x05 };
      char hash_id[2];
      hash_id[0] = 0x02;
      hash_id[1] = 0x06;
      result = lookup(hash_id);
      Object_Creator* NH_creator = result->result;
      strcpy(&bufferHash[4], hashBytes);
      create_result = NH_creator->fromBytes(&bufferHash);
      XYObject* PreviousHash_object = create_result->result;
      new_object_result = newObject(hash_id, PreviousHash_object);
      IntWeakArray_raw->add(IntWeakArray_raw, new_object_result->result);

      char* testText3 = "333";
      char buffer3[7] = { 0x10, 0x01, 5, 0, 0, 0, 0};
      strcpy(&buffer3[4], testText3);
      create_result = textHeuristic_creator->fromBytes(buffer3);
      IntWeakArray_raw->add(IntWeakArray_raw, create_result->result);


      uint32_t index = 1;
      char index_id[2];
      index_id[0] = 0x02;
      index_id[1] = 0x05;
      result = lookup(index_id);
      Object_Creator* index_creator = result->result;
      create_result = index_creator->create(index_id, &index);
      IntWeakArray_raw->add(IntWeakArray_raw, create_result->result);

      char payloadBuffer[21] = { 0, 0, 0, 21, 0, 0, 0, 10, 0x02, 0x05, 0, 0, 0, 65, 0, 0, 0, 7, 0x09, 0x01, 56 };
      char payload_id[2];
      payload_id[0] = 0x02;
      payload_id[1] = 0x04;
      result = lookup(payload_id);
      Object_Creator* payload_creator = result->result;
      XYResult* fromBytes_result = payload_creator->fromBytes(&payloadBuffer);
      Payload* Payload_raw = fromBytes_result->result;
      IntWeakArray* array1 = Payload_raw->signedHeuristics;
      IntWeakArray* array2 = Payload_raw->unsignedHeuristics;
      XYResult* newObject_result = newObject(payload_id, Payload_raw);
      XYObject* Payload_object = newObject_result->result;
      breakpoint();
      IntWeakArray_raw->add(IntWeakArray_raw, Payload_object);


      /* Strong Array Test */
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
        rssi_id[0] = 0x08;
        rssi_id[1] = 0x01;
        result = arrayCreator->create(rssi_id, NULL);

        if(result->error == OK){
          XYObject* arrayObject = result->result;
          IntStrongArray* arrayPointer = arrayObject->payload;
          uint8_t rssi1 = 0x10;
          uint8_t rssi2 = 0x12;
          uint8_t rssi3 = 0x13;
          rssi_id[0] = 0x08;
          rssi_id[1] = 0x01;
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
            XYResult* array_add_result = IntWeakArray_raw->add(IntWeakArray_raw, arrayObject);
            XYResult* get_result = IntWeakArray_raw->get(IntWeakArray_raw, 1);
            XYObject* got_object = get_result->result;

            XYResult* toBytes_result = IntWeakArray_creator->toBytes(IntWeakArray_object);
            char* theBytes = toBytes_result->result;

            breakpoint();
            printf("end test\n");
          }

        }
      }
      else {
        return -1;
      }
    }

  }
  else{
    return -1;
  }

  return 0;
}
