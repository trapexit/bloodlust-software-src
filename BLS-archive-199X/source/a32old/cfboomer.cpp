//Copyright(c) 1996 Bloodlust Software All rights reserved
//Enemy Control functions
#include <stdlib.h>
#include <string.h>

#define ANIMATOR

#include "cf.h"

//get series
#include "\a32\boomer.sdf"

int object::enemy_boomer(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  if (!in) return -1;

  if (instat&ID_LEFT) d^=1;

  if (instat&ID_BUT1) return pose1;

  if (instat&ID_BUT0)
   {
    if (instat&ID_DOWN) return duckchuck;
    return Bombchuck;
   }
  if (instat&ID_DOWN) return duck;
  if (csnum==duck) return Stance;
  if (instat&ID_RIGHT) return walk_R;
  return Stance;
 }







