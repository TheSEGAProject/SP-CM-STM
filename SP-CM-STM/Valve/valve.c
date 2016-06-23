///////////////////////////////////////////////////////////////////////////////
//! \file valve.c
//! \brief This is the file for controlling the valves of the SP-CM-STM.
//!
//!
//!
//! @addtogroup valve Valve Control
//! The valve control software turns the valves on or off, depending on the
//! parameter given. The timing of the electrical solenoid control is also handled.
//! @{
//!
//! Written by Samuel Boegli 2012
///////////////////////////////////////////////////////////////////////////////

#include <msp430x23x.h>
//#include valve.h //Now included in core.h
#include "../core/core.h"

//******************  Valve Global Variables  ***************************//
//! @name Valve Global Variables
//! These variables are used globally for the valve
//!
//! @{
//! \var unsigned int unInitTime
//! \brief This is the time on the timer when the valve was turned on.
//unsigned int unInitTime;

//! \var char Error
//! \brief This is the error flag, set if an error is detected.
char Error = 0;

//! @}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Initializes the valve program
//!
//!		Sets IO Ports, variables, and clock.
//!
//!   \param none
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
void vVALVE_Initialize(void){
	//unInitTime = 0;
	//Make outputs
	VALVE_P_DIR |= (VALVE_1_ON + VALVE_1_OFF + VALVE_2_ON + VALVE_2_OFF); // 0xF0 : 11110000
	VALVE_P_OUT &= ~VALVE_1_ON;
	VALVE_P_OUT &= ~VALVE_1_OFF;
	VALVE_P_OUT &= ~VALVE_2_ON;
	VALVE_P_OUT &= ~VALVE_2_OFF;
	//DRV_GOOD_P_DIR &= ~DRV_nFAULT; //Input, Made input at 'changeable_core_header.h'
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Starts and manages the timing of valve 1
//!
//!		If the valve is already running, it adds the given amount of time to the
//!		existing value.
//!
//!   \param value1: The time value for which the valve should run.
//!					 Each step is 16.66ms long, set by ONOFF_CYCLE
//!
//!   \return 1: success
///////////////////////////////////////////////////////////////////////////////
unsigned int unVALVE_Set1(unsigned int value1)
{
	//Function that checks to make sure the DRV H-Bridge runs properly
	if(!(DRV_nFAULT_P_IN & DRV_nFAULT))//nFAULT = 0 (not good)
	{
		//Error = 1;
//		vUARTCOM_TXString("\r\nError 1\r\n",11);
		return 0;
	}


	TBCTL = (TBSSEL_1 + MC1 + TBCLR + TBIE);//0X0126 = 0000 0001 0010 0110 //Continuous Mode, IE enable, ACLK, Cleared

	//unInitTime = TBR;
	if(value1==VALVE_ON)
	{
		VALVE_P_OUT |= VALVE_1_ON; //Turn on Valve1
	}
	if(value1==VALVE_OFF)
		VALVE_P_OUT |= VALVE_1_OFF; //Turn off Valve1

	TBR=0;
	TBCCR0 = ONOFF_CYCLE;// + unInitTime; //Set the Compare Register
	TBCCTL0 &= ~CCIFG;

	TBCCTL0 |= CCIE; //Start interrupt

	// Shutoff the MCLK, execution returns here after ON/OFF is done
	__bis_SR_register(GIE + LPM3_bits);

	TBCCR0 = 0;
	TBCCTL0 &= ~CCIE; //Stop interrupt
	TBCTL = (TBSSEL_1 + TBCLR);//0x0104; //Timer B Stopped, ACLK, clear

	//Function that checks to make sure the DRV H-Bridge runs properly
	if(!(DRV_nFAULT_P_IN & DRV_nFAULT))//nFAULT = 0 (not good)
	{
		//Error = 1;
//		vUARTCOM_TXString("\r\nError 1b\r\n",12);
		return 2;
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Starts and manages the timing of valve 2
//!
//!		If the valve is already running, it adds the given amount of time to the
//!		existing value.
//!
//!   \param value2: The time value for which the valve should run.
//!					 Each step is 16.66ms long, set by ONOFF_CYCLE
//!
//!   \return 1: success
///////////////////////////////////////////////////////////////////////////////
unsigned int unVALVE_Set2(unsigned int value2)
{
	//Function that checks to make sure the DRV H-Bridge runs properly
	if(!(DRV_nFAULT_P_IN & DRV_nFAULT))//nFAULT = 0 (not good)
	{
		//Error = 1;
//		vUARTCOM_TXString("\r\nError 2\r\n",11);
		return 0;
	}
	//If the valve is already turned on, add value2 to runtime
	TBCTL = (TBSSEL_1 + MC1 + TBCLR + TBIE);//0X0126 = 0000 0001 0010 0110 //Continuous Mode, IE enable, ACLK, Cleared

	//unInitTime = TBR;
	if(value2==VALVE_ON)
		VALVE_P_OUT |= VALVE_2_ON; //Turn on Valve2
	if(value2==VALVE_OFF)
		VALVE_P_OUT |= VALVE_2_OFF; //Turn off Valve2

	TBR=0;
	TBCCR0 = ONOFF_CYCLE;// + unInitTime; //Set the Compare Register
	TBCCTL0 &= ~CCIFG;

	TBCCTL0 |= CCIE; //Start interrupt
	// Shutoff the MCLK, execution returns here after ON/OFF is done
	__bis_SR_register(GIE + LPM3_bits);

	TBCCR0 = 0;
	TBCCTL0 &= ~CCIE; //Stop interrupt
	TBCTL = (TBSSEL_1 + TBCLR);//0x0104; //Timer B Stopped, ACLK, clear

	//Function that checks to make sure the DRV H-Bridge runs properly
	if(!(DRV_nFAULT_P_IN & DRV_nFAULT))//nFAULT = 0 (not good)
	{
		//Error = 1;
//		vUARTCOM_TXString("\r\nError 2b\r\n",12);
		return 2;
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Compare register 0 interrupt
//!
//!		If we were turning valve on: Finish
//!
//!		If we are finished turning off valve: Finish
//!
//!   \param none
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void)
{
	if(TBCCTL0 & CCIE)//Compare Register 0 interrupt enabled?
	{
		if(VALVE_P_OUT & VALVE_1_ON)//If valve was being turned on (VALVE_1_ON = 0x10)
		{
			VALVE_P_OUT &= ~VALVE_1_ON; //Stop turning on Valve1
		}
		else if(VALVE_P_OUT & VALVE_1_OFF)//If valve was being turned off and is now done (VALVE_1_OFF = 0x20)
		{
			VALVE_P_OUT &= ~VALVE_1_OFF; //Stop turning off Valve1
		}
		else if(VALVE_P_OUT & VALVE_2_ON)//If valve was being turned on (VALVE_2_ON = 0x40)
		{
			VALVE_P_OUT &= ~VALVE_2_ON; //Stop turning on Valve1
		}
		else if(VALVE_P_OUT & VALVE_2_OFF)//If valve was being turned off and is now done (VALVE_2_OFF = 0x80)
		{
			VALVE_P_OUT &= ~VALVE_2_OFF; //Stop turning off Valve1
		}

		TBCCTL0 &= ~CCIFG;//Clear Flag

		__bic_SR_register_on_exit(LPM4_bits);//CPU slept
	}
}

//! @}

