#include <dos.h>
#include <i86.h>
#include <conio.h>
#include "ttimer.h"

#include <stdio.h>

extern "C" {
void __cdecl SetDPMIVector(void far *vector,int num);
void __cdecl far *GetDPMIVector(int num);
int __cdecl  CheckDS();
void __cdecl SwitchStack();
void __cdecl SwitchBack();

extern volatile int smixbusy;

}

void (*timerhandler)(void)=0;  //pointer to our handler
void (*musichandler)(void)=0;  //pointer to the music handler

int timerbusy=0;

static void (interrupt far *oldintvector)(void);

unsigned int speed=100;     //speed of our handler,  ticks/sec
unsigned int mspeed=0;  //speed of music handler, ticks/sec
unsigned int difcnt=0;                   //differential count of rates between two handlers
unsigned int cnt=0;           //current count

unsigned int olddifcnt=0; //counts for old handler
unsigned int oldcnt=0;

unsigned int itu=0,imu=0,iuu=0;

void TimerSpeed(int x)
{
 outp(0x43,0x36);
 outp(0x40,x&0xFF);
 outp(0x40,(x>>8)&0xFF);
}    

//adjust the rate, speed, divisors
void AdjustTimer()
{
//asmcli;
_disable();
 if (mspeed>=speed)  //mspeed has precedence...
        {
         difcnt=(speed<<16)/mspeed;   //times that our handler must be called for ever one musichandler (less than one)
         cnt=0;
         TimerSpeed(1193180L/mspeed); //mspeed dictates overall timer speed
         olddifcnt=(182<<16)/10/mspeed;
         oldcnt=0;
        }
          else               //our handler speed has precedence
        {
         difcnt=(mspeed<<16)/speed;   //times that the music handler must be called for ever one of our handler (less than one)
         cnt=0;
         TimerSpeed(1193180L/speed);      //speed dictates overall timer speed

         olddifcnt=(182<<16)/10/speed;
         oldcnt=0;
        }
_enable();        
//asmsti;

}

extern volatile int quit;

extern "C" {
    void __cdecl loades();
}


//Generic handler
void interrupt TIMERIRQ()
{

if (!timerbusy) // && !smixbusy)
{
 timerbusy=1;
 loades();
 _enable();

// *((char *)(0xA0000+320*199+319))=1;
//SwitchStack();

 if (mspeed>=speed)
  {
       
         if (musichandler) (*musichandler)();
//         imu++;
         cnt+=difcnt;           //see if we have to call other handler
         if (cnt&0xFFFF0000)            //we do
          {
//               iuu++;
                cnt&=0xFFFF;   //clear int portion
                if (timerhandler) (*timerhandler)();  //call it
          }
  } else
  {
         if (timerhandler) (*timerhandler)();
//         iuu++;
         
         cnt+=difcnt;           //see if we have to call other handler
         if (cnt&0xFFFF0000)            //we do
          {
//              imu++;
                cnt&=0xFFFF;   //clear int portion
                if (musichandler) (*musichandler)();  //call it
          }
  }

_disable();         
//SwitchBack();
timerbusy=0;
}



oldcnt+=olddifcnt;
if (oldcnt&0xFFFF0000)
{
  oldcnt&=0xFFFF;
//itu++;
(*oldintvector)();
}  


outp(0x20,0x20);
//outp(0xA0,0xA0);
//*((char *)(0xA0000+320*199+319))=0;  
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


void InitializeTimers()
{

_disable();
oldintvector = _dos_getvect(8);
_dos_setvect(8, TIMERIRQ);
//oldintvector=(void interrupt (far *)())GetDPMIVector(8);
//SetDPMIVector((void far *)TIMERIRQ,8);
_enable();


AdjustTimer();

//printf("difcnt %X\n",olddifcnt);
}

void TerminateTimers()
{
_disable();
_dos_setvect(8, oldintvector);
//SetDPMIVector((void far *)oldintvector,8);
TimerSpeed(0);
_enable();
}
