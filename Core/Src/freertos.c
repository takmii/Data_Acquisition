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
/* Definitions for CAN_Task */
osThreadId_t CAN_TaskHandle;
const osThreadAttr_t CAN_Task_attributes = {
  .name = "CAN_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void sensorTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	setAvgDataEmpty();
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

  /* creation of CAN_Task */
  CAN_TaskHandle = osThreadNew(StartTask02, NULL, &CAN_Task_attributes);

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
	uint16_t v_ref = vRefValue(returnAvgData(readADCValue1(VREF_PIN),V_Ref_index));
	uint16_t data;
	if (RTOS_Time - Message_Debug_Time >= MESSAGE_DEBUG_REFRESH_RATE) {
		Message_Debug_Time = RTOS_Time;
		adc_value=resistorValue(readSensor(C1_1,Teste_index),v_ref);
		//adc_value=readSensor(C1_1);
		//hal_message = sendCANString("Teste: ");
		sprintf(value, "%u$", adc_value);
		//sprintf(value, "%u$", v_ref);
		hal_message = sendCANString(value);
		}

	if (RTOS_Time - DATA_01.time >= DATA_01.refresh_rate) {
	    DATA_01.time = RTOS_Time;

	    data = vBatValue(returnAvgData(readADCValue2(VBAT_PIN),V_Bat_index));  // Tensao da Bateria
	    DATA_01.data[0] = data&0xFF;
	    DATA_01.data[1] = (data>>8)&0x0F;
	    data = readADCValue2(returnAvgData(TEMP_PIN,Temp_index));  // Sensor de Temperatura Interno
	    DATA_01.data[1]|=(data&0x0F)<<4;
	    DATA_01.data[2] = (data>>4)&0xFF;
	    DATA_01.data[3] = v_ref&0xFF;    // Tensao Referencia
	    DATA_01.data[4] = (v_ref>>8)&0x0F;
	    data = readADCValue2(returnAvgData(GEAR_PIN,Gear_index));  // Sensor de Marcha
	    if (data>3682){
	    	DATA_01.data[4] |= (7<<4);
	    	}
	    else if (data>2995){
	    	DATA_01.data[4] |= (1<<4);
	    }
	    else if (data>2432){
	    	DATA_01.data[4] |= (2<<4);
	    }
	    else if (data>1852){
	    	DATA_01.data[4] |= (3<<4);
	    }
	    else if (data>1249){
	    	DATA_01.data[4] |= (4<<4);
	    }
	    else if (data>672){
	    	DATA_01.data[4] |= (5<<4);
	    }
	    else {
	    	DATA_01.data[4] |= (6<<4);
	    }
	    hal_message = sendCANData(DATA_01.data,DATA_01.id,DATA_01.dlc);
	}

	if (RTOS_Time - DATA_02.time >= DATA_02.refresh_rate) {
	    DATA_02.time = RTOS_Time;

	    data = readSensor(FR_Susp_Angle,FR_Susp_Angle_index);
	    DATA_02.data[0] = data&0xFF;;
	    DATA_02.data[1] = (data>>8)&0xF;

	    data = readSensor(FL_Susp_Angle,FL_Susp_Angle_index);
	    DATA_02.data[1] |= (data&0xF)<<4;
	    DATA_02.data[2] = (data>>4)&0xFF;

	    data = readSensor(RR_Susp_Angle,RR_Susp_Angle_index);
	    DATA_02.data[3] = data&0xFF;;
	    DATA_02.data[4] = (data>>8)&0xF;

	    data = readSensor(RL_Susp_Angle,RL_Susp_Angle_index);
	    DATA_02.data[4] |= (data&0xF)<<4;
	    DATA_02.data[5] = (data>>4)&0xFF;

	    data = readSensor(Wheel_Angle,Wheel_Angle_index);
	    DATA_02.data[6] = data&0xFF;;
	    DATA_02.data[7] = (data>>8)&0xF;

	    hal_message = sendCANData(DATA_02.data,DATA_02.id,DATA_02.dlc);
	}

	if (RTOS_Time - DATA_03.time >= DATA_03.refresh_rate) {
	    DATA_03.time = RTOS_Time;

	    data = readSensor(FR_Hall,FR_Hall_index);
	    DATA_03.data[0] = data&0xFF;;
	    DATA_03.data[1] = (data>>8)&0xF;

	    data = readSensor(FL_Hall,FL_Hall_index);
	    DATA_03.data[1] |= (data&0xF)<<4;
	    DATA_03.data[2] = (data>>4)&0xFF;

	    data = readSensor(RR_Hall,FL_Hall_index);
	    DATA_03.data[3] = data&0xFF;;
	    DATA_03.data[4] = (data>>8)&0xF;

	    data = readSensor(RL_Hall,RL_Hall_index);
	    DATA_03.data[4] |= (data&0xF)<<4;
	    DATA_03.data[5] = (data>>4)&0xFF;

	    hal_message = sendCANData(DATA_03.data,DATA_03.id,DATA_03.dlc);
	}

	/*if (RTOS_Time - DATA_04.time >= DATA_04.refresh_rate) {
	    DATA_04.time = RTOS_Time;

	    data = readSensor(FR_Disk_Temp,FR_Disk_Temp_index);
	    DATA_04.data[0] = data&0xFF;;
	    DATA_04.data[1] = (data>>8)&0xF;

	    data = readSensor(FL_Disk_Temp,FL_Disk_Temp_index);
	    DATA_04.data[1] |= (data&0xF)<<4;
	    DATA_04.data[2] = (data>>4)&0xFF;

	    data = readSensor(RR_Disk_Temp,RR_Disk_Temp_index);
	    DATA_04.data[3] = data&0xFF;;
	    DATA_04.data[4] = (data>>8)&0xF;

	    data = readSensor(RL_Disk_Temp,RL_Disk_Temp_index);
	    DATA_04.data[4] |= (data&0xF)<<4;
	    DATA_04.data[5] = (data>>4)&0xFF;

	    data = readSensor(Oil_Temp,Oil_Temp_index);
	    DATA_04.data[6] = data&0xFF;;
	    DATA_04.data[7] = (data>>8)&0xF;

	    hal_message = sendCANData(DATA_04.data,DATA_04.id,DATA_04.dlc);
	}

	if (RTOS_Time - DATA_05.time >= DATA_05.refresh_rate) {
	    DATA_05.time = RTOS_Time;

	    data = readSensor(FR_Caliper_Pressure);
	    DATA_05.data[0] = data&0xFF;;
	    DATA_05.data[1] = (data>>8)&0xF;

	    data = readSensor(FL_Caliper_Pressure);
	    DATA_05.data[1] |= (data&0xF)<<4;
	    DATA_05.data[2] = (data>>4)&0xFF;

	    data = readSensor(RR_Caliper_Pressure);
	    DATA_05.data[3] = data&0xFF;;
	    DATA_05.data[4] = (data>>8)&0xF;

	    data = readSensor(RL_Caliper_Pressure);
	    DATA_05.data[4] |= (data&0xF)<<4;
	    DATA_05.data[5] = (data>>4)&0xFF;

	    hal_message = sendCANData(DATA_05.data,DATA_05.id,DATA_05.dlc);
	}*/

	if (RTOS_Time - DATA_06.time >= DATA_06.refresh_rate) {
	    DATA_06.time = RTOS_Time;

	    /*data = readSensor(Acc_Pedal_Angle,Acc_Pedal_Angle_index);
	    DATA_06.data[0] = data&0xFF;;
	    DATA_06.data[1] = (data>>8)&0xF;

	    data = readSensor(Brake_Pedal_Angle,Brake_Pedal_Angle_index);
	    DATA_06.data[1] |= (data&0xF)<<4;
	    DATA_06.data[2] = (data>>4)&0xFF;*/

	    data = readSensor(F_Brakeline_Pressure,F_Brakeline_Pressure_index);
	    DATA_06.data[3] = data&0xFF;;
	    DATA_06.data[4] = (data>>8)&0xF;

	    data = readSensor(R_Brakeline_Pressure,R_Brakeline_Pressure_index);
	    DATA_06.data[4] |= (data&0xF)<<4;
	    DATA_06.data[5] = (data>>4)&0xFF;

	    hal_message = sendCANData(DATA_06.data,DATA_06.id,DATA_06.dlc);
	}

	/*if (RTOS_Time - DATA_07.time >= DATA_07.refresh_rate) {
	    DATA_07.time = RTOS_Time;

	    data = readSensor(Fuel_Temp,Fuel_Temp_index);
	    DATA_07.data[0] = data&0xFF;;
	    DATA_07.data[1] = (data>>8)&0xF;

	    data = readSensor(Intercooler_Temp,Intercooler_Temp_index);
	    DATA_07.data[1] |= (data&0xF)<<4;
	    DATA_07.data[2] = (data>>4)&0xFF;

	    data = readSensor(Firewall_Temp,Firewall_Temp_index);
	    DATA_07.data[3] = data&0xFF;
	    DATA_07.data[4] = (data>>8)&0xF;

	    data = readSensor(Intercooler_Pressure,Intercooler_Pressure_index);
	    DATA_07.data[4] |= (data&0xF)<<4;
	    DATA_07.data[5] = (data>>4)&0xFF;



	    hal_message = sendCANData(DATA_07.data,DATA_07.id,DATA_07.dlc);
	}

	if (RTOS_Time - DATA_08.time >= DATA_08.refresh_rate) {
	    DATA_08.time = RTOS_Time;


	    data = readSensor(Cooling_In_Temp,Cooling_In_Temp_index);
	    DATA_08.data[0] = data&0xFF;;
	    DATA_08.data[1] = (data>>8)&0xF;

	    data = readSensor(Cooling_Out_Temp,Cooling_Out_Temp_index);
	    DATA_08.data[1] |= (data&0xF)<<4;
	    DATA_08.data[2] = (data>>4)&0xFF;

	    data = readSensor(,);
	    DATA_08.data[3] = data&0xFF;
	    DATA_08.data[4] = (data>>8)&0xF;

	    data = readSensor(,);
	    DATA_08.data[4] |= (data&0xF)<<4;
	    DATA_08.data[5] = (data>>4)&0xFF;



	    hal_message = sendCANData(DATA_08.data,DATA_08.id,DATA_08.dlc);
	}

	if (RTOS_Time - DATA_09.time >= DATA_09.refresh_rate) {
	    DATA_09.time = RTOS_Time;
	}*/


	if (RTOS_Time - BUFFER_ACK.time >= BUFFER_ACK.refresh_rate) {
		BUFFER_ACK.time = RTOS_Time;
		BUFFER_ACK.data[0]='1';
		hal_message = sendCANData(BUFFER_ACK.data,BUFFER_ACK.id,BUFFER_ACK.dlc);
	}
	vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Time_Mult));
  }
  /* USER CODE END sensorTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the CAN_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

