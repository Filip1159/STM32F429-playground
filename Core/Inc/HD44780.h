#ifndef HD44780_H
#define HD44780_H

#include "main.h"
#include "stdint.h"

typedef struct {
	GPIO_TypeDef* RS_GPIO_Port;
	uint16_t RS_GPIO_Pin;
	GPIO_TypeDef* E_GPIO_Port;
	uint16_t E_GPIO_Pin;
	GPIO_TypeDef* D4_GPIO_Port;
	uint16_t D4_GPIO_Pin;
	GPIO_TypeDef* D5_GPIO_Port;
	uint16_t D5_GPIO_Pin;
	GPIO_TypeDef* D6_GPIO_Port;
	uint16_t D6_GPIO_Pin;
	GPIO_TypeDef* D7_GPIO_Port;
	uint16_t D7_GPIO_Pin;
} HD44780_TypeDef;


class HD44780 {
private:
	HD44780_TypeDef config;
	void write_upper(uint8_t cmd);
	void write_lower(uint8_t cmd);
	void command(uint8_t cmnd);

public:
	void init(void);
	void writeChar(char data);
	void charXY(uint8_t row, uint8_t pos, uint8_t data);
	void string(char *str);
	void stringXY(uint8_t row, uint8_t pos, char *str);
	void clear();
};

#endif
