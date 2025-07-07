#include "sensors.h"
#include "adc_settings.h"

// PINOS C1_6 - C1_12
// PINOS C1_18 - C1_24
// Dedicados para sensores da mega

// PINOS C2_1 - C2_3 e C2_13 - C2_15
// Sensor de marcha


const unsigned char Time_Mult = 5;

unsigned int Message_Debug_Time;
const unsigned short MESSAGE_DEBUG_REFRESH_RATE = 500;


const unsigned char Wheel_Angle = C1_1;
const unsigned char Acc_Pedal_Angle = C1_2;
const unsigned char Brake_Pedal_Angle = C1_3;
const unsigned char FR_Hall = C2_4;
const unsigned char FL_Hall = C2_5;
const unsigned char RR_Hall = C2_16;
const unsigned char RL_Hall = C2_17;
const unsigned char EBS_Sensor = C1_13;
const unsigned char Firewall_Temp = C1_14;
const unsigned char FR_Caliper_Pressure = C2_6;
const unsigned char FL_Caliper_Pressure = C2_7;
const unsigned char RR_Caliper_Pressure = C2_18;
const unsigned char RL_Caliper_Pressure = C2_19;
const unsigned char F_Brakeline_Pressure = C2_8;
const unsigned char R_Brakeline_Pressure = C2_20;
const unsigned char FR_Disk_Temp = C2_9;
const unsigned char FL_Disk_Temp = C2_10;
const unsigned char RR_Disk_Temp = C2_21;
const unsigned char RL_Disk_Temp = C2_22;
const unsigned char FR_Susp_Angle = C1_4;
const unsigned char FL_Susp_Angle = C1_5;
const unsigned char RR_Susp_Angle = C1_16;
const unsigned char RL_Susp_Angle = C1_17;
const unsigned char MAP_1_Pressure = C2_11;
const unsigned char MAP_2_Pressure = C2_23;
const unsigned char MAF_Flow = C1_15;
const unsigned char Cooling_In_Temp = C1_6;
const unsigned char Cooling_Out_Temp = C1_18;
const unsigned char Oil_Temp = C1_7;
const unsigned char Intercooler_Pressure = C2_12;
const unsigned char Fuel_Pressure = C2_24;

Sensor DATA_01;
Sensor DATA_02;
Sensor DATA_03;
Sensor DATA_04;
Sensor DATA_05;
Sensor DATA_06;
Sensor DATA_07;
Sensor DATA_08;
Sensor DATA_09;


HAL_StatusTypeDef setupSensors(){
	DATA_01.refresh_rate = 5*Time_Mult;
	DATA_01.id = 0x02;
	DATA_01.dlc = 5;

	DATA_02.refresh_rate = 5*Time_Mult;
	DATA_02.id = 0x03;
	DATA_02.dlc = 8;

	DATA_03.refresh_rate = 5*Time_Mult;
	DATA_03.id = 0x04;
	DATA_03.dlc = 8;

	DATA_04.refresh_rate = 50*Time_Mult;
	DATA_04.id = 0x05;
	DATA_04.dlc = 8;

	DATA_05.refresh_rate = 50*Time_Mult;
	DATA_05.id = 0x06;
	DATA_05.dlc = 6;

	DATA_06.refresh_rate = 5*Time_Mult;
	DATA_06.id = 0x07;
	DATA_06.dlc = 3;

	DATA_07.refresh_rate = 50*Time_Mult;
	DATA_07.id = 0x08;
	DATA_07.dlc = 6;

	DATA_08.refresh_rate = 50*Time_Mult;
	DATA_08.id = 0x09;
	DATA_08.dlc = 6;

	DATA_09.refresh_rate = 5*Time_Mult;
	DATA_09.id = 0x0A;
	DATA_09.dlc = 6;

	return HAL_OK;
}

unsigned short vBatValue(unsigned short ADC_Value){
	float vBat = ADC_Value*(VBAT_R1 + VBAT_R2)/VBAT_R2;
	unsigned short vBat0 = vBat/A_20V * 4095;
	return vBat0;
}

unsigned short vRefValue(unsigned short ADC_Value){
	float vRef = ADC_Value*(V_REF_R1 + V_REF_R2)/V_REF_R2;
	unsigned short vRef0 = vRef/A_5_5V * 4095;
	return vRef0;
}

unsigned short resistorValue(unsigned short ADC_Value,unsigned short vRef){
	if(ADC_Value==0){
		return 0;
	}
	unsigned short vRefProp = propVRef(vRef);
	float Rvalue = (((float)vRefProp/ADC_Value)* MUX2_R2) - MUX2_R1 - MUX2_R2;
	return (unsigned short)Rvalue;
}

unsigned short propVRef(unsigned short vRef){
	float prop = (float)vRef/4095;
	prop = prop * A_5_5V;
	return (unsigned short)prop;
}



