#include <sweet_leaf/pseudo_mem.h>
#include <sweet_leaf/pin.h>

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

	trueMemDeinit();

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

void pseudoMemDenit(void) {
	for(uint8_t i = 0; i < 16; ++i) {
		pinDeinit(D[i]);
	}
	for(uint8_t i = 0; i < 24; ++i) {
		pinDeinit(A[i]);
	}
	pinDeinit(NE);
	pinDeinit(NOE);
	pinDeinit(NWE);
	pinDeinit(NBL0);
	pinDeinit(NBL1);
	pinDeinit(NWAIT);
	pinDeinit(BERR);
}

uint16_t pseudoMemRead(uint32_t ulAddr) {

	for(uint8_t i = 0; i < 24; ++i) {
		pinSet(A[i], ulAddr & 1);
		ulAddr >>= 1;
	}
	pinSet(NBL0, 0);
	pinSet(NBL1, 0);
	pinSet(NE, 0);

	while(pinGet(NWAIT) == true) { }

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
	while(pinGet(NWAIT) == false) { }
	return uwResult;
}
