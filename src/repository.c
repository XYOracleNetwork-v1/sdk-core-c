/**
 ****************************************************************************************
 *
 * @file repository.c
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
#include "repository.h"

/*----------------------------------------------------------------------------*
*  NAME
*      initOriginChainProvider
*
*  DESCRIPTION
*     Initializes an origin chain provider as a Int Strong Array
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
OriginChainProvider_t* initOriginChainProvider(){
  /*
  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  if(lookup_result->error != OK) { return NULL; }
  ObjectProvider_t* ISA_Creator = lookup_result->result;
  //free(lookup_result);

  XYResult_t* create_result = ISA_Creator->create(IntStrongArray_id, NULL);
  if(create_result->error != OK){ return NULL; }

  OriginChainProvider_t* provider = malloc(sizeof(OriginChainProvider_t));
  if(provider){
    provider->repository = create_result->result;
    //free(create_result);
    provider->logicalEnd = 0;
    provider->append = append;
    provider->getChain = getChain;
    provider->deleteChain = deleteChain;
    return provider;
  } else {
    return NULL;
  }
  */
  return NULL;

}

/*----------------------------------------------------------------------------*
*  NAME
*      write
*
*  DESCRIPTION
*      Write block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
/*
XYResult_t* write(OriginChainProvider_t* self, ByteArray_t* value, uint16_t offset, uint16_t timeout) {
  // First we check if Storage Provider has been initialized
  if( self->repository == NULL ){
    XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
    if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    ObjectProvider_t* ISA_Creator = lookup_result->result;

    XYResult_t* create_result = ISA_Creator->create(IntStrongArray_id, NULL);
    if(create_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

    IntStrongArray_t* repo = create_result->result;
    //free(lookup_result);
    lookup_result = tableLookup(BoundWitness_id);
    if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL);}
    char* boundWitnessBytes = value->payload;
    ObjectProvider_t* boundWitness_Creator = lookup_result->result;
    XYResult_t* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
    if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

    BoundWitness_t* boundWitness_raw = fromBytes_result->result;
    XYResult_t* newObject_result = newObject(BoundWitness_id, boundWitness_raw);
    if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    self->logicalEnd = 1;
    return repo->add(repo, newObject_result->result);
  } else {
    for(int i = 0; i<=offset; i++){
      XYResult_t* lookup_result = tableLookup((const char*)&self->repository[i]);
      if(lookup_result->error == OK){
        continue;
      } else {
        lookup_result = tableLookup(BoundWitness_id);

        if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

        char* boundWitnessBytes = value->payload;
        ObjectProvider_t* boundWitness_Creator = lookup_result->result;

        XYResult_t* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
        if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

        BoundWitness_t* boundWitness_raw = fromBytes_result->result;

        XYResult_t* newObject_result = newObject(BoundWitness_id, boundWitness_raw);
        if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
        self->logicalEnd = i+1;
        return ((IntStrongArray_t*)self->repository)->add((IntStrongArray_t*)self->repository, newObject_result->result);
      }
    }
    RETURN_ERROR(ERR_CRITICAL);
  }
}
*/

/*----------------------------------------------------------------------------*
*  NAME
*      read
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
/*
XYResult_t* read(RepositoryProvider_t* self, uint16_t offset, uint16_t timeout){
  return ((IntStrongArray_t*)self->repository)->get(self->repository, offset);
}
*/

/*----------------------------------------------------------------------------*
*  NAME
*      append
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult_t*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult_t* append(OriginChainProvider_t* self, ByteArray_t* value, uint16_t timeout){
  /*
  if(timeout){ /* unimplemented * / }
  XYResult_t* lookup_result = tableLookup(BoundWitness_id);

  ObjectProvider_t* BoundWitness_Creator = lookup_result->result;
  //free(lookup_result);

  XYResult_t* fromBytes_result = BoundWitness_Creator->fromBytes(value->payload);
  if(fromBytes_result->error != OK){
    return fromBytes_result;
  }
  BoundWitness_t* boundWitness = fromBytes_result->result;
  //XYObject_t* boundWitnessObject = fromBytes_result->result;
  XYResult_t* newObject_result = newObject((const char*)&BoundWitness_id, boundWitness);
  XYObject_t* boundWitnessObject = newObject_result->result;
  //boundWitness->size = to_uint32((unsigned char*)(uintptr_t)&boundWitness->size);
  self->logicalEnd = self->logicalEnd +1;
  return ((IntStrongArray_t*)self->repository)->add(self->repository, boundWitnessObject);
  */
  return NULL;
}

/*----------------------------------------------------------------------------*
*  NAME
*      getChain
*
*  DESCRIPTION
*      Get the location of the origin chain on disk and return the array object.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* getChain(OriginChainProvider_t* self){
  /*
  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  ObjectProvider_t* ISA_Creator = lookup_result->result;

  XYResult_t* newObject_result = newObject(IntStrongArray_id, self->repository);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  XYResult_t* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  //free(lookup_result);
  free(newObject_result);

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  if(return_result){
    return_result->error = OK;
    return_result->result = toBytes_result->result;
    free(toBytes_result);
    return return_result;
  } else {
    free(toBytes_result);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  */
  return NULL;
}

/*----------------------------------------------------------------------------*
*  NAME
*      deleteChain
*
*  DESCRIPTION
*      Nullify and clear the origin chain.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult_t*                              [out]      XYObject_t*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult_t* deleteChain(OriginChainProvider_t* self){
  /* format old chain * /

  /* KNOWN MEMORY LEAK. TODO: IMPLEMENT ARRAY DECONSTRUCTOR * /
  free(((IntStrongArray_t*)self->repository)->payload);

  /* Create new IntStrongArray * /

  XYResult_t* lookup_result = tableLookup(IntStrongArray_id);
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  ObjectProvider_t* ISA_Creator = lookup_result->result;
  XYResult_t* create_result = ISA_Creator->create(IntStrongArray_id, NULL);
  if(create_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }
  IntStrongArray_t* repo = create_result->result;
  self->repository = repo;
  self->logicalEnd = 0;

  //free(lookup_result);
  free(create_result);

  XYResult_t* return_result = malloc(sizeof(XYResult_t));
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
  */
  return NULL;
}
