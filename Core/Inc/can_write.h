#ifndef __CAN_WRITE_H__
#define __CAN_WRITE_H__

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef sendCANString(const char* message);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

