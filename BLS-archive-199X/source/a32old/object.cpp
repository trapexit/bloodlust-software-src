//Copyright(c) 1996 Bloodlust Software All rights reserved
//Object functions
#include <i86.h>
#include <stdlib.h>
#include <string.h>


#define ANIMATOR

#include "types.h"
#include "r2img.h"
#include "input.h"
#include "smix.h"
#include "misc.h"

#include "effect.h"
#include "object.h"

//include list of all control functions
#include "\a32\cfunc.lst"

int object::none(int instat)
 {
  instat=instat;
  move(fptr->dx,fptr->dy,fptr->dz); //just move
  return -1; //no series change
 };

void mprintf(char *format,...);
extern volatile int needdelete;
extern object *o; //list of all objects
extern objectdef *odf;

int regionchecks=0,regionintersects=0,seriesstarts;



//adds object to main list
void addobject(object *a)
{
//find insertion point
for (object *tprev=0,*t=o; t; tprev=t,t=t->next)
  if (t->z>a->z) break;

//a goes after tprev, before t
a->prev=tprev;
a->next=t;
if (tprev) tprev->next=a; else o=a;
if (t)     t->prev=a;
};



void object::play(int s)
{ //store last thing
 #ifdef ANIMATOR
 if (fptr)
 {
  cs->cf=cf;
  oldcsnum=csnum;
 } 
 #endif

 playing=1; playrepeat=0;  //play it
 startseries(s);   //start up animation

 basey=getbasey();
}

void object::playlooped(int s)
{ //store last thing
 #ifdef ANIMATOR
 if (fptr)
 {
  cs->cf=cf;
  oldcsnum=csnum;
 }
 #endif
 
 playing=1; playrepeat=1;  //play it
 startseries(s);   //start up animation

 basey=getbasey();
}

void object::activate(int s)
{
 in->reset();
 if (r[0]) {delete r[0]; r[0]=0;}
 if (r[1]) {delete r[1]; r[1]=0;}
 if (r[2]) {delete r[2]; r[2]=0;}
 intersect=0;
  //reset energy
 invincible=0;
 if (od->sd[0].energy) energy=od->sd[0].energy;
                 else invincible=1;
 jumpptr=0;  shakedur=0;
 
 //activate object
 active=1; updated|=16;
 play(s);     
}    

void object::stop()
{
 if (r[0]) {delete r[0]; r[0]=0;}
 if (r[1]) {delete r[1]; r[1]=0;}
 if (r[2]) {delete r[2]; r[2]=0;}
 playing=0; //stop playing
 active=0; //no longer active either
 shakedur=0;
 startseries(oldcsnum);
 if (fptr)
 {
  cf=cs->cf;
  resetfptr();
 }
}    

//kills this object,sets kill flag
void object::kill()
{
 fptr=0;  playing=active=0; killme=1; needdelete++;
}

int object::startseries(int seriesnum)
{
seriesstarts++;    
//if suicide
if (seriesnum==0xFF)  {kill(); return -1;}
//if lastforever
if (seriesnum==0xFE) {cf=nf-1; lastforever=1; return -1;}
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
//updated|=15;

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

//starts a series but keeps a reference to jump tragectory
int object::startjumpseries(int seriesnum)
{
if (csnum==seriesnum) return 0; //dont repeat
    
if (!jumpptr) //not in some sort of jump...
  {jumpptr=fptr; jumpdur=dur; jumpcf=cf; jumpnf=nf; jumpcsnum=csnum;} //store jump vars
  
return startseries(seriesnum);
}    

//resumes series from jump tragectory
void object::resumejump()
{
if (!jumpptr) return; //no jump
startseries(jumpcsnum);
fptr=jumpptr; dur=jumpdur; cf=jumpcf; nf=jumpnf;
jumpptr=0; 
}    

extern object *p;

//object constructor
object::object(objectdef *objdef,int s,int tx,int ty,int tz,int td, object *c)
{
//memset(this,0,sizeof(object));
//erase all of structure
next=prev=0;
//child=parent=0;
fptr=0;      //no frame
playing=active=0; killme=0;
r[0]=r[1]=r[2]=0; slope=0;

shakedur=0; layer=0; jumpptr=0;
//oldcsnum=0;
 
od=objdef;       //bind to objectdef
onum=od->onum;   //store ordinal object type
creator=c;       //set creator

//initialize position
x=tx<<16; y=ty<<16; z=tz<<16; d=td;
updated=15; //position changed 421
getbasey();

//bind to null input
in=NULL;

if (!onum)  return;  //it's a null object, return

//load object
od->Read(); //read objectdef into memory (if not loaded already)

//activate object
if (s!=-1) activate(s);

//if (!p) p=this;
}

object::~object()
{
 //unbind from input
in->unbind();
 //dereference object
od->Kill();

if (r[0]) {delete r[0]; r[0]=0;}
if (r[1]) {delete r[1]; r[1]=0;}
if (r[2]) {delete r[2]; r[2]=0;}

 //unlink from chain
if (prev) prev->next=next;
if (next) next->prev=prev;
if (this==o) o=next;

//if (parent) parent->child=0; //unlink from parent
//if (child) delete child; //child dies with us
if (this==p) p=0;
}


image *frame::getimgptr()
{
 if (!this) return 0;    
 return (image *) (((char *)this)+sizeof(frame));
}

byte *frame::getextraptr()
{
 if (!this) return 0;    
 return (byte *) (((char *)this)+numimages*sizeof(image)+sizeof(frame));
}

int object::hitground(int groundy)
{
 if (y<=groundy || (cs->parm&SP_NOGROUND)) return 0; //this object doesn't hit ground

 y=groundy; updated|=2;
 resumejump(); //stop jump

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
   if ((cs->parm&SP_MOVEDOWN)) //   if (!jumpptr && !fptr->dy)
    move(0,2<<8,0); //make us move down
  }

}    



void object::move(int tx, int ty, int tz)
{
if (d) tx=-tx;    
 //move X
if (tx)
   {
    x+=tx<<8; updated|=1;
    if (y==basey)  //we're on a slope
    {
     getbasey();
     int dy=basey-y;  //get change in slope
     if (dy< (-3<<16)) //too high an incline....scootback
       {x-=tx<<8; updated&=~1; getbasey();}
     else
      if (dy<(3<<16)) //just follow slope naturally
       if (basey!=0x7FFF<<16) y=basey;//follow slope
                   else falldown();
     
    } else getbasey();
   }

 //move Y
if (ty)
   {
    y+=ty<<8;
    hitground(basey); updated|=2; //see if hit ground
    if (!(updated&1)) getbasey();
   }

 //move Z
if (tz) { z+=tz<<8; updated|=4; }

//update objects standing on us
if (updated && r[R_IMPERM] && r[R_IMPERM]->platnum) r[R_IMPERM]->moveaboveobjects(tx<<8,ty<<8,tz<<8);

}


//moves absolutely with pixel based coords
void object::moveabs(int tx, int ty, int tz)
{
 //move X
if (tx)
  { x+=tx; updated|=1;
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
  y+=ty; updated|=2;
  if (!(updated&1)) getbasey();
 }
if (tz) { z+=tz; updated|=4; }

if (updated && r[R_IMPERM] && r[R_IMPERM]->platnum) r[R_IMPERM]->moveaboveobjects(tx,ty,tz);

}


int swapcount=0;    

//swaps two objects in order
void swap(object *a,object *b)
{
 swapcount++;
 if (!a || !b) return;
     
 if (a->prev) a->prev->next=b;
   else o=b; //set as first now
 if (b->next) b->next->prev=a;

 a->next=b->next;
 b->prev=a->prev;

 b->next=a;
 a->prev=b;
}

void object::resort()
{
//if (!(updated&4)) return; //resort only if z changed

if (prev && (z<prev->z)) // || (z==prev->z && (y>prev->y)))// || (y==prev->y && x<prev->x)) )) ) //should be moved up in list
 {swap(prev,this); resort(); return;}
 
if (next && (z>next->z) )// || (z==next->z && (y<next->y)))  || (y==next->y && x>next->x)) )) )//should be moved up in list
 {swap(this,next); prev->resort(); return;}
}


int object::advanceframe()
{
 //we have completed a frame
 cf++;
 if (cf>=nf) //gone through all frames
  {        //done with series
   if (active) //if it's an active object
     {
      //if jump trajectory, and jump was reinitiated, resume it at last position
       if (jumpptr && cs->next==jumpcsnum) resumejump();             
             else
      //initiate next series
      if (!startseries(cs->next)) {fptr=0; playing=active=0;} //if no frames, stop playing
     } else //it's just a played series
     {
      if (!playrepeat) stop();
        else startseries(csnum);
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
 
 if (shakedur>0) shakedur--;

 if (!lastforever)
 {
 //call control func for this series
 if (active && controlfunc) //do control function if active
  {
   int tempcs=(this->*(this->controlfunc))(in->getstat());
   if (tempcs!=-1 && tempcs!=csnum)
    {
     if (od->sd[tempcs].nf) startseries(tempcs); //start up new series
    }
  } else none(0);

 //advance jumping trajectory frames
 if (jumpptr)
  {
   if (jumpdur>0) jumpdur--;
     else
     {
      jumpcf++;
      if (jumpcf>=jumpnf) //we completed jump...but attack is still going on
       {
        jumpptr=0; //stop trajectory
        startseries(od->sd[jumpcsnum].next); //abort attack
       }      else 
        {        //advance to next frame
         jumpptr=(frame *) (((char *)jumpptr)+jumpptr->size);
         jumpdur=jumpptr->dur;
        } 
     }
  }

 //dur
 if (dur>0) dur--;
  else advanceframe();

 } else falldown(); //falldown if lastforever
 
 //see if we are attacking anyone
 r[R_ATTACK]->checkattack();
 //see if we are pushing/standing on/etc
 r[R_IMPERM]->checkimperm();
 
 updated=0;
}


int object::loseenergy(int e)
{
if (!active || invincible) return 0;
energy-=e;

if (energy<0)
  {
   if (energy>=-5 || !startseries("dieexplode")) startseries("die");
   return 1; 
  }
return 0; 
}    


int oshake[8]={0,1,0,2,-1,0,-2,0};
extern char *screen;
void object::draw()
{
 if (!fptr) return;

 _disable();
 int tx=(x>>16);
 int ty=(y+z)>>16;
 register image *i=fptr->getimgptr();
 int j=fptr->numimages;
 _enable();


 //do object shaking
 if (shakedur>0)  tx+=oshake[dur&7];
 if (!d)
 { 
  for (; j>0; j--)
  {
   if ((!i->exclusive && i->layer<=layer) || (i->exclusive && i->layer==layer))
    od->id[i->index]->draw(screen,tx+i->dispx,ty+i->dispy,i->orient);
   i++;
  }
 } else
 { 
  for (; j>0; j--)
  {
   if ((!i->exclusive && i->layer<=layer) || (i->exclusive && i->layer==layer))
   {   
    IMG *iptr=od->id[i->index];
    iptr->draw(screen,tx-i->dispx-iptr->xw,ty+i->dispy,0x2^i->orient);
   }
   i++;
  }
 } 

 
}


void object::sound(int n)
{
 if (!sbstat) return;    
 if (n>=od->numsounds || !od->sounds[n]) return;
 
 start_sound(od->sounds[n],onum,255,0);
}

//extra effects
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
int object::E_SOUND_do(e_sound *t)
{
 sound(t->sound); //play sound
 return (sizeof(e_sound));
}

int shakelength[4]={5,25,50,150};
extern int gshakedur;
int object::E_GROUNDSHAKE_do(e_shake *t)
{
 gshakedur=shakelength[t->intensity]; //make ground shake
 return (sizeof(e_shake));
}    

int object::E_OBJECTSHAKE_do(e_shake *t)
{
 shakedur=shakelength[t->intensity];
 return (sizeof(e_shake));
}    


int object::E_GOTO_do(e_goto *t)
{
 startseries(t->s);
 return (sizeof(e_goto));
}

int object::E_DIR_do(e_dir *t)
{
 if (t->d<2) d=t->d;
    else d^=1;
 return (sizeof(e_dir));
}


int object::E_BLOOD_do(e_blood *t)
{
 zpoint p;
 p.x=t->pos.x; if (d) p.x=-p.x;
 p.x+=x>>16;
 p.y=(y>>16)+t->pos.y;
 p.z=(z>>16)+t->pos.z;
 switch(t->bloodtype)
 {
  case 0: //blood squirt
   {
   p.z+=2;
   for (int i=0; i<5; i++)
    addobject(new object(&odf[2],0,p.x,p.y,(unsigned)p.z,d,this));
   addobject(new object(&odf[2],3,p.x,p.y,(unsigned)p.z,d,this));
   }
  break;
  case 3: //flood
   {
   p.z-=2;
   addobject(new object(&odf[2],4,p.x,p.y,(unsigned)p.z,d,this));
   }
  break;
 }
 return (sizeof(e_blood));
}


//creates new object and makes it a child
int object::E_NEWOBJECT_do(e_newobject *t)
{ //create an instance of the object
 objectdef *objdef;
 if (!t->onum) objdef=&odf[onum];
          else objdef=&odf[t->onum];
 
 addobject(new object(objdef,t->s,!d ? (x>>16)+t->pos.x : (x>>16)-t->pos.x,(y>>16)+t->pos.y,(z>>16)+(t->pos.z),
       (t->pos.d) ? d^1 : d,this));
 return (sizeof(e_newobject));
}    

/*
//modify position of child
int object::E_CHILDPOS_do(e_newobject *t)
{ 
 if (!child) //no child,whoops
  return sizeof(e_newobject);

//reposition child object   
 if (!d) child->x=x+(t->pos.x<<16);
     else child->x=x-(t->pos.x<<16);
 child->y=y+(t->pos.y<<16);
 child->z=z+(t->pos.z<<16);
 child->d=d;  t=t;
 return sizeof(e_newobject);
}*/

int object::E_REGION_do(e_region *t)
{
 r[t->rtype&0x7F]=r[t->rtype&0x7F]->update(t,this);    
 
 return (sizeof(e_region));
}


//pointer to functions that do the specified extra
//returns the size of the extra
static int (object::* object::extrado[])(byte *)=
 {
  (int (object::*)(byte *)) object::E_SOUND_do,  //0
  (int (object::*)(byte *)) object::E_GROUNDSHAKE_do,  //1
  (int (object::*)(byte *)) object::E_OBJECTSHAKE_do,  //2
  (int (object::*)(byte *)) object::E_NEWOBJECT_do,  //3
  (int (object::*)(byte *)) object::E_GOTO_do,  //4
  (int (object::*)(byte *)) object::E_BLOOD_do,  //5
  (int (object::*)(byte *)) object::E_REGION_do,  //6  
  (int (object::*)(byte *)) object::E_DIR_do,  //7
 };


void object::effect()
{
if (!fptr || !fptr->numextra) return;
byte *t=fptr->getextraptr();   //pointer to first option

for (int n=fptr->numextra; n>0; n--)          //number of bytes of options
 {
   if (!((*t)&0x80)) t+=(this->*extrado[*t])(t);
               else t+=extrasize[(*t)&0x7F];
 }
}


//do all hit effects, centering around absolute x,y point in z axis
void object::hiteffect(int hx,int hy,int hz)
{
if (!fptr || !fptr->numextra) return;
byte *t=fptr->getextraptr();   //pointer to first option

//find relative hit point
hx-=INTC(x); if (d) hx=-hx;
hy-=INTC(y);

for (int n=fptr->numextra; n>0; n--)          //number of bytes of options
 {
   if (!((*t)&0x80)) t+=extrasize[*t];
    else
    {
     static uchar e[40]; //temp E space
     memcpy(e,t,extrasize[(*t)&0x7F]);
     switch((*t)&0x7F)
     {
      case E_CHILDPOS:
      case E_NEWOBJECT:
       ((e_newobject *)e)->pos.x+=hx;
       ((e_newobject *)e)->pos.y+=hy;
       ((e_newobject *)e)->pos.z+=hz;
       //mprintf("%dX %dY %dZ",((e_newobject *)e)->pos.x,((e_newobject *)e)->pos.y,((e_newobject *)e)->pos.z);
       break;
      case E_BLOOD:
       ((e_blood *)e)->pos.x+=hx;
       ((e_blood *)e)->pos.y+=hy;
       ((e_blood *)e)->pos.z+=hz;
       break;
     }
     t+=(this->*extrado[(*e)&0x7F])(e); //do effect
    }
 }
}




