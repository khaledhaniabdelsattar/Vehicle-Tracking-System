


#ifndef TWI_PRIV_H_
#define TWI_PRIV_H_

#include "Registers.h"
#include "BIT_MATH.h"
#include "STD_TYPES.h"

#define TWPrescaler_1	1
#define TWPrescaler_4	4
#define TWPrescaler_16	16
#define TWPrescaler_64	64


#define TWI_START_COND_ACK					0x08
#define TWI_REPEATED_START_COND_ACK			0x10
#define TWI_SEND_SLAVE_ADD_WRITE_ACK		0x18
#define TWI_SEND_SLAVE_ADD_WRITE_NACK		0x20
#define TWI_SEND_SLAVE_ADD_READ_ACK			0x40
#define TWI_SEND_SLAVE_ADD_READ_NACK		0x48
#define TWI_SEND_BYTE_ACK					0x28
#define TWI_SEND_BYTE_NACK					0x30
#define TWI_READ_BYTE_ACK					0x50
#define TWI_READ_BYTE_NACK					0x58


#define	NO_ERROR							0
#define	START_ERROR							1
#define	REPEATED_START_ERROR				2
#define	SLAVE_ADDRESS_WRITE_ERROR			3
#define	SLAVE_ADDRESS_READ_ERROR			4
#define	SEND_BYTE_ERROR						5
#define	READ_BYTE_ERROR						6

typedef u8 TWI_ERROR_STATUS;

#define TWIMasterMode	1
#define TWISlaveMode	2

#define TWIE	0
#define TWEN	2
#define TWWC	3
#define TWSTO	4
#define TWSTA	5
#define	TWEA	6
#define TWINT	7

#define TWPS0	0
#define TWPS1	1



#endif /* TWI_PRIV_H_ */