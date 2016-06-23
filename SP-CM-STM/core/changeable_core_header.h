///////////////////////////////////////////////////////////////////////////////
//! \file changeable_core_header.h
//! \brief The header file for the core that a user is allowed to change and
//!	adjust settings.
//!
//! This file is where SP transducer files can be included, packet size chosen,
//! the ID is set, the Label is set, and the transducer functions are declared
//! and set in the core.
//!
//! @addtogroup core
//! @{
//!
//! @addtogroup hdr Core Changeable Header
//! This is the header file that should be adjusted so the core can be used
//! with any SP Board transducer functions.
//! @{
///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// By: Samuel Boegli
//     Wireless Networks Research Lab
//     Dept of Electrical Engineering, CEFNS
//     Northern Arizona University
//
//*****************************************************************************

#ifndef CHANGEABLE_CORE_HEADER_H_
#define CHANGEABLE_CORE_HEADER_H_

//includes
  #include "../UART/uartCom.h" //Comment this out in final code.
  #include "../Valve/valve.h"
  #include "../FiveTM/FiveTM.h"


//! @name SP Board Software Version Variable
//!
//! @{
//!\def SOFTWAREVERSION
//! \brief The software version running on the SP Board. Not the Core version.
#define SOFTWAREVERSION "SP-CM-STM v2.00 "
//! @}

//! @name Core Property Set Variables
//! These variables are used to set properties of the core.
//! @{
//!\def PACKET_SIZE_128
//! \brief Chooses whether the data packet uses 128 bits or 32 bits of data
//!
//! 128 bit or 32 bit data return packet?
//!
//! If you want a 128 bit data size packet, define as: 1
//!
//! If you want a 32 bit data size packet, define as: 0
#define SP_PACKET_SIZE_128		1

//!@}

//! @name SP Board ID Variables
//! These variables are used to set the unique ID of the SP Board. Should
//! indicate what type of SP Board it is, what size of data packet it sends,
//! and have the unique hardware ID in it.
//! @{

#define  ID_PKT_HI_BYTE1	0x12;
#define  ID_PKT_LO_BYTE1	0x8B;
#define  ID_PKT_HI_BYTE2	0x12;
#define  ID_PKT_LO_BYTE2	0x8B;

//!@}

//! @name Transducer Labels
//! The labels for each transducer are set in constants here.
//! Each constant must be 16 characters long.
//! @{
#define TRANSDUCER_0_LABEL_TXT "Test Function   " //0000
#define TRANSDUCER_1_LABEL_TXT "STM1            " //0001
#define TRANSDUCER_2_LABEL_TXT "STM2            " //0010
#define TRANSDUCER_3_LABEL_TXT "STM12           " //0011
#define TRANSDUCER_4_LABEL_TXT "CM1             " //0100
#define TRANSDUCER_5_LABEL_TXT "CM1 STM1        " //0101
#define TRANSDUCER_6_LABEL_TXT "CM1 STM2        " //0110
#define TRANSDUCER_7_LABEL_TXT "CM1 STM12       " //0111
#define TRANSDUCER_8_LABEL_TXT "CM2             " //1000
#define TRANSDUCER_9_LABEL_TXT "CM2 STM1        " //1001
#define TRANSDUCER_A_LABEL_TXT "CM2 STM2        " //1010
#define TRANSDUCER_B_LABEL_TXT "CM2 STM12       " //1011
#define TRANSDUCER_C_LABEL_TXT "CM12            " //1100
#define TRANSDUCER_D_LABEL_TXT "CM12 STM1       " //1101
#define TRANSDUCER_E_LABEL_TXT "CM12 STM2       " //1110
#define TRANSDUCER_F_LABEL_TXT "CM12 STM12      " //1111

//!@}

//! @name Transducer Function Declarations
//! The transducer functions are declared here. They wil be defined in
//! the main file.
//!
//! Every function must be of the format "uint16 FunctionName(uint16 *);"
//! @{

uint16 main_Test(uint16 *);
uint16 main_STM1(uint16 *);
uint16 main_STM2(uint16 *);
uint16 main_STM12(uint16 *);
uint16 main_CM1(uint16 *);
uint16 main_CM1_STM1(uint16 *);
uint16 main_CM1_STM2(uint16 *);
uint16 main_CM1_STM12(uint16 *);
uint16 main_CM2(uint16 *);
uint16 main_CM2_STM1(uint16 *);
uint16 main_CM2_STM2(uint16 *);
uint16 main_CM2_STM12(uint16 *);
uint16 main_CM12(uint16 *);
uint16 main_CM12_STM1(uint16 *);
uint16 main_CM12_STM2(uint16 *);
uint16 main_CM12_STM12(uint16 *);

//!@}

//! @name Transducer Function Matching
//! The transducer functions are matched to their respective transducer.
//! if there is no function for a transducer, the definition NULL shall be used.
//!
//! Example:
//! '#define 	TRANSDUCER_X_FUNCTION 		&FunctionName'
//!
//! @{

#define 	TRANSDUCER_0_FUNCTION 		&main_Test
#define 	TRANSDUCER_1_FUNCTION 		&main_STM1
#define 	TRANSDUCER_2_FUNCTION 		&main_STM2
#define 	TRANSDUCER_3_FUNCTION 		&main_STM12
#define 	TRANSDUCER_4_FUNCTION 		&main_CM1
#define 	TRANSDUCER_5_FUNCTION 		&main_CM1_STM1
#define 	TRANSDUCER_6_FUNCTION 		&main_CM1_STM2
#define 	TRANSDUCER_7_FUNCTION 		&main_CM1_STM12
#define 	TRANSDUCER_8_FUNCTION 		&main_CM2
#define 	TRANSDUCER_9_FUNCTION 		&main_CM2_STM1
#define 	TRANSDUCER_A_FUNCTION 		&main_CM2_STM2
#define 	TRANSDUCER_B_FUNCTION 		&main_CM2_STM12
#define 	TRANSDUCER_C_FUNCTION 		&main_CM12
#define 	TRANSDUCER_D_FUNCTION 		&main_CM12_STM1
#define 	TRANSDUCER_E_FUNCTION 		&main_CM12_STM2
#define 	TRANSDUCER_F_FUNCTION 		&main_CM12_STM12

//!@}

//! @name I/O Port Setups
//! In some cases the I/O Ports must be defined by the Application Layer
//! Make those defines here. If no change is needed, use 'DEF'
//!
//! @{
#define		DEF			0x100	//Default Statement. Let the core do standard definitions.
#define 	CoreP1DIR	(~(c5TM_1_RX_PIN + c5TM_2_RX_PIN + DRV_nFAULT))//0xE3	//5TM Inputs must not be Low-Outputs. Also, DRV-nFAULT is an input and must also not be pulled low.
#define 	CoreP1OUT	DEF
#define 	CoreP2DIR	DEF
#define 	CoreP2OUT	DEF
#define 	CoreP3DIR	DEF
#define 	CoreP3OUT	DEF
#define 	CoreP4DIR	DEF
#define 	CoreP4OUT	DEF
#define 	CoreP5DIR	DEF
#define 	CoreP5OUT	DEF
#define 	CoreP6DIR	DEF
#define 	CoreP6OUT	DEF
//!@}

#endif /* CHANGEABLE_CORE_HEADER_H_ */

