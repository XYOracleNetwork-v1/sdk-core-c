/**
 * @Author: Nate Brune
 * @Date:   09-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: xyo.h
 * @Last modified by:   Nate Brune
 * @Last modified time: 10-Aug-2018
 * @Copyright: XY - The Findables Company
 */
#include <stddef.h>
#include <stdint.h>
#ifndef XYO_H
#include "xyobject.h"

#define RETURN_ERROR(ERR)                                   \
if(ERR == ERR_INSUFFICIENT_MEMORY){                         \
  preallocated_result->error = ERR_INSUFFICIENT_MEMORY;     \
  preallocated_result->result = 0;                          \
  return preallocated_result;                               \
} else {                                                    \
  XYResult* return_result = malloc(sizeof(XYResult));       \
    if(return_result != NULL){                              \
      return_result->error = ERR;                           \
      return_result->result = 0;                            \
      return return_result;                                 \
    }                                                       \
    else {                                                  \
      preallocated_result->error = ERR_INSUFFICIENT_MEMORY; \
      preallocated_result->result = 0;                      \
      return preallocated_result;                           \
    }                                                       \
}

int littleEndian();

/* Standard Object IDs */
char ByteStrongArray_id[2]   = { 0x01, 0x01 };
char ShortStrongArray_id[2]  = { 0x01, 0x02 };
char IntStrongArray_id[2]    = { 0x01, 0x03 };
char ByteWeakArray_id[2]     = { 0x01, 0x04 };
char ShortWeakArray_id[2]    = { 0x01, 0x05 };
char IntWeakArray_id[2]      = { 0x01, 0x06 };

char BoundWitness_id[2]      = { 0x02, 0x01 };
char KeySet_id[2]            = { 0x02, 0x02 };
char SignatureSet_id[2]      = { 0x02, 0x03 };
char Payload_id[2]           = { 0x02, 0x04 };
char Index_id[2]             = { 0x02, 0x05 };
char PreviousHash_id[2]      = { 0x02, 0x06 };
char NextPublicKey_id[2]     = { 0x02, 0x07 };

char Sha256_id[2]            = { 0x03, 0x05 };
char ECDSASecp256k1_id[2]    = { 0x04, 0x01 };
char ECDSASecp256k1Sig_id[2] = { 0x05, 0x01 };
char Rssi_id[2]              = { 0x08, 0x01 };

#define XYO_H
#endif
