//Copyright(c) 1996 Bloodlust Software All rights reserved
//Main Control functions
#include <stdlib.h>
#include <string.h>


#define ANIMATOR

#include "cf.h"

//get series
#include "\a32\main.sdf"


int object::main_stance(int instat)
 {
  if (instat&ID_LEFT)  d^=1;

//  if (y<0) return falldown;

  if (in && in->but&ID_BUT1) return jumpup;
  if (in && in->but&ID_BUT3) return climb;

  if (in && (in->but&ID_BUT0)) //button 0 is pushed
   if (!(in->but&ID_BUT2)) //not using holding button
   {
    if (instat&ID_RIGHT)
      {
       if (instat&ID_UP)   return walk_FUR;
       if (instat&ID_DOWN) return walk_FDR;
       return walk_FR;
      }
    if (instat&ID_UP)   return stance_FU;
    if (instat&ID_DOWN) return duck; //duck_f
    return stance_FR;
   }  else //holding button down
   {
    if (instat&ID_RIGHT)
      {
       if (instat&ID_UP)   return stance_FUR;
       if (instat&ID_DOWN) return stance_FDR;
       return stance_FR;
      }
    if (instat&ID_UP)   return stance_FU;
    if (instat&ID_DOWN) return stance_FD;
    return stance_FR;
   }
   

 //fire button not pushed
  if (instat&ID_RIGHT)
    {
     if (instat&ID_UP)   return walk_UR;
     if (instat&ID_DOWN) return walk_DR;
     return(walk_R);
    }
  if (instat&ID_LEFT)  return turn_around;
  if (instat&ID_UP)    return stance_U;

  if (instat&ID_DOWN) return stancetoduck;
  if (csnum==inactivity1 || (!(uu&127) && !random(20))) return inactivity1;
  return stance;
 };


int object::main_duck(int instat)
 {
   if (instat&ID_LEFT)  d^=1;
//   if (y<0) return falldown;
   
   if (in && in->but&ID_BUT1) return jumpup;
   
   if (!(instat&ID_DOWN)) return ducktostance;
   if (in && in->but&ID_BUT0)
    {
     if (in->but&ID_BUT2) //holding button
        return stance;
     return duck_F;
    } 
   if (csnum==duck_F) return duck;
   return -1;
 }


int object::main_walk(int instat)
 {
  if (instat&ID_LEFT)  d^=1;  //turn around
//  if (y<0) return falldown;  
  if (in && in->but&ID_BUT1) return jumpup;
  
  if (!(instat&ID_RIGHT)) return stance; //go back to stance if not pushing right

  move(fptr->dx,0,0);

  //they wanna fire while we runnin'
  if (in && in->but&ID_BUT0)
   { //go to fire series but same frame
    if (in->but&ID_BUT2) //holding button
      return main_stance(instat);
    int tempcf=cf,tempdur=dur;
    startseries(main_walkfire(instat));
    cf=tempcf; resetfptr();
    dur=tempdur;
    return -1;
   }

  //fire button not pushed
  if (instat&ID_RIGHT)
   {
    if (instat&ID_UP)   return walk_UR;
    if (instat&ID_DOWN) return walk_DR;
    return (walk_R);
   }
  if (instat&ID_LEFT)  return(turn_around);
  return stance;
 };



int object::main_walkfire(int instat)
 {
  if (instat&ID_LEFT)  d^=1;  //turn around
//  if (y<0) return falldown;  
  if (!(instat&ID_RIGHT)) return stance; //go back to stance if not pushing right
  if (in && in->but&ID_BUT1) return jumpup;  

  move(fptr->dx,0,0);

  //if button NOT pushed
  if (!(in && in->but&ID_BUT0))
   { //go to regular series but same frame
    int tempcf=cf,tempdur=dur;
    startseries(main_walk(instat));
    cf=tempcf; resetfptr();
    dur=tempdur;
    return -1;
   }


  //fire button IS pushed
  if (in->but&ID_BUT2) //holding button
      return main_stance(instat);
  
  if (instat&ID_RIGHT)
   {
    if (instat&ID_UP)   return walk_FUR;
    if (instat&ID_DOWN) return walk_FDR;
    return (walk_FR);
   }
  if (instat&ID_LEFT)  return(turn_around);

  return stance_FR;
 };



int object::main_jump(int instat)
 {
  if (instat&ID_LEFT)  {d^=1; return -1;} //turn around
  if (instat&ID_RIGHT) move(fptr->dx,fptr->dy,0);
    else               move(0, fptr->dy,0);

  if (in && in->but&ID_BUT3)
   {jumpptr=0; return hang;}

  //move up and down in jump
  if (jumpptr) //needs trajectory
   if (instat&ID_RIGHT) move(jumpptr->dx,jumpptr->dy,0);
       else             move(0, jumpptr->dy,0);

  if (in && (in->but&ID_BUT0)) //button 0 is pushed
  {
  //pointing gun around in jump
  if (instat&ID_UP) 
   {
    if (instat&ID_RIGHT) startjumpseries(jump_FUR); else
    startjumpseries(jump_FU); 
   } else
  if (instat&ID_DOWN) 
   {
    if (instat&ID_RIGHT) startjumpseries(jump_FDR); else
    startjumpseries(jump_FD);
   } else
  startjumpseries(jump_FR);
  }  else //NOT FIRING
      //return to normal jump if necessary
  if ((instat&0xF)==0 || (instat==ID_RIGHT)) //we're just idling
       resumejump();  //go back to jump
     else
  {
  //pointing gun around in jump
  if (instat&ID_UP) 
   {
    if (instat&ID_RIGHT) startjumpseries(jump_UR); else
    startjumpseries(jump_U);
   } else
  if (instat&ID_DOWN) 
   {
    if (instat&ID_RIGHT) startjumpseries(jump_DR); else
    startjumpseries(jump_D);
   } 
  }

 return -1;


}

 

int object::bullet(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  return -1;
 }




int object::randomdur(int instat)
 {
  dur=dur/2+random(dur);  //make random dur
//  move(fptr->dx*random(dur*3),fptr->dy*random(dur*3),fptr->dz*random(dur*3));

  controlfunc=none;
  return -1;
 }


int object::main_climb(int instat)
 {
  move(fptr->dx,fptr->dy,0);
  if (y>=0 && fptr->dy>0) return stance;    
  if (in && in->but&ID_BUT1)
   {
    if (instat&ID_DOWN) return falldown;
    return jumpoffclimb;
   }
   
  //firing
  if (in && in->but&ID_BUT0)
  {
  if (instat&ID_RIGHT)
    {
     if (instat&ID_UP)   return climb_FUR;
     if (instat&ID_DOWN) return climb_FDR;
     return(climb_FR);
    }
  if (instat&ID_LEFT)
    {
     if (instat&ID_UP)   return climb_FUL;
     if (instat&ID_DOWN) return climb_FDL;
     return(climb_FL);
    }
  if (instat&ID_UP)   return climb_FU;
  if (instat&ID_DOWN) return climb_FD;
  return climb_FL;
  }

 //NOT firing
  if (instat&ID_RIGHT)
    {
     if (instat&ID_UP)   return climb_UR;
     if (instat&ID_DOWN) return climb_DR;
     return(climb_R);
    }
  if (instat&ID_LEFT)
    {
     if (instat&ID_UP)   return climb_UL;
     if (instat&ID_DOWN) return climb_DL;
     return(climb_L);
    }
 
  if (instat&ID_UP)    return climbup;
  if (instat&ID_DOWN) return climbdown;
  return climb;
 }


int object::main_hang(int instat)
 {
  if (instat&ID_LEFT)  d^=1; 
  move(fptr->dx,fptr->dy,0);

  if (in && in->but&ID_BUT1)
   {
    if (instat&ID_DOWN) return falldown;
    return jumpoffhang;
   }
  

  if (in && (in->but&ID_BUT0)) //button 0 is pushed
   {
    if (instat&ID_RIGHT)
      {
       if (instat&ID_UP)   return hang_FUR;
       if (instat&ID_DOWN) return hang_FDR;
       return hang_FR;
      }
    if (instat&ID_UP)   return hang_FU;
    if (instat&ID_DOWN) return hang_FD;
    return hang_FR;
   }

 //fire button not pushed
  if (instat&ID_RIGHT)
    {
     if (instat&ID_UP)   return hang_UR;
     if (instat&ID_DOWN) return hang_DR;
     return(hangwalk);
    }
  if (instat&ID_UP)    return hang_U;
  if (instat&ID_DOWN) return hang_D;
  return hang;
 }










