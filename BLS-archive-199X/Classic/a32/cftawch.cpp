//Copyright(c) 1996 Bloodlust Software All rights reserved
//Enemy Control functions
#include <stdlib.h>
#include <string.h>

#include "object.h"

//get series
#include "\a32\tawch.sdf"

int object::enemy_tawch(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  if (!in) return -1;

  if (instat&ID_LEFT) flip();

  if (instat&ID_BUT1) return pose1;

  if (in && in->but&ID_BUT0)
   {
    if (instat&ID_UP)   return stance_FUR;
    if (instat&ID_DOWN) return stance_FDR;
    return stance_FR;
   }
  if (instat&ID_RIGHT) return walk_R;

  //if (csnum==walk_R)
   return stance;
  //return -1;
 }












