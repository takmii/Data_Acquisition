#include "adc.h"
#include "adc_settings.h"
#include "tim.h"

extern ADC_HandleTypeDef hadc1;


ADC_ChannelConfTypeDef sConfig1 = {0};
ADC_ChannelConfTypeDef sConfig2 = {0};

unsigned char readings_qtt=40;


unsigned short sensorData[sensor_qtt][sensor_buffer_size];

HAL_StatusTypeDef setADCChannel1(unsigned char channel){
	switch(channel) {
	       	case 0:
	            sConfig1.Channel = ADC_CHANNEL_0;
	            break;
	        case 1:
	            sConfig1.Channel = ADC_CHANNEL_1;
	            break;
	        case 2:
	            sConfig1.Channel = ADC_CHANNEL_2;
	            break;
	        case 3:
	            sConfig1.Channel = ADC_CHANNEL_3;
	            break;
	        case 4:
	            sConfig1.Channel = ADC_CHANNEL_4;
	            break;
	        case 5:
	            sConfig1.Channel = ADC_CHANNEL_5;
	            break;
	        case 6:
	            sConfig1.Channel = ADC_CHANNEL_6;
	            break;
	        case 7:
	            sConfig1.Channel = ADC_CHANNEL_7;
	            break;
	        case 8:
	            sConfig1.Channel = ADC_CHANNEL_8;
	            break;
	        case 9:
	            sConfig1.Channel = ADC_CHANNEL_9;
	            break;
	        case 10:
	            sConfig1.Channel = ADC_CHANNEL_10;
	            break;
	        case 11:
	            sConfig1.Channel = ADC_CHANNEL_11;
	            break;
	        case 12:
	            sConfig1.Channel = ADC_CHANNEL_12;
	            break;
	        case 13:
	            sConfig1.Channel = ADC_CHANNEL_13;
	            break;
	        case 14:
	            sConfig1.Channel = ADC_CHANNEL_14;
	            break;
	        case 15:
	            sConfig1.Channel = ADC_CHANNEL_15;
	            break;
	        case 16:
	            sConfig1.Channel = ADC_CHANNEL_16;
	            break;
	        case 17:
	            sConfig1.Channel = ADC_CHANNEL_17;
	            break;
	        default:
	            return HAL_ERROR; // Canal inválido
	    }

	    sConfig1.Rank = ADC_REGULAR_RANK_1;
	    sConfig1.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

	    return HAL_ADC_ConfigChannel(&hadc1, &sConfig1);

}

HAL_StatusTypeDef setADCChannel2(unsigned char channel){
	switch(channel) {
	        case 0:
	            sConfig2.Channel = ADC_CHANNEL_0;
	            break;
	        case 1:
	            sConfig2.Channel = ADC_CHANNEL_1;
	            break;
	        case 2:
	            sConfig2.Channel = ADC_CHANNEL_2;
	            break;
	        case 3:
	            sConfig2.Channel = ADC_CHANNEL_3;
	            break;
	        case 4:
	            sConfig2.Channel = ADC_CHANNEL_4;
	            break;
	        case 5:
	            sConfig2.Channel = ADC_CHANNEL_5;
	            break;
	        case 6:
	            sConfig2.Channel = ADC_CHANNEL_6;
	            break;
	        case 7:
	            sConfig2.Channel = ADC_CHANNEL_7;
	            break;
	        case 8:
	            sConfig2.Channel = ADC_CHANNEL_8;
	            break;
	        case 9:
	            sConfig2.Channel = ADC_CHANNEL_9;
	            break;
	        case 10:
	            sConfig2.Channel = ADC_CHANNEL_10;
	            break;
	        case 11:
	            sConfig2.Channel = ADC_CHANNEL_11;
	            break;
	        case 12:
	            sConfig2.Channel = ADC_CHANNEL_12;
	            break;
	        case 13:
	            sConfig2.Channel = ADC_CHANNEL_13;
	            break;
	        case 14:
	            sConfig2.Channel = ADC_CHANNEL_14;
	            break;
	        case 15:
	            sConfig2.Channel = ADC_CHANNEL_15;
	            break;
	        case 16:
	            sConfig2.Channel = ADC_CHANNEL_16;
	            break;
	        case 17:
	            sConfig2.Channel = ADC_CHANNEL_17;
	            break;
	        default:
	            return HAL_ERROR; // canal inválido
	    }

	    sConfig2.Rank = ADC_REGULAR_RANK_1;
	    sConfig2.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	    return HAL_ADC_ConfigChannel(&hadc2, &sConfig2);

}

uint16_t readADCValue1(unsigned char channelNumber) {

    if (setADCChannel1(channelNumber) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    delay_us(1);
    uint32_t sum=0;
    HAL_ADC_Start(&hadc1);
    for (uint8_t i = 0; i < readings_qtt; i++) {
    	if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK) {
    		HAL_ADC_Stop(&hadc1);
    		return 0xFFFF;  // erro
    	}
    	sum += HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
    return (uint16_t)(sum / readings_qtt);
}

uint16_t readADCValue2(unsigned char channelNumber) {
    if (setADCChannel2(channelNumber) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    delay_us(1);
    uint32_t sum=0;
    HAL_ADC_Start(&hadc2);
    for (uint8_t i = 0; i < readings_qtt; i++) {
        if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) != HAL_OK) {
            HAL_ADC_Stop(&hadc2);
            return 0xFFFF;
        }
        sum += HAL_ADC_GetValue(&hadc2);
    }
    HAL_ADC_Stop(&hadc2);
    return (uint16_t)(sum / readings_qtt);
}

uint16_t readSensor(unsigned char mux_pin, unsigned char index){
	unsigned char mux = mux_pin>>4;
	unsigned char porta  = mux_pin&0b1111;
	_Bool s0 = porta&0b0001;
	_Bool s1 = (porta&0b0010)>>1;
	_Bool s2 = (porta&0b0100)>>2;
	_Bool s3 = (porta&0b1000)>>3;

	HAL_GPIO_WritePin(EN1_GPIO, EN1_PIN, GPIO_PIN_SET); // EN1 (B4)
	HAL_GPIO_WritePin(EN2_GPIO, EN2_PIN, GPIO_PIN_SET); // EN2 (B3)
	HAL_GPIO_WritePin(EN3_GPIO, EN3_PIN, GPIO_PIN_SET); // EN3 (A15)

	HAL_GPIO_WritePin(S0_GPIO, S0_PIN, s0);
	HAL_GPIO_WritePin(S1_GPIO, S1_PIN, s1);
	HAL_GPIO_WritePin(S2_GPIO, S2_PIN, s2);
	HAL_GPIO_WritePin(S3_GPIO, S3_PIN, s3);

	uint16_t value;
	switch (mux) {
	        case 1:
	            HAL_GPIO_WritePin(EN1_GPIO, EN1_PIN, GPIO_PIN_RESET); // Ativa EN1 (B4)
	            value = readADCValue1(1);
	            break;
	        case 2:
	            HAL_GPIO_WritePin(EN2_GPIO, EN2_PIN, GPIO_PIN_RESET); // Ativa EN2 (B3)
	            value = readADCValue1(2);
	            break;
	        case 3:
	            HAL_GPIO_WritePin(EN3_GPIO, EN3_PIN, GPIO_PIN_RESET); // Ativa EN3 (A15)
	            value = readADCValue1(3);
	            break;
	        default:
	            break;
	    }
	return returnAvgData(value,index);
}

void delay_us(unsigned short us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    HAL_TIM_Base_Start(&htim1);

    while (__HAL_TIM_GET_COUNTER(&htim1) < us);

    HAL_TIM_Base_Stop(&htim1);
}

uint16_t returnAvgData(uint16_t data, uint8_t index){
	uint8_t weight = 3;
	uint32_t sum=data*weight;
  float factor = 0.95;
	uint8_t n = weight;
	for (uint8_t i=0;i<sensor_buffer_size;i++){
		if (sensorData[index][i]==0xFFFF){
			sensorData[index][i]=data;
			return sensorData[index][i];
		}
		else{
      if (sensorData[index][i]>(factor*data)&&sensorData[index][i]<((1+(1-factor))*data)){
			sum = sum+sensorData[index][i];
			n++;
      }
		}
	}
  for (uint8_t i=0;i<sensor_buffer_size-1;i++){
    sensorData[index][i]=sensorData[index][i+1];
  }
	sensorData[index][sensor_buffer_size-1]=sum/n;
	return sensorData[index][sensor_buffer_size-1];
}

void setAvgDataEmpty(){
	for (uint8_t i=0;i<sensor_qtt;i++){
		for (uint8_t j=0;j<sensor_buffer_size;j++){
			sensorData[i][j]=0xFFFF;
		}
	}
}





