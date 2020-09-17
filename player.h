//player.h

#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>
#include <stdlib.h>
#include "zStructs.h"
#include "images.h"
#include "stage.h"

//**********Make_Lonk******************************
//turns a player_t into a lonk
//inputs: lonk a pointer to a player_t
//outputs: none
void Make_Lonk(player_t *lonk);

//**********Make_Yerraballi******************************
//turns a player_t into a yerraballi
//inputs: yerr a pointer to a player_t
//outputs: none
void Make_Yerraballi(player_t *yerr);

//**********Make_Valvano******************************
//turns a player_t into a val
//inputs: val a pointer to a player_t
//outputs: none
void Make_Valvano(player_t *val);

//**********Switch_Item********************
//switches the current item to the next item in the array
//inputs: none
//outputs: none
void Switch_Item(player_t *lonk);

//**********Check_Item*********************
//Checks to see if an item is currently in player's inventory
//	used for initializing shop
//inputs: num the item number to check
//outputs: 1 if item is in the inventory
//				 0 if not
uint8_t Check_Item(uint8_t num);

//**********Check_Rupoos********************
//Checks to see if the player is currently collecting a rupoo
//inputs: lonk a pointer to the player
//outputs: none
void Check_Rupoos(player_t *lonk);

//**********Check_Damage********************
//Checks to see if the player is currently being damaged
//	Player gains invincibility for a short time after being damaged
//inputs: none
//outputs: none
void Check_Damage(player_t *lonk);
	
//**********Check_Move_Player********************
//Checks to see if the new image can be drawn without overriding an obstacle or wall
//inputs: x the x coord of the bottom left corner 
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Move_Player(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY);

//**********Player_Move_Up********************
//moves the player up if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Up(player_t *p);

//**********Player_Move_Right********************
//moves the player right if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Right(player_t *p);

//**********Player_Move_Down********************
//moves the player down if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Down(player_t *p);

//**********Player_Move_Left********************
//moves the player left if it is able to
//inputs: p a pointer to the player to move
//outputs: none
void Player_Move_Left(player_t *p);


#endif
