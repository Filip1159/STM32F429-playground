#include "HD44780.h"

void HD44780::write_upper(uint8_t cmd) {
	HAL_GPIO_WritePin(config.D4_GPIO_Port, config.D4_GPIO_Pin, (GPIO_PinState)(cmd & 0x10));
	HAL_GPIO_WritePin(config.D5_GPIO_Port, config.D5_GPIO_Pin, (GPIO_PinState)(cmd & 0x20));
	HAL_GPIO_WritePin(config.D6_GPIO_Port, config.D6_GPIO_Pin, (GPIO_PinState)(cmd & 0x40));
	HAL_GPIO_WritePin(config.D7_GPIO_Port, config.D7_GPIO_Pin, (GPIO_PinState)(cmd & 0x80));
}

void HD44780::write_lower(uint8_t cmd) {
	HAL_GPIO_WritePin(config.D4_GPIO_Port, config.D4_GPIO_Pin, (GPIO_PinState)(cmd & 0x01));
	HAL_GPIO_WritePin(config.D5_GPIO_Port, config.D5_GPIO_Pin, (GPIO_PinState)(cmd & 0x02));
	HAL_GPIO_WritePin(config.D6_GPIO_Port, config.D6_GPIO_Pin, (GPIO_PinState)(cmd & 0x04));
	HAL_GPIO_WritePin(config.D7_GPIO_Port, config.D7_GPIO_Pin, (GPIO_PinState)(cmd & 0x08));
}

void HD44780::init(void) {
	config.RS_GPIO_Port = LCD_TEXT_RS_GPIO_Port;
	config.RS_GPIO_Pin = LCD_TEXT_RS_Pin;
	config.E_GPIO_Port = LCD_TEXT_E_GPIO_Port;
	config.E_GPIO_Pin = LCD_TEXT_E_Pin;
	config.D4_GPIO_Port = LCD_TEXT_D4_GPIO_Port;
	config.D4_GPIO_Pin = LCD_TEXT_D4_Pin;
	config.D5_GPIO_Port = LCD_TEXT_D5_GPIO_Port;
	config.D5_GPIO_Pin = LCD_TEXT_D5_Pin;
	config.D6_GPIO_Port = LCD_TEXT_D6_GPIO_Port;
	config.D6_GPIO_Pin = LCD_TEXT_D6_Pin;
	config.D7_GPIO_Port = LCD_TEXT_D7_GPIO_Port;
	config.D7_GPIO_Pin = LCD_TEXT_D7_Pin;

	HAL_Delay(20);		/* LCD Power ON delay always >15ms */

	command(0x33);
	command(0x32);	/* Send for 4 bit initialization of LCD  */
	command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	command(0x0c);	/* Display on cursor off */
	command(0x06);	/* Increment cursor (shift cursor to right) */
	command(0x01);	/* Clear display screen */
}

void HD44780::command(uint8_t cmnd) {
	write_upper(cmnd);
	HAL_GPIO_WritePin(config.RS_GPIO_Port, config.RS_GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	write_lower(cmnd);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(2);
}

void HD44780::charXY(uint8_t row, uint8_t pos, uint8_t data) {
    if (row == 0 && pos < 16)
    	command((pos & 0x0F) | 0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos < 16)
		command((pos & 0x0F) | 0xC0);	/* Command of first row and required position<16 */
    writeChar(data);
}

void HD44780::writeChar(char data) {
	write_upper(data);
	HAL_GPIO_WritePin(config.RS_GPIO_Port, config.RS_GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	write_lower(data);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(config.E_GPIO_Port, config.E_GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(2);
}

void HD44780::string(char *str) {
	for (int i=0; str[i] != 0; i++) {
		writeChar(str[i]);
	}
}

void HD44780::stringXY(uint8_t row, uint8_t pos, char *str) {
	if (row == 0 && pos < 16)
		command((pos & 0x0F) | 0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos < 16)
		command((pos & 0x0F) | 0xC0);	/* Command of first row and required position<16 */
	string(str);		/* Call LCD string function */
}

void HD44780::clear()
{
	command(0x01);		/* Clear display */
	HAL_Delay(2);
	command(0x80);		/* Cursor at home position */
}
