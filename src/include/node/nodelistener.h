/**
 ****************************************************************************************
 *
 * @file nodelistener.h
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
#ifndef NODELISTENER_H
#define NODELISTENER_H
//#include "xyobject.h"
//#include "BoundWitness.h"

typedef struct NodeListener NodeListener;

typedef struct BoundWitness BoundWitness_t;

/**
 * A base class for bound witness options. For example a sentinel would have an option for
 * bridging.
 */
struct NodeListener {
  /**
   * This function will be called on every bound witness start.
   */
  void (*onBoundWitnessStart)(void);

  /**
   * This function will be called on evey time a bound witness ended successfully.
   */
  void (*onBoundWitnessDiscovered)(BoundWitness_t* boundWitness);

  /**
   * This function will be called on evey time a bound witness did not end successfully.
   */
  void (*onBoundWitnessEndFailure)(void);
};

#endif
