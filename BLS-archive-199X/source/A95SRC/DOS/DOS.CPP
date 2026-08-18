//dos interface functions
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "dos\keyb.h"
#include "dos\ttimer.h"

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
//int SCREENX=320;
//int SCREENY=200;
};

char *video;   // Pointer to video memory
char *screen; // Pointer to virtual screen

mouse m;


int timeperframe=0;

char errstr[80];
void cleanexit(int x)
{
 modetext();
 printf("Error: %s\n",errstr);
 exit(x);
}

void playsound(SOUND *s)
{

}    


void changeresolution(int x,int y)
{
 msg.printf(2,"DOS graphics limited to 320x200x256.");
}    

void setpalette(PALETTE *p)
{
 p->set(0,256);
}    


void mode256();
void modetext();

static int quit=0;

void quitgame()
{
 quit=1;
}    

extern int  (*keyboardhandler)(void);  //pointer to keyboard handler

//dos main
void main()
{

 //initialize mouse
 initmouse();

 //start up keyboard handler
 init_keyboard();
 keybios=0;
 keyrepeat=1;
 keyqueue=0; //1;

 //setup timer
 InitializeTimers();

 mode256();

 SCREENX=320;
 SCREENY=200;
 PITCH=SCREENX;
 screen=(char *)malloc(80000)+2000;
 video=(char *)0xA0000;
 msg.printf(2,"DOS graphics initialized. %dx%dx%d",SCREENX,SCREENY,8);

//   modetext();
 if (initgame())
  {
   modetext();
   return;
  }
 //  return;

// mode256();

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

  if (!cfg->get(CFG_NOFILLEDDESKTOP))
      memset(screen,1,SCREENX*SCREENY);

  //update screen  
  updatescreen();

  //copy to video buffer
  memcpy(video,screen,SCREENX*SCREENY);
 } while (!quit);


 SetTimerFunc(0);
 set_keyboard_func(0);

 terminategame();
 modetext();

}    




//dos surface
surface::surface(int txw,int tyw)
{
 s=SCR::newSCR(txw,tyw);
}

surface::~surface()
{
 free(s);
}

char *surface::lock()
{
 if (!s) return 0;

 oldscrx=SCREENX; oldscry=SCREENY; oldpitch=PITCH;
 SCREENX=s->xw;
 SCREENY=s->yw;
 PITCH=s->xw;

 return s->data();
}

void surface::unlock()
{
 if (!s) return;
 SCREENX=oldscrx; SCREENY=oldscry; PITCH=oldpitch;
}


void surface::blt(char *dest,int x,int y)
{
 if (s) s->draw(dest,x,y);
}









void mode256();
#pragma  aux mode256 =  \
  "mov eax,13h"            \
  "int 10h"               \
  parm   []               \
  modify [eax];

void modetext();
#pragma  aux modetext =  \
  "mov eax,3h"            \
  "int 10h"               \
  parm   []               \
  modify [eax];







































