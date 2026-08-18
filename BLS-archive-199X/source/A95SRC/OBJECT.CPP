//Copyright(c) 1996 Bloodlust Software All rights reserved
//Object functions
#include <stdlib.h>
#include <string.h>

#include "r2img.h"
#include "dd.h"

#include "guiroot.h"

#include "region.h"
#include "objdef.h"
#include "object.h"
#include "objspace.h"

#include "input.h"
#include "font.h"
#include "message.h"
#include "a32.h"

//include list of all control functions
#include "\a32\cfunc.lst"


//trajectory
//constructor
jumptrajectory::jumptrajectory(object *to):trajectory(to)
{
 //copy over jump parms
 fptr=o->fptr;
 dur=o->dur;
 cf=o->cf; nf=o->nf; csnum=o->csnum;

// msg.printf(2,"jumptrajectory created");
}

//destructor
jumptrajectory::~jumptrajectory()
{
// if (cf<nf)  msg.printf(2,"trajectory aborted %d/%d",cf,nf);
 if (cf<nf)
 {
 //copy jump parms back
 o->startseries(csnum);
 o->fptr=fptr;
 o->dur=dur;
 o->cf=cf; o->nf=nf; //o->csnum=csnum;
 }
// o->print();
// o->resetfptr();
}

void jumptrajectory::tick()
{
 //advance trajectory frames
 if (dur>0) dur--;
  else
   {
    cf++;
    if (cf>=nf) //we completed jump...but attack is still going on
      {
        o->startseries(o->od->sd[csnum].next); //abort attack
       // msg.printf(2,"end of trajectory reached");
        delete this; //no more trajectory
      }      else
      {        //advance to next frame
       fptr=(frame *) (((char *)fptr)+fptr->size);
       dur=fptr->dur;
      }
   }
}

void jumptrajectory::move(int instat)
{
 if (instat&ID_RIGHT) o->move(fptr->dx,fptr->dy,0);
     else             o->move(0, fptr->dy,0);
}

int shaketrajectory::oshake[8]={0,1,0,2,-1,0,-2,0};

//---------------------

//cheapo control function
int object::none(int instat)
 {
  instat=instat;
  move(fptr->dx,fptr->dy,fptr->dz); //just move
  return -1; //no series change
 };



//spawn object
object *object::spawn(int objnum,int series,int tx,int ty,int tz, int td)
{
 if (!osp) return 0;
 return new object(osp->odf[objnum],series,tx,ty,tz,td,this);
}


#ifdef ANIMATOR
void object::print()
{
 msg.printf(1,"%s %d/%d",cs->name,cf,nf);
}
#endif

extern int blah;
frame *object::resetfptr()
{
if (!cs) return 0;
fptr=cs->first; nf=cs->nf;
if (!fptr) {dur=0; return 0;}

//uchar i=cf;
//if (i>=nf) i=nf-1;
for (int i=0; i<cf; i++)  //skip ahead to frameptr
  fptr=(frame *) (((char *)fptr)+fptr->size);
dur=fptr->dur;
return fptr;
}



void object::play(int s)
{
 playing=1; playrepeat=0;  //play it
 startseries(s);   //start up animation
 basey=getbasey();
}

void object::playlooped(int s)
{
 playing=1; playrepeat=1;  //play it
 startseries(s);   //start up animation
 basey=getbasey();
}

void object::activate(int s)
{
 if (!od->loaded)
  {
   active=1; csnum=s;
   return;
  }

 if (in) in->reset();
 intersect=0;
 if (r[0]) delete r[0];
 if (r[1]) delete r[1];
 if (r[2]) delete r[2];

  //reset energy
 invincible=0;
 if (od->sd[0].energy) energy=od->sd[0].energy;
                 else invincible=1;
 if (traj) delete traj; traj=0;

 //activate object
 active=1; updated|=OU_ALL;
 play(s);
}

void object::stop()
{
 if (r[0]) delete r[0];
 if (r[1]) delete r[1];
 if (r[2]) delete r[2];
 playing=0; playrepeat=0; //stop playing
 active=0; //no longer active either
 if (traj) {delete traj; traj=0;}
}

//kills this object,sets kill flag
void object::kill()
{
 fptr=0;  playing=active=0; killme=1;
 if (osp)
  {
   if (osp->p==this) osp->p=0;
   osp->needdelete++;
  }
 root->refresh(GUIRFR_OBJSPACE,osp);
}

//extern int seriesstarts;
int object::startseries(int seriesnum)
{
//if suicide
if (seriesnum==SP_SUICIDE)  {kill(); return -1;}
//if lastforever
if (seriesnum==SP_LASTFOREVER) {cf=nf-1; lastforever=1; return -1;}
lastforever=0;

csnum=seriesnum;      //number of series
if (!od->loaded) {fptr=0; return 0;}  //if not loaded, hey, lets get the fuck out of here
cs=&od->sd[seriesnum]; //pointer to series

nf=cs->nf;  //store number of frames
if (!nf) {controlfunc=0; fptr=0; cf=0; return 0;}

controlfunc=object::cfunclist[cs->cfunc]; //get pointer to the control function.

cf=0; fptr=cs->first; //reset to first frame of series
if (playing)  effect();

dur=fptr->dur;
updated|=OU_SERIES|OU_FRAME;
return nf;
}

//finds a series from a character name
int object::findseries(char *seriesname)
{
series *s=od->sd;
for (int i=0; i<od->nums; i++,s++)
 if (!stricmp(s->name,seriesname))  return i;
return -1;
}

//starts series from a character name
int object::startseries(char *seriesname)
{
 int s=findseries(seriesname);
 if (s==-1) return 0;

 return startseries(s);
}


//object constructor
object::object(objectdef *objdef,int s,int tx,int ty,int tz,int td, object *c)
 :osp(0)
{
next=prev=0;
fptr=0;      //no frame
playing=active=0; killme=0;
net=0; //this is not a net object (presumably)

r[0]=r[1]=r[2]=0;
slope=0;

layer=0;

od=objdef;       //bind to objectdef
onum=od->onum;   //store ordinal object type
creator=c;       //set creator

//initialize position
x=tx<<16;
y=ty<<16;
z=tz<<16;
d=td;
updated=OU_X|OU_Y|OU_Z; //position changed 421
traj=0;

//bind to null input
in=NULL;

//load object
od->load(); //load objectdef into memory (if not loaded already, inc ref count)

//link to objectspace
od->osp->addobject(this);

//activate object
if (s!=-1) activate(s);
}

object::~object()
{
 //unbind from input
in->unbind();
 //dereference object
od->kill();

if (r[0]) delete r[0];
if (r[1]) delete r[1];
if (r[2]) delete r[2];
if (traj) delete traj;

 //unlink from objectspace
if (osp) osp->removeobject(this);
}



int object::hitground(int groundy)
{
 if (y<=groundy || (cs->parm&SP_NOGROUND)) return 0; //this object doesn't hit ground

 y=groundy; updated|=OU_Y;
 if (traj) delete traj; //stop jump

 //go to next frame with no y movement
 do
 {
  hiteffect(INTC(x),INTC(y),0); //play hiteffect of this frame
 } while (!advanceframe() && fptr->dy>0);

 return 1;
}

//make this character fall (if above the bg ground level)
void object::falldown()
{
 if  (y<basey) //nothing below us
  {
   if (!(cs->parm&SP_NOFALLDOWN))
     {
      if (cs->falldown) startseries(cs->falldown);
                   else startseries("falldown");
      return;
     }
   if ((cs->parm&SP_MOVEDOWN) && !traj)
    move(0,2<<8,0); //make us move down
  }
}

void object::flip()
{
 d^=1;
 updated|=OU_X;
}





//moves absolutely with pixel based coords
void object::moveabs(int tx, int ty, int tz)
{
 //move X
if (tx)
  { x+=tx; updated|=OU_X;
    if (y==basey)  //we're on a slope
    {
     getbasey();
     int dy=basey-y;  //get change in slope
     if (dy< (-3<<16)) //too high an incline....scootback
       {x-=tx; getbasey();}
     else
      if (dy<(3<<16)) //just follow slope naturally
       if (basey!=0x7FFF<<16) y=basey;//follow slope
                   else falldown();
    } else getbasey();
  }
if (ty)
 {
  y+=ty; updated|=OU_Y;
  hitground(basey);
  updated|=OU_Y; //see if hit ground
  if (!(updated&OU_X)) getbasey();
 }
if (tz) { z+=tz; updated|=OU_Z; }

//if (updated && r[R_IMPERM] && r[R_IMPERM]->platnum) r[R_IMPERM]->moveaboveobjects(tx,ty,tz);
}




void object::move(int tx, int ty, int tz)
{
 if (!d) moveabs(tx<<8,ty<<8,tz<<8);
   else  moveabs(-tx<<8,ty<<8,tz<<8);
}




void object::resort()
{
if (prev && (z<prev->z)) // || (z==prev->z && (y>prev->y)))// || (y==prev->y && x<prev->x)) )) ) //should be moved up in list
 {osp->swap(prev,this); resort(); return;}

if (next && (z>next->z) )// || (z==next->z && (y<next->y)))  || (y==next->y && x>next->x)) )) )//should be moved up in list
 {osp->swap(this,next); prev->resort(); return;}
}



//returns 0 if advanced frame, returns 1 if series completed
int object::advanceframe()
{
 //we have completed a frame
 cf++;
 updated|=OU_FRAME;
 if (cf>=nf) //gone through all frames
  {        //done with series
   if (active) //if it's an active object
     {
      //initiate next series
      if (!startseries(cs->next)) {fptr=0; playing=active=0;} //if no frames, stop playing
     } else //it's just a played series
     {
      if (!playrepeat) stop();
       startseries(csnum);
     }
     return 1; //series completed
    }
   else
    {        //advance to next frame
     fptr=(frame *) (((char *)fptr)+fptr->size);
     dur=fptr->dur;
     effect(); //do the extra effect thingees
     return 0; //just next frame
    }
}



//extern int playing;
void object::tick()
{
 if (!fptr) return; //must leave if no frame

 if (!lastforever)
 {
  if (traj) traj->move(in->getstat());
  if (traj) traj->tick();


 //call control func for this series
 if (active && controlfunc) //do control function if active
  {
   int tempcs=(this->*(this->controlfunc))(in->getstat());
   if (tempcs!=-1 && tempcs!=csnum) startseries(tempcs); //start up new series

  } else none(0);


 //dur
 if (dur>0) dur--;
  else advanceframe();

 } else falldown(); //falldown if lastforever


 //see if we are attacking anyone
// r[R_ATTACK]->checkattack();
 //see if we are pushing/standing on/etc
// r[R_IMPERM]->checkimperm();
//falldown();

}


int object::loseenergy(int e)
{
if (!active || invincible) return 0;
energy-=e;
      /*
if (energy<0)
  {
   if (energy>=-5 || !startseries("dieexplode")) startseries("die");
   return 1;
  }     */
return 0;
}


int oshake[8]={0,1,0,2,-1,0,-2,0};

void object::draw(char *dest)
{
 if (!fptr) return;

 _disable();
 int tx=(x>>16);
 int ty=(y+z)>>16;
 register image *i=fptr->getimgptr();
 int j=fptr->numimages;
 _enable();

 if (!d)
 {
  for (; j>0; j--)
  {
   if ((!i->exclusive && i->layer<=layer) || (i->exclusive && i->layer==layer))
    od->id[i->index]->draw(dest,tx+i->dispx,ty+i->dispy,i->orient);
   i++;
  }
 } else
 {
  for (; j>0; j--)
  {
   if ((!i->exclusive && i->layer<=layer) || (i->exclusive && i->layer==layer))
   {
    IMG *iptr=od->id[i->index];
    iptr->draw(dest,tx-i->dispx-iptr->xw,ty+i->dispy,0x2^i->orient);
   }
   i++;
  }
 }

 char s[30];
 font[0]->draw(itoa(intersect,s,10),dest,tx,ty);
 if (r[0] && r[0]->platform)
 font[0]->draw(itoa((int)r[0]->platform,s,16),dest,tx,ty+10);
}

void object::drawshadow(char *dest)
{
 if (!fptr) return;
 if(!(cs->parm&SP_NOSHADOW))     //and shadow exists
  {
   #ifdef ANIMATOR
   IMG *s=this!=osp->p ? shadow : a95vol.shadowsel;
   s->draw(dest,(x>>16)-s->xw/2,((basey+z)>>16)-s->yw/2,0);
   #else
//   shadow->drawmap(*shadowmap,dest,(x>>16)-shadow->xw/2,((basey+z)>>16)-shadow->yw/2);
   #endif
  } else //draw dot shadow
  {
  // if (!t->active)
  // if (uu&16) drawpixel(t->x>>16,t->z>>16,3);
  }
}


void object::effect()
{
 if (!fptr || !fptr->numextra) return;
 class effect *t=fptr->geteffectptr();

 for (int i=fptr->numextra; i>0; i--,t=t->next())
  if (!t->onhit) t->trigger(this);
}


//do all hit effects, centering around absolute x,y point in z axis
void object::hiteffect(int hx,int hy,int hz)
{
return;
}

void object::sound(int n)
{
 if (n>=od->numsounds || n<0) return;

 #ifdef DOS
 if (!od->sounds) return;
 playsound(od->sounds[n]);
 #endif
 #ifdef WIN95
 if (!od->dsounds) return;
 playsound(od->dsounds[n],((x>>16)-(osp->width()/2))*8000/osp->width());
 #endif
}


