/*
 * GPIODriver.h
 *
 *  Created on: 07.10.2017
 *      Author: Karol
 */

#ifndef DRIVERS_GPIODRIVER_GPIODRIVER_H_
#define DRIVERS_GPIODRIVER_GPIODRIVER_H_

#include "GPIOPin.h"
#include "gpio.h"

class GPIODriver
{
public:
	GPIODriver();

	void WritePin(GPIOPin,GPIOPinState);
	GPIOPinState ReadPin(GPIOPin);

	const GPIOPin& GetLedPin() const;
	const GPIOPin& GetPSOPin() const;
private:
	GPIOPin LedPin;
	GPIOPin PSOPin;
};

#endif /* DRIVERS_GPIODRIVER_GPIODRIVER_H_ */
