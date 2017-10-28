/*
 * GPIOPin.h
 *
 *  Created on: 07.10.2017
 *      Author: Karol
 */

#ifndef DRIVERS_GPIODRIVER_GPIOPIN_H_
#define DRIVERS_GPIODRIVER_GPIOPIN_H_

#include "gpio.h"

typedef GPIO_PinState GPIOPinState;

class GPIOPin
{
public:
	GPIOPin(GPIO_TypeDef* GPIO_Port, const uint16_t GPIO_Pin) : GPIOPort(GPIO_Port), GPIOPinNumber(GPIO_Pin) {}

	uint16_t GetPinNumber() const { return GPIOPinNumber;}
	GPIO_TypeDef* GetPort() const { return GPIOPort;}

private:
	GPIO_TypeDef* GPIOPort;
	uint16_t GPIOPinNumber;
};



#endif /* DRIVERS_GPIODRIVER_GPIOPIN_H_ */
