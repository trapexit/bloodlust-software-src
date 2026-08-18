#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include <windows.h>

#include "types.h"

#include "file.h"

#include "gen.h"

//current genesis machine
gmachine *gm=0;

dword gettime()
{
 return timeGetTime();
}

//speed up disp16
//bcd shit
//USP/SSP

byte g68_readb(dword a);
word g68_readw(dword a);
dword g68_readd(dword a);
void g68_writeb(byte d,dword a);
void g68_writew(word d,dword a);
void g68_writed(dword d,dword a);

//--------------------------------------------

void main(int argc,char *arg[])
{
 #ifdef WIN95
// HANDLE hProc=GetCurrentProcess();
// SetPriorityClass(hProc,HIGH_PRIORITY_CLASS);
 #endif

 printf("G68K test program\n");
 printf("Copyright 1997 Bloodlust Software\n\n");

 //build cpu
 g68_init();
 //set trap funcs
 g68_readbtrap=(dword)g68_readb;
 g68_readwtrap=(dword)g68_readw;
 g68_readdtrap=(dword)g68_readd;
 g68_writebtrap=(dword)g68_writeb;
 g68_writewtrap=(dword)g68_writew;
 g68_writedtrap=(dword)g68_writed;
 printf("CPU built\n");

 //create genesis machine
 gm=new gmachine();

 //load rom
 if (gm->loadrom((argc<2) ? "strider.bin" : arg[1])) return;

 gm->gcpu.dumpreg();
 gm->gcpu.dumpdisasm();

 int done=0;
 do
 {
  char key=getch();

  switch (key)
  {
   case 'h':
    while (!kbhit() && !gm->emulateframe(0));
    while(kbhit()) getch();
   break;


   case 'w': gm->writeram("x.ram"); break;

   case 'q':
   case 27:
     done=1;
    break;
   default: gm->gcpu.debugkey(key);
  };

 } while (!done);

 if (gm) delete gm;
}











