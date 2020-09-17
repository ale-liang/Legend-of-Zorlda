//zStructs.h
//zStructs contains all of the structs and definitions

#ifndef ZSTRUCTS_H
#define ZSTRUCTS_H
#include <stdint.h>
#include <stdlib.h>
#define X 19 //start of the x position in grid
#define Y 69 //start of the y position in grid
#define green 5
#define red 10
#define blue 50

#define NSexitX	59
#define	EWexitY	109
#define	NexitY	59
#define EexitX	109
#define SexitY 	159
#define WexitX	9
//#defines for ammo types
#define arrowAmmo 1
#define dekunutAmmo 2
#define bombAmmo 3
#define bombchuAmmo 4
#define waveAmmo 5
#define boltAmmo 6
#define needleAmmo 7
#define fireAmmo 8
#define lightAmmo 9

#define battleStage 0
#define shopStage 1
#define stairsStage 2

#define numGrnItems 5
#define numRedItems 5
#define numBluItems 4
#define pAsize 16
#define rQsize 16
#define up 0
#define rt 1
#define dn 2
#define lt 3

// An enemy, boss, or obstacle
typedef struct entity{
	//the current position of the entity {x,y}
	// 	x: [19,99]		y: [69,149]
	uint32_t pos[2];
	
	//the direction the entity is facing
	//	bit 0 for up, bit 1 for right, bit 2 for down, bit 3 for left
	uint8_t dir;
	
	//the duration the entity exists on the grid
	//	0x7FFF if infinite
	int16_t dur;
	
	//the current health of the entity
	//	0 if the entity is dead/destroyed
	//	0xFFFFFFFF if the entity cannot by destroyed
	int32_t health;
	
	//the speed that the enemy travels
	//	1 means 30 pixels/sec (same as player)
	//	3 means 10 pixels/sec 
	uint32_t speed;
	
	//variable to keep track of speed
	uint32_t speedCount;
	
	//the amount of damage dealt when the entity touches the player
	//For chests it is the item sold
	//		0 for HP heal
	//		F for heart contsiner
	uint8_t damage;
	
	//1 if the entity can pass through walls
	//0 if the entity cannot pass through walls
	uint8_t phasing;
	
	//the amount of coins dropped when defeated
	//For chests it is the cost of the item held
	uint32_t coins;
	
	//the delay between shots in terms of SysTick cycles
	uint8_t shootDelay;
	
	//variable to keep track of shoot delay
	uint8_t shootCount;
	
	//the image the entity displays
	const unsigned short* image;
	
	//the projectile shot
	//	bit 4 is set if omnidirectional projectiles
	//	0 if no projectile
	//	1 for arrow
	//	2 for dekunut
	//	3 for bomb
	uint8_t ammo;

	//dimensions of the entity
	//	most entities are 10x10
	uint8_t dims[2];
	
	//bits 1-0 are the movement type
	//	0 is random
	//	1 is homing
	//	2 is homing w projectiles
	//	3 is teleporting ?????
	//bits 7-2 are a countdown for a period to move randomly
	//	a homing enemy will move randomly when there is no ideal path
	uint8_t movement;
	
	//1 if needed to be redrawn
	//0 if no need to redraw
	uint8_t redraw;
	
}entity_t;

// Player character lonk
typedef struct player{
	//lonk's current bottom left coordinates {x, y}
	// 	x: [19,99]		y: [69,149]
	uint32_t pos[2];
	
	//the direction the player is currently facing
	//	bit 0 for up, bit 1 for right, bit 2 for down, bit 3 for left
	uint8_t dir;
	
	//the current ammo the player is firing
	//	0 if no projectile
	//	1 for arrow
	uint8_t ammo;
	
	//lonk's maximum health. Starts at 6
	int32_t maxHealth;

	//lonk's remaining health. Every 2 is one heart
	int32_t health;
	
	//the current amount of rupoos the player has
	int32_t rupoos;
	
	//the items that the player can switch between
	//	0 is none
	//	1 is sword
	//	2 is shield
	uint8_t items[16];		
	
	//the delay between using each item
	//	0 is ready to use
	uint16_t itemDelay[16];
	
	//the number of items currently held
	uint8_t numItems;
	
	//the current item that ther player is using
	//	based off index in items
	uint8_t currentItem;
	
	//the dimensions for the player
	//	{width,height}
	uint8_t dims[2];
	
	//count down timer for the player's invincibility
	//	given a value after taking damage
	uint32_t invinCount;
	
	//a pointer to the current player image
	const unsigned short* image;
	
	//used to keep track of time slowed
	uint8_t speedDelay;
	
	//used to keep track of speed start
	//	set to 0 unless slowed
	uint8_t speed;
	
	//variable used to keep track of speed
	//	set to 0 unless slowed or paused
	uint8_t speedCount;
	
	//variable used to redraw
	//	1 if needed to be redrawn
	// 	0 if dont need to redraw
	uint8_t redraw;

}player_t;

//currency picked up by the player
typedef struct rupoo{
	//the current position of the projectile {x,y}
	//	x: [19,99]		y: [69,149]
	uint32_t pos[2];
	
	//the worth of the rupoo
	uint8_t value;
	
	//1 if the rupoo exists
	//0 if not
	uint8_t exists;
	
	//the duration for the rupoo to stay on the map
	//	duration based off clock speed
	uint32_t duration;
	
	//the images the projectile displays
	const unsigned short* image;
	
	//1 if needed to be redrawn
	//0 if no need to redraw
	uint8_t redraw;
	
}rupoo_t;

// A projectile fired from an enemy
typedef struct projectile{
	//the current position of the projectile {x,y}
	//	x: [19,99]		y: [69,149]
	uint32_t pos[2];
	
	//the direction the projectile is moving
	//	bit 0 for up, bit 1 for right, bit 2 for down, bit 3 for left
	uint8_t dir;
	
	//the entities the projectile can harm
	//	1 for friendly (hurts enemies)
	//	0 for harmful (hurts player)
	uint8_t friendly;
	
	//the speed that the bullet travels
	//the 8 MSB are used for bombCount
	//the next 8 MSB are used for max distance to travel
	//	1 means 30 pixels/sec (same as player)
	//	3 means 10 pixels/sec 
	uint32_t speed;
	
	//variable to keep track of speed
	uint16_t speedCount;
	
	//the amount of damage dealt when the proj touches the player
	uint8_t damage;
	
	//1 if the entity can pass through walls
	//0 if the entity cannot pass through walls
	uint8_t phasing;
	
	//the images the projectile displays
	const unsigned short* image;
	
	//the type of projectile. same as ammo
	uint8_t ammo;
	
	//dimensions of the entity
	//	most entities are 10x10
	uint8_t dims[2];
	
	//1 if the projectile exists
	uint8_t exists;
	
	//1 if needed to be redrawn
	//0 if no need to redraw
	uint8_t redraw;
	
}proj_t;

//a 9x9 arena where the player performs an action
typedef struct stage{
	
	//current size of the rupoo array
	//	0-16
	uint8_t rSize;
	
	//the array of projectiles
	rupoo_t rQ[16];
	
	//current size of the projectile array
	//	0-16
	uint8_t pSize;
	
	//the array of projectiles
	proj_t pA[16];
	
	//total num of enemies in the stage (does not include projectiles)
	// 	1-29 enemies
	uint8_t totalEnemies;
	
	//pointer to the enemies in the stage
	// 	can have up to 29 enemies
	//	can only have a total of 29 enemies + obstacles
	//	when all enemies defeated, the floor is cleared
	//	0 if no enemy
	entity_t enemies[29];
	
	//total num of obstacles in the stage (includes projectiles)
	// 	0-28 obstacles
	uint8_t totalObstacles;
	
	//obstacles in the stage
	// 	the player does not need to defeat these to clear the stage
	//	can have up to 28 obstacles
	//	can only have a total of 29 enemies + obstacles
	//	STAIRS AND SHOP STAGES USE OBSTACLES AS STAIRS/ITEMS
	//	0 if no obstacle
	entity_t obstacles[28];
	
	//1 if cleared (enemies are all defeated), 0 if not
	uint8_t cleared;
	
	//4 exits max
	//	bit 0 is north exit
	//	bit 1 is east exit
	//	bit 2 is south exit
	//	bit 3 is west exit
	uint8_t exits;
	
	//array that holds the corresponding next rooms (based off the floor indexing)
	//	Leave it a value of 0xFF if exit does not exist
	//	index 0 is the north exit
	//	index 1 is the east exit
	//	index 2 is the south exit
	//	index 3 is the west exit
	uint8_t nextRooms[4];
	
	//3 stage types
	//	0 is battle
	//	1 is shop		(room to purchase items)
	//	2 is stairs (exit to next floor)
	uint8_t type;
	
	//the image for the stage booundaries
	const unsigned short *image;
	
}stage_t;


extern stage_t *currentStage;

#endif
