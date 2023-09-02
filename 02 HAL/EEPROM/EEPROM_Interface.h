


#ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

#include "EEPROM_Priv.h"

#define WRITE	0
#define READ	1

TWI_ERROR_STATUS EEPROM_WriteByteRoutine(u8 chipsel, u8 wordaddress, u8 data);
TWI_ERROR_STATUS EEPROM_WritePageRoutine(u8 * data);
TWI_ERROR_STATUS EEPROM_ReadCurrentByteRoutine(u8 chipsel, u8 * data);
TWI_ERROR_STATUS EEPROM_ReadRandomByteRoutine(u8 chipsel, u8 wordaddress, u8 * data);
TWI_ERROR_STATUS EEPROM_ReadRandomPageRoutine(u8 * data);




#endif /* EEPROM_INTERFACE_H_ */