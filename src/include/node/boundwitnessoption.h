/**
 ****************************************************************************************
 *
 * @file boundwitnessoption.h
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

#ifndef BOUNDWITNESSOPTION_H
#define BOUNDWITNESSOPTION_H
#include "xyobject.h"

typedef struct BoundWitnessOption BoundWitnessOption_t;

/**
 * A base class for bound witness options. For example a sentinel would have an option for
 * bridging.
 */
struct BoundWitnessOption {
  /**
 * The flag of the option.
 */
  uint8_t flag;

  /*
   * Gets the signed data to include in the bound witness.
   */
  XYObject_t* (*getSignedPayload)(void*);

  /*
   * Gets the unsigned payload to include in the bound witness.
   */
  XYObject_t* (*getUnsignedPayload)(void*);
};

#endif
