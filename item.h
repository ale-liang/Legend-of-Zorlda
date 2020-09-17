//item.h
#include "zstructs.h"

//**********Use_ShortSword********************
//makes the player swing a shortsword
//	deals 1 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 1
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Shortsword(player_t *p);

//**********Use_WoodShield********************
//uses a shield to block projectiles and enemies
//	Item ID: 2
//inputs: p a pointer to the player using the item
//outputs: none
void Use_WoodShield(player_t *p);

//**********Use_Bow********************
//shoots an arrow from link's bow
//	deals 1 damage if it hits 
//	deals 0 damage if it hits an obstacle
//	Item ID: 3
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bow(player_t *p);

//**********Use_Dekunut********************
//fires a dekunut that stuns any enemy it hits
//	deals 1 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 4
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Dekunuts(player_t *p);

//**********Use_Needles********************
//fires a needle
//	deals 2 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 5		Ammo: 7
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Needles(player_t *p);

//**********Use_Bomb********************
//fires a bomb that explodes, damaging nearby enemies
//	deals 2 damage if it hits an enemy
//	breaks any adjacent obstacles
//	Item ID: 6		Ammo: 3
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bomb(player_t *p);

//**********Use_BGSword********************
//makes the player swing the biggoron sword
//	deals 3 damage if it hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 7
//inputs: p a pointer to the player using the sword
//outputs: none
void Use_BGSword(player_t *p);

//**********Use_Bombchu********************
//fires a bomb that explodes, damaging nearby enemies
//faster than the normal bomb
//	deals 2 damage if it hits an enemy
//	breaks any adjacent obstacles
//	Item ID: 9		Ammo: 4
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Bombchu(player_t *p);

//**********Use_IceRod********************
//freezes all enemies on the screen temporarily
//	deals 1 damage
//	Item ID: 10	
//inputs: p a pointer to the player using the item
//outputs: none
void Use_IceRod(player_t *p);

//**********Use_Quake********************
//shakes the screen immobilizing and damaging all enemies
//	deals 2 damage
//	Item ID: 11	
//inputs: p a pointer to the player using the item
//outputs: none
void Use_Quake(player_t *p);

//**********Use_FireRod********************
//fires a fireball in all 4 directions
//	deals 5 damage if it hits an enemy
//	Item ID: 13		Ammo: 8
//inputs: p a pointer to the player using the item
//outputs: none
void Use_FireRod(player_t *p);

//**********Use_LightBow********************
//shoots 2 arrows from zorlda's lightbow
//	deals 3 damage ifor each hit
//	deals 0 damage if it hits an obstacle
//	Item ID: 14		Ammo: 9
//inputs: p a pointer to the player using the item
//outputs: none
void Use_LightBow(player_t *p);

//**********Use_MSword********************
//makes the player swing the master sword
//shoots out a wave projectile as well
//	deals 3 damage if it hits an enemy
//	deals 3 damage if the wave hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 15 		Ammo: 5
//inputs: p a pointer to the player using the sword
//outputs: none
void Use_MSword(player_t *p);

//**********Use_HShield********************
//uses a shield to block projectiles and enemies
//	creates a shield in all 4 directions
//	Item ID: 12
//inputs: p a pointer to the player using the item
//outputs: none
void Use_HShield(player_t *p);

//**********Use_Mjolnir********************
//makes the player swing mjolnir
//shoots out a bolt projectile as well
//	deals 4 damage if it hits an enemy
//	deals 2 damage if the bolt hits an enemy
//	deals 0 damage if it hits an obstacle
//	Item ID: 16 		Ammo: 6
//inputs: p a pointer to the player using the weapon
//outputs: none
void Use_Mjolnir(player_t *p);
