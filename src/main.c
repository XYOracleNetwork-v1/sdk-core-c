/**
 ****************************************************************************************
 *
 * @file crypto.c
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include "xyo.h"
#include "XYOHeuristicsBuilder.h"
#include "BoundWitness.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

XYResult* preallocated_result;

#ifdef BUILD_MAIN

int main(){
  preallocated_result = malloc(sizeof(struct XYResult));

  if(preallocated_result){
    initTable();

    XYResult* lookup_result = lookup((char*)&BoundWitness_id);
    if(lookup_result->error == OK){
      ObjectProvider* BoundWitness_creator = lookup_result->result;
      //free(lookup_result);
      BoundWitness* BoundWitness_raw = malloc(sizeof(BoundWitness));
      XYObject* BoundWitness_object;
      if(BoundWitness_raw){
        XYResult* create_result = BoundWitness_creator->create((char*)&BoundWitness_id, BoundWitness_raw);
        if(create_result->error != OK){
          return create_result->error;
        }
        BoundWitness_object = create_result->result;
      } else {
        return 1;
      }
      lookup_result = lookup((char*)&ShortStrongArray_id);
      if(lookup_result->error == OK){
        ObjectProvider* SSA_Creator = lookup_result->result;
        //free(lookup_result);
        lookup_result = lookup((char*)&IntStrongArray_id);
        if(lookup_result->error == OK){
          ObjectProvider* ISA_Creator = lookup_result->result;
          //free(lookup_result);
          XYResult* create_result = SSA_Creator->create((char*)&KeySet_id, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          // Set up Public Keys Array
          XYObject*  publicKeys_object = create_result->result;
          ShortStrongArray* publicKeys_raw = publicKeys_object->payload;

          lookup_result = lookup((char*)&ShortWeakArray_id);
          if(lookup_result->error != OK){
            return lookup_result->error;
          }
          ObjectProvider* SWA_Creator = lookup_result->result;
          create_result = SWA_Creator->create((char*)&ShortWeakArray_id, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          // Set up Public Keys Array
          XYObject*  keyset_object = create_result->result;
          ShortStrongArray* keySet_raw = keyset_object->payload;

          ECDSA_secp256k1_uncompressed* publicKey = malloc(sizeof(ECDSA_secp256k1_uncompressed));
          //free(create_result);
          strcpy(publicKey->point_x, "Hello");
          strcpy(publicKey->point_y, "World!");
          XYResult* newObject_result = newObject((char*)&ECDSASecp256k1_id, publicKey);
          if(newObject_result->error != OK){
            return newObject_result->error;
          }
          XYResult* add_result = keySet_raw->add(keySet_raw, newObject_result->result);
          if(add_result->error != OK){
            return add_result->error;
          }
          add_result = publicKeys_raw->add(publicKeys_raw, keyset_object);
          if(add_result->error != OK){
            return add_result->error;
          }
          //free(newObject_result);

          // Set up Payload Array
          create_result = ISA_Creator->create((char*)&Payload_id, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          XYObject* payload_object = create_result->result;
          IntStrongArray* payload_raw = payload_object->payload;
          //free(create_result);
          lookup_result = lookup((char*)&IntWeakArray_id);
          if(lookup_result->error != OK){
            return lookup_result->error;
          }

          ObjectProvider* IWA_Creator = lookup_result->result;
          create_result = IWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          XYObject* user_signedHeuristics_object = create_result->result;
          IntWeakArray* user_signedHeuristics = user_signedHeuristics_object->payload;
          //free(create_result);

          create_result = IWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          XYObject* user_unsignedHeuristics_object = create_result->result;
          IntWeakArray* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
          //free(create_result);

          // Add RSSI Heuristics to unsigned and signed payload
          lookup_result = lookup((char*)&Rssi_id);
          if(lookup_result->error != OK){
            return lookup_result->error;
          }
          ObjectProvider* Rssi_Creator = lookup_result->result;
          char rssi_val = 0xC4;
          create_result = Rssi_Creator->create((char*)&Rssi_id, &rssi_val);
          if(create_result->error != OK){
            return create_result->error;
          }
          add_result = user_signedHeuristics->add(user_signedHeuristics, create_result->result);
          if(add_result->error != OK){
            return add_result->error;
          }
          //free(add_result);
          add_result = user_unsignedHeuristics->add(user_unsignedHeuristics, create_result->result);
          if(add_result->error != OK){
            return add_result->error;
          }
          Payload* user_payload = malloc(sizeof(Payload));
          user_payload->size = user_signedHeuristics->size + user_unsignedHeuristics->size + 4;
          user_payload->signedHeuristics = user_signedHeuristics;
          user_payload->unsignedHeuristics = user_unsignedHeuristics;
          newObject_result = newObject((char*)&Payload_id, user_payload);
          add_result = payload_raw->add(payload_raw, newObject_result->result);
          Payload* user_payload2 = malloc(sizeof(Payload));
          user_payload2->size = user_signedHeuristics->size + user_unsignedHeuristics->size + 4;
          user_payload2->signedHeuristics = user_signedHeuristics;
          user_payload2->unsignedHeuristics = user_unsignedHeuristics;
          newObject_result = newObject((char*)&Payload_id, user_payload2);
          add_result = payload_raw->add(payload_raw, newObject_result->result);
          //free(add_result);
          //free(create_result);
          //free(Rssi_Creator);
          // Set up Signatures Array
          create_result = SSA_Creator->create((char*)&SignatureSet_id, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          XYObject* signatures_object = create_result->result;
          ShortStrongArray* signatures_raw = signatures_object->payload;
          //free(create_result);
          /*
          lookup_result = lookup(ShortWeakArray_id);
          if(lookup_result->error != OK){
            return lookup_result->result;
          }
          ObjectProvider* SWA_Creator = lookup_result->result;
          */
          create_result = SWA_Creator->create(NULL, NULL);
          if(create_result->error != OK){
            return create_result->error;
          }
          XYObject* user_signatures_object = create_result->result;
          ShortWeakArray* user_signatures = user_signatures_object->payload;

          char rssi_val2 = 0x44;
          create_result = Rssi_Creator->create((char*)&Rssi_id, &rssi_val2);
          if(create_result->error != OK){
            return create_result->error;
          }

          add_result = user_signatures->add(user_signatures, create_result->result);
          //free(create_result);
          newObject_result = newObject((char*)&ShortWeakArray_id, user_signatures);
          if(newObject_result->error != OK){
            return newObject_result->error;
          }
          //breakpoint();
          add_result = signatures_raw->add(signatures_raw, newObject_result->result);
          if(add_result->error != OK){
            return add_result->error;
          }
          if(publicKeys_raw && payload_raw && signatures_raw){
            BoundWitness_raw->publicKeys = publicKeys_raw;
            BoundWitness_raw->payloads = payload_raw;
            BoundWitness_raw->signatures = signatures_raw;
            BoundWitness_raw->size = publicKeys_raw->size + payload_raw->size + signatures_raw->size + (4 * sizeof(char));
            BoundWitness_raw->getSigningData = &BoundWitness_getSigningData;
            XYResult* toBytes_result = BoundWitness_creator->toBytes(BoundWitness_object);
            char* theBytes = toBytes_result->result;
            breakpoint();
            XYResult* GetSigningData_result = BoundWitness_raw->getSigningData(BoundWitness_raw);
            char* signingBytes = GetSigningData_result->result;
            breakpoint();
            printf("End Test <3");
          }
        } else {
          return lookup_result->error;
        }
      } else {
        return lookup_result->error;
      }
    }
  }
  else{
    return -1;
  }

  return 0;
}
#endif
