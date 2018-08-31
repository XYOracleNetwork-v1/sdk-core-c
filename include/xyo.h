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
typedef struct {
  char heuristicIdentifier[16];
  size_t size;
  void *heuristicPayload;
} basicHeuristic;

typedef struct {
  size_t heuristics_count;
  basicHeuristic heuristics[];
} basicHeuristicGroup;

typedef struct {
  char totalSize[4];
  struct XYObject *publicKeys;
  char hash[32];
  struct XYObject *payload;
  struct XYObject *sigs;
} basicOriginBlock;

typedef struct {
  basicOriginBlock client;
  basicOriginBlock server;
  int signature_client; //wrong type
  int signature_server; //wrong type
} boundBlock;

typedef struct basicOriginBlockNode{
  basicOriginBlock* block;
  struct basicOriginBlockNode *next;
} basicOriginBlockNode;

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

#define XYO_H
#endif
