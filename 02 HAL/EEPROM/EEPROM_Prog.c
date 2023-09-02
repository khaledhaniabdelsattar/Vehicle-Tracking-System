
#include "EEPROM_Interface.h"
#include "util/delay.h"

TWI_ERROR_STATUS EEPROM_SendStart()
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_StartCondition();
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_RepeatedStart()
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_RepeatedStartCondition();
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_SendDeviceAddress(u8 chipsel, u8 readwrite)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	u8 frame;
	
	
	/************************************************************************/
	/*                  Preparing Device Address frame r/w                  */
	/************************************************************************/
	if ( (chipsel <= 3) && (readwrite <= 1))
	{
		frame = (0xa0) | (chipsel<<1) | (readwrite<<0);
		
		if (readwrite == WRITE)
		{
			local_state = TWI_SendAddressWithWrite(frame);
		}
		else if (readwrite == READ)
		{
			local_state = TWI_SendAddressWithRead((frame - 1));
		}
	}
	else
	{
		/* returns an error in case of chipsel is more than 3 or if readwrite isn't WRITE nor READ */
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_SendWordAddress(u8 wordaddress)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_SendByte(wordaddress);
	
	return local_state;
}

u16 EEPROM_AddressPageWriteInc(u8 readwrite)
{
	static u16 ret	= 0;
	
	static u8 count = 0;
	
	static u8 readcount = 0;
	
	/************************************************************************/
	/*     return the address of the last written page in memory            */
	/************************************************************************/
	if ((readwrite == READ) && (readcount == 0))
	{
		if (count == 1)
		{
			return ret;
		}
		else if (count == 0)
		{
			return EEPROM_EMPTY_ERROR;
		}
	}
	else if ((readwrite == READ) && (readcount == 1))
	{
		readcount = 0;
		return 1008;
	}
	/************************************************************************/
	/*   Return the address of the next available page to be written to     */
	/************************************************************************/
	else if (readwrite == WRITE)
	{
		if (count == 0)
		{
			count++;
		}
		else if ((count > 0) && (ret >= 1008))
		{
			count		= 0;
			ret			= 0;
			readcount	= 1;
			return EEPROM_MEM_FULL_ERROR;
		}
		else if ((count > 0) && (ret < 1008))
		{
			ret += 16;
		}
		else
		{
			return EEPROM_MEM_ADD_INC_ERROR;
		}
	
		return ret;
	}
	else
	{
		return EEPROM_MEM_ADD_INC_ERROR;
	}
	return EEPROM_MEM_ADD_INC_ERROR;
}

u16 EEPROM_AddressPageReadInc()
{
	static u16 ret	= 0;
	
	static u8 count = 0;
	
	
	/************************************************************************/
	/*        Gets the address of the most recent page written to           */
	/************************************************************************/
	u16 lastWriteAddress = EEPROM_AddressPageWriteInc(READ);
	
	if (lastWriteAddress != EEPROM_EMPTY_ERROR)
	{
		/* Checks if no pages is written to */
		if (count == 0)
		{
			count++;
		}
		/* if next page to be read is NOT written to */
		else if ((count > 0) && (ret >= lastWriteAddress))
		{
			count	= 0;
			ret		= 0;
			return EEPROM_MEM_FULL_READ_ERROR;
		}
		/* if next page to be read is written to */
		else if ((count > 0) && (ret < lastWriteAddress))
		{
			ret += 16;
		}
		else
		{
			return EEPROM_MEM_ADD_INC_ERROR;
		}
	}
	else
	{
		return EEPROM_EMPTY_ERROR;
	}
	return ret;
}

TWI_ERROR_STATUS EEPROM_WriteByte(u8 data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_SendByte(data);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_WritePage(u8 * pdata)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	/* Writing to a page of 16 bytes */
	for (u8 i = 0; ((*(pdata + i) != '\0') && (i < 16)); i++)
	{
		if (local_state == NO_ERROR)
		{
			local_state = TWI_SendByte(*(pdata + i));
		}
		else
		{
			return local_state;
		}
	}
	
	return local_state;
}

void EEPROM_SendStop()
{
	TWI_voidStopCondition();
}

TWI_ERROR_STATUS EEPROM_ReadByteNACK(u8 * data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_ReadByte_NACK(data);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_ReadByteACK(u8 * data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;
	
	local_state = TWI_ReadByte_ACK(data);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_WriteByteRoutine(u8 chipsel, u8 wordaddress, u8 data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;

	/************************************************************************/
	/*                        Start Condition                               */
	/************************************************************************/
	local_state = TWI_StartCondition();
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, WRITE);
	}
	else
	{
		return EEPROM_START_ERROR;
	}
	
	/************************************************************************/
	/*                           Send Word Address                          */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendWordAddress(wordaddress);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                             Write Byte                               */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_WriteByte(data);
	}
	else
	{
		return EEPROM_WORD_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                          Stop Condition                              */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		EEPROM_SendStop();
	}
	else
	{
		return EEPROM_WRITE_BYTE_ERROR;
	}
	
	/* Delay to allow the stop condition to be executed */
	_delay_ms(10);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_WritePageRoutine(u8 * data)
{
	/************************************************************************/
	/*                        local variables                               */
	/************************************************************************/
	TWI_ERROR_STATUS local_state = NO_ERROR;
	u8 chipsel = 0, wordaddress = 0;

	/************************************************************************/
	/*                  increment previous address by page                  */
	/************************************************************************/
	u16 fulladd = EEPROM_AddressPageWriteInc(WRITE);
	
	/* Checks for a valid page address */
	if (fulladd % 16 == 0)
	{
		chipsel		= fulladd / 256;
		wordaddress = fulladd % 256;
	}
	else
	{
		/* if it's not it returns an error */
		return fulladd;
	}

	/************************************************************************/
	/*                        Start Condition                               */
	/************************************************************************/
	local_state = TWI_StartCondition();
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, WRITE);
	}
	else
	{
		return EEPROM_START_ERROR;
	}
	
	/************************************************************************/
	/*                           Send Word Address                          */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendWordAddress(wordaddress);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                             Write Page                               */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_WritePage(data);
	}
	else
	{
		return EEPROM_WORD_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                          Stop Condition                              */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		EEPROM_SendStop();
	}
	else
	{
		return EEPROM_WRITE_PAGE_ERROR;
	}
	
	/* Delay to allow the stop condition to be executed */
	_delay_ms(10);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_ReadCurrentByteRoutine(u8 chipsel, u8 * data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;

	/************************************************************************/
	/*                        Start Condition                               */
	/************************************************************************/
	local_state = TWI_StartCondition();
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, READ);
	}
	else
	{
		return EEPROM_START_ERROR;
	}
	
	/************************************************************************/
	/*                              Read Byte                               */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_ReadByteNACK(data);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                          Stop Condition                              */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		EEPROM_SendStop();
	}
	else
	{
		return EEPROM_READ_BYTE_ERROR;
	}
	
	/* Delay to allow the stop condition to be executed */
	_delay_ms(10);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_ReadRandomByteRoutine(u8 chipsel, u8 wordaddress, u8 * data)
{
	TWI_ERROR_STATUS local_state = NO_ERROR;

	/************************************************************************/
	/*                        Start Condition                               */
	/************************************************************************/
	local_state = TWI_StartCondition();
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, WRITE);
	}
	else
	{
		return EEPROM_START_ERROR;
	}
	
	/************************************************************************/
	/*                           Send Word Address                          */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendWordAddress(wordaddress);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                           Repeated Start                             */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_RepeatedStart();
	}
	else
	{
		return EEPROM_WORD_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, READ);
	}
	else
	{
		return EEPROM_REPEATED_START_ERROR;
	}
	
	/************************************************************************/
	/*                              Read Byte                               */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_ReadByteNACK(data);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                          Stop Condition                              */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		EEPROM_SendStop();
	}
	else
	{
		return EEPROM_READ_BYTE_ERROR;
	}
	
	/* Delay to allow the stop condition to be executed */
	_delay_ms(10);
	
	return local_state;
}

TWI_ERROR_STATUS EEPROM_ReadRandomPageRoutine(u8 * data)
{
	/************************************************************************/
	/*                        local variables                               */
	/************************************************************************/
	TWI_ERROR_STATUS local_state = NO_ERROR;
	u8 chipsel = 0, wordaddress = 0;

	/************************************************************************/
	/*                        Start Condition                               */
	/************************************************************************/
	u16 fulladd = EEPROM_AddressPageReadInc();
	
	/* Checks for a valid page address */
	if (fulladd % 16 == 0)
	{
		chipsel		= fulladd / 256;
		wordaddress = fulladd % 256;
	}
	else
	{
		return fulladd;
	}
	
	/************************************************************************/
	/*                  increment previous address by page                  */
	/************************************************************************/
	local_state = TWI_StartCondition();
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, WRITE);
	}
	else
	{
		return EEPROM_START_ERROR;
	}
	
	/************************************************************************/
	/*                           Send Word Address                          */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendWordAddress(wordaddress);
	}
	else
	{
		return EEPROM_DEVICE_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                           Repeated Start                             */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_RepeatedStart();
	}
	else
	{
		return EEPROM_WORD_ADDRESS_ERROR;
	}
	
	/************************************************************************/
	/*                        Send Device Address                           */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_SendDeviceAddress(chipsel, READ);
	}
	else
	{
		return EEPROM_REPEATED_START_ERROR;
	}
	
	/************************************************************************/
	/*                              Read Page                               */
	/************************************************************************/
	
	
	/* A 15-byte read with ACK followed by one byte read NOT ACK */
	for (u8 i = 0; i < 15; i++)
	{
		if (local_state == NO_ERROR)
		{
			local_state = EEPROM_ReadByteACK((data + i));
		}
		else
		{
			return local_state;
		}
	}
	
	if (local_state == NO_ERROR)
	{
		local_state = EEPROM_ReadByteNACK( (data+ 15) );
	}
	else
	{
		return EEPROM_READ_PAGE_ERROR;
	}
	
	/************************************************************************/
	/*                          Stop Condition                              */
	/************************************************************************/
	if (local_state == NO_ERROR)
	{
		EEPROM_SendStop();
	}
	else
	{
		return EEPROM_READ_BYTE_ERROR;
	}
	
	/* Delay to allow the stop condition to be executed */
	_delay_ms(10);
	
	return local_state;
}