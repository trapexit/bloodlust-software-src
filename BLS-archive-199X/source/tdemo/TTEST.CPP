#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "ttimer.h"

int uu=0;

//volatile int timerbusy;



void test()
{
 int i=0;
 for (i=0; i<10; i++);
 uu++;
}    

void main()
{

printf("initializing timer...");
InitializeTimers();
SetTimerSpeed(100);
timerhandler=test;
printf("done\n");



do
{
 printf("%d\n",uu);
} while (!kbhit());
    

TerminateTimers();
}    



