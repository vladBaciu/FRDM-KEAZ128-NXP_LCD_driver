/*
 * LCD_Driver.c
 *
 *  Created on: 13 mai 2018
 *      Author: Vlad
 */

#include "LCD_Driver.h"
#include "derivative.h"
#include "delay.h"
#include "types.h"

#define RS 4							/* Define Register Select (data reg./command reg.) signal pin */
#define EN 5							/* Define Enable signal pin */

void LCD_Send4bitCmd(uint8 cmnd) {
	GPIOB_PCOR = 1 << RS; /* RS=1, data. */
	//delay_ns(50);
	GPIOB_PSOR = 1 << EN; /* Enable pulse */
	GPIOB_PCOR = 0xF;
	//delay_ns(100);
	GPIOB_PSOR = (cmnd & 0x0F); /* sending upper nibble */
	delay_us(1);
	GPIOB_PCOR = 1 << EN;
	delay_us(2);
}

void LCD_Send4bitData(uint8 cmnd) {
	GPIOB_PSOR = 1 << RS; /* RS=1, data. */
	//delay_ns(50);
	GPIOB_PSOR = 1 << EN; /* Enable pulse */
	GPIOB_PCOR = 0xF;
	//delay_ns(100);
	GPIOB_PSOR = (cmnd & 0x0F); /* sending upper nibble */
	delay_us(1);
	GPIOB_PCOR = 1 << EN;
	delay_us(2);
}

void LCD_Command(uint8 cmnd) {

	LCD_Send4bitCmd(cmnd >> 4);

	delay_us(2);

	LCD_Send4bitCmd(cmnd & 0x0F);

	delay_ms(20);
}

void LCD_Char(uint8 data) {
	LCD_Send4bitData(data >> 4);

	delay_us(2);

	LCD_Send4bitData(data & 0x0F);

	delay_ms(20);
}

void LCD_Char_xy(uint8 row, uint8 pos, uint8 *str) /* Send string to LCD with xy position */
{
	if (row == 0 && pos < 16) {
		LCD_Command((pos & 0x0F) | 0x80); /* Command of first row and required position<16 */
		LCD_Command((pos & 0x0F) | 0x80);/* Command of first row and required position<16 */
	} else if (row == 1 && pos < 16) {
		LCD_Command((pos & 0x0F) | 0xC0); /* Command of second row and required position<16 */
		LCD_Command((pos & 0x0F) | 0xC0); /* Command of second row and required position<16 */
	}
	delay_us(40);
	LCD_Char(str); /* Call LCD string function */
}

void LCD_Init(void) /* LCD Initialize function */
{
	//GPIOB_PDDR |= 0x3f; /* Set pins as output */
	//GPIOB_PIDR |= 0x3f; /* Input disabled */
	int i;
	for (i = 0; i < 6; i++)
		GPIO_set_GPIOB_pin_mode(i, PIN_MODE_OUT);
	GPIOB_PCOR = 0x3F;
	delay_ms(20); // Wait for more than 15 ms after VCC rises to 4.5 V

	LCD_Command(0x03u);
	delay_ms(2);
	LCD_Command(0x2Cu);
	delay_us(40);
	LCD_Command(0x2Cu);
	delay_us(40);
	LCD_Command(0x08u);
	delay_us(40);
	LCD_Command(0x01u);
	delay_ms(2);
	LCD_Command(0x14u);
	delay_us(40);
	LCD_Command(0x06u);
	delay_us(40);
	LCD_Command(0x0Cu);
	delay_us(40);
}

void LCD_String(uint8 *str) /* Send string to LCD function */
{
	int i;
	for (i = 0; str[i] != 0; i++) /* Send each uint8 of string till the NULL */
	{
		LCD_Char(str[i]);
	}
}

void LCD_String_xy(uint8 row, uint8 pos, uint8 *str) /* Send string to LCD with xy position */
{
	if (row == 0 && pos < 16) {
		LCD_Command((pos & 0x0F) | 0x80); /* Command of first row and required position<16 */
		LCD_Command((pos & 0x0F) | 0x80);/* Command of first row and required position<16 */
	} else if (row == 1 && pos < 16) {
		LCD_Command((pos & 0x0F) | 0xC0); /* Command of second row and required position<16 */
		LCD_Command((pos & 0x0F) | 0xC0); /* Command of second row and required position<16 */
	}
	delay_us(40);
	LCD_String(str); /* Call LCD string function */
}

void LCD_Clear() {
	LCD_Command(0x01); /* Clear display */
	delay_ms(2);
	LCD_Command(0x01); /* Clear display */
}

