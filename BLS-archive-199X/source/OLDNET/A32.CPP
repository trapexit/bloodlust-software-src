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

#include "gui.h"

#include "amenu.h"

#include "uutimer.h"


#include "object.h"
#include "objdef.h"
#include "objspace.h"
#include "bg.h"

#include "a32.h"


char appname[]="A95";
char configfile[]="a95.cfg";
int SCREENX=640;
int SCREENY=480;


void enablegui() {}
//config info
config *cfg;

//input devices
input *inputdevice[2];
inputdevicesettings *ids;




int cfuncnum;
CFUNCLISTPTR cfuncnames;
int readcfuncnames(char *file,CFUNCLISTPTR &c);

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





int keyboard();

int readcfuncnames(char *file,CFUNCLISTPTR &c);


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

  //create default gui boxes
 initdefaultgui();


 //read control functions
 cfuncnum=readcfuncnames("cfunc.lst",cfuncnames);

/* msg.printf(2,"size=%d",sizeof(zdpoint));
 msg.printf(2,"effsize=%d",sizeof(effect));*/
 return 0;
}

void terminategame()
{
// delete root;
 a95vol.free();
 delete cfg;
}

void gametimer()
{
 uu++;
 input::refreshtimer();
 inputdevice[0]->read();

 if (objspace) objspace->tick();

 inputdevice[0]->reset();
}


int keyboard()
{
 if (kbscan==KB_ESC) {quitgame(); return 1;}

 if (input::refreshkeyboard(kbscan)) return 1; //dont add key to queue

// msg.printf(1,"scan: %X key: %c",kbscan,scan2ascii(kbscan&0x7f));
 if (kbscan&0x80) return 1; //release

 char key=scan2ascii(kbscan);
 if (root && root->keyhit(kbscan,key)) return 1;

 return 1;
}


void m_quit()
{
 quitgame();
}

void scanguitree(GUIrect *r,lpoint &p,GUIrect *parent)
{
 if (!r) return;
 if (parent!=r->parent)  {font[5]->printf(p.x,p.y,"ERROR parent link"); return;}

 font[GUIrect::modal!=r ? (r->focus ? 1 : 0) : 4]->printf(p.x,p.y,"%s (%d,%d) (%d,%d)",r->getname(),r->x1,r->y1,r->width(),r->height()); p.y+=10;


 p.x+=10;
   scanguitree(r->child,p,r);
 p.x-=10;

 if (r->next && r->next->prev!=r) {font[5]->printf(p.x,p.y,"ERROR sibling link"); return;}
 scanguitree(r->next,p,parent);
}

int blah=0;
int seriesstarts=0;

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








extern int blah;
void updatescreen()
{
 //draw message buffer
// if (!m.capture) msg.draw(SCREENX/2,10);
 input::refreshmain();

 //draw everything
 root->draw(screen);

 if (cfg->get(CFG_SHOWGUITREE))
 {
  font[0]->printf(20,40,"mouse capture: %s",m.capture ? m.capture->getname() : "none");
  lpoint p={20,60};
  scanguitree(root,p,0);
 }

 su++;
 if (cfg->get(CFG_SHOWFPS))
  {
   font[0]->printf(SCREENX-70,50,"fps=%d",calcfps());
   font[0]->printf(SCREENX-70,60,"spf=.%04d",spf);
  }

//  font[0]->printf(SCREENX-70,70,"%d",blah);
// font[0]->printf(SCREENX-70,70,"%d",inputdevice[0]->stat);
// font[0]->printf(SCREENX-70,80,"ss=%d",seriesstarts);
//font[0]->printf(SCREENX-70,70,"%d",blah);
// font[0]->printf(SCREENX-150,70,"wrkspace:%p",wrkspace);
// font[0]->printf(SCREENX-150,80,"objspace:%p",objspace);

 m.draw(screen); //draw cursor
}



//------------------------------
//       ROOT node
//------------------------------

ROOT::ROOT()
:GUIrect(0,0,0,SCREENX,SCREENY)
{
// setfocus(this);
}

ROOT::~ROOT() {}

void ROOT::resize(int xw,int yw)
{
// for (GUIrect *g=child; g; g=g->next)
//    g->resize(xw,yw);
 guiroot->resize(xw,yw);
 if (objspace)
  {
   GUImaximizebox *b=(GUImaximizebox *)objspace->parent;
   if (b)
    if (b->maximized) {b->restore(); b->maximize();}
     else if (xw<=320) b->maximize();

  }
 GUIrect::resize(xw,yw);
}

void ROOT::refresh(int r,void *c)
  {
  // msg.printf(2,"refresh %d",r);
   for (GUIrect *g=child; g; g=g->next)  g->refresh(r,c);
  };


void resizeroot(int xw,int yw) {root->resize(xw,yw);}






int readcfuncnames(char *file,CFUNCLISTPTR &c)
{
 FILE *f;
 f=fopen(file,"rt");
 if (!f) return 0;

 c=(CFUNCLISTPTR)calloc(256,sizeof(CFUNCNAME));

 char line[100];
 do {fscanf(f,"%s",line);} while (line[0]!='{'); //scan till first {

 for (int i=0; ; i++)
 {
  fscanf(f,"%s",line); //scan each funcname
  if (line[0]=='}') break; //done reading cfuncs
  *strchr(line,',')=0; //remove comma
  strcpy((*c)[i],strchr(line,':')+2);
 }

 c=(CFUNCLISTPTR)realloc(c,i*sizeof(CFUNCNAME));
 fclose(f);
 msg.printf(2,"%d control functions read",i);
 return i;
}


