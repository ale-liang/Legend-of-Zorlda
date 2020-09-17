// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 4/18/2020
// Student names: Zachary Chin and Alex Liang

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void){ int delay;
	SYSCTL_RCGCADC_R |= 0x1; 		//Turn on clock for ADC0
	SYSCTL_RCGCGPIO_R |= 0x8;		//Turn on clock for Port D
	delay = SYSCTL_RCGCGPIO_R; delay++;	//Wait for clocks to stabilize
	GPIO_PORTD_DIR_R &= ~0x6;		//Configure PD2-1 as an input
	GPIO_PORTD_AFSEL_R |= 0x6;	//Enable Alternative function for PD2-1
	GPIO_PORTD_DEN_R &= ~0x6;		//Disable digital function for PD2-1
	GPIO_PORTD_AMSEL_R |= 0x6;	//Enable analog function for PD2-1
	ADC0_PC_R &= ~0xF;					//Clear speed for ADC0
	ADC0_PC_R |= 0x1;						//Set speed to 125K samples/second
	ADC0_SSPRI_R = 0x0123;			//Set SS3 to highest priority and SS2 next
	ADC0_ACTSS_R &= ~0xC;				//Disable SS3 and SS2 to configure
	ADC0_EMUX_R &= ~0xFF00;			//Set SS3 and SS2 trigger to software start
	ADC0_SSMUX3_R &= ~0x000F;		//Clear SSMUX3 bits 3-0
	ADC0_SSMUX2_R &= ~0x000F;		//Clear SSMUX2 bits 3-0
	ADC0_SSMUX2_R += 6;					//Configure input source to channel 6 (PD1)
	ADC0_SSMUX3_R += 5;					//Configure input source to channel 5 (PD2)
	ADC0_SSCTL3_R = 0x6;				//no TS0 D0, yes IE0 END0
	ADC0_SSCTL2_R = 0x6;				//no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0xC;					//Disable interrupts for SS3 and SS2
	ADC0_SAC_R = 4;   					// 16-point averaging
	ADC0_ACTSS_R |= 0xC;				//Turn on SS3 and SS2
}

//------------ADC_InX------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_InX(void){  uint32_t data;
	ADC0_PSSI_R = 0x8;									//Start conversions
	while ((ADC0_RIS_R & 0x8) == 0) {};	//busy-wait until finished
	data = ADC0_SSFIFO3_R & 0xFFF;			//Read FIFO buffer (12-bit data)
	ADC0_ISC_R = 0x8;										//Clear flag (bit 3 in RIS)
  return data; 												//return data
}

//------------ADC_InY------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD1, analog channel 6
uint32_t ADC_InY(void){  uint32_t data;
	ADC0_PSSI_R = 0x4;									//Start conversions
	while ((ADC0_RIS_R & 0x4) == 0) {};	//busy-wait until finished
	data = ADC0_SSFIFO2_R & 0xFFF;			//Read FIFO buffer (12-bit data)
	ADC0_ISC_R = 0x4;										//Clear flag (bit 3 in RIS)
  return data; 												//return data
}


