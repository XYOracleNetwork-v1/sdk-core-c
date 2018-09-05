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


int main(){
  preallocated_result = malloc(sizeof(struct XYResult));
  if(preallocated_result){
    initTable();
    char BoundWitness_id[2] = { 0x02, 0x01 };
    char ShortStrongArray_id[2] = { 0x01, 0x02 };
    char ShortWeakArray_id[2] = { 0x01, 0x05 };
    char IntStrongArray_id[2] = { 0x01, 0x03 };
    char IntWeakArray_id[2] = { 0x01, 0x06 };
    char Payload_id[2] = { 0x02, 0x04 };
    char SignatureSet_id[2] = { 0x02, 0x03 };
    char NextPublicKey_id[2] = { 0x02, 0x07 };
    char Rssi_id[2] = { 0x08, 0x01 };
    XYResult* lookup_result = lookup(BoundWitness_id);
    if(lookup_result->error == OK){
      Object_Creator* BoundWitness_creator = lookup_result->result;
      //free(lookup_result);
      BoundWitness* BoundWitness_raw = malloc(sizeof(BoundWitness));
      XYObject* BoundWitness_object;
      if(BoundWitness_raw){
        XYResult* create_result = BoundWitness_creator->create(BoundWitness_id, BoundWitness_raw);
        if(create_result->error != OK){
          return create_result->result;
        }
        BoundWitness_object = create_result->result;
      } else {
        RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
      }
      lookup_result = lookup(ShortStrongArray_id);
      if(lookup_result->error == OK){
        Object_Creator* SSA_Creator = lookup_result->result;
        //free(lookup_result);
        lookup_result = lookup(IntStrongArray_id);
        if(lookup_result->error == OK){
          Object_Creator* ISA_Creator = lookup_result->result;
          //free(lookup_result);
          XYResult* create_result = SSA_Creator->create(ShortWeakArray_id, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          // Set up Public Keys Array
          XYObject*  publicKeys_object = create_result->result;
          ShortStrongArray* publicKeys_raw = publicKeys_object->payload;
          //free(create_result);
          char bufferPubKey[8] = { 0x02, 0x07, 0x04, 0x03, 0x00, 0x04, 0x00, 0x00 };
          lookup_result = lookup(NextPublicKey_id);
          Object_Creator* NPK_creator = lookup_result->result;
          //free(lookup_result);
          create_result = NPK_creator->fromBytes(&bufferPubKey);
          XYObject* NextPublicKey_object = create_result->result;
          //free(create_result);
          XYResult* newObject_result = newObject(NextPublicKey_id, NextPublicKey_object);
          if(newObject_result->error != OK){
            return newObject_result->result;
          }
          XYResult* add_result = publicKeys_raw->add(publicKeys_raw, newObject_result->result);
          if(add_result->error != OK){
            return add_result->result;
          }
          //free(newObject_result);

          // Set up Payload Array
          create_result = ISA_Creator->create(Payload_id, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          XYObject* payload_object = create_result->result;
          IntStrongArray* payload_raw = payload_object->payload;
          //free(create_result);
          lookup_result = lookup(ShortWeakArray_id);
          if(lookup_result->error != OK){
            return lookup_result->result;
          }
          /**/
          Object_Creator* SWA_Creator = lookup_result->result;
          create_result = SWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          XYObject* user_signedHeuristics_object = create_result->result;
          IntWeakArray* user_signedHeuristics = user_signedHeuristics_object->payload;
          //free(create_result);

          create_result = SWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          XYObject* user_unsignedHeuristics_object = create_result->result;
          IntWeakArray* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
          //free(create_result);

          // Add RSSI Heuristics to unsigned and signed payload
          lookup_result = lookup(Rssi_id);
          if(lookup_result->error != OK){
            return lookup_result->result;
          }
          Object_Creator* Rssi_Creator = lookup_result->result;
          char rssi_val = 0xC4;
          create_result = Rssi_Creator->create(Rssi_id, &rssi_val);
          if(create_result->error != OK){
            return create_result->result;
          }
          add_result = user_signedHeuristics->add(user_signedHeuristics, create_result->result);
          if(add_result->error != OK){
            return add_result->result;
          }
          //free(add_result);
          add_result = user_unsignedHeuristics->add(user_unsignedHeuristics, create_result->result);
          if(add_result->error != OK){
            return add_result->result;
          }
          breakpoint();
          add_result = payload_raw->add(payload_raw, user_signedHeuristics_object);
          add_result = payload_raw->add(payload_raw, user_unsignedHeuristics_object);
          //free(add_result);
          //free(create_result);
          //free(Rssi_Creator);
          // Set up Signatures Array
          create_result = SSA_Creator->create(SignatureSet_id, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          XYObject* signatures_object = create_result->result;
          ShortStrongArray* signatures_raw = signatures_object->payload;
          //free(create_result);
          /*
          lookup_result = lookup(ShortWeakArray_id);
          if(lookup_result->error != OK){
            return lookup_result->result;
          }
          Object_Creator* SWA_Creator = lookup_result->result;
          */
          create_result = SWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->result;
          }
          XYObject* user_signatures_object = create_result->result;
          ShortWeakArray* user_signatures = user_signatures_object->payload;

          char rssi_val2 = 0x44;
          create_result = Rssi_Creator->create(Rssi_id, &rssi_val2);
          if(create_result->error != OK){
            return create_result->result;
          }

          add_result = user_signatures->add(user_signatures, create_result->result);
          //free(create_result);
          newObject_result = newObject(ShortWeakArray_id, user_signatures);
          if(newObject_result->error != OK){
            return newObject_result->result;
          }
          //breakpoint();
          add_result = signatures_raw->add(signatures_raw, newObject_result->result);
          if(add_result->error != OK){
            return add_result->result;
          }
          if(publicKeys_raw && payload_raw && signatures_raw){
            BoundWitness_raw->publicKeys = publicKeys_raw;
            BoundWitness_raw->payload = payload_raw;
            BoundWitness_raw->signatures = signatures_raw;
            BoundWitness_raw->size = publicKeys_raw->size + payload_raw->size + signatures_raw->size + (4 * sizeof(char));
            XYResult* toBytes_result = BoundWitness_creator->toBytes(BoundWitness_object);
            char* theBytes = toBytes_result->result;
            breakpoint();
            printf("End Test <3");
          }
        } else {
          return lookup_result->result;
        }
      } else {
        return lookup_result->result;
      }
    }
  }
  else{
    return -1;
  }

  return 0;
}
