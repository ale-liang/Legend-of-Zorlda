//stage.c

#include <stdint.h>
#include <stdlib.h>
#include "zstructs.h"
#include "stage.h"
#include "entity.h"
#include "Images.h"
#include "proj.h"
#include "rupoo.h"
#include "ST7735.h"
#include "player.h"

extern stage_t *currentStage;
extern player_t lonk;

//A single enemy uninitialized
entity_t zygote;

//the pointer to the unmodified enemy
entity_t *zPtr = &zygote;

//**********Check_Move********************
//Checks to see if the new image can be drawn without overriding an obstacle or wall
//inputs: x the x coord of the bottom left corner 
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Move(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY){
	//checks if it would conflict with the walls
	if (x < 19 || x + dimX > 109 || y - dimY < 59 || y > 149){
		return 0;	//returns 0 if there is a conflict
	}
	int count = 0;
	//checks if it would conflict with any obstacles
	uint32_t *obPos;	uint8_t *obDim;	//local variables to point at obstacle position and dimensions
	int i=0;	//variable for looping
	while (count < currentStage->totalObstacles && i < 28){	
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
					return 0;	//returns 0 if there is a conflict
				}
			}
		}i++;
	}return 1;	//return 1 if no conflicts found
}

//**********Check_Walls********************
//Checks to see if the given variables would hit a wall
//inputs: x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Walls(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY){
	//checks if it would conflict with the walls
	if (x < X || x + dimX > X + 90 || y - dimY < Y - 10 || y > Y + 80){
		return 0;	//returns 0 if there is a conflict
	}return 1;
}
//**********Draw_Borders_Up********************
//Draws the top border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Up(void){
	//draw the top row
	for (int x=9;x<=99;x+=10){
		ST7735_DrawBitmap(x,59, currentStage->image, 10,10);
	}
	//if the stage is cleared and the north exit exists, draws the exit
	if (currentStage->cleared == 1 && (currentStage->exits & 0x1)){	
		ST7735_DrawBitmap(NSexitX,NexitY,black10x10,10,10);
	}
}

//**********Draw_Borders_Right********************
//Draws the right border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Right(void){
	//draw the right column
	for (int y=149;y>=59;y-=10){
		ST7735_DrawBitmap(109,y, currentStage->image, 10,10);
	}
	//if the stage is cleared and the east exit exists, draws the exit
	if (currentStage->cleared == 1 && (currentStage->exits & 0x2)){
		ST7735_DrawBitmap(EexitX,EWexitY,black10x10,10,10);
	}
}

//**********Draw_Borders_Down********************
//Draws the bottom border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Down(void){
	//draw the bottom row
	for (int x=9;x<=109;x+=10){		
		ST7735_DrawBitmap(x,159, currentStage->image, 10,10);
	}
	//if the stage is cleared and the south exit exists, draws the exit
	if (currentStage->cleared == 1 && (currentStage->exits & 0x4)){	
		ST7735_DrawBitmap(NSexitX,SexitY,black10x10,10,10);
	}
}

//**********Draw_Borders_Left********************
//Draws the left border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Left(void){
	//draw the left column
	for (int y=149;y>=69;y-=10){
		ST7735_DrawBitmap(9,y, currentStage->image, 10,10);
	}
	//if the stage is cleared and the west exit exists, draws the exit
	if (currentStage->cleared == 1 && (currentStage->exits & 0x8)){	
		ST7735_DrawBitmap(WexitX,EWexitY,black10x10,10,10);
	}
}

//**********Draw_borders********************
//Draws the 4 walls of the stage
//	stage boundaries are x:[19,109] y:[59,149]
//	each block is 10x10 and drawn in the lower left corner
//inputs: none
//outputs: none
void Draw_Borders(){
	Draw_Borders_Up();
	Draw_Borders_Right();
	Draw_Borders_Down();
	Draw_Borders_Left();
}

//**********Clear_Obstacles********************
//Clears all leftover obstacles, players, rupoos, and projectiles
//inputs: none
//outputs: none
void Clear_Obstacles(void){
	int count,i,deleted;			//variables to help in loopiong
	count = i = deleted = 0;	//initialize to 0
	//Loop until we reached the end of the array or looked at all existing projs
	while (count<currentStage->pSize && i < pAsize){
		if (currentStage->pA[i].exists == 1){		//if it exists, then we're gonna clear it
			proj_t *temp = &(currentStage->pA[i]);	//grab for ease of access
			//Fill in the appropriate dimensions with black
			if (temp->dims[0] == 9 && temp->dims[1] == 3){	
				ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black9x3,9,3);
			}else if (temp->dims[0] == 3 && temp->dims[1] == 9){
				ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black3x9,3,9);
			}currentStage->pA[i].exists = 0;		//make sure it no longer exists
			deleted++;		//increment deleted variable
		}i++;		//always increment i
	}currentStage->pSize -= deleted;	//decrease the size by the # deleted
	count = i = 0;		//reset var values
	while (count < currentStage->rSize && i < 16){		//Do the same for rupoos
		//Rupoos exist if their value isnt 0 or if their redraw is 1
		if (currentStage->rQ[i].value != 0 || currentStage->rQ[i].redraw == 1){
			count++;	
			rupoo_t *temp = &(currentStage->rQ[i]);
			temp->redraw = 1;		//Lets the rupoo be properly discarded in the main
			temp->duration = 0;	//rupoo is out of time and disappears next main
			ST7735_DrawBitmap(temp->pos[0],temp->pos[1],black10x10,10,10);	//Draw black over it 
		}i++;
	}currentStage->rSize = 0;		//reset the size of the rupoo array
	//iterates through the whole board and erases any leftover drawings
	for (int x = X;x<(X+81);x+=10){
		for (int y = Y;y<(Y+81);y+=10){
			ST7735_DrawBitmap(x,y,black10x10,10,10);
		}
	}
}

//**********Redraw_All********************
//Sets all the redraw flags (enemy/obstacle/player)
//input: none
//output: none
void Redraw_All(player_t *lonk){
	lonk->redraw = 1;	//lonk needs to be redrawn
	int i = 0; int count = 0;		//variables to help w loops
	//loop through enemies and if still alive, redraw them
	for (i=0;i<currentStage->totalEnemies;i++){
		if (currentStage->enemies[i].health > 0){
			currentStage->enemies[i].redraw = 1;
		}
	}i = 0;	
	//loop through obstacles and if they're still unbroken, redraw them
	while (count < currentStage->totalObstacles && i < 28){
		if (currentStage->obstacles[i].dur >= 0){
			count++;
			currentStage->obstacles[i].redraw = 1;
		}i++;
	}i = count = 0;
	//loop through rupoos and if they still exist, redraw them
	while (count < currentStage->rSize && i < 16){
		if (currentStage->rQ[i].exists == 1){
			count++;
			currentStage->rQ[i].redraw = 1;
		}i++;
	}
}

//**********Make_Stair********************
//Makes a stage into a room to ascend to the next floor
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Stairs(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 0;	st->totalObstacles = 1;
	Make_StairObstacle(zPtr,X+40,Y+40);
	st->obstacles[0] = *zPtr;
	st->cleared = 1;	st->exits = exits;
	st->type = 2;
	if (floor == 1)	st->image = bushObstacle;
	else if (floor == 2) st->image = waterObstacle;
	else if (floor == 3) st->image = brickObstacle;
	else if (floor == 4) st->image = iceObstacle;
	else if (floor == 5) st->image = lavaObstacle;
	else st->image = block;
}

//**********Make_Shop********************
//Makes a stage into a room to purchase items with rupoos
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//outputs: none
void Make_Shop(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 0;
	Make_Shopkeeper(zPtr,X+40,Y+40);
	st->obstacles[0] = *zPtr;
	Make_BrownChest(zPtr,25);
	st->obstacles[1] = *zPtr;
	Make_GreenChest(zPtr,50);
	st->obstacles[2] = *zPtr;
	st->totalObstacles = 3;
	if (floor > 1){
		Make_RedChest(zPtr,100);
		st->obstacles[3] = *zPtr;
		st->totalObstacles++;
		if (floor > 2){
			Make_BlueChest(zPtr,250);
			st->obstacles[4] = *zPtr;
			st->totalObstacles++;
		}
	}st->cleared = 1;
	st->exits = exits;
	st->type = 1;
	if (floor == 1)	st->image = bushObstacle;
	else if (floor == 2) st->image = waterObstacle;
	else if (floor == 3) st->image = brickObstacle;
	else if (floor == 4) st->image = iceObstacle;
	else if (floor == 5) st->image = lavaObstacle;
	else st->image = block;
}

//**********Next_Rooms*********************
//changes the nextRooms array of a stage
//	next room changes rooms based off index in floor array
//inputs: the 4 numbers for corresponding rooms (0xFF if DNE)
//outputs: none
void Next_Rooms(stage_t *st, uint8_t n, uint8_t e, uint8_t s, uint8_t w){
	st->nextRooms[0] = n;
	st->nextRooms[1] = e;
	st->nextRooms[2] = s;
	st->nextRooms[3] = w;
}
void Make_TestStage1(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
//	Make_Skeleton(zPtr,89,99,0);
//	st->enemies[1] = *zPtr;
//	Make_slimeBoss(zPtr,X+70,Y+70,lt);
//	st->enemies[2] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 1;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 0;	
	st->type = 0;	st->image = bushObstacle;
}

void Make_TestStage2(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
	Make_Skeleton(zPtr,89,99,0);
	st->enemies[1] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 2;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 2;	
	st->type = 0;	st->image = bushObstacle;
}

void Make_TestStage3(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
	Make_Skeleton(zPtr,89,99,0);
	st->enemies[1] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 2;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 0;	
	st->type = 0;	st->image = bushObstacle;
}

void Make_TestStage4(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
	Make_Skeleton(zPtr,89,99,0);
	st->enemies[1] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 2;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 0;	
	st->type = 0;	st->image = bushObstacle;
}

void Make_TestStage5(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
	Make_Skeleton(zPtr,89,99,0);
	st->enemies[1] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 2;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 0;	
	st->type = 0;	st->image = bushObstacle;
}

//DATABASE FOR STAGES WILL BE HERE
void Make_TestStage(stage_t* st){
	st->pSize = 0;
	Make_Slime(zPtr,49,99,0);
	st->enemies[0] = *zPtr;
	Make_Brick(zPtr,59,99);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,49,109);
	st->obstacles[1] = *zPtr;
	Make_Skeleton(zPtr,89,99,0);
	st->enemies[1] = *zPtr;
	st->totalObstacles = 2;
	st->totalEnemies = 2;
	
	st->cleared = 0;
	st->exits = 0xF;
	st->nextRooms[0] = 0;	st->nextRooms[1] = 1;	
	st->nextRooms[2] = 0;	st->nextRooms[3] = 0;	
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_Bush1Stage********************
//Makes a stage into a bush1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush1Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 5;
	Make_Bush(zPtr,X+40,Y+40);
	st->obstacles[0] = *zPtr;
	Make_Bush(zPtr,X+30,Y+30);
	st->obstacles[1] = *zPtr;
	Make_Bush(zPtr,X+50,Y+50);
	st->obstacles[2] = *zPtr;
	Make_Bush(zPtr,X+30,Y+50);
	st->obstacles[3] = *zPtr;
	Make_Bush(zPtr,X+50,Y+30);
	st->obstacles[4] = *zPtr;
	Make_Slime(zPtr,X+40,Y+50,0);
	st->enemies[0] = *zPtr;
	Make_Slime(zPtr,X+40,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Slime(zPtr,X+30,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+50,Y+40,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = bushObstacle;
}

//**********Make_Bush2Stage********************
//Makes a stage into a bush2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush2Stage(stage_t* st, uint8_t exits, uint8_t floor){
	st->pSize = 0;
	Make_Bear(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Snake(zPtr,X+80,Y,0);
	st->enemies[1] = *zPtr;
	Make_Snake(zPtr,X+40,Y+20,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+40,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Slime(zPtr,X+30,Y+10,0);
	st->enemies[4] = *zPtr;
	Make_Slime(zPtr,X+40,Y+80,0);
	st->enemies[5] = *zPtr;
	Make_Slime(zPtr,X+80,Y+80,0);
	st->enemies[6] = *zPtr;
	Make_Slime(zPtr,X+50,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Bush(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Bush(zPtr,X+60,Y+60);
	st->obstacles[1] = *zPtr;
	Make_Bush(zPtr,X+20,Y+60);
	st->obstacles[2] = *zPtr;
	Make_Bush(zPtr,X+60,Y+20);
	st->obstacles[3] = *zPtr;
	st->totalObstacles = 4;
	st->totalEnemies = 8;
	st->cleared = 0;
	st->exits = exits;
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_Bush3Stage********************
//Makes a stage into a bush3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush3Stage(stage_t* st, uint8_t exits, uint8_t floor){
	st->pSize = 0;
	Make_Bear(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Snake(zPtr,X+30,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Snake(zPtr,X+50,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+30,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Slime(zPtr,X+40,Y+30,0);
	st->enemies[4] = *zPtr;
	Make_Slime(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Slime(zPtr,X+50,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Snake(zPtr,X+30,Y+50,0);
	st->enemies[7] = *zPtr;
	Make_Snake(zPtr,X+50,Y+30,0);
	st->enemies[8] = *zPtr;
	st->totalObstacles = 0;
	st->totalEnemies = 9;
	st->cleared = 0;
	st->exits = exits;
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_Bush4Stage********************
//Makes a stage into a bush4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush4Stage(stage_t* st, uint8_t exits, uint8_t floor){
	st->pSize = 0;
	st->totalObstacles = 10;
	st->totalEnemies = 8;
	Make_Bear(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Snake(zPtr,X+80,Y,0);
	st->enemies[1] = *zPtr;
	Make_Snake(zPtr,X+40,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+30,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Slime(zPtr,X+20,Y+40,0);
	st->enemies[4] = *zPtr;
	Make_Slime(zPtr,X+50,Y+40,0);
	st->enemies[5] = *zPtr;
	Make_Slime(zPtr,X+80,Y+80,0);
	st->enemies[6] = *zPtr;
	Make_Slime(zPtr,X,Y+80,0);
	st->enemies[7] = *zPtr;
	Make_Bush(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Bush(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Bush(zPtr,X+40,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Bush(zPtr,X+50,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Bush(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Bush(zPtr,X+60,Y+60);
	st->obstacles[5] = *zPtr;
	Make_Bush(zPtr,X+50,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Bush(zPtr,X+40,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Bush(zPtr,X+30,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Bush(zPtr,X+20,Y+60);
	st->obstacles[9] = *zPtr;
	st->cleared = 0;
	st->exits = exits;
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_Bush5Stage******************** COPY OF BUSH3
//Makes a stage into a bush5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush5Stage(stage_t* st, uint8_t exits, uint8_t floor){
	st->pSize = 0;
	Make_Bear(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Snake(zPtr,X+30,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Snake(zPtr,X+50,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+30,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Slime(zPtr,X+40,Y+30,0);
	st->enemies[4] = *zPtr;
	Make_Slime(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Slime(zPtr,X+50,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Snake(zPtr,X+30,Y+50,0);
	st->enemies[7] = *zPtr;
	Make_Snake(zPtr,X+50,Y+30,0);
	st->enemies[8] = *zPtr;
	st->totalObstacles = 0;
	st->totalEnemies = 9;
	st->cleared = 0;
	st->exits = exits;
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_SlimeBossStage********************
//Makes a stage into a slime boss stage room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_SlimeBossStage(stage_t* st, uint8_t exits, uint8_t floor){
	st->pSize = 0;
	Make_slimeBoss(zPtr,X+40,Y+20,0);	
	st->enemies[0] = *zPtr;
	Make_Slime(zPtr,X+10,Y+10,0);
	st->enemies[1] = *zPtr;
	Make_Slime(zPtr,X+70,Y+10,0);
	st->enemies[2] = *zPtr;
	Make_Slime(zPtr,X+40,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Slime(zPtr,X+80,Y+40,0);
	st->enemies[4] = *zPtr;
	Make_Bush(zPtr,X+10,Y);
	st->obstacles[0] = *zPtr;
	Make_Bush(zPtr,X,Y+10);
	st->obstacles[1] = *zPtr;
	Make_Bush(zPtr,X,Y);
	st->obstacles[2] = *zPtr;
	Make_Bush(zPtr,X+70,Y);
	st->obstacles[3] = *zPtr;
	Make_Bush(zPtr,X+80,Y);
	st->obstacles[4] = *zPtr;
	Make_Bush(zPtr,X+80,Y+10);
	st->obstacles[5] = *zPtr;
	Make_Bush(zPtr,X+20,Y+40);
	st->obstacles[6] = *zPtr;
	Make_Bush(zPtr,X+20,Y+50);
	st->obstacles[7] = *zPtr;
	Make_Bush(zPtr,X+60,Y+40);
	st->obstacles[8] = *zPtr;
	Make_Bush(zPtr,X+60,Y+50);
	st->obstacles[9] = *zPtr;
	st->totalObstacles = 10;
	st->totalEnemies = 5;
	st->cleared = 0;
	st->exits = exits;
	st->type = 0;	st->image = bushObstacle;
}

//**********Make_Water1Stage********************
//Makes a stage into a bush1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water1Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 14;
	Make_Water(zPtr,X+40,Y+40);
	st->obstacles[0] = *zPtr;
	Make_Water(zPtr,X+30,Y+40);
	st->obstacles[1] = *zPtr;
	Make_Water(zPtr,X+50,Y+40);
	st->obstacles[2] = *zPtr;
	Make_Water(zPtr,X+40,Y+30);
	st->obstacles[3] = *zPtr;
	Make_Water(zPtr,X+40,Y+50);
	st->obstacles[4] = *zPtr;
	Make_Water(zPtr,X,Y);
	st->obstacles[5] = *zPtr;
	Make_Water(zPtr,X+10,Y);
	st->obstacles[6] = *zPtr;
	Make_Water(zPtr,X,Y+10);
	st->obstacles[7] = *zPtr;
	Make_Water(zPtr,X+80,Y+80);
	st->obstacles[8] = *zPtr;
	Make_Water(zPtr,X+80,Y+70);
	st->obstacles[9] = *zPtr;
	Make_Water(zPtr,X+70,Y+80);
	st->obstacles[10] = *zPtr;
	Make_Water(zPtr,X+80,Y);
	st->obstacles[11] = *zPtr;
	Make_Water(zPtr,X+80,Y+10);
	st->obstacles[12] = *zPtr;
	Make_Water(zPtr,X+70,Y);
	st->obstacles[13] = *zPtr;
	Make_Crab(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Crab(zPtr,X+50,Y+50,0);
	st->enemies[1] = *zPtr;
	Make_Squid(zPtr,X+30,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Squid(zPtr,X+50,Y+30,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_Water2Stage********************
//Makes a stage into a Water2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water2Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 6;	st->totalObstacles = 8;
	Make_Water(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Water(zPtr,X+10,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Water(zPtr,X+60,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Water(zPtr,X+70,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Water(zPtr,X+20,Y+60);
	st->obstacles[4] = *zPtr;
	Make_Water(zPtr,X+20,Y+70);
	st->obstacles[5] = *zPtr;
	Make_Water(zPtr,X+60,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Water(zPtr,X+60,Y+70);
	st->obstacles[7] = *zPtr;
	Make_Crab(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Crab(zPtr,X+50,Y+50,0);
	st->enemies[1] = *zPtr;
	Make_Squid(zPtr,X+30,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Squid(zPtr,X+50,Y+30,0);
	st->enemies[3] = *zPtr;
	Make_Dekuscrub(zPtr,X,Y,0);
	st->enemies[4] = *zPtr;
	Make_Dekuscrub(zPtr,X+80,Y,0);
	st->enemies[5] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_Water3Stage********************
//Makes a stage into a Water3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water3Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 0;
	Make_Crab(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Crab(zPtr,X+40,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Crab(zPtr,X+50,Y+30,0);
	st->enemies[2] = *zPtr;
	Make_Squid(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Squid(zPtr,X+50,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Squid(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Dekuscrub(zPtr,X+40,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Dekuscrub(zPtr,X+30,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Dekuscrub(zPtr,X+50,Y+40,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_Water4Stage********************
//Makes a stage into a Water4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water4Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 9;
	Make_Water(zPtr,X+30,Y+30);
	st->obstacles[0] = *zPtr;
	Make_Water(zPtr,X+40,Y+30);
	st->obstacles[1] = *zPtr;
	Make_Water(zPtr,X+50,Y+30);
	st->obstacles[2] = *zPtr;
	Make_Water(zPtr,X+30,Y+50);
	st->obstacles[3] = *zPtr;
	Make_Water(zPtr,X+50,Y+50);
	st->obstacles[4] = *zPtr;
	Make_Water(zPtr,X+40,Y+50);
	st->obstacles[5] = *zPtr;
	Make_Water(zPtr,X+40,Y+40);
	st->obstacles[6] = *zPtr;
	Make_Water(zPtr,X+30,Y+40);
	st->obstacles[7] = *zPtr;
	Make_Water(zPtr,X+50,Y+40);
	st->obstacles[8] = *zPtr;
	Make_Dekuscrub(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Dekuscrub(zPtr,X,Y+80,0);
	st->enemies[1] = *zPtr;
	Make_Dekuscrub(zPtr,X+80,Y,0);
	st->enemies[2] = *zPtr;
	Make_Dekuscrub(zPtr,X+80,Y+80,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_Water5Stage******************** COPY OF WATER3
//Makes a stage into a Water5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water5Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 0;
	Make_Crab(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Crab(zPtr,X+40,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Crab(zPtr,X+50,Y+30,0);
	st->enemies[2] = *zPtr;
	Make_Squid(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Squid(zPtr,X+50,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Squid(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Dekuscrub(zPtr,X+40,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Dekuscrub(zPtr,X+30,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Dekuscrub(zPtr,X+50,Y+40,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_OrcBossStage********************
//Makes a stage into a OrcBoss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_OrcBossStage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 16;
	Make_Water(zPtr,X,Y);
	st->obstacles[0] = *zPtr;
	Make_Water(zPtr,X+10,Y+10);
	st->obstacles[1] = *zPtr;
	Make_Water(zPtr,X+10,Y);
	st->obstacles[2] = *zPtr;
	Make_Water(zPtr,X,Y+10);
	st->obstacles[3] = *zPtr;
	Make_Water(zPtr,X+70,Y);
	st->obstacles[4] = *zPtr;
	Make_Water(zPtr,X+80,Y);
	st->obstacles[5] = *zPtr;
	Make_Water(zPtr,X+70,Y+10);
	st->obstacles[6] = *zPtr;
	Make_Water(zPtr,X+80,Y+10);
	st->obstacles[7] = *zPtr;
	Make_Water(zPtr,X,Y+70);
	st->obstacles[8] = *zPtr;
	Make_Water(zPtr,X,Y+80);
	st->obstacles[9] = *zPtr;
	Make_Water(zPtr,X+10,Y+70);
	st->obstacles[10] = *zPtr;
	Make_Water(zPtr,X+10,Y+80);
	st->obstacles[11] = *zPtr;
	Make_Water(zPtr,X+70,Y+70);
	st->obstacles[12] = *zPtr;
	Make_Water(zPtr,X+70,Y+80);
	st->obstacles[13] = *zPtr;
	Make_Water(zPtr,X+80,Y+70);
	st->obstacles[14] = *zPtr;
	Make_Water(zPtr,X+80,Y+80);
	st->obstacles[15] = *zPtr;
	Make_orcBoss(zPtr,X+35,Y+10,0);
	st->enemies[0] = *zPtr;
	Make_Crab(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Squid(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Dekuscrub(zPtr,X+40,Y+40,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = waterObstacle;
}

//**********Make_Brick1Stage********************
//Makes a stage into a Brick1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick1Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 7;	st->totalObstacles = 10;
	Make_Brick(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+50,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Brick(zPtr,X+60,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+30,Y+60);
	st->obstacles[4] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+60,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Brick(zPtr,X+50,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Brick(zPtr,X+40,Y+40);
	st->obstacles[8] = *zPtr;
	Make_Ebat(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Ebat(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Ebat(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+40,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+30,0);
	st->enemies[5] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+50,0);
	st->enemies[6] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle;
}

//**********Make_Brick2Stage********************
//Makes a stage into a Brick1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick2Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 12;	st->totalObstacles = 8;
	Make_Brick(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	
	Make_Brick(zPtr,X+20,Y);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[2] = *zPtr;
	
	Make_Brick(zPtr,X+20,Y+80);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	
	Make_Brick(zPtr,X+60,Y);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+60,Y+60);
	st->obstacles[6] = *zPtr;
	
	Make_Brick(zPtr,X+60,Y+80);
	st->obstacles[7] = *zPtr;
	Make_Ebat(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Ebat(zPtr,X+80,Y+80,0);
	st->enemies[1] = *zPtr;
	Make_Skeleton(zPtr,X+10,Y,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+70,Y+80,0);
	st->enemies[3] = *zPtr;
	Make_Skeleton(zPtr,X,Y+80,0);
	st->enemies[4] = *zPtr;
	Make_Spider(zPtr,X+10,Y+80,0);
	st->enemies[5] = *zPtr;
	Make_Skeleton(zPtr,X+80,Y,0);
	st->enemies[6] = *zPtr;
	Make_Spider(zPtr,X+70,Y,0);
	st->enemies[7] = *zPtr;
	Make_BrickArrows(zPtr,X+20,Y+70);
	st->enemies[8] = *zPtr;
	Make_BrickArrows(zPtr,X+60,Y+10);
	st->enemies[9] = *zPtr;
	Make_BrickArrows(zPtr,X+20,Y+10);
	st->enemies[10] = *zPtr;
	Make_BrickArrows(zPtr,X+60,Y+70);
	st->enemies[11] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle;
}

//**********Make_Brick3Stage********************
//Makes a stage into a Brick3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick3Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 16;
	Make_Brick(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+40,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Brick(zPtr,X+50,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Brick(zPtr,X+60,Y+30);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+60,Y+40);
	st->obstacles[6] = *zPtr;
	Make_Brick(zPtr,X+60,Y+50);
	st->obstacles[7] = *zPtr;
	Make_Brick(zPtr,X+60,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Brick(zPtr,X+50,Y+60);
	st->obstacles[9] = *zPtr;
	Make_Brick(zPtr,X+40,Y+60);
	st->obstacles[10] = *zPtr;
	Make_Brick(zPtr,X+30,Y+60);
	st->obstacles[11] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[12] = *zPtr;
	Make_Brick(zPtr,X+20,Y+50);
	st->obstacles[13] = *zPtr;
	Make_Brick(zPtr,X+20,Y+40);
	st->obstacles[14] = *zPtr;
	Make_Brick(zPtr,X+20,Y+30);
	st->obstacles[15] = *zPtr;
	Make_Ebat(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Spider(zPtr,X+40,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Ebat(zPtr,X+50,Y+30,0);
	st->enemies[2] = *zPtr;
	Make_Ebat(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Ebat(zPtr,X+50,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Spider(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Spider(zPtr,X+40,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Spider(zPtr,X+30,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Spider(zPtr,X+50,Y+40,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle;
}

//**********Make_Brick4Stage********************
//Makes a stage into a Brick3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick4Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 8;	st->totalObstacles = 16;
	Make_Brick(zPtr,X+60-4,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,X+70-4,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+60-4,Y+30);
	st->obstacles[2] = *zPtr;
	Make_Brick(zPtr,X+70-4,Y+30);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[4] = *zPtr;
	Make_Brick(zPtr,X+20,Y+70);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+30,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Brick(zPtr,X+30,Y+70);
	st->obstacles[7] = *zPtr;
	Make_Ebat(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Spider(zPtr,X,Y+10,0);
	st->enemies[1] = *zPtr;
	Make_Ebat(zPtr,X+10,Y+10,0);
	st->enemies[2] = *zPtr;
	Make_Ebat(zPtr,X+80,Y+80,0);
	st->enemies[3] = *zPtr;
	Make_Ebat(zPtr,X+70,Y+70,0);
	st->enemies[4] = *zPtr;
	Make_Spider(zPtr,X+10,Y,0);
	st->enemies[5] = *zPtr;
	Make_Spider(zPtr,X+70,Y+80,0);
	st->enemies[6] = *zPtr;
	Make_Spider(zPtr,X+80,Y+70,0);
	st->enemies[7] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle;
}

//**********Make_Brick5Stage******************** COPY OF BRICK3
//Makes a stage into a Brick3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick5Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 16;
	Make_Brick(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+40,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Brick(zPtr,X+50,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Brick(zPtr,X+60,Y+30);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+60,Y+40);
	st->obstacles[6] = *zPtr;
	Make_Brick(zPtr,X+60,Y+50);
	st->obstacles[7] = *zPtr;
	Make_Brick(zPtr,X+60,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Brick(zPtr,X+50,Y+60);
	st->obstacles[9] = *zPtr;
	Make_Brick(zPtr,X+40,Y+60);
	st->obstacles[10] = *zPtr;
	Make_Brick(zPtr,X+30,Y+60);
	st->obstacles[11] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[12] = *zPtr;
	Make_Brick(zPtr,X+20,Y+50);
	st->obstacles[13] = *zPtr;
	Make_Brick(zPtr,X+20,Y+40);
	st->obstacles[14] = *zPtr;
	Make_Brick(zPtr,X+20,Y+30);
	st->obstacles[15] = *zPtr;
	Make_Ebat(zPtr,X+30,Y+30,0);
	st->enemies[0] = *zPtr;
	Make_Spider(zPtr,X+40,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Ebat(zPtr,X+50,Y+30,0);
	st->enemies[2] = *zPtr;
	Make_Ebat(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Ebat(zPtr,X+50,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Spider(zPtr,X+40,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Spider(zPtr,X+40,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Spider(zPtr,X+30,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Spider(zPtr,X+50,Y+40,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle;
}

//**********Make_LynelBossStage********************
//Makes a stage into a OrcBoss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_LynelBossStage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 10;
	Make_Brick(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Brick(zPtr,X+20,Y+30);
	st->obstacles[1] = *zPtr;
	Make_Brick(zPtr,X+20,Y+40);
	st->obstacles[2] = *zPtr;
	Make_Brick(zPtr,X+20,Y+50);
	st->obstacles[3] = *zPtr;
	Make_Brick(zPtr,X+20,Y+60);
	st->obstacles[4] = *zPtr;
	Make_Brick(zPtr,X+60,Y+20);
	st->obstacles[5] = *zPtr;
	Make_Brick(zPtr,X+60,Y+30);
	st->obstacles[6] = *zPtr;
	Make_Brick(zPtr,X+60,Y+40);
	st->obstacles[7] = *zPtr;
	Make_Brick(zPtr,X+60,Y+50);
	st->obstacles[8] = *zPtr;
	Make_Brick(zPtr,X+60,Y+60);
	st->obstacles[9] = *zPtr;
	Make_LynelBoss(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Spider(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Ebat(zPtr,X+40,Y+40,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = brickObstacle	;
}

//**********Make_Ice1Stage********************
//Makes a stage into a Ice1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice1Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 6;	st->totalObstacles = 13;
	Make_Ice(zPtr,X,Y);
	st->obstacles[0] = *zPtr;
	Make_Ice(zPtr,X+10,Y);
	st->obstacles[1] = *zPtr;
	Make_Ice(zPtr,X+10,Y+70);
	st->obstacles[2] = *zPtr;
	Make_Ice(zPtr,X+10,Y+60);
	st->obstacles[3] = *zPtr;
	Make_Ice(zPtr,X+40,Y+40);
	st->obstacles[4] = *zPtr;
	Make_Ice(zPtr,X+40,Y+50);
	st->obstacles[5] = *zPtr;
	Make_Ice(zPtr,X+60,Y+80);
	st->obstacles[6] = *zPtr;
	Make_Ice(zPtr,X+60,Y+70);
	st->obstacles[7] = *zPtr;
	Make_Ice(zPtr,X+50,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Ice(zPtr,X+60,Y+60);
	st->obstacles[9] = *zPtr;
	Make_Ice(zPtr,X+80,Y);
	st->obstacles[10] = *zPtr;
	Make_Ice(zPtr,X+80,Y+10);
	st->obstacles[11] = *zPtr;
	Make_Ice(zPtr,X+70,Y);
	st->obstacles[12] = *zPtr;
	Make_Bear(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Bear(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Fbow(zPtr,X+20,Y,0);
	st->enemies[4] = *zPtr;
	Make_Fbow(zPtr,X+60,Y,0);
	st->enemies[5] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_Ice2Stage********************
//Makes a stage into a Ice2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice2Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 6;	st->totalObstacles = 16;
	Make_Ice(zPtr,X,Y);
	st->obstacles[0] = *zPtr;
	Make_Ice(zPtr,X+10,Y);
	st->obstacles[1] = *zPtr;
	Make_Ice(zPtr,X,Y+10);
	st->obstacles[2] = *zPtr;
	Make_Ice(zPtr,X+70,Y);
	st->obstacles[3] = *zPtr;
	Make_Ice(zPtr,X+80,Y);
	st->obstacles[4] = *zPtr;
	Make_Ice(zPtr,X+80,Y+10);
	st->obstacles[5] = *zPtr;
	Make_Ice(zPtr,X+70,Y+80);
	st->obstacles[6] = *zPtr;
	Make_Ice(zPtr,X+80,Y+70);
	st->obstacles[7] = *zPtr;
	Make_Ice(zPtr,X+80,Y+80);
	st->obstacles[8] = *zPtr;
	Make_Ice(zPtr,X,Y+80);
	st->obstacles[9] = *zPtr;
	Make_Ice(zPtr,X,Y+70);
	st->obstacles[10] = *zPtr;
	Make_Ice(zPtr,X+10,Y+80);
	st->obstacles[11] = *zPtr;
	Make_Ice(zPtr,X+20,Y+20);
	st->obstacles[12] = *zPtr;
	Make_Ice(zPtr,X+20,Y+60);
	st->obstacles[13] = *zPtr;
	Make_Ice(zPtr,X+60,Y+60);
	st->obstacles[14] = *zPtr;
	Make_Ice(zPtr,X+60,Y+20);
	st->obstacles[15] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+10,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+70,0);
	st->enemies[1] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+10,0);
	st->enemies[2] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+70,0);
	st->enemies[3] = *zPtr;
	Make_Fbow(zPtr,X+30,Y+40,0);
	st->enemies[4] = *zPtr;
	Make_Fbow(zPtr,X+50,Y+40,0);
	st->enemies[5] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_Ice3Stage********************
//Makes a stage into a Ice3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice3Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 16;	st->totalObstacles = 0;
	Make_Snowman(zPtr,X+10,Y+10,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+70,0);
	st->enemies[3] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+10,0);
	st->enemies[4] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+30,0);
	st->enemies[5] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+50,0);
	st->enemies[6] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+70,0);
	st->enemies[7] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+10,0);
	st->enemies[8] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+30,0);
	st->enemies[9] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+50,0);
	st->enemies[10] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+70,0);
	st->enemies[11] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+10,0);
	st->enemies[12] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+30,0);
	st->enemies[13] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+50,0);
	st->enemies[14] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+70,0);
	st->enemies[15] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_Ice4Stage********************
//Makes a stage into a Ice4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice4Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 6;	st->totalObstacles = 10;
	Make_Ice(zPtr,X,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Ice(zPtr,X+20,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Ice(zPtr,X+40,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Ice(zPtr,X+60,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Ice(zPtr,X+80,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Ice(zPtr,X,Y+60);
	st->obstacles[5] = *zPtr;
	Make_Ice(zPtr,X+20,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Ice(zPtr,X+40,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Ice(zPtr,X+60,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Ice(zPtr,X+80,Y+60);
	st->obstacles[9] = *zPtr;
	Make_Snowman(zPtr,X,Y,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X+80,Y+80,0);
	st->enemies[1] = *zPtr;
	Make_Bear(zPtr,X+80,Y,0);
	st->enemies[2] = *zPtr;
	Make_Bear(zPtr,X,Y+80,0);
	st->enemies[3] = *zPtr;
	Make_Fbow(zPtr,X+30,Y+40,0);
	st->enemies[4] = *zPtr;
	Make_Fbow(zPtr,X+50,Y+40,0);
	st->enemies[5] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_Ice5Stage******************** COPY OF ICE3
//Makes a stage into a Ice3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice5Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 16;	st->totalObstacles = 0;
	Make_Snowman(zPtr,X+10,Y+10,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+50,0);
	st->enemies[2] = *zPtr;
	Make_Snowman(zPtr,X+10,Y+70,0);
	st->enemies[3] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+10,0);
	st->enemies[4] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+30,0);
	st->enemies[5] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+50,0);
	st->enemies[6] = *zPtr;
	Make_Snowman(zPtr,X+30,Y+70,0);
	st->enemies[7] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+10,0);
	st->enemies[8] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+30,0);
	st->enemies[9] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+50,0);
	st->enemies[10] = *zPtr;
	Make_Snowman(zPtr,X+50,Y+70,0);
	st->enemies[11] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+10,0);
	st->enemies[12] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+30,0);
	st->enemies[13] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+50,0);
	st->enemies[14] = *zPtr;
	Make_Snowman(zPtr,X+70,Y+70,0);
	st->enemies[15] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_IcegolemBossStage********************
//Makes a stage into a Ice golem Boss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_IcegolemBossStage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 10;
	Make_Ice(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Ice(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Ice(zPtr,X+40,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Ice(zPtr,X+50,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Ice(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Ice(zPtr,X+20,Y+60);
	st->obstacles[5] = *zPtr;
	Make_Ice(zPtr,X+30,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Ice(zPtr,X+40,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Ice(zPtr,X+50,Y+60);
	st->obstacles[8] = *zPtr;
	Make_Ice(zPtr,X+60,Y+60);
	st->obstacles[9] = *zPtr;
	Make_icegolemBoss(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Snowman(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Snowman(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Snowman(zPtr,X+40,Y+40,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = iceObstacle;
}

//**********Make_Lava1Stage********************
//Makes a stage into a Lava1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava1Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 8;	st->totalObstacles = 16;
	Make_Lava(zPtr,X+20,Y);
	st->obstacles[0] = *zPtr;
	Make_Lava(zPtr,X+20,Y+10);
	st->obstacles[1] = *zPtr;
	Make_Lava(zPtr,X+60,Y+10);
	st->obstacles[2] = *zPtr;
	Make_Lava(zPtr,X+60,Y);
	st->obstacles[3] = *zPtr;
	Make_Lava(zPtr,X,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Lava(zPtr,X+10,Y+20);
	st->obstacles[5] = *zPtr;
	Make_Lava(zPtr,X,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Lava(zPtr,X+10,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Lava(zPtr,X+20,Y+80);
	st->obstacles[8] = *zPtr;
	Make_Lava(zPtr,X+20,Y+70);
	st->obstacles[9] = *zPtr;
	Make_Lava(zPtr,X+60,Y+70);
	st->obstacles[10] = *zPtr;
	Make_Lava(zPtr,X+60,Y+80);
	st->obstacles[11] = *zPtr;
	Make_Lava(zPtr,X+80,Y+20);
	st->obstacles[12] = *zPtr;
	Make_Lava(zPtr,X+70,Y+20);
	st->obstacles[13] = *zPtr;
	Make_Lava(zPtr,X+80,Y+60);
	st->obstacles[14] = *zPtr;
	Make_Lava(zPtr,X+70,Y+60);
	st->obstacles[15] = *zPtr;
	Make_Skelebomber(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+10,0);
	st->enemies[1] = *zPtr;
	Make_Skelebomber(zPtr,X,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+10,Y+40,0);
	st->enemies[3] = *zPtr;
	Make_Skelebomber(zPtr,X+40,Y+80,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+70,0);
	st->enemies[5] = *zPtr;
	Make_Skelebomber(zPtr,X+80,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Skeleton(zPtr,X+70,Y+40,0);
	st->enemies[7] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}

//**********Make_Lava2Stage********************
//Makes a stage into a Lava2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava2Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 8;
	Make_Lava(zPtr,X+20,Y);
	st->obstacles[0] = *zPtr;
	Make_Lava(zPtr,X+60,Y);
	st->obstacles[1] = *zPtr;
	Make_Lava(zPtr,X,Y+20);
	st->obstacles[2] = *zPtr;
	Make_Lava(zPtr,X,Y+60);
	st->obstacles[3] = *zPtr;
	Make_Lava(zPtr,X+20,Y+80);
	st->obstacles[4] = *zPtr;
	Make_Lava(zPtr,X+60,Y+80);
	st->obstacles[5] = *zPtr;
	Make_Lava(zPtr,X+80,Y+20);
	st->obstacles[6] = *zPtr;
	Make_Lava(zPtr,X+80,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Skelebomber(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+30,0);
	st->enemies[7] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+30,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}

//**********Make_Lava3Stage********************
//Makes a stage into a Lava3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava3Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 15;	st->totalObstacles = 0;
	Make_Skelebomber(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+30,0);
	st->enemies[7] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+30,0);
	st->enemies[8] = *zPtr;
	Make_Skelebomber(zPtr,X,Y,0);
	st->enemies[9] = *zPtr;
	Make_Skeleton(zPtr,X+10,Y,0);
	st->enemies[10] = *zPtr;
	Make_Skeleton(zPtr,X,Y+10,0);
	st->enemies[11] = *zPtr;
	Make_Skelebomber(zPtr,X+80,Y,0);
	st->enemies[12] = *zPtr;
	Make_Skeleton(zPtr,X+70,Y,0);
	st->enemies[13] = *zPtr;
	Make_Skeleton(zPtr,X+80,Y+10,0);
	st->enemies[14] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}

//**********Make_Lava4Stage********************
//Makes a stage into a Lava1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava4Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 9;	st->totalObstacles = 16;
	Make_Lava(zPtr,X,Y);
	st->obstacles[0] = *zPtr;
	Make_Lava(zPtr,X+10,Y+10);
	st->obstacles[1] = *zPtr;
	Make_Lava(zPtr,X+10,Y);
	st->obstacles[2] = *zPtr;
	Make_Lava(zPtr,X,Y+10);
	st->obstacles[3] = *zPtr;
	Make_Lava(zPtr,X+80,Y);
	st->obstacles[4] = *zPtr;
	Make_Lava(zPtr,X+70,Y);
	st->obstacles[5] = *zPtr;
	Make_Lava(zPtr,X+80,Y+10);
	st->obstacles[6] = *zPtr;
	Make_Lava(zPtr,X+70,Y+10);
	st->obstacles[7] = *zPtr;
	Make_Lava(zPtr,X,Y+70);
	st->obstacles[8] = *zPtr;
	Make_Lava(zPtr,X,Y+80);
	st->obstacles[9] = *zPtr;
	Make_Lava(zPtr,X+10,Y+70);
	st->obstacles[10] = *zPtr;
	Make_Lava(zPtr,X+10,Y+80);
	st->obstacles[11] = *zPtr;
	Make_Lava(zPtr,X+70,Y+70);
	st->obstacles[12] = *zPtr;
	Make_Lava(zPtr,X+70,Y+80);
	st->obstacles[13] = *zPtr;
	Make_Lava(zPtr,X+80,Y+70);
	st->obstacles[14] = *zPtr;
	Make_Lava(zPtr,X+80,Y+70);
	st->obstacles[15] = *zPtr;
	Make_Skelebomber(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+50,0);
	st->enemies[1] = *zPtr;
	Make_Bear(zPtr,X+30,Y+30,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+30,0);
	st->enemies[3] = *zPtr;
	Make_Bear(zPtr,X+50,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+40,0);
	st->enemies[5] = *zPtr;
	Make_Bear(zPtr,X+50,Y+30,0);
	st->enemies[6] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+40,0);
	st->enemies[7] = *zPtr;
	Make_Bear(zPtr,X+30,Y+50,0);
	st->enemies[8] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}

//**********Make_Lava5Stage******************** COPY OF LAVA3
//Makes a stage into a Lava5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava5Stage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 15;	st->totalObstacles = 0;
	Make_Skelebomber(zPtr,X+40,Y+40,0);
	st->enemies[0] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+30,0);
	st->enemies[1] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skeleton(zPtr,X+30,Y+50,0);
	st->enemies[3] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+50,0);
	st->enemies[4] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+50,0);
	st->enemies[5] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+40,0);
	st->enemies[6] = *zPtr;
	Make_Skeleton(zPtr,X+50,Y+30,0);
	st->enemies[7] = *zPtr;
	Make_Skeleton(zPtr,X+40,Y+30,0);
	st->enemies[8] = *zPtr;
	Make_Skelebomber(zPtr,X,Y,0);
	st->enemies[9] = *zPtr;
	Make_Skeleton(zPtr,X+10,Y,0);
	st->enemies[10] = *zPtr;
	Make_Skeleton(zPtr,X,Y+10,0);
	st->enemies[11] = *zPtr;
	Make_Skelebomber(zPtr,X+80,Y,0);
	st->enemies[12] = *zPtr;
	Make_Skeleton(zPtr,X+70,Y,0);
	st->enemies[13] = *zPtr;
	Make_Skeleton(zPtr,X+80,Y+10,0);
	st->enemies[14] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}

//**********Make_Ganon1BossStage********************
//Makes a stage into a Ganon1 Boss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ganon1BossStage(stage_t *st, uint8_t exits, uint8_t floor){
	st->rSize = 0;
	st->pSize = 0;
	st->totalEnemies = 4;	st->totalObstacles = 12;
	Make_Lava(zPtr,X+20,Y+20);
	st->obstacles[0] = *zPtr;
	Make_Lava(zPtr,X+30,Y+20);
	st->obstacles[1] = *zPtr;
	Make_Lava(zPtr,X+20,Y+30);
	st->obstacles[2] = *zPtr;
	Make_Lava(zPtr,X+50,Y+20);
	st->obstacles[3] = *zPtr;
	Make_Lava(zPtr,X+60,Y+20);
	st->obstacles[4] = *zPtr;
	Make_Lava(zPtr,X+60,Y+30);
	st->obstacles[5] = *zPtr;
	Make_Lava(zPtr,X+30,Y+60);
	st->obstacles[6] = *zPtr;
	Make_Lava(zPtr,X+20,Y+60);
	st->obstacles[7] = *zPtr;
	Make_Lava(zPtr,X+20,Y+50);
	st->obstacles[8] = *zPtr;
	Make_Lava(zPtr,X+60,Y+60);
	st->obstacles[9] = *zPtr;
	Make_Lava(zPtr,X+60,Y+50);
	st->obstacles[10] = *zPtr;
	Make_Lava(zPtr,X+50,Y+60);
	st->obstacles[11] = *zPtr;
	Make_Ganon1Boss(zPtr,X+40,Y,0);
	st->enemies[0] = *zPtr;
	Make_Bear(zPtr,X,Y+40,0);
	st->enemies[1] = *zPtr;
	Make_Bear(zPtr,X+80,Y+40,0);
	st->enemies[2] = *zPtr;
	Make_Skelebomber(zPtr,X+40,Y+40,0);
	st->enemies[3] = *zPtr;
	st->cleared = 0;	st->exits = exits;
	st->type = 0;
	st->image = lavaObstacle;
}
