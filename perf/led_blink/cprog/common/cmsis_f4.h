/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2015      by Roi Takeuchi
 *   
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 *
 * (3) When this software is redistributed in the forms unusable for the
 *     development of other software, such as the case when the software
 *     is embedded in a piece of equipment, either of the following two
 *     conditions must be satisfied:
 *
 * (a) The above copyright notice, this use conditions, and the
 *     disclaimer shown below must be shown without modification in
 *     the document provided with the redistributed software, such as
 *     the user manual.
 *
 * (b) How the software is to be redistributed must be reported to the
 *     TOPPERS Project according to the procedure described
 *     separately.
 *
 * (4) The above copyright holders and the TOPPERS Project are exempt
 *     from responsibility for any type of damage directly or indirectly
 *     caused from the use of this software and are indemnified by any
 *     users or end users of this software from any and all causes of
 *     action whatsoever.
 *
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  @(#) $Id: cmsis.h,v 1.1 2015/07/18 21:48:24 roi Exp $
 */

#ifndef _CMSIS_F4_H_
#define _CMSIS_F4_H_

#define __NVIC_PRIO_BITS          4       /* STM32F4XX uses 4 Bits for the Priority Levels */

/* Memory mapping of Cortex-M4 Hardware */
#define TADR_ITM_BASE       (0xE0000000UL)	/* ITM Base Address */
  #define TOFF_ITM_PORT     0x0000		/* (W)  ITM Stimulus Port Registers */
  #define TOFF_ITM_TER      0x0E00		/* (RW) ITM Trace Enable Register */
  #define TOFF_ITM_TPR      0x0E40		/* (RW) ITM Trace Privilege Register */
  #define TOFF_ITM_TCR      0x0E80		/* (RW) ITM Trace Control Register */
#define TADR_SCS_BASE       (0xE000E000UL)	/* System Control Space Base Address  */
  #define TOFF_SCS_ICTR     0x0004		/* (R)  Interrupt Controller Type Register */
  #define TOFF_SCS_ACTLR    0x0008		/* (RW) Auxiliary Control Register */
#define TADR_COREDEBUG_BASE (0xE000EDF0UL)				/* Core Debug Base Address */
#define TADR_SYSTICK_BASE   (TADR_SCS_BASE+0x0010UL)	/* SysTick Base Address */
  #define TOFF_SYSTICK_CTRL 0x0000		/* (RW) SysTick Control and Status Register */
  #define TOFF_SYSTICK_LOAD 0x0004		/* (RW) SysTick Reload Value Register */
  #define TOFF_SYSTICK_VAL  0x0008		/* (RW) SysTick Current Value Register */
  #define TOFF_SYSTICK_CALIB 0x000C		/* (R)  SysTick Calibration Register */
#define TADR_NVIC_BASE      (TADR_SCS_BASE+0x0100UL)	/* NVIC Base Address */
  #define TOFF_NVIC_ISER    0x0000		/* (RW) Interrupt Set Enable Register */
  #define TOFF_NVIC_ICER    0x0080		/* (RW) Interrupt Clear Enable Register */
  #define TOFF_NVIC_ISPR    0x0100		/* (RW) Interrupt Set Pending Register */
  #define TOFF_NVIC_ICPR    0x0180		/* (RW) Interrupt Clear Pending Register */
  #define TOFF_NVIC_IABR    0x0200		/* (RW) Interrupt Active bit Register */
  #define TOFF_NVIC_IP      0x0300		/* (RW) Interrupt Priority Register (8Bit wide) */
  #define TOFF_NVIC_STIR    0x0E00		/* (W)  Software Trigger Interrupt Register */
#define TADR_SCB_BASE       (TADR_SCS_BASE+0x0D00UL)	/* System Control Block Base Address  */
  #define TOFF_SCB_CPUID    0x0000		/* (R)  CPUID Base Register */
  #define TOFF_SCB_ICSR     0x0004		/* (RW) Interrupt Control and State Register */
  #define TOFF_SCB_VTOR     0x0008		/* (RW) Vector Table Offset Register */
  #define TOFF_SCB_AIRCR    0x000C		/* (RW) Application Interrupt and Reset Control Register */
  #define TOFF_SCB_SCR      0x0010		/* (RW) System Control Register */
  #define TOFF_SCB_CCR      0x0014		/* (RW) Configuration Control Register */
  #define TOFF_SCB_SHP0     0x0018		/* (RW-8) System Handlers Priority Registers 0 */
  #define TOFF_SCB_SHP1     0x0019		/* (RW-8) System Handlers Priority Registers 1 */
  #define TOFF_SCB_SHP2     0x001A		/* (RW-8) System Handlers Priority Registers 2 */
  #define TOFF_SCB_SHP3     0x001B		/* (RW-8) System Handlers Priority Registers 3 */
  #define TOFF_SCB_SHP4     0x001C		/* (RW-8) System Handlers Priority Registers 4 */
  #define TOFF_SCB_SHP5     0x001D		/* (RW-8) System Handlers Priority Registers 5 */
  #define TOFF_SCB_SHP6     0x001E		/* (RW-8) System Handlers Priority Registers 6 */
  #define TOFF_SCB_SHP7     0x001F		/* (RW-8) System Handlers Priority Registers 7 */
  #define TOFF_SCB_SHP8     0x0020		/* (RW-8) System Handlers Priority Registers 8 */
  #define TOFF_SCB_SHP9     0x0021		/* (RW-8) System Handlers Priority Registers 9 */
  #define TOFF_SCB_SHP10    0x0022		/* (RW-8) System Handlers Priority Registers 10 */
  #define TOFF_SCB_SHP11    0x0023		/* (RW-8) System Handlers Priority Registers 11 */
  #define TOFF_SCB_SHCSR    0x0024		/* (RW) System Handler Control and State Register */
  #define TOFF_SCB_CFSR     0x0028		/* (RW) Configurable Fault Status Register */
  #define TOFF_SCB_HFSR     0x002C		/* (RW) HardFault Status Register */
  #define TOFF_SCB_DFSR     0x0030		/* (RW) Debug Fault Status Register */
  #define TOFF_SCB_MMFAR    0x0034		/* (RW) MemManage Fault Address Register */
  #define TOFF_SCB_BFAR     0x0038		/* (RW) BusFault Address Register */
  #define TOFF_SCB_AFSR     0x003C		/* (RW) Auxiliary Fault Status Register */
  #define TOFF_SCB_PFR0     0x0040		/* (R)  Processor Feature Register 0 */
  #define TOFF_SCB_PFR1     0x0044		/* (R)  Processor Feature Register 1 */
  #define TOFF_SCB_DFR      0x0048		/* (R)  Debug Feature Register */
  #define TOFF_SCB_ADR      0x004C		/* (R)  Auxiliary Feature Register */
  #define TOFF_SCB_MMFR0    0x0050		/* (R)  Memory Model Feature Register 0 */
  #define TOFF_SCB_MMFR1    0x0054		/* (R)  Memory Model Feature Register 1 */
  #define TOFF_SCB_MMFR2    0x0058		/* (R)  Memory Model Feature Register 2 */
  #define TOFF_SCB_MMFR3    0x005C		/* (R)  Memory Model Feature Register 3 */
  #define TOFF_SCB_ISAR0    0x0060		/* (R)  Instruction Set Attributes Register 0 */
  #define TOFF_SCB_ISAR1    0x0064		/* (R)  Instruction Set Attributes Register 1 */
  #define TOFF_SCB_ISAR2    0x0068		/* (R)  Instruction Set Attributes Register 2 */
  #define TOFF_SCB_ISAR3    0x006C		/* (R)  Instruction Set Attributes Register 3 */
  #define TOFF_SCB_ISAR4    0x0070		/* (R)  Instruction Set Attributes Register 4 */
  #define TOFF_SCB_CPACR    0x0088		/* (RW) Coprocessor Access Control Register */

#define __ASM            __asm          /* asm keyword for GNU Compiler          */
#define __INLINE         inline         /* inline keyword for GNU Compiler       */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  Enable IRQ Interrupts
 *  This function enables IRQ interrupts by clearing the I-bit in the CPSR.
 *  Can only be executed in Privileged modes.
 */
__attribute__( ( always_inline ) ) static __INLINE void __enable_irq(void)
{
  __ASM volatile ("cpsie i");
}


/*
 *  Disable IRQ Interrupts
 *  This function disables IRQ interrupts by setting the I-bit in the CPSR.
 *  Can only be executed in Privileged modes.
 */
__attribute__( ( always_inline ) ) static __INLINE void __disable_irq(void)
{
  __ASM volatile ("cpsid i");
}

#endif

#endif  /* _CMSIS_F4_H_ */

