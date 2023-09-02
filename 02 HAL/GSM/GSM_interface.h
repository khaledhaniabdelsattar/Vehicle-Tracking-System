/*
 * GSM_interface.h
 *
 * Created: 8/20/2023 11:40:35 AM
 *  Author: MostafaElSadaad
 */ 


#ifndef GSM_INTERFACE_H_
#define GSM_INTERFACE_H_

#define MAX_URL_LENGTH				160

void GSM_GPRS_INIT();
void GSM_RESET();
void GSM_FireBase_URL_Init(u8* url);
u8	 GSM_SendCommand(u8* command,u8 waitchar);
void SendDataToFirebase(u8* URL,u8* payload);
void GSM_PrepareAndSendData(u8* URL, u8* timestamp, u8* lati, u8* longi,u8* speed, u8* status);

#endif /* GSM_INTERFACE_H_ */