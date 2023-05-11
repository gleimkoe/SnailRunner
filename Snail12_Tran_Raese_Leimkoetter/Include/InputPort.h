///////////////////////////////////////////////////////////
//  InputPort.h
//  Implementation of the Class InputPort
//  Created on:      21-Mai-2010 11:53:25
//  Original author: Michael Uelschen
///////////////////////////////////////////////////////////

#if !defined(EA_9B5F018F_AA70_4108_BE7C_02FFA03E8448__INCLUDED_)
#define EA_9B5F018F_AA70_4108_BE7C_02FFA03E8448__INCLUDED_

/*! Dieser Aufzählungstyp beschreibt die möglichen physikalischen Eingänge
 *  eines Controllers (TX-Controller, ROBO IF-Controller). Die tatsächlich
 *  vorhandenen Eingänge sind dem physikalischen Modell zu entnehmen. */
enum InputPort
{
	I1 = 0,		/*! Digitale Eingänge. */
	I2 = 1,
	I3 = 2,
	I4 = 3,
	I5 = 4,
	I6 = 5,
	I7 = 6,
	I8 = 7,
	I9 = 8,
	I10 = 9,
	I11 = 10,
	I12 = 11,
	I13 = 12,
	I14 = 13,
	I15 = 14,
	I16 = 15,
	I_NUM = I16+1,
	C1 = 0,		/*! Zähler-Eingänge. */
	C2 = 1,
	C3 = 2,
	C4 = 3,
	C_NUM = C4+1, 
	A1 = 0,		/*! Analog-Eingänge. */
	A2 = 1,
	A3 = 2,
	A4 = 3,
	A_NUM = A4+1
};
#endif // !defined(EA_9B5F018F_AA70_4108_BE7C_02FFA03E8448__INCLUDED_)
