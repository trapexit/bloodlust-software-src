//disgruntled
//main platform independant
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mem.h>
#include <malloc.h>
#include <direct.h>

#include "config.h"

#include "keyb.h"

#include "types.h"
#include "r2img.h"
#include "font.h"

#include "file.h"
#include "vol.h"
#include "mouse.h"
#include "message.h"

#include "dd.h"

#include "guirect.h"
#include "guiroot.h"
#include "guicolor.h"

#include "uutimer.h"

#include "object.h"
#include "objspace.h"
#include "bg.h"

#include "guivol.h"

#ifdef WIN95
#include "net.h"
#endif

char appname[]="Disgruntled";
char configfile[]="grunt.cfg";

float version=0.1;

int SCREENX=640;
int SCREENY=480;

//config info
config *cfg;

//input devices
input *inputdevice[2];
inputdevicesettings *ids;


GUIVOL guivol;
int GUIVOL::size() {return sizeof(GUIVOL);}

FONT *font[10]; //fonts
COLORMAP *shadowmap; //shaded colormapping
IMG *shadow; //generic shadow


volatile unsigned uu=1,su=1;

//buffer for messages
msgbuffer msg;

//root of a32
class ROOT *root=0;

//main root of gui tree
GUIroot *guiroot=0;

//root of objects
objectspace *objspace=0;


extern int guienabled;
void enablegui();
void disablegui();
void togglegui();


int keyboard();
void initdefaultgui();


 //create all the fonts and colors
void createfonts()
{
 for (int i=0; i<7; i++) font[i]=guivol.font->duplicate();
  //peach
 font[0]->convertcolor((char)0xca,(char)0xc6); font[0]->convertcolor((char)0xdb,(char)0xd8);
   //white
 font[1]->convertcolor((char)0xca,(char)0xff); font[1]->convertcolor((char)0xdb,(char)0x0);
  //green
 font[2]->convertcolor((char)0xca,(char)6*16); font[2]->convertcolor((char)0xdb,(char)0x0);
   //grey something
 font[3]->convertcolor((char)0xca,(char)19);   font[3]->convertcolor((char)0xdb,(char)0x0);
   //black
 font[4]->convertcolor((char)0xca,(char)0x0);  font[4]->convertcolor((char)0xdb,(char)15);
  //red
 font[5]->convertcolor((char)0xca,(char)2*16+1);   font[5]->convertcolor((char)0xdb,(char)2*16+14);
   // ???
 font[6]->convertcolor((char)0xca,(char)10*16+3);   font[5]->convertcolor((char)0xdb,(char)0);
 }

//initialize
int initgame()
{
// chdir("c:\\grunt");

 //set up keyboard handler
 set_keyboard_func(keyboard);

 #ifdef NETWORK
 initwinsock();
 #endif

  //get configuration
// cfg=new config();
// cfg->load(configfile);

 //initialize input devices
 ids=&cfg->ids; //copy over input settings
 inputdevice[0]=newinputdevice(cfg->pinput[0]);
 inputdevice[1]=newinputdevice(cfg->pinput[1]);


 //load up graphics and shit
 if (!guivol.read("a95.vol")) return -1;
// guivol.print();
 setpalette(guivol.pal);


 shadowmap=new COLORMAP;
 shadowmap->createshademap(*guivol.pal,20,20,20);

 shadow=(guivol.shadowsel)->duplicate();
 shadow->convertcolor(16+13,9*16+10);

 //create fonts
 createfonts();

 //create root
 root=new ROOT;

 //create root of all GUI
 new GUIroot(root);


 initdefaultgui();

/*
 //create objectspace
 objectspace::open("object.bin","bg.bin",320,200);

 //create bg
 objspace->newbgobject(1);

 //create background objects
 if (objspace->bg)
   objspace->bg->bd->instantiateobjposlist();

 //create main dude
// objspace->setp(objspace->newobject("Disgruntled",80,0,0,0));
  */
 return 0;
}


//terminates a game (deletes objectspace)
void endgame()
{
 if (!objspace) return;
 if (objspace->parent) delete objspace->parent;
}

//starts a game with certain bg
void startgame(char *bgname,int noobjs)
{
 if (objspace) endgame();

 objectspace::open("object.bin","bg.bin",320,200);
 if (bgname)
 {  //create bg
  objspace->newbgobject(bgname);
   //instantiate bg objects
  if (objspace->bg && !noobjs)
    objspace->bg->bd->instantiateobjposlist();
 }
}


int cmd_startgame(char *p)
{
 char bgname[32]="bgtest";
 sscanf(p,"%s",bgname);
 startgame(bgname,0);
 return 1;
}

int cmd_loadbg(char *p)
{
 char bgname[32]="bgtest";
 sscanf(p,"%s",bgname);
 startgame(bgname,1);
 return 1;
}

int cmd_newosp(char *p)
{
 startgame(0,0);
 return 1;
}

int cmd_loadobj(char *p)
{
 if (!objspace) cmd_newosp(0);
 if (!objspace) return 1;
 char objname[32];
 int x=objspace->width()/2,y=0;
 if (!sscanf(p,"%s %d %d",objname,&x,&y)) return 0;

 objspace->newobject(objname,x,y,0,0);
 return 1;
}

void terminategame()
{
 #ifdef NETWORK
 terminatewinsock();
 #endif
 delete root;
 guivol.free();
 delete cfg;
}

void gametimer()
{
 uu++;
 input::refreshtimer();
 inputdevice[0]->read();

 //tick objectspace
 if (objspace) objspace->tick();

 inputdevice[0]->reset();

 //tick network connection
// if (nc) nc->tick();
}


int keyboard()
{
// if (kbscan==KB_ESC) {quitgame(); return 1;}
 if (kbscan==KB_ESC) {togglegui(); return 1;}

 if (input::refreshkeyboard(kbscan)) return 1; //dont add key to queue

 if (kbscan&0x80) return 1; //release

 char key=scan2ascii(kbscan);
 if (root && root->keyhit(kbscan,key)) enablegui();
 return 1;
}



extern int timeperframe;
uutimer fpstimer(200);
int fps=0,spf=0;
int calcfps()
{
 //calculate fps
 if (fpstimer.check())
   {
    fps=su*100/fpstimer.dur;
    spf=timeperframe*10/su;
    fpstimer.reset();
    timeperframe=0;
    su=0;
   }
 return fps;
}



int blah=0,blah2=0;
extern int surfacelost;

void updatescreen()
{
 //draw message buffer
 input::refreshmain();

 if (guienabled)
 {
 //draw everything
 root->draw(screen);
 m.draw(screen); //draw cursor
 } else
  if (objspace) objspace->draw(screen);

 su++;
 if (cfg->get(CFG_SHOWFPS))
  {
   font[0]->printf(SCREENX-70,50,"fps=%d",calcfps());
   font[0]->printf(SCREENX-70,60,"spf=.%04d",spf);
  }

//  font[0]->printf(SCREENX-150,80,"%d",surfacelost);
//  font[0]->printf(SCREENX-150,80,"%d",blah);
// font[0]->printf(SCREENX-150,80,"objspace:%p",objspace);
//font[0]->printf(SCREENX-150,80,"%d %d",blah,blah2);
}



//------------------------------
//       ROOT node
//------------------------------

ROOT::ROOT():GUIrect(0,0,0,SCREENX,SCREENY){}
ROOT::~ROOT() {}

void ROOT::resize(int xw,int yw)
{
 if (guiroot) guiroot->resize(xw,yw);
 if (objspace) objspace->resize(xw,yw);
 GUIrect::resize(xw,yw);
}

void ROOT::refresh(int r,void *c)
  {
  // msg.printf(2,"refresh %d",r);
   for (GUIrect *g=child; g; g=g->next)  g->refresh(r,c);
  };






