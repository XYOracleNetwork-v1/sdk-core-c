/**
 ****************************************************************************************
 *
 * @file network.c
 *
 * @XY4 project source code.
 *
 * @brief primary network routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#include <stdlib.h>
#include "network.h"

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
 *      
 ****************************************************************************************
 */
proactiveNetworkProvider_t* newProactiveNetworkProvider(){
  
  proactiveNetworkProvider_t* proactiveNetworkProvider = malloc(sizeof(proactiveNetworkProvider_t));
  
  proactiveNetworkProvider->requestConnection = NULL;
  proactiveNetworkProvider->sendData = NULL;
  proactiveNetworkProvider->disconnect = NULL;
  
  return proactiveNetworkProvider;
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
 *      
 ****************************************************************************************
 */
reactiveNetworkProvider_t* newReactiveNetworkProvider(){
  
  reactiveNetworkProvider_t* reactiveNetworkProvider = malloc(sizeof(proactiveNetworkProvider_t));
  
  reactiveNetworkProvider->listen = NULL;
  reactiveNetworkProvider->disconnect = NULL;
  
  return reactiveNetworkProvider;
}

// end of file network.c

