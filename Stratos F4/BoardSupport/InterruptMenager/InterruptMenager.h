/*
 * InterruptMenager.h
 *
 *  Created on: 07.10.2017
 *      Author: Karol
 */

#ifndef INTERRUPTMENAGER_H_
#define INTERRUPTMENAGER_H_

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

enum InterruptID
{
	BluePushButtonInterrupt = 0,
	PinPushedInterrupt,
	USBDoneInterrupt,
};

class InterruptMenager
{
public:

	static int const semaphoreArraySize = 2;

	InterruptMenager();
	void NotifyInterruptOccured(InterruptID interruptID);
	SemaphoreHandle_t& GetPushButton();
	SemaphoreHandle_t& GetPin();
	bool InterruptOccured(InterruptID interruptID);

private:
	SemaphoreHandle_t* semaphoreArray;

	SemaphoreHandle_t Pin;
	SemaphoreHandle_t Button;
	void test(void) {;}
	friend void EXTI0_IRQHandler(void);
};


#endif /* INTERRUPTMENAGER_H_ */
