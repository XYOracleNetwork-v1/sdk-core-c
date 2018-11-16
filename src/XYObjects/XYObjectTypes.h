/**
 * @file major.h
 * @author Arie Trouw (developer@xy.company)
 * @brief 
 * @version 0.1
 * @date 2018-11-16
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

// Masks for deternining the method of length discovery for a given major
#define MAJOR_LENGTH_INTRINSIC  (0x00) /*00xxxxxx*/
#define MAJOR_LENGTH_2BYTE      (0x80) /*10xxxxxx*/
#define MAJOR_LENGTH_4BYTE      (0xC0) /*11xxxxxx*/

#define MAJOR_NULL        (0x00)
#define MAJOR_ARRAY       (0x01)

#define MAJOR_2BYTE_ARRAY (MAJOR_ARRAY | MAJOR_LENGTH_2BYTE)
#define MAJOR_4BYTE_ARRAY (MAJOR_ARRAY | MAJOR_LENGTH_4BYTE)

#define MINOR_NULL        (0x00)

#define MINOR_ARRAY_WEAK (0x01)
#define MINOR_ARRAY_STRONG (0x02)