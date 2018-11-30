/**
 ****************************************************************************************
 *
 * @file hash.c
 *
 * @XY4 project source code.
 *
 * @brief primary hashing routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <stdlib.h>
#include "hash.h"
#include "XYObjects/XYObject.h"

/*
 * FUNCTIONS & METHODS
 ****************************************************************************************
 */

 void trng_acquire(){return;}

/**
 ****************************************************************************************
 *  NAME
 *      getHashId
 *
 *  DESCRIPTION
 *      this routine returns the id of the supplied HashProvider object
 *
 *  PARAMETERS
 *      hashProviderObject    [in]      HashProvider_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr    [out]   XYResult_t*
 *                                                --------------------
 *                                                preallocated_return_result_ptr->error  (error code)
 *                                                preallocated_return_result_ptr->result (hashProviderObject->id)
 *  NOTES
 *      the wiki on HackMD is out of date / incorrect
 ****************************************************************************************
 */
XYResult_t getHashId( void ){}

/**
 ****************************************************************************************
 *  NAME
 *      createHash
 *
 *  DESCRIPTION
 *      this routine creates a hash of the data supplied
 *
 *  PARAMETERS
 *      dataToHash        [in]        ByteArray_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr    [out]   XYResult_t*   (contains an error code and any hash created)
 *                                                --------------------
 *                                    preallocated_return_result_ptr->error  (error code)
 *                                    preallocated_return_result_ptr->result (&sha256OutputBuffer)
 *
 *  NOTES
 *      SHA-256 is currently the only hash type implemented (to save runtime memory).
 *
 *      wc_ = wolf crypto library routine or data type
 ****************************************************************************************
 */
XYResult_t createHash(ByteArray_t* dataToHash){}

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
 *      preallocated_return_result_ptr    [out]     XYResult_t*   preallocated_return_result_ptr->result
 *                                                                contains the
 *                                                                bool result of this call.
 *                                                                --------------------
 *                                                                TRUE = hashes match
 *                                                                FALSE = hashes don't match
 *  NOTES
 *      wc_ = wolf crypto library routine or data type
 ****************************************************************************************
 */

XYResult_t verifyHash(ByteArray_t* dataToBeHashed, XYObject_t* hashForComparison){}

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
 *      preallocated_return_result_ptr      [out]     XYResult_t*
 *                                                    --------------------
 *                                          preallocated_return_result_ptr->error  (error code)
 *                                          preallocated_return_result_ptr->result ((HashProvider_t*)newHasher)
 *  NOTES
 *      will return a malloc error if malloc fails
 ****************************************************************************************
 */
XYResult_t newHashProvider(){}

// end of file hash.c
