#include <stdio.h>
#include <conio.h>

#include "ttimer.h"
#include "tkeyb.h"


extern "C" {
   void __cdecl ModeText();
   
}   

volatile int uu=0;
volatile int quit=0;

void Tick()
{
uu++;

}


void Keyboard()
{
quit=1;
}    


void main()
{

//ModeText();

printf("-initializing keyboard...");
InitKeyboard(Keyboard);
printf("done\n");

//Initialize Timer
printf("-initializing timer...");
InitializeTimers();
SetTimerSpeed(100);
printf("done\n");

timerhandler=Tick;

do
{
printf("%d\n",uu);


} while (!quit);


TerminateTimers();

TerminateKeyboard();
}

