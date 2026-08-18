#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "r2img.h"
#include "font.h"
#include "dd.h"

#include "mouse.h"
#include "message.h"
#include "gui.h"
#include "guicolor.h"
#include "guimenu.h"

#include "effect.h"
#include "effectw.h"
#include "objspace.h"
#include "object.h"
#include "objdef.h"

#include "sound.h"

#include "a32.h"

menu *getneweffectmenu()
{
 static menu *m=0;
 if (m) return m;

 //get number of effects
 for (int i=0; effectnames[i]; i++);

  //create menu
 m=(menu *)calloc(i+1,sizeof(menuitem));

 for (i=0; effectnames[i]; i++)
    m->m[i].text=effectnames[i];

 return m;
}

//----------------------------------------------
//assumes origin of osp is 0,0
void e_newobject::draw(char *dest,object *o)
{
 //draw x
 int x=(o->relx()>>16);
 int y=(o->rely()>>16)+pos.y+(o->relz()>>16)+pos.z;
 if (!o->d) x+=pos.x; else x-=pos.x;
 a95vol.xmark->draw(dest,x-a95vol.xmark->xw/2,y-a95vol.xmark->yw/2);

  //draw first frame
 objectdef *od=onum ? o->osp->odf[onum] : o->od;
 if (!od || !od->loaded) return;
 if (!pos.d) od->sd[s].first->draw(dest,x,y,od);
        else od->sd[s].first->drawflipx(dest,x,y,od);
}


//assumes origin of osp is 0,0
void e_blood::draw(char *dest,object *o)
{
 //draw x
 int x=(o->relx()>>16);
 int y=(o->rely()>>16)+pos.y+(o->relz()>>16)+pos.z;
 if (!o->d) x+=pos.x; else x-=pos.x;
 a95vol.xmark->draw(dest,x-a95vol.xmark->xw/2,y-a95vol.xmark->yw/2);
}




static char rcolor[3]={2,9*16+1,4};
//assumes origin of osp is 0,0
extern char *screen;
void e_region::draw(char *dest,object *o)
{
 int x1,y1,x2,y2;
 //draw rect
 x1=x2=o->osp->x1+(o->relx()>>16);
 y1=y2=o->osp->y1+(o->rely()>>16)+(o->relz()>>16);


 if (!o->d) {x1+=r.p1.x; x2+=r.p2.x;}
       else {x1-=r.p2.x; x2-=r.p1.x;}
 y1+=r.p1.y; y2+=r.p2.y;
 drawrect(dest,rcolor[type],x1,y1,x2-x1,y2-y1);
}


//--------------------------
class effectlistbox:public GUIlistbox
{
 public:
 virtual void drawitems(char *dest, int x,int y)
 {
  for (int j=itemv,i=scroll->getpos(); j>0 && i<numitems; j--,i++,y+=itemheight)
  {
   if (sel==i) drawrect(dest,2,x,y,width()-2,itemheight);
   if (items[i])  font[1]->draw(((effect *)items[i])->getname(),dest,x,y);
  }
 }

 effectlistbox(GUIrect *p,int x,int y,int xw,int iy)
  :GUIlistbox(p,x,y,xw,iy,10) {};
 effect *getselptr() {return (effect *)GUIlistbox::getselptr();}

 void refresheffects(effect *e,int num)
  {
    ITEMPTR *a=resizeitems(num);
    for (int i=0; i<num; i++,e=e->next()) a[i]=(ITEMPTR)e;
    if (num) setsel(0);
  }
 virtual char *getname() {return "effectlistbox";}
};


//---------------------------------------

epane *neweffectpane(class effect *e,class effectedit *edit);

void m_copyeffect();
//dialog for editing effects of object
class effectedit:public panelist
{
 public:
 objectspace *osp; //object space being monitored
 object *o;       //object being monitored
 objectdefw *od;
 series *s;
 frame *fptr;

 effectedit():panelist() {osp=0; o=0; od=0; s=0; fptr=0;}
 virtual void refresh(int r,void *c)
 {
  switch(r)
  {
   case GUIRFR_OBJSPACE:  osp=(objectspace *)c; if (osp) c=osp->p;
   case GUIRFR_OBJECT:  o=(object *)c; if (o) c=o->cs;
   case GUIRFR_OBJDEF: if (o) od=(objectdefw *)o->od; else od=0;
   case GUIRFR_SERIES: s=(series *)c; if (o) c=o->fptr;
   case GUIRFR_FRAME:  fptr=(frame *)c;
       if (first)  first->refresh(GUIRFR_EFFECT,fptr);
  }
 }
 virtual int keyhit(char kbscan,char key)
 {
  if (key=='[')
   {m_copyeffect(); return 1;}
   return 0;
 }
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "effectedit";}

  //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open();
 virtual ~effectedit() {pos.close((GUIbox *)parent);}
};
DLGPOS effectedit::pos;


epane::epane(class effectedit *tee):pane(tee,100,120),ee(tee) {}
epane::~epane()
{
 if (ee && ee->osp) ee->osp->seteffectpane(0);
}

void panelist::refreshsize()
{
 int xw=0,yw=0;
 for (pane *t=first; t; t=t->pnext)
   {xw+=t->width(); if (yw<t->height()) yw=t->height(); }
 resize(xw,yw);
}

int panelist::keyhit(char kbscan,char key)
{
/*
 if (key=='u')
  {
  for (pane *t=first; t; t=t->pnext)
    msg.printf(2,"%p:%p:%p",t->pprev,t,t->pnext);
   return 1;
  }*/
 return GUIrect::keyhit(kbscan,key);
};

//*******************************************************************8
//*******************************************************************8
//*******************************************************************8

//lists the effects of a frame
class frameeffectlist:public epane
{
 frame *fptr; //frameptr that this is monitoring

 //editing controls
 effectlistbox *list;
 GUIbutton *adde,*deletee;

 GUIpopupmenu *aem; //add effect menu
 public:
 frameeffectlist(effectedit *tee):epane(tee),fptr(0)
  {
   list=new effectlistbox(this,5,5,width()-10,(height()-15)/10);
   adde=new GUItextbutton(this,"Add",20,height()-13);
   deletee=new GUItextbutton(this,"Delete",60,height()-13);
   fptr=0;
   aem=0;
  }

 void resetepane()
 {
  if (pnext) delete pnext; //delete all shit to right of us
  if (!ee->osp) return;
  ee->osp->seteffectpane(neweffectpane(list->getselptr(),ee)); //create effect pane
  if (pnext) pnext->refresh(GUIRFR_EFFECT,list->getselptr()); //refresh effectpane
 }

 virtual void refresh(int r,void *c)
 {
  fptr=(frame *)c;
   //fill list box
  int oldselnum=list->getselnum();
  if (fptr) list->refresheffects(fptr->geteffectptr(),fptr->numextra);
       else list->refresheffects(0,0);
  list->setsel(oldselnum);
  resetepane();
 }

 void inserteffect(int type)
 {
  effect *e=neweffect(type);
  if (!e || !ee->fptr) return;

  _disable();
  void *ip=((char *)ee->fptr)+ee->fptr->size; //insertion point
  ee->fptr->size+=e->size();
  ee->fptr->numextra++;
  ip=ee->s->insertbytes(ip,e->size());
  memcpy(ip,e,e->size());
  delete e;
  ee->osp->resetodf(ee->o->onum);
  _enable();
 }

 void deleteeffect()
 {
  effect *e=list->getselptr();
  if (!e || !ee->fptr) return;

  _disable();
  ee->fptr->size-=e->size();
  ee->fptr->numextra--;
  ee->s->deletebytes(e,e->size());
  ee->osp->resetodf(ee->o->onum); //reset all objects that are of this objectdef
 _enable();
 }

 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (c==aem)
    {
     inserteffect(guimsg);
     aem=0;
     return 1;
    }
   if (guimsg==GUIMSG_LISTBOXSELCHANGED && c==list) resetepane();

   if (c==deletee && guimsg==GUIMSG_PUSHED) deleteeffect();
   if (c==adde && guimsg==GUIMSG_PUSHED)
     aem=new GUIpopupmenu(this,getneweffectmenu(),m.x,m.y);

   return 1;
  }
 virtual char *getname() {return "frameeffectlist";}
};



//---------------------------------------
class e_shake_edit:public epane
{
 e_shake *e;

 GUIstringlistbox *list;
 public:
 e_shake_edit(effectedit *tee):epane(tee),e(0)
  {
   new GUIstatictext(this,1,"Intensity:",5,5);
   list=new GUIstringlistbox(this,5,15,width()-10,4,10);
   ITEMPTR *a=list->resizeitems(4);
   a[0]="Weak"; a[1]="Violent"; a[2]="Orgasmic"; a[3]="Epileptic";
  }
 virtual void refresh(int r,void *c)
  {
   e=(e_shake *)c;
   if (!e) {list->clearsel(); return;}
   list->setsel(e->intensity);
  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 0;
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int snum=list->getselnum(); //selected soundnum
       if (snum>=0) e->intensity=snum;
      }
   return 1;
  };
 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual char *getname() {return "e_shake_edit";}
};



class e_sound_edit:public epane
{
 e_sound *e;

 GUIstringlistbox *list;
 GUIstatictext *output;
 GUItextbutton *test;
 public:
 e_sound_edit(effectedit *tee):epane(tee),e(0)
  {
   output=new GUIstaticcenteredtext(this,3,0,5,height()-22,width()-10);
   test=new GUItextbutton(this,"Test",width()/2,height()-13);

   list=new GUIstringlistbox(this,5,5,width()-10,(height()-27)/10,10);
  }
 virtual void refresh(int r,void *c)
  {
   e=(e_sound *)c;
   objectdefw *od=ee->od;
   if (!e || !od) {list->resizeitems(0); return;}
   ITEMPTR *a=list->resizeitems(od->numsounds);
   for (int i=0; i<od->numsounds; i++)
    if (od->sndnames) a[i]=(ITEMPTR)&od->sndnames[i];  else a[i]=0;
   list->setsel(e->sound);
  }

 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 0;
   if (c==test && guimsg==GUIMSG_PUSHED)
      if (ee->o) e->trigger(ee->o);

   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       objectdef *od=ee->od;
       int snum=list->getselnum(); //selected soundnum
       if (snum>=0) e->sound=snum;
       //print info on sound
       #ifdef DOS
       if (snum<0 || !od->sounds[snum]) {output->settext(0);return 1;}
       float sndsize=od->sounds[snum]->soundsize;
       #endif
       #ifdef WIN95
       if (snum<0 || !od->dsounds[snum]) {output->settext(0);return 1;}
       float sndsize=getsoundsize(od->dsounds[snum]);
       #endif
       char s[80];
       sprintf(s,"%2.1fKB %2.2fsec",sndsize/1024,sndsize/SOUNDFREQ);
       output->settext(s);
      }
   return 1;
  };
 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual char *getname() {return "e_sound_edit";}
};





class e_dir_edit:public epane
{
 e_dir *e;

 GUIstringlistbox *list;
 public:
 e_dir_edit(effectedit *ee): epane(ee),e(0)
  {
   list=new GUIstringlistbox(this,5,15,width()-10,4,10);
   ITEMPTR *a=list->resizeitems(3);
   a[0]="Right"; a[1]="Left"; a[2]="Flip";
  }
 virtual void refresh(int r,void *c)
 {
  e=(e_dir *)c;
  if (e) list->setsel(e->d);
    else list->clearsel();
 }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
     {
      int snum=list->getselnum(); //selected soundnum
      if (snum>=0) e->d=snum;
     }
   return 1;
  };
 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual char *getname() {return "e_dir edit";}
};




class e_goto_edit:public epane
{
 e_goto *e;

 GUIstringlistbox *list;
 public:
 e_goto_edit(effectedit *tee):epane(tee),e(0)
  {
   list=new GUIstringlistbox(this,5,5,width()-10,(height()-15)/10,10);
   ITEMPTR *a=list->resizeitems(ee->od->nums);
   for (int i=0; i<ee->od->nums; i++) a[i]=(ITEMPTR)&ee->od->sd[i].name;
  }
 virtual void refresh(int r,void *c)
 {
  e=(e_goto *)c;
  if (e) list->setsel(e->s);
    else list->clearsel();
 }

 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;  
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int snum=list->getselnum(); //selected series
       if (snum>=0) e->s=snum;
      }
   return 1;
  };
 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual char *getname() {return "e_goto_edit";}
};


class e_gotoseriesname_edit:public epane
{
 e_gotoseriesname *e;

 GUItextedit *name;
 public:
 e_gotoseriesname_edit(effectedit *ee):epane(ee),e(0)
  {
   name=new GUItextedit(this,"",0,5,5,width()-10,15);
  }
 virtual void refresh(int r,void *c)
 {
  e=(e_gotoseriesname *)c;
  name->setinput(e ? e->name : "");
 }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==name && guimsg==GUIMSG_EDITCHANGED)
     strcpy(e->name,name->getinput());
   return 1;
  };
 virtual char *getname() {return "e_gotoseriesname edit";}
};


class e_newobject_edit:public epane
{
 e_newobject *e;

 GUIstatictext *output;
 GUIstringlistbox *olist,*slist;
 public:
 void updatepos()
 {
  char s[80];
  sprintf(s,"(%d,%d,%d) %s",e->pos.x,e->pos.y,e->pos.z,e->pos.d ? "flipx" : "");
  output->settext(s);
 }

 e_newobject_edit(effectedit *ee):epane(ee),e(0)
  {
   olist=new GUIstringlistbox(this,5,3,width()-10,4,10);
   slist=new GUIstringlistbox(this,5,46,width()-10,6,10);
   ITEMPTR *a=olist->resizeitems(ee->osp->numodf);
   for (int i=0; i<ee->osp->numodf; i++) a[i]=(ITEMPTR)&ee->osp->odf[i]->name;
   a[0]="<self>";

   output=new GUIstaticcenteredtext(this,1,0,0,height()-10,width());
  }

  virtual void refresh(int r,void *c)
  {
   e=(e_newobject *)c;
   if (e)
    {
     olist->setsel(e->onum);
     updatepos();
    } else
    {
     olist->clearsel();
     output->settext("");
    }
  }

 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==olist && guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int onum=olist->getselnum(); //selected object
       if (onum<0) return 1;
       e->onum=onum;

       objectdef *od=ee->osp->odf[onum ? onum : ee->o->onum];
       ITEMPTR *a=slist->resizeitems(od->nums);
       for (int i=0; i<od->nums; i++) a[i]=(ITEMPTR)&od->sd[i].name;
       slist->setsel(e->s);
      }
   if (c==slist && guimsg==GUIMSG_LISTBOXSELCHANGED)
     {
      int snum=slist->getselnum(); //selected series
      if (snum>=0) e->s=snum;
     }
   return 1;
  };

 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual void draweffect(char *dest)
  {
   object      *o=ee->o;
   if (e) e->draw(dest,o);
  }
 virtual GUIrect *click(mouse &m)
  {
   objectspace *osp=ee->osp;
   object      *o=ee->o;

   //get coord of X
   int x=osp->x1+(o->relx()>>16);
   int y=osp->y1+(o->rely()>>16)+e->pos.y+(o->relz()>>16)+e->pos.z;
   if (!o->d) x+=e->pos.x; else x-=e->pos.x;
   //get dimensions of X
   int xw=a95vol.xmark->xw;
   int yw=a95vol.xmark->yw;

   if (m.x>x-xw/2 && m.x<x+xw/2 && m.y>y-yw/2 && m.y<y+yw/2)
     return this;
   return 0;
  }
 virtual int drag(mouse &m)
  {
    e->pos.x+=m.x-m.oldx;
    e->pos.y+=m.y-m.oldy;
    updatepos();
    return 1;
  };
 virtual int release(mouse &m) {return 1;}

 virtual char *getname() {return "e_newobject edit";}
};



class e_blood_edit:public epane
{
 e_blood *e;

 GUIstatictext *output;
 GUIstringlistbox *list;
 public:
 void updatepos()
 {
  char s[80];
  sprintf(s,"(%d,%d,%d) %s",e->pos.x,e->pos.y,e->pos.z,e->pos.d ? "flipx" : "");
  output->settext(s);
 }
 e_blood_edit(effectedit *tee):epane(tee)
  {
   list=new GUIstringlistbox(this,5,5,width()-10,7,10);
   ITEMPTR *a=list->resizeitems(4);
   a[0]="Squirt"; a[1]="Drip"; a[2]="Explosion"; a[3]="Flood";
   output=new GUIstaticcenteredtext(this,1,0,0,height()-10,width());
  }

 virtual void refresh(int r,void *c)
 {
   e=(e_blood *)c;
   if (e)
    {
     list->setsel(e->bloodtype);
     updatepos();
    } else
    {
     list->clearsel();
     output->settext("");
    }
 }
 virtual int keyhit(char key,char kbscan) {return 0;}
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int bnum=list->getselnum(); //selected object
       if (bnum>=0) e->bloodtype=bnum;
      }
   return 1;
  };

 virtual void draweffect(char *dest)
  {
   if (e) e->draw(dest,ee->o);
  }
 virtual GUIrect *click(mouse &m)
  {
   objectspace *osp=ee->osp;
   object      *o=ee->o;
   //get coord of X
   int x=osp->x1+(o->relx()>>16);
   int y=osp->y1+(o->rely()>>16)+e->pos.y+(o->relz()>>16)+e->pos.z;
   if (!o->d) x+=e->pos.x; else x-=e->pos.x;
   //get dimensions of X
   int xw=a95vol.xmark->xw;
   int yw=a95vol.xmark->yw;

   if (m.x>x-xw/2 && m.x<x+xw/2 && m.y>y-yw/2 && m.y<y+yw/2)
     return this;
   return 0;
  }
 virtual int drag(mouse &m)
  {
    e->pos.x+=m.x-m.oldx;
    e->pos.y+=m.y-m.oldy;
    updatepos();
    return 1;
  };
 virtual int release(mouse &m) {return 1;}
 virtual char *getname() {return "e_blood edit";}
};



//--------------------------------------
//region stuff, woo hoo
class impermflags:public epane
{
 uchar *f;

 GUIcheckbox *b[5];

 public:
 impermflags(effectedit *tee):epane(tee)
  {
   int ty=5;
   b[0]=new GUIcheckbox(this,"pusher",0,ty,0); ty+=10;
   b[1]=new GUIcheckbox(this,"pushable",0,ty,0); ty+=10;
   b[2]=new GUIcheckbox(this,"standontopable",0,ty,0); ty+=10;
   b[3]=new GUIcheckbox(this,"bounce",0,ty,0); ty+=10;
   b[4]=new GUIcheckbox(this,"stationary",0,ty,0); ty+=10;
   f=0;
   losechildfocus();
   lastfocus=0;
  }
 void refresh(int r,void *c)
  {
   f=(uchar *)c;
   if (!f) return;
   for (int i=0; i<5; i++)  b[i]->setstate(*f&(1<<i));
  }
 int sendmessage(GUIrect *c,int guimsg)
  {
   if (!f) return 1;
   if (guimsg==GUIMSG_CHECKED || guimsg==GUIMSG_UNCHECKED)
    for (int i=0; i<5; i++)
     if (b[i]==c)
      {
       if (guimsg==GUIMSG_CHECKED) {*f|=(1<<i); }
       if (guimsg==GUIMSG_UNCHECKED) {*f&=~(1<<i); }
       return 1;
      }
   return 1;
  }
 virtual int acceptfocus() {return 1;}

};

class vulnflags:public epane
{
 uchar *f;
 GUIcheckbox *b[3];

 public:
 vulnflags(effectedit *tee):epane(tee)
  {
   int ty=5;
   b[0]=new GUIcheckbox(this,"player",0,ty,0); ty+=10;
   b[1]=new GUIcheckbox(this,"enemy",0,ty,0); ty+=10;
   b[2]=new GUIcheckbox(this,"other",0,ty,0); ty+=10;
   f=0;
   losechildfocus();
   lastfocus=0;
  }
 void refresh(int r,void *c)
  {
   f=(uchar *)c;
   if (!f) return;
   for (int i=0; i<3; i++) b[i]->setstate(*f&(1<<i));
  }
 int sendmessage(GUIrect *c,int guimsg)
  {
   if (!f) return 1;
   if (guimsg==GUIMSG_CHECKED || guimsg==GUIMSG_UNCHECKED)
    for (int i=0; i<3; i++)
     if (b[i]==c)
      {
       if (guimsg==GUIMSG_CHECKED) *f|=(1<<i);
       if (guimsg==GUIMSG_UNCHECKED) *f&=~(1<<i);
       return 1;
      }
   return 1;
  }
 virtual int acceptfocus() {return 1;}

};

class attackflags:public epane
{
 uchar *f;
 GUIcheckbox *b[4];
 public:
 attackflags(effectedit *tee):epane(tee)
  {
   int ty=5;
   b[0]=new GUIcheckbox(this,"hits player",0,ty,0); ty+=10;
   b[1]=new GUIcheckbox(this,"hits enemy",0,ty,0); ty+=10;
   b[2]=new GUIcheckbox(this,"hits other",0,ty,0); ty+=10;
   b[3]=new GUIcheckbox(this,"force facing",0,ty,0); ty+=10;
   f=0;
   losechildfocus();
   lastfocus=0;
  }
 void refresh(int r,void *c)
  {
   f=(uchar *)c;
   if (!f) return;
   for (int i=0; i<4; i++) b[i]->setstate(*f&(1<<i));
  }
 int sendmessage(GUIrect *c,int msg)
  {
   if (!f) return 1;
   if (msg==GUIMSG_CHECKED || msg==GUIMSG_UNCHECKED)
    for (int i=0; i<4; i++)
     if (b[i]==c)
      {
       if (msg==GUIMSG_CHECKED) *f|=(1<<i);
       if (msg==GUIMSG_UNCHECKED) *f&=~(1<<i);
       return 1;
      }
   return 1;
  }
 virtual int acceptfocus() {return 1;}
};


class regioneffectlist:public epane
{
 e_region *e;

 effectlistbox *listl,*listr;
 GUIbutton *addl,*deletel;
 GUIbutton *addr,*deleter;
 GUIpopupmenu *alem,*arem; //add effect menus

 public:
 regioneffectlist(effectedit *tee):epane(tee),e(0)
 {
  alem=arem=0;
  new GUIstatictext(this,1,"Local:",5,5);
  listl=new effectlistbox(this,5,15,width()-10,3);
  addl=new GUItextbutton(this,"Add",20,48);
  deletel=new GUItextbutton(this,"Delete",60,48);

  new GUIstatictext(this,1,"Remote:",5,62);
  listr=new effectlistbox(this,5,72,width()-10,3);
  addr=new GUItextbutton(this,"Add",20,105);
  deleter=new GUItextbutton(this,"Delete",60,105);
 }

 effect *geteffectptr()
 {
  if (listl->getselptr()) return listl->getselptr();
  if (listr->getselptr()) return listr->getselptr();
  return 0;
 }

 void resetepane()
 {
  if (pnext) delete pnext; //delete all shit to right of us
  epane *ep=neweffectpane(geteffectptr(),ee);
  if (ee && ee->osp) ee->osp->seteffectpane(ep);
  if (ep) ep->refresh(GUIRFR_EFFECT,geteffectptr()); //refresh effectpane
 }

 void refresh(int r,void *c)
 {
  e=(e_region *)c;
  if (e)
  {
   listl->refresheffects(e->getleptr(),e->numle);
   listr->refresheffects(e->getreptr(),e->numre);
  }
  listl->clearsel();
  listr->clearsel();
  resetepane();
 }

 void insertleffect(int num)
 {
  if (!ee || !ee->fptr) return;
  effect *t=neweffect(num);
  if (!t) return;

   //insertion point
  _disable();
  void *ip=e->getleptr();
  ee->fptr->size+=t->size();
  e->numle++;
  ip=ee->s->insertbytes(ip,t->size());
  memcpy(ip,t,t->size());
  delete t;
  ee->osp->resetodf(ee->o->onum);
  _enable();
 }

 void insertreffect(int num)
 {
  if (!ee || !ee->fptr) return;
  effect *t=neweffect(num);
  if (!t) return;

   //insertion point
  _disable();
  void *ip=e->getreptr();
  ee->fptr->size+=t->size();
  e->numre++;
  ip=ee->s->insertbytes(ip,t->size());
  memcpy(ip,t,t->size());
  delete t;
  ee->osp->resetodf(ee->o->onum);
  _enable();
 }

 void deleteleffect()
 {
  if (!ee) return;
  effect *t=listl->getselptr();
  if (!e || !t  || !ee->fptr) return;

  _disable();
  ee->fptr->size-=t->size();
  e->numle--;
  ee->s->deletebytes(t,t->size());
  ee->osp->resetodf(ee->o->onum); //reset all objects that are of this objectdef
 _enable();
 }


 void deletereffect()
 {
  if (!ee) return;
  effect *t=listr->getselptr();
  if (!e || !t  || !ee->fptr) return;

  _disable();
  ee->fptr->size-=t->size();
  e->numre--;
  ee->s->deletebytes(t,t->size());
  ee->osp->resetodf(ee->o->onum); //reset all objects that are of this objectdef
 _enable();
 }


 int sendmessage(GUIrect *c,int guimsg)
  {
   if (!c) return 1;
   if (c==alem)
    {
     alem=0;
     if (guimsg!=E_REGION) insertleffect(guimsg);
     return 1;
    }
   if (c==arem)
    {
     arem=0;
     if (guimsg!=E_REGION) insertreffect(guimsg);
     return 1;
    }

   if (c==addl && guimsg==GUIMSG_PUSHED) alem=new GUIpopupmenu(this,getneweffectmenu(),m.x,m.y);
   if (c==addr && guimsg==GUIMSG_PUSHED) arem=new GUIpopupmenu(this,getneweffectmenu(),m.x,m.y);

   if (c==deletel && guimsg==GUIMSG_PUSHED) deleteleffect();
   if (c==deleter && guimsg==GUIMSG_PUSHED) deletereffect();

   if ((c==listl || c==listr) && guimsg==GUIMSG_LISTBOXSELCHANGED)
   {
    if (c==listl) listr->clearsel();
    if (c==listr) listl->clearsel();
    resetepane();
   }
  return 1;
  };

 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "regioneffectlist";}
};



class e_region_edit:public epane
{
 e_region *e;

 GUIstringlistbox *list;
 GUIcheckbox *append;
 zrect r; //temp rect for draggin
 GUIrect *flags;
 GUItextbutton *regedit,*flagedit;

 GUIstatictext *output1,*output2;

 static int showingflags,showingeffects;
 public:

 void updatepos()
 {
  char s[80];
  sprintf(s,"(%d,%d,%d)",e->r.p1.x,e->r.p1.y,e->r.p1.z);
  output1->settext(s);

  sprintf(s,"(%d,%d,%d)",e->r.p2.x,e->r.p2.y,e->r.p2.z);
  output2->settext(s);
 }

 void hide()
 {
  if (pnext) delete pnext;
  if (showingflags)
   {
    flagedit->settext("flags>>");
    showingflags=0;
   }
  if (showingeffects)
   {
    regedit->settext("effects>>");
    showingeffects=0;
   }
  if (ee && ee->osp)
   ee->osp->seteffectpane(this); //create effect pane
 }

 void showflags()
 {
  if (pnext) hide();
  if (!e) return;
  switch(e->type)
  {
   case R_IMPERM: new impermflags(ee); break;
   case R_VULN: new vulnflags(ee); break;
   case R_ATTACK: new attackflags(ee); break;
  }
  ee->losechildfocus();
  if (pnext)
   {
    pnext->refresh(GUIRFR_EFFECT,&e->flags);
    pnext->losefocus();

    pnext->lastfocus=0;
    showingflags=1;
    flagedit->settext("<<flags");
   }
 }

 void showeffects()
 {
  if (pnext) hide();
  if (!e) return;
  new regioneffectlist(ee);
  if (pnext)
   {
    pnext->refresh(GUIRFR_EFFECT,e);
    showingeffects=1;
    regedit->settext("<<effects");
   }
 }

 e_region_edit(effectedit *tee):epane(tee),e(0)
  {
   flags=0;
   list=new GUIstringlistbox(this,5,5,width()-10,3,10);
   ITEMPTR *a=list->resizeitems(3);
   a[0]="imperm"; a[1]="vuln"; a[2]="attack";
   append=new GUIcheckbox(this,"append",10,38,0);
   flagedit=new GUItextbutton(this,"flags>>",width()/2,height()-27);
   regedit=new GUItextbutton(this,"effects>>",width()/2,height()-13);
   output1=new GUIstaticcenteredtext(this,3,0,5,50,width()-10);
   output2=new GUIstaticcenteredtext(this,3,0,5,61,width()-10);
  }

 virtual void refresh(int r,void *c)
 {
   e=(e_region *)c;
   if (e)
    {
     list->setsel(e->type);
     append->setstate(e->append);
     if (showingflags) showflags(); else
     if (showingeffects) showeffects();
     updatepos();
    } else
    {
     list->clearsel();
     output1->settext(0);
     output2->settext(0);
     hide();
    }
 }

 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
     if (list->getselnum()>=0)
      {
       if (list->getselnum()!=e->type)  e->settype(list->getselnum());
       if (showingflags) showflags();
      }
   if (c==append) e->append=append->getstate();
   if (c==flagedit && guimsg==GUIMSG_PUSHED)
    {if (showingflags) hide(); else showflags();}
   if (c==regedit && guimsg==GUIMSG_PUSHED)
    {if (showingeffects) hide(); else showeffects();}
   return 1;
  };
 virtual char *getname() {return "e_region edit";}

 virtual void draweffect(char *dest)
  {
   if (e) e->draw(dest,ee->o);
  }

 virtual GUIrect *click(mouse &m)
  {
   objectspace *osp=ee->osp;
   object      *o=ee->o;
   if (!osp->hittest(m.x,m.y))
    {
     return 0;
    }
   r.clear();
   r.p1.x=m.x-osp->x1-(o->relx()>>16);
   r.p1.y=m.y-osp->y1-(o->rely()>>16);
   r.p1.z=-5;     //set first coord
   return this;
  }
 virtual int drag(mouse &m)
 {
  objectspace *osp=ee->osp;
  object      *o=ee->o;
  r.p2.x=m.x-osp->x1-(o->relx()>>16);
  r.p2.y=m.y-osp->y1-(o->rely()>>16);
  r.p2.z= 5;     //set second coord
  e->r=r; //set region rect
  e->r.reorient();
     updatepos();
  return 1;
 }
 virtual int release(mouse &m) {return 1;}
};

int e_region_edit::showingflags=0;
int e_region_edit::showingeffects=0;







class e_eraseregion_edit:public epane
{
 e_eraseregion *e;

 GUIstringlistbox *list;
 public:
 e_eraseregion_edit(effectedit *tee):epane(tee),e(0)
  {
   list=new GUIstringlistbox(this,5,5,width()-10,4,10);
   ITEMPTR *a=list->resizeitems(4);
   a[0]="imperm"; a[1]="vuln"; a[2]="attack"; a[3]="all";
  }

 virtual void refresh(int r,void *c)
 {
   e=(e_eraseregion *)c;
   if (e) list->setsel(e->type);
     else list->clearsel();
 }

 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED)
     if (list->getselnum()>=0) e->type=list->getselnum();
   return 1;
  };
 virtual char *getname() {return "e_eraseregion edit";}
};



class e_loseenergy_edit:public epane
{
 e_loseenergy *e;

 GUIintedit *edit;
 public:
 e_loseenergy_edit(effectedit *tee):epane(tee),e(0)
  {
   edit=new GUIintedit(this,"Energy: ",0,5,35,0,0,65535);
  }

 virtual void refresh(int r,void *c)
 {
   e=(e_loseenergy *)c;
   if (e) edit->set(e->energy);
 }

 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==edit && guimsg==GUIMSG_EDITCHANGED)
     e->energy=edit->get();
   return 1;
  };
 virtual char *getname() {return "e_loseenergy edit";}
};





class e_onenergy_edit:public epane
{
 e_onenergy *e;

 GUIstringlistbox *list;
 GUIintedit *edit;
 GUItextbutton *then;

 GUIpopupmenu *aem; //add effect menus
 public:
 e_onenergy_edit(effectedit *tee):epane(tee),e(0)
  {
   new GUIstatictext(this,1,"If energy is...",5,5);
   list=new GUIstringlistbox(this,5,18,width()-10,3,10);
   ITEMPTR *a=list->resizeitems(3);
   a[0]="equal to"; a[1]="less than"; a[2]="greater than";

   edit=new GUIintedit(this,"",15,54,28,0,-30000,30000);
   new GUIstatictext(this,1,"then...",5,72);
   then=0;
   aem=0;
  }

 effect *geteffectptr()
 {
  if (!e) return 0;
  return &e->e;
 }

 void resetepane()
 {
  if (pnext) delete pnext; //delete all shit to right of us
  epane *ep=neweffectpane(geteffectptr(),ee);
  if (ee && ee->osp) ee->osp->seteffectpane(ep);
  if (ep) ep->refresh(GUIRFR_EFFECT,geteffectptr()); //refresh effectpane
 }


 virtual void refresh(int r,void *c)
 {
   e=(e_onenergy *)c;
   if (!e) return;
   list->setsel(e->condition);
   edit->set(e->energy);

   if (then) delete then; then=0;
   then=new GUItextbutton(this,e->e.getname(),width()/2,85);
   resetepane();
 }

 void changeeffect(int num)
 {
  if (!ee || !ee->fptr) return;
  effect *t=neweffect(num);
  if (!t) return;

  //insertion point
  _disable();
  void *ip=geteffectptr();
  ee->fptr->size-=e->e.size();
  ee->fptr->size+=t->size();

  ip=ee->s->deletebytes(ip,e->e.size()); //delete old effect
  ip=ee->s->insertbytes(ip,t->size());
  memcpy(ip,t,t->size());
  delete t;

  ee->osp->resetodf(ee->o->onum);
  _enable();
 }


 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==aem)
    {
     changeeffect(guimsg);
     aem=0;
     return 1;
    }

   if (c==edit && guimsg==GUIMSG_EDITCHANGED) e->energy=edit->get();
   if (c==list && guimsg==GUIMSG_LISTBOXSELCHANGED) e->condition=list->getselnum();
   if (c==then && guimsg==GUIMSG_PUSHED) aem=new GUIpopupmenu(this,getneweffectmenu(),m.x,m.y);
   return 1;
  };
 virtual char *getname() {return "e_onenergy edit";}
};





class e_randomtraj_edit:public epane
{
 e_randomtraj *e;

 GUIintedit *dur;
 GUIfloatedit *dxmin,*dxmax;
 GUIfloatedit *dymin,*dymax;
 GUIfloatedit *dzmin,*dzmax;
 public:
 e_randomtraj_edit(effectedit *tee):epane(tee),e(0)
  {
  dur=new GUIintedit(this,"dur: ",5,55,25,0,1,65535);
  dxmin=new GUIfloatedit(this,"dX:",0,5,25,0,-100.00,100.00);
  dymin=new GUIfloatedit(this,"dY:",0,20,25,0,-100.00,100.00);
  dzmin=new GUIfloatedit(this,"dZ:",0,35,25,0,-100.00,100.00);

  dxmax=new GUIfloatedit(this,"-",52,5,25,0,-100.00,100.00);
  dymax=new GUIfloatedit(this,"-",52,20,25,0,-100.00,100.00);
  dzmax=new GUIfloatedit(this,"-",52,35,25,0,-100.00,100.00);
  }

 virtual void refresh(int r,void *c)
 {
   e=(e_randomtraj *)c;
   if (!e) return;
   dur->set(e->dur);
   dxmin->set(((float)e->dxmin)/0x100);
   dymin->set(((float)e->dymin)/0x100);
   dzmin->set(((float)e->dzmin)/0x100);

   dxmax->set(((float)e->dxmax)/0x100);
   dymax->set(((float)e->dymax)/0x100);
   dzmax->set(((float)e->dzmax)/0x100);
 }

 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (guimsg==GUIMSG_EDITCHANGED)
    {
     if (c==dur) e->dur=dur->get();
     if (c==dxmin) e->dxmin=(int)(dxmin->get()*0x100);
     if (c==dymin) e->dymin=(int)(dymin->get()*0x100);
     if (c==dzmin) e->dzmin=(int)(dzmin->get()*0x100);

     if (c==dxmax) e->dxmax=(int)(dxmax->get()*0x100);
     if (c==dymax) e->dymax=(int)(dymax->get()*0x100);
     if (c==dzmax) e->dzmax=(int)(dzmax->get()*0x100);
    }
   return 1;
  };
 virtual char *getname() {return "e_randomtraj edit";}
};








class e_onhitground_edit:public epane
{
 e_onhitground *e;

 GUItextbutton *then;

 GUIpopupmenu *aem; //add effect menus
 public:
 e_onhitground_edit(effectedit *tee):epane(tee),e(0)
  {
   new GUIstatictext(this,1,"If hit ground...",5,5);
   then=0;
   aem=0;
  }

 effect *geteffectptr()
 {
  if (!e) return 0;
  return &e->e;
 }

 void resetepane()
 {
  if (pnext) delete pnext; //delete all shit to right of us
  epane *ep=neweffectpane(geteffectptr(),ee);
  if (ee && ee->osp) ee->osp->seteffectpane(ep);
  if (ep) ep->refresh(GUIRFR_EFFECT,geteffectptr()); //refresh effectpane
 }


 virtual void refresh(int r,void *c)
 {
   e=(e_onhitground *)c;
   if (!e) return;

   if (then) delete then; then=0;
   then=new GUItextbutton(this,e->e.getname(),width()/2,20);
   resetepane();
 }

 void changeeffect(int num)
 {
  if (!ee || !ee->fptr) return;
  effect *t=neweffect(num);
  if (!t) return;

  //insertion point
  _disable();
  void *ip=geteffectptr();
  ee->fptr->size-=e->e.size();
  ee->fptr->size+=t->size();

  ip=ee->s->deletebytes(ip,e->e.size()); //delete old effect
  ip=ee->s->insertbytes(ip,t->size());
  memcpy(ip,t,t->size());
  delete t;

  ee->osp->resetodf(ee->o->onum);
  _enable();
 }


 virtual int keyhit(char key,char kbscan) { return 0;  }
 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (!e) return 1;
   if (c==aem)
    {
     changeeffect(guimsg);
     aem=0;
     return 1;
    }
   if (c==then && guimsg==GUIMSG_PUSHED) aem=new GUIpopupmenu(this,getneweffectmenu(),m.x,m.y);
   return 1;
  };
 virtual char *getname() {return "e_onhitground edit";}
};


//*******************************************************************8
//*******************************************************************8
//*******************************************************************8
void effectedit::open()
{
  effectedit *ee=new effectedit();
  new frameeffectlist(ee); //create first frame effect list
  pos.open(new GUIbox(guiroot,"Effects",ee,0,0));
  ee->refresh(GUIRFR_OBJSPACE,objspace);
};

void m_editeffects()
{
 effectedit::open();

}




epane *neweffectpane(class effect *e,class effectedit *ee)
{
 if (!e) return 0;
 switch(e->gettype())
 {
   case E_SOUND: return new e_sound_edit(ee);
   case E_OBJECTSHAKE:
   case E_GROUNDSHAKE: return new e_shake_edit(ee);
   case E_GOTOSERIES: return new e_goto_edit(ee);
   case E_GOTOSERIESNAME: return new e_gotoseriesname_edit(ee);
   case E_NEWOBJECT: return new e_newobject_edit(ee);
   case E_DIR: return new e_dir_edit(ee);
   case E_BLOOD: return new e_blood_edit(ee);
   case E_REGION: return new e_region_edit(ee);
   case E_ERASEREGION: return new e_eraseregion_edit(ee);
   case E_LOSEENERGY: return new e_loseenergy_edit(ee);
   case E_ONENERGY: return new e_onenergy_edit(ee);
   case E_RANDOMTRAJ: return new e_randomtraj_edit(ee);
   case E_ONHITGROUND: return new e_onhitground_edit(ee);
 };
 return 0;
};













































