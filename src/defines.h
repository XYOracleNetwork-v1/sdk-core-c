/**
 * @file defines.h
 * @author Arie Trouw (developer@xy.company)
 * @brief All standard definitions in XYO Library
 * @version 0.1
 * @date 2018-11-15
 * 
 * @copyright Copyright (c) 2018 XY - The Persistant COmpany
 * 
 */

/*
 * Defines
 ****************************************************************************************
 */

#define MAJOR_ARRAY                             0x01
#define MAJOR_CORE                              0x02
#define MAJOR_HASH                              0x03
#define MAJOR_KEYS                              0x04
#define MAJOR_SIGNATURES                        0x05
#define MAJOR_HEURISTICS                        0x08
#define MAJOR_CUSTOM                            0x10

/*
 * MAJOR_ARRAY group Minor values
 */
#define MINOR_BYTE_SINGLE_TYPE                  0x01
#define MINOR_SHORT_SINGLE_TYPE                 0x02
#define MINOR_INT_SINGLE_TYPE                   0x03
#define MINOR_BYTE_MULTI_TYPE                   0x04
#define MINOR_SHORT_MULTI_TYPE                  0x05
#define MINOR_INT_MULTI_TYPE                    0x06

/*
 * MAJOR_CORE group Minor values
 */
#define MINOR_BOUND_WITNESS                     0x01
#define MINOR_KEY_SET                           0x02
#define MINOR_SIGNATURE_SET                     0x03
#define MINOR_PAYLOAD                           0x04
#define MINOR_INDEX                             0x05
#define MINOR_PREVIOUS_HASH                     0x06
#define MINOR_NEXT_PUBLIC_KEY                   0x07
#define MINOR_OC_HASH_SET                       0x08
#define MINOR_OC_SET                            0x09
#define MINOR_BOUND_WITNESS_TRANSFER            0x0a

#define BOUND_WITNESS_STAGE_1                   0x01
#define BOUND_WITNESS_STAGE_2                   0x02
#define BOUND_WITNESS_STAGE_3                   0x03

/*
 * MAJOR_HASH group Minor values
 */
#define MINOR_MD2                               0x01
#define MINOR_MD5                               0x02
#define MINOR_SHA1                              0x03
#define MINOR_SHA224                            0x04
#define MINOR_SHA256                            0x05
#define MINOR_SHA512                            0x06

 /*
  * MAJOR_KEYS group Minor values
  */
#define MINOR_ECDSA_SECP256K1_UNCOMPRESSED      0x01
#define MINOR_ECDSA_SECP256K1_COMPRESSED        0x02
#define MINOR_RSA_STANDARD_EXPONENT             0x03

/*
 * MAJOR_SIGNATURES group Minor values
 */
#define MINOR_ECDSA_SECP256K1_SHA256_SIGNATURE  0x01 // TODO: Flip these two back
#define MINOR_ECDSA_SECP256K1_SHA1_SIGNATURE    0x02
#define MINOR_ECDSA_SECP256K1_SHA3_SIGNATURE    0x03
#define MINOR_RSA_SHA256                        0x08

/*
 * MAJOR_HEURISTICS group Minor values
 */
#define MINOR_RSSI                              0x01
#define MINOR_TEXT                              0x01

/*
 * Type Table defines
 */
#define TYPE_TABLE_MAJOR_MAX                    0x20
#define TYPE_TABLE_MINOR_MAX                    0x10

/*
 * originchain.c defines
 */
#define MIN_QUEUE                               0x05
#define MAX_QUEUE                               0x20
#define DEFAULT_TIMEOUT                         0xFFFF
#define ORIGINCHAIN_EXTRA_BITS                  256

/*
 * nodebase.c defines
 */
#define MAX_ALLOCATED_OPTIONS                   0x03
#define MAX_ALLOCATED_HEURISTICS                0x03
#define MAX_ALLOCATED_LISTENERS                 0x00
#define BOUNDWITNESS_OPTIONS                    0x03

/*
 * repository.c defines
 */
#define RAM_DISK_BYTES                          7500
#define MAX_BOUNDWITNESS_IN_CHAIN               1000

/*
 * relaynode.c defines
 */
#define BOUND_WITNESS_OPTION                    1
#define TAKE_ORIGIN_CHAIN_OPTION                2
#define GIVE_ORIGIN_CHAIN_OPTION                4

#ifndef TRUE
    #define TRUE  1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

/*
 * Network.c
 */
#define PARENT_RECV_BUFF_SIZE                  2048
#define CHILD_RECV_BUFF_SIZE                    512
#define XYO_NETWORK_PORT                       2421
#define ERROR_SOCKET_FAILED                     -20
#define ERROR_SETSOCKOPT_FAILED                 -21
#define ERROR_BIND_FAILED                       -22
#define ERROR_LISTEN_FAILED                     -23
#define ERROR_ACCEPT_FAILED                     -24
#define ERROR_READ_FAILED                       -25
#define ERROR_ALLOC_FAILED                      -26
#define ERROR_SEND_FAILED                       -27
#define MAX_PEERS                                10
#define CATALOG_BUFFER_SIZE                       9
#define CATALOG_SIZE                              4
#define DEFAULT_NUM_PEERS                         1

#define CAUSES_BUGS                          100000
/*
 * Configs
 */
#define NODE_MODE BOUND_WITNESS_OPTION+GIVE_ORIGIN_CHAIN_OPTION

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
