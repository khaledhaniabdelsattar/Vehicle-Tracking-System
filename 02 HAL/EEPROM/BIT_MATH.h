

#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define SET_BIT(REG, BIT)	(REG |= (1<<BIT))								/* Sets any BIT in a Register */
#define CLR_BIT(REG, BIT)	(REG &= (~(1<<BIT)))							/* Clears any BIT in a Register */
#define TOG_BIT(REG, BIT)	(REG ^= (1<<BIT))								/* Toggles any BIT in a Register */
#define isBITSET(REG, BIT)	(REG & (1<<BIT))								/* Checks if any BIT in a Register is SET */
#define isBITCLR(REG, BIT)	(!(REG & (1<<BIT)))								/* Checks if any BIT in a Register is Cleared */
#define ROR(REG, BIT)		(REG = (REG >> BIT) | (REG << (8 - BIT)))		/* Rotates any Register right by BIT numbers */
#define ROL(REG, BIT)		(REG = (REG << BIT) | (REG >> (8 - BIT)))		/* Rotates any Register left by BIT numbers */

#endif