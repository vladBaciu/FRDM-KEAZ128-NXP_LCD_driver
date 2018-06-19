/*
 * delay.c
 *
 *  Created on: 9 mai 2018
 *      Author: Alex
 */
#include "delay.h"
#define BUS_PERIOD 50

void init_IRQs(void) {
	NVIC_ClearPendingIRQ(PIT_CH0_IRQn); /* Clear any Pending IRQ for all PIT ch0 (#22) */
	NVIC_EnableIRQ(PIT_CH0_IRQn); /* Set Enable IRQ for PIT_CH0 */
	NVIC_SetPriority(PIT_CH0_IRQn, 0); /* Set Priority for PIT_CH0 */
}

void init_delay(void) {
	SIM_SCGC |= SIM_SCGC_PIT_MASK; /* Enable bus clock to PIT module */
	PIT_MCR = 0x1; /* Turn on PIT module, Freeze ENABLED */
	// PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;  /* Enable interrupt */
}
void delay_ns(unsigned int ns) {
	//PIT_LDVAL0 = 20000000 - 1;         /* PIT0: Load value to count 20M bus clocks */
	PIT_LDVAL0 = (ns / BUS_PERIOD) - 1; // setup timer 3 for 1500000 cycles
	//PIT_LDVAL0 = 20000000 - 1;
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; /* Enable (start) timer */
	while (!PIT_TFLG0)
		;
	PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // disable Timer 3
	PIT_TFLG0 = 1; // reset the flag
}
void delay_us(unsigned int us) {
	//PIT_LDVAL0 = 20000000 - 1;         /* PIT0: Load value to count 20M bus clocks */
	PIT_LDVAL0 = ((us * 1000) / BUS_PERIOD) - 1; // setup timer 3 for 1500000 cycles
	//PIT_LDVAL0 = 20000000 - 1;
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; /* Enable (start) timer */
	while (!PIT_TFLG0)
		;
	PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // disable Timer 3
	PIT_TFLG0 = 1; // reset the flag
}
void delay_ms(unsigned int ms) {
	//PIT_LDVAL0 = 20000000 - 1;         /* PIT0: Load value to count 20M bus clocks */
	PIT_LDVAL0 = ((ms * 1000000) / BUS_PERIOD) - 1; // setup timer 3 for 1500000 cycles
	//PIT_LDVAL0 = 20000000 - 1;
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; /* Enable (start) timer */
	while (!PIT_TFLG0)
		;
	PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; // disable Timer 3
	PIT_TFLG0 = 1; // reset the flag
}
