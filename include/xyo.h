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
  XYObject *publicKeys;
  char hash[32];
  XYObject *payload;
  XYObject *sigs;
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
