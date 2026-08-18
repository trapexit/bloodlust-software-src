//dos interface functions
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "vesa\include\svga.h"

#include "vesa\keyb.h"
#include "vesa\ttimer.h"

#include "r2img.h"
#include "mouse.h"
#include "input.h"
#include "config.h"
#include "message.h"

#include "dd.h"



extern "C" {
    void __cdecl initmouse();
    int  __cdecl readmouse(int *x,int*y);
};


void _disable() {};
void _enable() {};  


extern "C" {
int PITCH=320;
int SCREENX=320;
int SCREENY=200;
};

//super vga stuff
SV_devCtx *SVGA=0;


char *video;   // Pointer to video memory
char *screen; // Pointer to virtual screen

//config info
config *cfg;

//input devices
inputdevicesettings *ids;
input *inputdevice[2];
mouse m;


char errstr[80];
void cleanexit(int x)
{
 printf("Error: %s\n",errstr);
 exit(x);
}


void changeresolution(int x,int y)
{
 msg.printf(2,"DOS graphics limited to 320x200x256.");
}    

void setpalette(PALETTE *p)
{
 //p->set(0,256);
}    


static int quit=0;

void quitgame()
{
 quit=1;
}    

extern int  (*keyboardhandler)(void);  //pointer to keyboard handler


//dos main
void main()
{
 SVGA=SV_init(1);
 if (!SVGA) {printf("svga not initialized\n"); return;}
 printf("svga initialized\n");
 return ;
 

 //get configuration
 cfg=new config();
 cfg->load("a95.cfg");

 //initialize input devices
 ids=&cfg->ids; //copy over input settings
 inputdevice[0]=newinputdevice(cfg->pinput[0]);
 inputdevice[1]=newinputdevice(cfg->pinput[1]);

 //initialize mouse
 initmouse();

 //start up keyboard handler
 init_keyboard();
 keybios=0;
 keyrepeat=1;
 keyqueue=0; //1;

 //setup timer
 InitializeTimers();

// mode256();

 screen=(char *)malloc(64000);
 video=(char *)0xA0000;
 msg.printf(2,"DOS graphics initialized. %dx%dx%d",SCREENX,SCREENY,8);

 if (initgame())
  {
   //modetext();
   return;
  }

 SetTimerSpeed(100);
 SetTimerFunc(gametimer);

 do
 {
  //update mouse
  {
   int x,y;
   m.reset();
   m.updatebut(readmouse(&x,&y));
   m.updatexy(x,y);
  }

 //process keys
  while (keyhit())
   {
    kbscan=getkey();
    keyboardhandler();
   }

  memset(screen,1,SCREENX*SCREENY);
  //update screen  
  updatescreen();

  //copy to video buffer
  memcpy(video,screen,SCREENX*SCREENY);
 } while (!quit);

 SetTimerFunc(0);
 set_keyboard_func(0);

 terminategame();
//modetext();
}    






































