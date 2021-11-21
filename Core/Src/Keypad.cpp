#include "Keypad.h"

const char keys[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

GPIO_TypeDef* const ports[2][4] = {
	{KEYPAD_IN_0_GPIO_Port, KEYPAD_IN_1_GPIO_Port, KEYPAD_IN_2_GPIO_Port, KEYPAD_IN_3_GPIO_Port},
	{KEYPAD_OUT_0_GPIO_Port, KEYPAD_OUT_1_GPIO_Port, KEYPAD_OUT_2_GPIO_Port, KEYPAD_OUT_3_GPIO_Port}
};

const uint16_t pins[2][4] = {
	{KEYPAD_IN_0_Pin, KEYPAD_IN_1_Pin, KEYPAD_IN_2_Pin, KEYPAD_IN_3_Pin},
	{KEYPAD_OUT_0_Pin, KEYPAD_OUT_1_Pin, KEYPAD_OUT_2_Pin, KEYPAD_OUT_3_Pin}
};

char Keypad::get_key(void) {
	char c = 'x';
	for (uint8_t i=0; i<4; i++) {
		for(uint8_t j=0; j<4; j++) {
			if (HAL_GPIO_ReadPin(ports[0][i], pins[0][i]) == GPIO_PIN_SET) {
				HAL_GPIO_WritePin(ports[1][j], pins[1][j], GPIO_PIN_RESET);
				if (HAL_GPIO_ReadPin(ports[0][i], pins[0][i]) == GPIO_PIN_RESET) {
					c = keys[i][j];
					break;
				}
			}
		}
		HAL_GPIO_WritePin(KEYPAD_OUT_0_GPIO_Port, KEYPAD_OUT_0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(KEYPAD_OUT_1_GPIO_Port, KEYPAD_OUT_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(KEYPAD_OUT_2_GPIO_Port, KEYPAD_OUT_2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(KEYPAD_OUT_3_GPIO_Port, KEYPAD_OUT_3_Pin, GPIO_PIN_SET);
		if (c != 'x') return c;
	}
	return c;
}
