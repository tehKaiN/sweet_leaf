#include <sweet_leaf.h>
#include <gpio.h>

__attribute__((noreturn))
void slMain(void) {
	while(1) {
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
		HAL_Delay(400);
	}
}
