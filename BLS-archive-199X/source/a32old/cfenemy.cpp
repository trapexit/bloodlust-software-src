//Copyright(c) 1996 Bloodlust Software All rights reserved
//Enemy Control functions
#include <stdlib.h>
#include <string.h>

#define ANIMATOR

#include "glib.h"
#include "motion.h"
#include "input.h"
#include "smix.h"

#include "effect.h"
#include "object.h"

#include "misc.h"

//get series
#include "\a32\pig.sdf"

int object::enemy_pig(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  return -1;
 }



