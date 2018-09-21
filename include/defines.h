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
#define MINOR_ECDSA_SECP256K1_SHA256_SIGNATURE  0x01
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
 * nodebase.c defines
 */
#define MAX_ALLOCATED_OPTIONS                   0x03
#define MAX_ALLOCATED_HEURISTICS                0x03
#define MAX_ALLOCATED_LISTENERS                 0x03

#define TRUE                                    0x01
#define FALSE                                   0x00
