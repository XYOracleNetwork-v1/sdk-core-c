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
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>

/* Function to add a node at the beginning of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void push(struct basicOriginBlockNode** head_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    struct basicOriginBlockNode* new_node = (struct basicOriginBlockNode*)malloc(sizeof(struct basicOriginBlockNode));

    new_node->block  = malloc(data_size);
    new_node->next = (*head_ref);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i=0; i<data_size; i++)
        *(char *)(new_node->block + i) = *(char *)(new_data + i);

    // Change head pointer as new node is added at the beginning
    (*head_ref)    = new_node;
}


void generateGenisisBlock(basicOriginBlock* block, basicOriginBlock* lastblock, char* account){
  block->sig_type = 1;
  block->nonce = 2;
  block->rssi = 3;
  block->primary_pubkey = 4;
  block->secondary_pubkey = 5;
  strcpy(block->xyo_id, account);
  block->signature = 6;
  strcpy(block->previous_hash, "11111111111111111111111111117799");
}

char* serializeBlock(basicOriginBlock* block){

  int totalSize = sizeof(basicOriginBlock);
  for(int i = 0; i < block->heuristicGroup.heuristics_count; i++){
    totalSize+=block->heuristicGroup.heuristics[i].size + sizeof(basicHeuristic);
    printf("totalSize heuristic[%d]: %d\n", i, totalSize);
  }

  uint8_t *start = malloc(totalSize);

  printf("start: %p\n", start);
  uint8_t* itr = start;
  memcpy(itr, (char*)&totalSize, sizeof(int));
  memcpy(itr+4, block, totalSize);
  /*
  printf("totalSize before assignment: %d\n", totalSize);
  int spret = sprintf(itr, "%0d", totalSize);
  printf("spret %d\n", spret);

  itr+= sizeof(char)*spret; // reserve space for totalSize
  *itr = '\0';
  breakpoint();
  itr+= sizeof(char);
  //int totalSize = sizeof(int);

  memcpy(itr, &block->sig_type, sizeof(uint16_t));
  itr += sizeof(uint16_t);

  memcpy(itr, &block->nonce, sizeof(uint32_t));
  itr += sizeof(uint32_t);

  memcpy(itr, &block->rssi, sizeof(uint32_t));
  itr += sizeof(uint32_t);

  memcpy(itr, &block->primary_pubkey, sizeof(int));
  itr += sizeof(int);

  memcpy(itr, &block->secondary_pubkey, sizeof(int));
  itr += sizeof(int);

  memcpy(itr, &block->xyo_id, sizeof(char)*32);
  itr += sizeof(char)*32;

  memcpy(itr, &block->heuristicGroup.heuristics_count, sizeof(size_t));
  itr += sizeof(size_t);

  for(int i = 0; i < block->heuristicGroup.heuristics_count; i++){
    memcpy(itr, &block->heuristicGroup.heuristics[i].heuristicIdentifier, sizeof(char[16]));
    itr += sizeof(char)*16;
    memcpy(itr, &block->heuristicGroup.heuristics[i].size, sizeof(size_t));
    itr += sizeof(size_t);
    memcpy(itr, block->heuristicGroup.heuristics[i].heuristicPayload, block->heuristicGroup.heuristics[i].size);
    itr += sizeof(char)*block->heuristicGroup.heuristics[i].size;
  }

  memcpy(itr, &block->signature, sizeof(int));
  itr += sizeof(int);

  memcpy(itr, &block->previous_hash, sizeof(char)*32);
  itr += sizeof(int);
  memset(itr, '\0', sizeof(char));
  printf("itr: %p\n", itr);
  */
  return start;

}

void HASH(char* block, unsigned char *hash){

  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  char *sizeptr = block;
  printf("size in hash: %d\n", sizeptr[0]);
  SHA256_Update(&sha256, block, sizeptr[0]);
  SHA256_Final(hash, &sha256);
}

boundBlock* startBoundWitness(basicOriginBlock* lastBlock){
  basicOriginBlock block;
  srand(time(NULL));
  block.nonce = rand() % 2048;
  int distance = rand() % 20;
  block.rssi = distance;
  block.heuristicGroup.heuristics_count = 0;
  char* serialBlock = serializeBlock(lastBlock);
  //block.signerPreviousHash = HASH(serialBlock);
}
void breakpoint(){
  return;
}
int main(){
  basicOriginBlockNode *peerOneStart = NULL;
  basicOriginBlockNode *peerTwoStart = NULL;
  basicOriginBlock *peerOneBlock = malloc(sizeof(basicOriginBlock) + sizeof(basicHeuristicGroup)+(sizeof(char)*16));
  generateGenisisBlock(peerOneBlock, NULL, "account1");
  //basicOriginBlock *peerTwoBlock = malloc(sizeof(basicOriginBlock));
  //generateGenisisBlock(peerTwoBlock, "account2");
  push(&peerOneStart, &peerOneBlock, sizeof(peerOneBlock));
  //peerOneBlock->rssi = 13;
  peerOneBlock->heuristicGroup.heuristics_count = 2;
  strcpy(peerOneBlock->heuristicGroup.heuristics[0].heuristicIdentifier, "FirstHeuristic");
  peerOneBlock->heuristicGroup.heuristics[0].size = 16;
  char *heuritic = "123456789A12345" + '\0';
  peerOneBlock->heuristicGroup.heuristics[0].heuristicPayload = malloc(sizeof(char)*16);
  strcpy(peerOneBlock->heuristicGroup.heuristics[0].heuristicPayload, heuritic);
  strcpy(peerOneBlock->heuristicGroup.heuristics[1].heuristicIdentifier, "SecondHeuristic");
  peerOneBlock->heuristicGroup.heuristics[1].size = 16;
  heuritic = "210456789A12345" + '\0';
  peerOneBlock->heuristicGroup.heuristics[1].heuristicPayload = malloc(sizeof(char)*16);
  strcpy(peerOneBlock->heuristicGroup.heuristics[0].heuristicPayload, heuritic);
  //push(&peerTwoStart, &peerTwoBlock, sizeof(peerTwoBlock));
  void* serializedBlock = serializeBlock(peerOneBlock);
  printf("peerOneBlock %p\n", peerOneBlock);
  printf("serialized: %p\n", serializedBlock);
  unsigned char hash[SHA256_DIGEST_LENGTH];
  HASH(serializedBlock, &hash);
  printf("hash: %p\n", &hash);
  breakpoint();


  return 0;
}
