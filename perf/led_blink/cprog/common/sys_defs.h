
#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

/*	Values	*/

#if BOARDNO == 1
#define TARGET_CPU_STR		"STM32-E407"
#elif BOARDNO == 0
#define TARGET_CPU_STR		"STM32F4-Discovery"
#elif BOARDNO == 2
#define TARGET_CPU_STR		"STM32F429-board"
#elif BOARDNO == 3
#define TARGET_CPU_STR		"STM32F401 Nucleo"
#elif BOARDNO == 4
#define TARGET_CPU_STR		"STM32F446 Nucleo-64"
#elif BOARDNO == 5
#define TARGET_CPU_STR		"STM32F446 Nucleo-144"
#else
#error	"NOT SOPPORT BOARD !"
#endif

/* endian */
#define SIL_ENDIAN      0		/* LITTLE ENDIAN */

/*	Address		*/
#define RAM_BASE        0x20000000
#define RAM_SIZE        (128*1024)
#define VECTOR_SIZE		(4*8)
#define FIQ_DATA_SIZE   256
#define STACKINT        (RAM_BASE+RAM_SIZE-FIQ_DATA_SIZE)	/* Interrupt Stack Initiali Value */
#define STACKTOP        (STACKINT-256)						/* User Stack Initiali Value */

/*
 *  Definiton for Micro Wait
 */
#if BOARDNO == 3
#define SIL_DLY_TIM1    100
#define SIL_DLY_TIM2    102
#elif BOARDNO == 4 || BOARDNO == 5
#define SIL_DLY_TIM1    50
#if ROM_EXEC == 1
#define SIL_DLY_TIM2    74
#else
#define SIL_DLY_TIM2    30
#endif
#else
#define SIL_DLY_TIM1    200
#define SIL_DLY_TIM2    24
#endif

/*	Default Values	*/

#define MAX_INT_NUM     82
#define DEF_DUMP_LENGTH	256
#define NOT_INITDECT
#define RESET_VECTOR    1
#if BOARDNO == 1
#define SYSTEMCORECLOCK 120000000
#define PCLK1           30000000
#elif BOARDNO == 0
#define SYSTEMCORECLOCK 168000000
#define PCLK1           42000000
#elif BOARDNO == 3
#define SYSTEMCORECLOCK 84000000
#define PCLK1           42000000
#else
#define SYSTEMCORECLOCK 180000000
#define PCLK1           45000000
#endif

#ifndef TOPPERS_MACRO_ONLY
/*
 *  Compiler Dependent Data Type
 */
#define	_int8_		char		/* 8 bit Integer	*/
#define	_int16_		short		/* 16 bit Integer	*/
#define	_int32_		int			/* 32 bit Integer	*/

typedef struct {
	unsigned int SYSCLK_Frequency;	/*  SYSCLK clock frequency expressed in Hz */
	unsigned int HCLK_Frequency;	/*  HCLK clock frequency expressed in Hz */
	unsigned int PCLK1_Frequency;	/*  PCLK1 clock frequency expressed in Hz */
	unsigned int PCLK2_Frequency;	/*  PCLK2 clock frequency expressed in Hz */
}RCC_ClocksTypeDef;

/*
 *  Macro Definition for Compiler Extended Function
 */
#define	Inline          static inline

extern void sil_dly_nse(unsigned int);

#endif	/* TOPPERS_MACRO_ONLY */

/*	Serial Channels		*/

#define MAX_SERIAL		1

#include "stm32f4xx.h"

/* serial definitions */

#define DEFAULT_SPEED		115200

#ifndef TOPPERS_MACRO_ONLY

#if DEBUG == 0
#define	sys_puthex(val)
#define sys_printf(s)
#else
extern void sys_puthex(unsigned int val);
extern void sys_printf(const char *s);
#endif

#endif	/* TOPPERS_MACRO_ONLY */

#endif	/* SYS_DEFS_H_ */

