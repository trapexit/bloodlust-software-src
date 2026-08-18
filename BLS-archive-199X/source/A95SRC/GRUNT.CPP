//animator
//main platform independant
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mem.h>
#include <malloc.h>

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

#include "a32.h"


char appname[]="Disgruntled";
char configfile[]="grunt.cfg";

int SCREENX=320;
int SCREENY=240;

//config info
config *cfg;

//input devices
input *inputdevice[2];
inputdevicesettings *ids;


A95VOL a95vol;
int A95VOL::size() {return sizeof(A95VOL);}

FONT *font[8]; //fonts
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

//initialize
int initgame()
{
 //set up keyboard handler
 set_keyboard_func(keyboard);

  //get configuration
 cfg=new config();
 cfg->load(configfile);

 //initialize input devices
 ids=&cfg->ids; //copy over input settings
 inputdevice[0]=newinputdevice(cfg->pinput[0]);
 inputdevice[1]=newinputdevice(cfg->pinput[1]);


 //load up graphics and shit
 if (!a95vol.read("a95.vol")) return -1;
// a95vol.print();
 setpalette(a95vol.pal);


 shadowmap=new COLORMAP;
 shadowmap->createshademap(*a95vol.pal,20,20,20);

 shadow=(a95vol.shadowsel)->duplicate();
 shadow->convertcolor(16+13,9*16+10);

 //create fonts
 for (int i=0; i<=6; i++) font[i]=a95vol.font->duplicate();
 font[0]->convertcolor(0xca,0xc6); font[0]->convertcolor(0xdb,0xd8);
 font[1]->convertcolor(0xca,0xff); font[1]->convertcolor(0xdb,0x0);
 font[2]->convertcolor(0xca,6*16); font[2]->convertcolor(0xdb,0x0);
 font[3]->convertcolor(0xca,19);   font[3]->convertcolor(0xdb,0x0);
 font[4]->convertcolor(0xca,0x0);  font[4]->convertcolor(0xdb,15);
 font[5]->convertcolor(0xca,2*16+1);   font[5]->convertcolor(0xdb,2*16+14);

 //create root
 root=new ROOT;

 //create root of all GUI
 new GUIroot(root);


 initdefaultgui();

 //create objectspace
 objectspace::open("object.bin","bg.bin",320,200);

 //create bg
 objspace->newbgobject(1);

 //create main dude
 inputdevice[0]->bind(objspace->bg->track=objspace->newobject("Disgruntled",100,SETBASEY,0,0));


 return 0;
}

void terminategame()
{
 delete root;
 a95vol.free();
 delete cfg;
}

void gametimer()
{
 uu++;
 input::refreshtimer();
 inputdevice[0]->read();

 if (objspace) objspace->tick();
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




int blah=0;

void updatescreen()
{
 //draw message buffer
 input::refreshmain();

 if (guienabled)
 {
 //draw everything
 root->draw(screen);
 m.draw(screen); //draw cursor
 } else objspace->draw(screen);

 su++;
 if (cfg->get(CFG_SHOWFPS))
  {
   font[0]->printf(SCREENX-70,50,"fps=%d",calcfps());
   font[0]->printf(SCREENX-70,60,"spf=.%04d",spf);
  }

  font[0]->printf(SCREENX-150,80,"%d",blah);
// font[0]->printf(SCREENX-150,80,"objspace:%p",objspace);

}



//------------------------------
//       ROOT node
//------------------------------

ROOT::ROOT():GUIrect(0,0,0,SCREENX,SCREENY){}
ROOT::~ROOT() {}

void ROOT::resize(int xw,int yw)
{
 if (guiroot) guiroot->resize(xw,yw);
// if (objspace) objspace->resize(xw,yw);
 if (objspace) objspace->resize(xw,yw);
 GUIrect::resize(xw,yw);
}

void ROOT::refresh(int r,void *c)
  {
  // msg.printf(2,"refresh %d",r);
   for (GUIrect *g=child; g; g=g->next)  g->refresh(r,c);
  };






