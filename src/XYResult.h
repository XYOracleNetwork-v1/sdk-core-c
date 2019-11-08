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
#define XYSTATUS(_STATUS_) { result.status = _STATUS_; result.value.i = 0; return result; };
#define CHECK_NULL(_VALUE_) if (!_VALUE_) { XYERROR(XY_STATUS_ERROR); }
#define CHECK_RESULT(_RESULT_) if (_RESULT_.status != XY_STATUS_OK) { return _RESULT_; };

#define XY_STATUS_OK (0)
#define XY_STATUS_INDEXOUTOFRANGE (1)
#define XY_STATUS_ERROR (255)

// create a new result and initialize it to all zeros
// this also sets the initial status to OK
#define DECLARE_RESULT() XYResult_t result; memset(&result, 0, sizeof(XYResult_t));

#endif
