/*
 * USBDriver.cpp
 *
 *  Created on: 18.10.2017
 *      Author: Karol
 */

#include "USBDriver.h"
#include "usbh_pipes.h"

USBDriver::USBDriver(USBH_HandleTypeDef* usbHostHandle,GPIODriver& gpioDriver) : gpioDriver(gpioDriver),
                                                                                 usbHostHandle(usbHostHandle)
{
	MX_USB_HOST_Init(); //init usb by hal driver
	TurnOnPSO();
}

void USBDriver::USBHostProcess()
{
	MX_USB_HOST_Process();
}

bool USBDriver::NewDeviceWasAttached()
{
	if (usbHostHandle->gState == HOST_DEV_ATTACHED)
	{
		return true;
	}
	return false;
}

bool USBDriver::DeviceWasDisconnected()
{
	if (usbHostHandle->gState == HOST_DEV_DISCONNECTED)
	{
		usbDeviceReady = false;
		return true;
	}
	return false;
}
void USBDriver::InitHost()
{
	while(1)
	{
		USBHostProcess(); //process incomming usb request
		if (usbHostHandle->gState == HOST_CHECK_CLASS) //when device class is assigned allocate pipe for bulk transer
		{
			AllocTransferPipe(); //if pipe correctly allocated break loop
			if(usbDeviceReady == true)
				return;

			//TODO implement protection for ending in infinite loop
		}
	}
}

void USBDriver::AllocTransferPipe()
{
	  inPipe = USBH_AllocPipe(usbHostHandle, USB_PIPE_NUMBER);
	  USBH_StatusTypeDef status = USBH_OpenPipe(usbHostHandle,
		                                        inPipe,
		                                        USB_PIPE_NUMBER,
												usbHostHandle->device.address,
												usbHostHandle->device.speed,
		                                        USB_EP_TYPE_BULK,
		                                        512);

	  status == USBH_OK ?  usbDeviceReady = true : usbDeviceReady = false;

}

USBH_StatusTypeDef USBDriver::ControlTransfer(uint8_t request_type,
										      uint8_t bRequest,
											  uint16_t wValue,
											  uint16_t wIndex,
											  unsigned char* data,
											  uint16_t wLength)
{
	usbHostHandle->Control.setup.b.bmRequestType = request_type;
	usbHostHandle->Control.setup.b.bRequest      = bRequest;
	usbHostHandle->Control.setup.b.wValue.w      = wValue;
	usbHostHandle->Control.setup.b.wIndex.w      = wIndex;
	usbHostHandle->Control.setup.b.wLength.w     = wLength;

	USBH_StatusTypeDef status;
    do {
        status = USBH_CtlReq(usbHostHandle, data, wLength);
    } while (status == USBH_BUSY);

    if (status != USBH_OK)  {
    	usbHostHandle->RequestState = CMD_SEND;
    }
    return status;
}

void USBDriver::TurnOnPSO()
{
	gpioDriver.WritePin(gpioDriver.GetPSOPin(),GPIO_PIN_RESET);
}

void USBDriver::TurnOfPSO()
{
	gpioDriver.WritePin(gpioDriver.GetPSOPin(),GPIO_PIN_SET);
}

