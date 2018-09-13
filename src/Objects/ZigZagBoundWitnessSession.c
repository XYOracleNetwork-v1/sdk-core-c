#ifndef ZIGZAGBOUNDWITNESSSESSION_H
#include "ZigZagBoundWitnessSession.h"

/*----------------------------------------------------------------------------*
*  NAME
*      incomingData
*
*  DESCRIPTION
*     Adds data to the bound witness and returns whats the party should send back.
*
*  PARAMETERS
*     *boundWitness      [in]      BoundWitness*    The data from the other party.
*     *endpoint          [in]      ByteStrongArray* If not already turned around, decide if what to send sign and send back.
*
*  RETURNS
*      XYResult  [out]      bool       Returns XYResult<ByteArray*> the data to send to the other party.
*----------------------------------------------------------------------------*/
XYResult* completeBoundWitness(ZigZagBoundWitnessSession* userSession, ByteArray* bwData){
  RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
}

#define ZIGZAGBOUNDWITNESSSESSION_H
#endif
