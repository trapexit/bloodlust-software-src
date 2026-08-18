//Copyright(c) 1996 Bloodlust Software All rights reserved
//Enemy Control functions
#include <stdlib.h>
#include <string.h>

#include "object.h"

//get series
#include "\a32\boomer.sdf"

int object::enemy_boomer(int instat)
{
 move(fptr->dx,fptr->dy,fptr->dz);

 //we are bound to an input device....
 if (in)
  {
   if (instat&ID_LEFT) flip();
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

  /*
 //AI
 switch (strategy)
 {
  case 1: //attack target!
  {
   if (!findnewtarget()) {strategy=0; return stance;}
   if ((getdistx()<0)==(!d)) flip();
   return ;
  }

  default: // walk around aimlessly
//   if (findnewtarget()) {strategy=1; return -1;}
   if (!floor) return -1;

   if (d) {if ((x>>16)<floor->x1+15) flip();} //walking left
     else {if ((x>>16)>floor->x2-15) flip();} //walking right
   return walk_R;
 }
    */
 return -1;
}







