//Copyright(c) 1996 Bloodlust Software All rights reserved
//Enemy Control functions
#include <stdlib.h>
#include <string.h>

#define ANIMATOR

#include "cf.h"



//get series
#include "\a32\jet.sdf"

int object::enemy_jet(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  if (!in) return -1;

  if (instat&ID_LEFT) d^=1;


  if (instat&ID_BUT0)
   {
    if (instat&ID_DOWN) return firedr;
    if (instat&ID_UP) return fireur;
    return firer;
   }
   
  if (instat&ID_DOWN) 
   {
    if (instat&ID_RIGHT) return dr;
    return dn;
   } 
  if (instat&ID_UP) 
   {
    if (instat&ID_RIGHT) return upr;
    return up;
   }
  if (instat&ID_RIGHT) return r;
  return stance;
 }






