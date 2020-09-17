// LegendOfZorlda.c
// Runs on TM4C123
// Zachary Chin and Alex Liang
// This is a game project for EE319K Lab 10
// Last Modified: 5/7/2020

// ********** Hardware Connections **********
// Joystick pin 1 connected to ground
// Joystick sel pin 2 connected to ground
// Joystick yout pin 3 connected to PD1/AIN6
// Joystick xout pin 4 connected to PD2/AIN5
// Joystick VCC pin 5 connected to +3.3V 
// fire button connected to PE0
// switch weapon button connected to PE1
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2
// 4*R resistor DAC bit 3 on PB3 
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "music.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "TExaS.h"
#include "zstructs.h"
#include "player.h"
#include "entity.h"
#include "stage.h"
#include "proj.h"
#include "rupoo.h"
#include "item.h"
#include "DAC.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Button_Init(void);
void SysTick_Init(uint32_t);
void Draw_Item(uint8_t item, uint8_t shop);
void Draw_Health(uint32_t hp, uint32_t max);
void Make_Floor(uint8_t floor);
void Sample_Joystick(void);
void ScoreTrack(void);
void Ending(void);

typedef enum {English = 0, Spanish = 1} Language_t;
typedef enum {Lonk = 0, Yerr = 1, Val = 2} type_t;

const unsigned short* playerSprites[12] = {
	lonk_up, lonk_right, lonk_down, lonk_left,
	yerup, yerrt, yerdn, yerlt,
	vaup, vart, vadn, valt
};

char* languages[] = {"English", "Espa\xA4ol"};
char* otherDialogue[] = {
	"Choose your character",
	"Elige a tu personaje",
	" Cost:", "Costo:",
	"The Legend of", "La Leyenda de"
};

char* shopDialogue[] = {
	"    Welcome in!",
	"Brn:25 ", "Red :100",
	"Grn:50 ", "Blue:250",
	
	"  \xAD" "Bienvenidos!",
	"Mar:25 ",  "Rojo:100",
	"Ver:50 ",  "Azul:250"
};
char* itemsInfo[102] = {
	"         Potion",
	"Recovers all hp",
	"               ",
	
	"    Short Sword",
	"Basic Slash hit",
	"               ",

	"  Wooden Shield",
	"Makes temporary",
	"weak shield    ",
	
	"     Wooden Bow",
	"Shoots an arrow",
	"               ",
	
	"       Deku Nut",
	"Shoot nut that ",
	"stuns on impact",
	
	"        Needles",
	"Shoot 2 needles",
	"               ",
	
	"          Bombs",
	"Throws a bomb  ",
	"               ",
	
	" Biggoron Sword",
	"Stronger slash ",
	"attack         ",
	
	"Heart Container",
	"Add additional ",
	"heart          ",
	
	"        Bombchu",
	"Bomb that moves",
	"until unable to",
	
	"        Ice Rod",
	"Freezes all    ",
	"enemies        ",
	
	"Hyrulien Shield",
	"Makes shields  ",
	"all around     ",
	
	"       Fire Rod",
	"Shoots fire on ",
	"all sides      ",
	
	"Quake Medallion",
	"Quake that hits",
	"all enemies    ",
	
	"   Bow of Light",
	"Shoot 2 light  ",
	"arrows         ",
	
	"   Master Sword",
	"Slash with wave",
	"projectile     ",
	
	"        Mjolnir",
	"Swing with bolt",
	"projectile     ",
	
	
	"         Poci\xA2n",
	"Recuperar todos",
	"los HP         ",
	
	"   Espada Corta",
	"Barra b\xA0sica   ",
	"               ",
	
	"  Escudo B\xA0sico",
	"Hace un escudo ",
	"d\x82" "bil temporal ",
	
	" Arco de Madera",
	"Dispara una    ",
	"flecha         ",
	
	"      Nuez Deku",
	"Una tuerca que ",
	"aturde         ",
	
	"         Agujas",
	"Dispara dos    ",
	"agujas         ",
	
	"         Bombas",
	"Lanzar una     ",
	"bomba          ",
	
	"Espada Biggoron",
	"Ataque de corte",
	"m\xA0s fuerte     ",
	
	"  Coraz\xA2n extra",
	"Agrega un      ",
	"coraz\xA2n extra  ",
	
	"        Bombchu",
	"Bomba que se   ",
	"mueve lejos    ",
	
	" Barra de Hielo",
	"Congela a todos",
	"los enemigos   ",
	
	"Escudo Hyrulien",
	"Protege todas  ",
	"las direcciones",
	
	" Barra de Fuego",
	"Dispara fuego  ",
	"por todos lados",
	
	" Medall\xA2n Sismo",
	"Sacude a todos ",
	"los enemigos   ",
	
	"    Arco de Luz",
	"Dispara dos    ",
	"flechas de luz ",
	
	" Espada Maestra",
	"Fuerte corte   ",
	"con proyectil  ",
	
	"        Mjolnir",
	"Golpear con el ",
	"martillo trueno"
};



char* credits[] = {
	"F","l","o","o","r","s"," ",
	"C","o","m","p","l","e","t","e","d",":"," ",
	
	"P","i","s","o","s"," ",
	"t","e","r","m","i","n","a","d","o","s",":"," ",
	
	" "," ","T","o","t","a","l"," ","R","u","p","o","o","s",":"," ",
	"R","u","p","o","o","s"," ","T","o","t","a","l","e","s",":"," ",
	
	"Y","o","u","r"," ","S","c","o","r","e"," ","i","s",":"," ",
	"T","u"," ","p","u","n","t","a","j","e"," ","e","s",":"," ",
	
};

//variable that holds the current language
Language_t myLanguage = English;

uint8_t delayVar = 0;

//Variable to redraw the stats at the top
//	0x8 means current coins needs to be redrawn
//	0x4 means floor number needs to be redrawn
//	0x2 means hearts need to be redrawn
//	0x1 means item needs to be redrawn
uint8_t redrawStats = 0xF;

//the total number of rupoos collected by the player
uint32_t totalRupoos = 0;

//the player's score
uint32_t score = 0;

//shop variable to check if player is next to a chest
uint8_t shopVar;

//The five stages that we will modify to make our floors
stage_t stage1,stage2,stage3,stage4,stage5,stage6;

//the pointer to the unmodified rupoo
extern rupoo_t *rzPtr;

//the current floor number
uint8_t floorNum = 1;

//if 1, the main program runs
uint8_t playing = 1;

//the current speed of the clock
//	decrease by 333333 every floor
uint32_t clock = 3666666;

//the current floor
stage_t *currentFloor[6];

//the current stage
stage_t *currentStage;

//the player's avatar
player_t lonk;

//the character the player chose
type_t myType;

//a flag to redraw an enemy after taking damage
entity_t *damagePt;

//a flag to use an item
//	the 4 most significant bits are the charge counter
uint8_t itemFlag;

//position of the joystick
uint32_t dataX, dataY;

//a flag to invert the colors of the display
uint8_t invertFlag;

//a flag to display that the user must wait to use the current item
uint8_t cantUseItemFlag;

//a flag that signals to rotate the screen
uint8_t shakeFlag;

//a flag to redraw the borders
//	bit 0 is north
//	bit 1 is east
//	bit 2 is south
//	bit 3 is west
uint8_t borders;

uint8_t displayTitle = 1;

uint8_t pause = 0;

int main(void){
	DisableInterrupts();
	TExaS_Init();		//80MHz
	Output_Init();	//Initialize the LCD
	ADC_Init();			//Initialize the ADC
	Button_Init();	//Initialize 2 external buttons
	GPIO_PORTE_IM_R &= ~0x3;	//Turn off interrupts for the buttons
	
	//initialize systick timer to generate a seed for random_init
	NVIC_ST_CTRL_R = 0x0;
	NVIC_ST_RELOAD_R = 0xFFFFFF;
	NVIC_ST_CTRL_R = 0x5;
	
	//initialize sound and music
	Sound_Init();
	Music_Init();

	uint8_t l = 1;	//l is a variable that helps with joystick debouncing
	//Draw the title display
	ST7735_DrawBitmap(38,59,title_top,43,2);
	ST7735_DrawBitmap(11,79,title_zorlda,103,20);
	ST7735_DrawBitmap(48,100,title_bottom,22,21);
	ST7735_DrawBitmap2(0, 94,86,title_sword_top,16,2);
	ST7735_DrawBitmap2(0, 94,92,title_sword_bot,16,6);
	ST7735_DrawBitmap2(0, 10,89,title_sword_mid,101,3);
	ST7735_DrawString(3,5,otherDialogue[4 + myLanguage],0xFFFF);
	ST7735_DrawString(10,11,"^",0xFFFF);
	ST7735_DrawString(7,12,languages[myLanguage],0xFFE0);
	ST7735_DrawString(10,13,"v",0xFFFF);
	Timer0_Init(Sample_Joystick,clock);	//turn on the timer that samples joystick
	EnableInterrupts();	//Enable interrupts
	while (displayTitle){	//infinite loop until button is pressed
		//Check if joystick is moved up/down and switch language if so
		while (dataX > 0xB00 || dataX < 0x500 || dataY > 0xB00 || dataY < 0x500) {
			if (l && dataX < dataY && (dataX + dataY) < 4096){	
				if (myLanguage == English)	myLanguage = Spanish;
				else	myLanguage = English;
				ST7735_DrawString(7,12,languages[myLanguage],0xFFE0);
				ST7735_DrawString(3,5,otherDialogue[4 + myLanguage],0xFFFF);
				l=0;	//l prevents unprecedented spam of the joystick
			}else if (l && dataX > dataY && (dataX + dataY) > 4096){ 
				if (myLanguage == English)	myLanguage = Spanish;
				else	myLanguage = English;
				ST7735_DrawString(7,12,languages[myLanguage],0xFFE0);
				ST7735_DrawString(3,5,otherDialogue[4 + myLanguage],0xFFFF);
				l=0;
			}
		}l=1;	//once joystick is released, allowed to change language again
		//This while loop is for button debouncing
		//	will only move on once the button is pressed and then released
		while (GPIO_PORTE_DATA_R & 0x2){
			if ((GPIO_PORTE_DATA_R & 0x2) == 0){
				displayTitle = 0;
			}
		}
	}
	ST7735_FillScreen(0x0000);	//screen goes black
	displayTitle = 1;						//entry to another infinite loop
	ST7735_DrawBitmap(8,149,lunk,111,148);	//draw default player, lonk
	ST7735_DrawString(0,15,otherDialogue[myLanguage],0xFFFF);
	Make_Lonk(&lonk);	//Makes the player into a lonk player
	myType = Lonk;		//an enum for the player type
	while (displayTitle){
		//Sample the adc to see if joystick is moving right/left, then change players accordingly
		while (dataX > 0xB00 || dataX < 0x500 || dataY > 0xB00 || dataY < 0x500) {
			if (l && dataX > dataY && (dataX + dataY) < 4096){	
				if (myType == Lonk){
					ST7735_DrawBitmap2(0,8,149,valvano,111,148);
					myType = Val;
					Make_Valvano(&lonk);
				}else if (myType == Yerr){
					ST7735_DrawBitmap2(0,8,149,lunk,111,148);
					myType = Lonk;
					Make_Lonk(&lonk);
				}else {
					ST7735_DrawBitmap2(0,8,149,yerraballipic,111,148);
					myType = Yerr;
					Make_Yerraballi(&lonk);
				}
				l=0;
			}else if (l && dataX < dataY && (dataX + dataY) > 4096){ 
				if (myType == Lonk){
					ST7735_DrawBitmap2(0,8,149,yerraballipic,111,148);
					myType = Yerr;
					Make_Yerraballi(&lonk);
				}else if (myType == Yerr){
					ST7735_DrawBitmap2(0,8,149,valvano,111,148);
					myType = Val;
					Make_Valvano(&lonk);
				}else {
					ST7735_DrawBitmap2(0,8,149,lunk,111,148);
					myType = Lonk;
					Make_Lonk(&lonk);
				}
				l=0;
			}
		}l=1;
		while (GPIO_PORTE_DATA_R & 0x2){
			if ((GPIO_PORTE_DATA_R & 0x2) == 0){
				displayTitle = 0;
			}
		}
	}
	DisableInterrupts();	//once again disable interrupts to initialize again
	Random_Init(NVIC_ST_CURRENT_R);	//seed random number
	SysTick_Init(clock);		//set the SysTick clock to run
	ST7735_FillScreen(0x0000);	//Fill the screen black
	Make_Floor(floorNum);	//Instantiates the floor
	Draw_Borders();	//Draw the stage
	ST7735_DrawBitmap(0,15,FLO,20,16);	//Draw the letters FLO to be used in the stats
	ST7735_DrawString(6,1,"R:",0xFFFF);	//Draw the letter R for rupoo. This shows currency held
	EnableInterrupts();	//Enable interrupts
	Music_Play();		//start playing the legend of zelda music on loop
	GPIO_PORTE_IM_R |= 0x3;		//reenable PORTE button interrupts
	Timer1_Init(ScoreTrack,80000000);		//interrupt every second
	int i, count;	//variables for loops and misc
	//the main loop for the game
	while(playing){ 
		
		//allows the player to pause the game
		//	pause is written in PortF interrupt handler
		while (pause){}
		
		//if the player is unable to use the current item, draw a red X over it
		if (cantUseItemFlag == 1){
			ST7735_DrawBitmap2(0,99,39,cantUseItem,10,10);
			cantUseItemFlag++;	//adding ensures that the X isnt redrawn when not necessary
		}

		//Redraw the stats seen above the stage (includes floor number, hearts, and current item)
		if (redrawStats) {		//check the stats semiphore
			DisableInterrupts();
			if (redrawStats & 0x1) {	
				redrawStats &= ~0x1;	//check if the item semiphore is set, then reset it
				Draw_Item(lonk.items[lonk.currentItem],0);	//Draw the item and item help
			}if (redrawStats & 0x2) {	
				redrawStats &= ~0x2;	//check if the hearts stat semiphore is set, then reset it
				Draw_Health(lonk.health, lonk.maxHealth);		//redrawStats the hearts according to the players max and current health
			}if (redrawStats & 0x4) {	
				redrawStats &= ~0x4;	//check if the floor stat semiphore is set, then reset it
				if (floorNum){
					if (floorNum == 1){ 
						ST7735_DrawBitmap(20,15,one,8,16);		//if 1, draw 1
					}else if (floorNum == 2){
						ST7735_DrawBitmap(20,15,two,8,16);		//if 2, draw 2
					}else if (floorNum == 3){
						ST7735_DrawBitmap(20,15,three,8,16);	//if 3, draw 3
					}else if (floorNum == 4){
						ST7735_DrawBitmap(20,15,four,8,16);	//if 4, draw 4
					}else if (floorNum == 5){
						ST7735_DrawBitmap(20,15,five,8,16);	//if, draw 5
					}
				}
			}if (redrawStats & 0x8){	//check if the rupoo stat semiphore is set, then reset it
				redrawStats &= ~0x8;	
				ST7735_DrawString(8,1,"   ",0xFFFF);	//first, clear the previous content
				
				ST7735_SetCursor(8,1);	//set the cursor to where the rupoos will be shown
				LCD_OutDec(lonk.rupoos);			//print the current amount of rupoos
			}EnableInterrupts();
		}	
		
		//Draw all projectiles (they should be erased when checking movement)
		count = i = 0;	//reset count and index variables
		while (count<currentStage->pSize && i < pAsize){		//loop until we reach all known profs or the end of the array
			if (currentStage->pA[i].exists == 1){		//Check if the current proj exists
				if (currentStage->pA[i].redraw == 1){	//if it exists, check its redraw
					currentStage->pA[i].redraw = 0;			//reset the redraw
					proj_t *temp = &(currentStage->pA[i]);	//grab the proj in a pointer for ease of access
					ST7735_DrawBitmap2(0,temp->pos[0],temp->pos[1],temp->image,temp->dims[0],temp->dims[1]);
				}count++;		//increment count to know we've found one existing proj
			}i++;					//increment the index every loop
		}uint8_t deleted;
		//Draw all rupoos and erase rupoos when necessary
		count = i = deleted = 0;	//reset count variable
		while (count < currentStage->rSize && i < 16){
			if (currentStage->rQ[i].exists == 1 || currentStage->rQ[i].redraw == 1){
				count++;
				//grab the rupoo in a pointer for ease of access
				rupoo_t *temp = &(currentStage->rQ[i]);	
				if (temp->redraw == 1){		//if it needs redrawing, reset the redraw variable
					temp->redraw = 0;
					if (temp->duration == 0){		//if its duration drops to 0, erase it from the screen
						temp->value = 0;
						temp->exists = 0;
						deleted++;
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black10x10,10,10);
					}else{		//else redraw it as is
						ST7735_DrawBitmap2(0, temp->pos[0],temp->pos[1],temp->image,10,10);
					}
				}
			}i++;
		}currentStage->rSize -= deleted; 	//decrease the size of the array based off count
		
		uint8_t i2 = 0;
		count = 0;
		//Redraw all enemies that need to be redrawn (if killed, redraw black space over it)
		if (!currentStage->cleared){
			for (i = 0;i<currentStage->totalEnemies;i++){		//loop through the array of enemies in the current stage
					entity_t *e = &(currentStage->enemies[i]);		//grab the enemy for easier access
					if (e->health <= 0){ 
						count++;
					}if (e->redraw){	
						e->redraw = 0;		//check redraw semiphore and reset it 
						if (e->health <= 0)	{
							ST7735_DrawBitmap(e->pos[0],e->pos[1],black10x10,10,10);	//if its health is 0, erase it from the screen
							if (e->dims[0] == 20) {		//if its dimensions are 20x20, draw the other 3 10x10s to erase it
								ST7735_DrawBitmap(e->pos[0]+10,e->pos[1],black10x10,10,10);
								ST7735_DrawBitmap(e->pos[0],e->pos[1]-10,black10x10,10,10);
								ST7735_DrawBitmap(e->pos[0]+10,e->pos[1]-10,black10x10,10,10);
							}if (e->coins != 0 && currentStage->rSize != rQsize){		//if the enemy drops rupoos and there arent already 16 rupoos
								Make_Rup(e);		//create a rupoo using the values from enemy
								e->coins = 0;
								while (currentStage->rQ[i2].value != 0 || currentStage->rQ[i2].redraw == 1)	i2++;
								currentStage->rQ[i] = *rzPtr;	//add the new rupoo to the array
								currentStage->rSize++; 		//increase the size of the array
							}else if (e->image == ganon1Boss){
								DisableInterrupts();
								//once ganon v1 is defeated, destroy all obstacles and transform
								shakeFlag = 24;		//shake the stage for special effect
								lonk.speedDelay = 25;	//player is temporarily slowed
								lonk.speed = 8;	lonk.speedCount = 8;	
								count = i = 0;	//variables for looping
								//loop through obstacles and delete all all of them
								while (count < currentStage->totalObstacles && i < 28){
									if (currentStage->obstacles[i].dur != 1){
										count++;
										currentStage->obstacles[i].dur = 0;
									}i++;
								}count = 0;
								Make_orcBoss(&currentStage->enemies[1],X,Y+80,up);
								Make_orcBoss(&currentStage->enemies[2],X+70,Y+80,up);
								currentStage->enemies[1].speedCount = 240;
								currentStage->enemies[2].speedCount = 240;
								Make_Ganon2Boss(&currentStage->enemies[0],X+35,Y+15,dn);
								currentStage->enemies[3].speedCount = 240;
								lonk.invinCount = 60;
								EnableInterrupts();
								i=0;
							}
						}else	{ST7735_DrawBitmap2(0, e->pos[0],e->pos[1],e->image,e->dims[0],e->dims[1]);		//if health is > 0, redraw the enemy
						}
					}
			}
		}
		//If all enemies are killed, open the exits and mark the stage as cleared
		if (currentStage->cleared == 0 && count == currentStage->totalEnemies){	
			currentStage->cleared = 1; //set cleared
			TIMER0_CTL_R = 0;
			dataX = dataY = 0x800;
			Draw_Borders();	//redrawStats the borders with holes if it is an exit
			TIMER0_CTL_R = 1;
		}count = 0;	//reset the count variable
		i2 = 0;
		//Redraw all obstacles that need to be redrawn (if destroyed, redraw black space over it)
		i = 0;
		deleted = currentStage->totalObstacles;
		while (count < deleted && i < 28){		//loop through the array of obstacles in the current stage
			if (currentStage->obstacles[i].dur > 0){
				count++;
				if (currentStage->obstacles[i].redraw){	
					currentStage->obstacles[i].redraw = 0;		//check redraw semiphore and reset it 
					entity_t *e = &(currentStage->obstacles[i]);		//grab the obstacle for easier access
					if (e->health == 0 && e->dur > 0){	
						e->dur = 0;
						ST7735_DrawBitmap(e->pos[0],e->pos[1],black10x10,e->dims[0],e->dims[1]);	//if its health is 0, erase it from the screen
						if (e->coins != 0 && currentStage->rSize != rQsize){		//if the enemy drops rupoos and there arent already 16 rupoos
								Make_Rup(e);		//create a rupoo using the values from enemy
								e->coins = 0;
								while (currentStage->rQ[i2].value != 0 || currentStage->rQ[i2].redraw == 1)	i2++;
								currentStage->rQ[i] = *rzPtr;	//add the new rupoo to the array
								currentStage->rSize++; 		//increate the size of the array
							}
					}else	{ST7735_DrawBitmap2(0, e->pos[0],e->pos[1],e->image,e->dims[0],e->dims[1]);		//if health is > 0, redraw the obstacle
					}
				}
			}//for obstacles with durability like explosions or shields
			else if (currentStage->obstacles[i].dur == 0){
				count++;	//increment count
				entity_t *e = &(currentStage->obstacles[i]);	//grab for ease of access
				ST7735_DrawBitmap(e->pos[0],e->pos[1],black10x10,10,10);	//erase the obstacle by drawing black over it
				//These really only apply to explosions; if the explosion was out of bounds, redraw the bounds
				if (e->pos[0] < (X)) borders |= 0x8;			
				if (e->pos[0] > (X + 80)) borders |= 0x2;
				if (e->pos[1] < (Y)) borders |= 0x1;
				if (e->pos[1] > (Y + 80)) borders |= 0x4;
				Redraw_All(&lonk);	//redraw player, enemies, obstacles, and rupoos
				e->health = 0;		//reduce obstacle's health to 0 for extra death
				e->dur = -1;			//durability of -1 means finally broken 100%
				currentStage->totalObstacles--;		//the obstacle is now removed from the array
			}i++;
		}
		
		//Redraw the player character if needed (and any enemy that was touching the player)
		if (lonk.redraw) {	
			//if the player was overlapped with an enemy, redraw the enemy once the player moves
			if (damagePt->health > 0){	
				ST7735_DrawBitmap2(0, damagePt->pos[0],damagePt->pos[1],damagePt->image,damagePt->dims[0],damagePt->dims[1]);
			}else damagePt = 0;
			//redraw the player
			ST7735_DrawBitmap2(0, lonk.pos[0],lonk.pos[1],lonk.image,lonk.dims[0],lonk.dims[1]);
			lonk.redraw = 0;	//set the redraw player semiphore to 0
		}
		
		//Redraw the borders if needed
		if (borders){
			if (borders & 0x1){
				Draw_Borders_Up();
			}if (borders & 0x2){
				Draw_Borders_Right();
			}if (borders & 0x4){
				Draw_Borders_Down();
			}if (borders & 0x8){
				Draw_Borders_Left();
			}borders = 0;
		}
		
		//Change the stage if the player has exited
		if (currentStage->cleared) {	//first check if the stage has been cleared
			DisableInterrupts();	//player has moved stages, so disable player input
				//Check if the player has gone through the north exit (give or take 1 pixel in width)
			if (lonk.pos[0] <= NSexitX + 1 && lonk.pos[0] >= NSexitX - 1 && lonk.pos[1] == NexitY){
				lonk.pos[0] = NSexitX;	lonk.pos[1] = SexitY - 11;	//Set the player's position to the corresponding door (N<->S E<->W)
				Clear_Obstacles();	//Clear the screen from all previous entities
				currentStage = currentFloor[currentStage->nextRooms[0]];	//Change the stage to the corresponding next stage
				Draw_Borders();	//Redraw the stage borders
				Redraw_All(&lonk);		//Set all the redraw flags (player, enemies, and obstacles)
			}else if (lonk.pos[0] == EexitX && lonk.pos[1] <= EWexitY + 1 && lonk.pos[1] >= EWexitY - 1){
				lonk.pos[0] = WexitX + 11;	lonk.pos[1] = EWexitY;	//Set the player's position to the corresponding door (N<->S E<->W)
				Clear_Obstacles();	//Clear the screen from all previous entities
				currentStage = currentFloor[currentStage->nextRooms[1]];	//Change the stage to the corresponding next stage
				Draw_Borders();	//Redraw the stage borders
				Redraw_All(&lonk);		//Set all the redraw flags (player, enemies, and obstacles)
			}else if (lonk.pos[0] <= NSexitX + 1 && lonk.pos[0] >= NSexitX - 1 && lonk.pos[1] == SexitY){
				lonk.pos[0] = NSexitX;	lonk.pos[1] = NexitY + 11;	//Set the player's position to the corresponding door (N<->S E<->W)
				Clear_Obstacles();	//Clear the screen from all previous entities
				currentStage = currentFloor[currentStage->nextRooms[2]];	//Change the stage to the corresponding next stage
				Draw_Borders();	//Redraw the stage borders
				Redraw_All(&lonk);		//Set all the redraw flags (player, enemies, and obstacles)
			}else if (lonk.pos[0] == WexitX && lonk.pos[1] <= EWexitY + 1 && lonk.pos[1] >= EWexitY - 1){
				lonk.pos[0] = EexitX - 11;	lonk.pos[1] = EWexitY;	//Set the player's position to the corresponding door (N<->S E<->W)
				Clear_Obstacles();	//Clear the screen from all previous entities
				currentStage = currentFloor[currentStage->nextRooms[3]];	//Change the stage to the corresponding next stage
				Draw_Borders();	//Redraw the stage borders
				Redraw_All(&lonk);		//Set all the redraw flags (player, enemies, and obstacles)
			}EnableInterrupts();	//reallow player input
		}
		//Check if the player has lost all of his health
		if (lonk.health <= 0){	
			DisableInterrupts();	//if the player's health is 0 (player loses), disable interrupts
			ST7735_FillScreen(0x0000);	//Fill the screen black
			ST7735_DrawString(0,0,"YOU LOSE",0xFFFF);		//Write out "you lose" on the screen
			playing = 0;	//exit the loop to end the program
		}
		
		//Actions for player in the shop
		if (currentStage->type == shopStage){
			uint8_t x = 0;	//variable for loops
			if (shopVar){		//shop var is set when the player is adjacent to a shop obstacle (chest)
				GPIO_PORTE_IM_R = 0;	//disable button interrupts
				for (x=0;x<currentStage->totalObstacles;x++){
					//loop through and see if the player is still adjacent to the obstacle
					if (Check_Adjacent(&lonk,&(currentStage->obstacles[x]))){
						break;	//if it is, break from the for loop, leaving x as the adjacent obstacle
					}
				}if (x == (currentStage->totalObstacles)){	//if the for loop ended without a break, no longer adjacent
					shopVar = 0;	//shopVar 0 means to check for adjacent chests again	
					redrawStats |= 0x1;		//redraw the player's own inventory
				}else{
					//if player is still adjacent to a chest, check if the player presses the use weapon button to buy the item
					//	nothing will happen if player does not have enough rupoos
					if (delayVar == 0 && GPIO_PORTE_DATA_R & 0x2 && lonk.rupoos >= currentStage->obstacles[x].coins){
						delayVar = 1;	//a delay variable that prevents player from spending their rupees excessively
						lonk.rupoos -= currentStage->obstacles[x].coins;	//subtract the price from player's rupoos
						if (currentStage->obstacles[x].damage == 0){		//if the chest holds a potion, restore player's health
							lonk.health = lonk.maxHealth;
							redrawStats |= 0x2;
						}else if (currentStage->obstacles[x].damage == 8){	//if chest holds a heartcontainer, add an additional heart
							lonk.maxHealth += 2;
							lonk.health = lonk.maxHealth;
							redrawStats |= 0x2;
						}else{		//if a regular item, adds the item to the player's inventory
							lonk.items[lonk.numItems] = currentStage->obstacles[x].damage;
							lonk.numItems++;
						}currentStage->obstacles[x].damage = 0;	//reset the chest's contents to just hold a potion until the next floor
						redrawStats |= 0x8;		//redraw rupoos
					}
				}	
			}else{	//if player is not currently adjacent to an obstacle, check for it
				GPIO_PORTE_IM_R = 0x3;	//enable button interrupts
				DisableInterrupts();
				//loop through the array of obstacles to check for adjacency
				for (x=0;x<currentStage->totalObstacles;x++){
					if (Check_Adjacent(&lonk,&(currentStage->obstacles[x]))){
						Draw_Item(currentStage->obstacles[x].damage,1);	//if adjacent, display the chest contents and info
						//if it is a chest, display the price for the item
						if (currentStage->obstacles[x].coins){		
							ST7735_SetCursor(17,1);
							LCD_OutDec(currentStage->obstacles[x].coins);
						}
						redrawStats &= ~0x1;	//make sure the player's inventory doesn't show up and override the display
						shopVar = 1;					//player is now currently adjacent to a chest, set shopVar
						delayVar = 0;					//player is now able to purchase an item
					}
				}EnableInterrupts();
			}		
		}
		//Check if the player has advanced to the next floor
		if (currentStage->type == stairsStage){
			//Check if player is adjacent to the stairs
			if (Check_Adjacent(&lonk, &(currentStage->obstacles[0]))){
				DisableInterrupts();
					if (floorNum < 5){
						Clear_Obstacles();
						lonk.pos[0] = X;	lonk.pos[1] = Y+80;	lonk.dir = 2;
						floorNum++;
						redrawStats |= 0x4;
						Make_Floor(floorNum);
						score += 200;
						Draw_Borders();
						Redraw_All(&lonk);
						clock -= 333333;
						Timer0_Init(Sample_Joystick,clock);
						SysTick_Init(clock);
						EnableInterrupts();
					}else {
						score += 500;
						floorNum++;
						ST7735_FillScreen(0x0000);
						playing = 0;
					}
			}
		}
	}
	//after exiting the loop (either win or lose), play game end animation
	//	note: lose has an inverted screen
	NVIC_ST_CTRL_R = 0;
	TIMER0_CTL_R = 0;
	TIMER1_CTL_R = 0;
	NVIC_DIS0_R = 0x10;	//Enable interrupt 4 in NVIC
	NVIC_DIS0_R = 0x40000000;	//Enable interrupt 30 in NVIC
	Timer0_Init(Ending, 2000000);
	EnableInterrupts();
	while(1);
}

void Make_Floor(uint8_t floor){
	if(floor == 0){
		Make_Bush1Stage(&stage1, 0x3, floorNum); // North - Shop, East - Bush2
		Next_Rooms(&stage1,3,1,0xFF,0xFF);
		currentFloor[0] = &stage1;
		Make_Bush2Stage(&stage2, 0x9, floorNum); // North - Slimeboss, West - Bush 1
		Next_Rooms(&stage2,4,0xFF,0xFF,0);
		currentFloor[1] = &stage2;
		Make_Stairs(&stage3, 0x4,floorNum);     // South - Slime boss
		Next_Rooms(&stage3,0xFF,0xFF,4,0xFF);
		currentFloor[2] = &stage3;
		Make_Shop(&stage4,0x6,floorNum);		//South - Bush1, east- slimeboss
		Next_Rooms(&stage4,0xFF,4,0,0xFF);
		currentFloor[3] = &stage4;
		Make_SlimeBossStage(&stage5, 0xD, floorNum); //South - Bush2, West - Shop, North - Stairs
		Next_Rooms(&stage5,2,0xFF,1,3);
		currentFloor[4] = &stage5;
		currentStage = currentFloor[0];
	}
	else if (floor == 2){
		Make_Water1Stage(&stage1, 0x8, floorNum); // West - Water2
		Next_Rooms(&stage1,0xFF,0xFF,0xFF,1);
		currentFloor[0] = &stage1;
		Make_Water2Stage(&stage2, 0xB, floorNum); // North - Orc boss, West - Shop, East - Water1
		Next_Rooms(&stage2,4,0,0xFF,3);
		currentFloor[1] = &stage2;
		Make_Stairs(&stage3, 0x4,floorNum);     // South - Orc boss
		Next_Rooms(&stage3,0xFF,0xFF,4,0xFF);
		currentFloor[2] = &stage3;
		Make_Shop(&stage4,0x2,floorNum);		//East- Water2
		Next_Rooms(&stage4,0xFF,1,0xFF,0xFF);
		currentFloor[3] = &stage4;
		Make_OrcBossStage(&stage5, 0x5, floorNum); //South - Water2, North - Stairs
		Next_Rooms(&stage5,2,0xFF,1,0xFF);
		currentFloor[4] = &stage5;
		currentStage = currentFloor[0];
	}
	else if (floor == 3){
		Make_Brick1Stage(&stage1, 0xA, floorNum); // West - Shop, East - Brick2
		Next_Rooms(&stage1,0xFF,1,0xFF,3);
		currentFloor[0] = &stage1;
		Make_Brick2Stage(&stage2, 0x9, floorNum); // North - Lynel boss, West - Brick 1
		Next_Rooms(&stage2,4,0xFF,0xFF,0);
		currentFloor[1] = &stage2;
		Make_Stairs(&stage3, 0x8,floorNum);     // West - Lynel boss
		Next_Rooms(&stage3,0xFF,0xFF,0xFF,4);
		currentFloor[2] = &stage3;
		Make_Shop(&stage4,0x2,floorNum);		//East - Brick1
		Next_Rooms(&stage4,0xFF,0,0xFF,0xFF);
		currentFloor[3] = &stage4;
		Make_LynelBossStage(&stage5, 0x6, floorNum); //South - Brick2, East - Stairs
		Next_Rooms(&stage5,0xFF,2,1,0xFF);
		currentFloor[4] = &stage5;
		currentStage = currentFloor[0];
	}
	else if (floor == 4){
		Make_Ice1Stage(&stage1, 0x1, floorNum); // North - Ice2
		Next_Rooms(&stage1,1,0xFF,0xFF,0xFF);
		currentFloor[0] = &stage1;
		Make_Ice2Stage(&stage2, 0xF, floorNum); // North - Ice golem boss, West - Shop, East - Ice 3
		Next_Rooms(&stage2,4,2,0,3);
		currentFloor[1] = &stage2;
		Make_Ice3Stage(&stage3, 0x8,floorNum);     // West - Ice 2
		Next_Rooms(&stage3,0xFF,0xFF,0xFF,1);
		currentFloor[2] = &stage3;
		Make_Shop(&stage4,0x2,floorNum);		// east- Ice2
		Next_Rooms(&stage4,0xFF,1,0xFF,0xFF);
		currentFloor[3] = &stage4;
		Make_IcegolemBossStage(&stage5, 0x5, floorNum); //South - Ice2, North - Stairs
		Next_Rooms(&stage5,5,0xFF,1,0xFF);
		currentFloor[4] = &stage5;
		Make_Stairs(&stage6, 0x04, floorNum);  //South - Ice golem boss
		Next_Rooms(&stage6,0xFF,0xFF,4,0xFF);
		currentFloor[5] = &stage6;
		currentStage = currentFloor[0];
	}
	else if (floor == 5){
		Make_Lava1Stage(&stage1, 0xB, floorNum); // North - Shop, East - Lava2, West - Lava3
		Next_Rooms(&stage1,3,1,0xFF,2);
		currentFloor[0] = &stage1;
		Make_Lava2Stage(&stage2, 0x8, floorNum); // West - Lava1
		Next_Rooms(&stage2,0xFF,0xFF,0xFF,0);
		currentFloor[1] = &stage2;
		Make_Lava3Stage(&stage3, 0x2,floorNum);     // East - Lava1
		Next_Rooms(&stage3,0xFF,0,0xFF,0xFF);
		currentFloor[2] = &stage3;
		Make_Shop(&stage4,0x5,floorNum);		//South - Lava1, North - Ganon Boss
		Next_Rooms(&stage4,4,0xFF,0,0xFF);
		currentFloor[3] = &stage4;
		Make_Ganon1BossStage(&stage5, 0x5, floorNum); //South - Shop, North - Stairs
		Next_Rooms(&stage5,5,0xFF,3,0xFF);
		currentFloor[4] = &stage5;
		Make_Stairs(&stage6, 0x04, floorNum); //South - Ganon Boss
		Next_Rooms(&stage6,0xFF,0xFF,4,0xFF);
		currentFloor[5] = &stage6;
		currentStage = currentFloor[0];
	}
	//TESTING STAGES HERE
	else if (floor == 1){
		Make_Lava4Stage(&stage1, 0xF, floorNum);
		Next_Rooms(&stage1, 0xFF, 0xFF, 0xFF, 0xFF);
		currentFloor[0] = &stage1;
		currentStage = currentFloor[0];
		
	}
}

//**********Button_Init********************
//Initialize the two external buttons in PE1-0
//inputs: none
//outputs: none
void Button_Init(void){ int delay;
	SYSCTL_RCGCGPIO_R |= 0x30;		//Turn on the clock for Port E and F
	delay = SYSCTL_RCGCGPIO_R; delay++;		//Wait for the clock to stabilize
	GPIO_PORTE_DIR_R &= ~0x3;		//Set PE1-0 as inputs
	GPIO_PORTE_DEN_R |= 0x3;		//Digitally enable PE1-0
	GPIO_PORTE_PCTL_R &= ~0x3;	//Clear to use edge-triggered interrupts
	GPIO_PORTE_AFSEL_R &= ~0x3;	//Disable alternate functions
	GPIO_PORTE_AMSEL_R &= ~0x3;	//Disable analog functions
	GPIO_PORTE_IS_R &= ~0x3;		//PE1-0 are edge-sensitive
	GPIO_PORTE_IBE_R &= ~0x3;		//PE1-0 is not both edges
	GPIO_PORTE_IEV_R |= 0x3;		//Rising edge event
	GPIO_PORTE_ICR_R = 0x3;			//Clear flag
	GPIO_PORTE_IM_R |= 0x3;			//Arm interrupt on PE1-0
	NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF2F) | 0x20;	//set priority to 1
	NVIC_EN0_R = 0x10;	//Enable interrupt 4 in NVIC
	
	GPIO_PORTF_DIR_R &= ~0x10;		//Set Pf4 as inputs
	GPIO_PORTF_DEN_R |= 0x10;		//Digitally enable Pf4
	GPIO_PORTF_PCTL_R &= ~0x10;	//Clear to use edge-triggered interrupts
	GPIO_PORTF_AFSEL_R &= ~0x10;	//Disable alternate functions
	GPIO_PORTF_AMSEL_R &= ~0x10;	//Disable analog functions
	GPIO_PORTF_IS_R &= ~0x10;		//Pf4 are edge-sensitive
	GPIO_PORTF_IBE_R &= ~0x10;		//Pf4 is not both edges
	GPIO_PORTF_IEV_R |= ~0x10;		//Falling edge event
	GPIO_PORTF_ICR_R = 0x10;			//Clear flag
	GPIO_PORTF_IM_R |= 0x10;			//Arm interrupt on Pf4
	GPIO_PORTF_PUR_R = 0x10;			//Pull up resistor
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF2FFF0F) | 0x20;	//set priority to 1
	NVIC_EN0_R = 0x40000000;	//Enable interrupt 30 in NVIC
}

//**********Draw_Item********************
//Draws items in the stats section according to the input
//Also displays information about the item
//inputs: item the item number
//				shop 1 if the item is in the shop
//						 0 if the player owns the item
//output: none
void Draw_Item(uint8_t item, uint8_t shop){
	switch(item){		//check the value of the current item
		case 0: ST7735_DrawBitmap(89,49,potion,30,30);
			break;
		case 1: ST7735_DrawBitmap(89,49,shortsword,30,30);	//if 1 (shortsword) draw shortsword
			break;
		case 2: ST7735_DrawBitmap(89,49,woodenshield,30,30);	//else, draw woodenshield
			break;
		case 3: ST7735_DrawBitmap(89,49,bow,30,30);	//else, draw bow
			break;
		case 4: ST7735_DrawBitmap(89,49,dekunutdisplay,30,30);
			break;
		case 5: ST7735_DrawBitmap(89,49,needles,30,30);
			break;
		case 6: ST7735_DrawBitmap(89,49,bomb,30,30);
			break;
		case 7:	ST7735_DrawBitmap(89,49,biggoronsword,30,30);
			break;
		//wont ever reach 8 because heart container is an instant effect
		case 8: ST7735_DrawBitmap(89,49,heartcontainer,30,30);
			break;
		case 9: ST7735_DrawBitmap(89,49,bombchuDisplay,30,30);
			break;
		case 10: ST7735_DrawBitmap(89,49,icerod,30,30);
			break;
		case 11: ST7735_DrawBitmap(89,49,hyrulienshield,30,30);
			break;
		case 12: ST7735_DrawBitmap(89,49,firerod,30,30);
			break;
		case 13: ST7735_DrawBitmap(89,49,quakemedallion,30,30);
			break;
		case 14: ST7735_DrawBitmap(89,49,bowoflight,30,30);
			break;
		case 15: ST7735_DrawBitmap(89,49,mastersword,30,30);
			break;
		case 16: ST7735_DrawBitmap(89,49,mjolnir,30,30);
			break;
		//17 is purely an extra display. Player will never be able to get item 17
		case 17: ST7735_DrawBitmap(89,49,shopkeepdisplay,30,30);
				}
		if (item != 17){	//Display the item info
			ST7735_DrawString(5,0,itemsInfo[item * 3 + (myLanguage*51)],0xFFFF);
			ST7735_DrawString(0,3,itemsInfo[item * 3 + (myLanguage*51) + 1],0xFFFF);
			ST7735_DrawString(0,4,itemsInfo[item * 3 + (myLanguage*51) + 2],0xFFFF);
			if (shop)		ST7735_DrawString(11,1,otherDialogue[2 + myLanguage],0xFFFF);
			else 				ST7735_DrawString(11,1,"         ",0xFFFF);
		}else if (shop){	//if shopkeeper, display the shop dialogues showing prices of items
			ST7735_DrawString(5,0,shopDialogue[myLanguage * 5],0xFFFF);
			ST7735_DrawString(0,3,shopDialogue[(myLanguage * 5) + 1],0x12B7);
			ST7735_DrawString(7,3,shopDialogue[(myLanguage * 5) + 2],0xF800);
			ST7735_DrawString(0,4,shopDialogue[(myLanguage * 5) + 3],0x07E0);
			ST7735_DrawString(7,4,shopDialogue[(myLanguage * 5) + 4],0x001F);
			ST7735_DrawString(11,1,"         ",0xFFFF);
		}
}
//**********Draw_Health********************
//Draws hearts in the stats section according to current and max health
//	Remaining health is indicated by red hearts
//	Lost health is indicated by grey hearts
//inputs: hp the current hp of the player
//				max the max hp of the player
//outputs: none
void Draw_Health(uint32_t hp, uint32_t max) { 
	int i = 0;	//keeps track of the next x coord
	while (hp && hp <= max) {	//loops (current hp)/2 (rounded up) times
		if (hp == 1) { 
			ST7735_DrawBitmap(i,29,halfheart,12,12);	//if an odd number, draw a halfheart
			ST7735_DrawBitmap(i+6,29,halfbrokenheart,6,12);				//also draw a halfbrokenheart
			i += 12;	//increase the x coord by 12 (one heart width)
			hp--;			//decrement hp by 1
		} else { 
			ST7735_DrawBitmap(i,29,heart,12,12);	//if > 1, draw a full heart
			i += 12;	//increase the x coord by 16 (one heart width)
			hp -= 2;	//decrease hp by 2
		}
	}while (i < max * 6) { 
		ST7735_DrawBitmap(i,29,brokenheart,12,12);	//fill in the full lost hearts
		i += 12;	//increase the x coord by 12 (one heart width)
	}
}

//Initialize the SysTick ISR
void SysTick_Init(uint32_t speed){	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = speed;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x60000000;
	NVIC_ST_CTRL_R = 7;
}

void Sample_Joystick(void){
	dataX = ADC_InX();
	dataY = ADC_InY();
}

//SysTick Handler
void SysTick_Handler(void){
	int i,count;	//variables for loops
	
	//checks for tremors, will cause the screen to rotate
	if (shakeFlag){
		ST7735_SetRotation(shakeFlag % 4);
		shakeFlag--;
		if (shakeFlag % 4 ==  0){
			ST7735_SetRotation(0);
			ST7735_FillScreen(0x0000);
			redrawStats = 0xF;
			Draw_Borders();	//Draw the stage
			ST7735_DrawBitmap(0,15,FLO,20,16);	//Draw the letters FLO to be used in the stats
			ST7735_DrawString(6,1,"R:",0xFFFF);
			Redraw_All(&lonk);
		}
	}
	
	//Check if player has been slowed, and for how long
	if (lonk.speedDelay){
			lonk.speedDelay--;
	}else{
		lonk.speed = 0;
	}if (lonk.speed && lonk.speedCount == 0){
		lonk.speedCount = lonk.speed;
	}if (lonk.speedCount != 0){
		lonk.speedCount--;
	}//if no speed debuffs or waits, able to move 
	else{
		//reads the data from the ADC and determine joystick movement
		if (dataX > 0xB00 || dataX < 0x500 || dataY > 0xB00 || dataY < 0x500) {
			if (dataX < dataY && (dataX + dataY) < 4096){	
				Player_Move_Up(&lonk);
			}else if (dataX < dataY && (dataX + dataY) > 4096){	
				Player_Move_Right(&lonk);
			}else if (dataX > dataY && (dataX + dataY) > 4096){ 
				Player_Move_Down(&lonk);
			}else if (dataX > dataY && (dataX + dataY) < 4096){	
				Player_Move_Left(&lonk);
			}
		}
	}//if there are rupoos on the stage, check if player is touching them to collect
	if (currentStage->rSize)	Check_Rupoos(&lonk);
	
	//loop through obstacles and reduce the durability for temporary obstacles	
	i = count = 0;
	while (count < currentStage->totalObstacles && i < 28){
		if (currentStage->obstacles[i].dur > 0){
			count++;
			if (currentStage->obstacles[i].dur != 0x7FFF){	//0x7FFF means infinite durability
				currentStage->obstacles[i].dur--;	
			}
		}i++;
	}//check if player is currently invincible
	if (!lonk.invinCount){
		//if not, player can receive damage
		Check_Damage(&lonk);
	}else	{
		//if so, reduce invincibility time
		lonk.invinCount--;
	}//damagePt is used to manage redrawing collisions with player and an enemy
	//Will redraw enemy until not overlapped
	if (damagePt){
		if (Check_Overlap(lonk.pos[0], lonk.pos[1], lonk.dims[0], lonk.dims[1], damagePt)){
			if (damagePt->health != 0){
				damagePt->redraw = 1;
			}damagePt = 0;
		}
	}//decrease the item delay for all items
	for (i = 0;i < lonk.numItems;i++){
		if (lonk.itemDelay[i] != 0){
			lonk.itemDelay[i]--;
		}
	}//if the current item is not ready for use, set the cantUseItemFlag if not already set
	if (lonk.itemDelay[lonk.currentItem] != 0){
		GPIO_PORTE_IM_R &= ~0x2;
		if (cantUseItemFlag == 0)	cantUseItemFlag = 1;
	}else{
		//if delay is finished, reset cantUseItemFlag and redraw items
		if (cantUseItemFlag){
			cantUseItemFlag = 0;
			redrawStats |= 0x1;
		}//check if player used an item or not
		//player using an item is signalled by a value in itemFlag
		GPIO_PORTE_IM_R |= 0x3;			//Arm interrupt on PE1-0
		switch (itemFlag){
			case 1: {
				Use_Shortsword(&lonk);
				break; 
			}case 2:{
				Use_WoodShield(&lonk);
				break; 
			}case 3:{
				Use_Bow(&lonk);
				break; 
			}case 4: {
				Use_Dekunuts(&lonk);
				break; 
			}case 5: {
				Use_Needles(&lonk);
				break; 
			}case 6: {
				Use_Bomb(&lonk);
				break; 
			}case 7: {
				Use_BGSword(&lonk);
				break; 
			}case 9: {
				Use_Bombchu(&lonk);
				break; 
			}case 10: {
				Use_IceRod(&lonk);
				break; 
			}case 11: {
				Use_HShield(&lonk);
				break; 
			}case 12: {
				Use_FireRod(&lonk);
				break; 
			}case 13: {
				Use_Quake(&lonk);
				break; 
			}case 14: {
				Use_LightBow(&lonk);
				break; 
			}case 15: {
				Use_MSword(&lonk);
				break; 
			}case 16: {
				Use_Mjolnir(&lonk);
			}
		}
	}//invertFlag used to notify the player of damage taken
	if (invertFlag){
		if (invertFlag == 1){
			ST7735_InvertDisplay(0);
		}
		invertFlag--;
	}//for all enemies, reduce their speedCount and if 0, then move the enemy
	entity_t *e = &(currentStage->enemies[0]);
	for (i = 0;i<currentStage->totalEnemies;i++){
		if (e->health > 0){
			e->speedCount--;
			if (e->speedCount == 0){
				e->speedCount = e->speed;
				Move_Enemy(e);
			}//in addition, if the enemy uses projectiles, reduce their shootCount and if 0, fire a shot
			if (e->ammo){
				if (e->shootCount == 0){
					if (e->ammo & 0xF0){
						if (Omni_Fire(e)) e->shootCount = e->shootDelay;	//Omni_Fire shoots projectiles on all sides
					}
					else if (Entity_Fire(e, &lonk))	e->shootCount = e->shootDelay;
				}else	e->shootCount--;
			}
		}e++;
	}//loop through projectile array and move/explode/destroy projectiles when needed
	count = i = 0;
	uint8_t deleted = 0;
	while (count < currentStage->pSize && i < pAsize){
		if (currentStage->pA[i].exists == 1){
			proj_t *temp = &(currentStage->pA[i]);
			count++;
			if (temp->speedCount){
				temp->speedCount--;
			}//if bomb/bombchu, uses the more significant bits as timers until exploding
			else if ((temp->ammo == bombAmmo || temp->ammo == bombchuAmmo) && 
				(temp->speed & 0x00FF0000 || ((temp->speed  & 0xFF000000) == 0))){
				temp->speed -= 0x10000;
				if ((temp->speed & 0x00FF0000) == 0 && (temp->speed & 0xFFFF)){
					temp->speed &= ~0xFFFFFF; temp->speedCount = 0;
				}else if (temp->speed & 0x00FF0000){
					temp->speedCount = temp->speed;
					if (!Move_Proj(temp, currentStage)){
						temp->speed &= 0xFF000000; temp->speedCount = 0;
					}
				}
			}else if ((temp->ammo == bombAmmo || temp->ammo == bombchuAmmo)  
									&& temp->speed & 0xFF000000){
				if ((temp->speed & 0xFF000000) != 0x01000000){
						temp->redraw = 1;
						temp->speed -= 0x1000000;
				}else {
						//Blow up that bomb boy
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black10x10,10,10);
						temp->exists = 0;
						Explode_Bomb(temp);
						deleted++;
				}
			}//if not a bomb, move the projectile. If it can no longer move, it is destroyed
			else{
				temp->speedCount = temp->speed & 0xFFFF;
				if (!Move_Proj(temp, currentStage)){
					if (temp->dims[0] == 9 && temp->dims[1] == 3){
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black9x3,9,3);
					}else if (temp->dims[0] == 3 && temp->dims[1] == 9){
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black3x9,3,9);
					}else if (temp->dims[0] == 8 && temp->dims[1] == 8){
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black8x8,8,8);
					}else if (temp->dims[0] == 10 && temp->dims[1] == 10){
						ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black10x10,10,10);
					}currentStage->pA[i].exists = 0;
					deleted++;
				}
			}
		}i++;
	}currentStage->pSize -= deleted;	//update psize with deleted projectiles
	
	//loop through rupoo array and reduce durations and redraw them.
	//	rupoos have a higher draw priority than anything else
	i = count = 0;
	while (count < currentStage->rSize && i < 16){
		if (currentStage->rQ[i].value != 0 || currentStage->rQ[i].redraw == 1){
			count++;
			if (currentStage->rQ[i].duration != 0){
				currentStage->rQ[i].duration--;
				currentStage->rQ[i].redraw = 1;
			}
		}i++;
	}
}

//Rising edge trigger interrupt for pausing
//	will stop all other interrupts and music
//	will also resume all other interrupts and music
void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x10;
	//if resuming, turn back on all other interrupts and exit pause loop in main
	if (pause){
		pause = 0;
		Music_Resume();
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R = 3;
		NVIC_ST_CTRL_R = 7;
	}//if pausing, stop all other interrupts and loop infinitely in main
	else{
		pause = 1;
		Music_Pause();
		TIMER0_CTL_R = 0;
		GPIO_PORTE_IM_R = 0;
		NVIC_ST_CTRL_R = 0;
	}

}

//Will decrease the score by 5 every 30 seconds
void ScoreTrack(void){
	static uint8_t counter = 29;
	if (counter == 0 && score >= 5){	
		score -= 5;
		counter = 29;
	}else counter--;
}

//Display end credits and final stats
void Ending(void){
	static int x = 0;
	static int i = 0;
	static int line = 0;
	if (line == 0 && x < 118){
		ST7735_DrawBitmap(x,39,playerSprites[1 + (myType * 4)],10,10);
		if (x % 6 == 0 && x >= 12){
			ST7735_DrawString(i+1,3,credits[(myLanguage * 18) + i],0x07E0);
			i++;
		}
		x++;
	}if (line == 0 && x == 118){
		line = 1;
		ST7735_DrawBitmap(x-1,39,black10x10,10,10);
		ST7735_DrawChar(114,30,(floorNum - 1) + 0x30,0xFE00,0x0000,1);
		i = 15;
	}
	if (line == 1 && x >= 0){
		ST7735_DrawBitmap(x,79,playerSprites[3 + (myType * 4)],10,10);
		if (x == 90){
			ST7735_SetCursor(17,7);
			LCD_OutDec(totalRupoos);
		}else if (x % 6 == 0 && x < 84){
			ST7735_DrawString(i+1,7,credits[(myLanguage * 16) + 36 + i],0x001F);
			i--;
		}
		x--;
	}if (line == 1 && x == -1){
		line = 2;
		ST7735_DrawBitmap(x+1,79,black10x10,10,10);
		ST7735_DrawString(i+1,7,credits[(myLanguage * 16) + 36 + i],0x001F);
		i--;
		ST7735_DrawString(i+1,7,credits[(myLanguage * 16) + 36 + i],0x001F);
		x = i = 0;
	}if (line == 2 && x < 118){
		ST7735_DrawBitmap(x,119,playerSprites[1 + (myType * 4)],10,10);
		if (x % 6 == 0 && x >= 12 && x < 102){
			ST7735_DrawString(i+1,11,credits[(myLanguage * 15) + 68 + i],0xFE00);
			i++;
		}
		x++;
	}if (line == 2 && x == 118){
		line = 3;
		ST7735_DrawBitmap(x-1,119,black10x10,10,10);
		ST7735_SetCursor(16,11);
		LCD_OutDec(score);
	}
	if (line == 3 && x >= 0){
		ST7735_DrawBitmap(x,159,playerSprites[3 + (myType * 4)],10,10);
		x--;
	}if (line == 3 && x == -1){
		line = 4;
		ST7735_DrawBitmap(x+1,159,black10x10,10,10);
	}
}
