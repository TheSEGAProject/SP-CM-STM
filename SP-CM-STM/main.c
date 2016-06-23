//*****************************************************************************
// main.c
//
// By: Samuel Boegli
//     Wireless Networks Research Lab
//     Dept of Electrical Engineering, CEFNS
//     Northern Arizona University
//
// SP-CM-STM Main
//*****************************************************************************

#include <msp430x23x.h>
#include "core/core.h"

//UARTDELETE
const char splash[] = {"\r\n--------------------------------------------------\r\n     ########      |      NAU WNRL and MSP430\r\n    # $$##$$ #     |	       Present:\r\n     $  ##  $ #    |          SP-CM-STM\r\n      $$##   #     |            V 2.0\r\n        #####      |       No Copyright 2012\r\n        ##$$ #     |\r\n     $  ##  $ #    |    Written and Directed By:\r\n    # $$##$$ #     |         Samuel Boegli\r\n     ########      |\r\n--------------------------------------------------\r\n"};
//const char splash[] = {"\r\n------------------ SP-CM-STM V2 Code ------------------\r\n"};

char cValve_Initialized = 0;//To make sure we initialize only once.
char c5TM_Initialized = 0;


char main_Do5TM1(void);
char main_Do5TM2(void);



//Obsolete test functions
/*
uint16 vDUMMY_AddData(uint16 * input1, uint16 * input2)
{
	*input1 = (*input1 + *(input2+1));
	return 1;
}

uint16 vDUMMY_SubtractData(uint16 * input1, uint16 * input2)
{
	*input1 = (*input1 - *(input2+1));
	return 1;
}

uint16 vDUMMY_MultiplyData(uint16 * input1, uint16 * input2)
{
	*input1 = (*input1 * *(input2+1));
	return 1;
}

uint16 vDUMMY_DivideData(uint16 * input1, uint16 * input2)
{
	*input1 = (*input1 / *(input2+1));
	return 1;
}
*/


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Test Function is called
//!
//!   Sends a UART message what the command was.
//!
//!   \param pointer to an array where data will be placed
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_Test(uint16 * arr)
{
//	vUARTCOM_TXString("Test Function\r\n",15);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 1 is called
//!
//!   Sends a UART message what the command was, and activates 5TM 1
//!
//!   \param pointer to an array where data will be placed
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_STM1(uint16 * arr)
{
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

	char result = main_Do5TM1();
	if(result == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result = 0;
	}

	return result;
}
///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 2 is called
//!
//!   Sends a UART message what the command was, and activates 5TM 2
//!
//!   \param pointer to an array where data will be placed
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_STM2(uint16 * arr)
{
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

	char result = main_Do5TM2();
	if(result == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result = 0;
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 3 is called
//!
//!   Sends a UART message what the command was, and activates 5TM 1 and 2
//!
//!   \param pointer to an array where data will be placed
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_STM12(uint16 * arr)
{
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}
	char result1 = main_Do5TM1();
	if(result1 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result1 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result1 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result1 = 0;
	}


	char result2 = main_Do5TM2();
	if(result2 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result2 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	if(result1 && result2)
		return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 4 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 1, depending on the parameter.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM1(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	//UARTDELETE
//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result = unVALVE_Set1(*arr);
	if(!result)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result = 0;
	}
	return result;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 5 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 1, depending on the parameter. Then activates 5TM 1.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM1_STM1(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}
	//UARTDELETE
//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM1();
	if(result2 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result2 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	if(result1 && result2)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 6 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 1, depending on the parameter. Then activates 5TM 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM1_STM2(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}
	//UARTDELETE
//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM2();
	if(result2 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result2 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	if(result1 && result2)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 7 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 1, depending on the parameter. Then activates 5TM 1 and 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM1_STM12(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}
	//UARTDELETE
//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM1();
	if(result2 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result2 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	char result3 = main_Do5TM2();
	if(result3 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result3 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result3 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result3 = 0;
	}

	if(result1 && result2 && result3)
		return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 8 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 2, depending on the parameter
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM2(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	//UARTDELETE
//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 2 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 2 Off\r\n",16);
	char result = unVALVE_Set2(*(arr+1));
	if(!result)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result = 0;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer 9 is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 2, depending on the parameter. Then activates 5TM 1.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM2_STM1(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

	//UARTDELETE
//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 2 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 2 Off\r\n",16);
	char result1 = unVALVE_Set2(*(arr+1));
	if(!result1)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM1();
	if(result2 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result2 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	if(result1 && result2)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer A is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 2, depending on the parameter. Then activates 5TM 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM2_STM2(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

	//UARTDELETE
//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 2 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 2 Off\r\n",16);
	char result1 = unVALVE_Set2(*(arr+1));
	if(!result1)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM2();
	if(result2 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result2 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	if(result1 && result2)
		return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer B is called
//!
//!   Sends a UART message what the command was, and activates or
//!   deactivates valve 2, depending on the parameter. Then activates 5TM 1 and 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM2_STM12(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

	//UARTDELETE
//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 2 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 2 Off\r\n",16);
	char result1 = unVALVE_Set2(*(arr+1));
	if(!result1)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

	char result2 = main_Do5TM1();
	if(result2 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result2 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result2 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result2 = 0;
	}

	char result3 = main_Do5TM2();
	if(result3 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result3 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result3 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result3 = 0;
	}

	if(result1 && result2 && result3)
		return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer C is called
//!
//!   Sends a UART message what the command was, and activates or deactivates
//!   valves 1 and 2, depending on the parameters.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM12(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}

//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result2 = unVALVE_Set2(*(arr+1));
	if(!result2)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result2 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result2 = 0;
	}


	if(result1 && result2)
		return 1;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer D is called
//!
//!   Sends a UART message what the command was, and activates or deactivates
//!   valves 1 and 2, depending on the parameters. Then activates 5TM 1.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM12_STM1(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result2 = unVALVE_Set2(*(arr+1));
	if(!result2)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result2 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result2 = 0;
	}



	char result3 = main_Do5TM1();
	if(result3 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result3 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result3 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result3 = 0;
	}

	if(result1 && result2 && result3)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer E is called
//!
//!   Sends a UART message what the command was, and activates or deactivates
//!   valves 1 and 2, depending on the parameters. Then activates 5TM 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM12_STM2(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result2 = unVALVE_Set2(*(arr+1));
	if(!result2)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result2 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result2 = 0;
	}


	char result3 = main_Do5TM2();
	if(result3 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result3 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result3 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result3 = 0;
	}

	if(result1 && result2 && result3)
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Handle for when Transducer F is called
//!
//!   Sends a UART message what the command was, and activates or deactivates
//!   valves 1 and 2, depending on the parameters. Then activates 5TM 1 and 2.
//!
//!   \param g_unaCoreData.
//!		 A pointer at the data that came from the CP board and where the answer
//!      is to be written. 0x00: Turn off, 0x5B: Turn on
//!		 Data for 5TM is also written here
//!
//!   \return 1: success, 0: failure
///////////////////////////////////////////////////////////////////////////////
uint16 main_CM12_STM12(uint16 * arr)
{
	if(!cValve_Initialized)
	{
		vVALVE_Initialize();
	    cValve_Initialized = 1;
	}
	if(!c5TM_Initialized)
	{
		v5TM_Initialize();
		c5TM_Initialized = 1;
	}

//	if(*arr == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*arr == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result1 = unVALVE_Set1(*arr);
	if(!result1)
		*(arr) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result1 == 2)
	{
		*(arr) = VALVE_POST_FAULT_ERROR_CODE;
		result1 = 0;
	}

//	if(*(arr+1) == VALVE_ON)
//		vUARTCOM_TXString("CP-Valve 1 On\r\n",15);
//	if(*(arr+1) == VALVE_OFF)
//		vUARTCOM_TXString("CP-Valve 1 Off\r\n",16);
	char result2 = unVALVE_Set2(*(arr+1));
	if(!result2)
		*(arr+1) = VALVE_PRE_FAULT_ERROR_CODE;
	if(result2 == 2)
	{
		*(arr+1) = VALVE_POST_FAULT_ERROR_CODE;
		result2 = 0;
	}


	char result3 = main_Do5TM1();
	if(result3 == 1){
		*(arr+4) = i5TM_GetSoil(1);
		*(arr+5) = i5TM_GetTemp(1);
	}else if(result3 == 0){
		*(arr+4) = FIVETM_ERROR_CODE_1;
	}else if(result3 == 2){
		*(arr+4) = FIVETM_ERROR_CODE_2;
		result3 = 0;
	}



	char result4 = main_Do5TM2();
	if(result4 == 1){
		*(arr+6) = i5TM_GetSoil(2);
		*(arr+7) = i5TM_GetTemp(2);
	}else if(result4 == 0){
		*(arr+6) = FIVETM_ERROR_CODE_1;
	}else if(result4 == 2){
		*(arr+6) = FIVETM_ERROR_CODE_2;
		result4 = 0;
	}


	if(result1 && result2 && result3 && result4)
		return 1;
	return 0;
}

char main_Do5TM1()
{
	//UARTDELETE
//	vUARTCOM_TXString("5TM1 Start\r\n",12);
	char result = c5TM_Measure1();
//	if(result == 1)
//		v5TM_Display(1);
//	else if(result == 2)
//		vUARTCOM_TXString("5TM Timeout\r\n",13);
	//P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;//The turning off of the 5TM causes a falling
		//edge interrupt. Because we turned off the interrupt, it is not called, but
		//the flag still exists so when the P2 interrupt is called for the CP COMM,
		//the flag causes it to enter this interrupt falsely, disturbing communication
		//greatly(wait for 1200Baud_delay). So as long as these two share the port 2
		//interrupt, flags must be reset before trying to communicate (They no longer share, but I leave this as warning.)
	return result;
}

char main_Do5TM2()
{
	//UARTDELETE
//	vUARTCOM_TXString("5TM2 Start\r\n",12);
	char result = c5TM_Measure2();
//	if(result == 1)
//		v5TM_Display(2);
//	else if(result == 2)
//		vUARTCOM_TXString("5TM Timeout\r\n",13);
	//P_5TM_RX_IFG &= ~c5TM_2_RX_PIN;//The turning off of the 5TM causes a falling
		//edge interrupt. Because we turned off the interrupt, it is not called, but
		//the flag still exists so when the P2 interrupt is called for the CP COMM,
		//the flag causes it to enter this interrupt falsely, disturbing communication
		//greatly(wait for 1200Baud_delay). So as long as these two share the port 2
		//interrupt, flags must be reset before trying to communicate (They no longer share, but I leave this as warning.)
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief The main file for the SP-CM-STM SP Board
//!
//!   Initializes Core, turns off LEDs, initializes UART, 5TM, and Valves
//!	  Then runs vCORE_Run()
//!
//!   \param none
//!
//!   \return never
///////////////////////////////////////////////////////////////////////////////
void main()
{
  //Init the trusted core
  vCORE_Initilize();

  //P4.3/P4.4 are LEDs

  P4OUT |= BIT3;
  P4OUT |= BIT4;
  
/*  P4OUT &= ~BIT3;
  P4OUT &= ~BIT4;
*/

  //UARTDELETE
//  vUARTCOM_Initialize();
//  vUARTCOM_TXString( (char*)splash, sizeof splash); //Display the Splash screen of SP-CM-STM

//  vUART_DisplayVoltage();

  // ******************************* nFAULT Test Code
/*
  P4OUT |= BIT3;
  while(true)
  {
	  if(P1IN & BIT2)
		  P4OUT |= BIT3;
	  else
		  P4OUT &= !BIT3;
  }
*/
  // *******************************


  // Run the device
  vCORE_Run();
}
