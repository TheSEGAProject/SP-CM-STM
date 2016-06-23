///////////////////////////////////////////////////////////////////////////////
//! \file valve.h
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

#ifndef VALVE_H_
#define VALVE_H_

//! @name Valve Pin Defines
//! The pins of the MSP430 used to control the valves are defined here
//! @{
//!\def DRV_GOOD
//! \brief The pin where the DRV H-Bridge status can be monitored.
#define DRV_nFAULT			BIT2 //Port 1

#define DRV_nFAULT_P_DIR	P1DIR

#define DRV_nFAULT_P_IN		P1IN

//!\def VALVE_1_ON
//! \brief The pin where the H-Bridge can be made to pass positive voltage
//! to the Valve 1 Solenoid: 1 is on, 0 is off.
#define VALVE_1_ON 		BIT4 //Port 6

//!\def VALVE_1_OFF
//! \brief The pin where the H-Bridge can be made to pass negative voltage
//! to the Valve 1 Solenoid: 1 is on, 0 is off.
#define VALVE_1_OFF 	BIT5 //Port 6

//!\def VALVE_2_ON
//! \brief The pin where the H-Bridge can be made to pass positive voltage
//! to the Valve 2 Solenoid: 1 is on, 0 is off.
#define VALVE_2_ON 		BIT6 //Port 6

//!\def VALVE_2_OFF
//! \brief The pin where the H-Bridge can be made to pass negative voltage
//! to the Valve 2 Solenoid: 1 is on, 0 is off.
#define VALVE_2_OFF 	BIT7 //Port 6

#define VALVE_P_DIR		P6DIR

#define VALVE_P_OUT		P6OUT

//! @}

//! @name Valve Constants
//! The constants used in the valve program are defined here
//! @{
//!\def ONOFF_CYCLE
//! \brief The length of time the solenoid must be powered to turn the valve
//! on/off: 150/3kHz means 50 ms.
#define ONOFF_CYCLE		150

//!\def VALVE_ON
//! \brief The code needed to authorize the turning on of the valve. Passed as the
//! argument for the unValve_Set1 or 2 functions
#define VALVE_ON		0X5B

//!\def VALVE_OFF
//! \brief The code needed to authorize the turning off of the valve. Passed as the
//! argument for the unValve_Set1 or 2 functions
#define VALVE_OFF		0x00

//!\def VALVE_PRE_FAULT_ERROR_CODE
//! \brief The error code transmitted when nFAULT was active when we tried to switch
#define VALVE_PRE_FAULT_ERROR_CODE		0xE1

//!\def VALVE_POST_FAULT_ERROR_CODE
//! \brief The error code transmitted when nFAULT was active when we finished switching
#define VALVE_POST_FAULT_ERROR_CODE		0xE2
//! @}

  //! @name Control Functions
  //! These functions handle controlling the valves
  //! @{
void vVALVE_Initialize(void);
unsigned int unVALVE_Set1(unsigned int);
unsigned int unVALVE_Set2(unsigned int);

//! @}

#endif /* VALVE_H_ */
