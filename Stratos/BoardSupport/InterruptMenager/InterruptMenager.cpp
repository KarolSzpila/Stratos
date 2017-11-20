/*
 * InterruptMenager.cpp
 *
 *  Created on: 07.10.2017
 *      Author: Karol
 */

#include "InterruptMenager.h"



InterruptMenager::InterruptMenager()
{
	semaphoreArray = new SemaphoreHandle_t[semaphoreArraySize];

	for(uint8_t i = 0; i < semaphoreArraySize; i++)
	{
		semaphoreArray[i] = xSemaphoreCreateBinary();
	}

}

void InterruptMenager::NotifyInterruptOccured(InterruptID interruptID)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( semaphoreArray[interruptID] , &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

SemaphoreHandle_t* InterruptMenager::GetSem()
{
	return &(semaphoreArray[USBDoneInterrupt]);
}
bool InterruptMenager::CheckInterruptOccured(InterruptID interruptID)
{
	if( xSemaphoreTake( semaphoreArray[interruptID], portMAX_DELAY ) == pdTRUE )
	{
		return true;
	}
	else
	{
		return false;
	}
}


