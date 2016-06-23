///////////////////////////////////////////////////////////////////////////////
//! \file FiveTM.h
//! \brief This is the c file for controlling the 5TM interface on an SP-CM-STM
//! board
//!
//!
//!
//! @addtogroup
//! @{
//!
//! Written by Samuel Boegli 2012
///////////////////////////////////////////////////////////////////////////////

#include <msp430x23x.h>
//#include 5TM.h //Now included in core.h
#include "../core/core.h"

//******************  RX Variables  *****************************************//
//! @name Receive Variables
//! These variables are used in the receiving of data on the \ref comm Module.
//! @{
//! \var volatile uint8 g_ucaRXBuffer[RX_BUFFER_SIZE]
//! \brief The software UART RX Buffer
char g_uca5TM_1_RXBuffer[RX_BUFFER_SIZE];

//! \var volatile uint8 g_ucRXBufferIndex
//! \brief This index into g_ucaRXBuffer showing the current write position.
volatile char g_uc5TM_1_RXBufferIndex;

//! \var volatile uint8 g_ucaRXBuffer[RX_BUFFER_SIZE]
//! \brief The software UART RX Buffer
char g_uca5TM_2_RXBuffer[RX_BUFFER_SIZE];

//! \var volatile uint8 g_ucRXBufferIndex
//! \brief This index into g_ucaRXBuffer showing the current write position.
volatile char g_uc5TM_2_RXBufferIndex;

//! \var volatile uint8 g_ucaRXBuffer[RX_BUFFER_SIZE]
//! \brief The software UART RX Buffer
char g_uca5TM_3_RXBuffer[RX_BUFFER_SIZE];

//! \var volatile uint8 g_ucRXBufferIndex
//! \brief This index into g_ucaRXBuffer showing the current write position.
volatile char g_uc5TM_3_RXBufferIndex;

//! \var volatile uint8 g_ucaRXBuffer[RX_BUFFER_SIZE]
//! \brief The software UART RX Buffer
char g_uca5TM_4_RXBuffer[RX_BUFFER_SIZE];

//! \var volatile uint8 g_ucRXBufferIndex
//! \brief This index into g_ucaRXBuffer showing the current write position.
volatile char g_uc5TM_4_RXBufferIndex;

//! @}

//******************  Values  *****************************************//
//! @name The Temperature and Soil Moisture Values
//! Save the calculated Values here
//! @{
//! \var uint16 un5TM_1_Temperature
//! \brief The temperature Value of 5TM1. The correct value *10
unsigned int un5TM_1_Temperature;

//! \var uint16 un5TM_2_Temperature
//! \brief The temperature Value of 5TM2. The correct value *10
unsigned int un5TM_2_Temperature;

//! \var uint16 un5TM_3_Temperature
//! \brief The temperature Value of 5TM3. The correct value *10
unsigned int un5TM_3_Temperature;

//! \var uint16 un5TM_4_Temperature
//! \brief The temperature Value of 5TM4. The correct value *10
unsigned int un5TM_4_Temperature;

//! \var uint16 un5TM_1_Temperature_neg
//! \brief The temperature Value of 5TM1. The correct value *10
int un5TM_1_Temperature_neg = 0;

//! \var uint16 un5TM_2_Temperature_neg
//! \brief The temperature Value of 5TM2. The correct value *10
int un5TM_2_Temperature_neg = 0;

//! \var uint16 un5TM_3_Temperature_neg
//! \brief The temperature Value of 5TM3. The correct value *10
int un5TM_3_Temperature_neg = 0;

//! \var uint16 un5TM_4_Temperature_neg
//! \brief The temperature Value of 5TM4. The correct value *10
int un5TM_4_Temperature_neg = 0;

//! \var uint16 un5TM_1_Soil
//! \brief The Soil Moisture Value of 5TM1.
unsigned int un5TM_1_Soil;

//! \var uint16 un5TM_2_Soil
//! \brief The Soil Moisture Value of 5TM2.
unsigned int un5TM_2_Soil;

//! \var uint16 un5TM_3_Soil
//! \brief The Soil Moisture Value of 5TM3.
unsigned int un5TM_3_Soil;

//! \var uint16 un5TM_4_Soil
//! \brief The Soil Moisture Value of 5TM4.
unsigned int un5TM_4_Soil;


///NEED COMMENTS

char g_uc5TM_RXBitsLeft;
char g_uc5TM1_RXBusy;
char g_uc5TM2_RXBusy;
char g_uc5TM3_RXBusy;
char g_uc5TM4_RXBusy;
char timeoutcounter = 0;

int len;


///////////////////////////////////////////////////////////////////////////////
//!   \brief Initializes the 5TM program
//!
//!		IO Ports, variables, clock, and turns on 5V power.
//!
//!   \param none
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
void v5TM_Initialize(void)
{

   // We set the directionality of the RX pins based on the define.
   //This was already done in the changeable_core_header.h
   //P_5TM_RX_DIR &= ~c5TM_1_RX_PIN;
   //P_5TM_RX_DIR &= ~c5TM_2_RX_PIN;

   g_uc5TM1_RXBusy = 0;
   g_uc5TM2_RXBusy = 0;
   g_uc5TM3_RXBusy = 0;
   g_uc5TM4_RXBusy = 0;

   // Clear the RX buffer and reset index
   //The Excite Power for the 5TMs direction pin,
   //But keep them off.
#if NUM_1_5TM_ON
   for (g_uc5TM_1_RXBufferIndex = 0x00;
        g_uc5TM_1_RXBufferIndex < RX_BUFFER_SIZE_5TM;
	    g_uc5TM_1_RXBufferIndex++)
   {
      g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] = 0xFF;
   }
   g_uc5TM_1_RXBufferIndex = 0x00;

   P_5TM_PWR_DIR |= c5TM_1_PWR_PIN;
   P_5TM_PWR_OUT &= ~c5TM_1_PWR_PIN;
#endif
#if NUM_2_5TM_ON
   for (g_uc5TM_2_RXBufferIndex = 0x00;
        g_uc5TM_2_RXBufferIndex < RX_BUFFER_SIZE_5TM;
        g_uc5TM_2_RXBufferIndex++)
   {
      g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] = 0xFF;
   }
   g_uc5TM_2_RXBufferIndex = 0x00;

   P_5TM_PWR_DIR |= c5TM_2_PWR_PIN;
   P_5TM_PWR_OUT &= ~c5TM_2_PWR_PIN;
#endif
#if NUM_3_5TM_ON
   for (g_uc5TM_3_RXBufferIndex = 0x00;
        g_uc5TM_3_RXBufferIndex < RX_BUFFER_SIZE_5TM;
        g_uc5TM_3_RXBufferIndex++)
   {
      g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] = 0xFF;
   }
   g_uc5TM_3_RXBufferIndex = 0x00;

   P_5TM_PWR_DIR |= c5TM_3_PWR_PIN;
   P_5TM_PWR_OUT &= ~c5TM_3_PWR_PIN;
#endif
#if NUM_4_5TM_ON
   for (g_uc5TM_4_RXBufferIndex = 0x00;
        g_uc5TM_4_RXBufferIndex < RX_BUFFER_SIZE_5TM;
        g_uc5TM_4_RXBufferIndex++)
   {
      g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] = 0xFF;
   }
   g_uc5TM_4_RXBufferIndex = 0x00;

   P_5TM_PWR_DIR |= c5TM_4_PWR_PIN;
   P_5TM_PWR_OUT &= ~c5TM_4_PWR_PIN;
#endif


}


//**********************************************************************************************************************************************************************************************
//**********************************************************************************************************************************************************************************************
char c5TM_Measure1()
{

   // Clear the RX buffer and reset index WAS here, but I don't think necessary. Just a reminder it's an option...

   TBCTL |= TBSSEL_2;
   TBCTL &= ~TBSSEL_1; //Even though TBSSEL sets the bit we want, it doesn't unset the bit we don't want.
   TBCTL |= TBCLR; //Clear Clock
   P_5TM_PWR_OUT |= c5TM_1_PWR_PIN; //START exciting the 5TM1

   // ******************Delay...*******************************************************
   // Enable timer interrupt, configure for delay
   TBCTL |= TBCLR; //Clear
   TBCCTL1 |= CCIE;
   TBCCR0 = 0;
   TBCCR1 = 50000;//Equal to CPU delay 200000

   TBCTL &= ~TBIFG;
   TBCCTL1 &= ~TBIFG;//IFG

   TBCTL |= MC1;//Continuous Mode

   __bis_SR_register(LPM0_bits); //CPU asleep.

   TBCTL &= ~TBIFG;
   TBCCTL1 &= ~TBIFG;// Clear IFGs
   // **********************************************************************************
   //__delay_cycles(120000);//Allow time for the 5TM to pull the data line low(startup) then high. Otherwise there will be a bug...


   //Enable the falling edge interrupt
   P_5TM_RX_IES |= c5TM_1_RX_PIN;
   P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;
   P_5TM_RX_IE  |= c5TM_1_RX_PIN;

   g_uc5TM_1_RXBufferIndex = 0;

   //In case there's no 5TM attached, time out after 3 timer B roll overs. Timer was already started and ready for IFG back in delay
   TBR = 0;
   timeoutcounter = 0;


   //Sleep until measurement is done
   while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex-1] != 0x0A && timeoutcounter <3)
   {
      __bis_SR_register(LPM0_bits); //CPU asleep.
      timeoutcounter++;
   }


   //Disable Interrupt
   P_5TM_RX_IE  &= ~c5TM_1_RX_PIN;
   P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;
   //Turn off 5TM
   P_5TM_PWR_OUT &= ~c5TM_1_PWR_PIN;//END exciting the 5TM1

   TBCTL &= ~(MC0 | MC1 | TBIE | TBIFG);//Halt timer, Disable interrupts
   TBCTL |= TBCLR;//Clear Timer
   if(timeoutcounter >= 3)
	   return 2;
   c5TM_ReadValue(1);
   return c5TM_Test_Checksum(1);
}


char c5TM_Measure2()
{
   // Clear the RX buffer and reset index WAS here, but I don't think necessary. Just a reminder it's an option...

   TBCTL |= TBSSEL_2;
   TBCTL &= ~TBSSEL_1; //Even though TBSSEL_2 sets the bit we want, it doesn't unset the bit we don't want.
   TBCTL |= TBCLR; //Clear Clock
   P_5TM_PWR_OUT |= c5TM_2_PWR_PIN; //START exciting the 5TM1


   // ******************Delay...*******************************************************
   // Enable timer interrupt, configure for delay
   TBCTL |= TBCLR; //Clear
   TBCCTL1 |= CCIE;
   TBCCR0 = 0;
   TBCCR1 = 50000;//Equal to CPU delay 200000

   TBCTL &= ~TBIFG;
   TBCCTL1 &= ~TBIFG;//IFG

   TBCTL |= MC1;//Continuous Mode

   __bis_SR_register(LPM0_bits); //CPU asleep.

   TBCTL &= ~TBIFG;
   TBCCTL1 &= ~TBIFG;// Clear IFGs
   //__delay_cycles(120000);//Allow time for the 5TM to pull the data line low(startup) then high. Otherwise there will be a bug...
   // **********************************************************************************


   //Enable the falling edge interrupt
   P_5TM_RX_IES |= c5TM_2_RX_PIN;
   P_5TM_RX_IFG &= ~c5TM_2_RX_PIN;
   P_5TM_RX_IE  |= c5TM_2_RX_PIN;

   g_uc5TM_2_RXBufferIndex = 0;
   
   TBR = 0;
   timeoutcounter = 0;
    
   //Sleep until measurement is done
   while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex-1] != 0x0A && timeoutcounter <3)
  {
      __bis_SR_register(LPM0_bits); //CPU asleep.
      timeoutcounter++;
  }    
	
   //Disable Interrupt
   P_5TM_RX_IE  &= ~c5TM_2_RX_PIN;
   P_5TM_RX_IFG &= ~c5TM_2_RX_PIN;
   //Turn off 5TM
   P_5TM_PWR_OUT &= ~c5TM_2_PWR_PIN;//END exciting the 5TM1

   TBCTL &= ~(MC0 | MC1 | TBIE | TBIFG);//Halt timer, Disable interrupts
   TBCTL |= TBCLR;//Clear Timer
   if(timeoutcounter >= 3)
	   return 2;
   c5TM_ReadValue(2);
   return c5TM_Test_Checksum(2);
}

#if NUM_3_5TM_ON
char c5TM_Measure3()
{
	   // Clear the RX buffer and reset index WAS here, but I don't think necessary. Just a reminder it's an option...

	   TBCTL |= TBSSEL_2;
	   TBCTL &= ~TBSSEL_1; //Even though TBSSEL sets the bit we want, it doesn't unset the bit we don't want.
	   TBCTL |= TBCLR; //Clear Clock
	   P_5TM_PWR_OUT |= c5TM_3_PWR_PIN; //START exciting the 5TM1

	   // ******************Delay...*******************************************************
	   // Enable timer interrupt, configure for delay
	   TBCTL |= TBCLR; //Clear
	   TBCCTL1 |= CCIE;
	   TBCCR0 = 0;
	   TBCCR1 = 50000;//Equal to CPU delay 200000

	   TBCTL &= ~TBIFG;
	   TBCCTL1 &= ~TBIFG;//IFG

	   TBCTL |= MC1;//Continuous Mode

	   __bis_SR_register(LPM0_bits); //CPU asleep.

	   TBCTL &= ~TBIFG;
	   TBCCTL1 &= ~TBIFG;// Clear IFGs
	   // **********************************************************************************
	   //__delay_cycles(120000);//Allow time for the 5TM to pull the data line low(startup) then high. Otherwise there will be a bug...


	   //Enable the falling edge interrupt
	   P_5TM_RX_IES |= c5TM_3_RX_PIN;
	   P_5TM_RX_IFG &= ~c5TM_3_RX_PIN;
	   P_5TM_RX_IE  |= c5TM_3_RX_PIN;

	   g_uc5TM_1_RXBufferIndex = 0;
	   //Sleep until measurement is done
	   while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex-1] != 0x0A)
	      __bis_SR_register(LPM0_bits); //CPU asleep.

	   //Disable Interrupt
	   P_5TM_RX_IE  &= ~c5TM_3_RX_PIN;
	   P_5TM_RX_IFG &= ~c5TM_3_RX_PIN;
	   //Turn off 5TM
	   P_5TM_PWR_OUT &= ~c5TM_3_PWR_PIN;//END exciting the 5TM1

	   TBCTL &= ~(MC0 | MC1 | TBIE | TBIFG);//Halt timer, Disable interrupts
	   TBCTL |= TBCLR;//Clear Timer
	   c5TM_ReadValue(3);
	   return c5TM_Test_Checksum(3);
	}
#endif
#if NUM_4_5TM_ON

char c5TM_Measure4()
{
	   // Clear the RX buffer and reset index WAS here, but I don't think necessary. Just a reminder it's an option...

	   TBCTL |= TBSSEL_2;
	   TBCTL &= ~TBSSEL_1; //Even though TBSSEL sets the bit we want, it doesn't unset the bit we don't want.
	   TBCTL |= TBCLR; //Clear Clock
	   P_5TM_PWR_OUT |= c5TM_4_PWR_PIN; //START exciting the 5TM1

	   // ******************Delay...*******************************************************
	   // Enable timer interrupt, configure for delay
	   TBCTL |= TBCLR; //Clear
	   TBCCTL1 |= CCIE;
	   TBCCR0 = 0;
	   TBCCR1 = 50000;//Equal to CPU delay 200000

	   TBCTL &= ~TBIFG;
	   TBCCTL1 &= ~TBIFG;//IFG

	   TBCTL |= MC1;//Continuous Mode

	   __bis_SR_register(LPM0_bits); //CPU asleep.

	   TBCTL &= ~TBIFG;
	   TBCCTL1 &= ~TBIFG;// Clear IFGs
	   // **********************************************************************************
	   //__delay_cycles(120000);//Allow time for the 5TM to pull the data line low(startup) then high. Otherwise there will be a bug...
/*	   //An experiment... use rising edge instead of delay. Not reliable in this form.
		g_DelayFlag = 1;
	   //Enable the rising edge interrupt
	   P_5TM_RX_IES &= ~c5TM_1_RX_PIN;
	   P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;
	   P_5TM_RX_IE  |= c5TM_1_RX_PIN;
	   __bis_SR_register(LPM0_bits); //CPU asleep. Wakes up when Rising Edge done (5TM Active).
	   g_DelayFlag = 0;
	   __delay_cycles(1000);//Allow some time for it to settle
*/
	   //Enable the falling edge interrupt
	   P_5TM_RX_IES |= c5TM_4_RX_PIN;
	   P_5TM_RX_IFG &= ~c5TM_4_RX_PIN;
	   P_5TM_RX_IE  |= c5TM_4_RX_PIN;

	   g_uc5TM_4_RXBufferIndex = 0;
	   //Sleep until measurement is done
	   while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex-1] != 0x0A)
	      __bis_SR_register(LPM0_bits); //CPU asleep.

	   //Disable Interrupt
	   P_5TM_RX_IE  &= ~c5TM_4_RX_PIN;
	   P_5TM_RX_IFG &= ~c5TM_4_RX_PIN;
	   //Turn off 5TM
	   P_5TM_PWR_OUT &= ~c5TM_4_PWR_PIN;//END exciting the 5TM1

	   TBCTL &= ~(MC0 | MC1 | TBIE | TBIFG);//Halt timer, Disable interrupts
	   TBCTL |= TBCLR;//Clear Timer
	   c5TM_ReadValue(4);
	   return c5TM_Test_Checksum(4);
	}
#endif
///////////////////////////////////////////////////////////////////////////////
//!   \brief Displays the values of the sensor readings to UART. From Buffer
//!			Comment out this function in final version
//!
//!
//!		Example:
//!# 		5TM Sensor: 1 / 2
//!# 		Soil Moisture RAW: xxxx
//!# 		Soil Moisture: xx.xx
//!# 		Temperature RAW: xxx
//!# 		Temperature: xx.x
//!#
//!# 		Fulfills Checksum / Does Not Fulfill Checksum
//!
//!   \param char arg: 	1: Show Sensor 1
//!						2: Show Sensor 2
//!						3: Show both Sensor 1 and 2
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
void v5TM_Display(char arg)
{
	int tempint;
	char temp;
	vUARTCOM_TXString("\r\n5TM Sensor: ",14);


	//SENSOR 2 **********************************************************
	if(arg==2){
		vUARTCOM_TXString("2\r\n",3);
		vUARTCOM_TXString("Soil Moisture RAW: ",19);
		g_uc5TM_2_RXBufferIndex = 0;
		do{
			vUARTCOM_TXString(&g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex],1);
			g_uc5TM_2_RXBufferIndex++;
		}while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] != ' ');

		//50-4094, /50 for dielectric
		vUARTCOM_TXString("\r\nSoil Moisture: ",17);
		temp = (un5TM_2_Soil / 1000)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_2_Soil % 1000;
		temp = (tempint / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		tempint = un5TM_2_Soil % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		temp = (un5TM_2_Soil % 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString("\r\nTemperature RAW: ",19);
		g_uc5TM_2_RXBufferIndex += 3;
		do{
			vUARTCOM_TXString(&g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex],1);
			g_uc5TM_2_RXBufferIndex++;
		}while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] != 0xD);

		//-40C- 50C
		vUARTCOM_TXString("\r\nTemperature: ",15);
		if(un5TM_2_Temperature_neg)
			vUARTCOM_TXString("-",1);

		temp = (un5TM_2_Temperature / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_2_Temperature % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		temp = (un5TM_2_Temperature % 10)+48;
		vUARTCOM_TXString(&temp, 1);
		vUARTCOM_TXString("C\r\n", 3);

		if(c5TM_Test_Checksum(2))
			vUARTCOM_TXString("\r\nFulfills Checksum\r\n",21);
		else
			vUARTCOM_TXString("\r\nDoes Not Fulfill Checksum\r\n",29);
		g_uc5TM_2_RXBufferIndex = 0;
	}else if(arg==1){
	//SENSOR 1 **********************************************************
		vUARTCOM_TXString("1\r\n",3);
		vUARTCOM_TXString("Soil Moisture RAW: ",19);

		g_uc5TM_1_RXBufferIndex = 0;
		do{
			vUARTCOM_TXString(&g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex],1);
			g_uc5TM_1_RXBufferIndex++;
		}while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] != ' ');

		//50-4094, /50 for dielectric
		vUARTCOM_TXString("\r\nSoil Moisture: ",17);
		temp = (un5TM_1_Soil / 1000)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_1_Soil % 1000;
		temp = (tempint / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		tempint = un5TM_1_Soil % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		temp = (un5TM_1_Soil % 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString("\r\nTemperature RAW: ",19);
		g_uc5TM_1_RXBufferIndex += 3;
		do{
			vUARTCOM_TXString(&g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex],1);
			g_uc5TM_1_RXBufferIndex++;
		}while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] != 0xD);

		//-40C- 50C
		vUARTCOM_TXString("\r\nTemperature: ",15);
		if(un5TM_1_Temperature_neg)
			vUARTCOM_TXString("-",1);

		temp = (un5TM_1_Temperature / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_1_Temperature % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		temp = (un5TM_1_Temperature % 10)+48;
		vUARTCOM_TXString(&temp, 1);
		vUARTCOM_TXString("C\r\n", 3);

		if(c5TM_Test_Checksum(1))
			vUARTCOM_TXString("\r\nFulfills Checksum\r\n",21);
		else
			vUARTCOM_TXString("\r\nDoes Not Fulfill Checksum\r\n",29);
		g_uc5TM_1_RXBufferIndex = 0;
	}
#if NUM_3_5TM_ON
		else if(arg==3){
	//SENSOR 3 **********************************************************
		vUARTCOM_TXString("3\r\n",3);
		vUARTCOM_TXString("Soil Moisture RAW: ",19);

		g_uc5TM_3_RXBufferIndex = 0;
		do{
			vUARTCOM_TXString(&g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex],1);
			g_uc5TM_3_RXBufferIndex++;
		}while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] != ' ');

		//50-4094, /50 for dielectric
		vUARTCOM_TXString("\r\nSoil Moisture: ",17);
		temp = (un5TM_3_Soil / 1000)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_3_Soil % 1000;
		temp = (tempint / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		tempint = un5TM_3_Soil % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		temp = (un5TM_3_Soil % 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString("\r\nTemperature RAW: ",19);
		g_uc5TM_3_RXBufferIndex += 3;
		do{
			vUARTCOM_TXString(&g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex],1);
			g_uc5TM_3_RXBufferIndex++;
		}while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] != 0xD);

		//-40C- 50C
		vUARTCOM_TXString("\r\nTemperature: ",15);
		if(un5TM_3_Temperature_neg)
			vUARTCOM_TXString("-",1);

		temp = (un5TM_3_Temperature / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_3_Temperature % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		temp = (un5TM_3_Temperature % 10)+48;
		vUARTCOM_TXString(&temp, 1);
		vUARTCOM_TXString("C\r\n", 3);

		if(c5TM_Test_Checksum(3))
			vUARTCOM_TXString("\r\nFulfills Checksum\r\n",21);
		else
			vUARTCOM_TXString("\r\nDoes Not Fulfill Checksum\r\n",29);
		g_uc5TM_3_RXBufferIndex = 0;
	}
#endif
#if NUM_4_5TM_ON
		else if(arg==4){
	//SENSOR 4 **********************************************************
		vUARTCOM_TXString("4\r\n",3);
		vUARTCOM_TXString("Soil Moisture RAW: ",19);

		g_uc5TM_4_RXBufferIndex = 0;
		do{
			vUARTCOM_TXString(&g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex],1);
			g_uc5TM_4_RXBufferIndex++;
		}while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] != ' ');

		//50-4094, /50 for dielectric
		vUARTCOM_TXString("\r\nSoil Moisture: ",17);
		temp = (un5TM_4_Soil / 1000)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_4_Soil % 1000;
		temp = (tempint / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		tempint = un5TM_4_Soil % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		temp = (un5TM_4_Soil % 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString("\r\nTemperature RAW: ",19);
		g_uc5TM_4_RXBufferIndex += 3;
		do{
			vUARTCOM_TXString(&g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex],1);
			g_uc5TM_4_RXBufferIndex++;
		}while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] != 0xD);

		//-40C- 50C
		vUARTCOM_TXString("\r\nTemperature: ",15);
		if(un5TM_4_Temperature_neg)
			vUARTCOM_TXString("-",1);

		temp = (un5TM_4_Temperature / 100)+48;
		vUARTCOM_TXString(&temp, 1);

		tempint = un5TM_4_Temperature % 100;
		temp = (tempint / 10)+48;
		vUARTCOM_TXString(&temp, 1);

		vUARTCOM_TXString(".", 1);

		temp = (un5TM_4_Temperature % 10)+48;
		vUARTCOM_TXString(&temp, 1);
		vUARTCOM_TXString("C\r\n", 3);

		if(c5TM_Test_Checksum(4))
			vUARTCOM_TXString("\r\nFulfills Checksum\r\n",21);
		else
			vUARTCOM_TXString("\r\nDoes Not Fulfill Checksum\r\n",29);
		g_uc5TM_4_RXBufferIndex = 0;
	}
#endif

}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Tests whether the returned Checksum is correct -> Transmission
//!		was free of errors, or the error compensates itself(possible but unlikely.)
//!
//!
//!   \param char arg: 	1: Evaluate Sensor 1
//!						2: Evaluate Sensor 2
//!						3: Evaluate both Sensor 1 and 2
//!
//!   \return 1: Checksum ok
//!			  0: Checksum fail or parameter fail
///////////////////////////////////////////////////////////////////////////////
char c5TM_Test_Checksum(char arg)
{
	int add = 0;
	if(arg==2)
	{
		g_uc5TM_2_RXBufferIndex = 0;

		do{
			add += g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex];
			g_uc5TM_2_RXBufferIndex++;
		} while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] != 0xD);
		add += g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex]; //Add the 0xD
		add += g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex+1]; //Add one more (x or z)
		add  = (add % 64) + 32;
		if(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex+2] == add)
			return 1;
	}else if(arg==1){
		g_uc5TM_1_RXBufferIndex = 0;

		do{
			add += g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex];
			g_uc5TM_1_RXBufferIndex++;
		} while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] != 0xD);
		add += g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex]; //Add the 0xD
		add += g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex+1]; //Add one more (x or z)
		add  = (add % 64) + 32;
		if(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex+2] == add)
			return 1;
	}
#if NUM_3_5TM_ON
	else if(arg==3){
		g_uc5TM_3_RXBufferIndex = 0;

		do{
			add += g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex];
			g_uc5TM_3_RXBufferIndex++;
		} while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] != 0xD);
		add += g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex]; //Add the 0xD
		add += g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex+1]; //Add one more (x or z)
		add  = (add % 64) + 32;
		if(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex+2] == add)
			return 1;
	}
#endif
#if NUM_4_5TM_ON
	else if(arg==4){
		g_uc5TM_4_RXBufferIndex = 0;

		do{
			add += g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex];
			g_uc5TM_4_RXBufferIndex++;
		} while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] != 0xD);
		add += g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex]; //Add the 0xD
		add += g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex+1]; //Add one more (x or z)
		add  = (add % 64) + 32;
		if(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex+2] == add)
			return 1;
	}
#endif
	return 0; //It was neither 1 or 2 or 3 or 4 in arg or it failed
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Reads Value in Buffer to actual temperature and soil moisture
//!		values. Temperature * 10 and Soil Moisture *100 to avoid floats
//!
//!		From 5TM in ASCII code: "XXXX 0 YYY(0xD)xZ(0xA)"
//!		XXXX is up to 4 positions of soil moisture
//!		YYY is up to 3 positions of Temperature Data
//!		Z is the checksum
//!		In parentheses means that the character is hexadecimal for an unprintable ASCII char.
//!		If the sensor were a 5TE, The '0' would show electrical conductivity, and the 'x' would be a 'z'.
//!
//!   \param char arg: 	1: Evaluate Sensor 1
//!						2: Evaluate Sensor 2
//!						3: Evaluate both Sensor 1 and 2
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
void c5TM_ReadValue(char arg)
{
	int mult = 1;
	int i;
	len = 0;
	if(arg ==2)
	{
		g_uc5TM_2_RXBufferIndex = 0;
		//Change ASCII values to dec number values
		while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] != ' '){
			g_uc5TM_2_RXBufferIndex++;
			len++;
		}
		un5TM_2_Soil = 0;
		for(i = g_uc5TM_2_RXBufferIndex-1; i>=0; i--){
			un5TM_2_Soil += ((g_uca5TM_2_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		g_uc5TM_2_RXBufferIndex += 3;//Skip the ' 0 '

		mult = 1;
		len = 0;
		while(g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] != 0xD){
			g_uc5TM_2_RXBufferIndex++;
			len++;
		}
		un5TM_2_Temperature = 0;
		for(i = g_uc5TM_2_RXBufferIndex-1; i>(g_uc5TM_2_RXBufferIndex-len-1); i--){
			un5TM_2_Temperature += ((g_uca5TM_2_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		//Now turn raw value into degC or soil moisture
		un5TM_2_Soil *= 2;// /100 to get decimal point value, the formula is RAW/50 to get correct value with 2 decimal points

		if(un5TM_2_Temperature<400)//
		{
			un5TM_2_Temperature_neg = 1;
			un5TM_2_Temperature = 400 - un5TM_2_Temperature;
		}else
			un5TM_2_Temperature -= 400;
	}else if(arg == 1){
		g_uc5TM_1_RXBufferIndex = 0;
		//Change ASCII values to dec number values
		while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] != ' '){
			g_uc5TM_1_RXBufferIndex++;
			len++;
		}
		un5TM_1_Soil = 0;
		for(i = g_uc5TM_1_RXBufferIndex-1; i>=0; i--){
			un5TM_1_Soil += ((g_uca5TM_1_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		g_uc5TM_1_RXBufferIndex += 3;//Skip the ' 0 '

		mult = 1;
		len = 0;
		while(g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] != 0xD){
			g_uc5TM_1_RXBufferIndex++;
			len++;
		}
		un5TM_1_Temperature = 0;
		for(i = g_uc5TM_1_RXBufferIndex-1; i>(g_uc5TM_1_RXBufferIndex-len-1); i--){
			un5TM_1_Temperature += ((g_uca5TM_1_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		//Now turn raw value into degC or soil moisture
		un5TM_1_Soil *= 2; // /100 to get decimal point value

		if(un5TM_1_Temperature<400) 	// /10 to get decimal point value
		{
			un5TM_1_Temperature_neg = 1;
			un5TM_1_Temperature = 400 - un5TM_1_Temperature;
		}else
			un5TM_1_Temperature -= 400;


	}

#if NUM_3_5TM_ON
	else if(arg == 3)
	{
		g_uc5TM_3_RXBufferIndex = 0;
		//Change ASCII values to dec number values
		while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] != ' '){
			g_uc5TM_3_RXBufferIndex++;
			len++;
		}
		un5TM_3_Soil = 0;
		for(i = g_uc5TM_3_RXBufferIndex-1; i>=0; i--){
			un5TM_3_Soil += ((g_uca5TM_3_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		g_uc5TM_3_RXBufferIndex += 3;//Skip the ' 0 '

		mult = 1;
		len = 0;
		while(g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] != 0xD){
			g_uc5TM_3_RXBufferIndex++;
			len++;
		}
		un5TM_3_Temperature = 0;
		for(i = g_uc5TM_3_RXBufferIndex-1; i>(g_uc5TM_3_RXBufferIndex-len-1); i--){
			un5TM_3_Temperature += ((g_uca5TM_3_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		//Now turn raw value into degC or soil moisture
		un5TM_3_Soil *= 2; // /100 to get decimal point value

		if(un5TM_3_Temperature<400) 	// /10 to get decimal point value
		{
			un5TM_3_Temperature_neg = 1;
			un5TM_3_Temperature = 400 - un5TM_3_Temperature;
		}else
			un5TM_3_Temperature -= 400;
	}
#endif
#if NUM_4_5TM_ON
	else if(arg == 4)
	{
		g_uc5TM_4_RXBufferIndex = 0;
		//Change ASCII values to dec number values
		while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] != ' '){
			g_uc5TM_4_RXBufferIndex++;
			len++;
		}
		un5TM_4_Soil = 0;
		for(i = g_uc5TM_4_RXBufferIndex-1; i>=0; i--){
			un5TM_4_Soil += ((g_uca5TM_4_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		g_uc5TM_4_RXBufferIndex += 3;//Skip the ' 0 '

		mult = 1;
		len = 0;
		while(g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] != 0xD){
			g_uc5TM_4_RXBufferIndex++;
			len++;
		}
		un5TM_4_Temperature = 0;
		for(i = g_uc5TM_4_RXBufferIndex-1; i>(g_uc5TM_4_RXBufferIndex-len-1); i--){
			un5TM_4_Temperature += ((g_uca5TM_4_RXBuffer[i]-48) * mult);
			if(mult == 1)
				mult = 10;
			else
				mult*=10;
		}

		//Now turn raw value into degC or soil moisture
		un5TM_4_Soil *= 2; // /100 to get decimal point value

		if(un5TM_4_Temperature<400) 	// /10 to get decimal point value
		{
			un5TM_4_Temperature_neg = 1;
			un5TM_4_Temperature = 400 - un5TM_4_Temperature;
		}else
			un5TM_4_Temperature -= 400;
	}
#endif
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Returns Soil Moisture value 1
//!
//!   \param arg - what sensor info to return
//!
//!   \return un5TM_1_Soil
///////////////////////////////////////////////////////////////////////////////
int i5TM_GetSoil(char arg)
{
	if(arg == 1)
		return un5TM_1_Soil;
	if(arg == 2)
		return un5TM_2_Soil;
	if(arg == 3)
		return un5TM_3_Soil;
	if(arg == 4)
		return un5TM_4_Soil;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//!   \brief Returns Soil Temperature value 1
//!
//!   \param arg - what sensor info to return
//!
//!   \return Temperature
///////////////////////////////////////////////////////////////////////////////
int i5TM_GetTemp(char arg)
{
	if(arg == 1)
	{
		int temp = un5TM_1_Temperature;
		if(un5TM_1_Temperature_neg)
			temp |= 0x80;
		return temp;
	}
	if(arg == 2)
	{
		int temp = un5TM_2_Temperature;
		if(un5TM_2_Temperature_neg)
			temp |= 0x80;
		return temp;
	}
	if(arg == 3)
	{
		int temp = un5TM_3_Temperature;
		if(un5TM_3_Temperature_neg)
			temp |= 0x80;
		return temp;
	}
	if(arg == 4)
	{
		int temp = un5TM_4_Temperature;
		if(un5TM_4_Temperature_neg)
			temp |= 0x80;
		return temp;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//!   \brief Second Interrupt Handler for TimerB (TimerB0 is the other)
//!
//!	  Timer is used to read the UART data from the sensor. Count down the bits
//!   until there are none left(Start, 8 bits, plus stop bit to make a byte,
//!   then finish. If more bytes are to be sent, a new IO interrupt will be
//!   called when the start bit comes. The byte index is incremented.
//!
//!   \param none
//!
//!   \return none
///////////////////////////////////////////////////////////////////////////////
#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
   TBCTL |= TBCLR; //Clear
   TBCTL |= MC1;//Continuous Mode
#if NUM_1_5TM_ON
   if(g_uc5TM1_RXBusy)
   {
      switch(g_uc5TM_RXBitsLeft)
      {
         case 0x00:
            // There are no bits left, so lets reset all the values and stop timer
            TBCTL &= ~(MC0 | MC1);
            P_5TM_RX_IE |= c5TM_1_RX_PIN;
            P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;
            g_uc5TM_1_RXBufferIndex++;
            g_uc5TM1_RXBusy = 0;
            //__bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a byte. (check if it's last later)
            //If it is not the last Byte, the core will put us back into LPM0, which won't stop the clocks, just the CPU
            break;

         case 0x01:
            if (P_5TM_RX_IN & c5TM_1_RX_PIN)
            	g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] &= ~0x80;

            break;

         default:
            if (P_5TM_RX_IN & c5TM_1_RX_PIN)
            	g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] &= ~0x80;

            g_uca5TM_1_RXBuffer[g_uc5TM_1_RXBufferIndex] >>= 1;
            break;
      }
   }
#endif
#if NUM_2_5TM_ON
   if(g_uc5TM2_RXBusy)
   {
      switch(g_uc5TM_RXBitsLeft)
      {
         case 0x00://Stop Bit
            // There are no bits left, so lets reset all the values and stop timer
            TACTL &= ~(MC0 | MC1);
            P_5TM_RX_IE |= c5TM_2_RX_PIN;
            P_5TM_RX_IFG &= ~c5TM_2_RX_PIN;
            g_uc5TM_2_RXBufferIndex++;
            g_uc5TM2_RXBusy = 0;
            //__bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a byte. (check if it's last later)
            //If it is not the last Byte, the core will put us back into LPM0, which won't stop the clocks, just the CPU
            break;

         case 0x01://Last Bit
            if (P_5TM_RX_IN & c5TM_2_RX_PIN)
            	g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] &= ~0x80;

            break;

         default://Regular bits
            if (P_5TM_RX_IN & c5TM_2_RX_PIN)
            	g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] &= ~0x80;

            g_uca5TM_2_RXBuffer[g_uc5TM_2_RXBufferIndex] >>= 1;
            break;
      }
   }
#endif
#if NUM_3_5TM_ON

   if(g_uc5TM3_RXBusy)
   {
      switch(g_uc5TM_RXBitsLeft)
      {
         case 0x00://Stop Bit
            // There are no bits left, so lets reset all the values and stop timer
            TACTL &= ~(MC0 | MC1);
            P_5TM_RX_IE |= c5TM_3_RX_PIN;
            P_5TM_RX_IFG &= ~c5TM_3_RX_PIN;
            g_uc5TM_3_RXBufferIndex++;
            g_uc5TM3_RXBusy = 0;
            //__bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a byte. (check if it's last later)
            //If it is not the last Byte, the core will put us back into LPM0, which won't stop the clocks, just the CPU
            break;

         case 0x01://Last Bit
            if (P_5TM_RX_IN & c5TM_3_RX_PIN)
            	g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] &= ~0x80;

            break;

         default://Regular bits
            if (P_5TM_RX_IN & c5TM_3_RX_PIN)
            	g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] &= ~0x80;

            g_uca5TM_3_RXBuffer[g_uc5TM_3_RXBufferIndex] >>= 1;
            break;
      }
   }
#endif
#if NUM_4_5TM_ON

   if(g_uc5TM4_RXBusy)
   {
      switch(g_uc5TM_RXBitsLeft)
      {
         case 0x00://Stop Bit
            // There are no bits left, so lets reset all the values and stop timer
            TACTL &= ~(MC0 | MC1);
            P_5TM_RX_IE |= c5TM_4_RX_PIN;
            P_5TM_RX_IFG &= ~c5TM_4_RX_PIN;
            g_uc5TM_4_RXBufferIndex++;
            g_uc5TM4_RXBusy = 0;
            //__bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a byte. (check if it's last later)
            //If it is not the last Byte, the core will put us back into LPM0, which won't stop the clocks, just the CPU
            break;

         case 0x01://Last Bit
            if (P_5TM_RX_IN & c5TM_4_RX_PIN)
            	g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] &= ~0x80;

            break;

         default://Regular bits
            if (P_5TM_RX_IN & c5TM_4_RX_PIN)
            	g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] |= 0x80;
            else
            	g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] &= ~0x80;

            g_uca5TM_4_RXBuffer[g_uc5TM_4_RXBufferIndex] >>= 1;
            break;
      }
   }
#endif
   g_uc5TM_RXBitsLeft--;
   TBCCTL1 &= ~0x1;//IFG CLear

   if(!g_uc5TM1_RXBusy && !g_uc5TM2_RXBusy && !g_uc5TM3_RXBusy && !g_uc5TM4_RXBusy)
   {
 	  __bic_SR_register_on_exit(LPM4_bits); //Exit LPM if it was neither RX
   }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{

#if NUM_1_5TM_ON

   if(P_5TM_RX_IFG & c5TM_1_RX_PIN)//P1IFG & BIT3
   {
	  timeoutcounter = 0;
      // Delay for half bit, this ensures we start sampling at the middle of
      // each bit
      TBCTL &= ~(MC0 | MC1 | TAIE | TAIFG);//Halt timer, Disable interrupts
      TBCTL |= TBCLR;//Clear Timer
      TBCCTL1 &= ~CCIFG;
      TBCCTL1 &= ~CCIE;//Disable Compare interrupt
      TBCCTL0 &= ~CCIE;//Disable Compare interrupt
      TBCCR0 = BAUD_1200_DELAY; //Input delay time in compare register
      TBCTL |= MC_1;//Start timer in UP Mode (Counts to TBCCR0 then starts at 0 again)
      while(!(TBCTL & TBIFG)); //Wait until Compare register is met
      TBCTL &= ~TBIFG;
	  // Enable timer interrupt, configure for baud rate
      TBCTL &= ~(MC0 | MC1);//Halt timer
      TBCTL |= TBCLR; //Clear
      TBCCTL1 |= CCIE;
      TBCCR0 = 0;
      TBCCR1 = BAUD_1200;
	  __bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a start bit.
	  // Disable interrupt on RX
      P_5TM_RX_IE &= ~c5TM_1_RX_PIN;
	  //*****************
      g_uc5TM_RXBitsLeft = 0x08;
      g_uc5TM1_RXBusy = 1;
      //*****************
      TBCTL |= MC1;//Continuous Mode
	  //Clear Interrupt Flag
      TBCCTL1 &= ~TBIFG;//IFG
      P_5TM_RX_IFG &= ~c5TM_1_RX_PIN;

   }//END if(P_5TM_RX_IFG & c5TM_1_RX_PIN)//P2IFG & BIT4

#endif
#if NUM_2_5TM_ON

   if(P_5TM_RX_IFG & c5TM_2_RX_PIN)//P1IFG & BIT4
   {
	   timeoutcounter = 0;
      // Delay for half bit, this ensures we start sampling at the middle of
      // each bit
      TBCTL &= ~(MC0 | MC1 | TAIE | TAIFG);//Halt timer, Disable interrupts
      TBCTL |= TBCLR;//Clear Timer
      TBCCTL1 &= ~CCIFG;
      TBCCTL1 &= ~CCIE;//Disable Compare interrupt
      TBCCTL0 &= ~CCIE;//Disable Compare interrupt
      TBCCR0 = BAUD_1200_DELAY; //Input delay time in compare register
      TBCTL |= MC_1;//Start timer in UP Mode (Counts to TBCCR0 then starts at 0 again)

      while(!(TBCTL & TBIFG)); //Wait until Compare register is met

      TBCTL &= ~TBIFG;
	  // Enable timer interrupt, configure for baud rate
      TBCTL &= ~(MC0 | MC1);//Halt timer
      TBCTL |= TBCLR; //Clear
      TBCCTL1 |= CCIE;
      TBCCR0 = 0;
      TBCCR1 = BAUD_1200;
	  __bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a start bit.
	  // Disable interrupt on RX, don't need them until the next start
      P_5TM_RX_IE &= ~c5TM_2_RX_PIN;
	  //*****************
      g_uc5TM_RXBitsLeft = 0x08;
      g_uc5TM2_RXBusy=1;
      //*****************
      TBCTL |= MC1;//Continuous Mode
	  //Clear Interrupt Flag
      TBCCTL1 &= ~TBIFG;//IFG
      P_5TM_RX_IFG &= ~c5TM_2_RX_PIN;
   }//END if(P_5TM_RX_IFG & c5TM_2_RX_PIN)//P2IFG & BIT4

#endif
#if NUM_3_5TM_ON

   if(P_5TM_RX_IFG & c5TM_3_RX_PIN)//P1IFG & BIT
   {
	   timeoutcounter = 0;
      // Delay for half bit, this ensures we start sampling at the middle of
      // each bit
      TBCTL &= ~(MC0 | MC1 | TAIE | TAIFG);//Halt timer, Disable interrupts
      TBCTL |= TBCLR;//Clear Timer
      TBCCTL1 &= ~CCIFG;
      TBCCTL1 &= ~CCIE;//Disable Compare interrupt
      TBCCTL0 &= ~CCIE;//Disable Compare interrupt
      TBCCR0 = BAUD_1200_DELAY; //Input delay time in compare register
      TBCTL |= MC_1;//Start timer in UP Mode (Counts to TBCCR0 then starts at 0 again)

      while(!(TBCTL & TBIFG)); //Wait until Compare register is met

      TBCTL &= ~TBIFG;
	  // Enable timer interrupt, configure for baud rate
      TBCTL &= ~(MC0 | MC1);//Halt timer
      TBCTL |= TBCLR; //Clear
      TBCCTL1 |= CCIE;
      TBCCR0 = 0;
      TBCCR1 = BAUD_1200;
	  __bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a start bit.
	  // Disable interrupt on RX, don't need them until the next start
      P_5TM_RX_IE &= ~c5TM_3_RX_PIN;
	  // *****************
      g_uc5TM_RXBitsLeft = 0x08;
      g_uc5TM3_RXBusy=1;
      // *****************
      TBCTL |= MC1;//Continuous Mode
	  //Clear Interrupt Flag
      TBCCTL1 &= ~TBIFG;//IFG
      P_5TM_RX_IFG &= ~c5TM_3_RX_PIN;
   }//END if(P_5TM_RX_IFG & c5TM_3_RX_PIN)//P2IFG & BIT

#endif
#if NUM_4_5TM_ON

   if(P_5TM_RX_IFG & c5TM_4_RX_PIN)//P1IFG & BIT
   {
	   timeoutcounter = 0;
      // Delay for half bit, this ensures we start sampling at the middle of
      // each bit
      TBCTL &= ~(MC0 | MC1 | TAIE | TAIFG);//Halt timer, Disable interrupts
      TBCTL |= TBCLR;//Clear Timer
      TBCCTL1 &= ~CCIFG;
      TBCCTL1 &= ~CCIE;//Disable Compare interrupt
      TBCCTL0 &= ~CCIE;//Disable Compare interrupt
      TBCCR0 = BAUD_1200_DELAY; //Input delay time in compare register
      TBCTL |= MC_1;//Start timer in UP Mode (Counts to TBCCR0 then starts at 0 again)

      while(!(TBCTL & TBIFG)); //Wait until Compare register is met

      TBCTL &= ~TBIFG;
	  // Enable timer interrupt, configure for baud rate
      TBCTL &= ~(MC0 | MC1);//Halt timer
      TBCTL |= TBCLR; //Clear
      TBCCTL1 |= CCIE;
      TBCCR0 = 0;
      TBCCR1 = BAUD_1200;
	  __bic_SR_register_on_exit(LPM4_bits); //All Clocks and CPU etc awake now that we received a start bit.
	  // Disable interrupt on RX, don't need them until the next start
      P_5TM_RX_IE &= ~c5TM_4_RX_PIN;
	  // *****************
      g_uc5TM_RXBitsLeft = 0x08;
      g_uc5TM4_RXBusy=1;
      // *****************
      TBCTL |= MC1;//Continuous Mode
	  //Clear Interrupt Flag
      TBCCTL1 &= ~TBIFG;//IFG
      P_5TM_RX_IFG &= ~c5TM_4_RX_PIN;
   }//END if(P_5TM_RX_IFG & c5TM_4_RX_PIN)//P2IFG & BIT
#endif

}
//! @}




