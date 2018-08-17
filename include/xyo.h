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
  uint16_t sig_type;
  uint32_t nonce;
  uint32_t rssi;
  int primary_pubkey; // wrong data type
  int secondary_pubkey; //fingerprint; wrong data type
  char xyo_id[32]; //wrong data type
  int signature; //wrong data type
  char previous_hash[32];
  basicHeuristicGroup heuristicGroup;
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

typedef struct tagXYObject{
  char id[2];
  void* payload;
  char* (*GetId)(struct tagXYObject*);
  void* (*GetPayload)(struct tagXYObject*);
} XYObject;
