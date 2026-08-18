#include <dos.h>
#include <stdlib.h>
#include <i86.h>
#include <conio.h>
#include "ttimer.h"

#include <stdio.h>

//#define PROFILE

extern "C" {
 void SwitchStack();
 void SwitchBack();
};

void (*timerhandler)(void)=0;  //pointer to our handler
void (*musichandler)(void)=0;  //pointer to the music handler

volatile int timerbusy=0;

static void (interrupt far *oldintvector)(void);

unsigned int speed=100;     //speed of our handler,  ticks/sec
unsigned int mspeed=0;  //speed of music handler, ticks/sec
unsigned int difcnt=0;                   //differential count of rates between two handlers
unsigned int cnt=0;           //current count

unsigned int olddifcnt=0; //counts for old handler
unsigned int oldcnt=0;

unsigned int itu=0,imu=0,iuu=0;

void TimerSpeed(unsigned int x)
{
 outp(0x43,0x36);
 outp(0x40,x&0xFF);
 outp(0x40,(x>>8)&0xFF);
}    

//adjust the rate, speed, divisors
void AdjustTimer()
{
_disable();
 TimerSpeed(1193180/speed);      //speed dictates overall timer speed
_enable();        

}

extern volatile int quit;





void loades();
#pragma  aux loades =  \
  "push ds"            \
  "pop  es"               \
  parm   []               \
  modify [];


extern volatile unsigned uu;

//Generic handler
void interrupt __loadds TIMERIRQ()
{

uu++;

outp(0x20,0x20);
}





void __cdecl SetTimerSpeed(unsigned int x)
{
 speed=x;
 AdjustTimer();
}

void __cdecl  SetMusicSpeed(unsigned int x)
{
 mspeed=x;
 AdjustTimer();
}

void __cdecl SetMusicFunc(void (*func)(void))
{
musichandler=func;
}    

void __cdecl SetTimerFunc(void (*func)(void))
{
timerhandler=func;
}    

static int timersinstalled=0;
void _proftimer();

void InitializeTimers()
{
if (timersinstalled) return;    

_disable();
oldintvector = _dos_getvect(8);
_dos_setvect(8, TIMERIRQ);
_enable();

AdjustTimer();
atexit(TerminateTimers);

timersinstalled=1;

#ifdef PROFILE
SetMusicFunc(_proftimer);
SetMusicSpeed(1000);
#endif
}

void TerminateTimers()
{
if (!timersinstalled) return;    
_disable();
_dos_setvect(8, oldintvector);
TimerSpeed(0);
_enable();
timersinstalled=0;
}



#ifdef PROFILE

//profiling code
int areastack[16];
int areacnt=0;

//all area counts
int area[16];
int profticks;
#endif

//enters an area
void _enter(int area)
{
 area=area;
 #ifdef PROFILE
 areastack[areacnt++]=area;
 #endif
}    


void _leave()
{
 #ifdef PROFILE
 if (areacnt>0) areacnt--;
 #endif
}


void _proftimer()
{
 #ifdef PROFILE
 profticks++;
     //increase counts for everything on stack
//for (int i=0; i<areacnt; i++)
 if (areacnt>0)
  area[areastack[areacnt-1]]++;
 #endif
}



    




