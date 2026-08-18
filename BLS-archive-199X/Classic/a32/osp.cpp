#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#include "object.h"
#include "bg.h"
#include "objspace.h"

#include "message.h"
#include "dd.h"
#include "r2img.h"

#include "keyb.h"
#include "input.h"
#include "mouse.h"
#include "font.h"

#include "region.h"
#include "effect.h"
#include "effectw.h"

#include "config.h"

#include "gui.h"

#ifdef  WIN95
#ifndef ANIMATOR
#include "net.h"
#include "netp.h"
#include "netobj.h"
#endif
#endif
//------------------------------
//        objectspace
//------------------------------

objectspace::objectspace(char *objfile,char *bgfile,int txw,int tyw)
:GUIcontents(txw,tyw),maximized(0)
{
 xw=txw; yw=tyw;

 #ifdef ANIMATOR
 moving=0;
 ep=0; //no effect
 #endif

 odf=0; numodf=0; //no object definitions
 bgdf=0; numbgdf=0; //no bg defs
 o=0; //no objects
 p=0; //no objects selected
 bg=0; //no bg

 rlist[0]=rlist[1]=rlist[2]=0;

 ::objspace=this;

 #ifdef ANIMATOR
 //read .lst files
 readobjectdefs(objfile);
 readbgdefs(bgfile);
 #else
 //read .bin files
 readobjbin(objfile);
 readbgbin(bgfile);
 #endif

 msg.printf(2,"objectspace created. %d objs %d bgs defined.",numodf,numbgdf);
 root->refresh(GUIRFR_OBJSPACE,this);
 updated=0; //no updates necessary
}


objectspace::~objectspace()
{
if (this==::objspace) ::objspace=0;
disable();
p=0;
//delete all objects
while (o) delete o;
//delete all objectdefs
if (odf)
 {
  for (int i=0; i<numodf; i++) delete odf[i];
  free(odf);
  odf=0; numodf=0;
 }

//delete active background
if (bg) delete bg; bg=0;
//delete all bgdefs
if (bgdf)
 {
  for (int i=0; i<numbgdf; i++) delete bgdf[i];
  free(bgdf);
  bgdf=0; numbgdf=0;
 }

cfg->set(CFG_NOFILLEDDESKTOP,0);

enable();
root->refresh(GUIRFR_OBJSPACE,0);
msg.printf(2,"objectspace destroyed.");

#ifdef NETWORK
netdisconnect();
#endif
}


void objectspace::tick()
{
 {
 object *t;
 //tick all objects
 for (t=o; t; t=t->next)
  if (t->playing && t->fptr)
   {
    t->tick();
    if (t==p) updated|=t->updated; //main player was updated

     //move regions
    if (t->updated&7)
     {
      if (t->r[0]) t->r[0]->getabsregion();
      if (t->r[1]) t->r[1]->getabsregion();
      if (t->r[2]) t->r[2]->getabsregion();
     }
    t->updated=0;
   }
 }

/*
 //update all regions
 for (int i=0; i<3; i++)
  for (u=rlist[i]; u; u=u->next)
   if (u->o->updated&7) u->getabsregion();

 //clear updates
 for (t=o; t; t=t->next) t->updated=0;*/

 {
 region *u,*unext;
 //check all impermiability intersections
 for (u=rlist[R_IMPERM]; u; u=unext)
     {unext=u->next; u->checkimperm();}

 //check for attack intersections with vulns
 for (u=rlist[R_ATTACK]; u; u=unext)
     {unext=u->next; u->checkattack();}
 }

 //tick bg
 if (bg) bg->tick();
}


void objectspace::refreshneedload()
{
 for (int i=1; i<numodf; i++)
  if (odf[i]->refcount)
   odf[i]->refreshload();
}

void objectspace::refreshneeddelete()
{
 object *t=o;
 while (t)
  {
   object *next=t->next;
   if (t->killme) //delete all flagged objects
    {if (p==t) setp(NULL); delete t; }
    t=next;
  }
}

//set selected object
void objectspace::setp(object *newp)
{
 p=newp;
 if (p && !p->creator) inputdevice[0]->bind(p);
 if (bg) bg->track=p;
 root->refresh(GUIRFR_OBJECT,p);
}


//object maintenance
void objectspace::refreshobj()
{
 disable();
 #ifdef DOS
 if (needload) refreshneedload(); needload=0;
 #endif
 if (needdelete) refreshneeddelete(); needdelete=0;
  //sort characters
 for (object *t=o; t; t=t->next) t->resort();
 enable();
}

void objectspace::draw(char *dest)
{
#ifdef ANIMATOR
 if (updated)
  {
   if (updated&OU_SERIES) root->refresh(GUIRFR_SERIES,p->cs); else
   if (updated&OU_FRAME) root->refresh(GUIRFR_FRAME,p->fptr);
   updated=0;
  }
#endif
 refreshobj();

 if (!bg && !maximized) fill(1);

 {
  CLIP clip(dest,x1,y1,x2,y2);

  cfg->set(CFG_NOFILLEDDESKTOP,(bg && maximized));

  if (bg) bg->refreshbg(dest); //draw the background

  object *t;
  //draw shadows
  for (t=o; t; t=t->next) t->drawshadow(dest);
  //draw objects
  for (t=o; t; t=t->next)   t->draw(dest);

  if (bg) bg->drawfg(dest); //draw the foreground
 }


 #ifdef ANIMATOR
 //draw effect shit
 if (ep) ep->draweffect(dest);
 #endif

 if (cfg->get(CFG_SHOWREGIONS))
 {
  int num;
  region *u;

  num=0;
  for (u=rlist[0]; u; u=u->next,num++) u->draw(dest);
  font[0]->printf(10,40,"numimperm=%d",num);

  num=0;
  for (u=rlist[1]; u; u=u->next,num++) u->draw(dest);
  font[0]->printf(10,50,"numvuln=%d",num);

  num=0;
  for (u=rlist[2]; u; u=u->next,num++) u->draw(dest);
  font[0]->printf(10,60,"numattack=%d",num);
 }

}


/*
 font[0]->printf(10,50,"p=%X",p);
 font[0]->printf(10,60,"bg=%X",bg);
 font[0]->printf(10,50,"needload=%d",needload);
 font[0]->printf(10,60,"needdelete=%d",needdelete);
 font[0]->printf(10,70,"numobjs=%d",num);
 */

void objectspace::cleanup()
{
 if (!this) return;
 object *tnext;
 for (object *t=o; t; t=tnext)
 {
  tnext=t->next;
  if (t->creator)
   {
    if (t==p) setp(0);
    delete t;
   } else
   {
    //t->y=0;
    t->wall=t->ceiling=0;
    t->forcefloor();
    if (t->active) t->activate(0);
   }
 }
}


//adds object to main objectspace
object *objectspace::addobject(object *a)
{
if (!this) return a;
if (a->osp) a->osp->removeobject(a);

//find insertion point
for (object *tprev=0,*t=o; t; tprev=t,t=t->next)
  if (t->relz()>a->relz()) break;

//a goes after tprev, before t
a->prev=tprev;
a->next=t;
if (tprev) tprev->next=a; else o=a;
if (t)     t->prev=a;

a->osp=this; //link to this objectspace
return a;
}

void objectspace::removeobject(object *a)
{
 if (!this) return;
 if (!a->osp) return;

 a->osp=0;
 if (a->prev) a->prev->next=a->next; //else  objspace->o=a->next;
 if (a->next) a->next->prev=a->prev; //else objspace->lasto=a->prev;

 if (a==o) o=a->next;
 if (a==p) setp(0);
}


//swaps two objects in order
void objectspace::swap(object *a,object *b)
{
 if (!a || !b) return;

 if (a->prev) a->prev->next=b; else o=b; //set as first now
 if (b->next) b->next->prev=a;

 a->next=b->next;
 b->prev=a->prev;

 b->next=a;
 a->prev=b;
}


#include "file.h"
#ifndef ANIMATOR

void objectspace::readobjbin(char *filename)
{
 FILEIO f;

 struct {
  char name[32];
  char volfile[32];
 } OBJDESC;

 if (f.open(filename)) return;
 numodf=f.readint();
 f.readint();
 odf=(objectdef **)calloc(numodf,4);

 for (int i=0; i<numodf; i++)
  {
   f.read(&OBJDESC,sizeof(OBJDESC));
   odf[i]=new objectdef(this,i,OBJDESC.name,OBJDESC.volfile);
  }
 f.close();
};




void objectspace::readbgbin(char *filename)
{
 FILEIO f;

 struct {
  char name[32];
  char volfile[32];
 } OBJDESC;

 if (f.open(filename)) return;
 numbgdf=f.readint();
 f.readint();
 bgdf=(bgdef **)calloc(numbgdf,4);

 for (int i=0; i<numbgdf; i++)
  {
   f.read(&OBJDESC,sizeof(OBJDESC));
   bgdf[i]=new bgdef(this,i,OBJDESC.name,OBJDESC.volfile);
  }
 f.close();
};

#endif


void objectspace::open(char *objlist, char *bglist,int xw,int yw)
{
 GUImaximizebox *p=new GUImaximizebox(guiroot,"Objectspace",new objectspace(objlist,bglist,xw,yw),GUIdefx,GUIdefy);
 if (SCREENX<=320)   p->maximize();
 nextGUIdef();
};




void objectspace::resize(int txw,int tyw)
{
 GUIrect::resize(txw,tyw);
 if (maximized && parent)
  {
//   ((GUImaximizebox *)parent)->resize(txw,tyw);
   ((GUImaximizebox *)parent)->reposmaxbutton();
  }
}

void enablegui();
void objectspace::restore()
 {
  resize(xw,yw);
  maximized=0;
  if (parent) moveobjectsrel(-parent->x1,-parent->y1-12);
  if (bg && bg->active) bg->activate();
  enablegui();
 }
void objectspace::maximize()
 {
  moveto(0,0);
  resize(SCREENX,SCREENY);
  if (parent) moveobjectsrel(parent->x1,parent->y1+12);
  if (bg && bg->active) bg->activate();
  maximized=1;
 }


void objectspace::moveobjectsrel(int rx,int ry)
{
 if (bg) bg->move(-(rx<<16),-(ry<<16));
/*  else
 for (object *t=o; t; t=t->next)
  {
   t->x+=rx<<16;
   t->y+=ry<<16;
   t->getbasey();
   if (t->active) t->falldown();
   if (t->y>t->basey) t->y=t->basey;
  }*/
  

 if (p && (p->updated&OU_SERIES))
   root->refresh(GUIRFR_SERIES,p->cs);
// msg.printf(3,"moveobjrel %d %d",rx,ry);
}



//-------------------------------


object *objectspace::newobject(int num,int x,int y,int z,int d)
{
 if (!this || !odf || num<0 || num>=numodf) return 0;


 object *t;
 if (bg)
 {
  x+=bg->x>>16; y+=bg->y>>16;
  if (!bg->getbasey(x,y,y))//put it on nearest floor
         y+=height()/2;    //no floor, so put it in middle of screen
 } else y=height()*5/6;

 #ifdef NETWORK
 if (!nc)
 #endif
       t=new object(odf[num],0,x,y,z,d,0);
 #ifdef NETWORK
  else t=new localobject(odf[num],0,x,y,z,d);
 #endif
 return t;
}

object *objectspace::newobject(char *name,int x,int y,int z,int d)
{
 if (!this || !odf || !name) return 0;
 int onum=atoi(name);
 if (!onum)
  for (int i=0; i<numodf; i++)
  if (!stricmp(name,odf[i]->name))
   {onum=i; break;}

 return onum ? newobject(onum,x,y,z,d) : 0;
}



bgobject *objectspace::newbgobject(int num)
{
 if (!this || !bgdf || num<0 || num>=numbgdf) return 0;
 return new bgobject(bgdf[num],0,0);
}

bgobject *objectspace::newbgobject(char *name)
{
 if (!this || !bgdf || !name) return 0;

 int bgnum=atoi(name); //see if it's a decimal string

 if (!bgnum) //find bg name then
  for (int i=0; i<numbgdf; i++)
  if (!stricmp(name,bgdf[i]->name))
   {bgnum=i; break;}

 return bgnum ? newbgobject(bgnum) : 0;
}




//--------------------------------------------------


image *object::hittest(int mx,int my)
{
 if (!fptr) return 0;
 //find coordinates relative to our base
 if (!osp->bg) {mx-=x>>16; my-=y>>16;}
         else {mx-=(x-osp->bg->x)>>16; my-=(y-osp->bg->y)>>16;}

 image *i=fptr->getimgptr();

 for (int j=fptr->numimages-1; j>=0; j--)
 {
  int x1,x2,y1,y2;
  if (!(d&1)) //not x flipped
   { x1=i[j].dispx; x2=i[j].dispx+od->id[i[j].index]->xw;} else
   { x1=-i[j].dispx-od->id[i[j].index]->xw; x2=-i[j].dispx;}

  y1=i[j].dispy; y2=i[j].dispy+od->id[i[j].index]->yw;

  if (mx>=x1 && mx<x2 && my>=y1 && my<y2) return &i[j];
 }
 return 0;
}

#ifndef ANIMATOR
#include "ospmove.h"

GUIrect *objectspace::click(mouse &m)
{
 if (m.click&MBLEFT)
 {
  //do object hit testing
  object *oldp=p;
  for (object *t=o; t; t=t->next)
   if (t->hittest(m.x-x1,m.y-y1)) p=t; //this object is selected

  if (p!=oldp) setp(p);
  return 0;
 }

 //---------------------
 //right click
 if (m.click&MBRIGHT)
 if (bg && (!p || kbstat&KB_SHIFT))
 {
  if (!maximized)  return new bgmove(bg,x2+10,y1+10);
              else return new bgmove(bg,x2-70,y1+30);
 }
  else
 if (p) //drag selected object
 {
  if (!maximized)  return new objectmove(p,x2+10,y1+10);
              else return new objectmove(p,x2-70,y1+30);
 }
 return 0;
};

#endif





