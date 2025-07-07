#include "adc.h"
#include "adc_settings.h"

extern ADC_HandleTypeDef hadc1;


ADC_ChannelConfTypeDef sConfig1 = {0};
ADC_ChannelConfTypeDef sConfig2 = {0};

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
	    sConfig1.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

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
	    sConfig2.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	    return HAL_ADC_ConfigChannel(&hadc2, &sConfig2);

}

uint16_t readADCValue1(unsigned char channelNumber) {
    if (setADCChannel1(channelNumber) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    return (uint16_t) HAL_ADC_GetValue(&hadc1);
}

uint16_t readADCValue2(unsigned char channelNumber) {
    if (setADCChannel2(channelNumber) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    HAL_ADC_Start(&hadc2);
    if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) != HAL_OK) {
        return 0xFFFF;  // erro
    }
    return (uint16_t) HAL_ADC_GetValue(&hadc2);
}

uint16_t readSensor(unsigned char mux_pin){
	unsigned char mux = mux_pin>>4;
	unsigned char porta  = mux_pin&0b1111;
	_Bool s0 = porta&0b0001;
	_Bool s1 = (porta&0b0010)>>1;
	_Bool s2 = (porta&0b0100)>>2;
	_Bool s3 = (porta&0b1000)>>3;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, s0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, s1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, s2);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, s3);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // EN2 (B3)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // EN1 (B4)
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); // EN3 (A15)

	uint16_t value;
	switch (mux) {
	        case 1:
	            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); // Ativa EN1 (B4)
	            value = readADCValue1(1);
	            break;
	        case 2:
	            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // Ativa EN2 (B3)
	            value = readADCValue1(2);
	            break;
	        case 3:
	            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); // Ativa EN3 (A15)
	            value = readADCValue1(3);
	            break;
	        default:
	            break;
	    }
	return value;
}




