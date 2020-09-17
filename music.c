//music.c

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "music.h"
#include <stdlib.h>
#include "sound.h"
#include "Timer1.h"
#include "DAC.h"


extern const unsigned short wave[64];
extern uint8_t idx;
extern uint8_t idx2;
extern uint32_t melody;
extern uint32_t bass;
note_t *songSpot;
note_t *songPt; //points to note as song plays
note_t *bassPt; //points to note as song plays
uint32_t pitch=0;
uint32_t duration = 0;

note_t LegendOfZelda[267] = {
	{A,half},
	{noS, gap},
	{noS, quarter},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{G, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{noS, quarter},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{G, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{noS, quarter},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{E, quarterP},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, sixteenth},
	{noS, gap},
	{B, sixteenth},
	{noS, gap},
	{DF1, sixteenth},
	{noS, gap},
	{D1, sixteenth},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{noS, eighth},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{F1, eighth},
	{noS, gap},
	{G1, eighth},
	{noS, gap},
	{A1, half},
	{noS, gap},
	{noS, eighth},
	{noS, gap},
	{A1, eighth},
	{noS, gap},
	{A1, eighth},
	{noS, gap},
	{G1, eighth},
	{noS, gap},
	{F1, eighth},
	{noS, gap},
	{G1, eighthP},
	{noS, gap},
	{F1, sixteenth},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{E1, quarter},
	{noS, gap},
	{D1, eighth},
	{noS, gap},
	{D1, sixteenth},
	{noS, gap},
	{E1, sixteenth},
	{noS, gap},
	{F1, half},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{D1, eighth},
	{noS, gap},
	{C, eighth},
	{noS, gap},
	{C, sixteenth},
	{noS, gap},
	{D1, sixteenth},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{D1, eighth},
	{noS, gap},
	{C, eighth},
	{noS, gap},
	{B, eighth},
	{noS, gap},
	{B, sixteenth},
	{noS, gap},
	{DF1, sixteenth},
	{noS, gap},
	{EF1, half},
	{noS, gap},
	{GF1, quarter},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{E, quarterP},
	{noS, gap},
	{A, eighth},
	{noS, gap},
	{A, sixteenth},
	{noS, gap},
	{B, sixteenth},
	{noS, gap},
	{DF1, sixteenth},
	{noS, gap},
	{D1, sixteenth},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{noS, eighth},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{F1, eighth},
	{noS, gap},
	{G1, eighth},
	{noS, gap},
	{A1, half},
	{noS, gap},
	{noS, quarter},
	{noS, gap},
	{C1, quarter},
	{noS, gap},
	{B1, quarter},
	{noS, gap},
	{AF1, half},
	{noS, gap},
	{E1, quarter},
	{noS, gap},
	{F1, halfP},
	{noS, gap},
	{A1, quarter},
	{noS, gap},
	{AF1, quarter},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{E1, quarter},
	{noS, gap},
	{F1, halfP},
	{noS, gap},
	{A1, quarter},
	{noS, gap},
	{AF1, quarter},
	{noS, gap},
	{E1, half},
	{noS, gap},
	{DF1, quarter},
	{noS, gap},
	{DF1, halfP},
	{noS, gap},
	{F1, quarter},
	{noS, gap},
	{E1, quarter},
	{noS, gap},
	{C, half},
	{noS, gap},
	{A, quarter},
	{noS, gap},
	{B, eighth},
	{noS, gap},
	{B, sixteenth},
	{noS, gap},
	{C, sixteenth},
	{noS, gap},
	{D1, half},
	{noS, gap},
	{F1, quarter},
	{noS, gap},
	{E1, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, sixteenth},
	{noS, gap},
	{E, eighth},
	{noS, gap},
	{E, eighth},
	{1,1},
	{0,0},
};

//**************Timer3A_Init******************************************
//turns on TIMER3 interrupts to run user task
//inputs: Period is the the number of clock cycles * 12.5 ns want timer3 to run for
//Outputs: none
void Timer3A_Init(uint32_t period){
//	volatile int delay;
	SYSCTL_RCGCTIMER_R |= 0x08; // activate TIMER3
//	delay++;
	TIMER3_CTL_R = 0x00000000;	//disable TIMER3A
	TIMER3_CFG_R = 0x00000000;	//configure timer for 32 bits
	TIMER3_TAMR_R = 0x00000002; //configure for periodic interrupts

	TIMER3_TAILR_R = period-1; 	//reload value  (TAILR)

	TIMER3_TAPR_R = 0;					//bus clock resolution
	TIMER3_ICR_R = 0x00000001;		//clear TIMER3A timer
	TIMER3_IMR_R = 0x00000001;		//arm timeout interrupt arms it
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF) |0x20000000; //becomes priority 0 assigns priority
	//interrupts enabled in the main program
	//vector number 35, intterupt number 19
	NVIC_EN1_R = 1<<3;					//enables it
	TIMER3_CTL_R = 0x00000001; 	//turn on the timer
}

void Timer3A_Handler(void){ //called every time want to change the note
 	TIMER3_ICR_R = TIMER_ICR_TATOCINT; //acknowledge timer and will clear the interrupt flag	
	if((duration == 0) || (songPt == NULL)){
		//if(songPt == NULL){
		if(duration == 0){
			TIMER3_IMR_R = 0x00000000; //disarm Timer0A
			TIMER3_CTL_R = 0x00000000; // deactivate TIMER3
			NVIC_EN1_R = 1<<3;					//enables it
			Music_Play();
			TIMER3_CTL_R = 0x00000001; 	//turn on the timer
			pitch = songPt -> frequency;
			duration = songPt-> length;
		}
		//else{
		//	TIMER3_CTL_R = 0x00000001; 	//turn on the timer
		//}
	}
	else{
		pitch = songPt->frequency;
		duration = songPt->length;

		Sound_PlayMusic(pitch);
		TIMER3_TAILR_R = (duration);
		songPt++;
	}

}


//**************Music_init******************************************
//Initializes Timer3A and arms Timer3A, sets songPt to null
//inputs: none
//Outputs: none
void Music_Init(void){

	Timer3A_Init(10);
	TIMER3_IMR_R = 0x00000000; //disarm Timer3A
	songPt = NULL;

}


//**************Music_Play******************************************
//Initializes songPt to the start of song, duration to the length of first note, and arm timer3A
//inputs: none
//Outputs: none
void Music_Play(void){
	//initialize songPointer
	songPt = &LegendOfZelda[0];
	duration = songPt -> length;
	TIMER3_IMR_R = 0x00000001;	//ARM timer3A
}

//**************Music_Stop******************************************
//Disarm Timer3 and set songPt to NULL (complete stop of music)
//inputs: none
//Outputs: none
void Music_Stop(void){
	TIMER3_IMR_R = 0x00000000; //disarm Timer3A
	songPt = NULL;	


}

//**************Music_Pause******************************************
//Turns off Timer 2 and 3
//inputs: none
//Outputs: none
void Music_Pause(void){
	TIMER3_CTL_R = 0x00000000; 	//turn off the timer
	TIMER2_CTL_R = 0x00000000; 	//turn off the timer
}


//**************Music_Resume******************************************
//Call Sound_PlayMusic at the current pitch to reset the timer value of timer2, 
//initialize timer3 reload value to the current duration, and turn timer 3 and 2 back on
//inputs: none
//Outputs: none
void Music_Resume(void){
	Sound_PlayMusic(pitch);
	TIMER3_TAILR_R = (duration);
	TIMER3_CTL_R = 0x00000001; 	//turn on the timer
	TIMER2_CTL_R = 0x00000001; 	//turn on the timer

	//songPt = songSpot;
}

