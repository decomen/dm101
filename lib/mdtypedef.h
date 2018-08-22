/**************************************************************************************************
  Revised:        2014-12-04
  Author:         Zhu Jie . Jay . Sleepace
**************************************************************************************************/

/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
 *   重定义
 * ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
 */

#ifndef __MD_TYPEDEF_H__
#define __MD_TYPEDEF_H__

#include <string.h>
#include <stdio.h>
#include "stdint.h"

#define MD_ABS(_x)      ((_x)>=0?(_x):(-_x))
#define MD_OFS(_t, _m)  (unsigned int)(&(((_t *)0)->_m))
#define MD_IS_UPPER(_c) (((_c)>='A')&&((_c)<='Z'))
#define MD_IS_LOWER(_c) (((_c)>='a')&&((_c)<='z'))
#define MD_IS_DIGIT(_c) (((_c)>='0')&&((_c)<='9'))

#define mdNULL      0
#define mdTRUE      1
#define mdFALSE     0

typedef int8_t          mdINT8;
typedef int16_t         mdINT16;
typedef int32_t         mdINT32;
typedef int64_t         mdINT64;

typedef uint8_t         mdUINT8;
typedef uint16_t        mdUINT16;
typedef uint32_t        mdUINT32;
typedef uint64_t        mdUINT64;

// Special numbers

typedef mdUINT8         mdBOOL;
typedef mdUINT16	    mdWORD;
typedef mdUINT32	    mdDWORD;

// Unsigned numbers
typedef unsigned char   mdBYTE;
typedef unsigned char   mdUCHAR;
typedef unsigned short  mdUSHORT;
// int, long 平台相关
typedef unsigned int    mdUINT;
typedef unsigned long   mdULONG;

// Signed numbers
typedef signed char     mdCHAR;
typedef signed short    mdSHORT;
// int, long 平台相关
typedef signed int      mdINT;
typedef signed long     mdLONG;

// decimal
typedef float           mdFLOAT;
typedef double          mdDOUBLE;

typedef struct MD_DATA {
    mdUSHORT usDataLen;
    mdBYTE *pData;
} MD_Data_t;

typedef struct xMD_WEEK_SELECT {
    mdBOOL bMON :1;
    mdBOOL bTUE :1;
    mdBOOL bWED :1;
    mdBOOL bTHU :1;
    mdBOOL bFRI :1;
    mdBOOL bSTA :1;
    mdBOOL bSUN :1;
} MD_WeekSelect_t;

typedef enum {
    MD_ENOERR,      // 正常
    MD_EPARAM,      // 参数异常
    MD_EOVERFLOW,   // 越界
    MD_ENOINIT,     // 未初始化

    MD_EBUSY,       // 忙
    MD_ETIMEOUT,    // 超时

    MD_EUNKNOWN     // 未知错误
} eMDErrorCode;

typedef enum {
    MD_VER_TYPE_INVALID     = 0x00,     //无效
    MD_VER_TYPE_DEBUG       = 0x01,     //调试版
    MD_VER_TYPE_BETA        = 0x02,     //测试版
    MD_VER_TYPE_RELEASE     = 0x03,     //发行版本
} eMDVerType;

typedef enum {
    MD_IDE_TYPE_IAR         = 0x00,     //IAR
    MD_IDE_TYPE_KEIL        = 0x01,     //KEIL
    MD_IDE_TYPE_GCC         = 0x02,     //GCC
} eMDIDEType;

typedef enum {
    MD_MODEL_BLE_B502       = 0x0010,   //枕扣
    // ...
} eMDModel;

// 范围 0x00-0x0F
typedef enum {
    MD_NET_MODEL_NOX        = 0x00,     //Nox
    MD_NET_MODEL_RESTON     = 0x01,     //WIFI版RestOn
    MD_NET_MODEL_MAT_ONE    = 0x02,     //WIFI版单人
    MD_NET_MODEL_MAT_TWO    = 0x03,     //WIFI版双人
} eMDNetModel;

#define BIN_MAGIC_WORD      (0xF57BA045930AE418ULL)
typedef struct xMD_BIN_INFO {
    mdINT64   ullMagic;     // 8字节魔数(指纹信息,防止读取错误信息)
    mdBYTE    btIDEType;    // 开发环境
    mdUSHORT  usModel;      // 设备类型
    mdBYTE    btVerType;    // 固件类型
    mdUSHORT  usVerCode;    // 固件版本
} MD_BinInfo_t;

#endif

