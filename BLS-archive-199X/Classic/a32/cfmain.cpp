//Copyright(c) 1996 Bloodlust Software All rights reserved
//Main Control functions
#include <stdlib.h>
#include <string.h>

#include "objspace.h"
#include "region.h"
#include "effect.h"
#include "bg.h"
#include "object.h"
#include "dd.h"


#include "message.h"

//get series
#include "\a32\main.sdf"

int object::doclimbwall(int instat)
{
 if (traj) delete traj;

 startseries(climb); //we should have new region by now
 if (!r[0]) return -1;
 r[0]->getabsregion();

 //position against wall....
 x=(wall->x2<<16);
 if (!d) x-=(r[0]->e->r.p2.x-1)<<16;
    else x+=(r[0]->e->r.p2.x)<<16;

 startseries(main_climb(instat));
 return -1;
}


int object::dohangceiling(int instat)
{
 if (traj) delete traj;

 startseries(hang); //we should have new region by now
 if (!r[0]) return -1;
 r[0]->getabsregion();

 //position against ceiling....
 y=ceiling->calcy(x>>16)<<16;
 y-=(r[0]->e->r.p1.y)<<16;

 startseries(main_hang(instat));
 return -1;
}




extern int blah;
int object::main_stance(int instat)
 {
//  blah++;
  if (instat&ID_LEFT)  flip();

  if (in && in->but&ID_BUT1) return jumpup;
  if (in && in->but&ID_BUT3) return findseries("magic");

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
  if (csnum>=inactivity1 && csnum<=inactivity3) return csnum; //stay in inactivity...
   else
    if (!(uu&1023)) // && !random(25)) //do inactivity
     {
      int s=inactivity1+random(3); //pick one....
      if (od->sd[s].nf) return s;   //do it
     }
  return stance;
 };


int object::main_duck(int instat)
 {
   if (instat&ID_LEFT)  flip();

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
  if (instat&ID_LEFT)  flip();  //turn around
  if (in && in->but&ID_BUT1) return jumpup;

  if (!(instat&ID_RIGHT)) return stance; //go back to stance if not pushing right

  move(fptr->dx,0,0);

  if (wall) //we walked into a wall....
    if (r[0]->r.y1>=wall->y1) //if we're not taller than it...
     return doclimbwall(instat);

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
  if (instat&ID_LEFT)  flip();  //turn around
  if (!(instat&ID_RIGHT)) return stance; //go back to stance if not pushing right
  if (in && in->but&ID_BUT1) return jumpup;

  move(fptr->dx,0,0);

  if (wall) //we walked into a wall....
    if (r[0]->r.y1>=wall->y1) //if we're not taller than it...
     return doclimbwall(instat);

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
  if (csnum==jumpoffclimb)
   {
    if (!(instat&ID_LEFT)) move(fptr->dx,fptr->dy,0);
                  else move(0, fptr->dy,0);
    return -1;
   }

  if (instat&ID_LEFT)  {flip(); return -1;} //turn around
  if (instat&ID_RIGHT) move(fptr->dx,fptr->dy,0);
                  else move(0, fptr->dy,0);

   if (ceiling) //we jumped into a ceiling
     return dohangceiling(instat);
                  
   if (instat&ID_RIGHT)
   if (wall) //we jumped into a wall....
      if (r[0]->r.y1>=wall->y1) //if we're not taller than it...
        return doclimbwall(instat);



//  if (in && in->but&ID_BUT3) {if (traj) delete traj; return hang;}


  //move up and down in jump
  int s=-1;
  if (in && (in->but&ID_BUT0)) //button 0 is pushed
  {
  //pointing gun around in jump
  if (instat&ID_UP)
    if (instat&ID_RIGHT) s=jump_FUR;  else s=jump_FU;
   else
  if (instat&ID_DOWN)
    if (instat&ID_RIGHT) s=jump_FDR;   else  s=jump_FD;
   else s=jump_FR;
  }
   else //NOT FIRING
  {
  //pointing gun around in jump
  if (instat&ID_UP)
    if (instat&ID_RIGHT) s=jump_UR;  else  s=jump_U;
   else
  if (instat&ID_DOWN)
    if (instat&ID_RIGHT) s=jump_DR;  else  s=jump_D;
  }

 if (s==-1)
  {
   if (traj) delete traj; //remove trajectory
  } else
  {
   if (!traj) new jumptrajectory(this);
  }
 return s;


}



int object::bullet(int instat)
 {
  move(fptr->dx,fptr->dy,fptr->dz);
  return -1;
 }




int object::randomdur(int instat)
{
 new movetrajectory(this,0,-0x50+random(0x100),
     -0x10+random(0x20),30);
 controlfunc=&object::none;
 return -1;
}




int object::main_climb(int instat)
 {
  //move(fptr->dx,fptr->dy,0);
  if (fptr->dy)
  {
   if (fptr->dy<0) //going up....
   {
    if (wall)
     if (r[0]->r.y1>=wall->y1) //if we're not taller than it...
       move(0,fptr->dy,0); //move up wall
      else {wall=0; return jumpoffclimb; }//jumpup;} //top of wall

//   if (ceiling) //we climbed into a ceiling
//    {wall=0; return dohangceiling(instat);}

   } else     //going down
   {
    move(0,fptr->dy,0); //move down wall
    if (y>=basey && fptr->dy>0) //hit floor
        return stance;
     if (r[0]->r.y1>=wall->y2) //if we're not taller than it...
       {wall=0; return falldown;}
   }
  }

/*  if (wall && r[0])
  {
   //position against wall....
   x=(wall->x2<<16)-osp->bg->x;
   if (!d) x-=(r[0]->e->r.p2.x-1)<<16;
      else x+=(r[0]->e->r.p2.x)<<16;
  }*/

  if (in && in->but&ID_BUT1 && !(instat&ID_RIGHT))
   {
    if (instat&ID_DOWN) return falldown;
    return jumpup; //jumpoffclimb;
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
  if (instat&ID_DOWN)  return climbdown;
  return climb;
 }


int object::main_hang(int instat)
 {
  if (instat&ID_LEFT)  flip();

  move(fptr->dx,0,0);
  if (ceiling && r[0])
   {
    if (r[0]->r.x1>ceiling->x2 || r[0]->r.x2<ceiling->x1)
       {ceiling=0; return falldown;}
    //position against ceiling....
    y=(ceiling->calcy(x>>16)-r[0]->e->r.p1.y)<<16;
   }

/*  if (instat&ID_RIGHT)
  if (wall) //we hanged into a wall....
     if (r[0]->r.y1>=wall->y1) //if we're not taller than it...
       {ceiling=0; return doclimbwall(instat);}
*/

  if (in && in->stat&ID_BUT1)
   {
    if (!ceiling)
     {
      if (instat&ID_DOWN) return falldown;
      return jumpoffhang;
     } else {ceiling=0;return falldown;}
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










