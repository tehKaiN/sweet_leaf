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

void trueMemInit(void) {
	HAL_SRAM_MspInit(0);
}

void trueMemDeinit() {
	HAL_SRAM_MspDeInit(0);
}

void readTest(void) {
	uint32_t ulTickStart = HAL_GetTick();

	const uint32_t ulStartAddr = 0x000000;
	uint32_t ulPrev = 0, ulVal = 0;
	do {
		ulPrev = ulVal;
		ulVal = pseudoMemRead(ulStartAddr);
	} while(ulPrev != ulVal);

	uint32_t ulTickEnd = HAL_GetTick();
	logPrintf("Stabilized after %lums\r\n", ulTickEnd - ulTickStart);

	logPrintf("================Start read\r\n");
	// const uint32_t ulStartAddr = 0xFC0000;
	uint16_t pRef[6] = {0x1111, 0x4EF9, 0x00FC, 0x00D2, 0x0000, 0xFFFF};
	uint16_t pRead[6] = {0x0000};
	for(uint8_t i = 0; i < 6; ++i) {
		uint32_t ulAddr = ulStartAddr + i*2;
		pRead[i] = pseudoMemRead(ulAddr);
		logPrintf(
			"%06lX: %04X vs %04X, diff %04X\r\n",
			ulAddr, pRead[i], pRef[i], pRead[i] ^ pRef[i]
		);
	}
	logPrintf("================End read\r\n");

}

__attribute__((noreturn))
void slMain(void) {
	logPrintf("init, clock: %uMHz\r\n", SystemCoreClock / 1000000);
	bool wasSwitch = 0;
	bool isAmiOnline = false;
	while(1) {
		bool isSwitch = HAL_GPIO_ReadPin(gpioBtn_GPIO_Port, gpioBtn_Pin);

		if(isSwitch && !wasSwitch) {
			if(!isAmiOnline) {
				HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 1);
				if(clockToExternal()) {
					logPrintf("clock to external: success, clock: %uMHz\r\n", SystemCoreClock / 1000000);
					isAmiOnline = true;
					pseudoMemInit();
					readTest();
				}
				else {
					clockToInternal();
					HAL_GPIO_WritePin(gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 0);
					logPrintf("ERR: Clock to external\r\n");
					isAmiOnline = false;
				}
			}
			else {
				readTest();
			}
		}
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		HAL_Delay(900);

		wasSwitch = isSwitch;
	}
}
