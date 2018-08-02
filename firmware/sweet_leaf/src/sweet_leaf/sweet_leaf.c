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

__attribute__((noreturn))
void slMain(void) {
	logPrintf("init, clock: %u\r\n", SystemCoreClock);
	bool wasSwitch = 0;
	while(1) {
		bool isSwitch = HAL_GPIO_ReadPin(gpioBtn_GPIO_Port, gpioBtn_Pin);

		if(isSwitch && !wasSwitch) {
			HAL_GPIO_WritePin(
				gpioShifterEnable_GPIO_Port, gpioShifterEnable_Pin, 1
			);
			if(clockToExternal()) {
				logPrintf("clock to external: success, clock: %u\r\n", SystemCoreClock);
			}
			else {
				clockToInternal();
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
