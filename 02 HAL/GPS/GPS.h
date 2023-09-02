/*
 * GPS.h
 *
 *  Created on: ٠٤‏/٠٨‏/٢٠٢٣
 *      Author: houras
 */

#ifndef GPS_MODULE_GPS_H_
#define GPS_MODULE_GPS_H_


#define GPS_Array_Size	149
#define GPS_GPGGA_Size	72

typedef struct{
	u8 Data		[GPS_Array_Size];
	u8 GPGGA	[GPS_GPGGA_Size];
	u8 Longitude	[9];
	u8 Latitude	[10];
	u8 Altitude	[6];
	u8 Time		[12];
	u8 Satellite_No	[3];
	u8 HDOP		[5];
}GPS_Frames;


void	GPS_Data	( void );
void	GPS_GPGGA	( void );
void	GPS_Update	( void );
u8*	GPS_Longitude	( void );
u8*	GPS_Latitude	( void );
u8*	GPS_Altitude	( void );
u8*	GPS_Time	( void );
u8*	GPS_Satellite_No( void );
u8*	GPS_HDOP	( void );


#endif /* GPS_MODULE_GPS_H_ */
