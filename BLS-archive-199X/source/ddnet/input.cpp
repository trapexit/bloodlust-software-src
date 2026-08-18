#include <dos.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>

#include "glib.h"
#include "input.h"
#include "keyb.h"

#include "effect.h"
#include "object.h"

#include "dinput.h"

//input device settings.
inputdevicesettings *ids=0;

//last read analog positions/buttons
JOYINFO joypos[2];

//whether or not each joystick is installed
char analoginstalled=0;

//keyboardstats for the two keyboard input devices that need to be updated
unsigned kstat[2]={0,0};
keymap   *keymapptr[2]={0,0}; //keyboard maps for the input devices

//main loop input hardware refresh
int refreshinputkeyboard()
{
  //update the keystats from the keymaps
 if (keymapptr[0]) if (keymapptr[0]->read(&kstat[0])) return 1;
 if (keymapptr[1]) if (keymapptr[1]->read(&kstat[1])) return 1;
 return 0;
}

void refreshinputtimer()
{
  //refresh analog buttons on timer
// if (analoginstalled) joybuttons=ReadAnalogJoyButtons();

}

void refreshinputmain()
{
  //refresh analog only on main loop
 if (analoginstalled&1)
  if (joyGetPos(0,&(joypos[0]))!=JOYERR_NOERROR) analoginstalled^=1;

 if (analoginstalled&2)
  if (joyGetPos(1,&(joypos[1]))!=JOYERR_NOERROR) analoginstalled^=2;

}

//binds the input device to an object...
void input::bind(object *t)
{
 if (!this) return;
  //unbind the object from whatever it's attached to
 unbind();
 //bind to object
 o=t; o->in=this;
}

//unbinds input device from object it's attached to
void input::unbind()
{
 if (!this) return;
 if (o) //if object attached
  { //unbind from object
    o->in=NULL;
    o=NULL;
  }
}



//input initialization
void input::init(char t)
{ //if no inputdevicesettings, can't initialize
 if (!ids) t=ID_NONE;

 type=t;
 oldstat=stat=but=0; keystat=0;
 o=NULL; //not bound to any object


 switch (type)
  {
   case ID_JOY1:
       analoginstalled|=1;
      break;
   case ID_JOY2:
       analoginstalled|=2;
      break;

   case ID_GRAVIS:
       analoginstalled|=1;
      break;


   case ID_KEY1:
        keymapptr[0]=&ids->km[0]; //set up keymap pointers
        kstat[0]=0;
        break;
   case ID_KEY2:
        keymapptr[1]=&ids->km[1]; //set up keymap
        kstat[1]=0;
       break;
  }
}

void input::reset()
{
 if (!this) return;
 stat&=0xF; //clear button triggers
 oldstat=stat;
}

unsigned input::getstat()
{
if (!this) return 0;
return stat;
}

//input reading functions
void (input::* input::idfunc[])()=
 {
  &input::none,
  &input::gravis,
  &input::key,
  &input::key,
  &input::analog1,
  &input::analog2,
  &input::none,
  &input::none,
 };


//updates the 'stat' position and trigger information from the hardware latches
//returns true if input device was read
void input::read()
{
 oldstat=stat;
 (this->*(idfunc[type]))();
}

void input::none() {}

void input::analog1()
{
stat&=~0xF; //clear stat dirs
if (joypos[0].wXpos>32768+5000) stat|=ID_RIGHT;  else
if (joypos[0].wXpos<32768-5000) stat|=ID_LEFT;
if (joypos[0].wYpos>32768+5000) stat|=ID_DOWN; else
if (joypos[0].wYpos<32768-5000) stat|=ID_UP;


int x=(joypos[0].wButtons)<<4;
stat|=((but^x)&x);
but=x;

if (o && o->d && (stat&3)) stat^=3;
}

void input::analog2()
{
stat&=~0xF; //clear stat dirs
if (joypos[1].wXpos>32768+5000) stat|=ID_RIGHT;  else
if (joypos[1].wXpos<32768-5000) stat|=ID_LEFT;
if (joypos[1].wYpos>32768+5000) stat|=ID_DOWN; else
if (joypos[1].wYpos<32768-5000) stat|=ID_UP;

int x=(joypos[0].wButtons)<<4;
stat|=((but^x)&x);
but=x;

if (o && o->d && (stat&3)) stat^=3;
}

unsigned char gravtable[16]=
{
 0, //0000
 1, //0001
 4, //0010
 5, //0011

 2, //0100
 3, //0101
 6, //0110
 7, //0111

 8, //1000
 9, //1001
 12, //1010
 13, //1011

 10,//1100
 11,//1101
 14,//1110
 15,//1111
};

void input::gravis()
{
stat&=~0xF; //clear stat dirs
if (joypos[0].wXpos>32768+5000) stat|=ID_RIGHT;  else
if (joypos[0].wXpos<32768-5000) stat|=ID_LEFT;
if (joypos[0].wYpos>32768+5000) stat|=ID_DOWN; else
if (joypos[0].wYpos<32768-5000) stat|=ID_UP;

int x=(gravtable[joypos[0].wButtons&15])<<4;
stat|=((but^x)&x);
but=x;

if (o && o->d && (stat&3)) stat^=3;
}

int keymap::read(unsigned *kstat)
{
register unsigned char scan=(unsigned char) (kbscan&0x7f);
for (int i=0; i<sizeof(keymap); i++)
  if (scan==((char *)this)[i]) //compare scan codes
  {
   if (kbscan&0x80) *kstat&=~(1<<i); //release keystat
       else         *kstat|=1<<i; //press keystat
   return 1; //we found da key
  }
return 0; //none of our mapped key
}

void  input::key()
{

stat|=((kstat[type&1]&(~keystat))&0xFF00)>>4;

//store keystat
keystat=kstat[type&1];
//recalculate stats
stat&=~0xF;
if (keystat& ( (1<< 0) + (1<< 1) + (1<< 2)  ) ) stat|=ID_UP;   //Up
       else
if (keystat& ( (1<< 5) + (1<< 6) + (1<< 7)  ) ) stat|=ID_DOWN;   //Down

if (keystat& ( (1<< 2) + (1<< 4) + (1<< 7)  ) ) stat|=ID_RIGHT;   //right
         else
if (keystat& ( (1<< 0) + (1<< 3) + (1<< 5)  ) ) stat|=ID_LEFT;    //Left

but=(keystat&0xFF00)>>4;

if (o && o->d && (stat&3)) stat^=3;
};










