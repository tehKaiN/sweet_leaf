/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sweet_leaf/sweet_leaf.h>
#include <string.h>
#include <gpio.h>
#include <usart.h>
#include <main.h>
#include <sweet_leaf/clock.h>
#include <sweet_leaf/log.h>

// TODO submodule
// https://github.com/mpaland/printf

typedef struct _tPinPort {
	GPIO_TypeDef *pPort;
	uint32_t ulPin;
} tPinPort;

tPinPort D[16] = {
	{GPIOD, GPIO_PIN_14},
	{GPIOD, GPIO_PIN_15},
	{GPIOD, GPIO_PIN_0},
	{GPIOD, GPIO_PIN_1},
	{GPIOE, GPIO_PIN_7},
	{GPIOE, GPIO_PIN_8},
	{GPIOE, GPIO_PIN_9},
	{GPIOE, GPIO_PIN_10},
	{GPIOE, GPIO_PIN_11},
	{GPIOE, GPIO_PIN_12},
	{GPIOE, GPIO_PIN_13},
	{GPIOE, GPIO_PIN_14},
	{GPIOE, GPIO_PIN_15},
	{GPIOD, GPIO_PIN_8},
	{GPIOD, GPIO_PIN_9},
	{GPIOD, GPIO_PIN_10}
};
tPinPort A[24] = {
	{GPIOF, GPIO_PIN_0},
	{GPIOF, GPIO_PIN_1},
	{GPIOF, GPIO_PIN_2},
	{GPIOF, GPIO_PIN_3},
	{GPIOF, GPIO_PIN_4},
	{GPIOF, GPIO_PIN_5},
	{GPIOF, GPIO_PIN_12},
	{GPIOF, GPIO_PIN_13},
	{GPIOF, GPIO_PIN_14},
	{GPIOF, GPIO_PIN_15},
	{GPIOG, GPIO_PIN_0},
	{GPIOG, GPIO_PIN_1},
	{GPIOG, GPIO_PIN_2},
	{GPIOG, GPIO_PIN_3},
	{GPIOG, GPIO_PIN_4},
	{GPIOG, GPIO_PIN_5},
	{GPIOD, GPIO_PIN_11},
	{GPIOD, GPIO_PIN_12},
	{GPIOD, GPIO_PIN_13},
	{GPIOE, GPIO_PIN_3},
	{GPIOE, GPIO_PIN_4},
	{GPIOE, GPIO_PIN_5},
	{GPIOE, GPIO_PIN_6},
	{GPIOE, GPIO_PIN_2}
};

tPinPort NWAIT = {GPIOD, GPIO_PIN_6};
tPinPort NOE = {GPIOD, GPIO_PIN_4};
tPinPort NWE = {GPIOD, GPIO_PIN_5};
tPinPort NE = {GPIOD, GPIO_PIN_7};
tPinPort NL = {GPIOB, GPIO_PIN_7};
tPinPort NBL0 = {GPIOE, GPIO_PIN_0};
tPinPort NBL1 = {GPIOE, GPIO_PIN_1};

void initInput(tPinPort sPinPort) {
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = sPinPort.ulPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(sPinPort.pPort, &GPIO_InitStruct);
}

void initOutput(tPinPort sPinPort) {
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = sPinPort.ulPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(sPinPort.pPort, &GPIO_InitStruct);
}

void set(tPinPort sPinPort, bool isHigh) {
	HAL_GPIO_WritePin(sPinPort.pPort, sPinPort.ulPin, isHigh);
}

bool get(tPinPort sPinPort) {
	return HAL_GPIO_ReadPin(sPinPort.pPort, sPinPort.ulPin);
}

void pseudoMemInit(void) {

	HAL_SRAM_MspDeInit(0);

	// Data
	for(uint8_t i = 0; i < 16; ++i) {
		initInput(D[i]);
	}

	// Addr
	for(uint8_t i = 0; i < 24; ++i) {
		initOutput(A[i]);
	}

	// /OE, /WE, /E, /BL0, /BL1, /WAIT
	initOutput(NOE);
	initOutput(NWE);
	initOutput(NE);
	initOutput(NBL0);
	initOutput(NBL1);
	initInput(NWAIT);
}

uint16_t pseudoMemRead(uint32_t ulAddr) {
	set(NE, 1);
	set(NWE, 1);
	set(NOE, 1);

	for(uint8_t i = 0; i < 24; ++i) {
		set(A[i], ulAddr & 1);
		ulAddr >>= 1;
	}
	set(NBL0, 0);
	set(NBL1, 0);
	set(NE, 0);

	while(get(NWAIT)) { }

	uint16_t uwResult = 0;
	for(uint8_t i = 16; i--;) {
		uwResult <<= 1;
		uwResult |= get(D[i]);
	}

	set(NE, 1);
	set(NWE, 1);
	set(NOE, 1);
}

__attribute__((noreturn))
void slMain(void) {
	logPrintf("init, clock: %u\r\n", SystemCoreClock);
	bool wasSwitch = 0;
	while(1) {
		bool isSwitch = HAL_GPIO_ReadPin(gpioBtn_GPIO_Port, gpioBtn_Pin);

		if(isSwitch && !wasSwitch) {
			HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 1);
			if(clockToExternal()) {
				logPrintf("clock to external: success, clock: %u\r\n", SystemCoreClock);
			}
			else {
				clockToInternal();
				HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 0);
				logPrintf("ERR: Clock to external\r\n");
			}
		}
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		HAL_Delay(900);

		wasSwitch = isSwitch;
	}
}
