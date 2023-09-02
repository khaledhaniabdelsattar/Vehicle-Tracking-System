


#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_


#include "TWI_Priv.h"



void TWI_voidInit(u8 TWI_address);
TWI_ERROR_STATUS TWI_StartCondition(void);
void TWI_voidStopCondition(void);
TWI_ERROR_STATUS TWI_RepeatedStartCondition (void);
TWI_ERROR_STATUS TWI_SendAddressWithWrite (u8 slaveAddress);
TWI_ERROR_STATUS TWI_SendAddressWithRead (u8 slaveAddress);
TWI_ERROR_STATUS TWI_SendByte (u8 data);
TWI_ERROR_STATUS TWI_ReadByte_NACK (u8 *data);
TWI_ERROR_STATUS TWI_ReadByte_ACK (u8 *data);

#endif /* TWI_INTERFACE_H_ */