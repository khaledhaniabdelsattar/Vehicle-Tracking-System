/*
 * GSM_program.c
 *
 * Created: 8/23/2023 5:08:15 PM
 *  Author: MostafaElSadaad
 */ 
#include "STD_TYPES.h"
#include "MyStringTools.h"
#include "GSM_config.h"
#include "GSM_interface.h"
#include "LCD_interface.h"
#include "UART_interface.h"
#define  F_CPU 16000000UL
#include <util/delay.h>



u8     test[5]           =   "AT\r\n";
u8     SetCToGPRS[39]    =   "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
u8     SetAPNCfg[65]     =   "AT+SAPBR=3,1,\"APN\",\"internet.etisalat\"\r\n";
u8     Connect[18]       =   "AT+SAPBR=1,1\r\n";
u8	   EnableHTTP[16]	 =   "AT+HTTPINIT\r\n"; 
u8	   EnableSSL[15]	 =   "AT+HTTPSSL=1\r\n";
u8	   BearerProfID[25]	 =   "AT+HTTPPARA=\"CID\",1\r\n";
//u8	   Data[120]	     =   "{\"fields\": {\"key\": {\"stringValue\": \"key\"}, \"value\": {\"stringValue\": \"1691123033,29.97177,30.94329\"}}}\r\n";
u8	   Redirect[25]		 =   "AT+HTTPPARA=\"REDIR\",1\r\n";
u8	   DataType[50]		 =   "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n";
u8	   datalength[25]	 =   "AT+HTTPDATA=182,15000\r\n";//115
u8	   PostRequest[25]   =   "AT+HTTPACTION=1\r\n";
u8	   HTTPTerm[25]      =   "AT+HTTPTERM\r\n";
u8	   NetworkCheck[15]  =   "AT+CREG?\r\n";
u8	   AirPModeOn[15]    =   "AT+CFUN=0\r\n";
u8	   AirPModeOff[15]   =   "AT+CFUN=1\r\n";






/****************************************************************/
/* Description    : Sends certain data to database  			*/
/*																*/
/*					Inputs : u8* URL, u8* data					*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after GPRS		*/
/*     				 initialized 							    */
/****************************************************************/

void SendDataToFirebase(u8* URL,u8* payload){
	GSM_SendCommand(EnableHTTP,'K');
	GSM_SendCommand(EnableSSL,'K');
	GSM_SendCommand(BearerProfID,'K');
	GSM_SendCommand(URL,'K');
	GSM_SendCommand(Redirect,'K');
	GSM_SendCommand(DataType,'K');
	GSM_SendCommand(datalength,'D');
	_delay_ms(3000);
	GSM_SendCommand(payload,'K');
	_delay_ms(500);
	GSM_SendCommand(PostRequest,',');
	_delay_ms(1000);
	lcdWriteText("postdone");
	GSM_SendCommand(HTTPTerm,'K');
}




/****************************************************************/
/* Description    : Initializes GSM into GPRS Mode				*/
/*																*/
/*					Inputs : void								*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after UART		*/
/*     				 initialized 							    */
/****************************************************************/


void GSM_GPRS_INIT(){
		//GSM_RESET();
		GSM_SendCommand(SetCToGPRS,'K');
		GSM_SendCommand(SetAPNCfg,'K');
		GSM_SendCommand(Connect,'K');
}


/****************************************************************/
/* Description    : Resets GSM by airplane mode	    			*/
/*																*/
/*					Inputs : u8* URL(out), u8* data				*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after UART		*/
/*     				 initialized 							    */
/****************************************************************/

void GSM_RESET(){
	GSM_SendCommand(AirPModeOn,'K');
	GSM_SendCommand(AirPModeOff,'l');
}


/****************************************************************/
/* Description    : makes firebase API URL			  			*/
/*																*/
/*					Inputs : u8* URL (out)						*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  None										*/
/*     							 							    */
/****************************************************************/

void GSM_FireBase_URL_Init(u8* url){
		//u8 url[MAX_URL_LENGTH] = {0};
		const u8 FIREBASE_PROJECT_ID[20] = _FIREBASE_PROJECT_ID;
		const u8 FIREBASE_COLLECTION[20] = _FIREBASE_COLLECTION;
		const u8 FIREBASE_API_KEY[40]	 = _FIREBASE_API_KEY;
		u8 url_command[25]		= "AT+HTTPPARA=\"URL\",\"";
		u8 base_url[46]			= "https://firestore.googleapis.com/v1/projects/";
		u8 middle_url[32]		= "/databases/(default)/documents/";
		u8 key_url[6]			= "?key=";
		u8 url_command_f[10]	= "\"\r\n";

		// Concatenate the URL Command
		concatenateString(url, url_command);
		// Concatenate the Base URL
		concatenateString(url, base_url);

		// Concatenate the project ID
		concatenateString(url, FIREBASE_PROJECT_ID);

		// Concatenate the middle URL
		concatenateString(url, middle_url);

		// Concatenate the collection
		concatenateString(url, FIREBASE_COLLECTION);

		// Concatenate the key URL
		concatenateString(url, key_url);

		// Concatenate the API key
		concatenateString(url, FIREBASE_API_KEY);
		
		// Concatenate the API key
		concatenateString(url, url_command_f);

	
}

/****************************************************************/
/* Description    : Sends gps data to database					*/
/*																*/
/*		 Inputs : u8* URL, u8* timestamp,u8* latitude,			*/
/*		 u8* longitude, u8* speed, u8* status					*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after UART		*/
/*     				 initialized 							    */
/****************************************************************/


void GSM_PrepareAndSendData(u8* URL, u8* timestamp, u8* lati, u8* longi,u8* speed, u8* status){ // 200 {"fields":{"Latitude":{"doubleValue":29.97171},"Longitude":{"doubleValue":30.94321},"Speed":{"integerValue":60},"Status":{"integerValue":3},"TimeStamp":{"integerValue":1691123035}}}
	
		const u8 JsonBase[45] = "{\"fields\":{\"Latitude\":{\"doubleValue\":";
		const u8 Jsonlongi[35] = "},\"Longitude\":{\"doubleValue\":";
		const u8 JsonSpeed[32] = "},\"Speed\":{\"integerValue\":";
		const u8 JsonStatus[32] =	"},\"Status\":{\"integerValue\":";
		const u8 JsonTimestamp[35] = "},\"TimeStamp\":{\"integerValue\":";
		const u8 JsonEnding[8] =  "}}}\r\n";
		u8 payload[200] = {0};
			
		concatenateString(payload, JsonBase);
		concatenateString(payload, lati);
		concatenateString(payload, Jsonlongi);
		concatenateString(payload, longi);
		concatenateString(payload, JsonSpeed);
		concatenateString(payload, speed);
		concatenateString(payload, JsonStatus);
		concatenateString(payload, status);
		concatenateString(payload, JsonTimestamp);
		concatenateString(payload, timestamp);
		concatenateString(payload, JsonEnding);
		LCD_CLR();
		lcdGoToXY(0,0);
		lcdWriteText(payload);
		
		
		GSM_SendCommand(EnableHTTP,'K');
		GSM_SendCommand(EnableSSL,'K');
		GSM_SendCommand(BearerProfID,'K');
		GSM_SendCommand(URL,'K');
		GSM_SendCommand(Redirect,'K');
		GSM_SendCommand(DataType,'K');
		GSM_SendCommand(datalength,'D');
		_delay_ms(4000);
		GSM_SendCommand(payload,'K');
		_delay_ms(1000);
		GSM_SendCommand(PostRequest,',');
		_delay_ms(1000);
		lcdWriteText("postdone");
		GSM_SendCommand(HTTPTerm,'K');
	
}

/****************************************************************/
/* Description    : Sends command to gsm (for portability)  	*/
/*																*/
/*					Inputs : u8* command, u8* wait character	*/
/*					Return : u8 error status		 			*/
/****************************************************************/
/* Pre_condition  :  this function must be used after UART		*/
/*     				 initialized 							    */
/****************************************************************/

u8 GSM_SendCommand(u8* command,u8 waitchar){
		UART_SendString(command);
		u8 tempp = UART_RxChar();
		while( tempp != waitchar)
		{
			tempp = UART_RxChar();
		}
		return tempp;
}