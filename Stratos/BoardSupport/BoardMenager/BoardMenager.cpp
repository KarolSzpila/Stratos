/*
 * BoardMenager.cpp
 *
 *  Created on: 19.10.2017
 *      Author: Karol
 */

#include <BoardMenager.h>

extern "C"
{
	USBH_HandleTypeDef hUsbHostHS;
}

BoardMenager::BoardMenager() : rccDriver(),
							   gpioDriver(),
							   fmcDriver(),
							   ltdcDriver( 1024,600 , 10,10 , 210,100, 18,10),
							   usbDriverHandle(&hUsbHostHS,gpioDriver),
							   rtlSdrHandle(&usbDriverHandle)
{
}

