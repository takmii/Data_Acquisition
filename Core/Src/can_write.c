#include "can.h"
#include "can_write.h"


const unsigned char DEBUG_ID = 0x7B;
const unsigned char DEBUG_DLC = 8;

HAL_StatusTypeDef sendCANString(const char* message){
	CAN_TxHeaderTypeDef txHeader;
	unsigned long txMailbox;
	unsigned char txData[8];
	
	HAL_StatusTypeDef status = HAL_OK;
	
	txHeader.StdId = DEBUG_ID;
    txHeader.ExtId = 0;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = DEBUG_DLC;
    txHeader.TransmitGlobalTime = DISABLE;
	
	unsigned char len = 0;
    while (1) {
        if (message[len] == '\0') {
        	  len++;
              break;
        }
        len++;
    }
	
	for (unsigned char i = 0; i < len; i += 8) {
        txHeader.DLC = (len - i > 8) ? 8 : (len - i);
        for (unsigned char j = 0; j < 8; j++) {
            txData[j] = 0;
        }
        for (unsigned char j = 0; j < txHeader.DLC; j++) {
            txData[j] = message[i + j];
        }
        status = HAL_CAN_AddTxMessage(&hcan, &txHeader, txData, &txMailbox);
        if (status != HAL_OK) {
            return status;
        }
        while (HAL_CAN_IsTxMessagePending(&hcan, txMailbox));
    }
	return HAL_OK;

}


HAL_StatusTypeDef sendCANData(unsigned char message[8], unsigned char CAN_ID, unsigned char CAN_DLC){
	CAN_TxHeaderTypeDef txHeader;
	unsigned long txMailbox;

	HAL_StatusTypeDef status = HAL_OK;

	txHeader.StdId = CAN_ID;
    txHeader.ExtId = 0;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = CAN_DLC;
    txHeader.TransmitGlobalTime = DISABLE;

    status = HAL_CAN_AddTxMessage(&hcan, &txHeader, message, &txMailbox);
    if (status != HAL_OK) {
        return status;
    }
    while (HAL_CAN_IsTxMessagePending(&hcan, txMailbox)){}
	return HAL_OK;
}
