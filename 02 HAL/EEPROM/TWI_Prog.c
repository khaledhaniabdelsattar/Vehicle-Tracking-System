


#include "TWI_Conf.h"

void TWI_voidInit(u8 TWI_address)
{
#if (TWI_Mode == TWIMasterMode)

	/* Bitrate calculation */
	TWBR = ((((u32)F_CPU / F_SCL) - 16) / (2 * TWPreScaler));


	/* Setting prescaler (TWPS0 and TWPS1 Bits in TWSR Register) */
	#if (TWPreScaler == TWPrescaler_1)

		CLR_BIT(TWSR, TWPS0);
		CLR_BIT(TWSR, TWPS1);

	#elif (TWPreScaler == TWPrescaler_4)	

		SET_BIT(TWSR, TWPS0);
		CLR_BIT(TWSR, TWPS1);

	#elif (TWPreScaler == TWPrescaler_16)

		CLR_BIT(TWSR, TWPS0);
		SET_BIT(TWSR, TWPS1);

	#elif (TWPreScaler == TWPrescaler_64)

		SET_BIT(TWSR, TWPS0);
		SET_BIT(TWSR, TWPS1);

	#endif
	
#endif

	/* Setting address of device in case it's being addressed */
	TWAR = (TWI_address<<1);
	
	/* Enabling TWI */
	SET_BIT(TWCR, TWEN);
	
}


TWI_ERROR_STATUS TWI_StartCondition(void)
{
	
	TWI_ERROR_STATUS local_state = NO_ERROR;
	/* Set StartCondition BIT */
	/* Clear TWI Interrupt flag */
	CLR_BIT(TWCR, TWSTO);	
	TWCR |= (1<<TWSTA) | (1<<TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while (isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Start Condition has been transmitted */
	if ((TWSR & 0xf8) != TWI_START_COND_ACK)
	{
		local_state = START_ERROR;
	}
	else
	{
		/* Do Nothing */
	}
	
	return local_state;
}


void TWI_voidStopCondition(void)
{
	/* Set Stop Condition BIT */
	SET_BIT(TWCR, TWSTO);
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);
	
	while (isBITSET(TWCR, TWSTO))
	{
		/* Do Nothing */
	}
}

TWI_ERROR_STATUS TWI_RepeatedStartCondition (void)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Set StartCondition BIT */
	/* Clear TWI Interrupt flag */
	TWCR |= (1<<TWSTA) | (1<<TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while (isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Repeated Start Condition has been transmitted */
	if ((TWSR & 0xf8) != TWI_REPEATED_START_COND_ACK)
	{
		local_Error = REPEATED_START_ERROR;
	}
	else
	{
		/* Do Nothing*/
	}
	
	return local_Error;
}


TWI_ERROR_STATUS TWI_SendAddressWithWrite (u8 slaveAddress)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Sending SLA+W */
	TWDR = slaveAddress + 0;
	
	/* Clear Start Condition Bit */
	CLR_BIT(TWCR, TWSTA);
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while (isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Required ACK (or NACK depending on APP) has been received */
	if (((TWSR & 0xf8) != TWI_SEND_SLAVE_ADD_WRITE_ACK)/* && ((TWSR & 0xf8) != TWI_SEND_SLAVE_ADD_WRITE_NACK)*/)
	{
		local_Error = SLAVE_ADDRESS_WRITE_ERROR;
	}
	else
	{
		/* Do Nothing */
	}
	
	return local_Error;
}


TWI_ERROR_STATUS TWI_SendAddressWithRead (u8 slaveAddress)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Sending SLA+R */
	TWDR = slaveAddress + 1;
	
	/* Clear Start Condition Bit */
	CLR_BIT(TWCR, TWSTA);
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);

	/* wait for the flag to be set by any event that occurs */
	while (isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Required ACK (or NACK depending on APP) has been received */
	if (((TWSR & 0xf8) != TWI_SEND_SLAVE_ADD_READ_ACK)/* && ((TWSR & 0xf8) != TWI_SEND_SLAVE_ADD_READ_NACK)*/)
	{
		local_Error = SLAVE_ADDRESS_READ_ERROR;
	}
	else
	{
		/* Do Nothing */
	}
	
	return local_Error;
}


TWI_ERROR_STATUS TWI_SendByte (u8 data)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Move data to be transmitted to TWDR Register */
	TWDR = data;
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while (isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Required ACK (or NACK depending on APP) has been received */
	if (((TWSR & 0xf8) != TWI_SEND_BYTE_ACK)/* && ((TWSR & 0xf8) != TWI_SEND_BYTE_NACK)*/)
	{
		local_Error = SEND_BYTE_ERROR;
	}
	else
	{
		/* Do Nothing */
	}
	
	return local_Error;
}


TWI_ERROR_STATUS TWI_ReadByte_NACK (u8 * data)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Disable ACK */
	CLR_BIT(TWCR, TWEA);
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while(isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Required NACK has been received */
	if ((TWSR & 0xf8) != TWI_READ_BYTE_NACK)
	{
		local_Error = READ_BYTE_ERROR;
	}
	else
	{
		/* Move Received data to a variable */
		*data = TWDR;
	}
	
	return local_Error;
}

TWI_ERROR_STATUS TWI_ReadByte_ACK (u8 * data)
{
	TWI_ERROR_STATUS local_Error = NO_ERROR;
	
	/* Enable ACK */
	SET_BIT(TWCR, TWEA);
	
	/* Clear TWI Interrupt flag */
	SET_BIT(TWCR, TWINT);
	
	/* wait for the flag to be set by any event that occurs */
	while(isBITCLR(TWCR, TWINT))
	{
		/* Do Nothing */
	}
	
	/* Checking if the Required ACK has been received */
	if ((TWSR & 0xf8) != TWI_READ_BYTE_ACK)
	{
		local_Error = READ_BYTE_ERROR;
	}
	else
	{
		/* Move Received data to a variable */
		*data = TWDR;
	}
	
	return local_Error;
}