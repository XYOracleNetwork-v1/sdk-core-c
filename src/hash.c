/**
 ****************************************************************************************
 *
 * @file hash.c
 *
 * @XY4 project source code.
 *
 * @brief primary hashing routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "hash.h"     
#include "xyobject.h"
#include "xyo.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *  NAME
 *      getHashId
 *
 *  DESCRIPTION
 *      this routine returns the id of the supplied HashProvider object  
 *
 *  PARAMETERS
 *      hashProviderObject  [in]      HashProvider_t*
 *
 *  RETURNS
 *      id                  [out]     char*
 *
 *  NOTES
 *      the wiki on HackMD is out of date / incorrect
 ****************************************************************************************
 */
char* getHashId(HashProvider_t* hashProviderObject){
  
  return hashProviderObject->id;
}

/**
 ****************************************************************************************
 *  NAME
 *      createHash
 *
 *  DESCRIPTION
 *      this routine creates a hash of the data supplied  
 *
 *  PARAMETERS
 *      dataToHash        [in]      ByteArray_t*
 *
 *  RETURNS
 *      return_result     [out]     XYResult_t*   (contains an error code and any hash created)
 *
 *  NOTES
 *      SHA-256 is currently the only hash type implemented (to save runtime memory).
 *
 *      wc_ = wolf crypto library routine or data type
 ****************************************************************************************
 */
XYResult_t* createHash(ByteArray_t* dataToHash){
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  if(!dataToHash || 
     !dataToHash->payload || 
     !dataToHash->size) 
  {
     RETURN_ERROR(ERR_BADDATA);   
  }
     
  static byte sha256OutputBuffer[WC_SHA256_DIGEST_SIZE];    // currently 32 bytes
  
  XMEMSET(sha256OutputBuffer, 0, WC_SHA256_DIGEST_SIZE);    // clear the output buffer 
  
  static XYResult_t return_result;
      
  /**************************************************/
  /* SHA-256 is currently the only hash implemented */
  /**************************************************/
  
  wc_Sha256 sha256HashObject;                               
  
  int wc_init_error = wc_InitSha256(&sha256HashObject);     // prepares the wolf crypto lib
  
  if (!wc_init_error) {                                     // no error = 0
      
    /*********************************************************/
    /* this can be called again and again to update the hash */
    /*********************************************************/
    wc_Sha256Update(&sha256HashObject, 
                    (unsigned char*)dataToHash->payload, 
                    dataToHash->size);  

    wc_Sha256Final(&sha256HashObject, sha256OutputBuffer);  // sha256OutputBuffer now contains
                                                            // the digest of the hashed data.
    return_result.error = OK;
    return_result.result = &sha256OutputBuffer;
    
    wc_Sha256Free(&sha256HashObject);                       // releases the wc_sha256 object
    
    int cleanupError = wolfCrypt_Cleanup();                 // clean up resources used by wolfCrypt
  }
    
  return &return_result;
}

/**
 ****************************************************************************************
 *  NAME
 *      verifyHash
 *
 *  DESCRIPTION
 *      this routine compares the supplied hash to a new hash created from the supplied 
 *      'unhashed' data  
 *
 *  PARAMETERS
 *      dataToBeHashed      [in]      ByteArray_t*
 *      hashForComparison   [in]      XYObject_t*
 *
 *  RETURNS
 *      return_result       [out]     XYResult_t*   return_result->error contains the 
 *                                                  bool result of this call. 
 *                                                  TRUE = hashes match
 *                                                  FALSE = hashes don't match
 *  NOTES
 *      wc_ = wolf crypto library routine or data type
 ****************************************************************************************
 */
XYResult_t* verifyHash(ByteArray_t* dataToBeHashed, XYObject_t* hashForComparison){
  
  /********************************/
  /* some guards against bad data */
  /********************************/
  if(!dataToBeHashed || 
     !dataToBeHashed->size || 
     !hashForComparison || 
     !hashForComparison->payload) 
  {
    RETURN_ERROR(ERR_BADDATA);
  }
  
  static XYResult_t return_result;
    
  HashProvider_t* newHasher = newHashProvider();            //TODO: wal, make sure this is freed

  if(newHasher && newHasher->createHash) {                  // make sure we have a new hasher!
                                                            
    XYResult_t* verify_Hash_return_result = newHasher->createHash(dataToBeHashed);   
                                                            // make the hash. the hash is currently                                                            
                                                            // returned in return_result.result
                                                            // and the error code is returned in
                                                            // return_result.error
    return_result.error = verify_Hash_return_result->error;
    return_result.result = verify_Hash_return_result->result;
    
    free(newHasher); 
    
    if(!return_result.error) {
      
      // compare the new hash created with the one supplied to us for comparison. 
      
      return_result.error = FALSE;                          // presume hashes don't match

      if(memcmp(return_result.result,                       // new hash
         dataToBeHashed->payload,                           // supplied hash 
         WC_SHA256_DIGEST_SIZE))                            // currently 32 bytes 
      {    
        return_result.error = TRUE;                         // success, hashes match
      }
    }
    
    int cleanupError = wolfCrypt_Cleanup();                 // clean up resources used by wolfCrypt
  }
      
  return &return_result;                                    // return_result.error holds the boolean
                                                            // result of this verify hash operation.
                                                            // TRUE = 1 = hashes match
                                                            // FALSE = 0 = hashes don't match
}

/**
 ****************************************************************************************
 *  NAME
 *      newHashProvider
 *
 *  DESCRIPTION
 *      this routine creates a new instance of a hash provider  
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      newHasher      [out]     HashProvider_t*
 *
 *  NOTES
 *      will return a malloc error if malloc fails
 ****************************************************************************************
 */
HashProvider_t* newHashProvider(){
  
  HashProvider_t* newHasher = malloc(sizeof(HashProvider_t*));    //TODO: wal, make sure this is freed
  
  if (newHasher) {
    
    newHasher->createHash = &createHash;
    newHasher->verifyHash = &verifyHash;
    newHasher->getHashId = &getHashId;
    newHasher->id[0] = 0x00;             // major
    newHasher->id[1] = 0x00;             // minor
  }
  
  return newHasher;
}


/* type references

struct ByteArray{
  uint32_t size;
  char reserved[2];
 char* payload;
}

struct XYResult{
  int error;
  void* result;
};

struct XYObject{
  char id[2];
  void* payload;
  char* (*GetXyobjectId)(struct XYObject*);  // Fetch the above id object and return it.
  void* (*GetPayload)(struct XYObject*);     // Fetch the above payload pointer object and return it.
} ;
*/

// end of file hash.c

