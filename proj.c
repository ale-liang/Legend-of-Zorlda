//proj.c

#include <stdint.h>
#include <stdlib.h>
#include "zstructs.h"
#include "entity.h"
#include "ST7735.h"

extern entity_t *zPtr;
extern player_t lonk;
extern uint8_t borders;
extern uint8_t redrawStats;
extern uint8_t invertFlag;
//A single projectile uninitialized
proj_t pZygote;

//the pointer to the unmodified projectile
proj_t *pzPtr = &pZygote;

//**********Make_Proj********************
//changes the bottom left coords and its direction for a specified entity
//	used for initializing the position, direction, dimensions, image
//	speed, speedCount, damage, phasing, and redraw
//	PROJECTILE SIZE MUST BE LESS THAN OR EQUAL TO MOTHER SIZE
//	CHECK FOR OVERLAP BEFORE CREATING
//inputs: z a pointer to the projectile to change values
//				e a pointer to the entity/player that spawned the projectile
//				direction the direction to fire the arrow
//outputs: none
void Make_Proj(entity_t *e, uint8_t direction){
	int i = 0;
	pzPtr->friendly = 0;
	pzPtr->dir = direction;
	pzPtr->exists = 1;
	if (direction == up){
		pzPtr->pos[0] = e->pos[0] + (e->dims[0] - pzPtr->dims[0])/2;
		pzPtr->pos[1] = e->pos[1] - e->dims[1];
		if ((e->ammo & 0xF) == arrowAmmo){
			pzPtr->image = arrowup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if ((e->ammo & 0xF) == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = waveup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needleup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}
	}else if (direction == rt){
		pzPtr->pos[0] = e->pos[0] + e->dims[0];
		pzPtr->pos[1] = e->pos[1] - (e->dims[1] - pzPtr->dims[1])/2;
		if ((e->ammo & 0xF) == arrowAmmo){
			pzPtr->image = arrowright;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if ((e->ammo & 0xF) == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if ((e->ammo & 0xF) == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavert;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltrt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballrt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightrt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}
	}else if (direction == dn){
		pzPtr->pos[0] = e->pos[0] + (e->dims[0] - pzPtr->dims[0])/2;
		pzPtr->pos[1] = e->pos[1] + pzPtr->dims[1];
		if ((e->ammo & 0xF) == arrowAmmo){
			pzPtr->image = arrowdown;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if ((e->ammo & 0xF) == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavedn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltdn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needledn;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballdn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightdn;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}
	}else if (direction == lt){
		pzPtr->pos[0] = e->pos[0] - pzPtr->dims[0];
		pzPtr->pos[1] = e->pos[1] - (e->dims[1] - pzPtr->dims[1])/2;
		if ((e->ammo & 0xF) == arrowAmmo){
			pzPtr->image = arrowleft;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if ((e->ammo & 0xF) == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if ((e->ammo & 0xF) == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavelt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltlt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needlelt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballlt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightlt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}
	}if ((e->ammo & 0xF) == arrowAmmo){
		pzPtr->ammo = arrowAmmo;
		pzPtr->speed = 1;	pzPtr->speedCount = 1;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == dekunutAmmo){
		pzPtr->ammo = dekunutAmmo;
		pzPtr->speed = 2;	pzPtr->speedCount = 2;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == bombAmmo){
		pzPtr->ammo = bombAmmo;
		//a speed that uses the 16 MSB means the proj stops and waits to blow up
		pzPtr->speed = 0x3C280002;	pzPtr->speedCount = 2;
		pzPtr->damage = 2;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->ammo = waveAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 2;	pzPtr->phasing = 1;
	}else if ((e->ammo & 0xF) == boltAmmo){
		pzPtr->ammo = boltAmmo;
		pzPtr->speed = 1;	pzPtr->speedCount = 1;
		pzPtr->damage = 3;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == needleAmmo){
		pzPtr->ammo = needleAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == fireAmmo){
		pzPtr->ammo = fireAmmo;
		pzPtr->speed = 3;	pzPtr->speedCount = 3;
		pzPtr->damage = 4;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == lightAmmo){
		pzPtr->ammo = lightAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 2;	pzPtr->phasing = 1;
	}
	pzPtr->redraw = 1;
	while (currentStage->pA[i].exists == 1) i++;		//find the next open spot
	currentStage->pA[i] = *pzPtr;
	currentStage->pSize++;
}

//**********Make_Proj_Player********************
//changes the bottom left coords and its direction for a specified entity
//	used for initializing the position, direction, dimensions, image
//	speed, speedCount, damage, phasing, and redraw
//	PROJECTILE SIZE MUST BE LESS THAN OR EQUAL TO MOTHER SIZE
//	CHECK FOR OVERLAP BEFORE CREATING
//	Only difference from Make_Proj is the proj is friendly
//inputs: z a pointer to the projectile to change values
//				e a pointer to the entity/player that spawned the projectile
//				direction the direction to fire the arrow
//outputs: none
void Make_Proj_Player(player_t *e, uint8_t direction){
	int i = 0;	//variable for loops
	pzPtr->friendly = 1;	//projectile will damage enemies, but not player
	pzPtr->dir = direction;	//set direction
	pzPtr->exists = 1;	//the projectile exists
	//determine the direction of the projectile and initialize its position,
	//	dimensions, and image from the type of projectile to make
	if (direction == up){
		pzPtr->pos[0] = e->pos[0] + (e->dims[0] - pzPtr->dims[0])/2;
		pzPtr->pos[1] = e->pos[1] - e->dims[1];
		if (e->ammo == arrowAmmo){
			pzPtr->image = arrowup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if (e->ammo == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if (e->ammo == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if (e->ammo == bombchuAmmo){
			pzPtr->image = bombchuup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = waveup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needleup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballup;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightup;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}
	}else if (direction == rt){
		pzPtr->pos[0] = e->pos[0] + e->dims[0];
		pzPtr->pos[1] = e->pos[1] - (e->dims[1] - pzPtr->dims[1])/2;
		if (e->ammo == arrowAmmo){
			pzPtr->image = arrowright;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if (e->ammo == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if (e->ammo == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if (e->ammo == bombchuAmmo){
			pzPtr->image = bombchurt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavert;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltrt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needlert;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballrt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightrt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}
	}else if (direction == dn){
		pzPtr->pos[0] = e->pos[0] + (e->dims[0] - pzPtr->dims[0])/2;
		pzPtr->pos[1] = e->pos[1] + pzPtr->dims[1];
		if (e->ammo == arrowAmmo){
			pzPtr->image = arrowdown;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if (e->ammo == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if (e->ammo == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if (e->ammo == bombchuAmmo){
			pzPtr->image = bombchudn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavedn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltdn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needledn;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballdn;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightdn;
			pzPtr->dims[0] = 3;
			pzPtr->dims[1] = 9;
		}
	}else if (direction == lt){
		pzPtr->pos[0] = e->pos[0] - pzPtr->dims[0];
		pzPtr->pos[1] = e->pos[1] - (e->dims[1] - pzPtr->dims[1])/2;
		if (e->ammo == arrowAmmo){
			pzPtr->image = arrowleft;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if (e->ammo == dekunutAmmo){ 
			pzPtr->image = dekunut;
			pzPtr->dims[0] = 8;
			pzPtr->dims[1] = 8;
		}else if (e->ammo == bombAmmo){
			pzPtr->image = bombProj;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if (e->ammo == bombchuAmmo){
			pzPtr->image = bombchult;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == waveAmmo){
			pzPtr->image = wavelt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == boltAmmo){
			pzPtr->image = boltlt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == needleAmmo){
			pzPtr->image = needlelt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}else if ((e->ammo & 0xF) == fireAmmo){
			pzPtr->image = fireballlt;
			pzPtr->dims[0] = 10;
			pzPtr->dims[1] = 10;
		}else if ((e->ammo & 0xF) == lightAmmo){
			pzPtr->image = lightlt;
			pzPtr->dims[0] = 9;
			pzPtr->dims[1] = 3;
		}
	}//initialize final characteristics based off the type of proj
	if (e->ammo == arrowAmmo){
		pzPtr->ammo = arrowAmmo;
		pzPtr->speed = 1;	pzPtr->speedCount = 1;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if (e->ammo == dekunutAmmo){
		pzPtr->ammo = dekunutAmmo;
		pzPtr->speed = 2;	pzPtr->speedCount = 2;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if (e->ammo == bombAmmo){
		pzPtr->ammo = bombAmmo;
		//a speed that uses the 16 MSB means the proj stops and waits to blow up
		pzPtr->speed = 0x3C1E0003;	pzPtr->speedCount = 3;
		pzPtr->damage = 2;	pzPtr->phasing = 0;
	}else if (e->ammo == bombchuAmmo){
		pzPtr->ammo = bombchuAmmo;
		//a speed that uses the 16 MSB means the proj stops and waits to blow up
		pzPtr->speed = 0x50500000;	pzPtr->speedCount = 0;
		pzPtr->damage = 2;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == waveAmmo){
		pzPtr->ammo = waveAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 3;	pzPtr->phasing = 1;
	}else if ((e->ammo & 0xF) == boltAmmo){
		pzPtr->ammo = boltAmmo;
		pzPtr->speed = 1;	pzPtr->speedCount = 1;
		pzPtr->damage = 4;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == needleAmmo){
		pzPtr->ammo = needleAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 1;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == fireAmmo){
		pzPtr->ammo = fireAmmo;
		pzPtr->speed = 4;	pzPtr->speedCount = 4;
		pzPtr->damage = 5;	pzPtr->phasing = 0;
	}else if ((e->ammo & 0xF) == lightAmmo){
		pzPtr->ammo = lightAmmo;
		pzPtr->speed = 0;	pzPtr->speedCount = 0;
		pzPtr->damage = 3;	pzPtr->phasing = 1;
	}
	pzPtr->redraw = 1;
	while (currentStage->pA[i].exists == 1) i++;		//find the next open spot
	currentStage->pA[i] = *pzPtr;	//add the new proj to the array
	currentStage->pSize++;	//update the size of the proj array
}



//Moves a projectile on the grid
//	unsuccessful if the projectile can no longer move forward (hits enemy or obstacle)
//	damage done to player is not dealt in this method
//inputs: p the pointer to the projectile
//outputs: 1 if the move was successful
//				 0 if the move was unsuccessful
//				 0 if the proj struck an enemy
int Move_Proj(proj_t *p, stage_t *stage){
	//if the proj is friendly, see if it hits an enemy
	if (p->friendly && !(currentStage->cleared)){
		//iterate through the array of enemies
		for (int i = 0;i<currentStage->totalEnemies;i++){
			entity_t *e = &(currentStage->enemies[i]);	//grab entity for ease of access
			if (e->health > 0){		//make sure the enemy isnt already killed
				//if the projectile is a bomb, will not directly harm enemy, it will just stop
				if (p->ammo == bombAmmo || p->ammo == bombchuAmmo){	
					if ((p->pos[0] - e->pos[0]) < e->dims[0] || (e->pos[0] - p->pos[0]) < p->dims[0]){
						if ((p->pos[1] - e->pos[1]) < p->dims[1] || (e->pos[1] - p->pos[1]) < e->dims[1]){
							return 0;	//return 0 if the bomb touches an enemy
						}
					}
				}//if it isnt a bomb, check for a hit, then decrease enemy health
				else if ((p->pos[0] - e->pos[0]) < e->dims[0] || (e->pos[0] - p->pos[0]) < p->dims[0]){
					if ((p->pos[1] - e->pos[1]) < p->dims[1] || (e->pos[1] - p->pos[1]) < e->dims[1]){
						e->health -= p->damage;	//decrease enemy health
						//if the projectile is a dekunut, stops the enemy from movign shortly
						if (p->ammo == dekunutAmmo || p->ammo == boltAmmo){
							e->speedCount = 90;	//on floor 1, stopped for 3 seconds
						}e->redraw = 1;	//redrawStats the enemy after being struck
						return 0;	//return an unsuccessful move, so the proj is destroyed
					}
				}
			}
		}
	}Redraw_All(&lonk);
	if (p->phasing == 1){
		if (p->dir == up){		//check if the proj is facing up
			if (Check_Walls(p->pos[0],(p->pos[1])-1,p->dims[0],p->dims[1])){		//see if the proj can move up 1 pixel
				p->pos[1]--;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == rt){	//check if the proj is facing right
			if (Check_Walls(p->pos[0]+1,p->pos[1],p->dims[0],p->dims[1])){			//see if the proj can move right 1 pixel
				p->pos[0]++;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == dn){	//check if the proj is facing down
			if (Check_Walls(p->pos[0],p->pos[1]+1,p->dims[0],p->dims[1])){			//see if the proj can move down 1 pixel
				p->pos[1]++;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == lt){	//check if the proj is facing left
			if (Check_Walls(p->pos[0]-1,p->pos[1],p->dims[0],p->dims[1])){			//see if the proj can move left 1 pixel
				p->pos[0]--;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}
	}else{
		//determine the direction of the proj and see if it can move once more
		if (p->dir == up){		//check if the proj is facing up
			if (Check_Move(p->pos[0],(p->pos[1])-1,p->dims[0],p->dims[1])){		//see if the proj can move up 1 pixel
				p->pos[1]--;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == rt){	//check if the proj is facing right
			if (Check_Move(p->pos[0]+1,p->pos[1],p->dims[0],p->dims[1])){			//see if the proj can move right 1 pixel
				p->pos[0]++;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == dn){	//check if the proj is facing down
			if (Check_Move(p->pos[0],p->pos[1]+1,p->dims[0],p->dims[1])){			//see if the proj can move down 1 pixel
				p->pos[1]++;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}else if (p->dir == lt){	//check if the proj is facing left
			if (Check_Move(p->pos[0]-1,p->pos[1],p->dims[0],p->dims[1])){			//see if the proj can move left 1 pixel
				p->pos[0]--;		//if it can move, move it
				p->redraw = 1;	//redraw it
				return 1;				//return successful
			}
		}
	}return 0;	//return unsuccessful
}

//**********Explode_Bomb********************
//explodes the bomb, damaging all nearby enemies
//inputs: p the bomb exploding
//outputs: none
void Explode_Bomb(proj_t *p){
	int i = 0; int count = 0;		//variables for looping
	entity_t * e;	//pointer to an entity for ease of grabbing
	//only check enemies if the stage isnt cleared yet
	if (currentStage->cleared == 0){
		for (i = 0;i<currentStage->totalEnemies;i++){	//iterate through the enemies
			e = &(currentStage->enemies[i]);	//grab for ease
			if (e->health > 0){		//dont need to check if enemy is already dead
				//Check overlap with all 5 squares of explosion. Can only be hit once
				if (!Check_Overlap(p->pos[0],p->pos[1],10,10,e)){	
					e->health -= p->damage;
					e->redraw = 1;
				}else if (!Check_Overlap(p->pos[0],p->pos[1]-10,10,10,e)){
					e->health -= p->damage;
					e->redraw = 1;
				}else if (!Check_Overlap(p->pos[0]+10,p->pos[1],10,10,e)){
					e->health -= p->damage;
					e->redraw = 1;
				}else if (!Check_Overlap(p->pos[0],p->pos[1]+10,10,10,e)){
					e->health -= p->damage;
					e->redraw = 1;
				}else if (!Check_Overlap(p->pos[0]-10,p->pos[1],10,10,e)){
					e->health -= p->damage;
					e->redraw = 1;
				}
			}
		}i = 0; //reset i variable
	}
	//can only break obstacles in battle stages, not shop or stairs
	//loop until all obstacles checked or exceeding maximum
	while (count < currentStage->totalObstacles && i < 28){
		//only check if obstacle isn't broken
		if (currentStage->obstacles[i].dur > 0){
			count++;	//increase count because we found an obstacle
			e = &(currentStage->obstacles[i]);	//pointer for ease of access
			//check overlap with all 5 squares of explosion
			//	if hit, break the obstacle and redraw
			if (!Check_Overlap(p->pos[0],p->pos[1],10,10,e)){
				if (currentStage->type == 0) e->health = 0;
				e->redraw = 1;
			}else if (!Check_Overlap(p->pos[0],p->pos[1]-10,10,10,e)){
				if (currentStage->type == 0) e->health = 0;
				e->redraw = 1;
			}else if (!Check_Overlap(p->pos[0]+10,p->pos[1],10,10,e)){
				if (currentStage->type == 0) e->health = 0;
				e->redraw = 1;
			}else if (!Check_Overlap(p->pos[0],p->pos[1]+10,10,10,e)){
				if (currentStage->type == 0) e->health = 0;
				e->redraw = 1;
			}else if (!Check_Overlap(p->pos[0]-10,p->pos[1],10,10,e)){
				if (currentStage->type == 0) e->health = 0;
				e->redraw = 1;
			}
		}i++;		//increment i every time
	}i = count = 0;	//reset counter variables
	//Create 5 explosion obstacles (one in each direction and one where bomb was
	//	add each one to the array of obstacles
	//	explosion created regardless of overlap
	Make_Explode(zPtr,p->pos[0],p->pos[1],up);		//create the explosion
	//iterate through the array until an empty obstacle is found
	while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
			i++;
	}currentStage->obstacles[i] = *zPtr;	//add the new obstacle at the found index
	currentStage->totalObstacles++;				//increment the size
	//and repeat 4 other times!
	i = 0;
	Make_Explode(zPtr,p->pos[0],p->pos[1]-10,up);
	while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
			i++;
	}currentStage->obstacles[i] = *zPtr;
	currentStage->totalObstacles++;
	i = 0;
	Make_Explode(zPtr,p->pos[0]+10,p->pos[1],rt);
	while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
			i++;
	}currentStage->obstacles[i] = *zPtr;
	currentStage->totalObstacles++;
	i = 0;
	Make_Explode(zPtr,p->pos[0],p->pos[1]+10,dn);
	while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
			i++;
	}currentStage->obstacles[i] = *zPtr;
	currentStage->totalObstacles++;
	i = 0;
	Make_Explode(zPtr,p->pos[0]-10,p->pos[1],lt);
	while (currentStage->obstacles[i].dur > 0 ||
				(currentStage->obstacles[i].dur == 0 && i < currentStage->totalObstacles)){
			i++;
	}currentStage->obstacles[i] = *zPtr;
	currentStage->totalObstacles++;
}

