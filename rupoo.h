//rupoo.h

#ifndef rupoo_H
#define rupoo_H
#include <stdint.h>
#include <stdlib.h>
#include "zStructs.h"
#include "images.h"
#include "entity.h"

//**********Make_Rup********************
//creates a rupoo object that can be picked up to get $
//	dimensions are 10x10
//inputs: z a pointer to the rupoo to reinitialize
//				e a pointer to the entity that dropped it
//outputs: none
void Make_Rup(entity_t *e);

#endif
