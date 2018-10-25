/**
 ****************************************************************************************
 *
 * @file network.c
 *
 * @XY4 project source code.
 *
 * @brief primary network routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

#include <stdlib.h>
#include "network.h"
#include "xyo.h"

extern XYResult_t preallocated_return_result;
extern XYResult_t* preallocated_return_result_ptr;

/**
 ****************************************************************************************
 *  NAME
 *      newProactiveNetworkProvider
 *
 *  DESCRIPTION
 *      this routine returns a new proactiveNetworkProvider object  
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      proactiveNetworkProvider      [out]     proactiveNetworkProvider_t*
 *
 *  NOTES
 *      returns a malloc error if malloc fails
 ****************************************************************************************
 */
XYResult_t* newProactiveNetworkProvider(){
  
  proactiveNetworkProvider_t* proactiveNetworkProvider = malloc(sizeof(proactiveNetworkProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(!proactiveNetworkProvider) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};

  proactiveNetworkProvider->requestConnection = NULL;
  proactiveNetworkProvider->sendData = NULL;
  proactiveNetworkProvider->disconnect = NULL;
  
  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = proactiveNetworkProvider;  
  
  return preallocated_return_result_ptr;
}

/**
 ****************************************************************************************
 *  NAME
 *      newReactiveNetworkProvider
 *
 *  DESCRIPTION
 *      this routine returns a new reactiveNetworkProvider object  
 *
 *  PARAMETERS
 *      none
 *
 *  RETURNS
 *      reactiveNetworkProvider      [out]     reactiveNetworkProvider_t*
 *
 *  NOTES
 *      returns a malloc error if malloc fails
 ****************************************************************************************
 */
XYResult_t* newReactiveNetworkProvider(){
  
  reactiveNetworkProvider_t* reactiveNetworkProvider = malloc(sizeof(proactiveNetworkProvider_t));

  /********************************/
  /* guard against malloc errors  */
  /********************************/
  
  if(!reactiveNetworkProvider) {RETURN_ERROR(ERR_INSUFFICIENT_MEMORY)};
  
  reactiveNetworkProvider->listen = NULL;
  reactiveNetworkProvider->disconnect = NULL;
  
  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = reactiveNetworkProvider;  
  
  return preallocated_return_result_ptr;
}

// end of file network.c

