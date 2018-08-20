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
 * This function expects a pointer to the data to be added
 * and size of the data type
 */
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

int main(){
  
  return 0;
}
