/**
 ****************************************************************************************
 *
 * @file hash.h
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

#ifndef HASH_H
#include "xyobject.h"

typedef struct HashProvider HashProvider;

struct HashProvider{
  char id[2];
  struct XYResult* (*Hash)(ByteArray*); // Given just a null terminated char* return a cryptographic hash for it
  /*
   * Given a cryptographic hash and a piece of data, verify the given hash == hash(data).
   */
  int (*VerifyHash)(ByteArray* hash, ByteArray* data);
  char* (*GetId)(); // Fetch the above id object and return it.
};

#define HASH_H
#endif
