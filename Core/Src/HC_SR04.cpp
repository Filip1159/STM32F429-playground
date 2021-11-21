#include "HC_SR04.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
volatile uint32_t first_value = 0;
volatile uint32_t second_value = 0;
volatile uint8_t is_first_captured = 0;
volatile uint8_t distance  = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		if (is_first_captured == 0) {
			first_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			is_first_captured = 1;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		} else {
			second_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(htim, 0);
			uint32_t difference;
			if (second_value > first_value) {
				difference = second_value-first_value;
			} else {
				difference = 0xffff - first_value + second_value;
			}
			distance = difference * .034/2;
			is_first_captured = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}

uint8_t HC_SR04::read(void) {
	HAL_GPIO_WritePin(HC_SR04_TRIG_GPIO_Port, HC_SR04_TRIG_Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < 10);
	HAL_GPIO_WritePin(HC_SR04_TRIG_GPIO_Port, HC_SR04_TRIG_Pin, GPIO_PIN_RESET);

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	distance = 0;
	while (distance == 0);
	return distance;
}
