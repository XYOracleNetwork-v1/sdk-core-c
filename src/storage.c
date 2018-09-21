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
#include <stdlib.h>
#include "storage.h"

StorageProvider* newStorageProvider(){
  StorageProvider* store = malloc(sizeof(struct StorageProvider));
  store->write = NULL;
  store->read = NULL;
  store->GetAllKeys = NULL;
  store->delete = NULL;
  store->contains = NULL;
  return store;
}
