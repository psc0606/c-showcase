/**
 * @file ttypes.h
 *
 * @brief Types definition for Minsdpeed Transcede devices
 *
 */

#ifndef __TTYPES_H__
#define __TTYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TRUE
#define TRUE    1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE   0
#endif /*FALSE*/

#ifndef NULL
#define NULL   (void*)0
#endif /*NULL*/

#define _ASSERT(x)
#define _ASSERT_RC(rc) if(rc) _ASSERT(0)

// Generic System Error Codes (0x00..0xFF)
#define SYSRC_SUCCESS                       0x00000000
#define SYSRC_MALLOC_ERROR                  0x00000006
#define SYSRC_INVALID_PARAM                 0x0000000E
#define SYSRC_UNKNOWN_INST                  0x0000000C
//MAP Error Codes (0x500..0x5FF)
#define SYSRC_MAP_WRONG_CONF_PRM            0x00000500
/************************************************************************/
/*     UINT64, UINT32, UINT16 and UINT8 definition                      */
/************************************************************************/
#ifndef _UINT64_
#define _UINT64_
typedef unsigned long long UINT64, *PUINT64;
#endif /*_UINT64_*/

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int UINT32, *PUINT32;
#endif /*_UINT32_*/

#ifndef _UINT16_
#define _UINT16_
typedef unsigned short UINT16, *PUINT16;
#endif /*_UINT16_*/

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char UINT8, *PUINT8;
#endif /*_UINT8_*/

#ifndef _VUINT64_
#define _VUINT64_
typedef volatile unsigned long long VUINT64, *PVUINT64;
#endif /*_UINT64_*/

#ifndef _VUINT32_
#define _VUINT32_
typedef volatile unsigned int VUINT32, *PVUINT32;
#endif /*_UINT32_*/

#ifndef _VUINT16_
#define _VUINT16_
typedef volatile unsigned short VUINT16, *PVUINT16;
#endif /*_UINT16_*/

#ifndef _VUINT8_
#define _VUINT8_
typedef volatile unsigned char VUINT8, *PVUINT8;
#endif /*_UINT8_*/

/************************************************************************/
/*     SINT64, SINT32, SINT16 and SINT8 definition                      */
/************************************************************************/
#ifndef _SINT64_
#define _SINT64_
typedef signed long long SINT64, *PSINT64;
#endif /*_SINT64_*/

#ifndef _SINT32_
#define _SINT32_
typedef int SINT32, *PSINT32;
#endif /*_SINT32_*/

#ifndef _SINT16_
#define _SINT16_
typedef short SINT16, *PSINT16;
#endif /*_SINT16_*/

#ifndef _SINT8_
#define _SINT8_
typedef char SINT8, *PSINT8;
#endif /*_SINT8_*/

#ifndef _PVOID_
#define _PVOID_
typedef void *PVOID;
#endif /*_PVOID_*/

#ifndef _BOOL_
#define _BOOL_
typedef UINT8 BOOL;
#endif /*_BOOL_*/

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

#define CONV_ENDIAN_32(v) ((v & 0xff) << 24 | (v >> 24) | ((v & 0xff00) << 8) | ((v & 0xff0000) >> 8));


#ifndef MAX
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#endif

#ifdef __cplusplus
}
#endif

typedef void (*VOIDCB)(void);

typedef void (*CALLBACK)(PVOID);

#define BCTRL_LEN_MASK      0x0000FFFF
#define BCTRL_BLAST_MASK    0x00010000

#define DMAFCTRL_IRQEN      0x00000001
#define DMAFCTRL_INBOFF     0x0000FFF0

#define ARRAY_COUNT(v)      (sizeof(v)/sizeof(v[0]))

#define MAX_FPDMA_BDESC     2

#if !defined(__ASM_ARCH_TRANSCEDE_2200_H) && !defined(_DMABDESC_)
#define _DMABDESC_
// DMABDESC is defined in transcede-2200.h, if it is not included we need to define it.
typedef struct tDMABDESC
{
    volatile unsigned int       BPtr;
    volatile unsigned int       BCtrl;
} DMABDESC, *PDMABDESC;

#endif

typedef struct tMAPDMADESC
{
    volatile struct  tMAPDMADESC  *NextFDesc;
    volatile unsigned int       FControl;
    volatile unsigned int       FStatus0;
    volatile unsigned int       FStatus1;
    volatile DMABDESC   InOutBDesc[MAX_FPDMA_BDESC];
} MAPDMADESC, *PMAPDMADESC;

#endif /*__SYSTYPES_H__ */

