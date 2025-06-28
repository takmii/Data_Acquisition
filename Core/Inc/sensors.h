#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef setupSensors();

#define V_4095 3.3
#define A_20V 24818
#define A_5_5V 6825

#define V_REF_R1 1520
#define V_REF_R2 1780

#define MUX1_R1 2200
#define MUX1_R2 3890

#define MUX2_R1 2200
#define MUX2_R2 3900

#define MUX3_R1 2200
#define MUX3_R2 3890

#define VBAT_R1 20000
#define VBAT_R2 3900

unsigned short vBatValue(unsigned short ADC_Value);

extern unsigned int Message_Debug_Time;
extern const unsigned short MESSAGE_DEBUG_REFRESH_RATE;


extern const unsigned char Wheel_Angle;
extern const unsigned char Acc_Pedal_Angle;
extern const unsigned char Brake_Pedal_Angle;
extern const unsigned char FR_Hall;
extern const unsigned char FL_Hall;
extern const unsigned char RR_Hall;
extern const unsigned char RL_Hall;
extern const unsigned char EBS_Sensor;
extern const unsigned char Firewall_Temp;
extern const unsigned char FR_Caliper_Pressure;
extern const unsigned char FL_Caliper_Pressure;
extern const unsigned char RR_Caliper_Pressure;
extern const unsigned char RL_Caliper_Pressure;
extern const unsigned char F_Brakeline_Pressure;
extern const unsigned char R_Brakeline_Pressure;
extern const unsigned char FR_Disk_Temp;
extern const unsigned char FL_Disk_Temp;
extern const unsigned char RR_Disk_Temp;
extern const unsigned char RL_Disk_Temp;
extern const unsigned char FR_Susp_Angle;
extern const unsigned char FL_Susp_Angle;
extern const unsigned char RR_Susp_Angle;
extern const unsigned char RL_Susp_Angle;
extern const unsigned char MAP_1_Pressure;
extern const unsigned char MAP_2_Pressure;
extern const unsigned char MAF_Flow;
extern const unsigned char Cooling_In_Temp;
extern const unsigned char Cooling_Out_Temp;
extern const unsigned char Intercooler_Pressure;
extern const unsigned char Fuel_Pressure;



typedef struct {
    unsigned int time;
    unsigned char refresh_rate;
    unsigned char id;
    unsigned char dlc;
    unsigned char data[8];
} Sensor;

extern Sensor DATA_01;
extern Sensor DATA_02;
extern Sensor DATA_03;
extern Sensor DATA_04;
extern Sensor DATA_05;
extern Sensor DATA_06;
extern Sensor DATA_07;
extern Sensor DATA_08;
extern Sensor DATA_09;



#ifdef __cplusplus
}
#endif

#endif /* __SENSORS_H__ */

