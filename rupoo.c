//rupoo.c

#include <stdint.h>
#include <stdlib.h>
#include "zstructs.h"
#include "rupoo.h"
#include "images.h"
#include "entity.h"

//A single rupoo uninitialized
rupoo_t rZygote;

//the pointer to the unmodified rupoo
rupoo_t *rzPtr = &rZygote;

//**********Make_Rup********************
//creates a rupoo object that can be picked up to get $
//	dimensions are 10x10
//inputs: e a pointer to the entity that dropped it
//outputs: none
void Make_Rup(entity_t *e){
	if (e->dims[0] == 20){ 
		rzPtr->pos[0] = e->pos[0] + 5;
		rzPtr->pos[1] = e->pos[1] - 5;
	}else{ 
		rzPtr->pos[0] = e->pos[0];
		rzPtr->pos[1] = e->pos[1];
	}rzPtr->value = e->coins;
	rzPtr->exists = 1;
	rzPtr->duration = 239;	//duration is set for 240 iterations of clock
	if (e->coins == green) rzPtr->image = grnrupoo;
	else if (e->coins == red) rzPtr->image = redrupoo;
	else if (e->coins == blue) rzPtr->image = blurupoo;
	rzPtr->redraw = 1;
}

