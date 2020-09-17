//player.c
//Runs on TM4C123
//Zachary Chin and Alex Liang
//Last Modified: 5/4/2020
//This module contains the functions used to create a player
//	and actions performed on the player.

#include <stdint.h>
#include <stdlib.h>
#include "zstructs.h"
#include "player.h"
#include "images.h"
#include "stage.h"
#include "ST7735.h"

typedef enum {Lonk = 0, Yerr = 1, Val = 2} type_t;

extern uint8_t redrawStats;
extern stage_t *currentStage;
extern player_t lonk;
extern uint8_t invertFlag;
extern entity_t *damagePt;
extern type_t myType;
extern uint32_t score,totalRupoos;

//**********Make_Lonk******************************
//turns a player_t into a lonk
//inputs: lonk a pointer to a player_t
//outputs: none
void Make_Lonk(player_t *lonk){
	//Sets the player's initial position to the bottom left corner
	lonk->pos[0] = 19;	lonk->pos[1] = 149;
	//the player starts off with 8 health max
	lonk->maxHealth = 6;	lonk->health = 6;
	//starts off with no rupoos and not slowed
	lonk->rupoos = 0;			lonk->speedDelay = 0;
	//the player moves every other clock cycle
	lonk->speed = 0;	lonk->speedCount = 0;
	//the items the player currently starts with are:
	//shortsword and wooden shield
	lonk->items[0] = 1;		lonk->items[1] = 2;	
	lonk->items[2] = 15;	lonk->items[3] = 6;
	lonk->items[4] = 13;
	lonk->numItems = 5;	lonk->currentItem = 0;
	//player starts facing downwards and not invincible
	lonk->image = lonk_down;	lonk->invinCount = 0;
	//the player's hitbox is 10x10
	lonk->dims[0] = 10;	lonk-> dims[1] = 10;
	//starts facing down and needs to be drawn
	lonk->dir = dn; 	lonk->redraw = 1;
}

//**********Make_Yerraballi******************************
//turns a player_t into a yerraballi
//inputs: yerr a pointer to a player_t
//outputs: none
void Make_Yerraballi(player_t *yerr){
	//Sets the player's initial position to the bottom left corner
	yerr->pos[0] = 19;	yerr->pos[1] = 149;
	//the player starts off with 6 health max
	yerr->maxHealth = 8;	yerr->health = 8;
	//starts off with no rupoos and not slowed
	yerr->rupoos = 0;			yerr->speedDelay = 0;
	//the player moves every clock cycle
	yerr->speed = 0;	yerr->speedCount = 0;
	//the items the player currently starts with are:
	//shortsword, wooden shield, and bow and arrow
	yerr->items[0] = 1;		yerr->items[1] = 2;	
	yerr->items[2] = 14;	yerr->items[3] = 9;
	yerr->numItems = 4;	yerr->currentItem = 0;
	//player starts facing downwards and not invincible
	yerr->image = yerdn;	yerr->invinCount = 0;
	//the player's hitbox is 10x10
	yerr->dims[0] = 10;	yerr-> dims[1] = 10;
	//starts facing down and needs to be drawn
	yerr->dir = dn; 	yerr->redraw = 1;
}

//**********Make_Valvano******************************
//turns a player_t into a val
//inputs: val a pointer to a player_t
//outputs: none
void Make_Valvano(player_t *val){
	//Sets the player's initial position to the bottom left corner
	val->pos[0] = 19;	val->pos[1] = 149;
	//the player starts off with 8 health max
	val->maxHealth = 8;	val->health = 8;
	//starts off with 50 rupoos and not slowed
	val->rupoos = 50;			val->speedDelay = 0;
	//the player moves every clock cycle
	val->speed = 0;	val->speedCount = 0;
	//the items the player currently starts with are:
	//shortsword, wooden shield, and bombchu
	val->items[0] = 1;		val->items[1] = 2;	
	val->items[2] = 9;		val->items[3] = 16;
	val->numItems = 4;	val->currentItem = 0;
	//player starts facing downwards and not invincible
	val->image = vadn;	val->invinCount = 0;
	//the player's hitbox is 10x10
	val->dims[0] = 10;	val-> dims[1] = 10;
	//starts facing down and needs to be drawn
	val->dir = 2; 	val->redraw = 1;
}

//**********Switch_Item********************
//switches the current item to the next item in the array
//inputs: lonk the player to switch items
//outputs: none
void Switch_Item(player_t *lonk){
	//increment the currentItem, to point to the next item
	lonk->currentItem = (lonk->currentItem + 1) % lonk->numItems;	
	redrawStats |= 0x1;	//redraw the currentItem
}

//**********Check_Item*********************
//Checks to see if an item is currently in player's inventory
//	used for initializing shop
//inputs: num the item number to check
//outputs: 1 if item is in the inventory
//				 0 if not
uint8_t Check_Item(uint8_t num){
	//iterates through player's inventory
	for (int i=0; i < lonk.numItems; i++){
		//if the item is in the inventory, return 1
		if (lonk.items[i] == num){
			return 1;
		}
	}return 0;	//no match found, return 0
}

//**********Check_Rupoos********************
//Checks to see if the player is currently collecting a rupoo
//inputs: lonk a pointer to the player
//outputs: none
void Check_Rupoos(player_t *lonk){
	int i = 0; int count = 0;;	//loop variable
	uint32_t *plPos; uint8_t *plDim;	//pointers to keep track of positions and dimensions
	plPos = &(lonk->pos[0]);	plDim = &(lonk->dims[0]);	//set the pl pointers to the player variables
	while (count < currentStage->rSize && i < 16){	//look through the rupoo array
		if (currentStage->rQ[i].exists == 1 || currentStage->rQ[i].redraw == 1){
			count++;
			rupoo_t *temp = &(currentStage->rQ[i]);	//grab the rupoo for ease of access
			if ((*plPos - (temp->pos[0] + 3) ) < 4 || ((temp->pos[0] + 3) - *plPos) < *plDim){	//check for overlap in x
				plPos++; plDim++;
				if ((*plPos - (temp->pos[1] - 1)) < *plDim || ((temp->pos[1] - 1) - *plPos) < 8){	//check for overlap in y
					lonk->rupoos += temp->value;	//if overlapping, add the rupoo amount to the player's rupoos
					totalRupoos += temp->value;
					score += temp->value;					//update score variable
					temp->duration = 0;						//reduce the rupoo duration to 0 so it disappears in the main
					temp->redraw = 1;							//flag for redraw in the main (will redraw black)
					temp->value = 0;							//additionally reduces its value to 0 to not be picked up again
					redrawStats |= 0x8;						//redrawStats the player's rupoos
				}
			}
		}i++;
	}
}

//**********Check_Damage********************
//Checks to see if the player is currently being damaged
//	Player gains invincibility for a short time after being damaged
//	All player damage is managed here
//inputs: lonk the player to check damage for
//outputs: none
void Check_Damage(player_t *lonk){
	int i, count;	//loop variable
	uint32_t *plPos, *ePos; uint8_t *plDim, *eDim;	//pointers to keep track of positions and dimensions
	//won't take damage from enemies or projectiles if the floor is cleared
	if (currentStage->cleared == 0){
		//Check to see if player is being damaged by an enemy
		plPos = &(lonk->pos[0]);	plDim = &(lonk->dims[0]);	//set the pl pointers to the player variables
		for (i=0;i<currentStage->totalEnemies;i++){				//loop a totalEnemies # of times
			if (currentStage->enemies[i].health > 0){			//skip this enemy if it is dead
				ePos = &(currentStage->enemies[i].pos[0]);		//set the e pointers to enemy variables
				eDim = &(currentStage->enemies[i].dims[0]);
				//check if the xs are overlapped
				if ((*plPos - *ePos) < *eDim || (*ePos - *plPos) < *plDim){	
					plPos++; ePos++; plDim++; eDim++;	//pointers point at y/height
					//check if the ys are overlapped
					if ((*plPos - *ePos) < *plDim || (*ePos - *plPos) < *eDim){
						lonk->health-= currentStage->enemies[i].damage;	//decrease health by enemy damage
						lonk->redraw = 1;		//redraw lonk
						ST7735_InvertDisplay(1);	//invert the display to indicate damage taken
						invertFlag = 4;		//screen is inverted for 4 loops of systick
						redrawStats |= 0x2;	//redraw player health
						currentStage->enemies[i].redraw = 1;	//redraw enemy
						damagePt = &(currentStage->enemies[i]);	//store pointer to enemy in a flag (to keep redrawing until player moves)
						lonk->invinCount = 60;	//player has 2 seconds of invincibility
						return;	//exit if damage taken
					}plPos--; plDim--;	//decrement the pl pointers to point back at x/width
				}
			}
		}i=count=0;	//reset variables to 0
		//check to see if player is being damaged by any projectiles
		plPos = &(lonk->pos[0]);	plDim = &(lonk->dims[0]);	//set the pl pointers to the player variables
		while (count<currentStage->pSize && i < pAsize){	//iterate until pSize # of projs found or end of array
			if (currentStage->pA[i].exists == 1){		//make sure the proj actually exists
				proj_t *temp = &(currentStage->pA[i]);	//grab proj for ease of access
				if (!temp->friendly && temp->ammo != bombAmmo && temp->ammo != bombchuAmmo){		//make sure the proj isn't the player's
					if ((*plPos - temp->pos[0]) < temp->dims[0] || (temp->pos[0] - *plPos) < *plDim){	//check for overlap in x
						plPos++; plDim++;
						if ((*plPos - temp->pos[1]) < *plDim || (temp->pos[1] - *plPos) < temp->dims[1]){	//check for overlap in y
							//if hit by a dekunut, player can't move for a short time (stunned)
							if ((temp->ammo & 0xF) == dekunutAmmo || (temp->ammo & 0xF) == boltAmmo){	
								lonk->speedCount = 60;
							}lonk->health -= temp->damage;	//reduce player health by the damage of the proj
							lonk->redraw = 1;			//redraw the player
							lonk->invinCount = 60;	//player has 2 seconds of invincibility
							ST7735_InvertDisplay(1);	//invert display to indicate damage
							invertFlag = 4;		//make sure inversion is visible
							redrawStats |= 0x2;	//redraw player health
							//erase the projectile after striking player
							if (temp->dims[0] == 9 && temp->dims[1] == 3){	
								ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black9x3,9,3);
							}else if (temp->dims[0] == 3 && temp->dims[1] == 9){
								ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black3x9,3,9);
							}else if (temp->dims[0] == 8 && temp->dims[1] == 8){
								ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black8x8,8,8);
							}else if (temp->dims[0] == 10 && temp->dims[1] == 10){
								ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black10x10,10,10);
							}currentStage->pA[i].exists = 0;	//proj no longer exists after hit
							currentStage->pSize--;	//reduce the num of projectiles in the array
							return;	//exit the method
						}plPos--; plDim--;	//decrement the pl pointers to point back at x/width
					}
				}count++;		//increment count if an existing proj is found
			}i++;		//increment i every loop
		}
	}i=count=0;	//reset variables
	//check to see if player is being damaged by any obstacles (ex: explosion)
	plPos = &(lonk->pos[0]);	plDim = &(lonk->dims[0]);	//set the pl pointers to the player variables
	while (count<currentStage->totalObstacles && i < 28){	//loop until all obstacles checked or out of bounds
		//obstacle doesn't exist unless its durability is greater than 0
		if (currentStage->obstacles[i].dur > 0){
			count++;	//found one, so increment count
			entity_t *temp = &(currentStage->obstacles[i]);	//grab obstacle for ease of access
			if (temp->damage > 0){		//make sure the obstacle deals damage (a block wont hurt player)
				if ((*plPos - temp->pos[0]) < temp->dims[0] || (temp->pos[0] - *plPos) < *plDim){	//check overlap in x
					plPos++; plDim++;
					if ((*plPos - temp->pos[1]) < *plDim || (temp->pos[1] - *plPos) < temp->dims[1]){ //check overlap in y
						lonk->health -= temp->damage;		//overlapped, so decrease player health by obstacle damage
						lonk->redraw = 1;		//redraw player
						ST7735_InvertDisplay(1);	//invert display to indicate damage
						invertFlag = 4;						//make sure inversion is visible
						redrawStats |= 0x2;	//redraw player health
						lonk->invinCount = 60;	//player has 2 seconds of invincibility
						return;		//exit if hit
					}plPos--; plDim--;	//decrement the pl pointers to point back at x/width
				}
			}			
		}i++;		//increment i every loop
	}lonk->redraw = 1;
}

//**********Check_Move_Player********************
//Checks to see if the new image can be drawn without overriding an obstacle or wall
//inputs: x the x coord of the bottom left corner 
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Move_Player(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY){
	//allows the player to leave through the exits if the stage is cleared
	if (currentStage->cleared){
		uint8_t exits = currentStage->exits;	//store exits in a local variable
		//check if the exit exists and if the player is about to exit
		if ((exits & 0x1 && x <= NSexitX + 1 && x >= NSexitX - 1 && y <= NexitY + 10) ||
			(exits & 0x2 && x >= EexitX - 10 && y <= EWexitY + 1 && y >= EWexitY - 1) ||
			(exits & 0x4 && x <= NSexitX + 1 && x >= NSexitX - 1 && y >= SexitY - 10) ||
			(exits & 0x8 && x <= WexitX + 10 && y <= EWexitY + 1 && y >= EWexitY - 1)){	
				return 1;
			}
	}return Check_Move(x,y,dimX,dimY);	//if not, check move normally
}

//**********Player_Move_Up********************
//moves the player up if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Up(player_t *p){ 
	if (p->dir == up){
		//if player is able to move, then move
		if (Check_Move_Player(p->pos[0], p->pos[1] - 1, 10, 11)){
			p->redraw = 1;			//redraw player
			p->pos[1]--;				//move position up by 1
		}
	}else{
		if (myType == Lonk)	p->image = lonk_up;	//set the image of lonk to up
		else if (myType == Yerr) p->image = yerup;
		else	p->image = vaup;
		p->dir = up;				//set direction to up
		p->redraw = 1;			//redraw player
	}
}

//**********Player_Move_Right********************
//moves the player right if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Right(player_t *p){
	if (p->dir == rt){
		//if player is able to move, then move
		if (Check_Move_Player(p->pos[0] + 1, p->pos[1], 11, 10)){
			p->redraw = 1; 					//redraw player
			p->pos[0]++;						//move position right by 1
		}
	}else{
		if (myType == Lonk)	p->image = lonk_right;	//set the image of lonk to up
		else if (myType == Yerr) p->image = yerrt;
		else	p->image = vart;
		p->dir = rt;						//set direction to right
		p->redraw = 1; 					//redraw player
	}
	
	
}

//**********Player_Move_Down********************
//moves the player down if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Down(player_t *p){ 
	if (p->dir == dn){
		//if player is able to move, then move
		if (Check_Move_Player(p->pos[0], p->pos[1] + 1, 10, 11)){
			p->redraw = 1;					//redraw player
			p->pos[1]++;						//move position down by 1
		}
	}else{
		if (myType == Lonk)	p->image = lonk_down;	//set the image of lonk to up
		else if (myType == Yerr) p->image = yerdn;
		else	p->image = vadn;
		p->dir = dn;						//player is now facing down
		p->redraw = 1;					//redraw player
	}
}

//**********Player_Move_Left********************
//moves the player left if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Left(player_t *p){ 
	if (p->dir == lt){
		//if player is able to move, then move
		if (Check_Move_Player(p->pos[0] - 1, p->pos[1], 11, 10)){
			p->redraw = 1;					//redraw player
			p->pos[0]--;						//move position left by 1
		}
	}else{
		if (myType == Lonk)	p->image = lonk_left;	//set the image of lonk to up
		else if (myType == Yerr) p->image = yerlt;
		else	p->image = valt;
		p->dir = lt;						//player is now facing left
		p->redraw = 1;					//redraw player
	}
}

