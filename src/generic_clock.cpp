/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
// imported from variants/STM32H7xx/H742X(G-I)H_H743X(G-I)H_H745X(G-I)H_H747X(G-I)H_H750XBH_H753XIH_H755XIH_H757XIH/generic_clock.c
#include "stm32_def.h"
#if defined(ARDUINO_GENERIC_H742XGHX) || defined(ARDUINO_GENERIC_H742XIHX) ||\
    defined(ARDUINO_GENERIC_H743XGHX) || defined(ARDUINO_GENERIC_H743XIHX) ||\
    defined(ARDUINO_GENERIC_H745XGHX) || defined(ARDUINO_GENERIC_H745XIHX) ||\
    defined(ARDUINO_GENERIC_H747XGHX) || defined(ARDUINO_GENERIC_H747XIHX) ||\
    defined(ARDUINO_GENERIC_H750XBHX) || defined(ARDUINO_GENERIC_H753XIHX) ||\
    defined(ARDUINO_GENERIC_H755XIHX) || defined(ARDUINO_GENERIC_H757XIHX)
#include "pins_arduino.h"

/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
WEAK void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {};

  /** Supply configuration update enable
  */
#if defined(SMPS)
  /** If SMPS is available on this MCU, assume that the MCU's board is
   *  built to power the MCU using the SMPS since it's more efficient.
   *  In this case, we must configure the MCU to use DIRECT_SMPS_SUPPLY.
   *
   *  N.B.: if the hardware configuration does not match the argument to
   *  HAL_PWREx_ConfigSupply(), the board will deadlock at this function call.
   *  This can manifest immediately or after a RESET/power cycle.
   *
   *  Trying to flash the board at this point will result in errors such as
   *  "No STM32 target found". To overcome this problem, erase the MCU's flash.
   *
   *  The following settings in STM32CubeProgrammer appear to work for this purpose:
   *   - Mode: Power down
   *   - Reset mode: Hardware reset
   */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
#else
  /** No SMPS available: use the internal voltage regulator (LDO).
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
#endif

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_CSI
                                     | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    Error_Handler();
  }

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_CEC | RCC_PERIPHCLK_CKPER
                                             | RCC_PERIPHCLK_HRTIM1 | RCC_PERIPHCLK_I2C123 | RCC_PERIPHCLK_I2C4
                                             | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | RCC_PERIPHCLK_LPTIM3
                                             | RCC_PERIPHCLK_QSPI | RCC_PERIPHCLK_SDMMC | RCC_PERIPHCLK_USB
                                             | RCC_PERIPHCLK_SPI123 | RCC_PERIPHCLK_SPI45 | RCC_PERIPHCLK_SPI6
                                             | RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_USART16 | RCC_PERIPHCLK_USART234578
                                             | RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
  PeriphClkInitStruct.CecClockSelection = RCC_CECCLKSOURCE_CSI;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  PeriphClkInitStruct.Hrtim1ClockSelection = RCC_HRTIM1CLK_CPUCLK;
  PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.Lptim345ClockSelection = RCC_LPTIM345CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_PLL;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
    Error_Handler();
  }
}

#endif /* ARDUINO_GENERIC_* */
