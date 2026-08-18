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

#include "effect.h"
#include "effectw.h"
#include "config.h"

#include "imgmove.h"

#include "file.h"
#include "gui.h"
//------------------------------
//        objectspace
//------------------------------

void object::dirty() {if (od) od->dirty=1;}



void objectdefw::addnewseries()
{
  //add new series at end
 sd=(series *)realloc(sd,(nums+1)*sizeof(series));
  //set to zero
 memset(&sd[nums],0,sizeof(series));

 sprintf(sd[nums].name,"series %d",nums);
 nums++;
}


void object::addnewseries()
{
 if (od->readonly || playing) return;
 _disable();

 ((objectdefw *)od)->addnewseries();
 startseries(od->nums-1);

 dirty();
 osp->resetodf(onum); //reset all objects that are of this objectdef
 _enable();
 root->refresh(GUIRFR_OBJECT,this);
}




void object::insertframe()
{
 if (!cs || od->readonly || playing) return;
 _disable();
 cs->insertframe(fptr);
 dirty();
 osp->resetodf(onum); //reset all objects that are of this objectdef
 _enable();
 root->refresh(GUIRFR_FRAME,fptr);
}

void object::insertframeafter()
{
 if (!cs || !fptr || od->readonly || playing) return;
 _disable();
 cs->insertframe((frame *)(((char *)fptr)+fptr->size));
 dirty();
 osp->resetodf(onum); //reset all objects that are of this objectdef
 _enable();
 root->refresh(GUIRFR_FRAME,fptr);
}

void object::deleteframe()
{
 if (!cs || !fptr || od->readonly  || playing) return;
 _disable();
 cs->deleteframe(fptr);
 if (cf>=cs->nf && cf>0) cf--;
 dirty();
 osp->resetodf(onum); //reset all objects that are of this objectdef
 _enable();
 root->refresh(GUIRFR_FRAME,fptr);
}


void objectspace::resetodf(int onum)
{
 for (object *t=o; t; t=t->next)
  if (t->onum==onum)  t->resetfptr();
 root->refresh(GUIRFR_OBJSPACE,this);
}

void object::frameforeward()
{
 if (!this || !od->loaded || playing) return;
 if (playing) stop();
 if (cf<nf-1) cf++;
 resetfptr();
 root->refresh(GUIRFR_FRAME,fptr);
}

void object::framebackward()
{
 if (!this || !od->loaded) return;
 if (playing) stop();
 if (cf>0) cf--;
 resetfptr();
 root->refresh(GUIRFR_FRAME,fptr);
}

void object::seriesup()
{
 if (!this  || !od->loaded || playing) return;
 if (active) return;
 if (csnum>0) startseries(csnum-1);
 root->refresh(GUIRFR_SERIES,cs);
}

void object::seriesdown()
{
 if (!this || !od->loaded || playing) return;
  if (active) return;
 if (csnum<od->nums-1) startseries(csnum+1);
 root->refresh(GUIRFR_SERIES,cs);
}


int objectspace::keyhit(char kbscan,char key)
{
 switch(kbscan)
 {
  case KB_LEFT:  p->framebackward();  break;
  case KB_RIGHT: p->frameforeward();  break;
  case KB_UP: p->seriesup(); break;
  case KB_DOWN: p->seriesdown(); break;

  case KB_TAB:
      if (!p) p=o;
       else   p=p->next;
      if (p && !p->creator) inputdevice[0]->bind(p);
      root->refresh(GUIRFR_OBJECT,p);
     break;

  case KB_SPACE:
  case KB_ENTER:
    if (p && p->playing)
     {
      root->refresh(GUIRFR_STOPPED,p);
      p->stop();
      break;
     } else return 0;
  case KB_DEL:
    if (bg && bg->selbgline)
     {
      bg->bd->deletebgline(bg->selbgline);
      bg->selbgline=0;
     }
   break;
  default:  return 0;
 }
return 1;
}

void objectspace::play()
{
 if (!this || !p) return;
 p->play(p->csnum);
// msg.printf(3,"%s playing",p->od->name);
}

void objectspace::flipx()
{
 if (!this || !p) return;
 p->flip();

}

void objectspace::kill()
{
 if (!this || !p) return;
 msg.printf(3,"%s killed",p->od->name);

 for (object *t=o; t; t=t->next)
  if (t->creator==p) delete t;

 object *next=p->next;
 delete p;
 if (next) p=next; else p=o;
 root->refresh(GUIRFR_OBJECT,p);
}


void objectspace::playlooped()
{
 if (!this || !p) return;
 p->playlooped(p->csnum);
// msg.printf(3,"%s playing looped",p->od->name);
}

void objectspace::stop()
{
 if (!this || !p || !p->playing) return;
 p->stop();
// msg.printf(3,"%s stopped",p->od->name);
}

void objectspace::activate()
{
 if (!this || !p) return;
 p->activate(0);
 inputdevice[0]->bind(p);
 msg.printf(3,"%s activated",p->od->name);
}


void objectspace::receivefocus()
{
 if (p && p->active && !p->creator) inputdevice[0]->bind(p);

 if (::objspace!=this)
 {
  ::objspace=this;
   root->refresh(GUIRFR_OBJSPACE,this);
 }

 GUIrect::receivefocus();
}

void objectspace::insertframe()
{
 if (!this || !p) return;
 p->insertframe();
}

void objectspace::deleteframe()
{
 if (!this || !p) return;
 p->deleteframe();
}

void objectspace::insertframeafter()
{
 if (!this || !p) return;
 p->insertframeafter();
}


#include "ospmove.h"



//----------------------------------------------------------------------


//store information about object for image shit
void bgimagemove::bind(bgobject *tbgo)
{
 if (bg && i)
  {
   i->dispx+=-(bg->x>>16)+osp->x1; //make absolute
   i->dispy+=-(bg->y>>16)+osp->y1;
  }
 bg=tbgo;      //object
 if (bg) {osp=bg->osp;}
    else {osp=0;}
 if (bg && i)
  {
   i->dispx-=-(bg->x>>16)+osp->x1; //make relative
   i->dispy-=-(bg->y>>16)+osp->y1;
  }
}

//start floating
bgimagemove::bgimagemove(bgdef *tbd,bgimage *ti,int x,int y)
 :GUIrect(guiroot,x,y,x+65,y+12),bg(0),i(0)
{
 bd=tbd;
 bind(0);
 i=ti;
 setmodal(this);
 m.hidecursor();
}

//start draggin from object
bgimagemove::bgimagemove(bgobject *tbgo,bgimage *ti,int x,int y)
   :GUIrect(guiroot,x,y,x+65,y+53),bg(0),i(0)
   {
    bd=tbgo->bd; //objectdef
    bind(tbgo);
    i=ti;     //image itself
    setmodal(this);
    m.hidecursor();
   };

bgimagemove::~bgimagemove()
{
 if (!osp && i) delete i; //delete floating image
 setmodal(0);
 m.capture=0;
 m.showcursor();
}

//delete the image i from the bgobject bgo
void bgimagemove::deleteimage()
{
 if (!i) return;
 if (!osp || !bg) return;
 if (bg->active) return; 
 _disable();

 //make image floaty now
 bgimage *newi=new bgimage;
 *newi=*i;

 //remove bgimage from bgdef
 bg->bd->deleteimage(i);

 i=newi;
 _enable();
}

void bgimagemove::insertimage()
{
 if (!i) return;
 if (!osp || !bg) return;
 if (bg->active) return;
 _disable();

 //add bgimage to bgdef
 bgimage *im=bg->bd->insertimage(i);

 //remove floaty image, make fixed
 delete i;
 i=im;

 _enable();

}

int bgimagemove::drag(mouse &m)
  {
   if (!i) return 0;
   i->dispx+=m.x-m.oldx;
   if (!(kbstat&KB_SHIFT))
     i->dispy+=m.y-m.oldy;
    else
     i->dispz+=m.y-m.oldy;

   if (osp)
    if (root->hittest(m.x,m.y)!=osp) //we left the object space!
     {
      deleteimage();
      bind(0); //unbind
      resize(width(),12);
      return 1;
     }

   if (!osp)
    if (objspace && root->hittest(m.x,m.y)==objspace) //we entered the currently selected object space
     if (objspace->bg && !objspace->bg->active)
      {
       bind(objspace->bg);
       insertimage(); //insert i into our bgo
       resize(width(),53);
      }
   return 1;
  };

int bgimagemove::release(mouse &m)
  {
   deleteimage();
   insertimage();
   delete this;
   return 1;
  }
void bgimagemove::draw(char *dest)
 {
   fill(0);
   outline(2);
   if (!i) return;

   font[1]->drawcentered(bd->imgnames ? bd->imgnames[i->index] : "<noname>",dest,(x1+x2)/2,y1+2);
   if (osp)
   {
    font[2]->printf(x1+2,y1+12,"X: %d",i->dispx);
    font[2]->printf(x1+2,y1+22,"Y: %d",i->dispy);
    font[2]->printf(x1+2,y1+32,"Z: %d",i->dispz);
    font[3]->printf(x1+2,y1+42,"%c%c",(i->orient&2) ? 'X' : ' ',(i->orient&1) ? 'Y' : ' ');
   } else bd->id[i->index]->draw(dest,i->dispx,i->dispy,i->orient);
 }


int bgimagemove::keyhit(char kbscan,char key)
{
 if (i)
 {
  if (key=='x') {i->orient^=2; return 1;}
  if (key=='y') {i->orient^=1; return 1;}


  if (kbscan==KB_DEL)
   if (i)
   {
    deleteimage();
    delete i;
    i=0;
    delete this;
    return 1;
   }
 }
 return 0;
}
//---------------------------------------------------------
class bglinemove:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  bgobject *bg;       //bgobject bound to
 class  bgdef *bd;          //bgobject def
 struct  bgline *l;        //bgline
 public:
 bglinemove(bgobject *tbgo,bgline *tl)
   :GUIrect(guiroot,0,0,0,0),bg(tbgo),l(tl)
   {
    bg->selbgline=l;
    bd=tbgo->bd; //objectdef
    setmodal(this);
    m.hidecursor();
   };
 virtual ~bglinemove()
  {
   setmodal(0);
   m.capture=0;
   m.showcursor();
  };
 virtual int drag(mouse &m){l->moverel(m.x-m.oldx,m.y-m.oldy); return 1;};
 virtual int release(mouse &m) {delete this; return 1; }
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "bglinemove";}
 virtual void draw(char *dest) {};
 virtual int keyhit(char kbscan,char key)
 {
  if (kbscan==KB_DEL)
   if (l && bd)
   {
    bd->deletebgline(l);
    bg->selbgline=0;
    delete this;
    return 1;
   }
  return 0;
 };
};





class bglinedraw:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  bgobject *bg;       //bgobject bound to
 class  bgdef *bd;          //bgobject def
 struct  bgline *l;        //bgline
 public:
 bglinedraw(bgobject *tbgo,int tx,int ty)
   :GUIrect(guiroot,0,0,0,0),bg(tbgo),osp(tbgo->osp)
   {
    bd=tbgo->bd; //objectdef

    x1=x2=(bg->x>>16)+tx;
    y1=y2=(bg->y>>16)+ty;

    bgline x;
    x.type=bg->etype;
    x.x1=x1; x.y1=y1;
    x.x2=x2; x.y2=y2;
    l=bg->selbgline=bd->insertbgline(&x);

    setmodal(this);
    m.hidecursor();
   };
 virtual ~bglinedraw()
  {
    //msg.printf(2,"%p %d %d %d %d",l,x1,y1,x2,y2);
    if ((l->type==BGL_FLOOR || l->type==BGL_CEILING) && l->x1==l->x2)
    {
     bd->deletebgline(l);
     bg->selbgline=0;
    } else
    if ((l->type==BGL_WALLLEFT || l->type==BGL_WALLRIGHT) && l->y1==l->y2)
    {
     bd->deletebgline(l);
     bg->selbgline=0;
    }

   setmodal(0);
   m.capture=0;
   m.showcursor();
  };
 virtual int drag(mouse &m)
  {
   if (!l) return 1;
//   x2+=m.x-m.oldx;
//   y2+=m.y-m.oldy;
   x2=(bg->x>>16)+m.x-osp->x1;
   y2=(bg->y>>16)+m.y-osp->y1;

   l->set(x1,y1,x2,y2);
   return 1;
  };
 virtual void draw(char *dest)
 {
   if (m.x>osp->x2-20) bg->x+=3<<16;
   if (m.x<osp->x1+20) bg->x-=3<<16;
   if (m.y>osp->y2-20) bg->y+=3<<16;
   if (m.y<osp->y1+20) bg->y-=3<<16;
 };
 virtual int release(mouse &m) {delete this; return 1; }
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "bglinemove";}
// virtual void draw(char *dest) {};
 virtual int keyhit(char kbscan,char key)
 {
  if (kbscan==KB_DEL)
   if (l && bd)
   {
    bd->deletebgline(l);
    bg->selbgline=0;
    
    delete this;
    return 1;
   }
   return 0;
 };
};

//-----------------------------------------------------------------------
//---------------------------------------------------------
class bgboundarymove:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  bgobject *bg;       //bgobject bound to
 class  bgdef *bd;          //bgobject def
 struct  boundary *l;        //bgline
 public:
 bgboundarymove(bgobject *tbgo,boundary *tl)
   :GUIrect(guiroot,0,0,0,0),bg(tbgo),l(tl)
   { bd=tbgo->bd; setmodal(this); m.hidecursor();};
 virtual ~bgboundarymove() {setmodal(0); m.capture=0; m.showcursor(); };
 virtual int drag(mouse &m){l->moverel(m.x-m.oldx,m.y-m.oldy); return 1;};
 virtual int release(mouse &m) {delete this; return 1; }
 virtual int acceptfocus() {return 1;}
 virtual void draw(char *dest) {};
 virtual int keyhit(char kbscan,char key)
 {
  if (kbscan==KB_DEL)
   if (l && bd)
   {
    bd->deleteboundary(l);
    delete this;
    return 1;
   }
  return 0;
 };
};

class bgboundarydraw:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  bgobject *bg;       //bgobject bound to
 class  bgdef *bd;          //bgobject def
 struct  boundary *l;
 public:
 bgboundarydraw(bgobject *tbgo,int tx,int ty)
   :GUIrect(guiroot,0,0,0,0),bg(tbgo),osp(tbgo->osp)
   {
    bd=tbgo->bd; //objectdef

    x1=x2=(bg->x>>16)+tx;
    y1=y2=(bg->y>>16)+ty;

    boundary x;
    x.type=bg->etype; //BGB_BOTTOM;
    x.x1=x1; x.y1=y1;
    x.x2=x2; x.y2=y2;
    l=bd->insertboundary(&x);
    setmodal(this);
   };
 virtual ~bgboundarydraw()
  {
   setmodal(0);
   m.capture=0;
  };
 virtual int drag(mouse &m)
  {
   if (!l) return 1;
//   x2+=m.x-m.oldx;
//   y2+=m.y-m.oldy;
   x2=(bg->x>>16)+m.x-osp->x1;
   y2=(bg->y>>16)+m.y-osp->y1;
   l->set(x1,y1,x2,y2);
   return 1;
  };
 virtual int release(mouse &m) {delete this; return 1; }
 virtual int acceptfocus() {return 1;}
 virtual void draw(char *dest)
 {
   if (m.x>osp->x2-20) bg->x+=3<<16;
   if (m.x<osp->x1+20) bg->x-=3<<16;
   if (m.y>osp->y2-20) bg->y+=3<<16;
   if (m.y<osp->y1+20) bg->y-=3<<16;
 };
 virtual int keyhit(char kbscan,char key)
 {
  if (kbscan==KB_DEL)
   if (l && bd)
   {
    bd->deleteboundary(l);
    delete this;
    return 1;
   }
   return 0;
 };
};

//----------------------------------------------------------------------

//store information about object for image shit
void imagemove::bind(object *to)
{
 if (o && i)
  {
   i->dispx+=(o->relx()>>16)+osp->x1; //make absolute
   i->dispy+=(o->rely()>>16)+osp->y1;
  }
 o=to;      //object
 if (o) {fptr=o->fptr; cs=o->cs; osp=o->osp;}
   else {fptr=0; cs=0; osp=0;}
 if (o && i)
  {
   i->dispx-=(o->relx()>>16)+osp->x1; //make relative
   i->dispy-=(o->rely()>>16)+osp->y1;
  }
}

//start floating
imagemove::imagemove(objectdefw *tod,image *ti,int x,int y)
 :GUIrect(guiroot,x,y,x+65,y+12),o(0),i(0)
{
 od=tod;
 bind(0);
 i=ti;
 setmodal(this);
 m.hidecursor();
}

//start draggin from object
imagemove::imagemove(object *to,image *ti,int x,int y)
   :GUIrect(guiroot,x,y,x+65,y+43),o(0),i(0)
   {
    od=(objectdefw *)to->od; //objectdef
    bind(to);
    i=ti;     //image itself
    bringtofront();
    setmodal(this);
    m.hidecursor();
   };

imagemove::~imagemove()
{
 if (!osp && i) delete i; //delete floating image
 setmodal(0);
 m.capture=0;
 m.showcursor();
}


image *swap(image *a,image *b)
{
 image temp;
 temp=*a; *a=*b; *b=temp;
 return b;
}

void imagemove::sendtoback()
{
 if (!osp || !o || !fptr || !cs || !i) return;
 i=swap(i,fptr->getimgptr());
}

void imagemove::bringtofront()
{
 if (!osp || !o || !fptr || !cs || !i) return;
 i=swap(i,fptr->getimgptr()+fptr->numimages-1);
}

void imagemove::backward()
{
 if (!osp || !o || !fptr || !cs || !i) return;
 if (i<=fptr->getimgptr()) return;
 i=swap(i,i-1);
}

void imagemove::forward()
{
 msg.printf(2,"forward");
 if (!osp || !o || !fptr || !cs || !i) return;
 if (i>=fptr->getimgptr()+fptr->numimages-1) return;
 i=swap(i,i+1);
}




//delete the image i from the object o
void imagemove::deleteimage()
{
 if (!i) return;
 if (osp && o && fptr && cs)
 {
  _disable();
  image *newi=new image;
  *newi=*i;

  fptr->numimages--;
  fptr->size-=sizeof(image);
  cs->deletebytes(i,sizeof(image));
  i=newi;

  osp->resetodf(o->onum); //reset all objects that are of this objectdef
  fptr=o->fptr;
  _enable();
 }
// msg.printf(2,"delete image");
}

void imagemove::insertimage()
{
 if (!i) return;
 if (osp && o && fptr && cs)
 {
  _disable();
  image *im=fptr->getimgptr()+fptr->numimages; //image insert location
  fptr->size+=sizeof(image);
  fptr->numimages++;
  im=(image *)cs->insertbytes(im,sizeof(image));
  *im=*i;
  delete i;
  i=im;

  osp->resetodf(o->onum); //reset all objects that are of this objectdef
  fptr=o->fptr;

  _enable();
 }
// msg.printf(2,"insert image");
}

int imagemove::drag(mouse &m)
  {
   if (!i) return 0;
   i->dispx+=m.x-m.oldx;
   i->dispy+=m.y-m.oldy;

   if (osp)
    if (root->hittest(m.x,m.y)!=osp) //we left the object space!
     {
      deleteimage();
      bind(0); //unbind
      resize(width(),12);
      return 1;
     }

   if (!osp)
    if (objspace && root->hittest(m.x,m.y)==objspace) //we entered the currently selected object space
     if (objspace->p && objspace->p->fptr && objspace->p->od==od)
      {
       bind(objspace->p);
       insertimage(); //insert i into our o
       resize(width(),43);
      }
   return 1;
  };

int imagemove::release(mouse &m)
  {
   delete this;
   return 1;
  }
void imagemove::draw(char *dest)
 {
   fill(0);
   outline(2);
   if (!i) return;

   font[1]->drawcentered(od->imgnames ? od->imgnames[i->index] : "<noname>",dest,(x1+x2)/2,y1+2);
   if (osp)
   {
    font[2]->printf(x1+2,y1+12,"X: %d",i->dispx);
    font[2]->printf(x1+2,y1+22,"Y: %d",i->dispy);
    font[3]->printf(x1+2,y1+32,"%c%c",(i->orient&2) ? 'X' : ' ',(i->orient&1) ? 'Y' : ' ');
   } else
   {
    od->id[i->index]->draw(dest,i->dispx,i->dispy,i->orient);
   }
 }
int imagemove::keyhit(char kbscan,char key)
{
 if (i)
 {
  if (key=='x') {i->orient^=2; return 1;}
  if (key=='y') {i->orient^=1; return 1;}

  if (key=='B') {backward(); return 1;}
  if (key=='b') {sendtoback(); return 1;}
  if (key=='F') {forward(); return 1;}
  if (key=='f') {bringtofront(); return 1;}

  if (kbscan==KB_DEL)
   if (i)
   {
    if (fptr) deleteimage();
    delete i;
    i=0;
    delete this;
    return 1;
   }
 }
 return 0;
}





GUIrect *objectspace::click(mouse &m)
{
 if (m.click&MBLEFT)
 {
  if (p && !p->playing)
   {
    if (ep) //effect is being edited
     if (ep->click(m)) return ep; //drag the effect

    image *i=p->hittest(m.x-x1,m.y-y1);
    if (i  && !p->od->readonly)
      if (!maximized)  return new imagemove(p,i,x2+10,y1+10);
                else   return new imagemove(p,i,x2-70,y1+30);
   }

  //do object hit testing
  object *oldp=p;
 // p=0;
  for (object *t=o; t; t=t->next)
   if (t->hittest(m.x-x1,m.y-y1)) p=t; //this object is selected

  if (p!=oldp)
  {
   if (p && !p->creator) inputdevice[0]->bind(p);
   if (!p && bg) root->refresh(GUIRFR_BG,bg);      //bg being edited
            else root->refresh(GUIRFR_OBJECT,p);   //object being edited
  } else
  if (bg  && !bg->bd->readonly)   //do bg hittesting
   switch (bg->emode)
   {
    case BGE_IMAGES:
     {
      bgimage *i=bg->hittest(m.x-x1,m.y-y1);
      if (i) return !maximized ? new bgimagemove(bg,i,x2+10,y1+10) : new bgimagemove(bg,i,x2-70,y1+30);
     }
     break;
    case BGE_LINES:
     {
      bgline *l=bg->linehittest(m.x-x1,m.y-y1);
      if (l && !(kbstat&KB_SHIFT)) return new bglinemove(bg,l);
         else return new bglinedraw(bg,m.x-x1,m.y-y1);
     }
     break;
    case BGE_BOUNDARY:
     {
      boundary *l=bg->boundaryhittest(m.x-x1,m.y-y1);
      if (l && !(kbstat&KB_SHIFT)) return new bgboundarymove(bg,l);
        else return new bgboundarydraw(bg,m.x-x1,m.y-y1);
     }
     break;
   }
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




void objectspace::neweditablebg(int num)
{
 if (bg) delete bg; //delete old bg
 msg.printf(2,"Loading %s...",bgdf[num]->name);
 bg=new bgobject(bgdf[num],0,0);
 p=0;
 root->refresh(GUIRFR_BG,bg); //new bgobject now
}

void objectspace::neweditableobject(int num,int activate)
{
 //find appropriate xcoord
 int x=width()/2,y=0;
 if (bg)
  {
   x+=bg->x>>16;
   y+=bg->y>>16;
  }
 if (p) x=(p->x>>16)+80;

 if (!activate)
 {
  p=new object(odf[num],-1,x,y,0,0,0);
  p->startseries(0);
 } else
 {
  setp(new object(odf[num],0,x,y,0,0,0));
  //inputdevice[0]->bind(p);
 }

 if (p->od->loaded)
  {
   p->forcefloor();
  // p->moveabs(0,p->getbasey()-p->y,0);
 
//   p->y=p->getbasey();
//   if (p->r[0]) p->r[0]->getabsregion();
//   if (p->r[1]) p->r[1]->getabsregion();
//   if (p->r[2]) p->r[2]->getabsregion();
   root->refresh(GUIRFR_OBJECT,p); //new object now
  }
}





//--------------------------------------
//read objectdefs into objectspace

int objectspace::readobjectdefs(char *filename)
{
 FILE *f;
 f=fopen(filename,"rt");
 if (!f) return 0;

 odf=(objectdefw **)calloc(256,sizeof(objectdefw *));

 //create null object
 odf[0]=new objectdefw(this,0,"<null>","","","");
 numodf=1;

 char line[100];
 while (fgets(line,100,f)) //read line into memory
 {
  static int onum;
  static char name[32],sdf[32],dir[32],volfile[32];
  if (sscanf(line,"%d %s %s %s %s",&onum,name,sdf,dir,volfile)<5)  continue;

  //already allocated
  if (odf[onum]) {msg.printf(5,"duplicated objnum %d",onum); continue;}

  odf[onum]=new objectdefw(this,onum,name,volfile,sdf,dir);
  if (onum>=numodf) numodf=onum+1;
 }
 fclose(f);
 //realloc
 odf=(objectdefw **)realloc(odf,numodf*4);
 writeobjbin("object.bin",numodf,(objectdef **)odf);
 return numodf;
};


int objectspace::readbgdefs(char *filename)
{
 FILE *f;
 f=fopen(filename,"rt");
 if (!f) return 0;

 bgdf=(bgdef **)calloc(256,sizeof(bgdef *));

 //create null object
 bgdf[0]=new bgdef(this,0,"<null>","","","");
 numbgdf=1;

 char line[100];
 while (fgets(line,100,f)) //read line into memory
 {
  static int bgnum;
  static char name[32],sdf[32],dir[32],volfile[32];
  if (sscanf(line,"%d %s %s %s %s",&bgnum,name,sdf,dir,volfile)<5)  continue;

  //already allocated
  if (bgdf[bgnum]) {msg.printf(5,"duplicated bgdef %d",bgnum); continue;}

  bgdf[bgnum]=new bgdef(this,bgnum,name,volfile,sdf,dir);
  if (bgnum>=numbgdf) numbgdf=bgnum+1;
 }
 fclose(f);
 //realloc
 bgdf=(bgdef **)realloc(bgdf,numbgdf*4);
 writeobjbin("bg.bin",numbgdf,(objectdef **)bgdf);
 return numbgdf;
};



void objectspace::writeobjbin(char *filename,int num,class objectdef **odf)
{
 FILEIO f;

 struct {
  char name[32];
  char volfile[32];
 } OBJDESC;

 f.create(filename);
 f.writeint(num);
 f.writeint(sizeof(OBJDESC));
 for (int i=0; i<num; i++)
  {
   memset(&OBJDESC,0,sizeof(OBJDESC));
   strcpy(OBJDESC.name,odf[i]->name);
   strcpy(OBJDESC.volfile,odf[i]->volfile);
   f.write(&OBJDESC,sizeof(OBJDESC));
  }
 f.close();
};












