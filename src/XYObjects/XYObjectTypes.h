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

#define XY_TYPE_NULL        (0x00)
#define MAJOR_ARRAY         (0x01)

#define MAJOR_2BYTE_ARRAY   (MAJOR_ARRAY | MAJOR_LENGTH_2BYTE)
#define MAJOR_4BYTE_ARRAY   (MAJOR_ARRAY | MAJOR_LENGTH_4BYTE)

#define TYPE_NULL          (0x00)

#define TYPE_ARRAY   (0x01)
//#define TYPE_ARRAY_STRONG  (0x02)