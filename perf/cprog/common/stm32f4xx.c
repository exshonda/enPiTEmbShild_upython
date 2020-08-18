/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: $
 */
/*******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS. 
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************/

/*
 *	rommon Serial Functions
 */
#include "sys_defs.h"
#include <string.h>
#include "sil.h"

#define VECT_TAB_OFFSET     0x00		/* Vector Table base offset field. 
                                		   This value must be a multiple of 0x200. */
#define HSE_STARTUP_TIMEOUT 0x0500		/* Time out for HSE start up */
#define HSI_VALUE           16000000	/* Value of the Internal oscillator in Hz*/
#if BOARDNO == 2	/* STM32F409 baord */
#define HSE_VALUE           25000000	/* Value of the External oscillator in Hz */

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      (HSE_VALUE / 1000000)/* Possible value 0 and 63 */
#define PLL_N      360

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

/* I2SCLK =  PLLVCO / PLLR */                          
#define PLL_R      0	/* Possible value between 2 and 7 */

#elif BOARDNO == 1	/* STM32-E407 */
#define HSE_VALUE           12000000	/* Value of the External oscillator in Hz */
/* PLLVCO = (HSE_VALUE / PLL_M) * PLL_N */
#define PLL_M      (HSE_VALUE / 1000000)/* Possible value 0 and 63 */
#define PLL_N      240	/* Possible value 192 and 432 */

/* SYSCLK = PLLVCO / PLL_P !!!! DO NOT EXCEED 120MHz */
#define PLL_P      2	/* Possible value 2, 4, 6, or 8 */

/* OTGFS, SDIO and RNG Clock =  PLLVCO / PLLQ */
#define PLL_Q      5	/* Possible value between 4 and 15 */

/* I2SCLK =  PLLVCO / PLLR */
#define PLL_R      2	/* Possible value between 2 and 7 */

#else				/* STM32F4-Discovery */
#define HSE_VALUE           8000000	/* Value of the External oscillator in Hz */

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#if BOARDNO == 3 || BOARDNO == 4
#define RCC_PLLCFGR_PLLSRC_VAL RCC_PLLCFGR_PLLSRC_HSI
#define PLL_M      (HSI_VALUE / 1000000)/* Possible value 0 and 63 */
#else
#define RCC_PLLCFGR_PLLSRC_VAL RCC_PLLCFGR_PLLSRC_HSE
#define PLL_M      (HSE_VALUE / 1000000)/* Possible value 0 and 63 */
#endif
#if BOARDNO == 4 || BOARDNO == 5
#define PLL_N      360
#else
#define PLL_N      336
#endif

/* SYSCLK = PLL_VCO / PLL_P */
#if BOARDNO == 3
#define PLL_P      4
#else
#define PLL_P      2
#endif

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

/* I2SCLK =  PLLVCO / PLLR */
#if BOARDNO == 4
#define PLL_R      6	/* Possible value between 2 and 7 */
#elif BOARDNO == 5
#define PLL_R      2	/* Possible value between 2 and 7 */
#else
#define PLL_R      0	/* Possible value between 2 and 7 */
#endif

/* PLLI2S_VCO = (HSE_VALUE Or HSI_VALUE / PLL_M) * PLLI2S_N
   I2SCLK = PLLI2S_VCO / PLLI2S_R */
#define PLLI2S_N   192
#define PLLI2S_R   5
#endif

#if BOARDNO == 3 || BOARDNO == 4 || BOARDNO == 5
#define PLL_TIMEOUT_VALUE          1000  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  50000 /* 5 s    */
#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000

#if BOARDNO == 4 || BOARDNO == 5
#define PWR_REGULATOR_VOLTAGE_SCALE PWR_CR_VOS		/* SCALE1 */
#define FLASH_LATENCY              FLASH_ACR_LATENCY_5WS	/* FLASH Five Latency cycles     */
#else
#define PWR_REGULATOR_VOLTAGE_SCALE PWR_CR_VOS_1	/* SCALE2 */
#define FLASH_LATENCY              FLASH_ACR_LATENCY_2WS	/* FLASH Two Latency cycles      */
#endif


/*
 *  RCC_BitAddress_AliasRegion RCC BitAddress AliasRegion
 *  RCC registers bit address in the alias region
 */
#define RCC_OFFSET                 (TADR_RCC_BASE - PERIPH_BASE)
/* --- CR Register ---*/
/* Alias word address of HSION bit */
#define RCC_CR_OFFSET              (RCC_OFFSET + 0x00)
#define RCC_HSION_BIT_NUMBER       0x00
#define RCC_CR_HSION_BB            (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_HSION_BIT_NUMBER * 4))
/* Alias word address of PLLON bit */
#define RCC_PLLON_BIT_NUMBER       0x18
#define RCC_CR_PLLON_BB            (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLON_BIT_NUMBER * 4))

#define RCC_PLLSAION_BIT_NUMBER    0x1C
#define RCC_CR_PLLSAION_BB         (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLSAION_BIT_NUMBER * 4))

/*
 *  ACR register byte 0 (Bits[7:0]) base address  
 */
#define ACR_BYTE0_ADDRESS           (0x40023C00)
/*
 *  CR register byte 3 (Bits[23:16]) base address
 */
#define RCC_CR_BYTE2_ADDRESS        (0x40023802)


#define PWR_OFFSET               (TADR_PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00
#define PWR_CSR_OFFSET           0x04
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)

#define ODEN_BIT_NUMBER          16
#define CR_ODEN_BB               (unsigned int)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32) + (ODEN_BIT_NUMBER * 4))

#define ODSWEN_BIT_NUMBER        17
#define CR_ODSWEN_BB             (unsigned int)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32) + (ODSWEN_BIT_NUMBER * 4))
#endif

#define RCC_CFGR_SW        (RCC_CFGR_SW_0 | RCC_CFGR_SW_1)
#define RCC_CFGR_SWS       (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1)
#define RCC_CFGR_HPRE      (RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3)
#define RCC_CFGR_PPRE1     (RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_1 | RCC_CFGR_PPRE1_2)
#define RCC_CFGR_PPRE2     (RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE2_1 | RCC_CFGR_PPRE2_2)
#define RCC_PLLCFGR_PLLM   (RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLM_1 | RCC_PLLCFGR_PLLM_2 \
                           | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_5)
#define RCC_PLLCFGR_PLLN   (RCC_PLLCFGR_PLLN_0 | RCC_PLLCFGR_PLLN_1 | RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_3 \
                           | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_8)
#define RCC_PLLCFGR_PLLP   (RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1)
#define RCC_PLLCFGR_PLLSRC (RCC_PLLCFGR_PLLSRC_HSE)
#define RCC_CR_HSITRIM     (RCC_CR_HSITRIM_0 | RCC_CR_HSITRIM_1 | RCC_CR_HSITRIM_2 | RCC_CR_HSITRIM_3 | RCC_CR_HSITRIM_4)
#define FLASH_ACR_LATENCY  (FLASH_ACR_LATENCY_0WS | FLASH_ACR_LATENCY_1WS | FLASH_ACR_LATENCY_2WS | FLASH_ACR_LATENCY_3WS \
                           | FLASH_ACR_LATENCY_4WS | FLASH_ACR_LATENCY_5WS | FLASH_ACR_LATENCY_6WS | FLASH_ACR_LATENCY_7WS)

#define RCC_HSE_BYPASS           0x05


/*
 *  GPIO PIN POSITION
 */
#if BOARDNO == 1	/* STM32-E407 */
#define TADR_U1_GPIO_BASE  TADR_GPIOB_BASE
#define TADR_U2_GPIO_BASE  TADR_GPIOD_BASE
#define U1_USART           RCC_APB1ENR_USART3EN
#define U1_PORT            RCC_AHB1ENR_GPIOBEN
#define U2_PORT            RCC_AHB1ENR_GPIODEN
#define TX1_PINPOS         10
#define RX1_PINPOS         11
#define TX2_PINPOS         5
#define RX2_PINPOS         6
#define GPIO_Speed         GPIO_Speed_50MHz
#elif BOARDNO == 2	/* STM32F429 */
#define TADR_U1_GPIO_BASE  TADR_GPIOA_BASE
#define U1_USART           RCC_APB2ENR_USART1EN
#define U1_PORT            RCC_AHB1ENR_GPIOAEN
#define TX1_PINPOS         9
#define RX1_PINPOS         10
#define GPIO_Speed         GPIO_Speed_50MHz
#elif BOARDNO == 5	/* STM32F446Z */
#define TADR_U1_GPIO_BASE  TADR_GPIOD_BASE
#define U1_USART           RCC_APB1ENR_USART3EN
#define U1_PORT            RCC_AHB1ENR_GPIODEN
#define TX1_PINPOS         8
#define RX1_PINPOS         9
#define GPIO_Speed         GPIO_Speed_100MHz
#else				/* STM32F4-Discovery STM32F401 */
#define TADR_U1_GPIO_BASE  TADR_GPIOA_BASE
#define U1_USART           RCC_APB1ENR_USART2EN
#define U1_PORT            RCC_AHB1ENR_GPIOAEN
#define TX1_PINPOS         2
#define RX1_PINPOS         3
#define GPIO_Speed         GPIO_Speed_50MHz
#endif

/*
 *  USART Priority
 */
#define MAIN_PRIORITY      0
#define SUB_PRIORITY       0

/*
 *  GPIO Configuration Mode enumeration
 */
#define GPIO_Mode_IN    0x0					/* GPIO Input Mode */
#define GPIO_Mode_OUT   GPIO_MODER_MODER0	/* GPIO Output Mode */
#define GPIO_Mode_AF    GPIO_MODER_MODER1	/* GPIO Alternate function Mode */
#define GPIO_Mode_AN    GPIO_MODER_MODER2	/* GPIO Analog Mode */

/*
 *  GPIO Output type enumeration
 */
#define GPIO_OType_PP   0x0
#define GPIO_OType_OD   0x1

/*
 *  GPIO Output Maximum frequency enumeration
 */
#define GPIO_Speed_2MHz   0x0						/* Low speed */
#define GPIO_Speed_25MHz  GPIO_OSPEEDER_OSPEEDR0	/* Medium speed */
#define GPIO_Speed_50MHz  GPIO_OSPEEDER_OSPEEDR1	/* Fast speed */
#define GPIO_Speed_100MHz GPIO_OSPEEDER_OSPEEDR2	/* High speed on 30 pF (80 MHz Output max speed on 15 pF) */

/*
 *  GPIO Configuration PullUp PullDown enumeration
 */
#define GPIO_PuPd_NOPULL 0x0
#define GPIO_PuPd_UP     GPIO_PUPDR_PUPDR0
#define GPIO_PuPd_DOWN   GPIO_PUPDR_PUPDR1

/*
 *  AF 7 selection
 */
#define GPIO_AF7                0x07	/* AF7 value */

/*
 *  USART Word Length
 */
#define USART_WordLength_8b     0x0000
#define USART_WordLength_9b     USART_CR1_M

/*
 *  USART Stop Bits
 */ 
#define USART_StopBits_1        0x0000
#define USART_StopBits_0_5      USART_CR2_STOP_0
#define USART_StopBits_2        USART_CR2_STOP_1
#define USART_StopBits_1_5      USART_CR2_STOP

/*
 *  USART Parity
 */
#define USART_Parity_No         0x0000
#define USART_Parity_Even       USART_CR1_PCE
#define USART_Parity_Odd        (USART_CR1_PCE | USART_CR1_PS) 

/*
 *  USART MODE
 */
#define USART_Mode_Rx           USART_CR1_RE
#define USART_Mode_Tx           USART_CR1_TE

/** @defgroup USART_Hardware_Flow_Control 
  * @{
  */ 
#define USART_HardwareFlowControl_None       0x0000
#define USART_HardwareFlowControl_RTS        USART_CR3_RTSE
#define USART_HardwareFlowControl_CTS        USART_CR3_CTSE
#define USART_HardwareFlowControl_RTS_CTS    (USART_CR3_RTSE | USART_CR3_CTSE)

#define CR1_CLEAR_MASK          (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE)
#define CR3_CLEAR_MASK          (USART_CR3_RTSE | USART_CR3_CTSE)


typedef struct usart_struct {
	unsigned int    taddr;
	unsigned int	no;
} usart_struct_t;

static usart_struct_t usart_table[MAX_SERIAL] = {
#if BOARDNO == 1	/* STM32-E407 */
	{	TADR_USART3_BASE, INTNO_USART3 },
#elif BOARDNO == 2	/* STM32F429 */
	{	TADR_USART1_BASE, INTNO_USART1 }
#elif BOARDNO == 5	/* STM32F446Z */
	{	TADR_USART3_BASE, INTNO_USART3 }
#else				/* STM32F4-Discovery STM32F401 */
	{	TADR_USART2_BASE, INTNO_USART2 }
#endif
};

extern unsigned int IsrVector[];

/*
 *  Modified 16bit Port
 */
static void sil_mdh_mem(VP addr, VH mask, VH val)
{
	VH tmpreg  = sil_reh_mem(addr) & ~mask;
	sil_wrh_mem(addr, tmpreg | val);
}

#if ROM_EXEC == 1
/*
 *  Modified 32bit Port
 */
static void sil_mdw_mem(VP addr, VW mask, VW val)
{
	VW tmpreg  = sil_rew_mem(addr) & ~mask;
	sil_wrw_mem(addr, tmpreg | val);
}

static void setup_gpio_source(VP base, unsigned char source, unsigned char select)
{
	VW tmpreg, regoff;
	regoff = TOFF_GPIO_AFR0+((source>>1) & 0x4);
	tmpreg = (sil_rew_mem((VP)(base+regoff)) & ~(0xF << ((source & 0x07) * 4)))
			    | (select << ((source & 0x07) * 4));
	sil_wrw_mem((VP)(base+regoff), tmpreg);
}

/*
 * Configures the System clock source, PLL Multiplier and Divider factors,
 * AHB/APBx prescalers and Flash settings
 * @Note   This function should be called only once the RCC clock configuration
 *         is reset to the default reset state (done in SystemInit() function).
 * @param  None
 * @retval None
 */
static void SetSysClock(void)
{
#if BOARDNO < 3
	unsigned int StartUpCounter = 0, HSEStatus = 0;

	/* Enable HSE */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), 0, RCC_CR_HSEON);

	/* Wait till HSE is ready and if Time out is reached exit */
	do{
		HSEStatus = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY;
		StartUpCounter++;
	}while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY) != 0){
#if BOARDNO == 2	/* STM32F409 board */
		/* Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz */
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), 0, RCC_APB1ENR_PWREN);
		sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), 0, PWR_CR_VOS);
#elif BOARDNO == 0	/* STM32F4-Discovry */
		/* Enable high performance mode, System frequency up to 168 MHz */
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), 0, RCC_APB1ENR_PWREN);
		sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), 0, PWR_CR_PMODE);
#endif

		/* HCLK = SYSCLK / 1*/
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0, RCC_CFGR_HPRE_DIV1);

		/* PCLK2 = HCLK / 2*/
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0, RCC_CFGR_PPRE2_DIV2);

		/* PCLK1 = HCLK / 4*/
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0, RCC_CFGR_PPRE1_DIV4);

		/* Configure the main PLL */
		sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24) | (PLL_R << 28));

 		/* Enable the main PLL */
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), 0, RCC_CR_PLLON);

		/* Wait till the main PLL is ready */
		while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0)
		{
		}

#if BOARDNO == 2	/* STM32F409 board */
		/* Enable the Over-drive to extend the clock frequency to 180 Mhz */
		sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), 0, PWR_CR_ODEN);
		while((sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODRDY) == 0)
		{
		}
		sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), 0, PWR_CR_ODSWEN);
		while((sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODSWRDY) == 0)
		{
		}

		/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
		sil_wrw_mem((VP)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS);
#elif BOARDNO == 1	/* STM32-E407 */
 		/*  Enable Flash prefetch, Instruction cache and Data cache for max performance
		    and set Flash 3 wait state */
		sil_wrw_mem((VP)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS);
#else				/* STM32F4-Discovry */
		/* Configure Flash prefetch, Instruction cache, Data cache and wait state */
		sil_wrw_mem((VP)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_ICEN | FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS);
#endif

		/* Select the main PLL as system clock source */
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_SW, RCC_CFGR_SW_PLL);

		/* Wait till the main PLL is used as system clock source */
		while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		{
		}
	}
	else{
		/* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
	}

#if BOARDNO == 0	/* STM32F4-Discovry */
	/* PLLI2S clock used as I2S clock source */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_I2SSRC, 0);

	/* Configure PLLI2S */
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLI2SCFGR), (PLLI2S_N << 6) | (PLLI2S_R << 28));

	/* Enable PLLI2S */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), 0, RCC_CR_PLLI2SON);

	/* Wait till PLLI2S is ready */
	while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLI2SRDY) == 0)
	{
	}
#endif
#else
	unsigned int tickstart = 0;
	unsigned int tmp;

  /* Enable Power Control clock */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), 0, RCC_APB1ENR_PWREN);
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR));

  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), PWR_CR_VOS, PWR_REGULATOR_VOLTAGE_SCALE);
	tmp = sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR));
  
#if BOARDNO == 5
	/* Set the new HSE configuration ---------------------------------------*/
	sil_wrb_mem((VP)RCC_CR_BYTE2_ADDRESS, RCC_HSE_BYPASS);

	/* Wait till HSE is ready */  
	while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSERDY) == 0){
		if(tickstart > HSE_STARTUP_TIMEOUT){
			return;
		}
		tickstart++;
		sil_dly_nse(1000*100);
	}
#else
	/* Enable HSI Oscillator and activate PLL with HSI as source */
	/*----------------------------- HSI Configuration --------------------------*/
	/* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS;
	if((tmp == RCC_CFGR_SWS_HSI)  ||
      ((tmp == RCC_CFGR_SWS_PLL) && ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))
	{
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSITRIM, (0x10 << 3));
	}
#endif
	/*-------------------------------- PLL Configuration -----------------------*/
	/* Check the parameters */
    /* Check if the PLL is used as system clock or not */
	if((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
        /* Disable the main PLL. */
		sil_wrw_mem((VP)(RCC_CR_PLLON_BB), 0);

        /* Get Start Tick*/
        tickstart = 0;

        /* Wait till PLL is ready */  
        while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if(tickstart > PLL_TIMEOUT_VALUE){
				return;
			}
        }

        /* Configure the main PLL clock source, multiplication and division factors. */
		sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_VAL) | (PLL_Q << 24) | (PLL_R << 28));
        /* Enable the main PLL. */
		sil_wrw_mem((VP)(RCC_CR_PLLON_BB), 1);

        /* Get Start Tick*/
		tickstart = 0;

        /* Wait till PLL is ready */  
        while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if( tickstart  > PLL_TIMEOUT_VALUE){
				return ;
			}
		}
	}

#if BOARDNO == 4 || BOARDNO == 5
	/* Enable the Over-drive to extend the clock frequency to 180 Mhz */
	sil_wrw_mem((VP)(CR_ODEN_BB), 1);

	tickstart = 0;
	while((sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODRDY) == 0){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart  > PWR_OVERDRIVE_TIMEOUT_VALUE){
			return;
		}
	}

	/* Enable the Over-drive switch */
	sil_wrw_mem((VP)(CR_ODSWEN_BB), 1);

	tickstart = 0;
	while((sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CSR)) & PWR_CSR_ODSWRDY) == 0){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart > PWR_OVERDRIVE_TIMEOUT_VALUE){
			return ;
		}
	}
#if BOARDNO == 5
	USB_RCC_PeriphCLKConfig(8, 384, 8);
#endif
#endif

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
	/* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
	sil_wrb_mem((VP)ACR_BYTE0_ADDRESS, FLASH_LATENCY);

	/* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((sil_rew_mem((VP)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLASH_LATENCY){
      return;
    }

    /*-------------------------- HCLK Configuration --------------------------*/
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    /*------------------------- SYSCLK Configuration ---------------------------*/ 
	/* PLL is selected as System Clock Source */
	/* Check the PLL ready flag */  
	if((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
		return;
	}

	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_SW, RCC_CFGR_SW_PLL);

	/* Get Start Tick*/
	tickstart = 0;

	while ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart > CLOCKSWITCH_TIMEOUT_VALUE){
            return;
		}
	}

	/*-------------------------- PCLK1 Configuration ---------------------------*/
#if BOARDNO == 4 || BOARDNO == 5
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
#else
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
#endif

	/*-------------------------- PCLK2 Configuration ---------------------------*/
#if BOARDNO == 5
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE2, (RCC_CFGR_PPRE1_DIV2 << 3));
#else
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE2, (RCC_CFGR_PPRE1_DIV1 << 3));
#endif
#endif
}
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const unsigned char APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/*
 * Returns the frequencies of different on chip clocks; SYSCLK, HCLK, 
 * PCLK1 and PCLK2.
 * @note   The system frequency computed by this function is not the real 
 *         frequency in the chip. It is calculated based on the predefined 
 *         constant and the selected clock source:
 * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
 * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
 * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(**) 
 *           or HSI_VALUE(*) multiplied/divided by the PLL factors.         
 * @note     (*) HSI_VALUE is a constant defined in stm32f4xx.h file (default value
 *               16 MHz) but the real value may vary depending on the variations
 *               in voltage and temperature.
 * @note     (**) HSE_VALUE is a constant defined in stm32f4xx.h file (default value
 *                25 MHz), user has to ensure that HSE_VALUE is same as the real
 *                frequency of the crystal used. Otherwise, this function may
 *                have wrong result.
 *
 * @note   The result of this function could be not correct when using fractional
 *         value for HSE crystal.
 *
 * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
 *         the clocks frequencies.
 *
 * @note   This function can be used by the user application to compute the 
 *         baudrate for the communication peripherals or configure other parameters.
 * @note   Each time SYSCLK, HCLK, PCLK1 and/or PCLK2 clock changes, this function
 *         must be called to update the structure's field. Otherwise, any
 *         configuration based on this function will be incorrect.
 *
 * @retval None
 */
static void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
	unsigned int tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

	/* Get SYSCLK source -------------------------------------------------------*/
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS;

	switch (tmp){
	case 0x00:  /* HSI used as system clock source */
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	case 0x04:  /* HSE used as system clock  source */
		RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
		break;
	case 0x08:  /* PLL used as system clock  source */

		/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
		   SYSCLK = PLL_VCO / PLLP
		 */
		pllsource = (sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLSRC) >> 22;
		pllm = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLM;

		if (pllsource != 0)	/* HSE used as PLL clock source */
			pllvco = (HSE_VALUE / pllm) * ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLN) >> 6);
		else				/* HSI used as PLL clock source */
			pllvco = (HSI_VALUE / pllm) * ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLN) >> 6);

		pllp = (((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
		RCC_Clocks->SYSCLK_Frequency = pllvco/pllp;
		break;
	default:
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	}
	/* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/

	/* Get HCLK prescaler */
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_HPRE;
	tmp = tmp >> 4;
	presc = APBAHBPrescTable[tmp];
	/* HCLK clock frequency */
	RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

	/* Get PCLK1 prescaler */
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_PPRE1;
	tmp = tmp >> 10;
	presc = APBAHBPrescTable[tmp];
	/* PCLK1 clock frequency */
	RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

	/* Get PCLK2 prescaler */
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_PPRE2;
	tmp = tmp >> 13;
	presc = APBAHBPrescTable[tmp];
	/* PCLK2 clock frequency */
	RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
}

/*
 * Setup the microcontroller system
 * Initialize the Embedded Flash Interface, the PLL and update the SystemFrequency variable.
 * @param  None
 * @retval None
 */
void hardware_init_hook(void)
{
	unsigned int  usart_base = usart_table[0].taddr;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	unsigned int  tmpreg = 0x00, apbclock = 0x00;
	unsigned int  integerdivider = 0x00;
	unsigned int  fractionaldivider = 0x00;

#if ROM_EXEC == 1
	/* Reset the RCC clock configuration to the default reset state ------------*/
	/* Set HSION bit */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), 0, RCC_CR_HSION);

	/* Reset CFGR register */
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0x00000000);

	/* Reset HSEON, CSSON and PLLON bits */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON, 0);

	/* Reset PLLCFGR register */
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO1PRE_2
					 | RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_HPRE_0);

	/* Reset HSEBYP bit */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSEBYP, 0);

	/* Disable all interrupts */
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CIR), 0x00000000);

	/* SystemInit_ExtMemCtl(); no ExtMem */

	/* Configure the System clock source, PLL Multiplier and Divider factors, 
       AHB/APBx prescalers and Flash settings ----------------------------------*/
	SetSysClock();
#endif

	/* Configure the Vector Table location add offset address ------------------*/
	sil_wrw_mem((VP)(TADR_SCB_BASE+TOFF_SCB_VTOR), (VW)IsrVector);	/* Vector Table Relocation in Internal SRAM */

#if ROM_EXEC == 1
	/* enable APB2 peripheral clock for USART1
	 * note that only USART1 and USART6 are connected to APB2
	 * the other USARTs are connected to APB1
	 */
#if U1_USART == RCC_APB2ENR_USART1EN
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB2ENR), 0, U1_USART);
#else
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), 0, U1_USART);
#endif
	/* enable the peripheral clock for the pins used by USART1, PB6 for TX and PB7 for RX */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_AHB1ENR), 0, U1_PORT);

	/* This sequence sets up the TX pin so they work correctly with the USART2 peripheral */
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (TX1_PINPOS*2)), (GPIO_Mode_AF << (TX1_PINPOS*2)));

	/* Speed mode configuration */
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (TX1_PINPOS*2)), (GPIO_Speed << (TX1_PINPOS*2)));

	/* Output mode configuration*/
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << TX1_PINPOS), (GPIO_OType_PP << TX1_PINPOS));

	/* Pull-up Pull down resistor configuration */
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (TX1_PINPOS*2)), (GPIO_PuPd_UP << (TX1_PINPOS*2)));

	/* This sequence sets up the RX pin so they work correctly with the USART2 peripheral */
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (RX1_PINPOS*2)), (GPIO_Mode_AF << (RX1_PINPOS*2)));

	/* Speed mode configuration */
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (RX1_PINPOS*2)), (GPIO_Speed << (RX1_PINPOS*2)));

	/* Output mode configuration*/
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << RX1_PINPOS), (GPIO_OType_PP << RX1_PINPOS));

	/* Pull-up Pull down resistor configuration*/
	sil_mdw_mem((VP)(TADR_U1_GPIO_BASE+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (RX1_PINPOS*2)), (GPIO_PuPd_UP << (RX1_PINPOS*2)));

	/* The RX and TX pins are now connected to their AF so that the USART1 can take over control of the pins */
	setup_gpio_source((VP)TADR_U1_GPIO_BASE, TX1_PINPOS, GPIO_AF7);
	setup_gpio_source((VP)TADR_U1_GPIO_BASE, RX1_PINPOS, GPIO_AF7);
#endif

	/* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit :
	   Set STOP[13:12] bits according to USART_StopBits value */
	sil_mdh_mem((VP)(usart_base+TOFF_USART_CR2), USART_CR2_STOP, USART_StopBits_1);

	/* Configure the USART Word Length, Parity and mode: 
	   Set the M bits according to USART_WordLength value 
	   Set PCE and PS bits according to USART_Parity value
	   Set TE and RE bits according to USART_Mode value */
	sil_mdh_mem((VP)(usart_base+TOFF_USART_CR1), CR1_CLEAR_MASK, USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx);

	/* Configure the USART HFC : 
	   Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
	sil_mdh_mem((VP)(usart_base+TOFF_USART_CR3), CR3_CLEAR_MASK, USART_HardwareFlowControl_None);

	/* Configure the USART Baud Rate */
	RCC_GetClocksFreq(&RCC_ClocksStatus);

	if ((usart_base == TADR_USART1_BASE) || (usart_base == TADR_USART6_BASE))
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	else
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;

	/* Determine the integer part */
	if ((sil_reh_mem((VP)(usart_base+TOFF_USART_CR1)) & USART_CR1_OVER8) != 0){
		integerdivider = ((25 * apbclock) / (2 * (DEFAULT_SPEED)));	/* 8 Samples */
		tmpreg = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & 0x07;
	}
	else{ /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
		integerdivider = ((25 * apbclock) / (4 * (DEFAULT_SPEED)));	/* 16 Samples */
		tmpreg = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & 0x0F;
	}
	/* Write to USART BRR register */
	sil_wrh_mem((VP)(usart_base+TOFF_USART_BRR), tmpreg);

	sil_mdh_mem((VP)(usart_base+TOFF_USART_CR1), 0, USART_CR1_RXNEIE);

    /* Enable the selected USART by setting the UE bit in the CR1 register */
	sil_mdh_mem((VP)(usart_base+TOFF_USART_CR1), 0, USART_CR1_UE);
}


void sys_putc(int c)
{
	unsigned int  usart_base = usart_table[0].taddr;
	while((sil_reh_mem((VP)(usart_base+TOFF_USART_SR)) & USART_SR_TC) == 0);
	sil_wrh_mem((VP)(usart_base+TOFF_USART_DR), (c & 0xFF));
}

void sys_puthex(unsigned int val)
{
	int i, j;

	for(i = 32; i > 0 ; i -= 4){
		j = (val >> (i-4)) & 15;
		if(j < 10)
			sys_putc(j + '0');
		else
			sys_putc(j - 10 + 'A');
	}
}

void sys_printf(const char *s)
{
	int c;

	while((c = *s++) != 0){
		if(c == '\n')
			sys_putc('\r');
		sys_putc(c);
	}
}

/*
 *  未定義の例外が入った場合の処理
 */
void NMI_Handler(void)
{
    sys_printf("NMI Exception occurs. !\n");
    while(1);
}

void HardFault_Handler(void)
{
    sys_printf("Hard Fault Exception occurs. !\n");
    while(1);
}

void MemManage_Handler(void)
{
    sys_printf("Memory Manager Exception occurs. !\n");
    while(1);
}

void BusFault_Handler(void)
{
    sys_printf("Bus Fault Exception occurs. !\n");
    while(1);
}


void UsageFault_Handler(void)
{
    sys_printf("Usage Fault Exception occurs. !\n");
    while(1);
}

void DebugMon_Handler(void)
{
    sys_printf("Debug Monitor Exception occurs. !\n");
    while(1);
}

void PendSV_Handler(void)
{
    sys_printf("Pend SV Exception occurs. !\n");
    while(1);
}

