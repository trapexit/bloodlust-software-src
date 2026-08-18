//G68 cpu control
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "types.h"

#include "g68k.h"


dword gettime();

void g68state::dumpreg()
{
 printf("Dn: %08X %08X %08X %08X %08X %08X %08X %08X\n",Dn[0],Dn[1],Dn[2],Dn[3],Dn[4],Dn[5],Dn[6],Dn[7]);
 printf("An: %08X %08X %08X %08X %08X %08X %08X %08X\n",An[0],An[1],An[2],An[3],An[4],An[5],An[6],An[7]);
}

void g68state::dumpflags()
{
 printf("CCR: %cX %cN %cZ %cV %cC\n",
  getflag(G68FLAG_X) ? '+' : ' ',
  getflag(G68FLAG_N) ? '+' : ' ',
  getflag(G68FLAG_Z) ? '+' : ' ',
  getflag(G68FLAG_V) ? '+' : ' ',
  getflag(G68FLAG_C) ? '+' : ' '  );
}

void g68state::dumpdisasm()
{
 char s[64];
 int size=disasm(s);
 printf("%08X: %s (%d)\n",PC,s,size);
}

void g68state::dumpinvalid()
{
 printf("Invalid opcode %04X: PC=%X\n",readw(PC),PC);
}

//memory read functions
byte g68state::readb(dword d)
{
 if (d<0x400000) return rom[d];
 if (d>0xE00000) return ram[d&0xFFFF];
 return 0;
}
word g68state::readw(dword d)
{
 if (d<0x400000) return SWAPW(*((word *)(rom+d)));
 if (d>0xE00000) {d&=0xFFFF; return SWAPW(*((word *)(ram+d)));}
 return 0;
}
dword g68state::readd(dword d)
{
 if (d<0x400000) return SWAPD(*((dword *)(rom+d)));
 if (d>0xE00000) {d&=0xFFFF; return SWAPD(*((dword *)(rom+d)));}
 return 0;
}

//------------------------------------------------------



void g68state::debugkey(char key)
{
 switch(key)
 {
   case 't':
    if (execute(1)) dumpinvalid();
    dumpdisasm();
    break;

   case 'd': dumpreg(); break;
   case 'f': dumpflags();  break;

   case 's':
    {
     int n;
     printf("Dn: ");
     while (kbhit()) getch();
     scanf("%X",&n);
     if (n<0 || n>7) break;
     printf("D%d:",n);
     scanf("%X",&Dn[n]);
    }
    break;

   case 'a':
    {
     int n;
     printf("An: ");
     scanf("%X",&n);
     if (n<0 || n>7) break;
     printf("A%d:",n);
     scanf("%X",&An[n]);
    }
    break;

   case 'b':
    {
     printf("break: ");
     scanf("%X",&breakpoint);
    }
    break;
   case 'B': breakpoint=0; break;

   case 'v': //vertical int
     doint(0x78);
     printf("vertical int triggered\n");
    break;

   case 'y':
    breakpoint=PC+disasm(0);
    printf("running to: %X\n",breakpoint);

   case 'r':
    {
     for (int i=0; i<100000; i++)
     {
      if (PC==breakpoint) {printf("Break\n");break;}
      if (execute(1))  {dumpinvalid(); break;}
     }
    dumpdisasm();
    }
    break;

   case 'R':
    if (execute(100000)) dumpinvalid();
    dumpdisasm();
   break;
   case 'g':
   do
   {
    unsigned inst=2000000;
    unsigned start=gettime();
    if (execute(inst)) {dumpinvalid(); break;}
    unsigned length=gettime()-start;
    printf("%d instructions/sec\n",inst*1000/length);
   } while (!kbhit());
    while(kbhit()) getch();
   break;
 }


}






















