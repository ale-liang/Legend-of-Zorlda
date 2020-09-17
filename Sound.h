// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// 1/17/2020

#include <stdint.h>



// ***************** SoundTask ****************
// Outputs the 8bit input into the DAC until sound is finished
// Inputs:  none
// Outputs: none
void SoundTask(void);

// ***************** Sound_Init ****************
// Activate TIMER2 interrupts to run user task periodically
// Activate DAC 
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Sound_Init(void);

// ***************** Sound_Play ****************
// Initializes SoundTask for the specific sound needing to be played
// Inputs:  pointer to the sound array, count is the number of notes played
// Outputs: none
void Sound_Play(const uint8_t *pt, uint32_t count);

// **************Sound_PlayMusic*********************
// Start sound output, and set Systick interrupt period 
// Sound continues until Sound_Play called again
// This function returns right away and sound is produced using a periodic interrupt
// Input: interrupt period
//           Units of period to be determined by YOU
//           Maximum period to be determined by YOU
//           Minimum period to be determined by YOU
//         if period equals zero, disable sound output
// Output: none
void Sound_PlayMusic(uint32_t period);

//*************Sound_Hyah*****************************************
//Pauses ongoing music, sets Timer2 reload value to 11kHz
//Plays sound effect
//inputs: none
//Outputs: none
void Sound_hyah(void);

//*************Sound_Projectile*****************************************
//Pauses ongoing music, sets Timer2 reload value to 11kHz
//Plays sound effect
//inputs: none
//Outputs: none
void Sound_Projectile(void);

//*************Sound_Slash*****************************************
//Pauses ongoing music, sets Timer2 reload value to 11kHz
//Plays sound effect
//inputs: none
//Outputs: none
void Sound_Slash(void);

//*************Sound_Hit*****************************************
//Pauses ongoing music, sets Timer2 reload value to 11kHz
//Plays sound effect
//inputs: none
//Outputs: none
void Sound_Hit(void);

void Sound_Shoot(void);
void Sound_Killed(void);
void Sound_Explosion(void);

void Sound_Fastinvader1(void);
void Sound_Fastinvader2(void);
void Sound_Fastinvader3(void);
void Sound_Fastinvader4(void);
void Sound_Highpitch(void);
