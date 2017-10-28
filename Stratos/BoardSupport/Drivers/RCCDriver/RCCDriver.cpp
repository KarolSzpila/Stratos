/*
 * RCCDriver.cpp
 *
 *  Created on: 21.10.2017
 *      Author: Karol
 */

#include "RCCDriver.h"

#include "stm32f7xx_hal.h"

RCCDriver::RCCDriver()
{
	HAL_Init();
	SystemClock_Config();
}


