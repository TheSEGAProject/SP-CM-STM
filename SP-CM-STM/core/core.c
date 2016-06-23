///////////////////////////////////////////////////////////////////////////////
//! \file core.c
//! \brief This is the primary file for the SP Board core
//!
//! This file contains the heart of the SP Board core. The function pointer
//! and label tables are kept here and maintained by the core.
//!
//! @addtogroup core Core
//! The Core Module handles all of the communication to the CP board as well
//! as acts as the supervisor to all activities on the SP Board. The user
//! built wrapper should interface with the Core Module as documented.
//! @{
///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
// By: Kenji Yamamoto
//     Wireless Networks Research Lab
//     Dept of Electrical Engineering, CEFNS
//     Northern Arizona University
//
//*****************************************************************************

#include <msp430x23x.h>
#include "core.h"

//******************  Software version variables  ***************************//
//! @name Software Version Variables
//! These variables contain ID and version information.
//! @{
//! \var static const uint8 g_ucaCoreVersion[VERSION_LABEL_LEN]
//! \brief The name and version of the core
//static const uint8 g_ucaCoreVersion[VERSION_LABEL_LEN] = "SP-Core v1.06   ";
#define VERSION_LABEL "SP-Core v2.00   "

//! \var uint8 g_ucaWrapperVersion[VERSION_LABEL_LEN]
//! \brief Byte array to store the name and version of the wrapper.
//uint8 g_ucaWrapperVersion[VERSION_LABEL_LEN];
//! @}

//! @name Transducer Label Variables
//! These variables are where the labels for the transducers are stored.
//! @{
//******************  Transducer Labels  ************************************//
//! \var uint8 g_ucaTransducerLabels[MAX_NUM_TRANSDUCERS][TRANSDUCER_LABEL_LEN]
//! \brief Array of byte arrays to store the transducer labels

//uint8 g_ucaTransducerLabels[MAX_NUM_TRANSDUCERS][TRANSDUCER_LABEL_LEN];


//! @name Core Data Array
  //! This array is where the information passed between the transducer function
  //! and the core stored.
  //! @var g_unaCoreData

  uint16 g_unaCoreData[8]={0,0,0,0,0,0,0,0};


//******************  Message Buffers  **************************************//
//! @name Message Buffer Variables
//! These variables are for the incoming and outgoing messages.
//! @{
//! \var union SP_32BitDataMessage  g_32DataMsg
//! \brief All incoming and outgoing 32 Bit data messages get stored in this variable
union SP_32BitDataMessage  g_32DataMsg;

//! \var union SP_128BitDataMessage  g_128DataMsg
//! \brief All incoming and outgoing 128 Bit data messages get stored in this variable
union SP_128BitDataMessage g_128DataMsg; //-scb

//! \var union SP_LabelMessage g_LabelMsg;
//! \brief All outgoing label messages get stored in this variable
union SP_LabelMessage g_LabelMsg;
//! @}

//******************  Sensor Function Table  ********************************//
//! @name Sensor Function Table Variable
//! This table contains the pointers to the functions used to measure the
//! various transducers.
//! @{
//! \var p_TransducerFunction gp_tfSensorTable[MAX_NUM_TRANSDUCERS]
//! \brief The table that stores the sensor function pointers
p_TransducerFunction gp_tfSensorTable[MAX_NUM_TRANSDUCERS];
//! @}

//******************  Functions  ********************************************//
///////////////////////////////////////////////////////////////////////////////
//! \brief This function starts up the Core and configures hardware & RAM
//!
//! All of the clock setup and initial port configuration is handled in this
//! function. At the end of the function, before the return, any additional
//! core initilization functions are called.
//!   \param None.
//!   \return None.
///////////////////////////////////////////////////////////////////////////////
void vCORE_Initilize(void)
{
  // First, stop the watchdog
  WDTCTL = WDTPW + WDTHOLD;

  // Configure DCO for 16 MHz
  DCOCTL  = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;


  // Configure VLO
  BCSCTL3 = 0x00;

  // MCLK = DCO/1    SMCLK = DCO / 4
  BCSCTL2 = SELM_0 | DIVM_0 | DIVS_2;

  // ACLK = VLO / 4 = ~3 kHz
  BCSCTL3 |= LFXT1S_2;
  BCSCTL1 &= ~(XT2OFF + XTS);// = 0xC0
  BCSCTL1 |= DIVA1;// = 0x20


  // TODO: Figure out which I/O's need to be configured as non
  //       digital I/O's to reduce power consumption

  if(CoreP1OUT == DEF)
	  P1OUT = 0x02;				 // P1.2 is the TX line, which needs to be pulled high.
  else
  {
	  P1OUT = CoreP1OUT;
	  P1OUT |= 0x02;
  }
  if(CoreP1DIR == DEF)
	  P1DIR = 0xFF;				 // Port 1 - all digital I/O, all outputs,
  else
	  P1DIR = CoreP1DIR;
  P1REN = 0x00;              //      all as low, pull up / pull downs disabled
  P1SEL = 0x00;

  if(CoreP2OUT == DEF)
	  P2OUT = 0x00;
  else
	  P2OUT = CoreP2OUT;
  if(CoreP2DIR == DEF)
	  P2DIR = 0xFE;				 // Port 2 - all digital I/O, all outputs,
  else
  {
	  P2DIR = CoreP2DIR;
	  P2DIR &= ~0x1;			//Port 2.0 must be input since it is the Interrupt line from the CP.
  }
  P1REN = 0x00;              //      all as low, pull up / pull downs disabled
  P2SEL = 0x00;

  if(CoreP3OUT == DEF)
	  P3OUT = 0x00;
  else
	  P3OUT = CoreP3OUT;
  if(CoreP3DIR == DEF)
	  P3DIR = 0xFF;				 // Port 3 - all digital I/O, all outputs,
  else
	  P3DIR = CoreP3DIR;
  P3REN = 0x00;              //      all as low, pull up / pull downs disabled
  P3SEL = 0x00;

  if(CoreP4OUT == DEF)
	  P4OUT = 0x00;
  else
	  P4OUT = CoreP4OUT;
  if(CoreP4DIR == DEF)
	  P4DIR = 0xFF;				 // Port 4 - all digital I/O, all outputs,
  else
	  P4DIR = CoreP4DIR;
  P4REN = 0x00;              //      all as low, pull up / pull downs disabled
  P4SEL = 0x00;

  if(CoreP5OUT == DEF)
	  P5OUT = 0x00;
  else
	  P5OUT = CoreP5OUT;
  if(CoreP5DIR == DEF)
	  P5DIR = 0xFF;				 // Port 5 - all digital I/O, all outputs,
  else
	  P5DIR = CoreP5DIR;
  P5REN = 0x00;              //      all as low, pull up / pull downs disabled
  P5SEL = 0x00;

  if(CoreP6OUT == DEF)
	  P6OUT = 0x00;
  else
	  P6OUT = CoreP6OUT;
  if(CoreP6DIR == DEF)
	  P6DIR = 0xFF;				 // Port 6 - all digital I/O, all outputs,
  else
	  P6DIR = CoreP6DIR;
  P6REN = 0x00;              //      all as low, pull up / pull downs disabled
  P6SEL = 0x00;

  // All core modules get initilized now
  vCORE_InitilizeTransducerTable();
  vCOMM_Init(BAUD_115200);//BAUD_57600    BAUD_115200

  // Enable interrupts
  __bis_SR_register(GIE);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Initilizes the transducer table
//!
//! The pointers to the transducer functions are assigned here
//!
//!   \param None.
//!   \return None.
//!   \sa vCORE_Initilize
///////////////////////////////////////////////////////////////////////////////
void vCORE_InitilizeTransducerTable(void)
{

  gp_tfSensorTable[0] = TRANSDUCER_0_FUNCTION;
  gp_tfSensorTable[1] = TRANSDUCER_1_FUNCTION;
  gp_tfSensorTable[2] = TRANSDUCER_2_FUNCTION;
  gp_tfSensorTable[3] = TRANSDUCER_3_FUNCTION;
  gp_tfSensorTable[4] = TRANSDUCER_4_FUNCTION;
  gp_tfSensorTable[5] = TRANSDUCER_5_FUNCTION;
  gp_tfSensorTable[6] = TRANSDUCER_6_FUNCTION;
  gp_tfSensorTable[7] = TRANSDUCER_7_FUNCTION;
  gp_tfSensorTable[8] = TRANSDUCER_8_FUNCTION;
  gp_tfSensorTable[9] = TRANSDUCER_9_FUNCTION;
  gp_tfSensorTable[10] = TRANSDUCER_A_FUNCTION;
  gp_tfSensorTable[11] = TRANSDUCER_B_FUNCTION;
  gp_tfSensorTable[12] = TRANSDUCER_C_FUNCTION;
  gp_tfSensorTable[13] = TRANSDUCER_D_FUNCTION;
  gp_tfSensorTable[14] = TRANSDUCER_E_FUNCTION;
  gp_tfSensorTable[15] = TRANSDUCER_F_FUNCTION;

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Measure the MSP430 supply voltage
//!
//! Uses the ADC12 to meausre the input voltage. Uses the MEM15 register.
//!
//!   \param none
//!
//!   \return insigned int Input voltage * 100
///////////////////////////////////////////////////////////////////////////////

unsigned int unCORE_GetVoltage(void)
{
   int rt_volts;

   ADC12CTL0 &= ~(SHT10 + SHT12 + SHT13 + MSC + ADC12OVIE + ADC12TOVIE + ENC + ADC12SC);//ADC12CTL0 &= ~0xD08F = ~1101 0000 1000 1111 //Have to turn ENC Off first
   ADC12CTL0 |= (SHT11 + REF2_5V + REFON + ADC12ON); //ADC12CTL0 |= 0x2070 = 0010 xxxx 0111 00(11)* - 16-Cycle Hold time + Single Conversion + 2.5V Ref + RefON + ADC ON + Interrupts off + (Enable + Start)
   ADC12CTL1 &= ~(SHS1 + SHS0 + ISSH + ADC12DIV2 + ADC12DIV1 + ADC12DIV0 + ADC12SSEL1 + ADC12SSEL0 + CONSEQ1 + CONSEQ0);//ADC12CTL1 &= ~0x0FDE = ~0000 1101 1111 1110
   ADC12MEM15 = 0;
   ADC12MCTL15 |= (SREF0 + INCH3 + INCH1 + INCH0); //ADC12MCTL15 |= 0x1B = x001 1011 - Reference Select + Input Select
   ADC12MCTL15 &= ~(SREF2 + SREF1 + INCH2); // ADC12MCTL15 &= ~0x64 = 0110 0100
   ADC12IE &= ~0x8000; //Turn off IE and clear IFG
   ADC12IFG &= ~0x8000;

   __delay_cycles(1000);

   ADC12CTL1 |= (CSTARTADD3 + CSTARTADD2 + CSTARTADD1 + CSTARTADD0 + SHP); //ADC12CTL1 |= 0xF200 = 1111 0010 0000 000x - MEM15 + Internal OSC CLK + Single-Channel, Single-conversion
   ADC12CTL0 |= ENC + ADC12SC;             // Sampling and conversion start

   while(!(ADC12IFG & 0x8000));//End when something is written in. Can't sleep because we wanted to keep interrupts for users (not in core)

   rt_volts = ADC12MEM15; //(0.5*Vin)/2.5V * 4095
   ADC12IFG &= ~0x8000; //Unset IFG Flag
   ADC12CTL0 &= ~ENC;
   ADC12CTL0 &= ~(REFON + ADC12ON);        // turn off A/D to save power

   rt_volts=(rt_volts*5)/41;
   return (rt_volts);
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Set the label for a specific transducer
//!
//! Sets the label for a particular transducer
//!   \param ucTransducer The transducer number to label
//!   \param p_ucaLabel A pointer to a 16-byte array containing the label
//!   \return None.
///////////////////////////////////////////////////////////////////////////////
//Not needed anymore
/*void vCORE_SetTransducerLabel(uint8 ucTransducer, uint8 * p_ucaLabel)
{
  uint8 ucLoopCount;

  for (ucLoopCount = 0x00; ucLoopCount < TRANSDUCER_LABEL_LEN; ucLoopCount++)
    g_ucaTransducerLabels[ucTransducer][ucLoopCount] = p_ucaLabel[ucLoopCount];
}*/

///////////////////////////////////////////////////////////////////////////////
//! \brief Sets the wrapper software title
//
//! This function tells the core the software version of the wrapper.
//!   \param p_ucaVersion Pointer to the 16-byte descriptor
//!   \return None.
///////////////////////////////////////////////////////////////////////////////
//Not needed anymore
/*
void vCORE_SetWrapperSoftwareString(uint8 * p_ucaVersion)
{
  uint8 ucLoopCount;

  for (ucLoopCount = 0x00; ucLoopCount < VERSION_LABEL_LEN; ucLoopCount++)
    g_ucaWrapperVersion[ucLoopCount] = p_ucaVersion[ucLoopCount];
}*/

///////////////////////////////////////////////////////////////////////////////
//! \brief Assigns a function call for a particular transducer
//!
//! This function assigns the passed function pointer \e p_tfFunction to the
//! passed transducer number \e ucNumber in the sensor table.
//!   \param ucNumber The transducer number to assign.
//!   \param p_tfFunction The function pointer to assign to \e ucNumber
//!   \return None.
//!   \sa core.h
///////////////////////////////////////////////////////////////////////////////
//Not needed anymore
/*void vCORE_AssignFunctionToTransducer(uint8 ucNumber,
                                      p_TransducerFunction p_tfFunction)
{
  gp_tfSensorTable[ucNumber] = p_tfFunction;
}*/

///////////////////////////////////////////////////////////////////////////////
//! \brief Send the confirm packet
//!
//!  Confirm packet includes all the data received so CP Board can confirm it
//!  is correct.
//!
//!   \param none
//!   \sa core.h
///////////////////////////////////////////////////////////////////////////////
vCORE_Send_ConfirmPKT()
{
	// Send confirm packet that we received message
	g_32DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION; //-scb STOP HERE
	g_32DataMsg.fields.ucMsgType = CONFIRM_COMMAND;
	g_32DataMsg.fields.ucMsgSize = SP_32BITDATAMESSAGE_SIZE;

	// The rest of the packet should be what was sent so CP Board can confirm
	// correct reception

	vCOMM_Send32BitDataMessage(&g_32DataMsg);
	//UARTDELETE
	vUARTCOM_TXString("Sent Confirm Packet.\r\n",22);
}


///////////////////////////////////////////////////////////////////////////////
//! \brief This functions runs the core
//!
//! This function runs the core. This function does not return, so all of the
//! core setup and init must be done before the call to this function. The
//! function waits for a data packet from the CP Board, then parses and handles
//! it appropriately. A response packet is then sent and the core waits for
//! the next data packet.
//!   \param None.
//!   \return NEVER. This function never returns
//!   \sa msg.h
///////////////////////////////////////////////////////////////////////////////
void vCORE_Run(void)
{
  uint8 ucLoopCount;
  uint16 unTransducerReturn; //The return parameter from the transducer function

  // First, tell the CP Board that we are ready for commands
  g_32DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION; //-scb
  g_32DataMsg.fields.ucMsgType = ID_PKT;
  g_32DataMsg.fields.ucMsgSize = SP_32BITDATAMESSAGE_SIZE;
  g_32DataMsg.fields.ucSensorNumber = ID_PKT_CODE;

  //ID_PKT content to tell CP Board to expect 128 bit data return packets
  g_32DataMsg.fields.ucData1_HI_BYTE = ID_PKT_HI_BYTE1;
  g_32DataMsg.fields.ucData1_LO_BYTE = ID_PKT_LO_BYTE1;
  g_32DataMsg.fields.ucData2_HI_BYTE = ID_PKT_HI_BYTE2;
  g_32DataMsg.fields.ucData2_LO_BYTE = ID_PKT_LO_BYTE2;

  if(unCORE_GetVoltage() < MIN_VOLTAGE)
  {
	  g_32DataMsg.fields.ucData2_HI_BYTE = 0xBA;
	  g_32DataMsg.fields.ucData2_LO_BYTE = 0xD1;
  }
  //Original ID_PKT content
  //g_32DataMsg.fields.ucData1_HI_BYTE = 0xAB;
  //g_32DataMsg.fields.ucData1_LO_BYTE = 0xCD;
  //g_32DataMsg.fields.ucData2_HI_BYTE = 0xEF;
  //g_32DataMsg.fields.ucData2_LO_BYTE = 0x12;


  // Send the message
  vCOMM_Send32BitDataMessage(&g_32DataMsg);
  //UARTDELETE
  vUARTCOM_TXString("ID_PKT sent.\r\n",14);

  // The primary execution loop
  while(TRUE)
  {
    vCOMM_WaitFor32BitDataMessage();//Wait to receive 64 bits (32 bit header, 32 bit data)
    ucCOMM_Grab32BitDataMessageFromBuffer(&g_32DataMsg);
    //UARTDELETE
    vUARTCOM_TXString("Got Message from CP.\r\n",22);

    switch(g_32DataMsg.fields.ucMsgType)
    {
      case COMMAND_PKT:
    	  //UARTDELETE
    	vUARTCOM_TXString("COMMAND_PKT Received\r\n",22);
    	unTransducerReturn = 0; //default return value to 0
    	//-scb everything in here got changed pretty much
    	switch(g_32DataMsg.fields.ucSensorNumber)
    	{
    		// For each transducer, get the return value by using the function
    		// pointer table
    		case TRANSDUCER_0:
   			case TRANSDUCER_1:
 			case TRANSDUCER_2:
 			case TRANSDUCER_3:
 			case TRANSDUCER_4:
 			case TRANSDUCER_5:
 			case TRANSDUCER_6:
 			case TRANSDUCER_7:
 			case TRANSDUCER_8:
 			case TRANSDUCER_9:
 			case TRANSDUCER_A:
 			case TRANSDUCER_B:
 			case TRANSDUCER_C:
 			case TRANSDUCER_D:
 			case TRANSDUCER_E:
 			case TRANSDUCER_F:
 			default:
           		break;
    	}// END: switch(g_32DataMsg.fields.ucSensorNumber)


    	if(gp_tfSensorTable[g_32DataMsg.fields.ucSensorNumber] != NULL)
    		vCORE_Send_ConfirmPKT();

		g_unaCoreData[0]=
			(((uint16)g_32DataMsg.fields.ucData1_HI_BYTE) << 8) +
			((uint16)g_32DataMsg.fields.ucData1_LO_BYTE);

		g_unaCoreData[1]=
			(((uint16)g_32DataMsg.fields.ucData2_HI_BYTE) << 8) +
			((uint16)g_32DataMsg.fields.ucData2_LO_BYTE);

		g_unaCoreData[2]= 0;
		g_unaCoreData[3]= 0;
		g_unaCoreData[4]= 0;
		g_unaCoreData[5]= 0;
		g_unaCoreData[6]= 0;
		g_unaCoreData[7]= 0;

		unTransducerReturn = //if everything went ok, unTransducerReturn > 0;
			(*gp_tfSensorTable[g_32DataMsg.fields.ucSensorNumber])(g_unaCoreData); //pass on g_unaCoreData.

		break; //END COMMAND_PKT

    case REQUEST_DATA:

#if SP_PACKET_SIZE_128
        // Now send message back to CP Board
        g_128DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION;
        g_128DataMsg.fields.ucMsgType = REPORT_DATA;
        g_128DataMsg.fields.ucMsgSize = SP_128BITDATAMESSAGE_SIZE;
        g_128DataMsg.fields.ucSensorNumber = g_32DataMsg.fields.ucSensorNumber;

        //unTransducerArray is an 'OK' message.
        //If not >0, then send error message in code
        if(!unTransducerReturn){
        	g_128DataMsg.fields.ucMsgType = REPORT_ERROR;
        }
        //collect data from g_unaCoreData

        g_128DataMsg.fields.ucData1_HI_BYTE = (uint8)(g_unaCoreData[0] >> 8);
        g_128DataMsg.fields.ucData1_LO_BYTE = (uint8)g_unaCoreData[0];
        g_128DataMsg.fields.ucData2_HI_BYTE = (uint8)(g_unaCoreData[1] >> 8);
        g_128DataMsg.fields.ucData2_LO_BYTE = (uint8)g_unaCoreData[1];
        g_128DataMsg.fields.ucData3_HI_BYTE = (uint8)(g_unaCoreData[2] >> 8);
        g_128DataMsg.fields.ucData3_LO_BYTE = (uint8)g_unaCoreData[2];
        g_128DataMsg.fields.ucData4_HI_BYTE = (uint8)(g_unaCoreData[3] >> 8);
        g_128DataMsg.fields.ucData4_LO_BYTE = (uint8)g_unaCoreData[3];
        g_128DataMsg.fields.ucData5_HI_BYTE = (uint8)(g_unaCoreData[4] >> 8);
        g_128DataMsg.fields.ucData5_LO_BYTE = (uint8)g_unaCoreData[4];
        g_128DataMsg.fields.ucData6_HI_BYTE = (uint8)(g_unaCoreData[5] >> 8);
        g_128DataMsg.fields.ucData6_LO_BYTE = (uint8)g_unaCoreData[5];
        g_128DataMsg.fields.ucData7_HI_BYTE = (uint8)(g_unaCoreData[6] >> 8);
        g_128DataMsg.fields.ucData7_LO_BYTE = (uint8)g_unaCoreData[6];
        g_128DataMsg.fields.ucData8_HI_BYTE = (uint8)(g_unaCoreData[7] >> 8);
        g_128DataMsg.fields.ucData8_LO_BYTE = (uint8)g_unaCoreData[7];
        // Send the message
        vCOMM_Send128BitDataMessage(&g_128DataMsg);
        //UARTDELETE
        vUARTCOM_TXString("Sent Return Message\r\n",21);
#else
        // Now send message back to CP Board
        g_32DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION;
        g_32DataMsg.fields.ucMsgType = REPORT_DATA;
        g_32DataMsg.fields.ucMsgSize = SP_32BITDATAMESSAGE_SIZE;


        //unTransducerArray is an 'OK' message.
        //If not >0, then send error message in code
        if(unTransducerReturn){
        //collect data from g_unaCoreData
        	g_32DataMsg.fields.ucData1_HI_BYTE = (uint8)(g_unaCoreData[0] >> 8);
        	g_32DataMsg.fields.ucData1_LO_BYTE = (uint8)g_unaCoreData[0];
        	g_32DataMsg.fields.ucData2_HI_BYTE = (uint8)(g_unaCoreData[1] >> 8);
        	g_32DataMsg.fields.ucData2_LO_BYTE = (uint8)g_unaCoreData[1];
        }
        else
        {//This is my ERRORMSG (EBB0B356) for now :P
        	g_32DataMsg.fields.ucData1_HI_BYTE = 0xEB;
        	g_32DataMsg.fields.ucData1_LO_BYTE = 0xB0;
        	g_32DataMsg.fields.ucData2_HI_BYTE = 0xB3;
        	g_32DataMsg.fields.ucData2_LO_BYTE = 0x56;
        }
        // Send the message
        vCOMM_Send32BitDataMessage(&g_32DataMsg);
        //UARTDELETE
        vUARTCOM_TXString("Sent Data Message\r\n",19);



#endif
        break; //END REQUEST_DATA

      case REQUEST_LABEL:
        // Format first part of return message
        g_LabelMsg.fields.ucMsgVersion = SP_LABELMESSAGE_VERSION;
        g_LabelMsg.fields.ucMsgType = REPORT_LABEL;
        g_LabelMsg.fields.ucMsgSize = SP_LABELMESSAGE_SIZE;
        g_LabelMsg.fields.ucSensorNumber = g_32DataMsg.fields.ucSensorNumber;

        switch(g_LabelMsg.fields.ucSensorNumber)
        {
          // For each transducer, use the table to get the label
          case TRANSDUCER_0_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_0_LABEL_TXT[ucLoopCount];
              break;
          case TRANSDUCER_1_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_1_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_2_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_2_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_3_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_3_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_4_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_4_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_5_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_5_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_6_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_6_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_7_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_7_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_8_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_8_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_9_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_9_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_A_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_A_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_B_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_B_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_C_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_C_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_D_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_D_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_E_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_E_LABEL_TXT[ucLoopCount];
              break;

          case TRANSDUCER_F_LABEL:
              for (ucLoopCount = 0x00;
                   ucLoopCount < TRANSDUCER_LABEL_LEN;
                   ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = TRANSDUCER_F_LABEL_TXT[ucLoopCount];
              break;

          case SP_CORE_VERSION:
            for (ucLoopCount = 0x00;
                 ucLoopCount < VERSION_LABEL_LEN;
                 ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = VERSION_LABEL[ucLoopCount];
            break;

          case WRAPPER_VERSION:
            for (ucLoopCount = 0x00;
                 ucLoopCount < VERSION_LABEL_LEN;
                 ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = SOFTWAREVERSION[ucLoopCount];
                //g_ucaWrapperVersion[ucLoopCount];
            break;
          default:
            for (ucLoopCount = 0x00;
                 ucLoopCount < TRANSDUCER_LABEL_LEN;
                 ucLoopCount++)
              g_LabelMsg.fields.ucaDescription[ucLoopCount] = "CANNOT COMPUTE!!"[ucLoopCount];
                //g_ucaTransducerLabels[g_LabelMsg.fields.ucSensorNumber][ucLoopCount];
              break;


        }// END: switch(g_LabelMsg.fields.ucSensorNumber)

        // Send the label message
        vCOMM_SendLabelMessage(&g_LabelMsg);
        break; //END REQUEST_LABEL

      //If the CP Board sent a Handshake message, respond with this packet.
      case HAND_SHK:
    	  //UARTDELETE
    	  vUARTCOM_TXString("HAND_SHK Received\r\n",19);
    	  g_32DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION; //-scb
    	  g_32DataMsg.fields.ucMsgType = HAND_SHK;
    	  g_32DataMsg.fields.ucMsgSize = SP_32BITDATAMESSAGE_SIZE;
    	  g_32DataMsg.fields.ucSensorNumber = HAND_SHK;
    	  g_32DataMsg.fields.ucData1_HI_BYTE = 0x12;
    	  g_32DataMsg.fields.ucData1_LO_BYTE = 0xEF;
    	  g_32DataMsg.fields.ucData2_HI_BYTE = 0xCD;
    	  g_32DataMsg.fields.ucData2_LO_BYTE = 0xAB;
    	  vCOMM_Send32BitDataMessage(&g_32DataMsg);
    	  //UARTDELETE
    	  vUARTCOM_TXString("HAND_SHK sent\r\n",15);
    	  break; //END HAND_SHK
      default:
        //_never_executed();//Unless there's an error ;)
    	vUARTCOM_TXString("Unknown Message\r\n",17);
    	vUARTCOM_TXString("Message Version: ",17);
    	vUARTCOM_TXString((char*)(g_32DataMsg.fields.ucMsgVersion+32),1);
    	vUARTCOM_TXString("\r\nMessage Type: ",16);
    	vUARTCOM_TXString((char*)(g_32DataMsg.fields.ucMsgType+32),1);
    	vUARTCOM_TXString("\r\nMessage Size: ",16);
    	vUARTCOM_TXString((char*)(g_32DataMsg.fields.ucMsgSize+32),1);
    	vUARTCOM_TXString("\r\nSensor Number: ",17);
    	vUARTCOM_TXString((char*)(g_32DataMsg.fields.ucSensorNumber+32),1);
    	vUARTCOM_TXString("\r\nFirst Byte: ",14);
    	vUARTCOM_TXString((char*)(g_32DataMsg.fields.ucData1_HI_BYTE+32),1);
    	vUARTCOM_TXString("\r\n",2);

    	  g_32DataMsg.fields.ucMsgVersion = SP_DATAMESSAGE_VERSION; //-scb
    	  g_32DataMsg.fields.ucMsgType = REPORT_ERROR;
    	  g_32DataMsg.fields.ucMsgSize = SP_32BITDATAMESSAGE_SIZE;
    	  g_32DataMsg.fields.ucSensorNumber = ID_PKT_CODE;

    	  //ID_PKT content to tell CP Board to expect 128 bit data return packets
    	  g_32DataMsg.fields.ucData1_HI_BYTE = PACKET_ERROR_CODE;
    	  vCOMM_Send32BitDataMessage(&g_32DataMsg);

        break; //END default
    }// END: switch(g_32DataMsg.fields.ucMsgType)
    __bis_SR_register(LPM3_bits);//LPM4 works too, but then no timers whatsoever. We did not want to limit the Applications that much.
    // After Tranceiver code has been executed, wait until you receive the
    // REQUEST_DATA packet and then send the packet. You can then go into
    // sleep mode again. It is expected that the SP will be turned off by
    // the CP. If not, in LPM3 it is still ready to receive messages.
  }// END: while(TRUE)
}

//! @}
