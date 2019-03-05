/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sweet_leaf/pin.h>

void pinInit(tPin sPinPort, bool isOutput) {
	// Enable clock
	if(sPinPort.pPort == GPIOA) { __HAL_RCC_GPIOA_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOD) { __HAL_RCC_GPIOD_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOE) { __HAL_RCC_GPIOE_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOF) { __HAL_RCC_GPIOF_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOG) { __HAL_RCC_GPIOG_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOH) { __HAL_RCC_GPIOH_CLK_ENABLE(); }
	else if(sPinPort.pPort == GPIOI) { __HAL_RCC_GPIOI_CLK_ENABLE(); }
	else {
		// TODO error
	}

  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = sPinPort.ulPin;
	if(isOutput) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	}
	else {
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	}
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(sPinPort.pPort, &GPIO_InitStruct);
}

void pinSet(tPin sPinPort, bool isHigh) {
	HAL_GPIO_WritePin(sPinPort.pPort, sPinPort.ulPin, isHigh);
}

volatile bool pinGet(tPin sPinPort) {
	return HAL_GPIO_ReadPin(sPinPort.pPort, sPinPort.ulPin);
}
