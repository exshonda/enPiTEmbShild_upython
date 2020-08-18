/*
 *
 *  LED Blink Program (Shift)
 *
 */
#include "sys_defs.h"

/*
 *  Programmable I/O Port
 */
#define TADR_RCC_AHB1ENR	0x40023830	/* RCC AHB1ENR Register */
#define TADR_GPIOA_MODER	0x40020000	/* A Port MODE Register */
#define TADR_GPIOA_ODR		0x40020014	/* A Port Output Data Register */

#define TREG_RCC_AHB1ENR	((volatile unsigned long *)(TADR_RCC_AHB1ENR))		/* RCC AHB1EN Register */
#define TREG_GPIOA_MODER	((volatile unsigned long *)(TADR_GPIOA_MODER))		/* A Port MODE Register */

/*
 *  LED connected port
 */
#define LED1_PINPOS		8
#define LED2_PINPOS		7
#define LED3_PINPOS		6
#define LED4_PINPOS		5

/*
 *  LED connected bit
 */
#define LED1	(1<<LED1_PINPOS)
#define LED2	(1<<LED2_PINPOS)
#define LED3	(1<<LED3_PINPOS)
#define LED4	(1<<LED4_PINPOS)

#define LED_MASK	(LED1 | LED2 | LED3 | LED4)

/*
 *  LED connected port writer
 */

/*
 *  Without Macro
 */
void
led_out_no_macro(unsigned short led_data){
	unsigned int reg;

	reg = *((volatile unsigned long *)(0x40020014));
	reg = reg & ~((1<<8)|(1<<7)|(1<<6)|(1<<5));

	led_data = led_data & ((1<<8)|(1<<7)|(1<<6)|(1<<5));
	
	reg = reg | led_data;
	*((volatile unsigned long *)(0x40020014)) = reg;
}

/*
 *  With Macro 1
 */

/*
 *  LED Connected Port Register
 */
#define TADR_GPIOA_BSRR   0x40020018  /* A Port SET/CLEAR Register */

void
led_out_macro1(unsigned short led_data){
	unsigned int reg;

	reg = *((volatile unsigned long *)(TADR_GPIOA_ODR));
	reg = reg & ~LED_MASK;

	led_data = led_data & LED_MASK;
	
	reg = reg | led_data;
	*((volatile unsigned long *)(TADR_GPIOA_ODR)) = reg;
}

/*
 *  With Macro 2
 */

/*
 *  LED Connected Port Register
 */
#define TREG_GPIOA_ODR		((volatile unsigned long *)(TADR_GPIOA_ODR))		/* A Port Output Data Register */

void
led_out_macro2(unsigned short led_data){
	unsigned int reg;

	reg = *TREG_GPIOA_ODR;
	reg = reg & ~LED_MASK;

	led_data = led_data & LED_MASK;
	
	reg = reg | led_data;
	*TREG_GPIOA_ODR = reg;
}

/*
 *  With Access Function
 */

/*
 *  4 Byte Write 
 */
void
sil_wrw_mem(unsigned int addr, unsigned int data){
	*((volatile unsigned int *)addr) = data;
}

/*
 *  4 Byte Read
 */
unsigned int
sil_rew_mem(unsigned int addr){
	return *((volatile unsigned int *)addr);
}

void
led_out_sil(unsigned short led_data){
	unsigned int reg;

	reg = sil_rew_mem(TADR_GPIOA_ODR);
	reg = reg & ~LED_MASK;

	led_data = led_data & LED_MASK;
	
	reg = reg | led_data;
	sil_wrw_mem(TADR_GPIOA_ODR, reg);
}

/*
 *  LED Connected Port Initialize
 */ 
void
led_init(void){
	unsigned long reg;

	*TREG_RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* Set As Output Mode */
	reg = *TREG_GPIOA_MODER & ~(0x03 << (LED1_PINPOS * 2));
	*TREG_GPIOA_MODER = reg | (0x01 << (LED1_PINPOS * 2));

	reg = *TREG_GPIOA_MODER & ~(0x03 << (LED2_PINPOS * 2));
	*TREG_GPIOA_MODER = reg | (0x01 << (LED2_PINPOS * 2));

	reg = *TREG_GPIOA_MODER & ~(0x03 << (LED3_PINPOS * 2));
	*TREG_GPIOA_MODER = reg | (0x01 << (LED3_PINPOS * 2));

	reg = *TREG_GPIOA_MODER & ~(0x03 << (LED4_PINPOS * 2));
	*TREG_GPIOA_MODER = reg | (0x01 << (LED4_PINPOS * 2));
}

#define DELAY_LOOP (250*1000)  /* loop number */

/*  
 *  Busy Loop Function
 */
void
busy_wait(void){
	long i;
	for(i = 0; i < DELAY_LOOP; i++)
		sil_dly_nse(1000);	/* 1 micro sec */
}

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define DWT_CONTROL             (*((volatile unsigned int*)0xE0001000))
/*!< DWT Control register */
#define DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define DWT_CYCCNT              (*((volatile unsigned int*)0xE0001004))
/*!< DWT Cycle Counter register */
#define DEMCR                   (*((volatile unsigned int*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define InitCycleCounter()		DEMCR |= TRCENA_BIT
#define ResetCycleCounter()		DWT_CYCCNT = 0
#define StartCycleCounter()		DWT_CONTROL |= DWT_CYCCNTENA_BIT
#define StopCycleCounter()		DWT_CONTROL &= ~DWT_CYCCNTENA_BIT
#define GetCycleCounter()		DWT_CYCCNT


void
led1_on(void)
{
	unsigned int reg;
	reg = *((volatile unsigned long *)(0x40020014));
	reg = reg | LED1;
	*((volatile unsigned long *)(0x40020014)) = reg;
}

void
led1_off(void)
{
	unsigned int reg;
	reg = *((volatile unsigned long *)(0x40020014));
	reg = reg & ~LED1;
	*((volatile unsigned long *)(0x40020014)) = reg;
}

#define NUM_LOOP 200000

int perf_count;

void
perf(void){
	int count;
	int i;

	InitCycleCounter();
	ResetCycleCounter();
	StartCycleCounter();
	for(i = 0; i < NUM_LOOP; i++) {
		led1_on();
		led1_off();
	}
	StopCycleCounter();
	perf_count = GetCycleCounter();
}


int fib_count;

#define FIB_NUM_LOOP 30

int
fib(int n) {
	int a = 0;
	int b = 1;
	int c ;
	int i;

	if (n == 1) {
		return a;
	}
	else if (n == 2) {
		return b;
	}
	else {
		for(i = 0; i < (n-2); i++) {
			c = a + b;
			a = b;
			b = c;
		}
		return b;
	}
}

int fib_result;

void
fib_perf(void){
	int count;
	int i;

	ResetCycleCounter();
	StartCycleCounter();
	fib_result = fib(FIB_NUM_LOOP);
	StopCycleCounter();
	fib_count = GetCycleCounter();
}




/*
 *  main Function
 */ 
void
_main(void){

	led_init();

	perf();
	fib_perf();

	for (;;) {
		led_out_no_macro(0x00);
		busy_wait();

		led_out_no_macro(LED1);
		busy_wait();

		led_out_macro1(LED2);
		busy_wait();

		led_out_macro2(LED3);
		busy_wait();

		led_out_sil(LED4);
		busy_wait();
	}
}
