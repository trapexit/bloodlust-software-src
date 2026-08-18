#ifndef _GVDP_
#define _GVDP_

typedef word GCOLOR;

//genesis video display processor
class VDP
{
 public:
 byte REG[24];

 byte flip;
 byte idcode; //code
 word addr;  //address
 word ainc;  //address increment

 //base addresses
 word hscrollbase,spritebase;
 word ntAbase,ntBbase,ntWbase;

 byte   *vram; //VDP memory (64k)
 GCOLOR *cram; //color ram

 //dma stuff
 word  dmalength;
 dword dmaaddr;
 byte  dmamode;
 byte  dmadata;
 byte  dmaenable;
 void startdma();

 //bgcolor
 byte bgcolor;
 byte bgpal;

 //resolution in cells
 byte hres,vres;

 byte shadow; //shadow highlight?
 byte interlace; //0-none 1-interlace 3-double res
 byte displayenable;

 //scroll size (in cells)
 int hsize,vsize;

 //interrupt
 byte Hintenable,Vintenable;
 byte Hinttiming;
 byte HVcounterenable;

 //timing flags
 byte vblank,frame;

 void reset()
 {
  memset(REG,0,24);
  flip=0; idcode=0; addr=ainc=0;
  dmalength=0; dmaaddr=0; dmamode=0; dmaenable=0;
  bgcolor=bgpal=0;
  hres=32; vres=28;
  hsize=vsize=32;
  shadow=0; displayenable=0;
  ntAbase=ntBbase=ntWbase=spritebase=hscrollbase=0;
  Hintenable=Vintenable=HVcounterenable=0; Hinttiming=0;
  memset(vram,0,0x10000); //clear vram
 }

 VDP()
  {
   vram=(byte *)malloc(0x10000);
   cram=(GCOLOR *)malloc(128);
   reset();
  }
 ~VDP() {free(vram); free(cram);}

 //writes data 'd' to reg 'n'
 void regwrite(int n,byte d);

 void datawriteb(byte d);  //$C00000/1/2/3
 void datawritew(word d);  //$C00000/2
 void controlwritew(word d); //$C00004/6

 word datareadw(); //$C00000/2
 word statusread(); //$C00004/6

 void startvblank();
 void startframe();

 //read write funcs
 byte  readb(dword a);
 word  readw(dword a);
 dword readd(dword a);

 void writeb(dword a,byte d);
 void writew(dword a,word d);
 void writed(dword a,dword d);
};

#endif