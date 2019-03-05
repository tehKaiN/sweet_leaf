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
#include <sweet_leaf/pin.h>

// TODO submodule
// https://github.com/mpaland/printf

tPin D[16] = {
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
tPin A[24] = {
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

tPin NWAIT = {GPIOD, GPIO_PIN_6};
tPin NOE = {GPIOD, GPIO_PIN_4};
tPin NWE = {GPIOD, GPIO_PIN_5};
tPin NE = {GPIOD, GPIO_PIN_7};
tPin NL = {GPIOB, GPIO_PIN_7};
tPin NBL0 = {GPIOE, GPIO_PIN_0};
tPin NBL1 = {GPIOE, GPIO_PIN_1};
tPin BERR = {GPIOC, GPIO_PIN_8};

void pseudoMemInit(void) {

	HAL_SRAM_MspDeInit(0);

	// Data
	for(uint8_t i = 0; i < 16; ++i) {
		pinInit(D[i], false);
	}

	// Addr
	for(uint8_t i = 0; i < 24; ++i) {
		pinInit(A[i], true);
	}

	// /OE, /WE, /E, /BL0, /BL1, /WAIT
	pinInit(NE, true);
	pinSet(NE, 1);

	pinInit(NOE, true);
	pinSet(NOE, 1);

	pinInit(NWE, true);
	pinSet(NWE, 1);

	pinInit(NBL0, true);
	pinSet(NBL0, 1);

	pinInit(NBL1, true);
	pinSet(NBL1, 1);

	pinInit(NWAIT, false);
	pinInit(BERR, false);
}

uint16_t pseudoMemRead(uint32_t ulAddr) {

	for(uint8_t i = 0; i < 24; ++i) {
		pinSet(A[i], ulAddr & 1);
		ulAddr >>= 1;
	}
	pinSet(NBL0, 0);
	pinSet(NBL1, 0);
	pinSet(NE, 0);

	bool wasWait = false;
	while(pinGet(NWAIT) == true) {wasWait = true;}

	uint16_t uwResult = 0;
	for(uint8_t i = 16; i--;) {
		uwResult <<= 1;
		uwResult |= pinGet(D[i]);
	}

	pinSet(NE, 1);
	pinSet(NBL0, 1);
	pinSet(NBL1, 1);
	pinSet(NWE, 1);
	pinSet(NOE, 1);
	while(pinGet(NWAIT) == false) {wasWait = true;}
	if(wasWait) {
		logPrintf("WAS WAIT!\r\n");
	}
	return uwResult;
}

__attribute__((noreturn))
void slMain(void) {
	logPrintf("init, clock: %u\r\n", SystemCoreClock);
	bool wasSwitch = 0;
	bool isAmiOnline = false;
	while(1) {
		bool isSwitch = HAL_GPIO_ReadPin(gpioBtn_GPIO_Port, gpioBtn_Pin);

		if(isSwitch && !wasSwitch) {
			if(!isAmiOnline) {
				HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 1);
				if(clockToExternal()) {
					logPrintf("clock to external: success, clock: %u\r\n", SystemCoreClock);
					isAmiOnline = true;
					pseudoMemInit();
				}
				else {
					clockToInternal();
					HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 0);
					logPrintf("ERR: Clock to external\r\n");
					isAmiOnline = false;
				}
			}
			else {
				// logPrintf("BERR: %d\r\n", pinGet(BERR));
				logPrintf("================Start read\r\n");
				const uint32_t ulStartAddr = 0xFC0000;
				// const uint32_t ulStartAddr = 0x000000;
				uint16_t pRef[6] = {0x1111, 0x4EF9, 0x00FC, 0x00D2, 0x0000, 0xFFFF};
				uint16_t pRead[6] = {0x0000};
				for(uint8_t i = 0; i < 6; ++i) {
					uint32_t ulAddr = ulStartAddr + i*2;
					pRead[i] = pseudoMemRead(ulAddr);
					logPrintf(
						"%06lX: %04X vs %04X, diff %04X\r\n",
						ulAddr, pRead[i], pRef[i], (pRead[i] ^ pRef[i]) & pRead[i]
					);
					// logPrintf("BERR: %d\r\n", pinGet(BERR));
				}
				logPrintf("================End read\r\n");
			}
		}
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		HAL_Delay(900);

		wasSwitch = isSwitch;
	}
}
