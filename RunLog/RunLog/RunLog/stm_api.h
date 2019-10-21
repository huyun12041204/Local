/**********************************************************************************
***********************************************************************************
****
****      File name:                                            stm_api.h
****
****      Comment:
****        include file for STARCOS Library V4.4
****
****      Revisions:
****
***********************************************************************************
********************   (C) Copyright Giesecke & Devrient 2004  ********************/

#if !defined STM_API_H
#define      STM_API_H

/** **************************************************************************** **/
/** Build and OS-System detection                                                **/
/** **************************************************************************** **/
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)  /* + WIN 32 bit    */
  #define STM_OS_WIN32
  #if defined(_WINDLL) && defined(STM_DLL)                    /*   DLL (build)   */
    #define STM_BUILD_WIN32
  #endif
#elif defined(_WINDOWS)                                       /* + WIN 16 bit    */
  #define STM_OS_WIN16
  #if defined(_WINDLL) && defined(STM_DLL)                    /*   DLL (build)   */
    #define STM_BUILD_WIN16
  #endif
#elif defined(_OS2)                                           /* + OS/2          */
  #if defined(_OS2DLL)                                        /*   DLL (build)   */
    #define STM_BUILD_OS216
  #else                                                       /*   call to DLL   */
    #define STM_OS_OS216
  #endif
#elif defined(_UNIX)                                          /* + Unix          */
  #define STM_OS_UNIX
  #define STM_BUILD_UNIX
#else                                                         /* + DOS - Lib.    */
  #define STM_OS_DOS
  #define STM_BUILD_DOS
  #if defined(__TURBOC__)
    #define __cdecl             cdecl
  #endif
#endif


/** **************************************************************************** **/
/** struct and class member alignment                                            **/
/** **************************************************************************** **/
#if defined(_MSC_VER)
  #if defined(STM_OS_WIN32)
    #pragma pack(push, old_pack)
    #pragma pack(8)
  #else
    #pragma pack(2)
  #endif
#elif defined(__TURBOC__)
  #if defined(STM_OS_WIN32)
    #pragma option -a8
  #else
    #pragma option -a2
  #endif
#endif 


/** **************************************************************************** **/
/** checking memory model                                                        **/
/** **************************************************************************** **/
#if defined(STM_OS_WIN16) || defined(STM_OS_DOS)
  #if !defined(M_I86LM)
    #error memory models different from Large are not supported
  #endif
#endif


/** **************************************************************************** **/
/** Global routine export macro STM_API (for avoiding def-files)                 **/
/** **************************************************************************** **/
#if defined(STM_OS_WIN32)
  #if defined(STM_BUILD_WIN32)
    #define STM_API1  __declspec(dllexport)
    #define STM_API2  __stdcall
    #define STM_INT1
    #define STM_INT2
    #define STM_INT3
  #else /* !STM_BUILD_WIN32 */
    #define STM_API1
    #define STM_API2  __stdcall  
    #define STM_INT1
    #define STM_INT2 
    #define STM_INT3 
  #endif
#elif defined(STM_OS_WIN16)
  #if defined(STM_BUILD_WIN16)
    #define STM_API1  
    #define STM_API2  __loadds __export __far pascal 
    #define STM_INT1
    #define STM_INT2  __loadds __far
    #define STM_INT3  __far
  #else /* !STM_BUILD_WIN16 */
    #define STM_API1  
    #define STM_API2  __loadds __far pascal 
    #define STM_INT1
    #define STM_INT2  __loadds __far
    #define STM_INT3  __far
  #endif /* STM_BUILD_WIN16 */
#elif defined(STM_OS_UNIX)
    #define STM_API1
    #define STM_API2
    #define STM_INT1
    #define STM_INT2
    #define STM_INT3
#else
    #define STM_API1
    #define STM_API2  __far
    #define STM_INT1
    #define STM_INT2  __far
    #define STM_INT3  __far
#endif

/** *** BEGIN EXTRACTION *** **/

/** **************************************************************************** **/
/** basic constants and macros                                                   **/
/** **************************************************************************** **/
#ifndef _WCHAR_T_DEFINED
  typedef unsigned short wchar_t;
  #define _WCHAR_T_DEFINED
#endif

#if defined TRUE
  #undef TRUE
#endif
#define TRUE    (1==1)

#if defined FALSE
  #undef FALSE
#endif
#define FALSE   (1==0)

#if defined min
  #undef min
#endif
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

#if defined max
  #undef max
#endif
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

#if defined NULL
  #undef NULL
#endif
#define NULL  (0)


/** **************************************************************************** **/
/** type definition for the card terminal handle                                 **/
/** **************************************************************************** **/
typedef unsigned long stmHandle;

/** **************************************************************************** **/
/** type definition for the journal callback function                            **/
/** **************************************************************************** **/
typedef void (*stmfctJouCbk)(unsigned short, unsigned short, unsigned short,
                             unsigned short, unsigned long, unsigned char*);

/** **************************************************************************** **/
/** type definition for the hardware callback function                           **/
/** **************************************************************************** **/
typedef int (STM_API2 *stmfctHardwareCbk)(unsigned int, unsigned char*);

/** **************************************************************************** **/
/** return codes                                                                 **/
/** **************************************************************************** **/

//return code if all went well
#define STM_OK                                0

//error codes
#define STM_ERR_NONE                          0x00

#define STM_ERR_NOT_AVAILABLE                 0x01
#define STM_ERR_LIB_ALREADY_INIT              0x02
#define STM_ERR_LIB_NOT_INIT                  0x03

/* parameter integrity */
#define STM_ERR_BAD_SOURCE_DATA               0x04
#define STM_ERR_BAD_DESTINATION_DATA          0x05
#define STM_ERR_DATA_TOO_LONG                 0x06


/* communication errors  */
#define STM_ERR_SEND_AUTO_REPEAT              0x100
#define STM_ERR_SEND_SW                       0x101
#define STM_ERR_SEND_LEN                      0x102
#define STM_ERR_SEND_T0_LESS_DATA             0x103
#define STM_ERR_SEND_BUFFER_TOO_SMALL         0x104
#define STM_ERR_SEND_AUTO_GET_RESPONSE        0x105

/* interface specific errors */
#define STM_ERR_INTERF_TERM_ALREADY_OPENED    0x140
#define STM_ERR_INTERF_TERM_NOT_OPENED        0x141
#define STM_ERR_INTERF_CARD_ALREADY_REQUESTED 0x142
#define STM_ERR_INTERF_CARD_NOT_REQUESTED     0x143

/* terminal commands: specific errors  */
#define STM_ERR_TERM_NOT_EXIST                0x150
#define STM_ERR_TERM_ALREADY_USED             0x151
#define STM_ERR_TERM_USED_MULTIPLE            0x152
#define STM_ERR_TERM_ALREADY_CREATED          0x153
#define STM_ERR_TERM_NOT_CREATED              0x154

/* PC/CTI specific errors  */
#define STM_ERR_TERM_NO_ICC                   0x160
#define STM_ERR_TERM_STATE                    0x161
#define STM_ERR_TERM_COMM_BREAK               0x162
#define STM_ERR_TERM_COMM_CT                  0x163
#define STM_ERR_TERM_COMM_ICC                 0x164
#define STM_ERR_TERM_COMX                     0x165
#define STM_ERR_TERM_CTH                      0x166
#define STM_ERR_TERM_NAD                      0x167
#define STM_ERR_TERM_PARA_ERR                 0x168
#define STM_ERR_TERM_CMD_LEN_ERR              0x169
#define STM_ERR_TERM_JOURNAL                  0x16a
#define STM_ERR_TERM_INS_NOT_SUPP             0x16b
#define STM_ERR_TERM_CLASS_NOT_SUPP           0x16c
#define STM_ERR_TERM_RESET                    0x16d
#define STM_ERR_TERM_USER_BREAK               0x16e
#define STM_ERR_TERM_USER_TIMEOUT             0x16f
#define STM_ERR_TERM_ICC_NOT_REMOVED          0x170
#define STM_ERR_TERM_CT_SW_ERR                0x171
#define STM_ERR_TERM_UNSPECIFIC               0x172
#define STM_ERR_TERM_MEM_ERR                  0x173
#define STM_ERR_TERM_PIN_NOT_IDENT            0x174
#define STM_ERR_TERM_PCSC_NOT_SUPP            0x175
#define STM_ERR_TERM_ICC_NOT_REQ              0x176
#define STM_ERR_TERM_ICC_LOCKED               0x177
#define STM_ERR_TERM_SERVER_CLOSED            0x178
#define STM_ERR_TERM_ACCESS_TIMEOUT           0x179

/* ICC errors */
#define STM_ERR_ICC_ALREADY_CREATED           0x180
#define STM_ERR_ICC_NOT_CREATED               0x181
#define STM_ERR_ICC_NOT_EJECTED_IN_USE        0x182
#define STM_ERR_ICC_SET_PTS                   0x183

/* logical channel errors */
#define STM_ERR_CHANNEL_NOT_OPEN              0x1a0
#define STM_ERR_CHANNEL_BASIC_NOT_CLOSED      0x1a1
#define STM_ERR_CHANNEL_INCONSISTENT          0x1a2
#define STM_ERR_CHANNEL_IN_USE                0x1a3
#define STM_ERR_CHANNEL_ALREADY_OPEN          0x1a4
#define STM_ERR_CHANNEL_INVALID_NUMBER        0x1a5

/* card commands: unspecific errors           */
#define STM_ERR_CARD_KEY_NOT_ISO              0x200
#define STM_ERR_CARD_INVALID_SHORT_ID         0x201
#define STM_ERR_CARD_INVALID_RECORD_ID        0x202
#define STM_ERR_CARD_INVALID_FILE_STRUCTURE   0x203
#define STM_ERR_CARD_INVALID_OFFSET           0x204
#define STM_ERR_CARD_INVALID_AID              0x205
#define STM_ERR_CARD_INVALID_MODIFY_VALUE     0x206
#define STM_ERR_CARD_PIN_LENGTH               0x207
#define STM_ERR_CARD_PUK_LENGTH               0x208
#define STM_ERR_CARD_BYTE_AMOUNT              0x209
#define STM_ERR_CARD_CHAINING                 0x20a
#define STM_ERR_CARD_NO_DEFAULT_KEY           0x20b
#define STM_ERR_CARD_KEY_TYPE                 0x20c
#define STM_ERR_CARD_CHIP_CONFIG              0x20d
#define STM_ERR_CARD_INVALID_KEY              0x20e
#define STM_ERR_CARD_INVALID_MAC              0x20f
#define STM_ERR_CARD_MASTER_KEY               0x210
#define STM_ERR_CARD_DERIVE_MASTER_KEY        0x211
#define STM_ERR_CARD_INVALID_KEY_HEADER       0x212
#define STM_ERR_CARD_INVALID_CHALL_LENGTH     0x213
/* SPK: */
#define STM_ERR_CARD_INVALID_HASH_LENGTH      0x214 
#define STM_ERR_CARD_INVALID_COUNTER_LENGTH   0x215
#define STM_ERR_CARD_INVALID_SIGNATURE_LENGTH 0x216
#define STM_ERR_CARD_INVALID_CHID_LENGTH      0x217 
#define STM_ERR_CARD_DERIVE_SESSION_KEY       0x218
#define STM_ERR_CARD_SESSION_KEY              0x219
#define STM_ERR_CARD_INVALID_PAD_MODE         0x21a 
#define STM_ERR_CARD_KEY_LENGTH               0x21b
#define STM_ERR_CARD_HASH_BUFFER_OVERFLOW     0x21c
#define STM_ERR_CARD_INVALID_KEY_NUMBER       0x21d
#define STM_ERR_CARD_INVALID_PADDING          0x21e
/* GSM: */
#define STM_ERR_CARD_INVALID_DATA_LENGTH      0x21f
#define STM_ERR_CARD_INVALID_MODE             0x220
/* STARCOS 2000: */
#define STM_ERR_CARD_INVALID_LEVEL            0x221
#define STM_ERR_CARD_INVALID_KEY_VERSION      0x222
#define STM_ERR_CARD_INVALID_TEMPLATE         0x223
#define STM_ERR_CARD_INVALID_SE_NUMBER        0x224
#define STM_ERR_CARD_MISSING_FILE_DESCRIPTOR  0x225
#define STM_ERR_CARD_LOGCHN_OPENED            0x226
#define STM_ERR_CARD_DIRECT_KEY               0x227
#define STM_ERR_CARD_DERIVE_KEY               0x228
#define STM_ERR_CARD_KEY_DERIVATION           0x229
#define STM_ERR_CARD_INVALID_CID              0x22a
#define STM_ERR_CARD_INVALID_INFO             0x22b
/* miscellaneous */
#define STM_ERR_CARD_INVALID_FORMAT           0x22c
/* OP 2.0.1' */
#define STM_ERR_CARD_INVALID_STATUS           0x22d
#define STM_ERR_CARD_INVALID_OPTION           0x22e
#define STM_ERR_CARD_INVALID_CIN              0x22f
#define STM_ERR_CARD_INVALID_CARD_DATA        0x230
#define STM_ERR_CARD_INVALID_RETRY_NUMBER     0x231
/* STARCOS 3.x */
#define STM_ERR_CARD_INVALID_DATA             0x232
#define STM_ERR_CARD_INVALID_FID              0x233
#define STM_ERR_CARD_INVALID_PASSWORD_NUMBER  0x234
#define STM_ERR_CARD_INVALID_DISCRETIONARY_DATA 0x235
#define STM_ERR_CARD_INVALID_RC_NUMBER        0x236

/* card commands: specific errors      */
#define STM_ERR_CARD_NOT_EXIST                0x250
#define STM_ERR_CARD_ALREADY_USED             0x251
#define STM_ERR_CARD_USED_MULTIPLE            0x252
#define STM_ERR_CARD_CANT_CREATE              0x253
#define STM_ERR_CARD_LOGCHN_NOTAVAIL          0x254
#define STM_ERR_CARD_SCVERSION_NOT_AVAIL      0x255           
#define STM_ERR_CARD_SCVERSION                0x256                     
#define STM_ERR_CARD_EXT_AUTH                 0x257
#define STM_ERR_CARD_INT_AUTH                 0x258
#define STM_ERR_CARD_MUT_AUTH                 0x259
#define STM_ERR_CARD_READ_BINARY              0x25a
#define STM_ERR_CARD_WRITE_BINARY             0x25b
#define STM_ERR_CARD_UPDATE_BINARY            0x25c
#define STM_ERR_CARD_READ_RECORD              0x25d
#define STM_ERR_CARD_UPDATE_RECORD            0x25e
#define STM_ERR_CARD_GET_CARD_DATA            0x25f
#define STM_ERR_CARD_GET_CHALLENGE            0x260
#define STM_ERR_CARD_SELECT_FILE              0x261
#define STM_ERR_CARD_INVALID_FILE_TYPE        0x262
#define STM_ERR_CARD_CANT_CHECK_FILE_TYPE     0x263
#define STM_ERR_CARD_INVALID_INFO_REQUEST     0x264
#define STM_ERR_CARD_GET_DATA                 0x265
#define STM_ERR_CARD_LOCK_FILE                0x266
#define STM_ERR_CARD_INCREASE                 0x267
#define STM_ERR_CARD_DECREASE                 0x268
#define STM_ERR_CARD_SECURE_INCREASE          0x269
#define STM_ERR_CARD_SECURE_DECREASE          0x26a
#define STM_ERR_CARD_VERIFY_PIN               0x26b
#define STM_ERR_CARD_CHANGE_PIN               0x26c
#define STM_ERR_CARD_UNBLOCK_PIN              0x26d
#define STM_ERR_CARD_REGISTER_DF              0x26e
#define STM_ERR_CARD_CREATE                   0x26f
#define STM_ERR_CARD_DELETE_MF                0x270
#define STM_ERR_CARD_CRYPT                    0x271
#define STM_ERR_CARD_EXCHANGE_CHALL           0x272
#define STM_ERR_CARD_INSTALL_KEY              0x273
#define STM_ERR_CARD_UPDATE_KEY               0x274
#define STM_ERR_CARD_READ_KEY_STATUS          0x275
#define STM_ERR_CARD_PUT_DATA                 0x276
/* SPK2.1 */
#define STM_ERR_CARD_COMPUTE_HASH             0x277
#define STM_ERR_CARD_SET_HASH                 0x278
#define STM_ERR_CARD_PUT_HASH                 0x279
#define STM_ERR_CARD_COMPUTE_SIGNATURE        0x27a
#define STM_ERR_CARD_VERIFY_SIGNATURE         0x27b
#define STM_ERR_CARD_SET_SECURITY_ENV         0x27c
#define STM_ERR_CARD_ERASE_OBJECT_FILE        0x27d
/* SPK2.2 */
#define STM_ERR_CARD_GENERATE_RSA_KEY         0x27e
#define STM_ERR_CARD_DECIPHER                 0x27f
#define STM_ERR_CARD_EXT_AUTH_ASYM            0x280
#define STM_ERR_CARD_INT_AUTH_ASYM            0x281
#define STM_ERR_CARD_ENCIPHER                 0x282
/* SRF1.3 (Air 1.0) */
#define STM_ERR_CARD_REQUEST_PURSE_ACCOUNT    0x283
#define STM_ERR_CARD_PURSE_TRANSACTION        0x284
/* GSM Phase2 */
#define STM_ERR_CARD_DELETE_FILE              0x285
/* SPK2.3 */
#define STM_ERR_CARD_READ_PUBLIC_KEY_RECORD    0x286
#define STM_ERR_CARD_READ_PUBLIC_KEY_SIGNATURE 0x287
#define STM_ERR_CARD_VERIFY_CERTIFICATE        0x288
#define STM_ERR_CARD_VERIFY_BIOMETRIC          0x289
/* STARCOS 2000 */
#define STM_ERR_CARD_MANAGE_CHANNEL           0x28a
#define STM_ERR_CARD_APPEND_RECORD            0x28b
#define STM_ERR_CARD_TERMINATE_DF             0x28c
#define STM_ERR_CARD_TERMINATE_EF             0x28d
#define STM_ERR_CARD_TERMINATE_CARD_USAGE     0x28e
#define STM_ERR_CARD_ACTIVATE_FILE            0x28f
/* SPK2.5 */
#define STM_ERR_CARD_DEFRAGMENT_FILE          0x290
/* OP 2.0.1' */
#define STM_ERR_CARD_INITIALIZE_UPDATE        0x291
#define STM_ERR_CARD_DELETE                   0x292
#define STM_ERR_CARD_GET_STATUS               0x293
#define STM_ERR_CARD_SET_STATUS               0x294
#define STM_ERR_CARD_INSTALL                  0x295
#define STM_ERR_CARD_LOAD                     0x296
#define STM_ERR_CARD_PUT_KEY                  0x297
/* OP 2.1.1 */
#define STM_ERR_CARD_STORE_DATA               0x298
/* STARCOS 3.x */
#define STM_ERR_CARD_DEACTIVATE_FILE          0x299
#define STM_ERR_CARD_GENERATE_PUBLIC_KEY_PAIR 0x29a
#define STM_ERR_CARD_SEARCH_RECORD            0x29b
#define STM_ERR_CARD_GET_KEYINFO              0x29c
#define STM_ERR_CARD_ACTIVATE_PIN             0x29d
#define STM_ERR_CARD_DEACTIVATE_PIN           0x29e
#define STM_ERR_CARD_TERMINATE_PIN            0x29f

/* card SW1/SW2 errors    */
#define STM_ERR_CARD_NORMAL_PROC_OBSOLETE     0x2a0
#define STM_ERR_CARD_NO_RESPONSE_BYTES_AVAIL  0x2a1
#define STM_ERR_CARD_REPONSE_BYTES_AVAIL      0x2a2
#define STM_ERR_CARD_FILE_NOT_CHANGED         0x2a3
#define STM_ERR_CARD_RET_DATA_MAY_CORRUPT     0x2a4
#define STM_ERR_CARD_END_OF_FILE              0x2a5
#define STM_ERR_CARD_FILE_INVALIDATED         0x2a6
#define STM_ERR_CARD_FCI_NOT_FORMATED         0x2a7
#define STM_ERR_CARD_COUNTER_PROVIDED_BY_X    0x2a8
#define STM_ERR_CARD_FILE_CHANGED             0x2a9
#define STM_ERR_CARD_FILLED_BY_LAST_WRITE     0x2aa
#define STM_ERR_CARD_MEMORY_UNCHANGED         0x2ab
#define STM_ERR_CARD_MEMORY_CHANGED           0x2ac 
#define STM_ERR_CARD_MEMORY_FAILURE           0x2ad
#define STM_ERR_CARD_WRONG_LENGTH             0x2ae
#define STM_ERR_CARD_CLASS_BYTE               0x2af
#define STM_ERR_CARD_LOG_CHN_NOT_SUPPORTED    0x2b0
#define STM_ERR_CARD_SM_NOT_SUPPORTED         0x2b1
#define STM_ERR_CARD_CMD_NOT_ALLOWED          0x2b2
#define STM_ERR_CARD_CMD_INCOMPATIBLE         0x2b3
#define STM_ERR_CARD_CMD_SECURITY_STATE       0x2b4
#define STM_ERR_CARD_AUTH_CMD_BLOCKED         0x2b5
#define STM_ERR_CARD_REF_DATA_INVALID         0x2b6
#define STM_ERR_CARD_COND_NOT_SATISFIED       0x2b7
#define STM_ERR_CARD_CMD_NO_CURRENT_EF        0x2b8
#define STM_ERR_CARD_SM_OBJECTS_MISSING       0x2b9
#define STM_ERR_CARD_SM_OBJECTS_INCORRECT     0x2ba
#define STM_ERR_CARD_PARAMETER                0x2bb
#define STM_ERR_CARD_DATA_FIELD_PARAM         0x2bc
#define STM_ERR_CARD_FUNC_NOT_SUPPORTED       0x2bd
#define STM_ERR_CARD_FILE_NOT_FOUND           0x2be
#define STM_ERR_CARD_RECORD_NOT_FOUND         0x2bf
#define STM_ERR_CARD_NOT_ENOUGH_FILE_SPACE    0x2c0
#define STM_ERR_CARD_LC_INCONSISTENT_TLV      0x2c1
#define STM_ERR_CARD_INCORRECT_P1_P2          0x2c2
#define STM_ERR_CARD_LC_INCONSISTENT          0x2c3
#define STM_ERR_CARD_REF_DATA_NOT_FOUND       0x2c4
#define STM_ERR_CARD_WRONG_LE_FIELD           0x2c5
#define STM_ERR_CARD_WRONG_INS_CODE           0x2c6
#define STM_ERR_CARD_CLASS_NOT_SUPPORTED      0x2c7
#define STM_ERR_CARD_UNKOWN                   0x2c8

/* S1.1 SW-Errors:    */
#define STM_ERR_CARD_FILE_LOCKED                        0x2c9
#define STM_ERR_CARD_EDC_OR_WRITE                       0x2ca
#define STM_ERR_CARD_INVALID_STATE                      0x2cb
#define STM_ERR_CARD_INVALID_PUK_RECORD                 0x2cc
#define STM_ERR_CARD_NO_FILE_SELECTED                   0x2cd
#define STM_ERR_CARD_KEY_LOCKED                         0x2ce
#define STM_ERR_CARD_MF_NOT_SELECTED_OR_FILE_EXIST      0x2cf
#define STM_ERR_CARD_INVALID_KEY_TYPE                   0x2d0
#define STM_ERR_CARD_INVALID_KEY_TYPE_OR_NO_RAND        0x2d1
#define STM_ERR_CARD_MF_NOT_SELECTED                    0x2d2
#define STM_ERR_CARD_INVALID_EF_ID                      0x2d3
#define STM_ERR_CARD_KEY_NOT_FOUND                      0x2d4
#define STM_ERR_CARD_DF_ALREADY_EXISTS                  0x2d5
#define STM_ERR_CARD_FILE_OR_KEY_NOT_FOUND              0x2d6
#define STM_ERR_CARD_PIN_NOT_FOUND                      0x2d7
#define STM_ERR_CARD_UNDERFLOW                          0x2d8
#define STM_ERR_CARD_OVERFLOW                           0x2d9       
#define STM_ERR_CARD_INVALID_DF_ID                      0x2da       
#define STM_ERR_CARD_SYSTEM                             0x2db

/* S1.2 SW-Errors:    */                   
#define STM_ERR_CARD_GET_RESPONSE                       0x2dc
#define STM_ERR_CARD_CANT_OVERWRITE_KEY                 0x2dd
#define STM_ERR_CARD_NO_DATA                            0x2de
  
/* SPK2.1 SW-Errors:  */
#define STM_ERR_CARD_PUBLIC_KEY_INCOMPLETE              0x2df
#define STM_ERR_CARD_DATA_OVERFLOW                      0x2e0
#define STM_ERR_CARD_INVALID_CMD_SEQ                    0x2e1
#define STM_ERR_CARD_INVALID_SECURITY_ENV               0x2e2
#define STM_ERR_CARD_PUBLIC_KEY_NOT_CERTIFIED           0x2e3
#define STM_ERR_CARD_KEY_PART_NOT_FOUND                 0x2e4
#define STM_ERR_CARD_SIGNATURE_FAILED                   0x2e5
#define STM_ERR_CARD_INVALID_KEY_FORMAT_LIST            0x2e6
#define STM_ERR_CARD_INVALID_KEY_LENGTH                 0x2e7

/* SRF1.3 (Air 1.0) SW-Errors:  */
#define STM_ERR_CARD_FREE_ACCESS_NOT_ALLOWED            0x2e8
#define STM_ERR_CARD_OFFLINE_NOT_ALLOWED                0x2e9
#define STM_ERR_CARD_CREDIT_NOT_ALLOWED                 0x2ea
#define STM_ERR_CARD_RPA_NOT_ALLOWED                    0x2eb
#define STM_ERR_CARD_RPA_NOT_ACTIVE                     0x2ec
#define STM_ERR_CARD_T_INCONSISTENT                     0x2ed
#define STM_ERR_CARD_AUTHENTICATION_FAILED              0x2ee
#define STM_ERR_CARD_DEBIT_LIMIT_EXCEEDED               0x2ef
#define STM_ERR_CARD_OVERFLOW_UNDERFLOW                 0x2f0
#define STM_ERR_CARD_RPA_FAILED                         0x2f1
#define STM_ERR_CARD_PT_FAILED                          0x2f2

/* GSM Phase2 SW-Errors:  */
#define STM_ERR_CARD_MEMORY_PROBLEM                     0x2f3
#define STM_ERR_CARD_NO_EF_SELECTED                     0x2f4
#define STM_ERR_CARD_OUT_OF_RANGE                       0x2f5
#define STM_ERR_CARD_FILE_PATTERN_NOT_FOUND             0x2f6
#define STM_ERR_CARD_FILE_CMD_INCONSISTENT              0x2f7
#define STM_ERR_CARD_CHV_NOT_INITIALIZED                0x2f8
#define STM_ERR_CARD_COND_NOT_FULFILLED                 0x2f9
#define STM_ERR_CARD_CHV_STATUS                         0x2fa
#define STM_ERR_CARD_INVALIDATION                       0x2fb
#define STM_ERR_CARD_CHV_BLOCKED                        0x2fc
#define STM_ERR_CARD_INCREASE_OVERFLOW                  0x2fd
#define STM_ERR_CARD_NOT_LATEST_CREATED                 0x2fe
#define STM_ERR_CARD_EF_ALREADY_EXISTS                  0x2ff

/* key class specific errors */
#define STM_ERR_KEY_STATE                              0x300
#define STM_ERR_KEY_PURPOSE                            0x301   
#define STM_ERR_KEY_BAD_KEYPURPOSE                     0x302
   
/* rsakey class specific errors */
#define STM_ERR_RSAKEY_MISSING_PURPOSE                 0x310
#define STM_ERR_RSAKEY_MISSING_SIZE                    0x311
#define STM_ERR_RSAKEY_MISSING_N                       0x312
#define STM_ERR_RSAKEY_MISSING_D                       0x313
#define STM_ERR_RSAKEY_MISSING_E                       0x314
#define STM_ERR_RSAKEY_INCONS_SIZE                     0x315 
#define STM_ERR_RSAKEY_INCONS_BITDIFF                  0x316
#define STM_ERR_RSAKEY_INCONS_NPQ                      0x317
#define STM_ERR_RSAKEY_INCONS_EPQ                      0x318
#define STM_ERR_RSAKEY_INCONS_DPQ                      0x319
#define STM_ERR_RSAKEY_INCONS_ED                       0x31a
#define STM_ERR_RSAKEY_INCONS_PQ                       0x31b
#define STM_ERR_RSAKEY_CHINESE_NOT_AVAILABLE           0x31c
#define STM_ERR_RSAKEY_SOURCE01_CONVERSION_FAILED      0x31d
#define STM_ERR_RSAKEY_SOURCE02_CONVERSION_FAILED      0x31e
#define STM_ERR_RSAKEY_RESULT_CONVERSION_FAILED        0x31f
#define STM_ERR_RSAKEY_MATH_OVERFLOW                   0x320
#define STM_ERR_RSAKEY_PADDING                         0x321
#define STM_ERR_RSAKEY_N_TOO_SMALL                     0x322
#define STM_ERR_RSAKEY_P_TOO_SMALL                     0x323
#define STM_ERR_RSAKEY_Q_TOO_SMALL                     0x324
#define STM_ERR_RSAKEY_E_TOO_SMALL                     0x325
#define STM_ERR_RSAKEY_D_TOO_SMALL                     0x326
#define STM_ERR_RSAKEY_BASE_TOO_LARGE                  0x327
#define STM_ERR_RSAKEY_MESSAGE_TOO_LARGE               0x328
#define STM_ERR_RSAKEY_SIGNATURE_EMPTY                 0x329
#define STM_ERR_RSAKEY_N_EVEN                          0x32A
#define STM_ERR_RSAKEY_P_EVEN                          0x32B
#define STM_ERR_RSAKEY_Q_EVEN                          0x32C
#define STM_ERR_RSAKEY_E_EVEN                          0x32D
#define STM_ERR_RSAKEY_D_EVEN                          0x32E
#define STM_ERR_RSAKEY_NO_PRIME_FOUND                  0x32F
#define STM_ERR_RSAKEY_SIZE_TOO_SMALL                  0x330
#define STM_ERR_RSAKEY_SIZE_TOO_LARGE                  0x331
#define STM_ERR_RSAKEY_BAD_GIVEN_PRIME                 0x332
#define STM_ERR_RSAKEY_MISSING_QUALITY                 0x333
#define STM_ERR_RSAKEY_SOURCE_CONVERSION_FAILED        0x334
#define STM_ERR_RSAKEY_INCONS_CHINESE                  0x335
                 
/* rabinkey class specific errors */
#define STM_ERR_RABINKEY_N_CONVERSTION_FAILED          0x350
#define STM_ERR_RABINKEY_N_TOO_SMALL                   0x351
#define STM_ERR_RABINKEY_N_EVEN                        0x352
#define STM_ERR_RABINKEY_P_CONVERSTION_FAILED          0x353
#define STM_ERR_RABINKEY_P_TOO_SMALL                   0x354
#define STM_ERR_RABINKEY_P_EVEN                        0x355
#define STM_ERR_RABINKEY_Q_CONVERSTION_FAILED          0x356
#define STM_ERR_RABINKEY_Q_TOO_SMALL                   0x357
#define STM_ERR_RABINKEY_Q_EVEN                        0x358
#define STM_ERR_RABINKEY_MISSING_PURPOSE               0x359
#define STM_ERR_RABINKEY_MISSING_PQ                    0x35a
#define STM_ERR_RABINKEY_MISSING_N                     0x35b
#define STM_ERR_RABINKEY_INCONS_NPQ                    0x35c
#define STM_ERR_RABINKEY_INCONS_SIZE                   0x35d
#define STM_ERR_RABINKEY_INCONS_BITDIFF                0x35e
#define STM_ERR_RABINKEY_MATH_OVERFLOW                 0x35f
#define STM_ERR_RABINKEY_MISSING_SIZE                  0x360
#define STM_ERR_RABINKEY_SIZE_TOO_SMALL                0x361
#define STM_ERR_RABINKEY_SIZE_TOO_LARGE                0x362
#define STM_ERR_RABINKEY_BAD_GIVEN_PRIME               0x363
#define STM_ERR_RABINKEY_NO_PRIME_FOUND                0x364
#define STM_ERR_RABINKEY_MISSING_QUALITY               0x365
#define STM_ERR_RABINKEY_INCONS_PQ                     0x366

/* rabinkey class specific errors */
#define STM_ERR_MODRABINKEY_BAD_N                       0x370
#define STM_ERR_MODRABINKEY_BAD_P                       0x371
#define STM_ERR_MODRABINKEY_BAD_Q                       0x372
#define STM_ERR_MODRABINKEY_CHINESE_NOT_AVAILABLE       0x373
#define STM_ERR_MODRABINKEY_BAD_WIDTH                   0x374
#define STM_ERR_MODRABINKEY_BASE_CONVERSION_FAILED      0x375
#define STM_ERR_MODRABINKEY_BASE_TOO_LARGE              0x376
#define STM_ERR_MODRABINKEY_SIGNATURE_CONVERSION_FAILED 0x377
#define STM_ERR_MODRABINKEY_SIGNATURE_TOO_LARGE         0x378
#define STM_ERR_MODRABINKEY_RESULT_CONVERSION_FAILED    0x379
#define STM_ERR_MODRABINKEY_MATH_EXC                    0x37a
#define STM_ERR_MODRABINKEY_INCONS                      0x37b
#define STM_ERR_MODRABINKEY_BAD_BASE                    0x37c

/* deskey, des3key class specific errors */
#define STM_ERR_DESKEY_MISSING_KEY                     0x3a0
#define STM_ERR_DESKEY_BAD_KEY_LENGTH                  0x3a1
#define STM_ERR_DESKEY_BAD_PLAINTEXT_LENGTH            0x3a2
#define STM_ERR_DESKEY_BAD_CIPHERTEXT_LENGTH           0x3a3
#define STM_ERR_DESKEY_BAD_CHALLENGE_LENGTH            0x3a4

/* dsakey class specific errors */
#define STM_ERR_DSAKEY_P_CONVERSION_FAILED             0x3b0
#define STM_ERR_DSAKEY_P_EVEN                          0x3b1
#define STM_ERR_DSAKEY_BAD_P_SIZE                      0x3b2
#define STM_ERR_DSAKEY_Q_CONVERSION_FAILED             0x3b3
#define STM_ERR_DSAKEY_Q_EVEN                          0x3b4
#define STM_ERR_DSAKEY_BAD_Q_SIZE                      0x3b5
#define STM_ERR_DSAKEY_G_CONVERSION_FAILED             0x3b6
#define STM_ERR_DSAKEY_G_TOO_SMALL                     0x3b7
#define STM_ERR_DSAKEY_Y_CONVERSION_FAILED             0x3b8
#define STM_ERR_DSAKEY_Y_TOO_SMALL                     0x3b9
#define STM_ERR_DSAKEY_X_CONVERSION_FAILED             0x3ba
#define STM_ERR_DSAKEY_BAD_X                           0x3bb
#define STM_ERR_DSAKEY_NO_WITNESS                      0x3bc
#define STM_ERR_DSAKEY_MISSING_PURPOSE                 0x3bd
#define STM_ERR_DSAKEY_MISSING_PQG                     0x3be
#define STM_ERR_DSAKEY_MISSING_Y                       0x3bf
#define STM_ERR_DSAKEY_MISSING_X                       0x3c0
#define STM_ERR_DSAKEY_MISSING_SIZE                    0x3c1
#define STM_ERR_DSAKEY_INCONS_SIZE                     0x3c2
#define STM_ERR_DSAKEY_INCONS_PQ                       0x3c3
#define STM_ERR_DSAKEY_INCONS_XY                       0x3c4
#define STM_ERR_DSAKEY_INCONS_Y                        0x3c5
#define STM_ERR_DSAKEY_MATH_OVERFLOW                   0x3c6
#define STM_ERR_DSAKEY_K_CONVERSION_FAILED             0x3c7
#define STM_ERR_DSAKEY_BAD_SEED_LENGTH                 0x3c8
#define STM_ERR_DSAKEY_SEED_CONVERSION_FAILED          0x3c9
#define STM_ERR_DSAKEY_BAD_SEED                        0x3ca
#define STM_ERR_DSAKEY_MISSING_QUALITY                 0x3cb
#define STM_ERR_DSAKEY_BAD_INPUT_LENGTH                0x3cc
#define STM_ERR_DSAKEY_BAD_K                           0x3cd
#define STM_ERR_DSAKEY_BAD_SIGNATURE_LENGTH            0x3ce
#define STM_ERR_DSAKEY_BAD_R                           0x3cf
#define STM_ERR_DSAKEY_BAD_S                           0x3d0
#define STM_ERR_DSAKEY_Q_NOT_PRIME                     0x3d1

/* long class specific errors */
#define STM_ERR_LONG_TOO_LONG                 0x400    

/* elcpkey class specific errors */
#define STM_ERR_ELCPKEY_MISSING_PURPOSE       0x420
#define STM_ERR_ELCPKEY_MISSING_W             0x421
#define STM_ERR_ELCPKEY_MISSING_S             0x422
#define STM_ERR_ELCPKEY_MISSING_K             0x423
#define STM_ERR_ELCPKEY_BAD_W                 0x424
#define STM_ERR_ELCPKEY_BAD_S                 0x425
#define STM_ERR_ELCPKEY_BAD_U                 0x426
#define STM_ERR_ELCPKEY_BAD_EC_PARAM          0x427
#define STM_ERR_ELCPKEY_INCONS_SW             0x428
#define STM_ERR_ELCPKEY_MATH_EXCEPTION        0x429

/* aeskey class specific errors */
#define STM_ERR_AESKEY_MISSING_KEY            0x450
#define STM_ERR_AESKEY_BAD_KEY_LENGTH         0x451
#define STM_ERR_AESKEY_BAD_PLAINTEXT_LENGTH   0x452
#define STM_ERR_AESKEY_BAD_CIPHERTEXT_LENGTH  0x453
#define STM_ERR_AESKEY_BAD_CHALLENGE_LENGTH   0x454

/* process class specific errors */
#define STM_ERR_PROCESS_NOT_INITIALISED       0x500
#define STM_ERR_PROCESS_INCOMPLETE_BLOCK      0x501
#define STM_ERR_PROCESS_BAD_PAD_MODE          0x502
#define STM_ERR_PROCESS_BAD_IV_LENGTH         0x503
#define STM_ERR_PROCESS_PADDING               0x504

/* crypt process class specific errors */
#define STM_ERR_CRYPT_PROCESS_INVALID_KEY     0x510
#define STM_ERR_CRYPT_PROCESS_KEY_ERROR       0x511

/* hash derived classes */
#define STM_ERR_HASH_BAD_PREHASH_LENGTH       0x520

/* STMCByteCounter  */
#define STM_ERR_COUNTER_BAD_COUNTER_LENGTH    0x530
#define STM_ERR_COUNTER_BAD_COUNTER_VALUE     0x531
 
/* keyed-hash derived error codes */
#define STM_ERR_KEYED_HASH_NOT_INITIALISED    0x540
#define STM_ERR_KEYED_HASH_INVALID_HASH       0x541
#define STM_ERR_KEYED_HASH_INVALID_KEY        0x542
#define STM_ERR_KEYED_HASH_HASH_ERROR         0x543

/* STMCSCVersion: error codes */
#define STM_ERR_SCVER_INVALID_SIZE            0x600
#define STM_ERR_SCVER_INVALID_TAG             0x601
#define STM_ERR_SCVER_FIELD_NOT_FOUND         0x602

/* STMCAtr: error codes */
#define STM_ERR_ATR_INVALID_SIZE              0x680
#define STM_ERR_ATR_INVALID                   0x681
#define STM_ERR_ATR_CHK                       0x682
#define STM_ERR_ATR_NOISO                     0x683
#define STM_ERR_ATR_BYTE_NOT_FOUND            0x684

/* PC/CTI response errors */
#define STM_ERR_TSI_BUILD_ERR                 0x700      
#define STM_ERR_TSI_INIT                      0x701
#define STM_ERR_TSI_CREATE_HANDLE             0x702
#define STM_ERR_TSI_RESET                     0x703
#define STM_ERR_TSI_REQUEST_ICC               0x704
#define STM_ERR_TSI_EJECT_ICC                 0x705
#define STM_ERR_TSI_SEND                      0x706
#define STM_ERR_TSI_DISPLAY                   0x707
#define STM_ERR_TSI_JOURNAL_ON                0x708
#define STM_ERR_TSI_JOURNAL_SAVE              0x709
#define STM_ERR_TSI_JOURNAL_GET               0x70a
#define STM_ERR_TSI_JOURNAL                   0x70b
#define STM_ERR_TSI_DEBUG                     0x70c
#define STM_ERR_TSI_INPUT                     0x70d
#define STM_ERR_TSI_ICC_STATUS                0x70e
#define STM_ERR_TSI_VERIFY_PIN                0x70f
#define STM_ERR_TSI_CHANGE_PIN                0x710
#define STM_ERR_TSI_GET_PORT_ID               0x711
#define STM_ERR_TSI_GET_CT_LIST               0x712
#define STM_ERR_TSI_CONFIG                    0x713
#define STM_ERR_TSI_SET_ACCESS_CONTROL        0x714
#define STM_ERR_TSI_CLOSE                     0x715
#define STM_ERR_TSI_CT_TYPE                   0x716

/* file data errors                     */
#define STM_ERR_FILE_CANT_OPEN                0x800  
#define STM_ERR_FILE_CANT_READ                0x801           
#define STM_ERR_FILE_CANT_WRITE               0x802           

/* TLV OBJECT ERRORS                    */
#define STM_ERR_TLV_STRUCTURE                 0x803  
#define STM_ERR_TLV_CONSTR_BIT                0x804  
#define STM_ERR_TLV_NOT_EMPTY                 0x805  
#define STM_ERR_TLV_END_REACHED               0x806  
#define STM_ERR_TLV_INVALID_LENGTH_FIELD      0x807  

/* APDU errors */
#define STM_ERR_APDU_INVALID_DATA             0x808  
#define STM_ERR_APDU_INVALID_LC               0x809  
#define STM_ERR_APDU_INVALID_LE               0x80A  
#define STM_ERR_APDU_INVALID_LENGTH           0x80B  

/* SECM errors */
#define  STM_ERR_SECM_INVALID_KEY_TYPE        0x80C
#define  STM_ERR_SECM_MODE_NOT_SUPPORTED      0x80D
#define  STM_ERR_SECM_MISSING_AUTH_KEY        0x80E
#define  STM_ERR_SECM_MISSING_CRYPT_KEY       0x80F
#define  STM_ERR_SECM_CANT_BUILD_APDU         0x810
#define  STM_ERR_SECM_CALC_MAC                0x811
#define  STM_ERR_SECM_INVALID_RESPONSE        0x812
#define  STM_ERR_SECM_OBJECTS_MISSING         0x813
#define  STM_ERR_SECM_INCORRECT_MAC           0x814
#define  STM_ERR_SECM_SW_MISMATCH             0x815
#define  STM_ERR_SECM_ENCRYPT_DATA            0x816
#define  STM_ERR_SECM_DECRYPT_DATA            0x817
#define  STM_ERR_SECM_SSC_OPERATION           0x818
#define  STM_ERR_SECM_MISSING_KEK             0x819

/* random generator errors */
#define STM_ERR_RANDGEN_BAD_SEED_LENGTH       0x850
#define STM_ERR_RANDGEN_NOT_INITIALIZED       0x851
#define STM_ERR_RANDGEN_CALLBACK              0x852
#define STM_ERR_RANDGEN_FIPS_TEST             0x853

/* STARCOS 2000 SW1/SW2-Errors: */
#define STM_ERR_CARD_FILE_TERMINATED          0x900   

/* OP 2.0.1' SW1/SW2-Errors: */
#define STM_ERR_CARD_FILE_LOGICALLY_DELETED    0x901
#define STM_ERR_CARD_CARD_MANAGER_LOCKED       0x902
#define STM_ERR_CARD_EXTERNAL_AUTH_FAILED      0x903
#define STM_ERR_CARD_MORE_DATA_AVAILABLE       0x904
#define STM_ERR_CARD_INITIALIZE_UPDATE_BLOCKED 0x905
#define STM_ERR_CARD_TYPE_INT_NOT_SUPPORTED    0x906
#define STM_ERR_CARD_INSTALL_IN_PROGRESS       0x907
#define STM_ERR_CARD_APPLICATION_NOT_FOUND     0x908
#define STM_ERR_CARD_TAG_NOT_SUPPORTED         0x909
#define STM_ERR_CARD_AID_NOT_FOUND             0x90A
#define STM_ERR_CARD_KEY_SET_INVALID           0x90B
#define STM_ERR_CARD_UNKNOWN_KEY_ID            0x90C
#define STM_ERR_CARD_INVALID_KEY_CHECK_VALUE   0x90D

/* STARCOS 3.x SW1/SW2-Errors: */
#define STM_ERR_CARD_READ_MEMORY_ERROR              0x90E
#define STM_ERR_CARD_SEARCH_PATTERN_NOT_FOUND       0x90F
#define STM_ERR_CARD_PIN_DEACTIVATED                0x910
#define STM_ERR_CARD_PIN_TERMINATED                 0x911
#define STM_ERR_CARD_GET_DATA_LENGTH_INCONSISTENT   0x912
#define STM_ERR_CARD_PIN_MEMORY_UNCHANGED           0x913
#define STM_ERR_CARD_PSO_MEMORY_UNCHANGED           0x914
#define STM_ERR_CARD_GPKP_MEMORY_UNCHANGED          0x915
#define STM_ERR_CARD_DUPLICATE_KEY_VERSION          0x916
#define STM_ERR_CARD_PUT_DATA_LENGTH_INCONSISTENT   0x917
#define STM_ERR_CARD_RC_MEMORY_UNCHANGED            0x918
#define STM_ERR_CARD_FILE_AC_ENC_SM                 0x919
#define STM_ERR_CARD_GPKP_MEMORY_FAILURE            0x91A
#define STM_ERR_CARD_FILE_MEMORY_FAILURE            0x91B
#define STM_ERR_CARD_FINAL_CHAINING_COMMAND         0x91C
#define STM_ERR_CARD_FILE_ID_ALREADY_EXISTS         0x91D
#define STM_ERR_CARD_RECORD_LIMIT_EXCEEDED          0x91E
#define STM_ERR_CARD_SEARCH_OFFSET_INVALID          0x91F
#define STM_ERR_CARD_PIN_BLOCKED                    0x920
#define STM_ERR_CARD_KEY_BLOCKED                    0x921
#define STM_ERR_CARD_RC_BLOCKED                     0x922
#define STM_ERR_CARD_SIG_KEY_BLOCKED                0x923
#define STM_ERR_CARD_GPKP_BLOCKED                   0x924
#define STM_ERR_CARD_PIN_INITIALIZED                0x925
#define STM_ERR_CARD_FILE_NOT_OPERATIONAL           0x926
#define STM_ERR_CARD_SIG_COND_NOT_SATISFIED         0x927
#define STM_ERR_CARD_FILE_CREATED_OR_INITIALIZED    0x928
#define STM_ERR_CARD_CIPHER_COND_NOT_SATISFIED      0x929
#define STM_ERR_CARD_EXT_AUTH_COND_NOT_SATISFIED    0x92A
#define STM_ERR_CARD_MUT_AUTH_COND_NOT_SATISFIED    0x92B
#define STM_ERR_CARD_INT_AUTH_COND_NOT_SATISFIED    0x92C
#define STM_ERR_CARD_GPKP_COND_NOT_SATISFIED        0x92D
#define STM_ERR_CARD_SEARCH_TLV_INCONSISTENT        0x92E
#define STM_ERR_CARD_PIN_INITIALIZED_OR_ACTIVATED   0x92F
#define STM_ERR_CARD_PIN_NOT_INITIALIZED            0x930
#define STM_ERR_CARD_CHANGE_COND_NOT_SATISFIED      0x931
#define STM_ERR_CARD_VERIFY_COND_NOT_SATISFIED      0x932
#define STM_ERR_CARD_FILE_ALREADY_ACTIVATED         0x933
#define STM_ERR_CARD_SM_CMD_DATA_ENC                0x934
#define STM_ERR_CARD_SIG_DATA_FIELD_PARAM           0x935
#define STM_ERR_CARD_CHANGE_DATA_FIELD_PARAM        0x936
#define STM_ERR_CARD_CREATE_DATA_FIELD_PARAM        0x937
#define STM_ERR_CARD_INT_AUTH_DATA_FIELD_PARAM      0x938
#define STM_ERR_CARD_MSE_DATA_FIELD_PARAM           0x939
#define STM_ERR_CARD_PUT_DATA_FIELD_PARAM           0x93A
#define STM_ERR_CARD_RC_DATA_FIELD_PARAM            0x93B
#define STM_ERR_CARD_SEARCH_DATA_FIELD_PARAM        0x93C
#define STM_ERR_CARD_VERIFY_DATA_FIELD_PARAM        0x93D
#define STM_ERR_CARD_CERT_DATA_FIELD_PARAM          0x93E
#define STM_ERR_CARD_FILE_TERMINATED_OR_DEACTIVATED 0x93F
#define STM_ERR_CARD_TERMINATED                     0x940
#define STM_ERR_CARD_PIN_DEACTIVATED_OR_TERMINATED  0x941
#define STM_ERR_CARD_SELECT_FILE_NOT_FOUND          0x942
#define STM_ERR_CARD_NO_EF_CERT                     0x943
#define STM_ERR_CARD_PUT_NOT_ENOUGH_SPACE           0x944
#define STM_ERR_CARD_PIN_REF_DATA_NOT_FOUND         0x945
#define STM_ERR_CARD_PIN_NO_LCS_INFO                0x946
#define STM_ERR_CARD_COMP_SIG_REF_DATA_NOT_FOUND    0x947
#define STM_ERR_CARD_CIPHER_REF_DATA_NOT_FOUND      0x948
#define STM_ERR_CARD_EXT_AUTH_REF_DATA_NOT_FOUND    0x949
#define STM_ERR_CARD_GPKP_REF_DATA_NOT_FOUND        0x94A
#define STM_ERR_CARD_GET_REF_DATA_NOT_FOUND         0x94B
#define STM_ERR_CARD_KEYINFO_REF_DATA_NOT_FOUND     0x94C
#define STM_ERR_CARD_INT_AUTH_REF_DATA_NOT_FOUND    0x94D
#define STM_ERR_CARD_RC_REF_DATA_NOT_FOUND          0x94E
#define STM_ERR_CARD_VER_SIG_REF_DATA_NOT_FOUND     0x94F
#define STM_ERR_CARD_EXTENDED_OFFSET_NOT_SUPPORTED  0x950
#define STM_ERR_CARD_UPDATE_END_OF_FILE             0x951
#define STM_ERR_CARD_INVALID_OBJECT_TAG             0x952


/* (parameter) errors of the flat C-API */
#define STM_ERR_HANDLE_UNDEF                  0x1000

#define STM_ERR_PARAM01_POINTER               0x1001
#define STM_ERR_PARAM02_POINTER               0x1002
#define STM_ERR_PARAM03_POINTER               0x1003
#define STM_ERR_PARAM04_POINTER               0x1004
#define STM_ERR_PARAM05_POINTER               0x1005
#define STM_ERR_PARAM06_POINTER               0x1006
#define STM_ERR_PARAM07_POINTER               0x1007
#define STM_ERR_PARAM08_POINTER               0x1008
#define STM_ERR_PARAM09_POINTER               0x1009
#define STM_ERR_PARAM10_POINTER               0x100a
#define STM_ERR_PARAM11_POINTER               0x100b
#define STM_ERR_PARAM12_POINTER               0x100c
#define STM_ERR_PARAM13_POINTER               0x100d
#define STM_ERR_PARAM14_POINTER               0x100e
#define STM_ERR_PARAM15_POINTER               0x100f

//parameter errors of the flat API 
#define STM_ERR_BAD_MODE                      0x1010 
#define STM_ERR_BAD_ALGO                      0x1011
#define STM_ERR_BAD_TYPE                      0x1012
#define STM_ERR_BAD_CRYPT_MODE                0x1013
#define STM_ERR_BAD_CHAIN_MODE                0x1014
#define STM_ERR_BAD_PAD_MODE                  0x1015
#define STM_ERR_BAD_KEY_LENGTH                0x1016
#define STM_ERR_BAD_CRYPT_KEY_TYPE            0x1017
#define STM_ERR_BAD_AUTH_KEY_TYPE             0x1018
#define STM_ERR_BAD_KEY_LEVEL                 0x1019
#define STM_ERR_BAD_KEYDATA_LENGTH            0x101a
#define STM_ERR_BAD_KEYHEADER_LENGTH          0x101b
#define STM_ERR_BAD_FORMAT_OPTION             0x101c
#define STM_ERR_BAD_BITDIFF                   0x101d
#define STM_ERR_BAD_DATA_LENGTH               0x101e
#define STM_ERR_BAD_AID_LENGTH                0x101f
#define STM_ERR_BAD_CMD_LENGTH                0x1020
#define STM_ERR_BAD_VALUE_LENGTH              0x1021
#define STM_ERR_BAD_EXPONENT_LENGTH           0x1022
#define STM_ERR_BAD_PORT                      0x1023
#define STM_ERR_BAD_DEST                      0x1024
#define STM_ERR_BAD_TEMPLATE                  0x1025
#define STM_ERR_BAD_DEVICE                    0x1026
#define STM_ERR_BAD_SEL_TYPE_OPTION           0x1027
#define STM_ERR_BAD_ERROR_LEVEL               0x1028
#define STM_ERR_BAD_LANGUAGE_OPTION           0x1029
#define STM_ERR_BAD_LINE_INDEX                0x102a
#define STM_ERR_BAD_BUFFER_SIZE               0x102b
#define STM_ERR_BAD_TLV_TAG                   0x102c
#define STM_ERR_INCOMPLETE_BLOCK              0x102d
#define STM_ERR_BAD_CHANNEL                   0x102e
#define STM_ERR_BAD_KEY_INFO                  0x102f

#define STM_ERR_BUFFER_TOO_SMALL              0x1030

#define STM_ERR_RESPONSE_INCONSISTENT         0x1031

#define STM_ERR_BAD_PARAMETER                 0x1032

#define STM_ERR_BAD_G_LENGTH                  0x1040
#define STM_ERR_BAD_Y_LENGTH                  0x1041
#define STM_ERR_BAD_PTS_MODE                  0x1042
#define STM_ERR_BAD_K_LENGTH                  0x1043
#define STM_ERR_AC_SC_INCONSISTENT            0x1044
#define STM_ERR_PURSE_WRONG_CARD              0x1045
#define STM_ERR_BAD_EC_PARAM                  0x1046
#define STM_ERR_BAD_IFD_KEY                   0x1047
#define STM_ERR_BAD_ICC_KEY                   0x1048

#define STM_ERR_PARAM16_POINTER               0x10f0
#define STM_ERR_PARAM17_POINTER               0x10f1
#define STM_ERR_PARAM18_POINTER               0x10f2
#define STM_ERR_PARAM19_POINTER               0x10f3
#define STM_ERR_PARAM20_POINTER               0x10f4
#define STM_ERR_PARAM21_POINTER               0x10f5
#define STM_ERR_PARAM22_POINTER               0x10f6
#define STM_ERR_PARAM23_POINTER               0x10f7
#define STM_ERR_PARAM24_POINTER               0x10f8
#define STM_ERR_PARAM25_POINTER               0x10f9
#define STM_ERR_PARAM26_POINTER               0x10fa
#define STM_ERR_PARAM27_POINTER               0x10fb
#define STM_ERR_PARAM28_POINTER               0x10fc
#define STM_ERR_PARAM29_POINTER               0x10fd
#define STM_ERR_PARAM30_POINTER               0x10fe
#define STM_ERR_PARAM31_POINTER               0x10ff

#define STM_ERR_INTERNAL_PARAM_ERROR          0x1100 //should not occur

#define STM_ERR_UNDEF                         0xffff //should not occur


/** **************************************************************************** **/
/** symbolic constants (switch  parameters)                                      **/
/** **************************************************************************** **/

//terminal selection
#define STM_OPEN_COM       0x0000  //autodetect CCR2, CCR5, ICT800, KCT800, OTI or PCT
#define STM_OPEN_MKT       0x0100  //MKT-terminal (rfu)
#define STM_OPEN_OTI       0x0200  //OTI Contactless Reader
#define STM_OPEN_CCR5      0x0300  //CCR5
#define STM_OPEN_CCR2      0x0400  //CCR2
#define STM_OPEN_CRM7X     0x0500  //CRM7x
#define STM_OPEN_ICT800    0x0600  //ICT800
#define STM_OPEN_KCT800    0x0700  //KCT800 serial
#define STM_OPEN_KCT800K   0x0800  //KCT800 keyboard-interface (rfu)
#define STM_OPEN_MIFBOARD  0x0900  //MIFARE PC-Card
#define STM_OPEN_MIFCOM    0x0a00  //MIFARE serial (rfu)
#define STM_OPEN_PCT       0x0b00  //PCT
#define STM_OPEN_CASHMOUSE 0x0c00  //CASHMOUSE
#define STM_OPEN_VCT800    0x0d00  //VCT800
#define STM_OPEN_EMU       0x1100  //SmardCard Emulator
#define STM_OPEN_SIM       0x1200  //SmardCard Simulator
#define STM_OPEN_PCSC      0x1300  //PC/SC Reader
#define STM_OPEN_OEM       0x1400  //OEM reader
#define STM_OPEN_DUMMY     0x1600  //Dummy reader
#define STM_OPEN_TS        0x4000  //Terminal Server

//destination port for STM_Open()
#define STM_COM1        1
#define STM_COM2        2
#define STM_COM3        3
#define STM_COM4        4
#define STM_COM5        5
#define STM_COM6        6
#define STM_COM7        7
#define STM_COM8        8
#define STM_COM9        9
#define STM_COM10      10
#define STM_COM11      11
#define STM_COM12      12
#define STM_COM13      13
#define STM_COM14      14
#define STM_COM15      15
#define STM_COM16      16
#define STM_COM17      17
#define STM_COM18      18
#define STM_COM19      19
#define STM_COM20      20
#define STM_COM21      21
#define STM_COM22      22
#define STM_COM23      23
#define STM_COM24      24
#define STM_COM25      25
#define STM_COM26      26
#define STM_COM27      27
#define STM_COM28      28
#define STM_COM29      29
#define STM_COM30      30
#define STM_COM31      31
#define STM_COM32      32
// Maximum Nb of COM Ports supported by PC/CTI (Win32):
#define STM_COM255    255

//communication destination for STM_Request()
#define STM_ICC1        1    
#define STM_ICC2        2
#define STM_ICC3        3
#define STM_ICC4        4
#define STM_ICC5        5
#define STM_ICC6        6
#define STM_ICC7        7
#define STM_ICC8        8
#define STM_ICC9        9
#define STM_ICC10      10
#define STM_ICC11      11
#define STM_ICC12      12
#define STM_ICC13      13
#define STM_ICC14      14

//channel paramters for STM_OpenChannel()/STM_GetChannel()
#define STM_CHANNEL_BASIC  0x00 //STM_GetChannel() only
#define STM_CHANNEL0       0x00 //STM_GetChannel() only
#define STM_CHANNEL1       0x01    
#define STM_CHANNEL2       0x02    
#define STM_CHANNEL3       0x03    
#define STM_CHANNEL_NEW    0x80 //STM_OpenChannel() only

//access modes for STM_Open()/STM_Request()/STM_OpenChannel
#define STM_EXCL               0   
#define STM_SHARED             1
#define STM_APPL_SHARED        3

//operation modes for STM_CloseAll()
#define STM_CLOSE_CARD  0
#define STM_CLOSE_TERM  1
#define STM_CLOSE_ALL   2

//card types
#define STM_CARD          0
#define STM_CARD_ISO      1
#define STM_CARD_S11      2
#define STM_CARD_S12      3 //4 would mean STARCOS S2.0 which is not supported
#define STM_CARD_S21      5
#define STM_CARD_SPK21    6
#define STM_CARD_SV10     7
#define STM_CARD_AIR10    8
#define STM_CARD_SPK22    9
#define STM_CARD_GSM2    10
#define STM_CARD_SPK23   11
#define STM_CARD_S2000   12
#define STM_CARD_ECD60   13
#define STM_CARD_SPK24   14
#define STM_CARD_SPK25   15
#define STM_CARD_OP201   16
#define STM_CARD_S25     17
#define STM_CARD_OP211   18
#define STM_CARD_S3      19

//terminal types
#define STM_CT          0
#define STM_CT_MKT      1
#define STM_CT_MKT_D    2
#define STM_CT_MKT_K    3
#define STM_CT_MKT_DK   4

//operation modes for STM_Create()
#define STM_CREATE_MF               0x00
#define STM_CREATE_DF               0x01
#define STM_CREATE_END              0x02
#define STM_CREATE_EF               0x03
#define STM_CREATE_ANY              0x04
#define STM_CREATE_DF_CONTINUED     0x11
#define STM_CREATE_SPECIAL        0x0100
#define STM_CREATE_CRC            0x0200

//delete modes for STM_DeleteFileId()
#define STM_DELETE_EF            0
#define STM_DELETE_DF            1
#define STM_DELETE_CURRENT_DF    2     
#define STM_DELETE_ANY           3
#define STM_DELETE_CURRENT_EF    4     
#define STM_DELETE_MF            5     

//terminate modes for STM_TerminateFile()
#define STM_TERMINATE_DF         0
#define STM_TERMINATE_EF         1

//defragmentation modes for STM_DefragmentFile()
#define STM_DEFRAGMENT_DF        0
#define STM_DEFRAGMENT_EF        1

//operation modes for STM_LockFile()
#define STM_FILE_LOCK   0    
#define STM_FILE_UNLOCK 1

//selection control for STM_SelectId() and STM_SelectFn()
#define STM_SEL_ANY        0x0000   
#define STM_SEL_DF         0x0001   
#define STM_SEL_EF         0x0002
#define STM_SEL_PARENT_DF  0x0003
#define STM_SEL_MF         0x0004
#define STM_RET_FCI        0x0010
#define STM_RET_FCP        0x0020
#define STM_RET_FMD        0x0030
#define STM_OPEN_CHANNEL1  0x0100
#define STM_OPEN_CHANNEL2  0x0200
#define STM_OPEN_CHANNEL3  0x0300
#define STM_SEL_FIRST      0x0000
#define STM_SEL_LAST       0x1000
#define STM_SEL_NEXT       0x2000
#define STM_SEL_PREVIOUS   0x3000

//eject modes for STM_SetEjectParam()
#define STM_EJECT_DEF      0x00   
#define STM_EJECT_BEEP     0x01   
#define STM_EJECT_BLINK    0x02
#define STM_EJECT_KEEPICC  0x04

//record ids for STM_ReadRecord() and STM_UpdateRecord()
#define STM_RECORD_FIRST     0x1000
#define STM_RECORD_LAST      0x2000
#define STM_RECORD_NEXT      0x3000
#define STM_RECORD_PREVIOUS  0x4000
#define STM_RECORD_APPEND    0x7000

//operation modes for STM_UpdateRecord()
#define STM_OVERWRITE   0
#define STM_APPEND      1

//operation modes for STM_SearchRecord()
#define STM_SEARCH_STANDARD    0
#define STM_SEARCH_EXTENDED    1
#define STM_SEARCH_SPECIFIC    2

//operation modes for STM_GetKeyInfo()
#define STM_SEARCH_CURRENT     0
#define STM_SEARCH_GLOBAL_DF   1
#define STM_SEARCH_GLOBAL_MF   2

//selection of data to be read with STM_GetCardData()
#define STM_SERIAL_NO     0     
#define STM_OS_VERSION    1   
#define STM_CHIP_CONFIG   2
#define STM_PERS_DATA     3
#define STM_INIT_DATA     4
#define STM_MODUL_DATA    5
#define STM_CHIP_DATA     6
#define STM_ALL_DATA      7

//subsets of status information for STM_GetStatus()
#define STM_STATUS_CM                   0x80
#define STM_STATUS_APPL                 0x40
#define STM_STATUS_EXE                  0x20

//possible state changes for STM_SetStatus()
#define STM_STATUS_CM_INITIALIZED       0x8007
#define STM_STATUS_CM_SECURED           0x800F
#define STM_STATUS_CM_CARD_LOCKED       0x807F
#define STM_STATUS_CM_TERMINATED        0x80FF
#define STM_STATUS_APPL_INSTALLED       0x4003
#define STM_STATUS_APPL_SELECTABLE      0x4007
#define STM_STATUS_APPL_PERSONALIZED    0x400F
#define STM_STATUS_APPL_BLOCKED         0x407F
#define STM_STATUS_APPL_LOCKED          0x40FF

//possible state changes for STM_SetStatus()
#define STM_INSTALL_FOR_MAKE_SELECTABLE  0x08
#define STM_INSTALL_FOR_INSTALL          0x04
#define STM_INSTALL_FOR_LOAD             0x02

//operation modes for STM_PutKey()
#define STM_PUT_KEY_SINGLE    0x00
#define STM_PUT_KEY_MULTIPLE  0x01

//operation modes for STM_PutKeyData()
#define STM_PUT_KEY_PLAIN     0x00
#define STM_PUT_KEY_CRYPT     0x10

//operation modes for STM_GeneratePublicKeyPair()
#define STM_GPKP_STANDARD       0x00
#define STM_GPKP_EXPORT         0x02

//operation modes for STM_CardDecipher()
#define STM_DECIPHER_STANDARD       0x00
#define STM_DECIPHER_ECDH           0x01

//authentication modes for symmetric authentication
#define STM_INTERNAL              0x0000
#define STM_EXTERNAL              0x0001
#define STM_MUTUAL                0x0002
#define STM_MUTUAL_SK             0x0012
#define STM_MUTUAL_2SK            0x0022
#define STM_MUTUAL_ESIGNK_2SK     0x0042

#define STM_MUTUAL_SK_SSC         0x0112 // deprecated
#define STM_MUTUAL_SK_SSC_CRYPT   0x1112 // deprecated
#define STM_MUTUAL_2SK_SSC        0x0122 // deprecated
#define STM_MUTUAL_2SK_SSC_CRYPT  0x1122 // deprecated

//authentication modes for asymmetric authentication
#define STM_AUTH_INTERNAL         0x0000
#define STM_AUTH_EXTERNAL         0x0001
#define STM_AUTH_SEQ_FIRST        0x0000
#define STM_AUTH_SEQ_SECOND       0x0008
#define STM_AUTH_HASH_SHA1        0x0000
#define STM_AUTH_HASH_RIPE160     0x0010

//session key negotiation options for authentication modes
#define STM_SK_AUTHENTIC_SSC      0x0100
#define STM_SK_AUTHENTIC_SSC_DYN  0x0200
#define STM_SK_AUTHENTIC_VER_APDU 0x0800
#define STM_SK_CRYPT_SSC          0x1000
#define STM_SK_CRYPT_SSC_DYN      0x2000
#define STM_SK_CRYPT_SSC_VAR      0x4000
#define STM_SK_CRYPT_VER_APDU     0x8000

//authentication partners
#define STM_HOST_ICC    0

//key info flags
#define STM_SESSION_KEY           0x0001
#define STM_MASTER_KEY            0x0002  // old name
#define STM_DES_MASTER_KEY        0x0002  // new name
#define STM_DES3_MASTER_KEY       0x0004
#define STM_DIRECT_KEY            0x0010
#define STM_DERIVE_DES_KEY        0x0020
#define STM_DERIVE_DES3_KEY       0x0040
#define STM_OP_CRYPT_KEY          0x1001
#define STM_OP_AUTH_KEY           0x1002
#define STM_OP_KEY_ENC_KEY        0x1003
#define STM_OP_KMC_MASTER_KEY     0x1100
#define STM_OP_CMK_MASTER_KEY     0x1200  // deprecated
#define STM_OP_CMK2_MASTER_KEY    0x1200
#define STM_OP_CMK4_MASTER_KEY    0x1300
#define STM_OP_ISK_MASTER_KEY     0x1400
#define STM_OP_SESSION_KEY        0x1800

//key level
#define STM_CURRENT   0
#define STM_GLOBAL    1

//key modes for STM_ReadKeyStatus()
#define STM_PIN       0x100 
#define STM_KEY       0x200

//key type
#define STM_DES_KEY   0        
#define STM_DES3_KEY  1        
#define STM_RSA_KEY   2        
#define STM_DSA_KEY   3       

//private use tags for asymmetric RSA authentication keys
#define STM_TAG_RSA      0xE0
#define STM_TAG_RSA_N    0x80
#define STM_TAG_RSA_E    0x81
#define STM_TAG_RSA_D    0x82
#define STM_TAG_RSA_P    0x83
#define STM_TAG_RSA_Q    0x84
#define STM_TAG_RSA_DP   0x85
#define STM_TAG_RSA_DQ   0x86
#define STM_TAG_RSA_PINV 0x87
#define STM_TAG_RSA_QINV 0x88

//private use tags for asymmetric DSA authentication keys
#define STM_TAG_DSA      0xE1
#define STM_TAG_DSA_P    0x80
#define STM_TAG_DSA_Q    0x81
#define STM_TAG_DSA_G    0x82
#define STM_TAG_DSA_Y    0x83
#define STM_TAG_DSA_X    0x84

//private use tags for EC domain parameters
#define STM_TAG_ELCP     0xE2
#define STM_TAG_ELCP_P   0x80
#define STM_TAG_ELCP_A   0x81
#define STM_TAG_ELCP_B   0x82
#define STM_TAG_ELCP_XG  0x83
#define STM_TAG_ELCP_YG  0x84
#define STM_TAG_ELCP_R   0x85
#define STM_TAG_ELCP_K   0x86

//(DES) crypt modes
#define STM_ENCRYPT     0
#define STM_DECRYPT     1
#define STM_MAC         2
#define STM_RETAIL_MAC8 2
#define STM_DECRYPT_PAD 3
#define STM_MAC8        4

//DES chaining modes
#define STM_CBC 0
#define STM_ECB 1

//DES and DES-Hash padding modes
#define STM_PAD_ISO_1   0
#define STM_PAD_ISO_2   1
#define STM_PAD_PKCS_5  2

//algorithms for DES cryptographic processes
#define STM_SINGLE_DES     0
#define STM_TRIPLE_DES     1
#define STM_TRIPLE_DES_16  1
#define STM_TRIPLE_DES_24  2

//algorithms for AES cryptographic processes
#define STM_AES_128        0
#define STM_AES_192        1
#define STM_AES_256        2

// Hash algorithms
#define STM_HASH_SHA1        0
#define STM_HASH_MD5         1
#define STM_HASH_DES_SINGLE  2
#define STM_HASH_DES_DOUBLE  3
#define STM_HASH_RIPE        4
#define STM_HASH_RIPE128     5
#define STM_HASH_RIPE160     6

// Keyed-Hash algorithms
#define STM_KEYED_HASH_HMAC  0x0100

// DH exchange modes
#define STM_DH       0
#define STM_DHC      1
#define STM_DHC_DH   2

//modes for STM_SecureMessaging()
#define STM_SECM_OFF               0x0000
#define STM_SECM_AUTHENTIC_ON      0x0001
#define STM_SECM_CRYPT_ON          0x0002
#define STM_SECM_COMBINED_ON       0x0003
#define STM_SECM_AUTHENTIC_CMD_ON  0x0004
#define STM_SECM_CRYPT_CMD_ON      0x0008
#define STM_SECM_COMBINED_CMD_ON   0x000C
#define STM_SECM_AUTHENTIC_RSP_ON  0x0010
#define STM_SECM_CRYPT_RSP_ON      0x0020
#define STM_SECM_COMBINED_RSP_ON   0x0030
#define STM_SECM_MAC_CMD_4BYTE     0x0040
#define STM_SECM_MAC_CMD_5BYTE     0x0080
#define STM_SECM_MAC_CMD_6BYTE     0x0100
#define STM_SECM_MAC_CMD_7BYTE     0x0200
#define STM_SECM_MAC_RSP_4BYTE     0x0400
#define STM_SECM_MAC_RSP_5BYTE     0x0800
#define STM_SECM_MAC_RSP_6BYTE     0x1000
#define STM_SECM_MAC_RSP_7BYTE     0x2000
#define STM_SECM_MAC_EXCL_HEADER   0x4000
#define STM_SECM_OP_LEVEL_0        0x8000
#define STM_SECM_OP_LEVEL_1        0x0004
#define STM_SECM_OP_LEVEL_3        0x000C
#define STM_SECM_OP_LEVEL_3_CASE2  0x800C

//modes for STM_SetSecureMessagingKey()
#define STM_SECM_KEY_AUTH            0x0003
#define STM_SECM_KEY_CRYPT           0x000C
#define STM_SECM_KEY_AUTH_CMD        0x0001
#define STM_SECM_KEY_AUTH_RSP        0x0002
#define STM_SECM_KEY_CRYPT_CMD       0x0004
#define STM_SECM_KEY_CRYPT_RSP       0x0008
#define STM_SECM_KEY_REF_MSE         0x0010
#define STM_SECM_KEY_REF_APDU        0x0020
#define STM_SECM_KEY_VER_APDU        0x0040
#define STM_SECM_ICV                 0x0100
#define STM_SECM_ICV_APDU            0x0200
#define STM_SECM_ICV_DYN             0x0400
#define STM_SECM_ICV_DYN_APDU        0x0600
#define STM_SECM_ICV_DYN_MAC         0x0C00
#define STM_SECM_ICV_DYN_MSE         0x1400
#define STM_SECM_ICV_DYN_SCRIPT      0x2400
#define STM_SECM_ICV_DYN_SCRIPT_APDU 0x2600
#define STM_SECM_ICV_DYN_SCRIPT_MSE  0x3400
#define STM_SECM_ICV_MAC             0x0800
#define STM_SECM_ICV_MSE             0x1000
#define STM_SECM_ICV_SCRIPT          0x2000
#define STM_SECM_ICV_SCRIPT_APDU     0x2200
#define STM_SECM_ICV_SCRIPT_MSE      0x3000
#define STM_SECM_ICV_VAR_APDU        0x4200
#define STM_SECM_ICV_VAR_MAC         0x4800
#define STM_SECM_KEY_KEK             0x8000

//operation modes for STM_Journal()
#define STM_JOU_ON    0
#define STM_JOU_OFF   1
#define STM_JOU_CLR   2
#define STM_JOU_SAVE  3

//save modes for STM_JournalSave()
#define STM_JOU_OVERWRITE  0
#define STM_JOU_APPEND     1

//linefeed modes for STM_JournalSave()
#define STM_JOU_NO_LF  0
#define STM_JOU_LF     1

//operation modes for the global journal-callback-function
#define STM_CBK_COMMAND    0x0001
#define STM_CBK_RESPONSE   0x0002
#define STM_CBK_SECM       0x0010
#define STM_CBK_STM        0x0100
#define STM_CBK_PCCTI      0x0200
#define STM_CBK_ERR        0x8000
//host ID for the global journal-callback-function
#define STM_HOST           0x0100
#define STM_REMOTE         0x1000
//port ID for the global journal-callback-function
/*      STM_COMx  x = 1 - 32       */
/*      STM_OPEN_x    (rfu)        */
//client ID for the global journal-callback-function
/*      STM_CT                     */
/*      STM_ICCx  x = 1 - 14       */
//PC/CTI callback buffer modification constants
#define STM_CBK_PCCTI_MAX_NEG_OFFSET        2
#define STM_CBK_PCCTI_RETCODE_OFFSET       -2
#define STM_CBK_PCCTI_RESPLEN_OFFSET       -1
#define STM_CBK_PCCTI_RETCODE_NOCHNG        0xFFFF
#define STM_CBK_PCCTI_RETCODE_CHNG_NOERR    0x0

//modes for STM_ManageSE()
#define STM_SE_STORE        0
#define STM_SE_RESTORE      1
#define STM_SE_ERASE        2

//modes for STM_SetSE()
#define STM_SET_SECM_CMD          0x01
#define STM_SET_SECM_RSP          0x10
#define STM_SET_SECM              0x11
#define STM_SET_INT_AUTH          0x02
#define STM_SET_COMP_DIG_SIG      0x02
#define STM_SET_DECIPHER          0x02
#define STM_SET_GPKP              0x02
#define STM_SET_HASH              0x02
#define STM_SET_EXT_AUTH          0x20
#define STM_SET_VERIFY_DIG_SIG    0x20
#define STM_SET_ENCIPHER          0x20
#define STM_SET_AUTH              0x22
#define STM_SET_DIG_SIG           0x22
#define STM_SET_CIPHER            0x22

//templates for STM_SetSE()
#define STM_TEMPLATE_DST    0
#define STM_TEMPLATE_AT     1
#define STM_TEMPLATE_CT     2
#define STM_TEMPLATE_CCT    3
#define STM_TEMPLATE_HT     4

//templates for STM_SetSecurityEnvironment()
#define STM_DIG_SIG    0
#define STM_AUTH       1
#define STM_CIPHER     2

//padding modes for DSA/RSA signature generation and verification
#define STM_RSA_PAD_RND                0  
#define STM_RSA_PAD_80                 1 
#define STM_RSA_PAD_NONE               2 //not supported by cards 
#define STM_RSA_PAD_9796               3 //method 1 or 2_RND padding
#define STM_RSA_PAD_9796_1             4 //ISO 9796 method 1 
#define STM_RSA_PAD_9796_2             5 //ISO 9796 method 2 
#define STM_RSA_PAD_9796_2_RND         6 //DINSIG: ISO 9796 method 2 + 8 random bytes
#define STM_RSA_PAD_DINSIG             6 //DINSIG: ISO 9796 method 2 + 8 random bytes
#define STM_RSA_PAD_PKCS1_01           7 //PKCS#1 block type 01
#define STM_RSA_PAD_PKCS1_01_SHA1      9 //PKCS#1 block type 01/SHA-1/OIW
#define STM_RSA_PAD_PKCS1_01_RIPE160  10 //PKCS#1 block type 01/RIPEMD-160/TeleTrust
#define STM_RSA_PAD_PKCS1_01_MD5      11 //PKCS#1 block type 01/MD-5/RSADSI
#define STM_RSA_PAD_AR                 8 //according to algorithm reference
#define STM_RSA_PAD_AR_11              6 //ISO 9796 method 2 + 8 random bytes/SHA-1
#define STM_RSA_PAD_AR_12              9 //PKCS#1 block type 01/SHA-1/OIW
#define STM_RSA_PAD_AR_21              6 //ISO 9796 method 2 + 8 random bytes/RIPEMD-160
#define STM_RSA_PAD_AR_22             10 //PKCS#1 block type 01/RIPEMD-160/TeleTrust
#define STM_RSA_PAD_AR_32             11 //PKCS#1 block type 01/MD-5/RSADSI
#define STM_RSA_PAD_PKCS1_02          12 //PKCS#1 block type 02
#define STM_RSA_PAD_14888             13 //ISO 14888
#define STM_DSA_PAD_NONE            0x80 //DSA does not feature padding

//PTS modes for STM_ICCSetPTS()
#define STM_PTS_AUTO_OFF   0
#define STM_PTS_AUTO_ON    1    
#define STM_PTS_MANUAL     2

//ICC states
#define STM_ICC_OK        0
#define STM_NO_ICC        1    
#define STM_ICC_NO_VCC    2
#define STM_ICC_UNKNOWN   3

//language option for GetMsg()
#define STM_LANG_E        0

//random generator types
#define STM_CRAND         0 //deprecated: insecure, do not use any longer
#define STM_DESRAND       1 //deprecated: insecure, do not use any longer
#define STM_DES3RAND      2 //deprecated: insecure, do not use any longer
#define STM_ANSIX917RAND  3 //Pseudo-random generator according to ANSI X9.17
//the following random generator is reserved for internal random number generation tests
//and must not be used for any purpose
#define STM_INTERNAL_DETERMINISTIC_RAND_TEST   0x6A3B0003 //reserved for internal tests, must not be used

//key generation algorithms for RSA
#define STM_RSA_TESTDIV       1 //test division by the first 1001 primes
                                //only usefull for small test keys!!
#define STM_RSA_RABMIL05      2 //5 rounds of Rabin-Miller-Test
#define STM_RSA_RABMIL10      3 //10 rounds of Rabin-Miller-Test
#define STM_RSA_STRONG        4 //pseudo primes p such that p-1 and p+1 are non-smooth
#define STM_RSA_GORDON        5 //pseudo primes fulfilling all of Gordon's criteria
#define STM_RSA_MAURER        6 //true primes genrated using a variant of 
                                //Maurer's algorithm
#define STM_RSA_RABMIL_E80    7 //Rabin-Miller-Test with error < 2E-80

//format options for STM_GenerateAndWriteRSAKey() and STM_WritePrivateRSAKey()
//and STM_WritePrivateDSAKey()
#define STM_ENTRY_N_D                0
#define STM_ENTRY_P_Q_D              1
#define STM_ENTRY_P_Q_DP_DQ          2
#define STM_ENTRY_P_Q_DP_DQ_QINV     3
#define STM_ENTRY_P_Q_DP_DQ_PINV     4
#define STM_ENTRY_MSB_TO_LSB     0x000
#define STM_ENTRY_LSB_TO_MSB     0x100

//key generation algorithms for DSA
#define STM_DSA_RABMIL05      1 //5 rounds of Rabin-Miller-Test
#define STM_DSA_RABMIL10      2 //10 rounds of Rabin-Miller-Test
#define STM_DSA_ANSIX930      3 //Implementation of the generation method given in 
                                //ANSI X9.30 - Part 1

//access control modes for STM_CardAccessControl() and STM_TermAccessControl()
#define STM_AC_UNLOCK       0 //release temporary exclusive access to this terminal
                              //or smartcard
#define STM_AC_LOCK         1 //request temporary exclusive access to this terminal
                              //or smartcard
//additional access control modes for STM_TermAccessControl()
#define STM_AC_SET_TIMEOUT  2 //set timeout for blocked requests to this terminal
                              //and its smartcards
//additional access control modes for STM_CardAccessControl()
#define STM_AC_UNLOCK_ALL   3 //release temporary exclusive access to this terminal
                              //and smartcard
#define STM_AC_LOCK_ALL     4 //request temporary exclusive access to this terminal
                              //and smartcard

//infinite timeout definition for STM_TermAccessControl()
#define STM_INFINITE        0xFFFFFFFF

//list mode for STM_GetTermList()
#define STM_LIST_ALL           0 //list all terminals
#define STM_LIST_PCSC          1 //list PCSC terminals only
#define STM_LIST_CTAPI         2 //list CTAPI terminals only
#define STM_LIST_MISC          4 //list miscellaneous terminals only

//config mode for STM_PCCTIConfig()
#define STM_PCCTI_CONFIG_MSG_BOX    0x0002 //control generation of PC/CTI message
                                           // boxes
#define STM_PCCTI_CONFIG_DLL        0x0100 //undocumented, must not be used
#define STM_PCCTI_CONFIG_INI_FILEA  0x0004 //(re)set CT-API INI-file(path &) ANSI name
#define STM_PCCTI_CONFIG_INI_FILEW  0x8004 //(re)set CT-API INI-file(path &) UNICODE name

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_PCCTI_CONFIG_INI_FILE  0x8004
  #else
    #define STM_PCCTI_CONFIG_INI_FILE  0x0004
  #endif
#else
  #define STM_PCCTI_CONFIG_INI_FILE  0x0004
#endif

//config mode for STM_TermConfig()
#define STM_TERM_CONFIG_GET_TIMING        0x0010 //get waiting times of T=1 Host
#define STM_TERM_CONFIG_SET_TIMING        0x0020 //set waiting times of T=1 Host
#define STM_TERM_CONFIG_REQUEST_SILENT    0x1000 //no GET CARD DATA during request
#define STM_TERM_CONFIG_REQUEST_CARD_DATA 0x2000 //eventually perform GET CARD DATA
                                                 //during request
#define STM_TERM_CONFIG_GET_RESPONSE_OFF  0x3000 //no GET RESPONSE during send card
#define STM_TERM_CONFIG_GET_RESPONSE_ON   0x4000 //eventually perform GET RESPONSE
                                                 //during send card

/** **** END EXTRACTION **** **/

/** ****************************************************************** **/
/** External Function Set - all STM_Functions()                        **/
/** ****************************************************************** **/

#if defined(__cplusplus) /* extern "C" declaration of C-functions for C++ */
extern "C" {
#endif /* __cplusplus */

STM_API1 int STM_API2 STM_ActivateFile(stmHandle hCard);
  
STM_API1 int STM_API2 STM_ActivatePIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel);

STM_API1 int STM_API2 STM_AES(unsigned int uiAlgo, unsigned int uiCryptMode,
  unsigned int uiChainMode, unsigned int uiPadMode, const unsigned char* ucpKey,
  const unsigned char* ucpIV, unsigned int uiLenData, const unsigned char* ucpData,
  unsigned int* uipLenResult, unsigned char* ucpResult);

STM_API1 int STM_API2 STM_AESData(stmHandle hAESProcess, unsigned int uiLenInput, 
  const unsigned char *ucpInput, unsigned int *uipActLenOutput, unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_AESFinish(stmHandle hAESProcess, unsigned int *uipActLenOutput, 
  unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_AESInit(stmHandle *hpAESProcess,  unsigned int uiAlgo, 
  unsigned int uiCryptMode, unsigned int uiChainMode, unsigned int uiPadMode, 
  const unsigned char *ucpKey, const unsigned char *ucpIV);

STM_API1 int STM_API2 STM_AppendRecord(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiLenData, const unsigned char *ucpData);
  
STM_API1 int STM_API2 STM_AuthAsym(stmHandle hCard, unsigned int uiAuthMode,
  const unsigned char *ucpAuthKey, const unsigned char *ucpCryptKey,
  unsigned char *ucpRFU, int *bpResult);

STM_API1 int STM_API2 STM_AuthAsymEx(stmHandle hCard, unsigned int uiAuthMode,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, const unsigned char *ucpIFDKey,
  const unsigned char *ucpICCKey, const unsigned char *ucpDeviceID,
  unsigned int uiAuthenticKeyVersion, unsigned int uiCryptKeyVersion, int *bpResult);

STM_API1 int STM_API2 STM_AuthClientServer(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_AuthClientServerEx(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenData, const unsigned char *ucpData,
  unsigned int uiResBufLen, unsigned int *uipActResLen, unsigned char *ucpResBuf);

// deprecated, undocumented!
STM_API1 int STM_API2 STM_Authentication(stmHandle hCard, unsigned int uiAuthMode,
  unsigned int uiDevice, unsigned int uiKeyNo, unsigned int uiKeyLevel,
  unsigned int uiKeyInfo, unsigned int uiKeyType, const unsigned char *ucpKey,
  int *bpResult);

STM_API1 int STM_API2 STM_AuthSym(stmHandle hCard, unsigned int uiAuthMode,
  unsigned int uiKeyNo, unsigned int uiKeyVersion, unsigned int uiKeyLevel,
  unsigned int uiKeyInfo, unsigned int uiKeyType, const unsigned char *ucpKey, int *bpResult);

STM_API1 int STM_API2 STM_AuthSymEx(stmHandle hCard, unsigned int uiAuthMode,
  unsigned int uiKeyNo, unsigned int uiKeyVersion, unsigned int uiKeyLevel,
  const unsigned char *ucpIFDID, const unsigned char *ucpICCID,
  unsigned int uiEncKeyInfo, unsigned int uiEncKeyType, const unsigned char *ucpEncKey,
  unsigned int uiMacKeyInfo, unsigned int uiMacKeyType, const unsigned char *ucpMacKey,
  unsigned int uiAuthenticKeyVersion, unsigned int uiCryptKeyVersion, int *bpResult);

STM_API1 int STM_API2 STM_CardAccessControl(stmHandle hCard, unsigned int uiAccessControl);

STM_API1 int STM_API2 STM_CardDecipher(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiMode, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_CardECDHExchange(stmHandle hCard, unsigned int uiLenPrimeP,
  unsigned int uiLenXPublKey, const unsigned char *ucpXPublKey, unsigned int uiLenYPublKey,
  const unsigned char *ucpYPublKey, unsigned int uiResBufLen, unsigned int *uipActResLen,
  unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_CardEncipher(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiRFU, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_CardGenerateRSAKey(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiBitLenKey, unsigned char *ucpModulus);

STM_API1 int STM_API2 STM_CardGenerateRSAKeyEx(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiBitLenKey,
  unsigned char *ucpModulus);

STM_API1 int STM_API2 STM_ChangePIN(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiLenOldPIN, const unsigned char *ucpOldPIN,
  unsigned int uiLenNewPIN, const unsigned char *ucpNewPIN, int *bpResult); 

STM_API1 int STM_API2 STM_CheckISOATR(stmHandle hCard, int *bpIsISO);

STM_API1 int STM_API2 STM_Close(stmHandle hCard);

STM_API1 int STM_API2 STM_CloseAll(unsigned int uiMode, unsigned int uiPort, unsigned int uiDest);

STM_API1 int STM_API2 STM_CloseChannel(stmHandle hCard);

STM_API1 int STM_API2 STM_Commands(unsigned int uiLineNo, unsigned int uiMaxlenTxt,
  char *szTxt);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_CommandsA(unsigned int uiLineNo, unsigned int uiMaxlenTxt,
  char *szTxt);

STM_API1 int STM_API2 STM_CommandsW(unsigned int uiLineNo, unsigned int uiMaxlenTxt,
  wchar_t *szTxt);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_Commands STM_CommandsW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_ComputeHash(stmHandle hCard, unsigned int uiLenInput,
  const unsigned char *ucpInput, unsigned int uiLenResBuf, unsigned int *uipActResLen,
  unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_ComputeHashData(stmHandle hCard, unsigned int uiLenInput,
  const unsigned char *ucpInput);

STM_API1 int STM_API2 STM_ComputeHashFinish(stmHandle hCard, unsigned int uiLenResBuf,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_ComputeHashInit(stmHandle hCard);

STM_API1 int STM_API2 STM_ComputeHashInitPre(stmHandle hCard, unsigned uiLenPreHash,
  const unsigned char* ucpPreHash, const unsigned char* ucpCounter);

STM_API1 int STM_API2 STM_ComputeSignature(stmHandle hCard, unsigned int uiPadMode,
  unsigned int uiResBufLen, unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_Create(stmHandle hCard, unsigned int uiCreateMode, 
  unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_Crypt(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiCryptMode, unsigned int uiLenData, 
  const unsigned char *ucpData, unsigned int uiResBufLen, unsigned int *uipActResLen,
  unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_DeactivateFile(stmHandle hCard);

STM_API1 int STM_API2 STM_DeactivatePIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel);

STM_API1 int STM_API2 STM_Decrease(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiLen, const unsigned char *ucpValue, unsigned int uiLenExtraData,
  const unsigned char *ucpExtraData, unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_DefragmentFile(stmHandle hCard, unsigned uiDefragMode);

STM_API1 int STM_API2 STM_DeinitializeLibrary();

STM_API1 int STM_API2 STM_Delete(stmHandle hCard, unsigned int uiLenObjectId,
  const unsigned char *ucpObjectId, unsigned int uiLenReceipt,
  unsigned int *uipActLenReceipt, unsigned char *ucpReceipt);

// deprecated, undocumented!
STM_API1 int STM_API2 STM_DeleteFile(stmHandle hCard, unsigned int uiFID);

STM_API1 int STM_API2 STM_DeleteFileId(stmHandle hCard, unsigned int uiDelCtrl, unsigned int uiFID);

STM_API1 int STM_API2 STM_DeleteMF(stmHandle hCard);

// deprecated, undocumented!
STM_API1 int STM_API2 STM_DeriveKey(stmHandle hCard, unsigned int uiKeyType, 
  const unsigned char *ucpMasterKey, unsigned char *ucpDerivedKey);

STM_API1 int STM_API2 STM_DeriveKeyEx(stmHandle hCard, unsigned int uiKeyInfo,
  unsigned int uiKeyType, const unsigned char *ucpMasterKey, unsigned char *ucpDerivedKey);
                                                        
STM_API1 int STM_API2 STM_DES(unsigned int uiAlgo, unsigned int uiCryptMode,
  unsigned int uiChainMode, unsigned int uiPadMode, const unsigned char *ucpKey,
  const unsigned char *ucpIV, unsigned int uiLenData, const unsigned char *ucpData,
  unsigned int *uipLenResult, unsigned char *ucpResult);

STM_API1 int STM_API2 STM_DESData(stmHandle hDESProcess, unsigned int uiLenInput, 
  const unsigned char *ucpInput, unsigned int *uipActLenOutput, unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_DESFinish(stmHandle hDESProcess, unsigned int *uipActLenOutput, 
  unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_DESInit(stmHandle *hpDESProcess,  unsigned int uiAlgo, 
  unsigned int uiCryptMode, unsigned int uiChainMode, unsigned int uiPadMode, 
  const unsigned char *ucpKey, const unsigned char *ucpIV);

STM_API1 int STM_API2 STM_DSASign(unsigned int uiLenPrimeP, const unsigned char *ucpPrimeP,
  const unsigned char *ucpPrimeQ, unsigned int uiLenBaseG, const unsigned char *ucpBaseG,
  const unsigned char *ucpSecKey, unsigned int uiLenRandK, const unsigned char *ucpRandK,
  const unsigned char *ucpData, unsigned char *ucpSig);

STM_API1 int STM_API2 STM_DSAVerify(unsigned int uiLenPrimeP, const unsigned char *ucpPrimeP,
  const unsigned char *ucpPrimeQ, unsigned int uiLenBaseG, const unsigned char *ucpBaseG,
  unsigned int uiLenPubKey, const unsigned char *ucpPubKey, const unsigned char *ucpData,
  const unsigned char *ucpSig, int *bpResult);

STM_API1 int STM_API2 STM_ECDHExchange(unsigned int uiMode, const unsigned char *ucpECParam,
  unsigned int uiLenPrivKey, const unsigned char *ucpPrivKey, unsigned int uiLenXPublKey,
  const unsigned char *ucpXPublKey, unsigned int uiLenYPublKey,
  const unsigned char *ucpYPublKey, unsigned int uiLenRes, unsigned int *uipActLenRes,
  unsigned char *ucpResult);

STM_API1 int STM_API2 STM_ECDSASign(const unsigned char *ucpECParam,
  unsigned int uiLenPrivKey, const unsigned char *ucpPrivKey, unsigned int uiLenRandU,
  const unsigned char *ucpRandU, unsigned int uiLenData, const unsigned char *ucpData,
  unsigned int uiLenSig, unsigned int *uipActLenSig, unsigned char *ucpSigC,
  unsigned char *ucpSigD);

STM_API1 int STM_API2 STM_ECDSAVerify(const unsigned char *ucpECParam,
  unsigned int uiLenXPublKey, const unsigned char *ucpXPublKey, unsigned int uiLenYPublKey,
  const unsigned char *ucpYPublKey, unsigned int uiLenData, const unsigned char *ucpData,
  unsigned int uiLenSigC, const unsigned char *ucpSigC, unsigned int uiLenSigD,
  const unsigned char *ucpSigD, int *bpResult);

STM_API1 int STM_API2 STM_Eject(stmHandle hCard);

STM_API1 int STM_API2 STM_EjectICC(stmHandle hCard, unsigned int uiDest);

STM_API1 int STM_API2 STM_EraseObjectFile(stmHandle hCard);

STM_API1 int STM_API2 STM_ExchangeChallenge(stmHandle hCard, const unsigned char *ucpMyChall,
  unsigned char *ucpCardChall);

STM_API1 int STM_API2 STM_FreeCard(stmHandle hCard);

STM_API1 int STM_API2 STM_GenerateAESKey(unsigned int uiAlgo, unsigned char* ucpKey);

STM_API1 int STM_API2 STM_GenerateAndWriteDSAKey(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenKeyData, unsigned int uiAlgo, unsigned int uiLenPrimeP,
  unsigned char *ucpPrimeP, unsigned char *ucpPrimeQ, unsigned int *uipActLenBaseG,
  unsigned char *ucpBaseG, unsigned int *uipActLenPubKey, unsigned char *ucpPubKey,
  unsigned int *uipActLenSeed, unsigned char *ucpSeed, int *ipCounter);

STM_API1 int STM_API2 STM_GenerateAndWriteRSAKey(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiFormat, unsigned int uiLenKeyData, unsigned int uiAlgo, 
  unsigned int uiLenKey, unsigned int uiBitDiff, unsigned int uiLenPublExp, 
  const unsigned char *ucpPublExp, unsigned char *ucpModulus);

STM_API1 int STM_API2 STM_GenerateDESKey(unsigned int uiAlgo, unsigned char *ucpKey);

STM_API1 int STM_API2 STM_GenerateDSAKey(unsigned int uiAlgo, unsigned int uiLenPrimeP,
  unsigned char *ucpPrimeP, unsigned char *ucpPrimeQ, unsigned int *uipActLenBaseG,
  unsigned char *ucpBaseG, unsigned char *ucpSecKey, unsigned int *uipActLenPubKey,
  unsigned char *ucpPubKey, unsigned int *uipActLenSeed, unsigned char *ucpSeed,
  int *ipCounter);

STM_API1 int STM_API2 STM_GenerateECKey( const unsigned char *ucpECParam,
  unsigned int uiLenPrivKey, unsigned int *uipActLenPrivKey, unsigned char *ucpPrivKey,
  unsigned int uiLenPublKey, unsigned int *uipActLenPublKey, unsigned char *ucpXPublKey,
  unsigned char *ucpYPublKey);

STM_API1 int STM_API2 STM_GeneratePublicKeyPair(stmHandle hCard, unsigned int uiMode,
  unsigned int uiKeyNo, unsigned int uiKeyLevel);

STM_API1 int STM_API2 STM_GenerateRSAKey(unsigned int uiAlgo, unsigned int uiLenKey,
  unsigned int uiBitDiff, unsigned int uiLenPublExp, const unsigned char *ucpPublExp,
  unsigned char *ucpModulus, unsigned int *uipActLenSecExp, unsigned char *ucpSecExp,
  unsigned int uiLenPrimeP, unsigned int *uipActLenPrimeP, unsigned char *ucpPrimeP, 
  unsigned int uiLenPrimeQ, unsigned int *uipActLenPrimeQ, unsigned char *ucpPrimeQ,
  unsigned int uiLenDP, unsigned int *uipActLenDP, unsigned char *ucpDP,
  unsigned int uiLenDQ, unsigned int *uipActLenDQ, unsigned char *ucpDQ,
  unsigned int uiLenPInv, unsigned int *uipActLenPInv, unsigned char *ucpPInv,
  unsigned int uiLenQInv, unsigned int *uipActLenQInv, unsigned char *ucpQInv);

STM_API1 int STM_API2 STM_GetATR(stmHandle hCard, unsigned int uiLenATR, 
  unsigned int *uipActLenATR, unsigned char *ucpATR);

STM_API1 int STM_API2 STM_GetCardData(stmHandle hCard, unsigned int uiMode, unsigned int uiLen,
  unsigned int uiLenData, unsigned int *uipActLenData, unsigned char *ucpData);

STM_API1 int STM_API2 STM_GetCardType(stmHandle hCard, unsigned int *uipType);

STM_API1 int STM_API2 STM_GetChallenge(stmHandle hCard, unsigned int uiLen,
  unsigned int uiLenDataBuf, unsigned int *uipActLenData, unsigned char *ucpData);

STM_API1 int STM_API2 STM_GetChannel(stmHandle hCard, unsigned int *uipChannel);

STM_API1 int STM_API2 STM_GetData(stmHandle hCard, const unsigned char *ucpTag, 
  unsigned int uiResBufLen, unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_GetECpParam(unsigned int uiLenPrimeP,
  const unsigned char *ucpPrimeP, unsigned int uiLenCoeffA,
  const unsigned char *ucpCoeffA, unsigned int uiLenCoeffB,
  const unsigned char *ucpCoeffB, unsigned int uiLenXGenG,
  const unsigned char *ucpXGenG, unsigned int uiLenYGenG,
  const unsigned char *ucpYGenG, unsigned int uiLenOrderR,
  const unsigned char *ucpOrderR, unsigned int uiLenCofactorK,
  const unsigned char *ucpCofactorK, unsigned int uiLenECParam,
  unsigned int *uipActLenECParam, unsigned char *ucpECParam);

STM_API1 int STM_API2 STM_GetError(stmHandle hCard, int iLevel, 
  int *ipErrorCode, unsigned int uiLenAddInfo, unsigned int *uipActLenAddInfo,
  unsigned char *ucpAddInfo);

STM_API1 int STM_API2 STM_GetErrorDepth(stmHandle hCard, int *ipDepth);

STM_API1 int STM_API2 STM_GetErrorSW(stmHandle hCard, int *ipSW);

STM_API1 int STM_API2 STM_GetIOBufferSize(stmHandle hCard, unsigned int *uipSize);

STM_API1 int STM_API2 STM_GetKeyId(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int *uipKID);

STM_API1 int STM_API2 STM_GetKeyInfo(stmHandle hCard, unsigned int uiSearchMode,
  unsigned int uiKeyNo, unsigned int uiResBufLen, unsigned int *uipActResLen,
  unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_GetKeyReference(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiKeyVersion, unsigned char* ucpKeyReference);

STM_API1 int STM_API2 STM_GetKFPC(stmHandle hCard, int *ipKFPC);

STM_API1 int STM_API2 STM_GetMsg(int iMsgCode, unsigned int uiMsgLanguage, 
  unsigned int uiLenMsgTxt, char *szTxt);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_GetMsgA(int iMsgCode, unsigned int uiMsgLanguage, 
  unsigned int uiLenMsgTxt, char *szTxt);

STM_API1 int STM_API2 STM_GetMsgW(int iMsgCode, unsigned int uiMsgLanguage, 
  unsigned int uiLenMsgTxt, wchar_t *szTxt);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_GetMsg STM_GetMsgW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

//undocumented!
STM_API1 int STM_API2 STM_GetPortId(const char *szTermName, unsigned int *uipPortId);

#ifndef STM_BUILD_INTPMAK
//undocumented!
STM_API1 int STM_API2 STM_GetPortIdA(const char *szTermName, unsigned int *uipPortId);

//undocumented!
STM_API1 int STM_API2 STM_GetPortIdW(const wchar_t *szTermName, unsigned int *uipPortId);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_GetPortId STM_GetPortIdW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

//deprecated, undocumented!
STM_API1 int STM_API2 STM_GetPrivateClass(stmHandle hCard, unsigned char *ucpClassByte);

//undocumented!
STM_API1 int STM_API2 STM_GetSecureMessagingMode(stmHandle hCard, unsigned int *uipMode);

//undocumented!
STM_API1 int STM_API2 STM_GetSTARCOSVersion(stmHandle hCard, unsigned int uiLenBuf, 
  unsigned int *uipActLen, unsigned char *ucpVersion);

STM_API1 int STM_API2 STM_GetStatus(stmHandle hCard, unsigned int uiSubSet,
  unsigned int uiLenSearchCriteria, const unsigned char *ucpSearchCriteria,
  unsigned int uiLenStatusInfo, unsigned int *uipActLenStatusInfo,
  unsigned char *ucpStatusInfo);

STM_API1 int STM_API2 STM_GetTermList(unsigned int uiListMode,
  unsigned int uiListBufLen, unsigned int *uipActListLen, char *mszTermList);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_GetTermListA(unsigned int uiListMode,
  unsigned int uiListBufLen, unsigned int *uipActListLen, char *mszTermList);

STM_API1 int STM_API2 STM_GetTermListW(unsigned int uiListMode,
  unsigned int uiListBufLen, unsigned int *uipActListLen, wchar_t *mszTermList);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_GetTermList STM_GetTermListW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_GetTermType(stmHandle hCard, unsigned int *uipType);

STM_API1 int STM_API2 STM_Hash(unsigned int uiAlgo, unsigned int uiPadMode, 
  unsigned int uiLenInput, const unsigned char *ucpInput, unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_HashData(stmHandle hHash, unsigned int uiLenInput, 
  const unsigned char *ucpInput, unsigned char *ucpOutput, unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_HashFinish(stmHandle hHash, unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_HashInit(stmHandle *hpHash, unsigned int uiAlgo, 
  unsigned int uiPadMode);

STM_API1 int STM_API2 STM_HashInitPre(stmHandle *hpHash, unsigned uiAlgo, unsigned uiPadMode,
  const unsigned char* ucpPreHash, const unsigned char* ucpCounter);

STM_API1 int STM_API2 STM_ICCReset(stmHandle hCard);

STM_API1 int STM_API2 STM_ICCSetPTS(stmHandle hCard, unsigned int uiDest,
  unsigned int uiPTSMode, unsigned int uiPTSLen, const unsigned char *ucpPTSData);

STM_API1 int STM_API2 STM_ICCStatus(stmHandle hCard, unsigned int uiDest,
  unsigned int *uipStatus);

STM_API1 int STM_API2 STM_Increase(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiLen, const unsigned char *ucpValue, unsigned int uiLenExtraData,
  const unsigned char *ucpExtraData, unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_InitializeLibrary(int bSetNewHandler);

STM_API1 int STM_API2 STM_Install(stmHandle hCard, unsigned int uiMode,
  unsigned int uiLenData, const unsigned char *ucpData, unsigned int uiLenReceipt,
  unsigned int *uipActLenReceipt, unsigned char *ucpReceipt);

STM_API1 int STM_API2 STM_InstallKey(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiLenHeader, const unsigned char *ucpHeader, 
  unsigned int uiLenKey, const unsigned char *ucpKey);

STM_API1 int STM_API2 STM_Journal(unsigned int uiMode);

STM_API1 int STM_API2 STM_JournalOn(unsigned int uiJouBufSize);

STM_API1 int STM_API2 STM_JournalSave(const char *szJournalFile, unsigned int uiSaveMode,
  unsigned int uiLineFeed);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_JournalSaveA(const char *szJournalFile, unsigned int uiSaveMode,
  unsigned int uiLineFeed);

STM_API1 int STM_API2 STM_JournalSaveW(const wchar_t *szJournalFile, unsigned int uiSaveMode,
  unsigned int uiLineFeed);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_JournalSave STM_JournalSaveW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_KeyedHash(unsigned int uiAlgo, unsigned int uiPadMode, 
  unsigned int uiLenKey, const unsigned char *ucpKey,  unsigned int uiLenInput,
  const unsigned char *ucpInput, unsigned char *ucpOutput);

STM_API1 int STM_API2 STM_KeyedHashData(stmHandle hKeyedHash, unsigned int uiLenInput,
  const unsigned char *ucpInput);

STM_API1 int STM_API2 STM_KeyedHashFinish(stmHandle hKeyedHash, unsigned char* ucpOutput);

STM_API1 int STM_API2 STM_KeyedHashInit(stmHandle *hpKeyedHash, unsigned int uiAlgo,
  unsigned int uiPadMode, unsigned int uiLenKey, const unsigned char *ucpKey);

STM_API1 int STM_API2 STM_LibVersion(char *szVersion);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_LibVersionA(char *szVersion);

STM_API1 int STM_API2 STM_LibVersionW(wchar_t *szVersion);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_LibVersion STM_LibVersionW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_Load(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiLenReceipt, unsigned int *uipActLenReceipt,
  unsigned char *ucpReceipt);

STM_API1 int STM_API2 STM_LockFile(stmHandle hCard, unsigned int uiFileID, unsigned int uiMode);

STM_API1 int STM_API2 STM_ManageSE(stmHandle hCard, unsigned int uiMode, unsigned int uiSENo);

STM_API1 int STM_API2 STM_Open(stmHandle *hpCard, unsigned int uiPortId,
  unsigned int uiAccessMode);

STM_API1 int STM_API2 STM_OpenChannel(stmHandle hCard, unsigned int uiChannel);

STM_API1 int STM_API2 STM_OpenEx(stmHandle *hpCard, const char *szTermName,
  unsigned int uiAccessMode);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_OpenExA(stmHandle *hpCard, const char *szTermName,
  unsigned int uiAccessMode);

STM_API1 int STM_API2 STM_OpenExW(stmHandle *hpCard, const wchar_t *szTermName,
  unsigned int uiAccessMode);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_OpenEx STM_OpenExW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_PCCTIConfig(unsigned int uiConfigMode, unsigned int uiInp,
  unsigned char *ucpPar1, unsigned char *ucpPar2);

STM_API1 int STM_API2 STM_PutData(stmHandle hCard, const unsigned char *ucpTag,
  unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_PutHash(stmHandle hCard, const unsigned char *ucpPrehash);

STM_API1 int STM_API2 STM_PutKey(stmHandle hCard, unsigned int uiMode,
  unsigned int uiKeyNo, unsigned int uiOldKeyVersion, unsigned int uiNewKeyVersion,
  unsigned int uiLenKeyData, const unsigned char *ucpKeyData, unsigned int uiLenReceipt,
  unsigned int *uipActLenReceipt, unsigned char *ucpReceipt);

STM_API1 int STM_API2 STM_PutKeyData(stmHandle hCard, unsigned int uiMode,
  unsigned int uiAlgo, unsigned int uiLenKeyData, const unsigned char *ucpKeyData);

STM_API1 int STM_API2 STM_PutKeyFinish(stmHandle hCard, unsigned int uiLenReceipt,
  unsigned int *uipActLenReceipt, unsigned char *ucpReceipt);

STM_API1 int STM_API2 STM_PutKeyInit(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiOldKeyVersion, unsigned int uiNewKeyVersion);

STM_API1 int STM_API2 STM_Rand(unsigned int uiLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_RandHardwareInit(unsigned int uiType,
  stmfctHardwareCbk pfctHardwareCbk);

STM_API1 int STM_API2 STM_RandInit(unsigned int uiType, const unsigned char *ucpSeed);

STM_API1 int STM_API2 STM_ReadBinary(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiOffset, unsigned int uiLen, unsigned int uiLenDataBuf, 
  unsigned int *uipActLenData, unsigned char *ucpData);

STM_API1 int STM_API2 STM_ReadCHID(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenCHID,
  unsigned int *uipActLenCHID, unsigned char *ucpCHID);

STM_API1 int STM_API2 STM_ReadKeyNo(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiLenCHID, const unsigned char *ucpCHID, unsigned int *uipKeyNo,
  unsigned int *uipKeyLevel);

STM_API1 int STM_API2 STM_ReadKeyStatus(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyType, unsigned int uiLenStatus, unsigned int *uipActLenStatus, 
  unsigned char *ucpStatus);

STM_API1 int STM_API2 STM_ReadPublicDSAKey(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenPrimeP,
  unsigned int *uipActLenPrimeP, unsigned char *ucpPrimeP, unsigned char *ucpPrimeQ,
  unsigned char *ucpBaseG, unsigned char *ucpPublKey);

STM_API1 int STM_API2 STM_ReadPublicKeyRecord(stmHandle hCard, unsigned int uiLenCHID,
  const unsigned char *ucpCHID, unsigned int uiRecBufLen, unsigned int *uipActRecLen,
  unsigned char *ucpKeyRecord);

STM_API1 int STM_API2 STM_ReadPublicKeySignature(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenCHID, const unsigned char *ucpCHID,
  unsigned int uiSigBufLen, unsigned int *uipActSigLen, unsigned char *ucpSignature);

STM_API1 int STM_API2 STM_ReadPublicRSAKey(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenModulus,
  unsigned int *uipBitLenModulus, unsigned int *uipActLenModulus, unsigned char *ucpModulus,
  unsigned int uiLenPublExp, unsigned int *uipActLenPublExp, unsigned char *ucpPublExp);

STM_API1 int STM_API2 STM_ReadRecord(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiRecordId, unsigned int uiLen, unsigned int uiLenDataBuf, 
  unsigned int *uipActLenData, unsigned char *ucpData);

STM_API1 int STM_API2 STM_RegisterDF(stmHandle hCard, unsigned int uiLenDF, unsigned int uiFID,
  unsigned int uiLenAID, const unsigned char *ucpAID);

STM_API1 int STM_API2 STM_Request(stmHandle hCard, unsigned int uiDest, unsigned int uiAccessMode);

STM_API1 int STM_API2 STM_ResetRetryCounter(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenResettingCode,
  const unsigned char *ucpResettingCode, unsigned int uiLenNewPIN,
  const unsigned char *ucpNewPIN, int *bpResult); 

STM_API1 int STM_API2 STM_RSACRTDecipher(unsigned int uiLenPrimeP,
  const unsigned char *ucpPrimeP, unsigned int uiLenPrimeQ, const unsigned char *ucpPrimeQ,
  unsigned int uiLenDP, const unsigned char *ucpDP, unsigned int uiLenDQ,
  const unsigned char *ucpDQ, unsigned int uiLenPInv, const unsigned char *ucpPInv,
  unsigned int uiLenQInv, const unsigned char *ucpQInv, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiPadMode, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_RSACRTSign(unsigned int uiLenPrimeP,
  const unsigned char *ucpPrimeP, unsigned int uiLenPrimeQ, const unsigned char *ucpPrimeQ,
  unsigned int uiLenDP, const unsigned char *ucpDP, unsigned int uiLenDQ,
  const unsigned char *ucpDQ, unsigned int uiLenPInv, const unsigned char *ucpPInv,
  unsigned int uiLenQInv, const unsigned char *ucpQInv, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiPadMode, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_RSADecipher(unsigned int uiLenKey, const unsigned char *ucpModulus,
  unsigned int uiLenSecExp, const unsigned char *ucpSecExp, unsigned int uiLenData, 
  const unsigned char *ucpData, unsigned int uiPadMode, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_RSAEncipher(unsigned int uiLenKey, const unsigned char *ucpModulus,
  unsigned int uiLenPubExp, const unsigned char *ucpPubExp, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiPadMode, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_RSASign(unsigned int uiLenKey, const unsigned char *ucpModulus,
  unsigned int uiLenSecExp, const unsigned char *ucpSecExp, unsigned int uiLenData,
  const unsigned char *ucpData, unsigned int uiPadMode, unsigned char *ucpResult);

STM_API1 int STM_API2 STM_RSAVerify(unsigned int uiLenKey, const unsigned char *ucpModulus, 
  unsigned int uiLenPubExp, const unsigned char *ucpPubExp, unsigned int uiLenData, 
  const unsigned char *ucpData, const unsigned char *ucpSig, 
  unsigned int uiPadMode, int *bpResult);

STM_API1 int STM_API2 STM_SearchRecord(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiMode, unsigned int uiRecordId, unsigned int uiLenSearchData,
  const unsigned char *ucpSearchData, unsigned int uiResBufLen,
  unsigned int *uipActResLen, unsigned char *ucpResBuf);

STM_API1 int STM_API2 STM_SecureDecrease(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, const unsigned char *ucpData, unsigned int uiKeyInfo,
  const unsigned char *ucpKey, unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_SecureIncrease(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, const unsigned char *ucpData, unsigned int uiKeyInfo,
  const unsigned char *ucpKey, unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_SecureMessaging(stmHandle hCard, unsigned int uiMode);

STM_API1 int STM_API2 STM_SelectFn(stmHandle hCard, unsigned int uiSelCtrl, unsigned int uiLenAID, 
  const unsigned char *ucpAID, unsigned int uiLenInfo, unsigned int *uipActLenInfo,
  unsigned char *ucpInfo);

STM_API1 int STM_API2 STM_SelectId(stmHandle hCard, unsigned int uiSelCtrl, 
  unsigned int uiFID, unsigned int uiLenInfo, unsigned int *uipActLenInfo, unsigned char *ucpInfo);

STM_API1 int STM_API2 STM_SendCard(stmHandle hCard, unsigned int uiLenCmd, 
  const unsigned char *ucpCmd, unsigned int uiLenResp, unsigned int* uipActLenResp,  
  unsigned char *ucpResp, unsigned int uiMinLenResp, unsigned int uiSW, unsigned int uiSWMask);

STM_API1 int STM_API2 STM_SendTerminal(stmHandle hCard, unsigned int uiLenCmd, 
  const unsigned char *ucpCmd,unsigned int uiLenResp, unsigned int *uipActLenResp,  
  unsigned char *ucpResp, unsigned int uiMinLenResp, unsigned int uiSW, unsigned int uiSWMask);

STM_API1 int STM_API2 STM_SetCardType(stmHandle hCard, unsigned int uiType);

STM_API1 int STM_API2 STM_SetCID(stmHandle hCard, unsigned uiLenCID,
  const unsigned char *ucpCID, int bSetSE);

//deprecated, undocumented!
STM_API1 int STM_API2 STM_SetEjectDisplay(stmHandle hCard, unsigned char ucEjTime,
  int bBeep, int bBlink, const char *szDispTxt);

STM_API1 int STM_API2 STM_SetEjectParam(stmHandle hCard, unsigned char ucEjTime,
  unsigned int uiEjMode, const char *szDispTxt);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_SetEjectParamA(stmHandle hCard, unsigned char ucEjTime,
  unsigned int uiEjMode, const char *szDispTxt);

STM_API1 int STM_API2 STM_SetEjectParamW(stmHandle hCard, unsigned char ucEjTime,
  unsigned int uiEjMode, const wchar_t *szDispTxt);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_SetEjectParam STM_SetEjectParamW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_SetErrorHandling(int bOutOfMemMsg, int bInitDLLMsg,   
  int bExceptMsg, int bOutOfMemTerm, int bInitDLLTerm, int bExceptTerm);

STM_API1 int STM_API2 STM_SetHash(stmHandle hCard, const unsigned char *ucpPrehash,
  const unsigned char *ucpCounter);

STM_API1 int STM_API2 STM_SetInternalSync(int bProcessSync, int bThreadSync);

STM_API1 int STM_API2 STM_SetIOBufferSize(stmHandle hCard, unsigned int uiSize);

//undocumented!
STM_API1 int STM_API2 STM_SetJournalCallbackFunction(stmfctJouCbk pfctJouCbk, int bChain);

//deprecated, undocumented!
STM_API1 int STM_API2 STM_SetPrivateClass(stmHandle hCard, unsigned char ucClassByte);

//deprecated, undocumented!
STM_API1 int STM_API2 STM_SetRequestDisplay(stmHandle hCard, unsigned char ucRegTime,
  const char *szDispTxt);

STM_API1 int STM_API2 STM_SetRequestParam(stmHandle hCard, unsigned char ucRegTime,
  const char *szDispTxt);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_SetRequestParamA(stmHandle hCard, unsigned char ucRegTime,
  const char *szDispTxt);

STM_API1 int STM_API2 STM_SetRequestParamW(stmHandle hCard, unsigned char ucRegTime,
  const wchar_t *szDispTxt);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_SetRequestParam STM_SetRequestParamW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_SetSE(stmHandle hCard, unsigned int uiMode, unsigned int uiTemplate,
  unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_SetSecureMessagingKey(stmHandle hCard, unsigned int uiSECMMode,
  unsigned int uiKeyNo, unsigned int uiKeyVersion, unsigned int uiKeyLevel,
  unsigned int uiKeyInfo, unsigned int uiKeyType, const unsigned char *ucpKey);

//deprecated, undocumented!
STM_API1 int STM_API2 STM_SetSecureMessagingKeys(stmHandle hCard, unsigned int uiCryptKeyInfo, 
  unsigned int uiCryptKeyType, const unsigned char *ucpCryptKey, unsigned int uiAuthKeyInfo, 
  unsigned int uiAuthKeyType, const unsigned char *ucpAuthKey);

//deprecated, undocumented!
STM_API1 int STM_API2 STM_SetSecurityEnvironment(stmHandle hCard, unsigned int uiTemplate,
  unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_SetStatus(stmHandle hCard, unsigned int uiStatus,
  unsigned int uiLenAID, const unsigned char *ucpAID);

//deprecated!
STM_API1 int STM_API2 STM_SingleDES(unsigned int uiCryptMode, unsigned int uiChainMode, 
  unsigned int uiPadMode,  const unsigned char *ucpKey, const unsigned char *ucpIV,
  unsigned int uiLenData, const unsigned char *ucpData, unsigned int *uipLenResult,
  unsigned char *ucpResult);

STM_API1 int STM_API2 STM_StoreData(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData);

STM_API1 int STM_API2 STM_TermAccessControl(stmHandle hCard, unsigned int uiAccessControl,
  unsigned long ulTimeout);

STM_API1 int STM_API2 STM_TermChangePIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermChangePINA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

STM_API1 int STM_API2 STM_TermChangePINW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermChangePIN STM_TermChangePINW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermChangePINEx(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenOldPINMask, const unsigned char *ucpOldPINMask,
  unsigned int uiPosOldPIN, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermChangePINExA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenOldPINMask, const unsigned char *ucpOldPINMask,
  unsigned int uiPosOldPIN, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

STM_API1 int STM_API2 STM_TermChangePINExW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenOldPINMask, const unsigned char *ucpOldPINMask,
  unsigned int uiPosOldPIN, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermChangePINEx STM_TermChangePINExW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

//undocumented!
STM_API1 int STM_API2 STM_TermConfig(stmHandle hCard, unsigned int uiConfigMode,
  unsigned char *ucpPar1, unsigned char *ucpPar2);

STM_API1 int STM_API2 STM_TermDebug(stmHandle hCard, unsigned int uiLenCmd, 
  unsigned int *uipActLenCmd, unsigned char *ucpCmd, unsigned int uiLenResp, 
  unsigned int *uipActLenResp, unsigned char *ucpResp); 

STM_API1 int STM_API2 STM_TermDisplay(stmHandle hCard, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermDisplayA(stmHandle hCard, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData);

STM_API1 int STM_API2 STM_TermDisplayW(stmHandle hCard, unsigned char ucTime, 
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermDisplay STM_TermDisplayW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TerminateCardUsage(stmHandle hCard);

STM_API1 int STM_API2 STM_TerminateFile(stmHandle hCard, unsigned uiTermMode);

STM_API1 int STM_API2 STM_TerminatePIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel);

STM_API1 int STM_API2 STM_TermInput(stmHandle hCard, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  unsigned int uiInputLen, int bIsPIN, unsigned int uiRespBufLen, unsigned int *uipActLenResp,
  unsigned char *ucpRespBuf);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermInputA(stmHandle hCard, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  unsigned int uiInputLen, int bIsPIN, unsigned int uiRespBufLen, unsigned int *uipActLenResp,
  unsigned char *ucpRespBuf);

STM_API1 int STM_API2 STM_TermInputW(stmHandle hCard, unsigned char ucTime, 
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  unsigned int uiInputLen, int bIsPIN, unsigned int uiRespBufLen, unsigned int *uipActLenResp,
  unsigned char *ucpRespBuf);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermInput STM_TermInputW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermReset(stmHandle hCard);

STM_API1 int STM_API2 STM_TermResetRetryCounter(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenPIN, int bIsASCII, 
  unsigned char ucTime, const char *szDispTxt, unsigned int uiDiscrDataLen,
  const unsigned char *ucpDiscrData, int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermResetRetryCounterA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenPIN, int bIsASCII,
  unsigned char ucTime, const char *szDispTxt, unsigned int uiDiscrDataLen,
  const unsigned char *ucpDiscrData, int *bpResult);

STM_API1 int STM_API2 STM_TermResetRetryCounterW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenPIN, int bIsASCII,
  unsigned char ucTime, const wchar_t *szDispTxt, unsigned int uiDiscrDataLen,
  const unsigned char *ucpDiscrData, int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermResetRetryCounter STM_TermResetRetryCounterW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermResetRetryCounterEx(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenResettingCodeMask,
  const unsigned char *ucpResettingCodeMask, unsigned int uiPosResettingCode,
  unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermResetRetryCounterExA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenResettingCodeMask,
  const unsigned char *ucpResettingCodeMask, unsigned int uiPosResettingCode,
  unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

STM_API1 int STM_API2 STM_TermResetRetryCounterExW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiResettingKeyNo, unsigned int uiLenResettingCodeMask,
  const unsigned char *ucpResettingCodeMask, unsigned int uiPosResettingCode,
  unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermResetRetryCounterEx STM_TermResetRetryCounterExW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermUnblockPIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermUnblockPINA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

STM_API1 int STM_API2 STM_TermUnblockPINW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermUnblockPIN STM_TermUnblockPINW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermUnblockPINEx(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPUKMask, const unsigned char *ucpPUKMask,
  unsigned int uiPosPUK, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermUnblockPINExA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPUKMask, const unsigned char *ucpPUKMask,
  unsigned int uiPosPUK, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

STM_API1 int STM_API2 STM_TermUnblockPINExW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPUKMask, const unsigned char *ucpPUKMask,
  unsigned int uiPosPUK, unsigned int uiLenNewPINMask, const unsigned char *ucpNewPINMask,
  unsigned int uiPosNewPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime,
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData,
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermUnblockPINEx STM_TermUnblockPINExW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermVerifyPIN(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermVerifyPINA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

STM_API1 int STM_API2 STM_TermVerifyPINW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermVerifyPIN STM_TermVerifyPINW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermVerifyPINEx(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPINMask, const unsigned char *ucpPINMask,
  unsigned int uiPosPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermVerifyPINExA(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPINMask, const unsigned char *ucpPINMask,
  unsigned int uiPosPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const char *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

STM_API1 int STM_API2 STM_TermVerifyPINExW(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenPINMask, const unsigned char *ucpPINMask,
  unsigned int uiPosPIN, unsigned int uiLenPIN, int bIsASCII, unsigned char ucTime, 
  const wchar_t *szDispTxt, unsigned int uiDiscrDataLen, const unsigned char *ucpDiscrData, 
  int *bpResult);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermVerifyPINEx STM_TermVerifyPINExW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

STM_API1 int STM_API2 STM_TermVersion(stmHandle hCard, char *szVersion);

#ifndef STM_BUILD_INTPMAK
STM_API1 int STM_API2 STM_TermVersionA(stmHandle hCard, char *szVersion);

STM_API1 int STM_API2 STM_TermVersionW(stmHandle hCard, wchar_t *szVersion);

#ifndef STM_BUILD_UNICODE
  #if defined(UNICODE) || defined(_UNICODE)
    #define STM_TermVersion STM_TermVersionW
  #endif
#endif
#endif //STM_BUILD_INTPMAK

//deprecated!
STM_API1 int STM_API2 STM_TripleDES(unsigned int uiCryptMode, unsigned int uiChainMode, 
  unsigned int uiPadMode, const unsigned char *ucpKey, const unsigned char *ucpIV,
  unsigned int uiLenData, const unsigned char *ucpData, unsigned int *uipLenResult,
  unsigned char *ucpResult);

STM_API1 int STM_API2 STM_UnblockPIN(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiLenPUK, const unsigned char *ucpPUK,
  unsigned int uiLenNewPIN, const unsigned char *ucpNewPIN, int *bpResult); 

STM_API1 int STM_API2 STM_UpdateBinary(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiOffset, unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_UpdateRecord(stmHandle hCard, unsigned int uiShortId, 
  unsigned int uiMode, unsigned int uiRecordId, unsigned int uiLenData,
  const unsigned char *ucpData);

STM_API1 int STM_API2 STM_VerifyBiometric(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiLenBioData, const unsigned char *ucpBioData,
  int *ipScore, int *bpResult); 

STM_API1 int STM_API2 STM_VerifyCertificate(stmHandle hCard, unsigned int uiLenData,
  const unsigned char *ucpData, int *bpResult);

STM_API1 int STM_API2 STM_VerifyPIN(stmHandle hCard, unsigned int uiKeyNo, 
  unsigned int uiKeyLevel, unsigned int uiLenPIN, const unsigned char *ucpPIN,
  int *bpResult); 

STM_API1 int STM_API2 STM_VerifySignature(stmHandle hCard, unsigned int uiLenSig,
  const unsigned char *ucpSig, int *bpResult);

//undocumented!
STM_API1 int STM_API2 STM_WriteBinary(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiOffset, unsigned int uiLenData, const unsigned char *ucpData);

STM_API1 int STM_API2 STM_WriteCHID(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenCHID,
  const unsigned char *ucpCHID);

STM_API1 int STM_API2 STM_WriteKey(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiLenKey, const unsigned char *ucpKey);

STM_API1 int STM_API2 STM_WritePrivateDSAKey(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiFormat, unsigned int uiLenKeyData,
  unsigned int uiLenPrimeP, const unsigned char *ucpPrimeP, unsigned int uiLenPrimeQ,
  const unsigned char *ucpPrimeQ, unsigned int uiLenBaseG,
  const unsigned char *ucpBaseG, unsigned int uiLenSecKey,
  const unsigned char *ucpSecKey);

STM_API1 int STM_API2 STM_WritePrivateRSAKey(stmHandle hCard, unsigned int uiKeyNo,
  unsigned int uiKeyLevel, unsigned int uiFormat, unsigned int uiLenKeyData,
  unsigned int uiLenModulus, const unsigned char *ucpModulus, unsigned int uiLenSecExp,
  const unsigned char *ucpSecExp, unsigned int uiLenPrimeP, const unsigned char *ucpPrimeP,
  unsigned int uiLenPrimeQ, const unsigned char *ucpPrimeQ);

STM_API1 int STM_API2 STM_WritePublicDSAKey(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenPrimeP,
  const unsigned char *ucpPrimeP, unsigned int uiLenPrimeQ,
  const unsigned char *ucpPrimeQ, unsigned int uiLenBaseG,
  const unsigned char *ucpBaseG, unsigned int uiLenPublKey,
  const unsigned char *ucpPublKey);

STM_API1 int STM_API2 STM_WritePublicRSAKey(stmHandle hCard, unsigned int uiShortId,
  unsigned int uiKeyNo, unsigned int uiKeyLevel, unsigned int uiLenModulus,
  const unsigned char *ucpModulus, unsigned int uiLenPublExp,
  const unsigned char *ucpPublExp);

#if defined(__cplusplus) /* closing brake of extern "C" { */
}
#endif /* __cplusplus */


/** **************************************************************************** **/
/** reset struct and class member alignment                                      **/
/** **************************************************************************** **/
#if defined(_MSC_VER)
  #if defined(STM_OS_WIN32)
    #pragma pack(pop, old_pack)
  #else
    #pragma pack()
  #endif
#elif defined(__TURBOC__)
  #pragma option -a.
#endif 


#endif /* STM_API_H */
