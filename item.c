//item.c

#include <stdint.h>
#include <stdlib.h>
#include "zstructs.h"
#include "player.h"
#include "ST7735.h"
#include "entity.h"
#include "Sound.h"
#include "../inc/tm4c123gh6pm.h"

extern player_t lonk;
extern uint32_t dataX, dataY;
extern entity_t *zPtr;
extern uint8_t itemFlag;
extern uint8_t redrawStats;
extern uint8_t cantUseItemFlag;
extern stage_t *currentStage;
extern uint8_t shakeFlag;

//Rising edge trigger interrupt
void GPIOPortE_Handler(void){
	if (GPIO_PORTE_RIS_R & 0x1) { 
		GPIO_PORTE_ICR_R = 0x1;
		Switch_Item(&lonk);
	} else if (GPIO_PORTE_RIS_R & 0x2) { 
		GPIO_PORTE_ICR_R = 0x2;
		GPIO_PORTE_IM_R &= ~0x3;
		TIMER0_CTL_R = 0;
		dataX = dataY = 0x800;
		itemFlag = lonk.items[lonk.currentItem];
	}
}

//**********Use_ShortSword********************
//makes the player swing a shortsword
//	deals 1 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 1
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Shortsword(player_t *p){
	int i, i2;
	entity_t *e;
	static uint8_t count;
	static uint32_t x, y, dir;
	if (x == 0){
		x = p->pos[0]; 
		y = p->pos[1]; 
		dir = p->dir;
	}if (count == 0){
		Sound_hyah();
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp1,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp1,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp1,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp1,10,10);
		}
	}else if (count == 1){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp2,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp2,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp2,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp2,10,10);
		}
	}else if (count == 2){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp3,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp3,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp3,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp3,10,10);
		}
	}else if (count == 3){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp4,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp4,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp4,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp4,10,10);
		}
	}else if (count == 4){
		i2 = 0;
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
					e->health--;
					Sound_Hit();
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Up();
		}else if (dir == rt){
			ST7735_DrawBitmap2(0, x+10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
					e->health--;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Right();
		}else if (dir == dn){
			ST7735_DrawBitmap2(0, x,y+10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
					e->health--;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Down();
		}else if (dir == lt){
			ST7735_DrawBitmap2(0, x-10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
					e->health--;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Left();
		}
		count = 0xFF;	x = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 10;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
		TIMER0_CTL_R = 1;
	}count++;
}

//**********Use_WoodShield********************
//uses a shield to block projectiles and enemies
//	Item ID: 2
//inputs: p a pointer to the player using the item
//outputs: none
void Use_WoodShield(player_t *p){
	int i = 0;
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Hit();
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10) && 
				currentStage->totalObstacles < 29){
			Make_WShieldObs(zPtr,x,y-10,up);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}else if (p->dir == rt && Check_Move(x+10,y,10,10) && 
				currentStage->totalObstacles < 29){
			Make_WShieldObs(zPtr,x+10,y,rt);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}else if (p->dir == dn && Check_Move(x,y+10,10,10) && 
				currentStage->totalObstacles < 29){
			Make_WShieldObs(zPtr,x,y+10,dn);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}else if (p->dir == lt && Check_Move(x-10,y,10,10) && 
				currentStage->totalObstacles < 29){
			Make_WShieldObs(zPtr,x-10,y,lt);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
		}
	}else if (count > 0 && count < 10){
		count++;
	}else if (count == 10){
		count = 0;	x = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 80;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_Bow********************
//shoots an arrow from link's bow
//	deals 1 damage if it hits 
//	deals 0 damage if it hits an obstacle
//	Item ID: 2		Ammo: 1
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bow(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = 1;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Projectile();
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			ST7735_DrawBitmap2(1,x,y-10,bow1,10,10);
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			ST7735_DrawBitmap2(0,x+10,y,bow1,10,10);
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			ST7735_DrawBitmap2(3,x,y+10,bow1,10,10);
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			ST7735_DrawBitmap2(2,x-10,y,bow1,10,10);
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 5){
		count++;
	}else if (count == 5){
		if (p->dir == up){
			ST7735_DrawBitmap2(1,x,y-10,bow2,10,10);
		}else if (p->dir == rt){
			ST7735_DrawBitmap2(0,x+10,y,bow2,10,10);
		}else if (p->dir == dn){
			ST7735_DrawBitmap2(3,x,y+10,bow2,10,10);
		}else if (p->dir == lt){
			ST7735_DrawBitmap2(2,x-10,y,bow2,10,10);
		}count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 20;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_Dekunut********************
//fires a dekunut that stuns any enemy it hits
//	deals 1 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 4		Ammo: 2
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Dekunuts(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = 2;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Projectile();
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 5){
		count++;
	}else if (count == 5){
		count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 90;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_Needles********************
//fires two needles
//	deals 1 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 5		Ammo: 7
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Needles(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = needleAmmo;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Projectile();
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 5){
		count++;
	}else if (count == 5){
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 30;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_Bomb********************
//fires a bomb that explodes, damaging nearby enemies
//	deals 2 damage if it hits an enemy
//	breaks any adjacent obstacles
//	Item ID: 6		Ammo: 3
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bomb(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = 3;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 5){
		count++;
	}else if (count == 5){
		count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 120;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_BGSword********************
//makes the player swing the biggoron sword
//	deals 3 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 7
//inputs: p a pointer to the player using the sword
//outputs: none
void Use_BGSword(player_t *p){
	int i, i2;
	entity_t *e;
	static uint8_t count;
	static uint32_t x, y, dir;
	if (x == up){
		x = p->pos[0]; 
		y = p->pos[1]; 
		dir = p->dir;
	}if (count == 0){
		Sound_hyah();
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp2,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp2,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp2,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp2,10,10);
		}
	}else if (count == 2){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp3,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp3,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp3,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp3,10,10);
		}
	}else if (count == 3){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp4,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp4,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp4,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp4,10,10);
		}
	}else if (count == 4){
		i2 = 0;
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Up();
		}else if (dir == rt){
			ST7735_DrawBitmap2(0, x+10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Right();
		}else if (dir == dn){
			ST7735_DrawBitmap2(0, x,y+10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Down();
		}else if (dir == lt){
			ST7735_DrawBitmap2(0, x-10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Left();
		}
		count = 0xFF;	x = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 30;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
		TIMER0_CTL_R = 1;
	}count++;
}

//**********Use_Bombchu********************
//fires a bomb that explodes, damaging nearby enemies
//faster than the normal bomb
//	deals 2 damage if it hits an enemy
//	breaks any adjacent obstacles
//	Item ID: 9		Ammo: 4
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bombchu(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = 4;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 4){
		count++;
	}else if (count == 4){
		count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 80;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_IceRod********************
//freezes all enemies on the screen temporarily
//	deals 1 damage
//	Item ID: 10	
//inputs: p a pointer to the player using the item
//outputs: none
void Use_IceRod(player_t *p){
	static uint8_t count;
	if (count == 50){
		int i=0;
		count++;
		if (!currentStage->cleared){
			for (i = 0;i<currentStage->totalEnemies;i++){		//loop through the array of enemies in the current stage
				entity_t *e = &(currentStage->enemies[i]);		//grab the enemy for easier access
				if (e->health > 0){
					e->health--;
					Sound_Hit();
					e->redraw = 1;
					e->speedCount = 300;
				}
			}
		}
	}else if (count < 100){
		count++;
	}else if (count == 100){
		count = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 420;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_Quake********************
//shakes the screen immobilizing and damaging all enemies
//	deals 2 damage
//	Item ID: 11	
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Quake(player_t *p){
	static uint8_t count;
	if (count == 50){
		int i=0;
		count++;
		if (!currentStage->cleared){
			for (i = 0;i<currentStage->totalEnemies;i++){		//loop through the array of enemies in the current stage
				entity_t *e = &(currentStage->enemies[i]);		//grab the enemy for easier access
				if (e->health > 0){
					e->health -= 2;
					Sound_Hit();
					e->redraw = 1;
					e->speedCount = 210;
				}
			}shakeFlag = 23;
		}
	}else if (count < 100){
		count++;
	}else if (count == 100){
		count = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 500;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_HShield********************
//uses a shield to block projectiles and enemies
//	creates a shield in all 4 directions
//	Item ID: 12
//inputs: p a pointer to the player using the item
//outputs: none
void Use_HShield(player_t *p){
	int i = 0;
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Hit();
		count++;
		if (Check_Move(x,y-10,10,10) && 
				currentStage->totalObstacles < 29){
			Make_HShieldObs(zPtr,x,y-10,up);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}if (Check_Move(x+10,y,10,10) && 
				currentStage->totalObstacles < 29){
			Make_HShieldObs(zPtr,x+10,y,rt);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}if (Check_Move(x,y+10,10,10) && 
				currentStage->totalObstacles < 29){
			Make_HShieldObs(zPtr,x,y+10,dn);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}if (Check_Move(x-10,y,10,10) && 
				currentStage->totalObstacles < 29){
			Make_HShieldObs(zPtr,x-10,y,lt);
			while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
					i++;
			}currentStage->obstacles[i] = *zPtr;
			currentStage->totalObstacles++;
		}
	}else if (count > 0 && count < 30){
		count++;
	}else if (count == 30){
		count = 0;	x = 0;
		itemFlag = 0;
		p->itemDelay[p->currentItem] = 80;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}


//**********Use_FireRod********************
//fires a fireball in all 4 directions
//	deals 5 damage if it hits an enemy
//	Item ID: 13		Ammo: 8
//inputs: p a pointer to the player using the item
//outputs: none
void Use_FireRod(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = fireAmmo;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 40){
		count++;
		if (Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}if (Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}if (Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}if (Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}
	}else if (count < 50){
		count++;
	}else if (count == 50){
		count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 200;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_LightBow********************
//shoots 2 arrows from zorlda's lightbow
//	deals 3 damage ifor each hit
//	deals 0 damage if it hits an obstacle
//	Item ID: 14		Ammo: 9
//inputs: p a pointer to the player using the item
//outputs: none
void Use_LightBow(player_t *p){
	static uint8_t count;
	static uint32_t x, y;
	if (x == 0){
		p->ammo = lightAmmo;
		x = p->pos[0];
		y = p->pos[1];
	}if (count == 0){
		Sound_Projectile();
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			ST7735_DrawBitmap2(0,x,y-10,lightbowup,10,10);
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			ST7735_DrawBitmap2(3,x+10,y,lightbowup,10,10);
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			ST7735_DrawBitmap2(2,x,y+10,lightbowup,10,10);
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			ST7735_DrawBitmap2(1,x-10,y,lightbowup,10,10);
			Make_Proj_Player(p, lt);
		}else{
			count = 0;	x = 0;
			itemFlag = 0;
			TIMER0_CTL_R = 1;
			GPIO_PORTE_IM_R |= 0x3;
			p->ammo = 0;
		}
	}else if (count > 0 && count < 8){
		count++;
	}else if (count == 8){
		count++;
		if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}
	}else if (count > 5 && count < 15){
		count++;
	}else if (count == 15){
		if (p->dir == up){
			ST7735_DrawBitmap2(0,x,y-10,lightbow2,10,10);
		}else if (p->dir == rt){
			ST7735_DrawBitmap2(3,x+10,y,lightbow2,10,10);
		}else if (p->dir == dn){
			ST7735_DrawBitmap2(2,x,y+10,lightbow2,10,10);
		}else if (p->dir == lt){
			ST7735_DrawBitmap2(1,x-10,y,lightbow2,10,10);
		}count = 0;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 50;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		TIMER0_CTL_R = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
	}
}

//**********Use_MSword********************
//makes the player swing the master sword
//shoots out a wave projectile as well
//	deals 3 damage if it hits an enemy
//	deals 3 damage if the wave hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 15 		Ammo: 5
//inputs: p a pointer to the player using the sword
//outputs: none
void Use_MSword(player_t *p){
	int i, i2;
	entity_t *e;
	static uint8_t count;
	static uint32_t x, y, dir;
	if (x == up){
		p->ammo = waveAmmo;
		x = p->pos[0]; 
		y = p->pos[1]; 
		dir = p->dir;
	}if (count == 0){
		Sound_hyah();
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp2,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp2,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp2,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp2,10,10);
		}
	}else if (count == 2){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp3,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp3,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp3,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp3,10,10);
		}
	}else if (count == 3){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwingUp4,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,ssSwingUp4,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,ssSwingUp4,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,ssSwingUp4,10,10);
		}
	}else if (count == 4){
		i2 = 0;
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Up();
		}else if (dir == rt){
			ST7735_DrawBitmap2(0, x+10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Right();
		}else if (dir == dn){
			ST7735_DrawBitmap2(0, x,y+10,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Down();
		}else if (dir == lt){
			ST7735_DrawBitmap2(0, x-10,y,ssSwing5,10,10);
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
					e->health -= 3;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Left();
		}if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}count = 0xFF;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 70;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
		TIMER0_CTL_R = 1;
	}count++;
}

//**********Use_Mjolnir********************
//makes the player swing mjolnir
//shoots out a bolt projectile as well
//	deals 4 damage if it hits an enemy
//	deals 4 damage if the bolt hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 16 		Ammo: 6
//inputs: p a pointer to the player using the weapon
//outputs: none
void Use_Mjolnir(player_t *p){
	int i, i2;
	entity_t *e;
	static uint8_t count;
	static uint32_t x, y, dir;
	if (x == up){
		p->ammo = boltAmmo;
		x = p->pos[0]; 
		y = p->pos[1]; 
		dir = p->dir;
	}if (count == 0){
		Sound_hyah();
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,mjolnir1,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,mjolnir1,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,mjolnir1,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,mjolnir1,10,10);
		}
	}else if (count == 1){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,mjolnir2,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,mjolnir2,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,mjolnir2,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,mjolnir2,10,10);
		}
	}else if (count == 2){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,mjolnir3,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,mjolnir3,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,mjolnir3,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,mjolnir3,10,10);
		}
	}else if (count == 3){
		if (dir == up){
			ST7735_DrawBitmap2(0, x,y-10,mjolnir4,10,10);
		}else if (dir == rt){
			ST7735_DrawBitmap2(3, x+10,y,mjolnir4,10,10);
		}else if (dir == dn){
			ST7735_DrawBitmap2(2, x,y+10,mjolnir4,10,10);
		}else if (dir == lt){
			ST7735_DrawBitmap2(1, x-10,y,mjolnir4,10,10);
		}
	}else if (count == 4){
		i2 = 0;
		if (dir == up){
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
					e->health -= 4;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y-10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Up();
		}else if (dir == rt){
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
					e->health -= 4;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x+10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Right();
		}else if (dir == dn){
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
					e->health -= 4;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x,y+10,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Down();
		}else if (dir == lt){
			e = &(currentStage->enemies[0]);
			for (i=0;i<currentStage->totalEnemies;i++){
				if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
					e->health -= 4;
					void Sound_Hit(void);
					e->redraw = 1;
				}e++;
			}e = &(currentStage->obstacles[0]);
			while (i2 < currentStage->totalObstacles && i < 28){
				if (e->dur >= 0){
					i2++;
					if (!Check_Overlap(x-10,y,10,10,e) && e->health > 0){
						e->redraw = 1;
					}e++;
				}i++;
			}Draw_Borders_Left();
		}if (p->dir == up && Check_Move(x,y-10,10,10)){
			Make_Proj_Player(p, up);
		}else if (p->dir == rt && Check_Move(x+10,y,10,10)){
			Make_Proj_Player(p, rt);
		}else if (p->dir == dn && Check_Move(x,y+10,10,10)){
			Make_Proj_Player(p, dn);
		}else if (p->dir == lt && Check_Move(x-10,y,10,10)){
			Make_Proj_Player(p, lt);
		}count = 0xFF;	x = 0;
		itemFlag = 0;
		p->ammo = 0;
		p->itemDelay[p->currentItem] = 90;
		redrawStats &= ~0x1;
		cantUseItemFlag = 1;
		GPIO_PORTE_IM_R |= 0x1;			//Arm interrupt on PE1-0
		TIMER0_CTL_R = 1;
	}count++;
}
