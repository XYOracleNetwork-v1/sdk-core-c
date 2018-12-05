/**
 ****************************************************************************************
 *
 * @file trng.h
 *
 * @brief True Random Number Generator header file.
 *
 * Copyright (C) 2014 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef _TRNG_H_
#define _TRNG_H_

#include <stdint.h>

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief  Acquires 128 bits, saves them in trng_bits[] and restores the modified regs.
 * @param[in] trng_bits_ptr Pointer to number
 ****************************************************************************************
 */
void trng_acquire(uint8_t *trng_bits_ptr);

#endif // _TRNG_H_
