//entity.c

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../inc/tm4c123gh6pm.h"
#include "zstructs.h"
#include "ST7735.h"
#include "entity.h"
#include "Images.h"
#include "stage.h"
#include "player.h"
#include "Random.h"

extern player_t lonk;

//**********Check_Overlap********************
//Checks if the given variables currently overlap with an entity
//inputs:	x the x coord of the bottom left corner of the image
//				y the y coord of the bottom left corner of the image
//				dimX the width of the image
//				dimY the height of the image
//				e the entity to compare overlap with
//outputs: 1 if there is no overlap
//				 0 if there is an overlap
int Check_Overlap(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY, entity_t *e){
	uint32_t *ePos;	uint8_t *eDim;		//pointers to the entity variables
	ePos = &(e->pos[0]);	eDim = &(e->dims[0]);	//set the pointers to point towards position and dimension
	//check if the xs are overlapped
	if ((x - *ePos) < *eDim || (*ePos - x) < dimX){	
		ePos++, eDim++;	//increment the pointers to look at y/height
		//check if the ys are overlapped
		if ((y - *ePos) < dimY || (*ePos - y) < *eDim){
			return 0;	//there is an overlap, return 0
		}
	}return 1;	//there is not an overlap, return 1
}


//**********Check_Adjacent********************
//Checks if the given player is adjacent to a specific obstacle
//inputs: p the pointer to the player
//				e the pointer to the obstacle
//output: 1 if they are adjacent
//				0 if not
uint8_t Check_Adjacent(player_t *p, entity_t *e){
	if (p->pos[0] - e->pos[0] < 3 || e->pos[0] - p->pos[0] < 3){
		if (e->pos[1] - (p->pos[1] + p->dims[1]) < 8 || p->pos[1] - (e->pos[1] + e->dims[1]) < 8){
			return 1;
		}
	}if (e->pos[1] - p->pos[1] < 3 || p->pos[1] - e->pos[1] < 3){
		if (p->pos[0] - (e->pos[0] + e->dims[0]) < 8 || e->pos[0] - (p->pos[0] + p->dims[0]) < 8){
			return 1;
		}
	}return 0;
}
//**********Move_Enemy_Random********************
//Moves an enemy in a straight line until it hits an obstruction
//	if obstructed, randomly generates a different direction to travel
//inputs: e a pointer to the enemy to move
//outputs: none
void Move_Enemy_Random(entity_t *e){
	if (e->movement >> 2){
		e->movement -= 0x4;
		if (e->dir == up){
			if (Check_Move(e->pos[0],(e->pos[1])-1,e->dims[0],e->dims[1])){
				e->pos[1]--;
				e->redraw = 1;
				return;
			}
		}else if (e->dir == rt){
			if (Check_Move(e->pos[0]+1,e->pos[1],e->dims[0],e->dims[1])){
				e->pos[0]++;
				e->redraw = 1;
				return;
			}
		}else if (e->dir == dn){
			if (Check_Move(e->pos[0],e->pos[1]+1,e->dims[0],e->dims[1])){
				e->pos[1]++;
				e->redraw = 1;
				return;
			}
		}else if (e->dir == lt){
			if (Check_Move(e->pos[0]-1,e->pos[1],e->dims[0],e->dims[1])){
				e->pos[0]--;
				e->redraw = 1;
				return;
			}
		}
	}uint8_t newDir = Random()%4;	//get a random value from 0-3
	e->movement &= 0x3;
	e->movement |= 0x28;
	for (int i = 0;i<4;i++){
		if (newDir == up){
			if (Check_Move(e->pos[0],(e->pos[1])-1,e->dims[0],e->dims[1])){
				e->pos[1]--;
				e->dir = newDir;
				e->redraw = 1;
				return;
			}
		}else if (newDir == rt){
			if (Check_Move(e->pos[0]+1,e->pos[1],e->dims[0],e->dims[1])){
				e->pos[0]++;
				e->dir = newDir;
				e->redraw = 1;
				return;
			}
		}else if (newDir == dn){
			if (Check_Move(e->pos[0],e->pos[1]+1,e->dims[0],e->dims[1])){
				e->pos[1]++;
				e->dir = newDir;
				e->redraw = 1;
				return;
			}
		}else if (newDir == lt){
			if (Check_Move(e->pos[0]-1,e->pos[1],e->dims[0],e->dims[1])){
				e->pos[0]--;
				e->dir = newDir;
				e->redraw = 1;
				return;
			}
		}newDir = (newDir + 1)%4;
	}entity_t *o = Find_Conflict(e);
	e->dir = (o->dir);
	e->movement &= 0x3;
	e->movement |= 0x28;
}

//**********Find_Conflict********************
//Returns the obstacles that the enemy is colliding with
//inputs: e a pointer to the enemy
//outputs: a pointer to the obstacle
//				 0 if no conflicts
entity_t* Find_Conflict(entity_t *e){
	uint32_t x, y;	uint8_t dimX, dimY;
	x = e->pos[0];	y = e->pos[1];	dimX = e->dims[0];	dimY = e->dims[1];
	//checks if it would conflict with any obstacles
	uint32_t *obPos;	uint8_t *obDim;	//local variables to point at obstacle position and dimensions
	int i, count;	//variable for looping
	i = count = 0;
	while (count < currentStage->totalObstacles && i < 28) {	
		if (currentStage->obstacles[i].dur >= 0){
			count++;
			//set the pointers to the positions and dimensions
			obPos = &(currentStage->obstacles[i].pos[0]);
			obDim = &(currentStage->obstacles[i].dims[0]);
			//check if the xs would conflict
			if ((x - *obPos) <  *obDim || (*obPos - x) < dimX) {
				obPos++;	obDim++;	//update to point at the y values
				//check if the ys would conflict
				if ((y - *obPos) < dimY || (*obPos - y) < *obDim){ 
					return &(currentStage->obstacles[i]);	//returns 0 if there is a conflict
				}
			}
		}i++;
	}return 0;
}

//**********Move_Enemy_Proj********************
//Moves an enemy towards the player, but maintains a distance to shoot
//	will move randomly for some time if it cannot move towards
//inputs: e a pointer to the enemy to move
//				p1 a pointer to the first player
//				p2 a pointer to the second player
//outputs: none
void Move_Enemy_Proj(entity_t *e, player_t *p1){
	//obtain a priority of directions (EX: up then left (left will be used if up is blocked))
	uint8_t i;	//variable to hold the priority of directions and a counter
	i = 0; //both set to 0
	//disX = mag of x distance between enemy and player and disY is y distance. disT is total
	uint32_t disX, disY;	
	//the magnitude of the x distance between p1 and enemy
	if (p1->pos[0] > e->pos[0]){
		disX = p1->pos[0] - e->pos[0]; 
		i &= ~0x2;		//if the player is to the right, bit 1 of i is 0
	}else{
		disX = e->pos[0]-p1->pos[0];
		i |= 0x2;		//if the player is to the left, bit 1 of i is 1
	}//the magnitude of the y distance between p1 and enemy
	if (p1->pos[1] > e->pos[1]){ 
		disY = p1->pos[1] - e->pos[1]; 
		i |= 0x01;		//if the player is below the enemy, bit 0 of i is 1
	}else{ 
		disY = e->pos[1]-p1->pos[1];
		i &= ~0x01;		//if the player is below the enemy, bit 0 of i is 0
	}if (disY <= disX){
		if (disX < 40 && Check_Move(e->pos[0] + (-1 + (i & 0x2)),e->pos[1],e->dims[0],e->dims[1])){
			e->pos[0] = e->pos[0] + (-1 + (i & 0x2));
			e->dir = 0x3 - (i & 0x2);
			e->redraw = 1;
			return;
		}if (disY > 3 && Check_Move(e->pos[0],e->pos[1] + (-1 + (2*(i & 0x1))),e->dims[0],e->dims[1])){
			e->pos[1] = e->pos[1] + (-1 + (2*(i & 0x1)));
			e->dir = 0x0 + 2*(i & 0x1);
			e->redraw = 1;
			return;
		}if (Check_Move(e->pos[0] + (1 - (i & 0x2)),e->pos[1],e->dims[0],e->dims[1])){
			e->pos[0] = e->pos[0] + (1 - (i & 0x2));
			e->dir = 0x1 + (i & 0x2);
			e->redraw = 1;
			return;
		}
	}else{
		if (disY < 40 && Check_Move(e->pos[0],e->pos[1] + (1 - 2*(i & 0x1)),e->dims[0],e->dims[1])){
			e->pos[1] = e->pos[1] + (1 - 2*(i & 0x1));
			e->dir = 0x2 - 2*(i & 0x1);
			e->redraw = 1;
			return;
		}if (disX > 3 && Check_Move(e->pos[0] + (1 - (i & 0x2)),e->pos[1],e->dims[0],e->dims[1])){
			e->pos[0] = e->pos[0] + (1 - (i & 0x2));
			e->dir = 0x1 + (i & 0x2);
			e->redraw = 1;
			return;
		}if (disY > 3 && Check_Move(e->pos[0],e->pos[1] + (-1 + (2*(i & 0x1))),e->dims[0],e->dims[1])){
			e->pos[1] = e->pos[1] + (-1 + (2*(i & 0x1)));
			e->dir = 0x0 + 2*(i & 0x1);
			e->redraw = 1;
			return;
		}
	}e->movement |= 0x4C;			//if both ways are obstructed, move randomly for 20 pixels
	Move_Enemy_Random(e);				//Move randomly
}

//**********Move_Enemy_Homing********************
//Moves an enemy towards the player
//	will move randomly for some time if it cannot move towards
//inputs: e a pointer to the enemy to move
//				p1 a pointer to the first player
//				p2 a pointer to the second player
//outputs: none
void Move_Enemy_Homing(entity_t *e, player_t *p1){
	//obtain a priority of directions (EX: up then left (left will be used if up is blocked))
	uint8_t dirTemp, i;	//variable to hold the priority of directions and a counter
	dirTemp = i = 0; //both set to 0
	//disX = mag of x distance between enemy and player and disY is y distance. disT is total
	uint32_t disX, disY;	
	player_t *closerP = p1;	//the closer player is initialized to p1 (in case there is no p2)
	//the magnitude of the x distance between p1 and enemy
	if (p1->pos[0] > e->pos[0]) disX = p1->pos[0] - e->pos[0]; else disX = e->pos[0]-p1->pos[0];
	//the magnitude of the y distance between p1 and enemy
	if (p1->pos[1] > e->pos[1]) disY = p1->pos[1] - e->pos[1]; else disY = e->pos[1]-p1->pos[1];
	if (disY >= disX){		//Check if the enemy is further away vertically or horizontally
		if (closerP->pos[1] < e->pos[1]){	//Then check if player is above the enemy or below
			dirTemp = up;		//if above, set up as first priority
		}else{
			dirTemp = dn;		//if below, set down as first priority
		}i+=2;			//increment counter by 2
	}if (closerP->pos[0] < e->pos[0]){	//then check if player is to the right or left of the enemy
		dirTemp |= lt << i;	//if left, set left as the next priority (1st priority if disX > disY, 2nd if not)
	}else{
		dirTemp |= rt << i;		//if right, set right as the next priority
	}i+=2;				//increment counter by 2
	if (i == 2){	//Check if counter has been incremented once (meaning disX > disY)
		if (closerP->pos[1] < e->pos[1]){	//Check if enemy is above or below
			dirTemp |= up << 2;	//if above, set up as 2nd priority
		}else{
			dirTemp |= dn << 2;	//if below, set down as 2nd priority
		}
	}
	
	//Now we have 2 directions in order of priority. 1st is bits 1-0, 2nd is 3-2
	//	Check for overlap in order of priority 
	//		move randomly for 3 blocks if neither is available
	for (i=0; i<3; i+=2){				//check in order of priority; loop twice
		dirTemp = dirTemp >> i;		//dirTemp right shifted i times (0 the first time and 2 the second
		if ((dirTemp & 0x3) == up){	//if first 2 bits are 0
			if (Check_Move(e->pos[0],(e->pos[1])-1,e->dims[0],e->dims[1])){	//check if obstructed
				e->pos[1]--;					//if obstructed, update position, dir, and redraw
				e->dir = dirTemp & 0x3;
				e->redraw = 1;
				return;		//done
			}
		}else if ((dirTemp & 0x3) == rt){	//if first 2 bits are 1
			if (Check_Move(e->pos[0]+1,e->pos[1],e->dims[0],e->dims[1])){		//check if obstructed
				e->pos[0]++;					//if obstructed, update position, dir, and redraw
				e->dir = dirTemp & 0x3;
				e->redraw = 1;
				return;		//done
			}
		}else if ((dirTemp & 0x3) == dn){	//if first 2 bits are 2
			if (Check_Move(e->pos[0],e->pos[1]+1,e->dims[0],e->dims[1])){		//check if obstructed
				e->pos[1]++;					//if obstructed, update position, dir, and redraw
				e->dir = dirTemp & 0x3;
				e->redraw = 1;
				return;		//done
			}
		}else if ((dirTemp & 0x3) == lt){	//if first 2 bits are 3
			if (Check_Move(e->pos[0]-1,e->pos[1],e->dims[0],e->dims[1])){		//check if obstructed
				e->pos[0]--;					//if obstructed, update position, dir, and redraw
				e->dir = dirTemp & 0x3;
				e->redraw = 1;
				return;		//done
			}
		}
	}e->movement |= 0x4C;			//if both ways are obstructed, move randomly for 20 pixels
	Move_Enemy_Random(e);				//Move randomly
}

//**********Teleport_Enemy*********************
//Teleports the enemy to a random spot
//	moves for a short duration after
//inputs: e a pointer to the enemy to move
//outputs: none
void Teleport_Enemy(entity_t *e){
	uint32_t x, y;
	while (1){
		if (e->dims[0] > 10){
			x = X + (Random() % 70);
			y = Y + (Random() % 70) + 10;
		}else{
			x = X + (Random() % 80);
			y = Y + (Random() % 80);
		}if (Check_Move(x,y,e->dims[0],e->dims[1])){
			ST7735_DrawBitmap(e->pos[0],e->pos[1],black10x10,10,10);
			if (e->dims[0] == 20){
				ST7735_DrawBitmap(e->pos[0]+10,e->pos[1],black10x10,10,10);
				ST7735_DrawBitmap(e->pos[0],e->pos[1]-10,black10x10,10,10);
				ST7735_DrawBitmap(e->pos[0]+10,e->pos[1]-10,black10x10,10,10);
			}e->pos[0] = x;
			e->pos[1] = y;
			e->redraw = 1;
			e->dir = Random() % 4;
			e->movement |= 156;
			return;
		}
	}
}

//**********Move_Enemy*********************
//Moves an enemy according to the movement type
//inputs: e a pointer to the enemy to move
//outputs: none
void Move_Enemy(entity_t *e){
	if ((e->movement & 0x3) == 0 || (e->movement >> 2)){
		Move_Enemy_Random(e);
	}else if ((e->movement & 0x3) == 1){
		Move_Enemy_Homing(e, &lonk);
	}else if ((e->movement & 0x3) == 2){
		Move_Enemy_Proj(e, &lonk);
	}else if ((e->movement & 0x3) == 3){
		Teleport_Enemy(e);
	}
}

extern proj_t *pzPtr;
extern stage_t *currentStage;

//**********Omni_Fire********************
//Fires a projectile in all four directions from enemy
//	unsuccessful if unable to fire in any direction
//	if projectile array is almost full, will fire until full
//inputs: e a pointer to an entity to fire from
//outputs: 1 if one or more projectiles were fired
//				 0 if no projectiles fired
int Omni_Fire(entity_t *e){
	if (currentStage->pSize == pAsize) return 0;
	uint8_t count,pdX,pdY,edX,edY;
	count = 0;
	uint32_t x, y;
	x = e->pos[0];			y = e->pos[1];
	edX = e->dims[0];		edY = e->dims[1];
	if ((e->ammo & 0xF) == arrowAmmo){
		pdX = 3; pdY = 9;
	}else if ((e->ammo & 0xF) == dekunutAmmo){
		pdX = 8; pdY = 8;
	}else{
		pdX = 10; pdY = 10;
	}
	//Check and create an upwards projectile
	if (Check_Move(x+((edX - pdX)/2),y-edY,10,10)){
		Make_Proj(e,up);
		count = 1;
	}if (currentStage->pSize == pAsize) return count;
	//Check and create a right projectile
	if (Check_Move(x+edX,y-((edY + pdX)/2),10,10)){
		Make_Proj(e,rt);
		count = 1;
	}if (currentStage->pSize == pAsize) return count;
	//Check and create a downward projectile
	if (Check_Move(x+((edX - pdX)/2),y+pdY,10,10)){
		Make_Proj(e,dn);
		count = 1;
	}if (currentStage->pSize == pAsize) return count;
	//Check and create a left projectile
	if (Check_Move(x-pdY,y-((edY + pdX)/2),10,10)){
		Make_Proj(e,lt);
		count = 1;
	}return count;	//if no projectiles made, return 0, else return 1
}

//**********Entity_Fire********************
//Fires a projectile from an entity if able to
//	unsuccessful if player is not in line of sight
//	unsuccessful if an obstacle is in the way
//inputs: e a pointer to an entity to fire from
//outputs: 1 if the shoot is successful
//				 0 if the shoot is unsuccessful
int Entity_Fire(entity_t *e, player_t *p1){
	//Check if the proj array is full. Return 0 if full
	if (currentStage->pSize == pAsize) return 0;
	uint32_t diff;	//variable to keep track of the difference of x and y coords
	//compute magnitude distance between the x coords of entity and player1
	if (p1->pos[0] > e->pos[0])	diff = p1->pos[0] - e->pos[0];	
	else	diff = e->pos[0] - p1->pos[0];
	//Check if the are aligned enough to fire
	if (diff < 10){
		//check if player is above and there is no immediate obstruction
		if (p1->pos[1] <= e->pos[1] && Check_Move(e->pos[0] + (e->dims[0]-10)/2,
			e->pos[1] - e->dims[1],10,10)){
			Make_Proj(e,0);	//then create a projectile directly above, facing up
			return 1;	//exit the method and return successful
		}//Check if the player is below and there is no immediate obstruction
		if (p1->pos[1] > e->pos[1] && Check_Move(e->pos[0] + (e->dims[0]-10)/2,
			e->pos[1] + 10,10,10)){
			Make_Proj(e,2);	//then create a projectile directly below, facing down
			return 1;	//exit the method and return successful
		}
	}//compute magnitude distance between the y coords of entity and player1
	if (p1->pos[1] > e->pos[1])	diff = p1->pos[1] - e->pos[1];
	else	diff = e->pos[1] - p1->pos[1];
	//Check if the are aligned enough to fire
	if (diff < 10){
		//check if player is right and there is no immediate obstruction
		if (p1->pos[0] >= e->pos[0] && Check_Move(e->pos[0] + e->dims[0],
			e->pos[1] - (e->dims[1] - 10)/2,10,10)){
			Make_Proj(e,1);	//then create a projectile directly right, facing right
			return 1;	//exit the method and return successful
		}//Check if the player is left and there is no immediate obstruction
		if (p1->pos[0] < e->pos[0] && Check_Move(e->pos[0] - 10,
			e->pos[1] - (e->dims[1] - 10)/2,10,10)){
			Make_Proj(e,3);	//then create a projectile directly left, facing left
			return 1;	//exit the method and return successful
		}
	}return 0;	//at this point there is no successful shot possibly, return unsuccessful
}

//DATABASE FOR ENEMIES WILL BE HERE

//**********Make_Slime********************
//turns an entity into a slime enemy
//inputs: e a pointer to the entity to make a slime
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Slime(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x3;	e->dir = dir;
	e->speed = 8;			e->speedCount = 8;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = slimeEnemy;
	e->dur = 0x7FFF;
	e->movement = 0;
	e->redraw = 1;
}

//**********Make_Skeleton********************
//turns an entity into a skeleton enemy
//inputs: e a pointer to the entity to make a skeleton
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Skeleton(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x4;	e->dir = dir;
	e->speed = 10;		e->speedCount = 10;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 150;
	e->shootCount = 150;	e->ammo = 0x01;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = skeletonEnemy;
	e->dur = 0x7FFF;
	e->movement = 0;
	e->redraw = 1;
}

//**********Make_Snake********************
//turns an entity into a snake enemy
//inputs: e a pointer to the entity to make a snake
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Snake(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x4;	e->dir = dir;
	e->speed = 7;		e->speedCount = 7;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = snakeEnemy;
	e->dur = 0x7FFF;
	e->movement = 2;	
	e->redraw = 1;
}

//**********Make_Squid********************
//turns an entity into a squid enemy
//inputs: e a pointer to the entity to make a squid
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Squid(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x5;	e->dir = dir;
	e->speed = 14;		e->speedCount = 14;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 255;
	e->shootCount = 255;	e->ammo = 5;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = squidEnemy;
	e->dur = 0x7FFF;
	e->movement = 2;	
	e->redraw = 1;
}

//**********Make_Crab********************
//turns an entity into a crab enemy
//inputs: e a pointer to the entity to make a squid
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Crab(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x9;	e->dir = dir;
	e->speed = 4;		e->speedCount = 4;
	e->damage = 2;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = crabEnemy;
	e->dur = 0x7FFF;
	e->movement = 0;	
	e->redraw = 1;
}

//**********Make_Spider********************
//turns an entity into a spider enemy
//inputs: e a pointer to the entity to make a spider
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Spider(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x2;	e->dir = dir;
	e->speed = 4;		e->speedCount = 4;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = spiderEnemy;
	e->dur = 0x7FFF;
	e->movement = 3;	
	e->redraw = 1;
}

//**********Make_Bear********************
//turns an entity into a bear enemy
//inputs: e a pointer to the entity to make a bear
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Bear(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0xA;	e->dir = dir;
	e->speed = 10;		e->speedCount = 10;
	e->damage = 3;		e->phasing = 0;
	e->coins = red;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = bearEnemy;
	e->dur = 0x7FFF;
	e->movement = 1;	
	e->redraw = 1;
}

//**********Make_Snowman********************
//turns an entity into a Snowman enemy
//inputs: e a pointer to the entity to make a snowman
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Snowman(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x6;	e->dir = dir;
	e->speed = 8;		e->speedCount = 8;
	e->damage = 2;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = snowmanEnemy;
	e->dur = 0x7FFF;
	e->movement = 0;	
	e->redraw = 1;
}

//**********Make_Skelebomber********************
//turns an entity into a skeleton bomber enemy
//inputs: e a pointer to the entity to make a skeleton bomber enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Skelebomber(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x2;	e->dir = dir;
	e->speed = 5;		e->speedCount = 5;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 150;
	e->shootCount = 150;	e->ammo = bombAmmo;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = skelebomberEnemy;
	e->dur = 0x7FFF;
	e->movement = 2;	
	e->redraw = 1;
}

//**********Make_Dekuscrub********************
//turns an entity into a Deku Scrub enemy
//inputs: e a pointer to the entity to make a Deku scrub enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Dekuscrub(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x6;	e->dir = dir;
	e->speed = 6;		e->speedCount = 6;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 100;
	e->shootCount = 100;	e->ammo = dekunutAmmo;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = dekuScrubEnemy;
	e->dur = 0x7FFF;
	e->movement = 2;	
	e->redraw = 1;
}

//**********Make_Fbow********************
//turns an entity into a Flying bow enemy
//inputs: e a pointer to the entity to make a Flying bow enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Fbow(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x8;	e->dir = dir;
	e->speed = 5;		e->speedCount = 5;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 100;
	e->shootCount = 100;	e->ammo = arrowAmmo;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = fbowEnemy;
	e->dur = 0x7FFF;
	e->movement = 2;	
	e->redraw = 1;
}

//**********Make_Ebat********************
//turns an entity into a electric bat enemy
//inputs: e a pointer to the entity to make a electric bat enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Ebat(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e-> health = 0x8;	e->dir = dir;
	e->speed = 4;		e->speedCount = 4;
	e->damage = 1;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 125;
	e->shootCount = 125;	e->ammo = boltAmmo;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->image = ebatEnemy;
	e->dur = 0x7FFF;
	e->movement = 1;	
	e->redraw = 1;
}

//**********Make_LynelBoss********************
//turns an entity into a Lynel boss
//inputs: e a pointer to the entity to make a Lynel
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_LynelBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x14;	e->dir = dir;
	e-> speed = 7;		e->speedCount = 7;
	e-> damage = 2;		e->phasing = 0;
	e-> coins = red;		e->shootDelay = 150;
	e->shootCount = 150; e->ammo = 0x16;
	e->dims[0] = 20; 	e->dims[1] = 20;
	e->image = lynelBoss;
	e->dur = 0x7FFF;
	e->movement = 0;
	e->redraw = 1;
}

//**********Make_SlimeBoss********************
//turns an entity into a Slime Boss
//inputs: e a pointer to the entity to make a Slime boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_slimeBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x20;	e->dir = dir;
	e-> speed = 12;		e->speedCount = 12;
	e-> damage = 1;		e->phasing = 0;
	e-> coins = red;		e->shootDelay = 0;
	e->shootCount = 0; e->ammo = 0;
	e->dims[0] = 20; 	e->dims[1] = 20;
	e->dur = 0x7FFF;	e->movement = 0x3;
	e->image = slimeBoss;
	e->redraw = 1;
}

//**********Make_orcBoss********************
//turns an entity into a Orc Boss
//inputs: e a pointer to the entity to make a Orc boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_orcBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x1A;	e->dir = dir;
	e-> speed = 9;		e->speedCount = 9;
	e-> damage = 2;		e->phasing = 0;
	e-> coins = red;		e->shootDelay = 255;
	e->shootCount = 255; e->ammo = 7;
	e->dims[0] = 20; 	e->dims[1] = 20;
	e->dur = 0x7FFF;
	e->image = orcBoss;
	e->movement = 1;
	e->redraw = 1;
}

//**********Make_icegolemBoss********************
//turns an entity into a Ice golem Boss
//inputs: e a pointer to the entity to make a Ice golem boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_icegolemBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x20;	e->dir = dir;
	e-> speed = 30;		e->speedCount = 30;
	e-> damage = 2;		e->phasing = 0;
	e-> coins = blue;		e->shootDelay = 70;
	e->shootCount = 70; e->ammo = 19;
	e->dims[0] = 20; 	e->dims[1] = 20;
	e->dur = 0x7FFF;
	e->image = icegolemBoss;
	e->movement = 3;
	e->redraw = 1;
}

//**********Make_Ganon1Boss********************
//turns an entity into a Ganon1 Boss
//inputs: e a pointer to the entity to make a Ganon1 boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Ganon1Boss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x1A;	e->dir = dir;
	e-> speed = 6;		e->speedCount = 6;
	e-> damage = 2;		e->phasing = 0;
	e->coins = 0;		e->shootDelay = 100;
	e->shootCount = 100; e->ammo = 0x15;
	e->dims[0] = 10; 	e->dims[1] = 10;
	e->dur = 0x7FFF;
	e->image = ganon1Boss;
	e->movement = 2;
	e->redraw = 1;
}

//**********Make_Ganon2Boss********************
//turns an entity into a Ganon2 Boss
//inputs: e a pointer to the entity to make a Ganon2 boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dir the direction the entity is facing
//outputs: none
void Make_Ganon2Boss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x; 	e->pos[1] = y;
	e-> health = 0x30;	e->dir = dir;
	e-> speed = 12;		e->speedCount = 12;
	e-> damage = 2;		e->phasing = 0;
	e-> coins = blue;		e->shootDelay = 60;
	e->shootCount = 60; e->ammo = 0x18;
	e->dims[0] = 20; 	e->dims[1] = 20;
	e->dur = 0x7FFF;
	e->image = ganon2boss;
	e->movement = 3;
	e->redraw = 1;
}

//DATABASE FOR OBSTACLES WILL BE HERE

//**********Make_WShieldObs*******************
//turns an entity into a shield obstacle
//inputs: e a pointer to the entity to make a wood shield obstacle
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_WShieldObs(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;	e->dir = dir;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = 0;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->dur = 20;
	if (dir == 0)	e->image = wshieldup;
	else if (dir == 1)	e->image = wshieldrt;
	else if (dir == 2)	e->image = wshielddn;
	else if (dir == 3)	e->image = wshieldlt;
	e->redraw = 1;
}

//**********Make_HShieldObs*******************
//turns an entity into a hyrulien shield obstacle
//inputs: e a pointer to the entity to make a hyrulien shield obstacle
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_HShieldObs(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;	e->dir = dir;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = 0;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->dur = 50;
	if (dir == 0)	e->image = hshieldup;
	else if (dir == 1)	e->image = hshieldrt;
	else if (dir == 2)	e->image = hshielddn;
	else if (dir == 3)	e->image = hshieldlt;
	e->redraw = 1;
}

//**********Make_Explode*******************
//turns an entity into an explosion obstacle
//inputs: e a pointer to the entity to make a n explosion
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Explode(entity_t *e, uint32_t x, uint32_t y, uint8_t dir){
	e->dir = dir;
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;	
	e->speed = 0;			e->speedCount = 0;
	e->damage = 2;		e->phasing = 0;
	e->coins = 0;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->dur = 10;
	e->image = explode; e->redraw = 1;
}

//**********Make_StairObstacle******************************
//turns an entity into a stair obstacle
//inputs: e a pointer to the entity to make a stair
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_StairObstacle(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = 0;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = stairs;
	e->redraw = 1;
}

//**********Make_BrownChest******************************
//turns an entity into a brown chest
//	position is always X,Y
//inputs: e a pointer to the entity to make a brown chest
//outputs: none
void Make_BrownChest(entity_t *e, uint32_t price){
	e->pos[0] = X;		e->pos[1] = Y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = price;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = brnchest;
	e->redraw = 1;
}

//**********Make_GreenChest******************************
//turns an entity into a Green chest
//	position is always X+80,Y
//inputs: e a pointer to the entity to make a Green chest
//outputs: none
void Make_GreenChest(entity_t *e, uint32_t price){
	uint8_t item = (Random() % numGrnItems) + 3;	//change this when adding items
	e->pos[0] = X+80;		e->pos[1] = Y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	int i;
	for (i = 0; i < numGrnItems; i++){
		if (Check_Item(item)){
			item = ((item + 1) % numGrnItems) + 3;		//change this when adding items
		}else break;
	}if (i == numGrnItems){
		e->damage = 0;	//no items left, so restore health
	}else{
		e->damage = item;		//sets the next random item
	}	
	e->phasing = 0;
	e->coins = price;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = grnchest;
	e->redraw = 1;
}

//**********Make_RedChest******************************
//turns an entity into a Red chest
//	position is always X,Y+80
//inputs: e a pointer to the entity to make a Red chest
//outputs: none
void Make_RedChest(entity_t *e, uint32_t price){	
	uint8_t item = (Random() % numRedItems) + 8;	//change this when adding items
	e->pos[0] = X;		e->pos[1] = Y+80;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	int i;
	for (i = 0; i < numRedItems; i++){
		if (Check_Item(item)){
			item = ((item + 1) % numRedItems) + 8;		//change this when adding items
		}else break;
	}if (i == numRedItems){
		e->damage = 0;
	}else{
		e->damage = item;		
	}
	e->phasing = 0;
	e->coins = price;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = redchest;
	e->redraw = 1;
}

//**********Make_BlueChest******************************
//turns an entity into a Blue chest
//	position is always X+80,Y+80
//inputs: e a pointer to the entity to make a Blue chest
//outputs: none
void Make_BlueChest(entity_t *e, uint32_t price){	
	uint8_t item = (Random() % numBluItems) + 13;	//change this when adding items
	e->pos[0] = X+80;		e->pos[1] = Y+80;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	int i;
	for (i = 0; i < numBluItems; i++){
		if (Check_Item(item)){
			item = ((item + 1) % numBluItems) + 13;		//change this when adding items
		}else break;
	}if (i == numBluItems){
		e->damage = 0;
	}else{
		e->damage = item;		
	}
	e->phasing = 0;
	e->coins = price;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = bluchest;
	e->redraw = 1;
}

//**********Make_Brick******************************
//turns an entity into a brick obstacle
//inputs: e a pointer to the entity to make a brick
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Brick(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = brickObstacle;
	e->redraw = 1;
}

//**********Make_BrickArrows******************************
//turns an entity into a brick obstacle that shoots arrows out
//inputs: e a pointer to the entity to make a brick
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_BrickArrows(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x30;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 100;
	e->shootCount = 100;	e->ammo = 0x11;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = brickObstacle;
	e->redraw = 1;
}

//**********Make_Bush******************************
//turns an entity into a bush obstacle
//inputs: e a pointer to the entity to make a bush
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Bush(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = bushObstacle;
	e->redraw = 1;
}

//**********Make_Water******************************
//turns an entity into a water obstacle
//inputs: e a pointer to the entity to make a water pool
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Water(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = waterObstacle;
	e->redraw = 1;
}

//**********Make_Ice******************************
//turns an entity into a ice obstacle
//inputs: e a pointer to the entity to make a ice block
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Ice(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = iceObstacle;
	e->redraw = 1;
}

//**********Make_Lava******************************
//turns an entity into a lava obstacle
//inputs: e a pointer to the entity to make a lava pool
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Lava(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 0;		e->phasing = 0;
	e->coins = green;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = lavaObstacle;
	e->redraw = 1;
}

//**********Make_Shopkeeper******************************
//turns an entity into a shopkeeper obstacle
//inputs: e a pointer to the entity to make a shopkeeper
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Shopkeeper(entity_t *e, uint32_t x, uint32_t y){
	e->pos[0] = x;		e->pos[1] = y;
	e->health = 0x7FFFFFF;
	e->speed = 0;			e->speedCount = 0;
	e->damage = 17;		e->phasing = 0;
	e->coins = 0;			e->shootDelay = 0;
	e->shootCount = 0;	e->ammo = 0;
	e->dur = 0x7FFF;
	e->dims[0] = 10;	e->dims[1] = 10;
	e->image = shopkeepObstacle;
	e->redraw = 1;
}



