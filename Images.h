// images.h
// contains image buffers for LegendOfZorlda
// Zachary Chin and Alex Liang
// Last Modified: 4/23/2020
// Capture image dimensions from BMP files

#ifndef __images_h
#define __images_h
#include <stdint.h>

// *************************** Images ***************************

//toad image
//width=16 x height=25
extern const unsigned short toad[];

//regular block
// width=10 x height=10
extern const unsigned short block[];

//breakable brick obstacle
//	width=10 x height=10
extern const unsigned short brickObstacle[];

//breakable bush obstacle
//width=10 x height=10
extern const unsigned short bushObstacle[];

//short sword swing images 1-5
//	width=10 x height=x10
extern const unsigned short ssSwingUp1[];
extern const unsigned short ssSwingUp2[];
extern const unsigned short ssSwingUp3[];
extern const unsigned short ssSwingUp4[];
extern const unsigned short ssSwing5[];

//mjolnir item sprites facing up
//	width=10 x height=10
extern const unsigned short mjolnir1[];
extern const unsigned short mjolnir2[];
extern const unsigned short mjolnir3[];
extern const unsigned short mjolnir4[];

//bow item sprites
//	width=10 x height=10
//item number 3
extern const unsigned short bow1[];
extern const unsigned short bow2[];

//black for reset
//	width=10 x height=10
extern const unsigned short black10x10[];

//half a heart or 1 health
//	width=12 x height=12
extern const unsigned short halfheart[];

//a whole heart or 2 health
//	width=12 x height=12
extern const unsigned short heart[];

//black for reset
//	width=16 x height=16
extern const unsigned short black16x16[];

//brokenheart for lost health 
//	width=12 x height=12
extern const unsigned short brokenheart[];

//half brokenheart for lost health
//	width=6 x height=12
extern const unsigned short halfbrokenheart[];

//one for floor
//	width=8 x height=16
extern const unsigned short one[];

//two for floor
//	width=8 x height=16
extern const unsigned short two[];

//three for floor
//	width=8 x height=16
extern const unsigned short three[];

//four for floor
//	width=8 x height=16
extern const unsigned short four[];

//five for floor
//	width=8 x height=16
extern const unsigned short five[];

//FLO = floor
//	width=20 x height=16
extern const unsigned short FLO[];

//potion display
//	width=30 x height=30
extern const unsigned short potion[];

//short sword
//	width=30 x height=30
extern const unsigned short shortsword[];

//regular wooden shield
//	width=30 x height=30
extern const unsigned short woodenshield[];

//deku nut display
//	width=30 x height=30
extern const unsigned short dekunutdisplay[];

//needles display
//	width=30 x height=30
extern const unsigned short needles[];

//biggoron sword display
//	width=30 x height=30
extern const unsigned short biggoronsword[];

//bombchu display
//	width=30 x height=30
extern const unsigned short bombchuDisplay[];

//ice rod display
//	width=30 x height=30
extern const unsigned short icerod[];

//hyrulien shield display
//	width=30 x height=30
extern const unsigned short hyrulienshield[];
//quake medallion display
//	width=30 x height=30
extern const unsigned short quakemedallion[];

//fire rod display
//	width=30 x height=30
extern const unsigned short firerod[];

//heart container display
//	widht=30 x height=30
extern const unsigned short heartcontainer[];

//bow of light display
//	width=30 x height=30
extern const unsigned short bowoflight[];

//shopkeepdisplay
//	width=30 x height=30
extern const unsigned short shopkeepdisplay[];

//regular lonk moving downwards
// width=10 x height=11
extern const unsigned short lonk_down[];

//regular lonk moving upwards
// width=10 x height=11
extern const unsigned short lonk_up[];

//regular lonk moving right
// width=11 x height=10
extern const unsigned short lonk_right[];

//regular lonk moving left
// width=11 x height=10
extern const unsigned short lonk_left[];

//yerraballi players
//	width=10 x height=10
extern const unsigned short yerup[];
extern const unsigned short yerrt[];
extern const unsigned short yerdn[];
extern const unsigned short yerlt[];

//valvano players
//	width=10 x height=10
extern const unsigned short vaup[];
extern const unsigned short vart[];
extern const unsigned short vadn[];
extern const unsigned short valt[];

//slime enemy mob
//width=10 x height=10
extern const unsigned short slimeEnemy[];

//skeleton enemy mob
//width=10 x height=10
extern const unsigned short skeletonEnemy[];

//Ganon 1st form boss
//width=10 x height=10
extern const unsigned short ganon1Boss[];

//Lynel boss
//width=20 x height=20
extern const unsigned short lynelBoss[];

//Lynel Enemy
//width=10 x height=10
extern const unsigned short lynelEnemy[];

//Bear Enemy
//width=10 x height=10
extern const unsigned short bearEnemy[];

//fbow Enemy
//width=10 x height=10
extern const unsigned short fbowEnemy[];

//deku scrub enemy
//width=10xheight=10
extern const unsigned short dekuScrubEnemy[];

//electric bat enemy
//width=10 x height=10
extern const unsigned short ebatEnemy[];

//Ice obstacle block
//width=10 x height=10
extern const unsigned short iceObstacle[];


//lava obstacle block
//width=10 x height=10
extern const unsigned short lavaObstacle[];

//Shopkeeper Obstacle
//width=10 x height=10
extern const unsigned short shopkeepObstacle[];

//Skeleton bomber enemy
//width=10 x height=10
extern const unsigned short skelebomberEnemy[];

//Snake enemy
//width=10 x height=10
extern const unsigned short snakeEnemy[];

//Snowman enemy
//width=10 x height = 10
extern const unsigned short snowmanEnemy[];

//Spider enemy
//width = 10 x height=10
extern const unsigned short spiderEnemy[];

//Squid enemy
//width=10 x height=10
extern const unsigned short squidEnemy[];

//Rock Totem Enemy
//width=10 x height=10
extern const unsigned short totemEnemy[];

//water obstacle block
//width =10 x height = 10
extern const unsigned short waterObstacle[];

//Ice golem boss
//width=20 x height=20
extern const unsigned short icegolemBoss[];


//Ganon2 boss
//width=20 x height =20
extern const unsigned short ganon2boss[];
	
//Crab enemy
//width=20 x height=20
extern const unsigned short crabEnemy[];

//Orc Boss
//width=20 x height=20
extern const unsigned short orcBoss[];

//Slime Boss
//width=20 x height=20;
extern const unsigned short slimeBoss[];

//top of title
//	width=43 x height=2
extern const unsigned short title_top[];

//zorlda of title
//	width=103 x height=20
extern const unsigned short title_zorlda[];

//top of sword of title
//	width=16 x height=2
extern const unsigned short title_sword_top[];

//mid of sword of title
//	width=101 x height=3
extern const unsigned short title_sword_mid[];

//bottom of sword of title
//	width=16 x height=6
extern const unsigned short title_sword_bot[];

//title bottom
//	width=22 x height=21
extern const unsigned short title_bottom[];

//Arrow Projectile Up
//	width=3 x height=9
extern const unsigned short arrowup[];

//Arrow Projectile Down
//	width=3 x height=9
extern const unsigned short arrowdown[];

//Arrow Projectile Right
//	width=9 x height=3
extern const unsigned short arrowright[];;

//Arrow Projectile Left
//	width=9 x height=3
extern const unsigned short arrowleft[];

//Wooden Shield Obstacles
//	width=10 x height=10
extern const unsigned short wshieldup[];
extern const unsigned short wshieldrt[];
extern const unsigned short wshielddn[];
extern const unsigned short wshieldlt[];

//dekunut projectile
//	width=8 x height=8
extern const unsigned short dekunut[];

//needle projectiles
//	width=3/9 x height=9/3
extern const unsigned short needleup[];
extern const unsigned short needlert[];
extern const unsigned short needledn[];
extern const unsigned short needlelt[];
//bomb projectile
//	width=10 x height=10
extern const unsigned short bombProj[];

//hyrulien shield obstacle
//	width=10 x height=10
extern const unsigned short hshieldup[];
extern const unsigned short hshieldrt[];
extern const unsigned short hshielddn[];
extern const unsigned short hshieldlt[];

//fireball projectiles
//	width=10 x height=10
extern const unsigned short fireballup[];
extern const unsigned short fireballrt[];
extern const unsigned short fireballdn[];
extern const unsigned short fireballlt[];

//light projectiles
//	width=3/9 x height=9/3
extern const unsigned short lightup[];
extern const unsigned short lightrt[];
extern const unsigned short lightdn[];
extern const unsigned short lightlt[];

//light bow needs rotation
//	width=10 x height=10
extern const unsigned short lightbowup[];
extern const unsigned short lightbow2[];

//bombchu projectiles
//	width=10 x height=10
extern const unsigned short bombchuup[];
extern const unsigned short bombchurt[];
extern const unsigned short bombchudn[];
extern const unsigned short bombchult[];

//wave effects projectile
//	width=10 x height=10
extern const unsigned short waveup[];
extern const unsigned short wavert[];
extern const unsigned short wavedn[];
extern const unsigned short wavelt[];

//bolt effects projectile
//	width=10 x height=10
extern const unsigned short boltup[];
extern const unsigned short boltrt[];
extern const unsigned short boltdn[];
extern const unsigned short boltlt[];

//explode effect
//	width=10 x height=10
extern const unsigned short explode[];

//black8x8
//	width=8 x height=8
extern const unsigned short black8x8[];
//black3x9
//	width=3 x height=9
extern const unsigned short black3x9[];

//black9x3
//	width=9 x height=3
extern const unsigned short black9x3[];

//green rupoo
//	width=10 x height=10
extern const unsigned short grnrupoo[];

//red rupoo
//	width=10 x height=10
extern const unsigned short redrupoo[];

//blue rupoo
//	width=10 x height=10
extern const unsigned short blurupoo[];

//brown chest
//	width=10 x height=10
extern const unsigned short brnchest[];

//green chest
//	width=10 x height=10
extern const unsigned short grnchest[];

//red chest
//	width=10 x height=10
extern const unsigned short redchest[];

//blue chest
//	width=10 x height=10
extern const unsigned short bluchest[];

//stairs
//	width=10 x height=10
extern const unsigned short stairs[];

//mjolnir item display
//	width=30 x height=30
extern const unsigned short mjolnir[];

//mastersword item display
//	width=30 x height=30
extern const unsigned short mastersword[];

//bomb item display
//	width=30 x height=30
extern const unsigned short bomb[];

//bow item display
//	width=30 x height=3
extern const unsigned short bow[];

//X that signifies that the item is not usable due to delay
//	width=10 x height=10
extern const unsigned short cantUseItem[];

//yerraballi test
extern const unsigned short yerraballipic[];
//valvano test
extern const unsigned short valvano[];
//lunk test
extern const unsigned short lunk[];
	
#endif /* images.h */

