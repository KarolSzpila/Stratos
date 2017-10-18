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


class InterruptMenager
{
public:

	InterruptMenager();

	void NotifyURBChange(void);
private:
};


#endif /* INTERRUPTMENAGER_H_ */
