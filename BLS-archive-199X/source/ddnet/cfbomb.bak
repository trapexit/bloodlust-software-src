//Copyright(c) 1996 Bloodlust Software All rights reserved
//Main Control functions
#include <stdlib.h>
#include <string.h>


#define ANIMATOR

#include "glib.h"
#include "motion.h"
#include "input.h"
#include "smix.h"

#include "object.h"
#include "effect.h"


//get series
#include "\a32\main.sdf"


int object::main_control(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);

  //check the motions
  motiondef *mdf=in->readmotions(MO_STANCE);
  if (mdf) {mpriority=mdf->priority; return mdf->series;} //do da motion dude
  
  if (instat&ID_BUT1) //button 1 is pushed
    {
     if (instat&ID_RIGHT) return jump_forward;
     if (instat&ID_LEFT)  return jump_backward;
     return jump_up;
    }
  if (instat&(ID_RIGHT|ID_UP|ID_DOWN)) return(walk_forward);
  if (instat&ID_LEFT)  return (walk_backward);
  return -1;
 };

int object::main_walk(int instat)
 {
  //check the motions
  motiondef *mdf=in->readmotions(MO_STANCE);
  if (mdf) {mpriority=mdf->priority; return mdf->series;} //do da motion dude

  if (instat&ID_BUT1) //button 1 is pushed
    {
     if (instat&ID_RIGHT) return jump_forward;
     if (instat&ID_LEFT)  return jump_backward;
     return jump_up;
    }

  if (!(instat&0xF)) return stance; //go back to stance


  if (instat&ID_UP)   move(0,-fptr->dy,0);
  if (instat&ID_DOWN) move(0, fptr->dy,0);
  
  if (instat&ID_RIGHT)
   {
    move(fptr->dx,0,0);
    return (walk_forward);
   }

  if (instat&ID_LEFT)
   {
    move(fptr->dx,0,0);
    return (walk_backward);
   }

   
  return -1;
 };

int object::main_jump(int instat)
 {
  //check the motions
  motiondef *mdf=in->readmotions(MO_JUMP);
  if (mdf)
   {
    //set jump trajectory
    jumpptr=fptr; jumpdur=dur; jumpcf=cf; jumpnf=nf;
    //do the jumping attack
    mpriority=mdf->priority;
    return mdf->series; 
   }

  if (instat&ID_UP)   move(fptr->dx,-fptr->dy,fptr->dz); else
  if (instat&ID_DOWN) move(fptr->dx, fptr->dy,fptr->dz); else
                       move(fptr->dx, 0,fptr->dz);

  return -1;
 }



int object::main_run(int instat)
 {
  //check the motions
  motiondef *mdf=in->readmotions(MO_RUN);
  if (mdf) {mpriority=mdf->priority; return mdf->series;} //do da motion dude


 if (csnum!=runbackward)
//    if (!(instat&ID_LEFT)) return stance; //stop running if not held
//       else
     {
      if (!(instat&ID_RIGHT)) return post_runforward; //stop running if not held
      if (instat&ID_BUT1) //button 1 is pushed, jump
         return running_jump;
     }
   

  if (instat&ID_UP)   move(fptr->dx,-fptr->dy,fptr->dz); else
  if (instat&ID_DOWN) move(fptr->dx, fptr->dy,fptr->dz); else
                       move(fptr->dx, 0,fptr->dz);
  
  return -1;
 };


int object::main_attack(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);

  //check the motions
  motiondef *mdf=in->readmotions(MO_STANCE);
  if (mdf && cf<2 && mdf->priority>mpriority)
       {
        mpriority=mdf->priority;
        return mdf->series; //do the higher priority motion 
       }
 return -1;
 }


int object::main_jumpattack(int instat)
 {
  //check the motions
  in->readmotions(0);

  //move as defined by the attack frame
  move(fptr->dx,fptr->dy,fptr->dz);

  //move up and down in jump
  if (jumpptr) //needs trajectory
  {
   if (instat&ID_UP)   move(jumpptr->dx,-jumpptr->dy,jumpptr->dz); else
   if (instat&ID_DOWN) move(jumpptr->dx, jumpptr->dy,jumpptr->dz); else
                       move(jumpptr->dx, 0,jumpptr->dz);
  }

  return -1;
 }

