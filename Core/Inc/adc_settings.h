#ifndef __ADC_SETTINGS_H__
#define __ADC_SETTINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

extern ADC_ChannelConfTypeDef sConfig1;
extern ADC_ChannelConfTypeDef sConfig2;

HAL_StatusTypeDef setADCChannel1(unsigned char channel);
HAL_StatusTypeDef setADCChannel2(unsigned char channel);
uint16_t readADCValue1(unsigned char channelNumber);
uint16_t readADCValue2(unsigned char channelNumber);
uint16_t readSensor(unsigned char mux_pin,unsigned char index);
void delay_us(unsigned short us);
uint16_t returnAvgData(uint16_t data, uint8_t index);
void setAvgDataEmpty();

extern unsigned char readings_qtt;
#define sensor_qtt 37
#define sensor_buffer_size 10
extern unsigned short sensorData[sensor_qtt][sensor_buffer_size];

#define C1_1  0b101011
#define C1_2  0b101001
#define C1_3  0b100101
#define C1_4  0b100000
#define C1_5  0b100010
#define C1_6  0b011100
#define C1_7  0b011110
#define C1_8  0b011011
#define C1_9  0b011001
#define C1_10 0b010000
#define C1_11 0b010010
#define C1_12 0b010100
#define C1_13 0b101010
#define C1_14 0b101000
#define C1_15 0b100100
#define C1_16 0b100001
#define C1_17 0b100011
#define C1_18 0b011101
#define C1_19 0b011111
#define C1_20 0b011010
#define C1_21 0b011000
#define C1_22 0b010001
#define C1_23 0b010011
#define C1_24 0b010101

#define C2_4  0b111001
#define C2_5  0b111011
#define C2_6  0b111101
#define C2_7  0b111111
#define C2_8  0b110000
#define C2_9  0b110010
#define C2_10 0b110100
#define C2_11 0b101100
#define C2_12 0b101110
#define C2_16 0b111000
#define C2_17 0b111010
#define C2_18 0b111100
#define C2_19 0b111110
#define C2_20 0b110001
#define C2_21 0b110011
#define C2_22 0b110101
#define C2_23 0b101101
#define C2_24 0b101111

#define S0_PIN GPIO_PIN_7
#define S0_GPIO GPIOB

#define S1_PIN GPIO_PIN_8
#define S1_GPIO GPIOB

#define S2_PIN GPIO_PIN_5
#define S2_GPIO GPIOB

#define S3_PIN GPIO_PIN_6
#define S3_GPIO GPIOB

#define EN1_PIN GPIO_PIN_4
#define EN1_GPIO GPIOB

#define EN2_PIN GPIO_PIN_3
#define EN2_GPIO GPIOB

#define EN3_PIN GPIO_PIN_15
#define EN3_GPIO GPIOA

#define VREF_PIN 0
#define VBAT_PIN 4
#define GEAR_PIN 5
#define TEMP_PIN 6


#ifdef __cplusplus
}
#endif

#endif /* __ADC_SETTINGS_H__ */

