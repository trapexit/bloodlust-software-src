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
#include "guimenu.h"
#include "guicolor.h"

#include "malloc.h"
#include "amenu.h"

#include "keyb.h"

#include "object.h"
#include "bg.h"
#include "objspace.h"

#include "clip.h"

#include "config.h"

#include "a32.h"

#include "imgmove.h"



//----------------------
//  GUI messagebox
//----------------------
class GUImessage:public GUIcontents
{
 class msgbuffer *m;
 GUIpopupmenu *pm;
 public:
 GUImessage(class msgbuffer *tm):
  GUIcontents(250,7*10) {m=tm; pm=0;}

 virtual void draw(char *dest) { fill(CLR_BOX); m->draw(x1+2,y1);}
 virtual char *getname() {return "message";}

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIbox(guiroot,"Message",new GUImessage(&msg),0,0));
   cfg->set(CFG_SHOWMESSAGE,1);
  };
 virtual ~GUImessage() {pos.close((GUIbox *)parent);    cfg->set(CFG_SHOWMESSAGE,0);}
};
DLGPOS GUImessage::pos;



//monitors object/series/frame etc
class GUImonitor:public GUIcontents
{
 protected:
 objectspace *osp;

 bgobject *bg;
 bgdef *bd;

 object    *o;
 objectdefw *od;
 series *s;
 frame *f;

 public:
 GUImonitor(int xw,int yw):GUIcontents(xw,yw)
  {
   osp=0; bg=0; bd=0; o=0; od=0; s=0; f=0;
  }

 virtual void refreshobjspace() {}
 virtual void refreshbg() {}
 virtual void refreshobject() {}
 virtual void refreshobjdef() {}
 virtual void refreshbgdef() {}
 virtual void refreshseries() {}
 virtual void refreshframe() {}
 virtual void refreshstopped() {}

 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);}
 virtual int acceptfocus() {return 1;}

 virtual void refresh(int r,void *c)
 {
   switch(r)
   {
    case GUIRFR_OBJSPACE:
//        if (osp==(objectspace *)c) return;
        osp=(objectspace *)c;
        if (!osp) {refresh(GUIRFR_BG,0); refresh(GUIRFR_OBJECT,0); }
         else
       // if (osp->p) {refresh(GUIRFR_BG,0); refresh(GUIRFR_OBJECT,osp->p); }
        //     else   {refresh(GUIRFR_OBJECT,0); refresh(GUIRFR_BG,osp->bg);}
        {
         refresh(GUIRFR_BG,osp->bg); refresh(GUIRFR_OBJECT,osp->p);
        }
        refreshobjspace();
        return;

    case GUIRFR_BG:
//        if (bg==(bgobject *)c) return;
        bg=(bgobject *)c;
        //if (bg) refresh(GUIRFR_OBJECT,0);
        refresh(GUIRFR_BGDEF,bg ? bg->bd : 0);
        refreshbg();
        return;

    case GUIRFR_OBJECT:
  //     if (o==(object *)c) return;
        o=(object *)c;
        //if (o) refresh(GUIRFR_BG,0);
        refresh(GUIRFR_OBJDEF,o ? o->od : 0);
        refresh(GUIRFR_SERIES,o ? o->cs : 0);
        refresh(GUIRFR_FRAME,o ? o->fptr : 0);
        refreshobject();
        return;

    case GUIRFR_OBJDEF:
  //      if (od==(objectdefw *)c) return;
        od=(objectdefw *)c;
        refreshobjdef();
        return;

    case GUIRFR_BGDEF:
   //     if (bd==(bgdef *)c) return;
        bd=(bgdef *)c;
        refreshbgdef();
        return;

    case GUIRFR_SERIES:
  //      if (s==(series *)c) return;
        s=(series *)c;
        refreshseries();
        return;

    case GUIRFR_FRAME:
  //      if (f==(frame *)c) return;
        f=(frame *)c;
        refreshframe();
        return;

    case GUIRFR_STOPPED:
        refreshstopped();
        return;
   }
 }

};



//----------------------
//   load bgdef
//----------------------
class loadbgdlg:public GUImonitor
{
 GUIstringlistbox *list;

 public:
 loadbgdlg(objectspace *tosp):GUImonitor(120,110)
 {
  list=new GUIstringlistbox(this,5,5,width()-10,9,10);
  refresh(GUIRFR_OBJSPACE,tosp);
 }
 virtual char *getname() {return "loadbg";}

 int sendmessage(GUIrect *c,int guimsg)
  {
   if (guimsg==GUIMSG_OK || guimsg==GUIMSG_LISTBOXDBLCLICKED)
    {
     int num=list->getselnum()+1;
     if (!num) return 0;
     if (osp)
      {
       osp->neweditablebg(num);
      }
    }
   return 1;
  };
 virtual void refreshobjspace()
 {
   if (!osp) {list->resizeitems(0); return;}
   //fill list box
   ITEMPTR *a=list->resizeitems(osp->numbgdf-1);
   for (int i=1; i<osp->numbgdf; i++)
    a[i-1]=(ITEMPTR)&osp->bgdf[i]->name;
 }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Load background",new loadbgdlg(objspace),"Load",0,0));
  };
 virtual ~loadbgdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS loadbgdlg::pos;






//----------------------
//   load objectdef
//----------------------
class loadobjectdlg:public GUImonitor
{
 GUIstringlistbox *list;
 GUIcheckbox *act;

 public:
 loadobjectdlg(objectspace *tosp):GUImonitor(120,110)
 {
  list=new GUIstringlistbox(this,5,5,width()-10,9,10);
  act=new GUIcheckbox(this,"activate",30,height()-12,cfg->get(CFG_LOADACTIVATE));
  refresh(GUIRFR_OBJSPACE,tosp);
 }
 virtual char *getname() {return "loadobject";}

 int sendmessage(GUIrect *c,int guimsg)
  {
   if (guimsg==GUIMSG_OK || guimsg==GUIMSG_LISTBOXDBLCLICKED)
    {
     cfg->set(CFG_LOADACTIVATE,act->getstate());
     int num=list->getselnum()+1;
     if (!num) return 0;
     if (osp)
       osp->neweditableobject(num,cfg->get(CFG_LOADACTIVATE));
    }
   return 1;
  };
 virtual void refreshobjspace()
 {
  if (!osp) {list->resizeitems(0); return;}
  //fill list box
  ITEMPTR *a=list->resizeitems(osp->numodf-1);
  for (int i=1; i<osp->numodf; i++)
    a[i-1]=(ITEMPTR)&osp->odf[i]->name;
 }

 virtual int keyhit(char kbscan,char key)
 {
  if (key=='v')
  {
   if (list->getselnum()<0) return 1;
   if (osp) osp->odf[list->getselnum()+1]->volumize();
   return 1;
  }

  return GUIrect::keyhit(kbscan,key);
 }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUIonebuttonbox(guiroot,"Load object",new loadobjectdlg(objspace),"Load",0,0));
  };
 virtual ~loadobjectdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS loadobjectdlg::pos;






//----------------------
//   select control function
//----------------------
class selectcfunc:public GUImonitor
{
 protected:
 GUIstringlistbox *list;

 public:
 selectcfunc(objectspace *tosp):GUImonitor(140,180)
 {
  list=new GUIstringlistbox(this,5,5,width()-10,17,10);

  if (!cfuncnames) list->resizeitems(0);
   else
   {
   //fill list box
   ITEMPTR *a=list->resizeitems(cfuncnum);
   for (int i=0; i<cfuncnum; i++)
    a[i]=(ITEMPTR)&((*cfuncnames)[i]);
   }

  refresh(GUIRFR_OBJSPACE,tosp);
 }
 int sendmessage(GUIrect *c,int guimsg)
  {
   if (c==list)
    {
     if (guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int x=list->getselnum(); //selected cfunc
       if (x<0) {return 1;}
       if (s)  s->cfunc=x;
       root->refresh(GUIRFR_SERIES,s);
      }
    }

    if (guimsg==GUIMSG_B1) //none
     {list->setsel(s->cfunc=0); return 0;}

    if (guimsg==GUIMSG_B2) //set all!
     {
      if (od)
       {
        for (int i=0; i<od->nums; i++)
             od->sd[i].cfunc=list->getselnum();
       }
      return 0;
     }

   return 1;
  };

 virtual void refreshseries()
 {
   if (s) list->setsel(s->cfunc);
 }
 virtual int acceptfocus() {return 0;}
 virtual char *getname() {return "selectcfunc";}
 virtual int keyhit(char kbscan,char key) {return 0;}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUItwobuttonbox(guiroot,"Control function",new selectcfunc(objspace),"None","Set all",0,0));
  };
 virtual ~selectcfunc() {pos.close((GUIbox *)parent);}
};
DLGPOS selectcfunc::pos;









//----------------------
//   select series
//----------------------
class selectseries:public GUImonitor
{
 protected:
 GUIstringlistbox *list;
 GUIstatictext *output;

 public:
 selectseries():GUImonitor(130,150)
 {
  list=new GUIstringlistbox(this,5,5,width()-10,13,10);
  output=new GUIstaticcenteredtext(this,3,0,5,height()-12,width()-10);
 }
 int sendmessage(GUIrect *c,int guimsg)
  {
   if (c==list)
    {
     if (guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int x=list->getselnum(); //selected seriesnum
       if (x<0) {output->settext(0);return 1;}
       series *s=&od->sd[x];

       char str[80];
       sprintf(str,"#%d: %dframes",x,s->nf);
       output->settext(str);
      }
    }
   return 1;
  };
 virtual int keyhit(char kbscan,char key) { return 0;};

 virtual void refreshobjdef()
 {
   if (!od) {list->resizeitems(0); return;}
   //fill list box
   ITEMPTR *a=list->resizeitems(od->nums);
   for (int i=0; i<od->nums; i++) a[i]=(ITEMPTR)&od->sd[i].name;
 }
 virtual char *getname() {return "selectseries";}
};



//-------------------------------------
// dialog for changing afterseries
//-------------------------------------
class afterseries:public selectseries
{
 public:
 virtual char *getname() {return "afterseries";}
 afterseries(objectspace *tosp):selectseries() {refresh(GUIRFR_OBJSPACE,tosp); };
 virtual void refreshobjdef()
  {
   selectseries::refreshobjdef();
   if (od)
     {
      ITEMPTR *a=list->resizeitems(od->nums+2);
      a[0]="<default>";
      a[od->nums]="<lastforever>";
      a[od->nums+1]="<suicide>";
     }
  }

 virtual void refreshseries()
  {
    if (s)
    {
      if (s->next==SP_LASTFOREVER) list->setsel(od->nums);
        else
      if (s->next==SP_SUICIDE) list->setsel(od->nums+1);
         else  list->setsel(s->next);
    }
  }
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_B2)
   {
    if (o) list->setsel(o->csnum);
    guimsg=GUIMSG_B1;
   }
   if (guimsg==GUIMSG_B1) //select
      {
       int x=list->getselnum(); //selected seriesnum
       if (x<0) return 1;
       if (o)
        {
         if (x==od->nums) s->next=SP_LASTFOREVER; else
         if (x==od->nums+1) s->next=SP_SUICIDE; else
            s->next=x;
         root->refresh(GUIRFR_SERIES,s);
        }
      return 0;
      }
  return selectseries::sendmessage(c,guimsg);
 }

 virtual int keyhit(char kbscan,char key) {return 0;}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUItwobuttonbox(guiroot,"Select afterseries",new afterseries(objspace),"Select","Self",0,0));
  };
 virtual ~afterseries() {pos.close((GUIbox *)parent);}
};
DLGPOS afterseries::pos;




//---------------------
//   series properties
//---------------------
class seriesproperties:public GUImonitor
{
 GUItextedit *name;
 GUIcheckbox *shadow,*hitground,*falldown;
 GUItextbutton *next,*fallseries,*cfunc;

 public:
 seriesproperties(objectspace *tosp):GUImonitor(150,120)
 {
  shadow=new GUIcheckbox(this,"shadow",10,20,0);
  hitground=new GUIcheckbox(this,"can hit ground",10,30,0);
  falldown=new GUIcheckbox(this,"falldown",10,40,0);
  name=new GUItextedit(this,"Name: ",0,5,3,90,15); name->disabled=1;

  new GUIstatictext(this,1,"After series completes:",5,68);
  new GUIstatictext(this,1,"Control function:",5,95);
  next=fallseries=cfunc=0;

  refresh(GUIRFR_OBJSPACE,tosp);
 };

 virtual void refreshseries()
 {
     if (s)
     {
      name->setinput(s->name); name->disabled=0;
      shadow->setstate(!(s->parm&SP_NOSHADOW));
      hitground->setstate(!(s->parm&SP_NOGROUND));
      falldown->setstate(!(s->parm&SP_NOFALLDOWN) );

      if (fallseries) {delete fallseries; fallseries=0;}
      if (falldown->getstate())
       fallseries=new GUItextbutton(this,
       s->falldown==0 ?  "<falldown>" :
       s->falldown==SP_SUICIDE ?  "<suicide>" :
       s->falldown==SP_LASTFOREVER ?  "<lastforever>" :
        od->sd[s->falldown].name,width()/2-5,50);

      if (next) delete next;
      next=new GUItextbutton(this,
       //s->next==0 ?  "<default>" :
       s->next==SP_SUICIDE ?  "<suicide>" :
       s->next==SP_LASTFOREVER ?  "<lastforever>" :
         od->sd[s->next].name,width()/2-5,80);

      if (cfunc) delete cfunc;
      if (cfuncnames)
       cfunc=new GUItextbutton(this, (*cfuncnames)[od->sd[s->next].cfunc],width()/2-5,105);

     } else
     {
      name->disabled=1;
      if (fallseries) delete fallseries; fallseries=0;
      if (next) delete next; next=0;
      if (cfunc) delete cfunc; cfunc=0;
     }
 };

 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (s)
  {
   if (guimsg==GUIMSG_EDITCHANGED)
   {
    if (c==name)
     {
      strcpy(s->name,name->getinput());
      root->refresh(GUIRFR_SERIES,s);
     }
   }
   if (guimsg==GUIMSG_CHECKED)
   {
    if (c==shadow) s->parm&=~SP_NOSHADOW;
    if (c==falldown) s->parm&=~SP_NOFALLDOWN;
    if (c==hitground) s->parm&=~SP_NOGROUND;
    refresh(GUIRFR_SERIES,s);
   }
   if (guimsg==GUIMSG_UNCHECKED)
   {
    if (c==shadow) s->parm|=SP_NOSHADOW; else
    if (c==falldown) s->parm|=SP_NOFALLDOWN; else
    if (c==hitground) s->parm|=SP_NOGROUND;
    refresh(GUIRFR_SERIES,s);
   }

   if (guimsg==GUIMSG_PUSHED)
    {
     if (c==next) afterseries::open();
     if (c==cfunc) selectcfunc::open();
    }
  }
  return 1;
 }
 virtual char *getname() {return "seriesproperties";}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUIbox(guiroot,"Series properties",new seriesproperties(objspace),0,0));
  };
 virtual ~seriesproperties() {pos.close((GUIbox *)parent);}
};
DLGPOS seriesproperties::pos;







//-----------------------------------------------------------------
// dialog for changing series of selected object in an objectspace
//-----------------------------------------------------------------
class changeseries:public selectseries
{
 public:

 changeseries(objectspace *tosp):selectseries()
   {   refresh(GUIRFR_OBJSPACE,tosp);
   };
 virtual void refreshstopped()
  {
   if (!o) return;
   o->startseries(list->getselnum());
   root->refresh(GUIRFR_SERIES,o->cs);
  }
 virtual void refreshseries()
  {
   if (o && !o->active) list->setsel(o->csnum);
  }

 virtual int sendmessage(GUIrect *c,int guimsg)
 {
   if (c==list)
    {
     if (guimsg==GUIMSG_LISTBOXSELCHANGED)
      {
       int x=list->getselnum(); //selected seriesnum
       if (x<0) return 1;
       if (o) // && !o->active)
        {
         o->stop();
         o->startseries(list->getselnum());
         root->refresh(GUIRFR_SERIES,o->cs);
        }
      }
     if (guimsg==GUIMSG_LISTBOXDBLCLICKED)
      {
       //properties
       seriesproperties::open();
       return 0;
      }
    }
  return selectseries::sendmessage(c,guimsg);
 }
 virtual void losefocus() { return;}
 virtual int keyhit(char kbscan,char key) {return list->keyhit(kbscan,key);}
 virtual char *getname() {return "changeseries";}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUIbox(guiroot,"Change series",new changeseries(objspace),0,0));
  };
 virtual ~changeseries() {pos.close((GUIbox *)parent);}
};
DLGPOS changeseries::pos;


//----------------------
//   select image
//----------------------
class selectimagedlg:public GUImonitor
{
 GUIhscrollbar *scroll;

 uutimer dragtimer;
 int dragging;
 int dragx;

 public:
 selectimagedlg(objectspace *tosp):GUImonitor(300,130)
 {
  dragging=0;
  scroll=new GUIhscrollbar(this,0,height()-11,width());
  refresh(GUIRFR_OBJSPACE,tosp);
 };
 virtual void refreshobjdef()
 {
   if (!od) {scroll->setrange(0,0); return;}

    int totalwidth=0;
    for (int i=0; i<od->numi; i++)
     if (od->id[i]) totalwidth+=od->id[i]->xw;
    totalwidth-=width();
    scroll->setrange(0,totalwidth);
 }

 virtual void refreshbgdef()
 {
  od=bd; refreshobjdef();
 }

 virtual int sendmessage(GUIrect *c,int guimsg) {return 1;};
 virtual char *getname() {return "selectimage";}
 virtual void draw(char *dest);
 virtual int keyhit(char,char);

 virtual GUIrect *click(mouse &m);
 virtual int drag(mouse &m);
 virtual int release(mouse &m);

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUIbox(guiroot,"Select image",new selectimagedlg(objspace),0,0));
  };
 virtual ~selectimagedlg() {pos.close((GUIbox *)parent);}
};
DLGPOS selectimagedlg::pos;

void selectimagedlg::draw(char *dest)
{
 fill(0);

if (od)
{
 if (dragging)
  if (dragtimer.check())
  {
   scroll->scroll(dragx/2);
   dragtimer.reset();
  }

  GUIrect::draw(dest);

 {
  CLIP clip(dest,x1,y1,x2,y2);
  int dx=-scroll->getpos();
  for (int i=0; i<od->numi; i++)
  if (od->id[i])
   {
    od->id[i]->draw(dest,dx,0);
    dx+=od->id[i]->xw;
    if (dx>width()) break;
   }
 }


}

}

int selectimagedlg::keyhit(char kbscan,char key)
{
// if (kbscan==KB_PGUP) {scroll->scroll(-width()); return 1;} //pageup
// if (kbscan==KB_PGDN) {scroll->scroll(width()); return 1;} //pagedown
 return 0; //GUIrect::keyhit(kbscan,key);
}

GUIrect *selectimagedlg::click(mouse &m)
{
 if (m.click&MBLEFT) //drag image
  {
   if (!osp) return 0;
   int mx=m.x-x1; //distance from left edge of image sel box
   int dx=-scroll->getpos();
   for (int i=0; i<od->numi; i++)
     if (od->id[i])
      {
       int dx2=dx+od->id[i]->xw;
       if (mx<dx2)
        {
         //create image drag
         bgimage *im=new bgimage;
         im->index=i;
         im->dispx=dx+x1;
         im->dispy=y1;
         im->dispz=0;
         im->orient=0;
         im->layer=0;
         if (!bd)
          if (osp->maximized)  return new imagemove((objectdefw *)od,im,osp->x2-70,osp->y1+30);
                          else return new imagemove((objectdefw *)od,im,osp->x2+10,osp->y1+10);
         else
          if (osp->maximized)  return new bgimagemove(bd,im,osp->x2-70,osp->y1+30);
                          else return new bgimagemove(bd,im,osp->x2+10,osp->y1+10);
        }
       dx=dx2;
       if (dx>width()) break;
      }
   return 0;
  }
 if (m.click&MBRIGHT) //scrolling
  {
   dragging=1;
   dragx=0;
   dragtimer.set(5);
   return this;
  }
 return 0;
};
int selectimagedlg::drag(mouse &m)
{
 if (m.b&MBRIGHT) {dragx+=m.x-m.oldx;return 1;}
 return 0;
}
int selectimagedlg::release(mouse &m) {dragging=0; return 1;};




//shows series of frames as blocks
class framebar:public GUIrect
{
 object *o;
 int size; //size of each square
 int width;
 public:
 framebar(GUIrect *p,int x,int y,int xw,int ts):
  GUIrect(p,x,y,x+xw,y+ts),size(ts),o(0),width(xw) {}
 void setobject(object *to) {o=to;}

 virtual GUIrect *click(mouse &m) {return this;}
 virtual int release(mouse &m) {return 1;}
 virtual int drag(mouse &m)
  {
   if (!o) return 0;
   if (o->playing) o->stop();
   int cf=(m.x-x1)/size;
   if (cf!=o->cf)
    {
     _disable();
     o->cf=cf;
     if (o->cf>=o->nf) o->cf=o->nf-1;
     o->resetfptr();
     _enable();
     root->refresh(GUIRFR_FRAME,o->fptr);
    }
   return 1;
  }

 virtual void draw(char *dest)
  {
   if (!o) return;
   drawhline(dest,1,x1,y1,x1+o->nf*size);
   drawhline(dest,1,x1,y2,x1+o->nf*size);

   for (int i=0; i<=o->nf; i++)
   {
    if (o->nf && i==o->cf)
      drawrect(dest,2,x1+i*size,y1+1,size,size-1);

    drawvline(dest,1,x1+i*size,y1,y2+1);
   }

//   fill(0);
  }
};

//--------------------
//   object frame editor
//--------------------
class objectedit:public GUImonitor
{
 GUIimagebutton *left,*right;

 GUIimagebutton *play,*stop,*playloop,*activate;

 GUIstatictext *seriestext;
// GUIstatictext *frametext;
 GUIintedit *dur;
 GUIfloatedit *dx,*dy,*dz;
 framebar *fbar;
 public:
 objectedit(objectspace *tosp):GUImonitor(230,52)
 {
  seriestext=new GUIstatictext(this,2,0,3,5);

//  frametext=new GUIstatictext(this,1,"frame:",3,20);
  int x=7,y=18;
  play=new GUIimagebutton(this,a95vol.play,x,y); x+=11;
  playloop=new GUIimagebutton(this,a95vol.playlooped,x,y);  x+=11;
  stop=new GUIimagebutton(this,a95vol.stop,x,y);  x+=11;
  activate=new GUIimagebutton(this,a95vol.active,x,y);   x+=11;
  x+=3;

  left=new GUIimagebutton(this,a95vol.lmark,0,0);  left->moveto(x,y); x+=left->width();
  right=new GUIimagebutton(this,a95vol.rmark,0,0); right->moveto(x,y); x+=right->width();

  x+=2;
  fbar=new framebar(this,x,y,width()-x-2,10);

  x=3; y=35;
  dur=new GUIintedit(this,"dur:",x,y,25,0,1,9999);
  dx=new GUIfloatedit(this,"dX:",dur->x2+4,y,25,0,-100.00,100.00);
  dy=new GUIfloatedit(this,"dY:",dx->x2+3,y,25,0,-100.00,100.00);
  dz=new GUIfloatedit(this,"dZ:",dy->x2+3,y,25,0,-100.00,100.00);
  dur->disabled=1; dx->disabled=1; dy->disabled=1; dz->disabled=1;
 };

 virtual void refreshobject()
  {
   if (parent) ((GUIbox *)parent)->settitle(o ? o->od->name : "No object selected");
   fbar->setobject(o);
  }

 virtual void draw(char *dest)
 {
  if (o)
  {
   if (m.capture!=activate) activate->depressed=o->active;
   if (m.capture!=playloop) playloop->depressed=o->playrepeat;
   if (m.capture!=play) play->depressed=o->playing;
  }

  GUImonitor::draw(dest);
 }

 virtual void refreshframe()
 {
     if (f)
     {
//      char s[80]; s[0]=0;
//      sprintf(s,"frame: %d/%d",o->cf+1,o->nf);
  //    frametext->settext(s);
      dur->set(f->dur);
      dx->set(((float)f->dx)/0x100);
      dy->set(((float)f->dy)/0x100);
      dz->set(((float)f->dz)/0x100);
      dur->disabled=0; dx->disabled=0; dy->disabled=0; dz->disabled=0;
     }  else
     {
      dur->disabled=1; dx->disabled=1; dy->disabled=1; dz->disabled=1;
//      frametext->settext("frame:");
     }
 }

 virtual void refreshseries()
  {
    if (o && o->od->loaded)
    {
     seriestext->settext(o->cs->name);
     f=o->fptr;
     refreshframe();

    } else {seriestext->settext(0);}
  }

 virtual int sendmessage(GUIrect *c,int guimsg)
 {
   if (guimsg==GUIMSG_EDITCHANGED && f)
    {
     if (c==dur) f->dur=dur->get();
     if (c==dx) f->dx=(int)(dx->get()*0x100);
     if (c==dy) f->dy=(int)(dy->get()*0x100);
     if (c==dz) f->dz=(int)(dz->get()*0x100);
     return 1;
    }
   if (guimsg==GUIMSG_HELDDOWN )
    {
     if (c==left) o->framebackward();
     if (c==right) o->frameforeward();
    }
   if (guimsg==GUIMSG_PUSHED)
    {
     if (osp)
     {
      if (c==play) osp->play();
      if (c==playloop) osp->playlooped();
      if (c==stop) osp->stop();
      if (c==activate) osp->activate();
     }
    }
  return 1;
 }

 virtual char *getname() {return "frameedit";}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
     pos.open(new GUIbox(guiroot,"No object selected",new objectedit(objspace),0,0));
   pos.opened->refresh(GUIRFR_OBJSPACE,objspace);
  };
 virtual ~objectedit() {pos.close((GUIbox *)parent);}
};
DLGPOS objectedit::pos;



//--------------------
//  background info
//--------------------

class bgedit:public GUImonitor
{
 GUIstringlistbox *list;

 public:

 bgedit():GUImonitor(120,50)
 {
  list=new GUIstringlistbox(this,5,5,60,4,10);
  //fill list box
  ITEMPTR *a=list->resizeitems(4);
  a[0]="Active";
  a[1]="Images";
  a[2]="Slopes";
  a[3]="Objects";
 }

 virtual void draw(char *dest)
 {
  GUImonitor::draw(dest);
  if (!bg) return;
 }

 virtual void refreshbgdef()
 {
//  if (osp) bg=osp->bg;
//  if (bg) bd=bg->bd;
   if (parent) ((GUIbox *)parent)->settitle(bd ? bd->name : "No background");
 }

 virtual void refreshbg()
 {
//  if (osp) bg=osp->bg;
  if (!bg) list->clearsel();
 }

 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_LISTBOXSELCHANGED)
   if (bg)
   {
    bg->emode=list->getselnum()+1;
    if (bg->emode==BGE_ACTIVE)  bg->activate();
                 else bg->deactivate();
    if (bg->emode>BGE_ACTIVE)
     {osp->p=0; root->refresh(GUIRFR_OBJECT,0);}
   } else list->clearsel();

  return 1;
 }


 virtual char *getname() {return "bgedit";}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIbox(guiroot,"No background",new bgedit(),0,0));
   pos.opened->refresh(GUIRFR_OBJSPACE,objspace);
  };
 virtual ~bgedit() {pos.close((GUIbox *)parent);}
};
DLGPOS bgedit::pos;







//--------------------
//   about box
//----------------------
extern char buildtime[],builddate[],buildcompiler[];
extern int buildcompilerversionhigh,buildcompilerversionlow;
class aboutdlg:public GUIcontents
{
 public:
 aboutdlg():GUIcontents(290,65)
 {
  new GUIstaticimage(this,a95vol.about,5,5);
  new GUIstatictext(this,2,"Buddy says:",70,5);
  #ifdef WIN95
  new GUIstatictext(this,3,"Animator Win95 32-bit",70,20);
  #endif
  #ifdef DOS
  new GUIstatictext(this,3,"Animator DOS 32-bit",70,20);
  #endif

  new GUIstatictext(this,3,"Copyright (C) 1996 Bloodlust Software",70,50);
  char s[80];
  sprintf(s,"%s %d.%d %s %s",buildcompiler,buildcompilerversionhigh,buildcompilerversionlow,builddate,buildtime);
  new GUIstatictext(this,3,s,70,30);
 }

 virtual int sendmessage(GUIrect *c,int guimsg) {return 1;};
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "aboutdlg";}
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
    pos.open(new GUIonebuttonbox(guiroot,"About A32",new aboutdlg(),"Thanks Buddy!",0,0));
  };
 virtual ~aboutdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS aboutdlg::pos;



//---------------------------
//     New object space
//--------------------------
class newobjectspace:public GUIcontents
{
 GUItextedit *objdeffile,*bgdeffile;
 GUInumbertextedit *w,*h;
 public:

 newobjectspace():GUIcontents(300,45)
  {
   objdeffile=new GUItextedit(this,"Object definition list file: ","object.lst",width()-10,3,90,30);
   bgdeffile=new GUItextedit(this,"Background definition list file: ","bg.lst",width()-10,15,90,30);
   objdeffile->moverel(-objdeffile->width(),0);
    bgdeffile->moverel(- bgdeffile->width(),0);
   w=new GUInumbertextedit(this,"Width: ",320,60,30,40,4);
   h=new GUInumbertextedit(this,"Height: ",200,160,30,40,4);

  };
 virtual void draw(char *dest)
  {
   fill(CLR_BOX);
   GUIrect::draw(dest);
  };

 virtual char *getname() {return "newobjspace";}
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
   {
    objectspace::open(objdeffile->getinput(),bgdeffile->getinput(),w->getstate(),h->getstate());
   }
  return 1;
 }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
     pos.open(new GUIonebuttonbox(guiroot,"New objectspace",new newobjectspace(),"Create",0,0));
  };
 virtual ~newobjectspace() {pos.close((GUIbox *)parent);}
};
DLGPOS newobjectspace::pos;









void m_play() {objspace->play();}
void m_playlooped(){objspace->playlooped();}
void m_stop() {objspace->stop();}
void m_activate() { objspace->activate();}
void m_flipx() { objspace->flipx();}

void m_insertframe() {objspace->insertframe();}
void m_deleteframe() {objspace->deleteframe();}
void m_insertframeafter() {objspace->insertframeafter();}



void setres(int xw,int yw)
{
 int bgwasactive=0;
 if (objspace && objspace->bg && objspace->bg->active)
  {bgwasactive=1; objspace->bg->deactivate();}

 changeresolution(xw,yw);
 root->resize(SCREENX,SCREENY);

 if (bgwasactive) objspace->bg->activate();
}

void res320x200() {setres(320,200);}
void res320x240() {setres(320,240);}
void res640x400() {setres(640,400);}
void res640x480() {setres(640,480);}
void res800x600() {setres(800,600);}
void res1024x768() {setres(1024,768);}


int showbglines=0;
int alwaysredrawbg=0;
void m_redrawbg() {msg.printf(1,"Always redrawbg %s",(alwaysredrawbg^=1) ? "on" :"off");}
void m_showbglines() {msg.printf(1,"bglines %s",(showbglines^=1) ? "on" :"off"); }


void m_heapcheck()
{
  switch(_heapset(0x69))
  {
   case _HEAPBADNODE: msg.printf(5,"ERROR: Bad heap node"); return;
   case _HEAPEMPTY: msg.printf(5,"ERROR: Heap empty"); return;
   case _HEAPOK: msg.printf(1,"Heap OK"); return;
  };
}

void m_filleddialogs()
{
 msg.printf(1,"Filled dialogs %s",cfg->toggle(CFG_FILLEDDLG) ? "on" :"off");
}

void m_filleddesktop()
{
 msg.printf(1,"Filled desktop %s",!cfg->toggle(CFG_NOFILLEDDESKTOP) ? "on" :"off");
}

void m_showregions()
{
 msg.printf(1,"Show regions %s",cfg->toggle(CFG_SHOWREGIONS) ? "on" :"off");
}


void m_showfps()
{
 msg.printf(1,"Show FPS %s",cfg->toggle(CFG_SHOWFPS) ? "on" :"off");
}

void m_viewguitree()
{
 msg.printf(1,"View GUI tree %s",cfg->toggle(CFG_SHOWGUITREE) ? "on" :"off");
}


extern char configfile[];
void m_saveconfig()
{
 cfg->save(configfile);
}

void m_save()
{
 if (objspace && objspace->p)
  ((objectdefw *)objspace->p->od)->save();
}

void m_kill()
{
 objspace->kill();
}


void m_bgkill()
{
 if (!objspace) return;
 if (!objspace->bg) return;
 msg.printf(3,"%s killed",objspace->bg->bd->name);
 delete objspace->bg;
 objspace->bg=0;
}

void m_bgsave()
{
 if (objspace && objspace->bg)
  objspace->bg->bd->save();
}

void m_bgactivate()
{
 if (objspace && objspace->bg)
 {
  objspace->bg->activate();
  msg.printf(3,"%s activated",objspace->bg->bd->name);
 }
}

void m_bgdeactivate()
{
 if (objspace && objspace->bg)
 {
  objspace->bg->deactivate();
  msg.printf(3,"%s deactivated",objspace->bg->bd->name);
 }
}



void editinputdevice0();
void editinputdevice1();

void m_editeffects();

#ifdef WIN95
void ddrawinfo();
void m_ddrawinfo()
{
 GUImessage::open();
 ddrawinfo();
}
#endif



void m_copyimages()
{
 if (objspace && objspace->p && objspace->p->fptr)
   new clip_images(objspace->p->fptr,objspace->p->od);
}

void m_copyframe()
{
 if (objspace && objspace->p && objspace->p->fptr)
   new clip_frame(objspace->p->fptr,1,objspace->p->od);
}

void m_copyeffect()
{
 if (objspace && objspace->p && objspace->p->fptr)
  {
   new clip_effects(objspace->p->fptr,objspace->p->od);
   //root->refresh(GUIRFR_FRAME,objspace->p->fptr);
  }
}

void m_paste()
{
 clipboard.paste();
}

void m_volumize()
{
 if (objspace && objspace->p) ((objectdefw *)objspace->p->od)->volumize();
}

void m_bgvolumize()
{
 if (objspace && objspace->bg) objspace->bg->bd->volumize();
}


#include "dir.h"
void m_getcwd()
{
 char buf[50];
 if (getcwd(buf,50))
  msg.printf(3,buf);
}

void m_cleanup()
{
 objspace->cleanup();
}


//*************************************
//           menu definition
//*************************************

menu objectmenu=
{
 {
  {"Load...",loadobjectdlg::open,'l',0},
  {"Save",m_save,'s',0},
  {"Kill",m_kill,'k',0},
  {"-----",0,0,0},
  {"Activate",m_activate,'a',0},
  {"FlipX",m_flipx,'x',0},
  {"Volumize",m_volumize,'v',0},
  {"-----",0,0,0},
  {"New objectspace",newobjectspace::open,0,0},
  {"Cleanup",m_cleanup,'C',0},
  {"Exit",m_quit,'q',0},
  {0,0,0,0},
 }
};


menu seriesmenu=
{
 {
  {"Play",m_play,'p',0},
  {"Play looped",m_playlooped,'P',0},
  {"Stop",m_stop,0,0},
  {"-----",0,0,0},
  {"Insert frame",m_insertframe,'i',0},
  {"Insert frame after",m_insertframe,'I',0},
  {"Delete frame",m_deleteframe,'D',0},
  {"-----",0,0,0},
  {"Properties",seriesproperties::open,0,0},
  {0,NULL,0,0},
 }
};



menu viewmenu=
{
 {
  {"Frame edit",objectedit::open,0,0},
  {"Effects edit",m_editeffects,0,0},
  {"Image select",selectimagedlg::open,0,0},
  {"Series select",changeseries::open,0,0},
  {"-----",0,0,0},
  {"Messages",GUImessage::open,0,0},
  {0,NULL,0,0},
 }
};


menu resmenu =
{
 {
  {"320x200",res320x200,0,0},
  {"320x240",res320x240,0,0},
  {"640x480",res640x480,0,0},
  {"800x600",res800x600,0,0},
  {"1024x768",res1024x768,0,0},
  {0,NULL,0,0},
 }
};


menu redefineinputmenu=
{
 {
  {"Device 1",editinputdevice0,0,0},
  {"Device 2",editinputdevice1,0,0},
  {0,NULL,0,0},
 }
};
menu miscmenu=
{
 {
  {"Show FPS",m_showfps,0,0},
  {"Show GUItree",m_viewguitree,0,0},
  {"Show regions",m_showregions,0,0},
  {"Always redraw bg",m_redrawbg,0,0},
  {"Show bg lines",m_showbglines,0,0},
  {"Filled dialogs",m_filleddialogs,0,0},
//  {"Filled desktop",m_filleddesktop,0,0},
  {"Redefine input",0,0,&redefineinputmenu},
  {"Resolution",0,0,&resmenu},
  {"Heap check",m_heapcheck,0,0},
  #ifdef WIN95
  {"Directdraw Info",m_ddrawinfo,0,0},
  #endif
  {"Show current dir",m_getcwd,0,0},

  {"-----",0,0,0},
  {"Save config",m_saveconfig,0,0},
  {0,NULL,0,0},
 }
};

menu helpmenu=
{
 {
  {"About A32...",aboutdlg::open,0,0},
  {0,NULL,0,0},
 }
};


menu bgmenu=
{
 {
  {"Load...",loadbgdlg::open,'L',0},
  {"Save",m_bgsave,'S',0},
  {"Kill",m_bgkill,'K',0},
  {"-----",0,0,0},
  {"Activate",m_bgactivate,'A',0},
  {"Deactivate",m_bgdeactivate,0,0},
  {"-----",0,0,0},
  {"Edit...",bgedit::open,0,0},
  {"Volumize",m_bgvolumize,'V',0},
  {0,NULL,0,0},
 }
};


menu editmenu=
{
 {
  {"Copy images",m_copyimages,'[',0},
  {"Copy frame",m_copyframe,'{',0},
  {"Copy effect",m_copyeffect,0,0},
  {"-----",0,0,0},
  {"Paste",m_paste,']',0},
  {0,NULL,0,0},
 }
};



menu mainmenu=
{
 {
  {"Object",0,0,&objectmenu},
  {"Bg",0,0,&bgmenu},
  {"Edit",0,0,&editmenu},
  {"Series",0,0,&seriesmenu},
  {"View",0,0,&viewmenu},
  {"Misc",0,0,&miscmenu},
  {"Help",0,0,&helpmenu},
  {0,NULL,0,0},
 }
};


void initdefaultgui()
{
 new GUIhmenu(guiroot,&mainmenu,0,0);

 #ifdef WIN95
 GUImessage::pos.setxy(380,40);
 objectedit::pos.setxy(90,280);
 changeseries::pos.setxy(400,200);
 selectimagedlg::pos.setxy(330,300);
 #endif

 objectspace::open("object.lst","bg.lst",320,200);
 #ifdef DOS
 if (objspace)
  ((GUImaximizebox *)objspace->parent)->maximize();
 #endif



 #ifdef WIN95
 objectedit::open();
 if (cfg->get(CFG_SHOWMESSAGE)) GUImessage::open();
// changeseries::open();
 #endif

}





