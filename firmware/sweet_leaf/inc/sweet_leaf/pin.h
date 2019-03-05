/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _SWEET_LEAF_PIN_H_
#define _SWEET_LEAF_PIN_H_

#include <stdbool.h>
#include "stm32f7xx_hal.h"

typedef struct _tPin {
	GPIO_TypeDef *pPort;
	uint32_t ulPin;
} tPin;

void pinInit(tPin sPinPort, bool isOutput);

void pinSet(tPin sPinPort, bool isHigh);

volatile bool pinGet(tPin sPinPort);


#endif // _SWEET_LEAF_PIN_H_
