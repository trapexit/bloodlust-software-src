#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "types.h"

#include "r2img.h"
#include "font.h"
#include "dd.h"

#include "mouse.h"
#include "message.h"
#include "gui.h"
#include "guimenu.h"
#include "guicolor.h"


#include "keyb.h"

#include "object.h"
#include "bg.h"
#include "objspace.h"


#include "config.h"

#include "guivol.h"

#include "grunt.h"

#ifdef WIN95
#define NETWORK
#include "net.h"
#endif


//----------------------
//  GUI messagebox
//----------------------
#define MSGYW 8 //height in lines

class GUImessage:public GUIcontents
{
 class msgbuffer *m;
 GUIvscrollbar *scroll;
 public:
 void setrange()
 {
  int yw=m->num-MSGYW;
  if (yw<0) yw=0;
  scroll->setrange(0,yw);
  scroll->setpos(yw);
  m->updated=0;
 }
 GUImessage(class msgbuffer *tm):
  GUIcontents(261,MSGYW*10)
  {
   m=tm;
   scroll=new GUIvscrollbar(this,width()-12,0,height());
   setrange();
  }

 virtual void draw(char *dest)
 {
  if (m->updated) setrange();
  fill(CLR_BOX);
  m->draw(x1+2,y1,scroll->getpos(),MSGYW);
  GUIrect::draw(dest);
//  font[0]->printf(50,50,"%d",m->num);
 }
 virtual char *getname() {return "message";}
 virtual ~GUImessage() {}
};

GUIrect *newguimessage(msgbuffer *tm)
 {return new GUImessage(tm);}

class systemmessages:public GUImessage
{
 public:
 systemmessages(msgbuffer *tm) :GUImessage(tm) {}
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIbox(guiroot,"Messages",new systemmessages(&msg),0,0));
//   cfg->set(CFG_SHOWMESSAGE,1);
  };
 virtual ~systemmessages() {pos.close((GUIbox *)parent); } //cfg->set(CFG_SHOWMESSAGE,0);}
};
DLGPOS systemmessages::pos;






//monitors object/series/frame etc
class GUImonitor:public GUIcontents
{
 protected:
 objectspace *osp;

 bgobject *bg;
 bgdef *bd;

 object    *o;
 objectdef *od;
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
        if (bg==(bgobject *)c) return;
        bg=(bgobject *)c;
        //if (bg) refresh(GUIRFR_OBJECT,0);
        refresh(GUIRFR_BGDEF,bg ? bg->bd : 0);
        refreshbg();
        return;

    case GUIRFR_OBJECT:
       if (o==(object *)c) return;
        o=(object *)c;
        //if (o) refresh(GUIRFR_BG,0);
        refresh(GUIRFR_OBJDEF,o ? o->od : 0);
        refresh(GUIRFR_SERIES,o ? o->cs : 0);
        refresh(GUIRFR_FRAME,o ? o->fptr : 0);
        refreshobject();
        return;

    case GUIRFR_OBJDEF:
        if (od==(objectdef *)c) return;
        od=(objectdef *)c;
        refreshobjdef();
        return;

    case GUIRFR_BGDEF:
        if (bd==(bgdef *)c) return;
        bd=(bgdef *)c;
        refreshbgdef();
        return;

    case GUIRFR_SERIES:
        if (s==(series *)c) return;
        s=(series *)c;
        refreshseries();
        return;

    case GUIRFR_FRAME:
        if (f==(frame *)c) return;
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
 loadbgdlg(objectspace *tosp):GUImonitor(120,100)
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
     if (osp) osp->newbgobject(num);
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

 public:
 loadobjectdlg(objectspace *tosp):GUImonitor(120,100)
 {
  list=new GUIstringlistbox(this,5,5,width()-10,9,10);
  refresh(GUIRFR_OBJSPACE,tosp);
 }
 virtual char *getname() {return "loadobject";}

 int sendmessage(GUIrect *c,int guimsg)
  {
   if (guimsg==GUIMSG_OK || guimsg==GUIMSG_LISTBOXDBLCLICKED)
    {
     if (list->getselnum()<0) return 0;
//     if (osp) osp->newobject(list->getselptr(),(osp->bg ? osp->bg->x : 0)+osp->width()/2,SETBASEY,0,0);
     if (osp) osp->newobject(list->getselptr(),osp->width()/2,0,0,0);
    }
   return 1;
  };
 virtual void refreshobjspace()
 {
  if (!osp) {list->resizeitems(0); return;}
  //fill list box

  int numexist=0;
  ITEMPTR *a=list->resizeitems(osp->numodf);
  for (int i=0; i<osp->numodf; i++)
     if (osp->odf[i]->volexist)
       a[numexist++]=osp->odf[i]->name;
  list->resizeitems(numexist);
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



//--------------------
//   about box
//----------------------
extern char buildtime[],builddate[],buildcompiler[];
extern int buildcompilerversionhigh,buildcompilerversionlow;
extern float version; //game version
class aboutdlg:public GUIcontents
{
 public:
 aboutdlg():GUIcontents(290,65)
 {
  char s[80];

  new GUIstaticimage(this,guivol.about,5,5);
  new GUIstatictext(this,2,"Buddy says:",70,5);
  #ifdef WIN95
  new GUIstatictext(this,3,"Disgruntled Win95",70,20);
  #endif
  #ifdef DOS
  new GUIstatictext(this,3,"Disgruntled DOS",70,20);
  #endif

  sprintf(s,"V%d.%02d",(unsigned)version,((unsigned)(version*100))%100);
  new GUIstatictext(this,5,s,250,20);

  new GUIstatictext(this,3,"Copyright (C) 1997 Bloodlust Software",70,50);
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
   pos.open(new GUIonebuttonbox(guiroot,"About",new aboutdlg(),"Thanks Buddy!",0,0));
  };
 virtual ~aboutdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS aboutdlg::pos;



#ifdef WIN95
#include "sound.h"
//-----------------------------------
class playwavdlg:public GUIcontents
{
 GUItextedit *wavfile;
 public:

 playwavdlg():GUIcontents(280,25)
  {
   wavfile=new GUItextedit(this,"WAV filename: ","grunt.wav",5,3,100,40);
  };
 virtual void draw(char *dest)
  {
   fill(CLR_BOX);
   GUIrect::draw(dest);
  };

 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
   {
    SOUND *s=ReadWavFile(wavfile->getinput());
    if (!s)
     {
      msg.error("error opening wav file %s",wavfile->getinput());
      return 1;
     }
    playsoundlooped(s);
    msg.printf(1,"playing %s looped %dK %.2f sec",wavfile->getinput(),
         s->soundsize/1024,((float)s->soundsize)/SOUNDFREQ);
    free(s);
   }
  return 1;
 }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
     pos.open(new GUIonebuttonbox(guiroot,"Play WAV looped",new playwavdlg(),"Play",0,0));
  };
 virtual ~playwavdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS playwavdlg::pos;
#endif






//--------------------------------------
void disablegui();

class introdlg:public GUIcontents
{
 public:
 introdlg():GUIcontents(295,160)
 {
  int x=5,y=5;
  new GUIstatictext(this,2,"Disgruntled",x,y); y+=10;
  new GUIstatictext(this,5,"by Bloodlust Software",x,y); y+=10;
  y+=10;
  new GUIstatictext(this,1," -Press ESC or click mouse to enable GUI",x,y); y+=10;
  new GUIstatictext(this,1," -Left click to select object",x,y); y+=10;
  new GUIstatictext(this,1," -Right click to move object",x,y); y+=15;

  new GUIstatictext(this,1," -Button 1 is fire. Button 2 is jump.",x,y); y+=10;
  new GUIstatictext(this,1," -To use joystick or change keyboard keys,",x,y); y+=10;
  new GUIstatictext(this,1,"    redefine your input devices (misc menu)",x,y); y+=15;

  new GUIstatictext(this,1," -Resolution is switchable under DirectDraw",x,y); y+=10;
  new GUIstatictext(this,1,"    or with VESA 2.0 compliant driver under DOS.",x,y); y+=10;
  new GUIstatictext(this,1,"      (you can change res from the misc menu)",x,y); y+=15;

  new GUIstatictext(this,1," -Included is grunt.gif for your viewing pleasure.",x,y); y+=10;
  new GUIstatictext(this,5,"Send bugreports/comments to bldlust@southwind.net",x,y); y+=10;

 }

 virtual int sendmessage(GUIrect *c,int guimsg)
  {
   if (guimsg==GUIMSG_OK) {disablegui();}
   return 1;
  };
 virtual int acceptfocus() {return 1;}
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Disgruntled",new introdlg(),"Ok",0,0));
  };
 virtual ~introdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS introdlg::pos;











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


void m_heapcheck()
{
  switch(_heapset(0x69))
  {
   case _HEAPBADNODE: msg.printf(5,"ERROR: Bad heap node"); return;
   case _HEAPEMPTY: msg.printf(5,"ERROR: Heap empty"); return;
   case _HEAPOK: msg.printf(1,"Heap OK"); return;
  };
}


void m_showfps()
{
 msg.printf(1,"Show FPS %s",cfg->toggle(CFG_SHOWFPS) ? "on" :"off");
}

extern char configfile[];
void m_saveconfig(){cfg->save(configfile);}

void editinputdevice0();
void editinputdevice1();

void m_editeffects();

#ifdef WIN95
void ddrawinfo();
void m_ddrawinfo()
{
 systemmessages::open();
 ddrawinfo();
}
#endif

void m_quit()
{
 quitgame();
}




GUImenu *gmenu=0;
int guienabled=0;
void enablegui()
{
 if (guienabled) return;
/* if (guiroot)
  {
   guiroot->reparent(root);
   guiroot->bringtofront();
  }*/
 guienabled=1;
}

void disablegui()
{
 if (!guienabled) return;
// if (guiroot) {guiroot->reparent(0); }
 GUIrect::setfocus(gmenu);
 guienabled=0;
}

void togglegui()
{
 if (guienabled) disablegui(); else enablegui();
}
void m_showregions()
{
 msg.printf(1,"Show regions %s",cfg->toggle(CFG_SHOWREGIONS) ? "on" :"off");
}

int showbglines=0;
int alwaysredrawbg=0;

void m_redrawbg() {msg.printf(1,"Always redrawbg %s",(alwaysredrawbg^=1) ? "on" :"off");}
void m_showbglines() {msg.printf(1,"bglines %s",(showbglines^=1) ? "on" :"off"); }


void m_showmessages()
{if (!systemmessages::pos.opened) systemmessages::open();}
void m_cleanup()
{
 if (objspace) objspace->cleanup();
}

int cmd_maximize(char *p)
{
 if (objspace) ((GUImaximizebox *)objspace->parent)->maximize();
 return 1;
}


void m_startgame()
{
 #ifdef NETWORK
 if (nc) return;
 #endif
 startgame("bgtest",0);
}

void m_endgame()
{
 #ifdef NETWORK
 if (nc) return;
 #endif
 endgame();
}

//------------

//*************************************
//           menu definition
//*************************************

/*
void m_loadobjects()
{
if (objspace && objspace->bg)
 objspace->bg->bd->instantiateobjposlist();
}*/

menu filemenu=
{
 {
  {"Load object...",loadobjectdlg::open,'l',0},
  {"Load background...",loadbgdlg::open,'L',0},
  {"-----",0,0,0},
  {"Start game...",m_startgame,0,0},
  {"End game",m_endgame,0,0},
  {"-----",0,0,0},

  #ifdef WIN95
  {"Play wav looped...",playwavdlg::open,'L',0},
  #endif
//  {"Load bgobjects...",m_loadobjects,0,0},
  {"Cleanup",m_cleanup,'C',0},
  {"Exit",m_quit,'q',0},
  {0,0,0,0},
 }
};

menu viewmenu=
{
 {
  {"Messages",m_showmessages,0,0},
  {0,NULL,0,0},
 }
};

menu resmenu =
{
 {
  {"320x200",res320x200,0,0},
  {"320x240",res320x240,0,0},
  {"640x400",res640x400,0,0},
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
//  {"Always redraw bg",m_redrawbg,0,0},
  {"Show bg lines",m_showbglines,0,0},
  {"Hide GUI",disablegui,0,0},
  {"Show regions",m_showregions,0,0},
  {"Show FPS",m_showfps,0,0},
  {"Redefine input",0,0,&redefineinputmenu},
  {"Resolution",0,0,&resmenu},
  {"Heap check",m_heapcheck,0,0},
  #ifdef WIN95
  {"Directdraw Info",m_ddrawinfo,0,0},
  #endif
  {"-----",0,0,0},
  {"Save config",m_saveconfig,0,0},
  {0,NULL,0,0},
 }
};

menu helpmenu=
{
 {
  {"Help...",introdlg::open,0,0},
  {"-----",0,0,0},
  {"About Disgruntled...",aboutdlg::open,0,0},
  {0,NULL,0,0},
 }
};


#ifdef NETWORK
void m_startserver();
void m_connect();
void netdisconnect();
void netdlgopen();
void m_opennetchat();

menu networkmenu=
{
 {
  {"Connect...",m_connect,0,0},
  {"Start server...",m_startserver,0,0},
  {"-----",0,0,0},
  {"Disconnect",netdisconnect,0,0},
  {"-----",0,0,0},
  {"Chat window",m_opennetchat,0,0},
  {"Show nodes",netdlgopen,0,0},
  {0,NULL,0,0},
 }
};

#endif



menu mainmenu=
{
 {
  {"File",0,0,&filemenu},
  {"View",0,0,&viewmenu},

  #ifdef NETWORK
  {"Network",0,0,&networkmenu},
  #endif


  {"Misc",0,0,&miscmenu},
  {"Help",0,0,&helpmenu},
  {0,NULL,0,0},
 }
};


void initdefaultgui()
{
 gmenu=new GUIhmenu(guiroot,&mainmenu,0,0);
// if (cfg->get(CFG_SHOWMESSAGE)) systemmessages::open();


// introdlg::open();
 enablegui();
}






