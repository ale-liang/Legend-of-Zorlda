//stage.h

#ifndef STAGE_H
#define STAGE_H
#include <stdint.h>
#include <stdlib.h>
#include "zStructs.h"
#include "entity.h"
#include "proj.h"
#include "rupoo.h"
#include "player.h"



//**********Check_Move********************
//Checks to see if the new image can be drawn without overriding an obstacle or wall
//inputs: x the x coord of the bottom left corner 
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Move(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY);

//**********Check_Walls********************
//Checks to see if the given variables would hit a wall
//inputs: x the x coord of the bottom left corner
//				y the y coord of the bottom left corner
//				dimX the width of the image
//				dimY the height of the image
//outputs: 1 if the new image can be drawn
//				 0 if the new image cannot be drawn
int Check_Walls(uint32_t x, uint32_t y, uint8_t dimX, uint8_t dimY);
	
//**********Draw_Borders_Up********************
//Draws the top border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Up(void);

//**********Draw_Borders_Right********************
//Draws the right border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Right(void);

//**********Draw_Borders_Down********************
//Draws the bottom border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Down(void);

//**********Draw_Borders_Left********************
//Draws the left border of the stage
//inputs: none
//outputs: none
void Draw_Borders_Left(void);

//**********Draw_borders********************
//Draws the 4 walls of the stage
//	stage boundaries are x:[19,109] y:[59,149]
//	each block is 10x10 and drawn in the lower left corner
//inputs: none
//outputs: none
void Draw_Borders(void);

//**********Make_Stair********************
//Makes a stage into a room to ascend to the next floor
//inputs: st the stage to edit
//ooutputs: none
void Make_Stairs(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Shop********************
//Makes a stage into a room to purchase items with rupoos
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//outputs: none
void Make_Shop(stage_t *st, uint8_t exits, uint8_t floor);
	
//**********Next_Rooms*********************
//changes the nextRooms array of a stage
//	next room changes rooms based off index in floor array
//inputs: the 4 numbers for corresponding rooms (0xFF if DNE)
//outputs: none
void Next_Rooms(stage_t *st, uint8_t n, uint8_t e, uint8_t s, uint8_t w);

void Make_TestStage1(stage_t* st);

void Make_TestStage2(stage_t* st);

void Make_TestStage3(stage_t* st);

void Make_TestStage4(stage_t* st);

void Make_TestStage5(stage_t* st);
//DATABASE FOR STAGES WILL BE HERE
void Make_TestStage(stage_t* st);

//**********Make_Bush1Stage********************
//Makes a stage into a bush1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush1Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Bush2Stage********************
//Makes a stage into a bush2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush2Stage(stage_t* st, uint8_t exits, uint8_t floor);

//**********Make_Bush3Stage********************
//Makes a stage into a bush3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush3Stage(stage_t* st, uint8_t exits, uint8_t floor);

//**********Make_Bush4Stage********************
//Makes a stage into a bush4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush4Stage(stage_t* st, uint8_t exits, uint8_t floor);

//**********Make_Bush5Stage******************** COPY OF BUSH3
//Makes a stage into a bush5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Bush5Stage(stage_t* st, uint8_t exits, uint8_t floor);

//**********Make_SlimeBossStage******************** (apart of bush floor)
//Makes a stage into a slime boss stage room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_SlimeBossStage(stage_t* st, uint8_t exits, uint8_t floor);

//**********Make_Water1Stage********************
//Makes a stage into a bush1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water1Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Water2Stage********************
//Makes a stage into a Water2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water2Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Water3Stage********************
//Makes a stage into a Water3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water3Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Water4Stage********************
//Makes a stage into a Water4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water4Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Water5Stage******************** COPY OF WATER3
//Makes a stage into a Water5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Water5Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_OrcBossStage******************** (apart of water floor)
//Makes a stage into a OrcBoss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_OrcBossStage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Brick1Stage********************
//Makes a stage into a Brick1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick1Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Brick2Stage********************
//Makes a stage into a Brick1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick2Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Brick3Stage********************
//Makes a stage into a Brick3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick3Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Brick4Stage********************
//Makes a stage into a Brick4 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick4Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Brick5Stage******************** COPY OF BRICK3
//Makes a stage into a Brick5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Brick5Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_LynelBossStage******************** (apart of brick floor)
//Makes a stage into a OrcBoss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_LynelBossStage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ice1Stage********************
//Makes a stage into a Ice1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice1Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ice2Stage********************
//Makes a stage into a Ice2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice2Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ice3Stage********************
//Makes a stage into a Ice3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice3Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ice4Stage********************
//Makes a stage into a Ice2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice4Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ice5Stage******************** COPY OF ICE3
//Makes a stage into a Ice3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ice5Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_IcegolemBossStage********************
//Makes a stage into a Ice golem Boss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_IcegolemBossStage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Lava1Stage********************
//Makes a stage into a Lava1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava1Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Lava2Stage********************
//Makes a stage into a Lava2 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava2Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Lava3Stage********************
//Makes a stage into a Lava3 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava3Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Lava4Stage********************
//Makes a stage into a Lava1 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava4Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Lava5Stage******************** COPY OF LAVA3
//Makes a stage into a Lava5 room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Lava5Stage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Make_Ganon1BossStage********************
//Makes a stage into a Ganon1 Boss room
//	DOES NOT CHANGE nextRooms ARRAY. MUST CHANGE W Next_Rooms()
//inputs: st the stage to edit
//				exits a 4 bit input w each bit representing an exit
//				floor the floor num used to draw borders
//ooutputs: none
void Make_Ganon1BossStage(stage_t *st, uint8_t exits, uint8_t floor);

//**********Clear_Obstacles********************
//Clears all leftover obstacles
//inputs: none
//outputs: none
void Clear_Obstacles(void);

//**********Redraw_All********************
//Sets all the redraw flags (enemy/obstacle/player)
//input: none
//output: none
void Redraw_All( player_t *lonk);
#endif
