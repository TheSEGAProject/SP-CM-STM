///////////////////////////////////////////////////////////////////////////////
//! \file FiveTM.h
//! \brief This is the h file for controlling the 5TM interface on an SP-CM-STM
//! board
//!
//!
//!
//! @addtogroup
//! @{
//!
//! Written by Samuel Boegli 2012
///////////////////////////////////////////////////////////////////////////////


#ifndef FiveTM_H_
#define FiveTM_H_

#define NUM_1_5TM_ON	1
#define NUM_2_5TM_ON	1
#define NUM_3_5TM_ON	0
#define NUM_4_5TM_ON	0

//******************  5TM Com Variables  *****************************************//
//! @name 5TM Com Variables
//! There variables are used in the receiving of data from the 5TM
//! @{

//! \def c5TM_1_RX_PIN
//! \brief The RX Pin for 5TM 1
#define c5TM_1_RX_PIN       BIT3

//! \def c5TM_2_RX_PIN
//! \brief The RX Pin for 5TM 2
#define c5TM_2_RX_PIN       BIT4

//! \def c5TM_3_RX_PIN
//! \brief The RX Pin for 5TM 3
#define c5TM_3_RX_PIN       NULL

//! \def c5TM_4_RX_PIN
//! \brief The RX Pin for 5TM 4
#define c5TM_4_RX_PIN       NULL

//! \def P_5TM_RX_DIR
//! \brief The PxDIR register of the RX pin
#define P_5TM_RX_DIR       P1DIR
//! \def P_5TM_RX_IN
//! \brief The PxIN register of the RX pin
#define P_5TM_RX_IN        P1IN
//! \def P_RX_IES
//! \brief The PxIES register of the RX pin
#define P_5TM_RX_IES       P1IES
//! \def P_RX_IFG
//! \brief The PxIFG register of the RX pin
#define P_5TM_RX_IFG       P1IFG
//! \def P_RX_IE
//! \brief The PxIE register of the RX pin
#define P_5TM_RX_IE        P1IE
//! @}

//******************  5TM Power Variables  *****************************************//
//! @name 5TM Power Variables
//! There variables are used in the powering of the 5TM
//! @{

//! \def c5TM_1_PWR_PIN
//! \brief The Pin for powering the 5TM1
#define c5TM_1_PWR_PIN       BIT0

//! \def c5TM_2_PWR_PIN
//! \brief The Pin for powering the 5TM2
#define c5TM_2_PWR_PIN       BIT1

//! \def c5TM_3_PWR_PIN
//! \brief The Pin for powering the 5TM3
#define c5TM_3_PWR_PIN       NULL


//! \def c5TM_4_PWR_PIN
//! \brief The Pin for powering the 5TM4
#define c5TM_4_PWR_PIN       NULL


//! \def P_5TM_PWR_DIR
//! \brief The PxDIR register of the 5TM Power
#define P_5TM_PWR_DIR       P3DIR
//! \def P_5TM_PWR_OUT
//! \brief The PxOUT register of the 5TMK Power
#define P_5TM_PWR_OUT        P3OUT
//! @}

#define RX_BUFFER_SIZE_5TM 	   16	 //4 SoilM, 1 Space, 1 Zero, 1 Space, 3 Temp, 1 CR = 11
								 	 //+ Metadata 1 (z for 5TE, x for 5TM), 1 Checksum, 2 CR+NL = 4

//! \def FIVETM_ERROR_CODE_1
//! \brief The Checksum didn't work out...
#define FIVETM_ERROR_CODE_1		0x51

//! \def FIVETM_ERROR_CODE_1
//! \brief Timed out without a response
#define FIVETM_ERROR_CODE_2		0x52


void v5TM_Initialize(void);
char c5TM_Measure1(void);
char c5TM_Measure2(void);
char c5TM_Measure3(void);
char c5TM_Measure4(void);

void v5TM_Display(char);
char c5TM_Test_Checksum(char);
void c5TM_ReadValue(char);

int i5TM_GetSoil(char);
int i5TM_GetTemp(char);



//!Interrupt Handler
__interrupt void TIMERB1_ISR(void);
__interrupt void PORT1_ISR(void);
#endif /* FiveTM_H_ */
