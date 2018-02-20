/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"

#include "BoardMenager.h"
#include "InterruptMenager.h"
#include "ADSBDecoder.h"
#include "GUI.h"
#include "FlightCotrolView.h"
#include "FlightControl.h"
#include "FlightControlControler.h"
#include "timers.h"

 BoardMenager boardMenager;


SemaphoreHandle_t xSemaphore = NULL;
QueueHandle_t messageQueue = NULL;
TimerHandle_t modelTimer = NULL;
TimerHandle_t radarRefreshTimer = NULL;



FlightCotrolView view;
FlightControl model;
FlightControlControler controler(model,view);

static uint32_t ticks = 0;

void USBMonitorTask(void*)
{
	USBDriver& usbDriverHandle = boardMenager.GetUSB();
	RTLSDR& rtlSdrHandle =  boardMenager.GetRTLSDR();
	//controler.NotifyDisconnected();
	while(1)
	{
		usbDriverHandle.USBHostProcess();
		if(usbDriverHandle.NewDeviceWasAttached() == true)
		{
			vTaskSuspendAll();
			usbDriverHandle.InitHost();
			rtlSdrHandle.OpenDevice(0,1090000000,2000000);
			//controler.NotifyConnected();
			xTaskResumeAll();
		}
		if(usbDriverHandle.DeviceWasDisconnected() == true)
		{
			//controler.NotifyDisconnected();
		}
		vTaskDelay(100);
	}
}
void RTLSDRDataAquisitionTask(void*)
{
	RTLSDR& rtlsdrHandle = boardMenager.GetRTLSDR();
	ADS_BDecoder decoder(messageQueue);

	while(1)
	{
		if( xSemaphoreTake( xSemaphore, 100) == pdTRUE )
		{
			decoder.ProcessRawSamples(rtlsdrHandle.GetRawSamplesFromBuffer());
			rtlsdrHandle.GetNewRawSamples();
		}
	}
}


void FlightControlerTask(void *)
{
	ADS_BMessage msg;
	while(1)
	{
		if(ticks > 0)
		{
			uint32_t ticksTmp = ticks;
			controler.UpdateTicksCount(ticksTmp);
			ticks -= ticksTmp;
			controler.UpdateView();
		}
		if(xQueueReceive(messageQueue,&msg,10) == pdTRUE)
		{
			controler.PassNewMessage(msg);
		}
	}
}



void GUITask(void*)
{

	while(1)
	{
		GUI_Exec();

		vTaskDelay(100);

	}
}


void vTimerCallback(TimerHandle_t xTimer)
{
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_15);
	ticks++;
}

void vTimer2Callback(TimerHandle_t xTimer)
{
	view.UpdateRadar();
}

int main(void)
{


  MX_I2C4_Init();
  MX_SDMMC1_SD_Init();
  MX_TIM4_Init();
  MX_USART6_UART_Init();
  MX_DMA2D_Init();
  MX_CRC_Init();

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_Delay(200);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);

  view.Init();
  GUI_Exec();

  xTaskCreate(USBMonitorTask          ,"task",1024,NULL,osPriorityHigh,NULL);
  xTaskCreate(GUITask          ,"hjhjk",1024,NULL,osPriorityNormal,NULL);
  xTaskCreate(RTLSDRDataAquisitionTask,"task3",2048,NULL,osPriorityHigh  ,NULL);
  xTaskCreate(FlightControlerTask,"dupa",2048,NULL,osPriorityNormal  ,NULL);
  xSemaphore = xSemaphoreCreateBinary();
  messageQueue = xQueueCreate(5,sizeof(ADS_BMessage));
  modelTimer= xTimerCreate("Timer",1000U,pdTRUE,NULL, vTimerCallback);
  xTimerStart(modelTimer,1000);
  radarRefreshTimer= xTimerCreate("Timer2",1000U,pdTRUE,NULL, vTimer2Callback);
  xTimerStart(radarRefreshTimer,1000);
  /* Start scheduler */


  osKernelStart();

  while (1)
  {


  }

}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}
#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

extern "C" void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
#if (USBH_USE_OS == 1)
  USBH_LL_NotifyURBChange(hhcd->pData);
#endif

  UNUSED(hhcd);
  UNUSED(chnum);

  if(boardMenager.GetRTLSDR().IsDeviceReady() == false) { return; }

  if (urb_state == URB_DONE)
  {
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR( xSemaphore , &xHigherPriorityTaskWoken );
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
  }
}
