/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "can.h"
#include "can_write.h"
#include "adc_settings.h"
#include "sensors.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
HAL_StatusTypeDef hal_message;
/* USER CODE END Variables */
/* Definitions for sensor_Task */
osThreadId_t sensor_TaskHandle;
const osThreadAttr_t sensor_Task_attributes = {
  .name = "sensor_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void sensorTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	hal_message=setupSensors();

	Message_Debug_Time = HAL_GetTick();

	DATA_01.time = HAL_GetTick();
	DATA_02.time = HAL_GetTick();
	DATA_03.time = HAL_GetTick();
	DATA_04.time = HAL_GetTick();
	DATA_05.time = HAL_GetTick();
	DATA_06.time = HAL_GetTick();
	DATA_07.time = HAL_GetTick();
	DATA_08.time = HAL_GetTick();
	DATA_09.time = HAL_GetTick();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of sensor_Task */
  sensor_TaskHandle = osThreadNew(sensorTask, NULL, &sensor_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_sensorTask */
/**
* @brief Function implementing the sensor_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensorTask */
void sensorTask(void *argument)
{
  /* USER CODE BEGIN sensorTask */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint32_t RTOS_Time;
	uint16_t adc_value;
	char value[20];
  /* Infinite loop */
  for(;;)
  {
	RTOS_Time = HAL_GetTick();
	uint16_t v_ref = readADCValue1(VREF_PIN);
	uint16_t data;
	if (RTOS_Time - Message_Debug_Time >= MESSAGE_DEBUG_REFRESH_RATE) {
		Message_Debug_Time = RTOS_Time;
		adc_value=readADCValue1(0);
		//hal_message = sendCANString("Teste: ");
		sprintf(value, "%u", adc_value);
		hal_message = sendCANString(value);
		}

	if (RTOS_Time - DATA_01.time >= DATA_01.refresh_rate) {
	    DATA_01.time = RTOS_Time;

	    data = vBatValue(readADCValue1(VBAT_PIN));  // Tensao da Bateria
	    DATA_01.data[0] = data&0xFF;
	    DATA_01.data[1] = (data>>8)&0xF;
	    data = readADCValue1(TEMP_PIN);  // Sensor de Temperatura Interno
	    DATA_01.data[1]+=(data&0xF)<<4;
	    DATA_01.data[2] = (data>>4)&0xFF;
	    DATA_01.data[3] = v_ref&0xFF;    // Tensao Referencia
	    DATA_01.data[4] = (v_ref>>8)&0xF;
	    data = readADCValue1(VREF_PIN);  // Sensor de Marcha
	    if (data>3682){
	    	DATA_01.data[4] += (7<<4);
	    	}
	    else if (data>2995){
	    	DATA_01.data[4] += (1<<4);
	    }
	    else if (data>2432){
	    	DATA_01.data[4] += (2<<4);
	    }
	    else if (data>1852){
	    	DATA_01.data[4] += (3<<4);
	    }
	    else if (data>1249){
	    	DATA_01.data[4] += (4<<4);
	    }
	    else if (data>672){
	    	DATA_01.data[4] += (5<<4);
	    }
	    else {
	    	DATA_01.data[4] += (6<<4);
	    }
	    hal_message = sendCANData(DATA_01.data,DATA_01.id,DATA_01.dlc);
	}

	if (RTOS_Time - DATA_02.time >= DATA_02.refresh_rate) {
	    DATA_02.time = RTOS_Time;






	    DATA_02.data[0] = 240;
	    DATA_02.data[1] = 174;
	    DATA_02.data[2] = 213;
	    DATA_02.data[3] = 19;
	    DATA_02.data[4] = 32;
	    DATA_02.data[5] = 124;
	    DATA_02.data[6] = 158;
	    DATA_02.data[7] = 67;

	    //hal_message = sendCANData(DATA_02.data,DATA_02.id,DATA_02.dlc);
	}

	if (RTOS_Time - DATA_03.time >= DATA_03.refresh_rate) {
	    DATA_03.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_04.time >= DATA_04.refresh_rate) {
	    DATA_04.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_05.time >= DATA_05.refresh_rate) {
	    DATA_05.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_06.time >= DATA_06.refresh_rate) {
	    DATA_06.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_07.time >= DATA_07.refresh_rate) {
	    DATA_07.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_08.time >= DATA_08.refresh_rate) {
	    DATA_08.time = RTOS_Time;
	}

	if (RTOS_Time - DATA_09.time >= DATA_09.refresh_rate) {
	    DATA_09.time = RTOS_Time;
	}

	vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
  }
  /* USER CODE END sensorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

