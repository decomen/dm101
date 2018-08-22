/**************************************************************************************************
  Revised:        2014-12-04
  Author:         Zhu Jie . Jay . Sleepace
**************************************************************************************************/

/* ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
 *   �ض���
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
// int, long ƽ̨���
typedef unsigned int    mdUINT;
typedef unsigned long   mdULONG;

// Signed numbers
typedef signed char     mdCHAR;
typedef signed short    mdSHORT;
// int, long ƽ̨���
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
    MD_ENOERR,      // ����
    MD_EPARAM,      // �����쳣
    MD_EOVERFLOW,   // Խ��
    MD_ENOINIT,     // δ��ʼ��

    MD_EBUSY,       // æ
    MD_ETIMEOUT,    // ��ʱ

    MD_EUNKNOWN     // δ֪����
} eMDErrorCode;

typedef enum {
    MD_VER_TYPE_INVALID     = 0x00,     //��Ч
    MD_VER_TYPE_DEBUG       = 0x01,     //���԰�
    MD_VER_TYPE_BETA        = 0x02,     //���԰�
    MD_VER_TYPE_RELEASE     = 0x03,     //���а汾
} eMDVerType;

typedef enum {
    MD_IDE_TYPE_IAR         = 0x00,     //IAR
    MD_IDE_TYPE_KEIL        = 0x01,     //KEIL
    MD_IDE_TYPE_GCC         = 0x02,     //GCC
} eMDIDEType;

typedef enum {
    MD_MODEL_BLE_B502       = 0x0010,   //���
    // ...
} eMDModel;

// ��Χ 0x00-0x0F
typedef enum {
    MD_NET_MODEL_NOX        = 0x00,     //Nox
    MD_NET_MODEL_RESTON     = 0x01,     //WIFI��RestOn
    MD_NET_MODEL_MAT_ONE    = 0x02,     //WIFI�浥��
    MD_NET_MODEL_MAT_TWO    = 0x03,     //WIFI��˫��
} eMDNetModel;

#define BIN_MAGIC_WORD      (0xF57BA045930AE418ULL)
typedef struct xMD_BIN_INFO {
    mdINT64   ullMagic;     // 8�ֽ�ħ��(ָ����Ϣ,��ֹ��ȡ������Ϣ)
    mdBYTE    btIDEType;    // ��������
    mdUSHORT  usModel;      // �豸����
    mdBYTE    btVerType;    // �̼�����
    mdUSHORT  usVerCode;    // �̼��汾
} MD_BinInfo_t;

#endif

