/*
 * LTDCDriver.h
 *
 *  Created on: 24.10.2017
 *      Author: Karol
 */

#ifndef DRIVERS_LTDCDRIVER_LTDCDRIVER_H_
#define DRIVERS_LTDCDRIVER_LTDCDRIVER_H_

#include "ltdc.h"

#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001

class LTDCDriver
{
public:
	LTDCDriver(	uint16_t screenWidth,
				uint16_t screenHeight,

				uint16_t vsync,
				uint16_t hsync,

				uint16_t horizontalFP,
				uint16_t horizontalBP,

				uint16_t verticalFP,
				uint16_t verticalBP	);

	uint16_t GetScreenWidth() const;
	uint16_t GetScreenHeight() const;

	void ConfigLayer(uint8_t layerIndex, uint32_t bufferAddress);

private:
	uint16_t screenWidth;
	uint16_t screenHeight;

	uint16_t vsync;
	uint16_t hsync;

	uint16_t horizontalFP;
	uint16_t horizontalBP;

	uint16_t verticalFP;
	uint16_t verticalBP;
};

#endif /* DRIVERS_LTDCDRIVER_LTDCDRIVER_H_ */
