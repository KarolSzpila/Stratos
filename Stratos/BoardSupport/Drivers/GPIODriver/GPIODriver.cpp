/*
 * GPIODriver.cpp
 *
 *  Created on: 07.10.2017
 *      Author: Karol
 */

#include "GPIODriver.h"

GPIODriver::GPIODriver() : LedPin(GPIOPin(GPIOA,GPIO_PIN_15)),
						   PSOPin(GPIOPin(GPIOA,GPIO_PIN_12))
{
	MX_GPIO_Init();
}

void GPIODriver::WritePin(GPIOPin pin, GPIOPinState pinState)
{
	HAL_GPIO_WritePin(pin.GetPort(),pin.GetPinNumber(),pinState);
}

const GPIOPin& GPIODriver::GetLedPin() const
{
	return LedPin;
}

const GPIOPin& GPIODriver::GetPSOPin() const
{
	return PSOPin;
}
