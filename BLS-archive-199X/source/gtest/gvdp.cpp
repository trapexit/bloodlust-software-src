//G68 memory trap functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "types.h"

#include "g68k.h"
#include "gvdp.h"


//----------------------------------------------------
// VDP

static int ssize[4]={32,64,64,128};

void VDP::regwrite(int n,byte d)
{
 if (n>23) return;
 //set register
 REG[n]=d;
 printf("REG#%d=%X\n",n,d);

 switch (n)
 {
  //mode set #1
  case 0: Hintenable=(d&0x10); HVcounterenable=(d&2) ? 0 : 1; break;
  //mode set #2
  case 1:
    displayenable=d&0x40;
    Vintenable=d&0x20;
    dmaenable=d&0x10;
    vres=(d&0x8) ? 30 : 28;
   break;

  //base address
  case 2: ntAbase=(d&0x38)<<10; break;
  case 3: if (hres==40) d&=0x3C; ntWbase=(d&0x3E)<<10; break;
  case 4: ntBbase=(d&0x7)<<13; break;
  case 5: if (hres==40) d&=0x7E; spritebase=(d&0x7F)<<9; break;

  //bg color
  case 7: bgcolor=d&0xF; bgpal=(d>>4)&3; break;
  case 12: hres=(d&0x81) ? 40 : 32; shadow=d&8; interlace=(d>>1)&3; break;
  case 13: hscrollbase=(d&0x3F)<<10; break;

  //address increment
  case 15: ainc=d; break;
  //scroll size
  case 16:
    hsize=ssize[(d)&3];
    vsize=ssize[(d>>4)&3];
   break;
  //DMA length
  case 19: dmalength =((word)d); break;    //low
  case 20: dmalength|=((word)d)<<8; break; //high

  //DMA addr
  case 21: dmaaddr =((dword)d); break;    //low
  case 22: dmaaddr|=((dword)d)<<8; break; //mid
  case 23: dmaaddr|=((dword)d)<<16; dmamode=d>>6;
           if (d&0x80) dmaaddr&=0xFFFF; //vram fill/copy
             else {//memory->vram
                   if (d&0x3F) dmaaddr|=0xFF0000;
                          else dmaaddr&=0x7FFFFF;
                  }
           break; //high
 }
}



void VDP::startvblank()
{
 vblank=1; frame=0;
}

void VDP::startframe()
{
 vblank=0; frame=1;
}


word VDP::statusread()
{
 return vblank ? 0x8 : 0;
}


//---------------------------------------

//$C00000
inline void VDP::datawriteb(byte d)
{
 switch (idcode&0xF)
 {
  case 1:
   vram[addr^1]=d;
   addr+=ainc;
   break; //vram
  case 3:
   //printf("cram[%X]=%X\n",addr,d);
   ((byte *)cram)[(addr^1)&0x7F]=d;
   addr+=ainc;
  break; //cram
  case 5: break; //vsram
 }
}
inline void VDP::datawritew(word d)
{
 switch (idcode&0xF)
 {
  case 1:
   vram[addr]=d>>8; vram[addr^1]=d&0xFF;
   addr+=ainc;
   break;
  case 3:
   // printf("cram[%X]=%X\n",addr,d);
   cram[(addr>>1)&0x3F]=d;
   addr+=ainc;
  break; //cram
  case 5: break; //vsram
 }
}

inline word VDP::datareadw()
{
 word d;
 switch (idcode&0xF)
 {
  case 0:
   d=(vram[addr&(~1)]<<8) | vram[addr|1];
   addr+=ainc;
   return d;
  case 8: return 0; //cram
  case 4: return 0; //vsram
  default: return 0;
 }
}



//$C00004
void VDP::controlwritew(word d)
{
 //register?
 if ((d&0xC000)==0x8000) {regwrite((d&0x1F00)>>8,d&0xFF); flip=0; return;}
 //set addressing mode
 if (!flip)
 {
  idcode=d>>14;  //CD1 CD0
  addr=d&0x3FFF; //A13-0
 } else
 {
  idcode|=(d&0xF0)>>2; //idcode&=0xF;
  addr|=(d&3)<<14; //A15-14
  //  if (idcode==3 && !dmalength)
  printf("VDPid=%X addr=%X\n",idcode,addr);
 //  if (dmalength && dmamode<2) startdma(); //begin mem->vram DMA
 }
 flip^=1;
}



//-----------------------------------
// VDP reads

byte  VDP::readb(dword a) {return 0;}
dword VDP::readd(dword a) {return readw(a)|(readw(a+2)<<16);}
word  VDP::readw(dword a)
{
// printf("VDPreadw[%08X]\n",a);
 switch (a)
 {
  case 0xC00000:
  case 0xC00002: return datareadw();
  case 0xC00004:
  case 0xC00006: flip=0; return statusread();
 }
 return 0;
}



//-----------------------------------
// VDP writes

void VDP::writeb(dword a,byte d)
{
// printf("VDPwriteb[%08X]=%02X\n",a,d);
 switch(a)
 {
  case 0xC00000: //data port
  case 0xC00001:
  case 0xC00002:
  case 0xC00003: datawriteb(d); break;
 }
}

void VDP::writew(dword a,word d)
{
// printf("VDPwritew[%08X]=%04X\n",a,d);
 switch(a)
 {
  case 0xC00000: //data port
  case 0xC00002: datawritew(d);   break;
  case 0xC00004: //control port
  case 0xC00006: controlwritew(d);  break;
 }
}

void VDP::writed(dword a,dword d)
{
// printf("VDPwrited[%08X]=%08X\n",a,d);
 switch(a)
 {
  case 0xC00000:
    datawritew((word)(d>>16));
    datawritew((word)(d));
    break;
  case 0xC00004: //control port
   controlwritew((word)(d>>16));
   controlwritew((word)(d));
  break;
 }
}


//----------------------
/*
void VDP::startdma()
{
 if (dmaenable)
 switch(dmamode)
 {
   case 0x00: //RAM->VRAM
   case 0x01:

//     printf("DMA %X %X %X\n",dmaaddr,addr,dmalength);
    switch (idcode)
    {
//     case 1: printf("RAM->VRAM %X %X %X\n",dmaaddr,addr,dmalength); break;
//     case 3: printf("RAM->CRAM %X %X %X\n",dmaaddr,addr,dmalength); dobreak(); break;
//     case 5: printf("RAM->SRAM %X %X %X\n",dmaaddr,addr,dmalength); break;
    }
     while (dmalength>0)
     {
      datawritew(gcpu.readw(dmaaddr));
      dmaaddr+=2;
      dmalength--;
     }

    break;

   case 0x10:
    dmamode=0;
//    printf("VRAM fill\n");
   break; //VRAM FILL
   case 0x11:
    dmamode=0;
//    printf("VRAM copy\n");
   break; //VRAM copy
 }

 dmalength=0;
} */

