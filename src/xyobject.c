/**
 ****************************************************************************************
 *
 * @file xyobject.c
 *
 * @XYO Core library source code.
 *
 * @brief primary xy object routines for the XYO Core.
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
#include <string.h>
#include "xyo.h"
#include "xyobject.h"
//#include "BoundWitness.h"
//#include "XYOHeuristicsBuilder.h"

/*
 * STRUCTURES
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 *  NAME
 *      getXyobjectId
 *
 *  DESCRIPTION
 *      this routine returns the id of the supplied xyo object
 *
 *  PARAMETERS
 *      object    [in]      XYObject_t*
 *
 *  RETURNS
 *      id        [out]     XYResult_t*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t* getXyobjectId(XYObject_t* xyobject){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!xyobject) {RETURN_ERROR(ERR_BADDATA);}

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &xyobject->id;

  return preallocated_return_result_ptr;
}

/**
 ****************************************************************************************
 *  NAME
 *      getPayload
 *
 *  DESCRIPTION
 *      this routine returns the payload of the supplied xyo object
 *
 *  PARAMETERS
 *      object      [in]      XYObject_t*
 *
 *  RETURNS
 *      preallocated_return_result_ptr     [out]     XYResult_t*
 *
 *  NOTES
 *
 ****************************************************************************************
 */
XYResult_t* getPayload(XYObject_t* xyobject){

  /********************************/
  /* guard against bad input data */
  /********************************/

  if(!xyobject) {RETURN_ERROR(ERR_BADDATA);}

  preallocated_return_result_ptr = &preallocated_return_result;

  preallocated_return_result_ptr->error = OK;
  preallocated_return_result_ptr->result = &xyobject->payload;

  return preallocated_return_result_ptr;
}

XYResult_t* newObject(const char id[2], void* payload){
  XYObject_t* new_object = malloc(sizeof(XYObject_t));
  if(new_object != NULL){
    if(payload != 0){
      new_object->payload = payload;
    }
    else{
      RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
    }
    strncpy(new_object->id, id, 2);
    new_object->GetXyobjectId = &getXyobjectId;
    new_object->GetPayload = &getPayload;

    preallocated_return_result_ptr->error = OK;
    preallocated_return_result_ptr->result = new_object;

    return preallocated_return_result_ptr;
  }
  else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
