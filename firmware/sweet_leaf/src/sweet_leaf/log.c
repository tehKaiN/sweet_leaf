/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sweet_leaf/log.h>
#include <stdint.h>
#include <printf/printf.h>
#include <usart.h>

void logPrintf(const char *szFmt, ...) {
	va_list vaArgs;
	va_start(vaArgs, szFmt);
	char szMsg[1000];
	int32_t lLen = vsnprintf(szMsg, 1000, szFmt, vaArgs);
	va_end(vaArgs);
	HAL_UART_Transmit(&huart3, (uint8_t*)szMsg, lLen, 100);
}
