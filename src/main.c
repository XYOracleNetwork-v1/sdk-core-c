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

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "BoundWitness.h"   // includes "xyobject.h", "hash.h", <stdint.h>

XYResult_t* preallocated_result;

#ifdef BUILD_MAIN

int main(){
  
  preallocated_result = malloc(sizeof(XYResult_t*));    //TODO: wal, where is this freed?

  if(preallocated_result){
    initTable();

    XYResult_t* lookup_result = tableLookup((char*)&BoundWitness_id);
    
    if(lookup_result->error == OK){
      
      ObjectProvider_t* BoundWitness_creator = lookup_result->result;
      //free(lookup_result);
      BoundWitness* BoundWitness_raw = malloc(sizeof(BoundWitness));    //TODO: wal, where is this freed?
      XYObject_t* BoundWitness_object;
      
      if(BoundWitness_raw){
        
        XYResult_t* create_result = BoundWitness_creator->create((char*)&BoundWitness_id, BoundWitness_raw);
        
        if(create_result->error != OK){
          return create_result->error;
        }
        BoundWitness_object = create_result->result;
      } else {
        return 1;   //TODO: wal, should be a constant
      }
      
      lookup_result = tableLookup((char*)&ShortStrongArray_id);
      
      if(lookup_result->error == OK){
        
        ObjectProvider_t* SSA_Creator = lookup_result->result;
        //free(lookup_result);
        lookup_result = tableLookup((char*)&IntStrongArray_id);
        
        if(lookup_result->error == OK){
          
          ObjectProvider_t* ISA_Creator = lookup_result->result;
          //free(lookup_result);
          XYResult_t* create_result = SSA_Creator->create((char*)&KeySet_id, NULL);
          
          if(create_result->error != OK){
            return create_result->error;
          }
          
          // Set up Public Keys Array
          XYObject_t*  publicKeys_object = create_result->result;
          ShortStrongArray_t* publicKeys_raw = publicKeys_object->payload;

          lookup_result = tableLookup((char*)&ShortWeakArray_id);
          
          if(lookup_result->error != OK){
            return lookup_result->error;
          }
          
          ObjectProvider_t* SWA_Creator = lookup_result->result;
          create_result = SWA_Creator->create((char*)&ShortWeakArray_id, NULL);
          
          if(create_result->error != OK){
            return create_result->error;
          }
          
          // Set up Public Keys Array
          XYObject_t*  keyset_object = create_result->result;
          ShortStrongArray_t* keySet_raw = keyset_object->payload;

          ECDSA_secp256k1_uncompressed_t* publicKey = malloc(sizeof(ECDSA_secp256k1_uncompressed_t));   //TODO: wal, where is this freed?
          //TODO: wal, should check for any malloc errors
          //free(create_result);
          strcpy(publicKey->point_x, "Hello");
          strcpy(publicKey->point_y, "World!");
          XYResult_t* newObject_result = newObject((char*)&ECDSASecp256k1_id, publicKey);
          
          if(newObject_result->error != OK){
            return newObject_result->error;
          }
          
          XYResult_t* add_result = keySet_raw->add(keySet_raw, newObject_result->result);
          
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
          
          XYObject_t* payload_object = create_result->result;
          IntStrongArray_t* payload_raw = payload_object->payload;
          //free(create_result);
          lookup_result = tableLookup((char*)&IntWeakArray_id);
          
          if(lookup_result->error != OK){
            return lookup_result->error;
          }

          ObjectProvider_t* IWA_Creator = lookup_result->result;
          create_result = IWA_Creator->create(NULL, NULL);
          
          if(create_result->error != OK){
            return create_result->error;
          }
          
          XYObject_t* user_signedHeuristics_object = create_result->result;
          IntWeakArray_t* user_signedHeuristics = user_signedHeuristics_object->payload;
          //free(create_result);

          create_result = IWA_Creator->create(NULL, NULL);
          
          if(create_result->error != OK){
            return create_result->error;
          }
          
          XYObject_t* user_unsignedHeuristics_object = create_result->result;
          IntWeakArray_t* user_unsignedHeuristics = user_unsignedHeuristics_object->payload;
          //free(create_result);

          // Add RSSI Heuristics to unsigned and signed payload
          lookup_result = tableLookup((char*)&Rssi_id);
          
          if(lookup_result->error != OK){
            return lookup_result->error;
          }
          
          ObjectProvider_t* Rssi_Creator = lookup_result->result;
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
          
          Payload_t* user_payload = malloc(sizeof(Payload_t));    //TODO: wal, where is this freed?
          //TODO: wal, should check for any malloc errors
          user_payload->size = user_signedHeuristics->size + user_unsignedHeuristics->size + 4;
          user_payload->signedHeuristics = user_signedHeuristics;
          user_payload->unsignedHeuristics = user_unsignedHeuristics;
          newObject_result = newObject((char*)&Payload_id, user_payload);
          add_result = payload_raw->add(payload_raw, newObject_result->result);
          Payload_t* user_payload2 = malloc(sizeof(Payload_t));   //TODO: wal, where is this freed?
          //TODO: wal, should check for any malloc errors
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
          
          XYObject_t* signatures_object = create_result->result;
          ShortStrongArray_t* signatures_raw = signatures_object->payload;
          
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
          
          XYObject_t* user_signatures_object = create_result->result;
          ShortWeakArray_t* user_signatures = user_signatures_object->payload;

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
            XYResult_t* toBytes_result = BoundWitness_creator->toBytes(BoundWitness_object);
            //char* theBytes = toBytes_result->result;    //TODO: wal, declared but never used
            //breakpoint();
            XYResult_t* GetSigningData_result = BoundWitness_raw->getSigningData(BoundWitness_raw);
            //char* signingBytes = GetSigningData_result->result;   //TODO: wal, declared but never used
            //breakpoint();
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

// end of file main.c

