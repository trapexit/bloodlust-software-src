//Copyright(c) 1996 Bloodlust Software All rights reserved
//Main Control functions
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

extern int regionchecks,regionintersects,seriesstarts;


//lists of all active regions
region *rlist[3]={0,0,0};

//region constructor
region::region(object *x,e_region *e)
{
 platform=0; platnum=0;
 o=x; r=e; intersect=0;

  //link to chain
 list=&rlist[e->rtype];
 prev=0; next=*list;
 if (*list) (*list)->prev=this;
 *list=this;
}    

//region destructor
region::~region()
{ //unlink from list
 if (prev) prev->next=next; else *list=next;
 if (next) next->prev=prev;
 //force some additional checks now, for stuff that should fall when a region is removed
 if (list==&rlist[R_IMPERM])
 for (region *t=*list; t; t=t->next)
  if (t->platform==this)   {t->platform=0; t->o->updated|=16;}
}

region *region::update(e_region *e,object *o)
{
 if (e->rtype&0x80) //erase me
    { if (this) delete(this); return 0;} //remove region from list

 if (this)
 {
  if (r!=e)
  {
   r=e; 
   o->updated|=16;
   width=r->r.p2.x-r->r.p1.x; //get width of this region
   if (e->rtype==R_IMPERM)
   {
    for (region *t=*list; t; t=t->next)
     if (t->platform==this)   {t->platform=0; t->o->updated|=16;}
    platnum=0; //nobody standing on us now.
   }
  }
  return this;
 }

 return new region(o,e); //make new region
}    

region::getabsregion(zrect &a)
{
 a=r->r; //get region
 if (o->d) {short t=a.p1.x; a.p1.x=-a.p2.x; a.p2.x=-t;} //flip
 a.p1.x+=INTC(o->x); a.p2.x+=INTC(o->x);    
 a.p1.y+=INTC(o->y); a.p2.y+=INTC(o->y);
 a.p1.z+=INTC(o->z); a.p2.z+=INTC(o->z);
 return 1;
}    

    


//sees if two regions intersect
int intersectregion(zrect *a, zrect *b)
{
// regionchecks++;
   //see if NO intersection
 if (!a || !b) return 0;
 if (b->p1.x>a->p2.x || a->p1.x>b->p2.x) return 0;
 if (b->p1.y>a->p2.y || a->p1.y>b->p2.y) return 0;
 if (b->p1.z>a->p2.z || a->p1.z>b->p2.z) return 0;

  //must be some intersection
 return 1;
}


//calculates the x,y edge distances between the boundaries of two INTERSECTING rectangles
int edgeregion(zrect *a, zrect *b,lpoint *i)
{
 int result=0;
 if (a->p2.x>=b->p2.x) {result|=1;  i->x=b->p2.x-a->p1.x; if (i->x<1) return result;} //intersects left of our rect
 if (a->p1.x<=b->p1.x) {if (result&1) result=0; else {result|=2; i->x=b->p1.x-a->p2.x; if (i->x>-1) return result;} } //intersects right of our rect
 
 if (a->p2.y>=b->p2.y) {result|=4; i->y=b->p2.y-a->p1.y; } //intersects top of our rect
 if (a->p1.y<=b->p1.y)
     {if (result&4) result&=~12; else {result|=8; i->y=b->p1.y-a->p2.y; }} //intersects bottom of our rect
//if (i->y>-5) return result&(~3);
//if (i->y<5) return result&(~3);

 //result=15 if a encompasses b
// if (!result) result=IR_WITHIN; //result if b encompasses a
 return result;
}

//calculates the center point of intersection
int centerregion(zrect *a, zrect *b,lpoint *i)
{
 int result=0;
 if (a->p2.x>=b->p2.x)   //b is left of a's right edge
 {
  if (a->p1.x<=b->p1.x)   //b is right of a's left edge
        {result|=3; i->x=(b->p1.x+b->p2.x)/2;} //b is inside A
  else   {result|=1;  i->x=(b->p2.x+a->p1.x)/2;} //b is to the left
 } else
 {
  if (a->p1.x<=b->p1.x)   //b is right of a's left edge
    {result|=2; i->x=(b->p1.x+a->p2.x)/2;} //b is to the right
    else       {i->x=(a->p1.x+a->p2.x)/2;} //A is inside b
 }

 if (a->p2.y>=b->p2.y)   //b is below of a's top edge
 {
  if (a->p1.y<=b->p1.y)   //b is right of a's left edge
        {result|=3<<2; i->y=(b->p1.y+b->p2.y)/2;} //b is inside A
  else   {result|=1<<2;  i->y=(b->p2.y+a->p1.y)/2;} //b is to the left
 } else
 {
  if (a->p1.y<=b->p1.y)   //b is right of a's left edge
      {result|=2<<2; i->y=(b->p1.y+a->p2.y)/2;} //b is to the right
  else      { i->y=(a->p1.y+a->p2.y)/2;} //A is inside b

 }

 return result;
}    

#define R_NOATTACK 0
#define R_DOATTACK 1
#define R_DOALLATTACK 2

//checks to see if this region's attack intersects any other vuln regions
void region::checkattack()
{
 static intersection i;
 if (!this) return; 
 if (!o->updated) return; //not updated, dont bother

 i.a=o;
 i.ar=r;    //store this region
 getabsregion(i.arect); //get attack region absolute coords

 int attackmade=0;
 for (region *tnext,*t=rlist[R_VULN]; t; t=tnext) //go through all vuln regions
 {
  tnext=t->next;
  if (o!=t->o) //get abs vuln regions
  {
   t->getabsregion(i.brect);
   regionchecks++;
   if (intersectregion(&i.arect,&i.brect)) //see if they intersect
    {
//      regionintersects++;
      i.result=centerregion(&i.arect,&i.brect,&i.p);
      i.b=t->o; //store object pointer
      i.br=t->r; //store e_region

       //call region functions
      //call OUR attack region function    
      int aresult=(i.*(intersection::rfunclist[i.ar->rfunc]))();
      
      //call vuln if attack made
      if (aresult) {(i.*(intersection::rfunclist[i.br->rfunc]))(); attackmade++;}
      if (aresult==R_DOATTACK) break;
    }
  }
 }

  
 //finalize attack by calling attack region function
 if (attackmade)
  { i.b=0; (i.*(intersection::rfunclist[i.ar->rfunc]))();}
}    

extern lpoint regionpoint;



//checks to see if this region's imperm intersects any other regions' imperm 
void region::checkimperm()
{
static intersection i;
if (!this) return;

if (o->updated)  //check only if updated
{
 i.a=o;
 i.ar=r;    //store this region
 getabsregion(i.arect);  //get imperm region

 //do object collision movements
 intersect=0; 
 for (region *t=*list; t; t=t->next) //go through all regions
  if (t!=this) 
   {
    regionchecks++;
    t->getabsregion(i.brect);
    if (intersectregion(&i.arect,&i.brect)) //see if they intersect
     {
      i.result=edgeregion(&i.arect,&i.brect,&i.p);
      i.b=t->o; //store object pointer
      i.br=t->r; //store e_region
      
      intersect|=(i.*(intersection::rfunclist[i.br->rfunc]))();
      getabsregion(i.arect);  //get imperm region
     }
   }
 o->intersect=intersect;
}

 if (!(intersect&8))
  {if (platform) {platform->platnum--; platform=0;}} //nothing under us, so no platform
 if (!platform) o->falldown();
}    

//updates all objects that intersect the top of this object
void region::moveaboveobjects(int x,int y,int z) //x,y,z<<16
{
 int i=platnum;
 for (region *t=*list; i && t; t=t->next)
  if (t->platform==this) {t->o->moveabs(x,y,z); i--;}
}    



//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
//------------------------------------------------




#include "\a32\rfunc.lst"

#define R_NONE 0
#define R_PUSH 1
#define R_SOLID 2
#define R_FIXED 3
#define R_MAINATTACK 4
#define R_MAINVULN 5
#define R_ENEMYATTACK 6
#define R_ENEMYVULN 7
#define R_ALLATTACK 8
#define R_ALLVULN 9
#define R_BOUNCE 10



int edgeregion(zrect *a, zrect *b,lpoint *i);
int centerregion(zrect *a, zrect *b,lpoint *i);

lpoint regionpoint;
int regiontest,rpush,rsolid;
// regionpoint=*p;
// regiontest=result;

int intersection::r_none() {return 0;};

void region::setplatform(region *newplat,intersection *i)
{
 if (platform==newplat) return;
 if (!platform ||  ( (!(i->result&3) || abs(i->p.x)>=platwidth) && i->brect.p1.y<=platformy))
  {
   if (platform) platform->platnum--;
    //set new platform
   platform=newplat; 
   platformy=i->brect.p1.y;
   platwidth=abs(i->p.x);
   platform->platnum++;
  }
}    


//pushes through other objects
int intersection::r_push()
{
   
 rpush++;
 if (ar->rfunc==R_PUSH || ar->rfunc==R_BOUNCE) return 0; //can't push another pusher

 if (INTC(b->y)>INTC(a->y)) a->z=b->z+1;
 if (INTC(b->y)==INTC(a->y))
   if (a->x>b->x) a->z=b->z+1;
 
 if ((result&8) && p.y)
 {
  if (p.y<-20) //they are low enough to push
   if ((result&3) && p.x) {a->moveabs(p.x<<15,0,0); b->moveabs((-p.x)<<15,0,0);}    
 } else
 {
  if ((result&3) && !(result&(~3)) && p.x) {a->moveabs(p.x<<15,0,0); b->moveabs((-p.x)<<15,0,0);}    
  b->updated|=1;
 }
 return (result&~8);
};

int intersection::r_solid()
{
 rsolid++;
 if (INTC(b->y)>INTC(a->y)) a->z=b->z+1;
 if (INTC(b->y)==INTC(a->y))
   if (a->x>b->x) a->z=b->z+1;

 if ((result&8))// && p.y)
 { 
  if (p.y>=-20 || !p.y) //we're over it, let them stand on us
   {
    if (p.y) {a->hitground(a->y+(p.y<<16)); b->updated|=2;}
    a->r[R_IMPERM]->setplatform(b->r[R_IMPERM],this); //make this new platform possibly
   }
  else if ((result&3) && p.x) {a->moveabs(p.x<<15,0,0); b->moveabs((-p.x)<<15,0,0);}
 } else
 if ((result&4) && p.y)
 {
  if (p.y<=20) //they are low enough to push
   if ((result&3) && p.x) {a->moveabs(p.x<<15,0,0); b->moveabs((-p.x)<<15,0,0);}        
 }
         else if ((result&3) && !(result&(~3)) && p.x) {a->moveabs(p.x<<15,0,0); b->moveabs((-p.x)<<15,0,0);}

 
 return result;
};


int intersection::r_fixed()
{
 if ((result&8) && p.y)
 {
  if (p.y>=-20) //we're over it
     a->hitground(a->y+(p.y<<16));
  else
   if ((result&3) && p.x) {a->x+=p.x<<16; }    
 } else
 {
  if ((result&3) && !(result&(~3))  && p.x) a->x+=p.x<<16;
 }

 return result;
};


int intersection::r_bounce()
{
 if (ar->rfunc!=R_SOLID && ar->rfunc!=R_FIXED) return 0;
 if (!(result&(12))) {b->d^=1; b->x+=p.x<<16;}
// b->hiteffect(&p);
 b->hiteffect(p.x,p.y,0); //play hiteffect of this frame
 b->advanceframe(); //skip

 return result&(~8);
}


//region function called when attack intersects another object
int intersection::r_mainattack()
 {
  //check to see if we can attack this object
  if (b)
   if (br->rfunc==R_MAINVULN) return R_NOATTACK; //can't be mainvuln
    else
    {
     b->loseenergy(a->cs->energy); //decrease enemies energy
     return R_DOATTACK;
    }

  //finalize attack
  if (a->csnum!=ar->afterseries) a->startseries(ar->afterseries); //go to next series for attacker
  return 0;
 };

//region function called when vuln gets intersected by attacker object
int intersection::r_mainvuln()
 {
  if (b->energy>=0) //we are not dead
   {
    if (br->afterseries)
    {b->d=a->d^1; b->startseries(br->afterseries);} //go to next series as dictated by region
      else
      {
       if (b->d!=a->d || !b->startseries("painback"))
          b->startseries("painhigh");
      }
   } else if (ar->rfunc!=R_ALLATTACK) b->d=a->d^1;

  //create hit effects
//  b->hiteffect(&p);
 b->hiteffect(p.x,p.y,0); //play hiteffect of this frame

  return 1;
 };


//enemy rfuncs
//region function called when attack intersects another object
int intersection::r_enemyattack()
 {
  //check to see if we can attack this object
  if (b)
   if (br->rfunc==R_ENEMYVULN) return R_NOATTACK; //can't be mainvuln
    else
    {
     b->loseenergy(a->cs->energy); //decrease enemies energy
     return R_DOATTACK;
    }

  //finalize attack
  if (a->csnum!=ar->afterseries) a->startseries(ar->afterseries); //go to next series for attacker
  return 0;
 };

//region function called when vuln gets intersected by attacker object
int intersection::r_enemyvuln()
 {
  if (b->energy>=0) //we are not dead
   {
    if (br->afterseries)
    {b->d=a->d^1; b->startseries(br->afterseries);} //go to next series as dictated by region
      else
      {
       if (b->d!=a->d || !b->startseries("painback"))
          b->startseries("painhigh");
      }
   } else if (ar->rfunc!=R_ALLATTACK) b->d=a->d^1;


  //create hit effects
//  b->hiteffect(&p);
 b->hiteffect(p.x,p.y,0); //play hiteffect of this frame

  return 1;
 };


//region function called when attack intersects another object
int intersection::r_allattack()
 {
  //check to see if we can attack this object
  if (b)
    {
     b->loseenergy(a->cs->energy); //decrease enemies energy
     if (b->x<a->x) b->d=0; else b->d=1; //make us turn around  to face it
     return R_DOALLATTACK;
    }
  //finalize attack
  if (a->csnum!=ar->afterseries)  a->startseries(ar->afterseries); //go to next series for attacker
  return 0;
 };

//region function called when vuln gets intersected by attacker object
int intersection::r_allvuln()
 {
  if (b->energy>=0) //we are not dead
   {
     b->startseries(br->afterseries); //go to next series for vulnerable
   } else b->d=a->d;
//  if (b->x<a->x) b->d=0; else b->d=1; //make us turn around  to face it

  //create hit effects
//  b->hiteffect(&p);
 b->hiteffect(p.x,p.y,0); //play hiteffect of this frame

  return 1;
 };



