/*
 * FMCDriver.h
 *
 *  Created on: 23.10.2017
 *      Author: Karol
 */

#ifndef DRIVERS_FMCDRIVER_FMCDRIVER_H_
#define DRIVERS_FMCDRIVER_FMCDRIVER_H_

#include "fmc.h"

class FMCDriver
{
public:
	FMCDriver();
private:
	uint32_t startAdress;
};

#endif /* DRIVERS_FMCDRIVER_FMCDRIVER_H_ */
