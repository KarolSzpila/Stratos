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
	USBDoneInterrupt = 0,
};

class InterruptMenager
{
public:

	static int const semaphoreArraySize = 1;

	InterruptMenager();
	void NotifyInterruptOccured(InterruptID interruptID);
	bool CheckInterruptOccured(InterruptID interruptID);
	SemaphoreHandle_t* GetSem();
private:
	SemaphoreHandle_t* semaphoreArray;
};


#endif /* INTERRUPTMENAGER_H_ */
