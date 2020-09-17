//entity.h

#ifndef ENTITY_H
#define ENTITY_H
#include <stdint.h>
#include <stdlib.h>
#include "zStructs.h"
#include "player.h"

//**********Check_Overlap********************
//Checks if the given variables currently overlap with an entity
//inputs:	x the x coord of the bottom left corner of the image
//				y the y coord of the bottom left corner of the image
//				dimX the width of the image
//				dimY the height of the image
//				e the entity to compare overlap with
//outputs: 1 if there is no overlap
//				 0 if there is an overlap
int Check_Overlap(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY, entity_t *e);

//**********Check_Adjacent********************
//Checks if the given player is adjacent to a specific obstacle
//inputs: p the pointer to the player
//				e the pointer to the obstacle
//output: 1 if they are adjacent
//				0 if not
uint8_t Check_Adjacent(player_t *p, entity_t *e);

//**********Move_Enemy_Random********************
//Moves an enemy in a straight line until it hits an obstruction
//	if obstructed, randomly generates a different direction to travel
//inputs: e a pointer to the enemy to move
//outputs: none
void Move_Enemy_Random(entity_t *e);

//**********Find_Conflict********************
//Returns the obstacles that the enemy is colliding with
//inputs: e a pointer to the enemy
//outputs: o a pointer to the obstacle
entity_t* Find_Conflict(entity_t *e);

//**********Move_Enemy_Proj********************
//Moves an enemy towards the player, but maintains a distance to shoot
//	will move randomly for some time if it cannot move towards
//inputs: e a pointer to the enemy to move
//				p1 a pointer to the first player
//				p2 a pointer to the second player
//outputs: none
void Move_Enemy_Proj(entity_t *e, struct player *p1);

//**********Move_Enemy_Homing********************
//Moves an enemy towards the player
//	will move randomly for some time if it cannot move towards
//inputs: e a pointer to the enemy to move
//				p1 a pointer to the first player
//				p2 a pointer to the second player
//outputs: none
void Move_Enemy_Homing(entity_t *e, player_t *p1);

//**********Teleport_Enemy*********************
//Teleports the enemy to a random spot
//	moves for a short duration after
//inputs: e a pointer to the enemy to move
//outputs: none
void Teleport_Enemy(entity_t *e);

//**********Move_Enemy*********************
//Moves an enemy according to the movement type
//inputs: e a pointer to the enemy to move
//outputs: none
void Move_Enemy(entity_t *e);

//**********Omni_Fire********************
//Fires a projectile in all four directions from enemy
//	unsuccessful if unable to fire in any direction
//inputs: e a pointer to an entity to fire from
//outputs: 1 if one or more projectiles were fired
//				 0 if no projectiles fired
int Omni_Fire(entity_t *e);

//Fires a projectile from an entity if able to
//	unsuccessful if player is not in line of sight
//	unsuccessful if an obstacle is in the way
//inputs: e a pointer to an entity to fire from
//outputs: 1 if the shoot is successful
//				 0 if the shoot is unsuccessful
int Entity_Fire(entity_t *e, player_t *p1);


//DATABASE FOR ENEMIES WILL BE HERE

//**********Make_Slime********************
//turns an entity into a slime enemy
//inputs: e a pointer to the entity to make a slime
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Slime(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Skeleton********************
//turns an entity into a skeleton enemy
//inputs: e a pointer to the entity to make a skeleton
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Skeleton(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Snake********************
//turns an entity into a snake enemy
//inputs: e a pointer to the entity to make a snake
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Snake(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Squid********************
//turns an entity into a squid enemy
//inputs: e a pointer to the entity to make a squid
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Squid(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Crab********************
//turns an entity into a crab enemy
//inputs: e a pointer to the entity to make a squid
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Crab(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Bear********************
//turns an entity into a crab enemy
//inputs: e a pointer to the entity to make a squid
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Bear(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Snowman********************
//turns an entity into a Snowman enemy
//inputs: e a pointer to the entity to make a snowman
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Snowman(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Spider********************
//turns an entity into a spider enemy
//inputs: e a pointer to the entity to make a spider
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Spider(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Skelebomber********************
//turns an entity into a skeleton bomber enemy
//inputs: e a pointer to the entity to make a skeleton bomber enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Skelebomber(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Dekuscrub********************
//turns an entity into a Deku Scrub enemy
//inputs: e a pointer to the entity to make a Deku scrub enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Dekuscrub(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Fbow********************
//turns an entity into a Flying bow enemy
//inputs: e a pointer to the entity to make a Flying bow enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Fbow(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Ebat********************
//turns an entity into a electric bat enemy
//inputs: e a pointer to the entity to make a electric bat enemy
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Ebat(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_LynelBoss********************
//turns an entity into a Lynel boss
//inputs: e a pointer to the entity to make a Lynel
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_LynelBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);


//**********Make_slimeBoss********************
//turns an entity into a Slime Boss
//inputs: e a pointer to the entity to make a Slime boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_slimeBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_orcBoss********************
//turns an entity into a Orc Boss
//inputs: e a pointer to the entity to make a Orc boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_orcBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Ganon1Boss********************
//turns an entity into a Ganon1 Boss
//inputs: e a pointer to the entity to make a Ganon1 boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Ganon1Boss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_icegolemBoss********************
//turns an entity into a Ice golem Boss
//inputs: e a pointer to the entity to make a Ice golem boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_icegolemBoss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Ganon2Boss********************
//turns an entity into a Ganon2 Boss
//inputs: e a pointer to the entity to make a Ganon2 boss
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Ganon2Boss(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);
	
//DATABASE FOR OBSTACLES WILL BE HERE

//**********Make_WShieldObs*******************
//turns an entity into a shield obstacle
//inputs: e a pointer to the entity to make a wood shield obstacle
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_WShieldObs(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_HShieldObs*******************
//turns an entity into a hyrulien shield obstacle
//inputs: e a pointer to the entity to make a hyrulien shield obstacle
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_HShieldObs(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_Explode*******************
//turns an entity into an explosion obstacle
//inputs: e a pointer to the entity to make a n explosion
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Explode(entity_t *e, uint32_t x, uint32_t y, uint8_t dir);

//**********Make_StairObstacle******************************
//turns an entity into a stair obstacle
//inputs: e a pointer to the entity to make a stair
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_StairObstacle(entity_t *e, uint32_t x, uint32_t y);

//**********Make_BrownChest******************************
//turns an entity into a brown chest
//	position is always X,Y
//inputs: e a pointer to the entity to make a brown chest
//outputs: none
void Make_BrownChest(entity_t *e, uint32_t cost);

//**********Make_GreenChest******************************
//turns an entity into a Green chest
//	position is always X+80,Y
//inputs: e a pointer to the entity to make a Green chest
//outputs: none
void Make_GreenChest(entity_t *e, uint32_t price);

//**********Make_RedChest******************************
//turns an entity into a Red chest
//	position is always X,Y
//inputs: e a pointer to the entity to make a Red chest
//outputs: none
void Make_RedChest(entity_t *e, uint32_t price);

//**********Make_BlueChest******************************
//turns an entity into a Blue chest
//	position is always X+80,Y+80
//inputs: e a pointer to the entity to make a Blue chest
//outputs: none
void Make_BlueChest(entity_t *e, uint32_t price);

//**********Make_Brick******************************
//turns an entity into a brick obstacle
//inputs: e a pointer to the entity to make a brick
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Brick(entity_t *e, uint32_t x, uint32_t y);

//**********Make_BrickArrows******************************
//turns an entity into a brick obstacle that shoots arrows out in all 4 directions
//inputs: e a pointer to the entity to make a brick
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_BrickArrows(entity_t *e, uint32_t x, uint32_t y);

//**********Make_Bush******************************
//turns an entity into a bush obstacle
//inputs: e a pointer to the entity to make a bush
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Bush(entity_t *e, uint32_t x, uint32_t y);
	
//**********Make_Water******************************
//turns an entity into a water obstacle
//inputs: e a pointer to the entity to make a water pool
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Water(entity_t *e, uint32_t x, uint32_t y);

//**********Make_Ice******************************
//turns an entity into a ice obstacle
//inputs: e a pointer to the entity to make a ice block
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Ice(entity_t *e, uint32_t x, uint32_t y);
	
//**********Make_Lava******************************
//turns an entity into a lava obstacle
//inputs: e a pointer to the entity to make a lava pool
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Lava(entity_t *e, uint32_t x, uint32_t y);

//**********Make_Shopkeeper******************************
//turns an entity into a shopkeeper obstacle
//inputs: e a pointer to the entity to make a shopkeeper
//				x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//outputs: none
void Make_Shopkeeper(entity_t *e, uint32_t x, uint32_t y);

#endif
