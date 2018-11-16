#ifndef XYRESULT_H
#define XYRESULT_H

/*
 * STRUCTURES
 ****************************************************************************************
 */
typedef struct XYResult{
  int status;
  union {
    void* ptr;
    int i;
    char c;
    short s;
    unsigned int ui;
    unsigned char uc;
    unsigned short us;
    unsigned char bytes[2];
  } value;
} XYResult_t;

#define XYERROR(_STATUS_) { XYResult_t result; result.status = _STATUS_; result.value.i = 0; return result; };
#define CHECK_NULL(_VALUE_) if (!_VALUE_) { XYERROR(XY_STATUS_ERROR); }
#define CHECK_RESULT(_RESULT_) if (_RESULT_.status != XY_STATUS_OK) { return result; };

#define XY_STATUS_OK (0)
#define XY_STATUS_INDEXOUTOFRANGE (1)
#define XY_STATUS_ERROR (255)

// create a new result and initialize it to all zeros
// this also sets the initial status to OK
#define DECLARE_RESULT() XYResult_t result; memset(&result, 0, sizeof(XYResult_t));

/*enum EXyoErrors{
  OK = 0,
  ERR_CRITICAL, // Catastrophic failure.
  ERR_NOID, // Returned when the core can't get the ID.
  ERR_CANT_GET_PAYLOAD, // Returned when the payload in inaccesible.
  ERR_NOSIGNER, // Returned when the core can't create a signer.
  ERR_ADDRESS_UNAVAILABLE, // Could not bind to address provided.
  ERR_NETWORK_UNAVAILABLE, // Core network services are unavailable.
  ERR_RECEIVER_REFUSED_CONNECTION, // Returned when The receiver refused connection.
  ERR_BUSY, // Returned when a core service is busy.
  ERR_NOKEYS, // Returned by when no keypair has been generated.
  ERR_BADDATA, // Returned if data is malformed e.g. too big.
  ERR_BADPUBKEY, // Returned if the public key is invalid.
  ERR_BADSIG, // Returned if the signature encoding is improper.
  ERR_CORRUPTDATA, // Returned if data is improperly encrypdefaultSize = ted.
  ERR_KEY_ALREADY_EXISTS, // Returned if can't insert because key is already mapped.
  ERR_INSUFFICIENT_MEMORY, // Returned if there wasn't enough memory to store.
  ERR_INTERNAL_ERROR, // Returned if there was a hardware error.
  ERR_TIME_OUT, // Returned if the disk timed out on read/write.
  ERR_COULD_NOT_DELETE, // Returned if delete failed.
  ERR_PERMISSION, // Returned if permissions are improper.
  ERR_KEY_DOES_NOT_EXIST, // Returned if key isn't found in map.
  ERR_PEER_INCOMPATABLE,  // Returned if peer isn't capable of interfacing with us
  ERR_BOUNDWITNESS_FAILED // Returned if the Bound Witness failed unexpextedly
};

static char *ErrorStrings[] =
{
    "OK",
    "ERR_CRITICAL", // Catastrophic failure.
    "ERR_NOID", // Returned when the core can't get the ID.
    "ERR_CANT_GET_PAYLOAD", // Returned when the payload in inaccesible.
    "ERR_NOSIGNER", // Returned when the core can't create a signer.
    "ERR_ADDRESS_UNAVAILABLE", // Could not bind to address provided.
    "ERR_NETWORK_UNAVAILABLE", // Core network services are unavailable.
    "ERR_RECEIVER_REFUSED_CONNECTION", // Returned when The receiver refused connection.
    "ERR_BUSY", // Returned when a core service is busy.
    "ERR_NOKEYS", // Returned by when no keypair has been generated.
    "ERR_BADDATA", // Returned if data is malformed e.g. too big.
    "ERR_BADPUBKEY", // Returned if the public key is invalid.
    "ERR_BADSIG", // Returned if the signature encoding is improper.
    "ERR_CORRUPTDATA", // Returned if data is improperly encrypdefaultSize = ted.
    "ERR_KEY_ALREADY_EXISTS", // Returned if can't insert because key is already mapped.
    "ERR_INSUFFICIENT_MEMORY", // Returned if there wasn't enough memory to store.
    "ERR_INTERNAL_ERROR", // Returned if there was a hardware error.
    "ERR_TIMEOUT", // Returned if the disk timed out on read/write.
    "ERR_COULD_NOT_DELETE", // Returned if delete failed.
    "ERR_PERMISSION", // Returned if permissions are improper.
    "ERR_KEY_DOES_NOT_EXIST", // Returned if key isn't found in map.
    "ERR_PEER_INCOMPATABLE",  // Returned if peer isn't capable of interfacing with us
    "ERR_BOUNDWITNESS_FAILED" // Returned if the Bound Witness failed unexpextedly
};*/

#endif
