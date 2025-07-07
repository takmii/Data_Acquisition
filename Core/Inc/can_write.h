#ifndef __CAN_WRITE_H__
#define __CAN_WRITE_H__

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef sendCANString(const char* message);
HAL_StatusTypeDef sendCANData(unsigned char message[8], unsigned char CAN_ID, unsigned char CAN_DLC);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

