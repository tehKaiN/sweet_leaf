/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <sweet_leaf/clock.h>
#include <main.h>

bool clockToExternal(void) {
	// STM32 docs is a pain in the butt
	// so I've temporarily generated Cube project with HSE as main source
	// and copied its SystemClock_Config() here
  HAL_RCC_DeInit();
  HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitTypeDef sOscInit;
  sOscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  sOscInit.HSEState = RCC_HSE_BYPASS;
  sOscInit.PLL.PLLState = RCC_PLL_ON;
  sOscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  sOscInit.PLL.PLLM = 7;
  sOscInit.PLL.PLLN = 320;
  sOscInit.PLL.PLLP = RCC_PLLP_DIV2;
  sOscInit.PLL.PLLQ = 9;
  sOscInit.HSIState = RCC_HSI_ON;
  if (HAL_RCC_OscConfig(&sOscInit) != HAL_OK)
  {
    return false;
  }

	RCC_ClkInitTypeDef sClkInit;
  sClkInit.ClockType =
		RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
		RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  sClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  sClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
  sClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
  sClkInit.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&sClkInit, FLASH_LATENCY_5) != HAL_OK)
  {
    return false;
  }

	// Peripheral clocks - switch UART to HSI
	RCC_PeriphCLKInitTypeDef sUartClkInit;
  sUartClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  sUartClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&sUartClkInit) != HAL_OK)
  {
    return false;
  }

	// Systick
  // HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  // HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	return true;
}

bool clockToInternal(void) {
  SystemClock_Config();
	return true;
}
