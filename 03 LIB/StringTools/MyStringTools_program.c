/*
 * MyStringTools_program.c
 *
 * Created: 8/23/2023 5:35:45 PM
 *  Author: MostafaElSadaad
 */ 
#include "STD_TYPES.h"


/****************************************************************/
/* Description    : Concatenates strings			  			*/
/*																*/
/*					Inputs : u8* destination, u8* source		*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  None										*/
/*     							 							    */
/****************************************************************/

void concatenateString(u8* destination, const u8* source) {
	while (*destination) {
		destination++;
	}
	while (*source) {
		*destination = *source;
		destination++;
		source++;
	}
	*destination = '\0';
}

