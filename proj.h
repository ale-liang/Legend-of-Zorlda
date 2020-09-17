//proj.h

#ifndef PROJ_H
#define PROJ_H

#include <stdint.h>
#include <stdlib.h>
#include "images.h"
#include "entity.h"

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
void Make_Proj(entity_t *e, uint8_t direction);

//**********Make_Proj_Player********************
//changes the bottom left coords and its direction for a specified entity
//	used for initializing the position, direction, dimensions, image
//	speed, speedCount, damage, phasing, and redraw
//	PROJECTILE SIZE MUST BE LESS THAN OR EQUAL TO MOTHER SIZE
//	CHECK FOR OVERLAP BEFORE CREATING
//inputs: z a pointer to the projectile to change values
//				e a pointer to the entity/player that spawned the projectile
//				direction the direction to fire the arrow
//outputs: none
void Make_Proj_Player(player_t *e, uint8_t direction);

//Moves a projectile on the grid
//	unsuccessful if the projectile can no longer move forward
//inputs: p the pointer to the projectile
//outputs: 1 if the move was successful
//				 2 if the move was unsuccessful
int Move_Proj(proj_t *p, stage_t *stage);

//**********Explode_Bomb********************
//explodes the bomb, damaging all nearby enemies
//inputs: p the bomb exploding
//outputs: none
void Explode_Bomb(proj_t *p);

#endif

