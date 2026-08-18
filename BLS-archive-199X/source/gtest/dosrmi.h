#ifndef _DOSRMI_
#define _DOSRMI_

// simulate real mode int
typedef unsigned DWORD;
typedef unsigned short WORD;

//real mode interrupt structure
struct RMI
{
 DWORD edi;
 DWORD esi;
 DWORD ebp;
 DWORD reserved;
 DWORD ebx,edx,ecx,eax;
 WORD flags;
 WORD es,ds,fs,gs;
 WORD ip,cs;
 WORD sp,ss;

 void clear() {memset(this,0,sizeof(RMI));}

 //constructor
 RMIREGS() {clear();}

 //invokes a real mode interrupt
 void rmint(int intnum)
 {
  REGS regs;
  SREGS sregs;
  memset(&sregs,0,sizeof(sregs));

  regs.x.eax=0x300; //simulate real mode int
  regs.x.ebx=intnum; //interrupt number
  regs.x.ecx=0; //0 stack words
  //point to RMI int structure
  sregs.es=FP_SEG(this);
  regs.x.edi=FP_OFF(this);
  //call DPMI
  int386x(0x31, &regs, &regs, &sregs );
 }

 int dosint(DWORD dosfunc)
 {
  eax=dosfunc;
  rmint(0x21); //call dos interrupt
  return (flags&1); //carry flag set on error
 }
 WORD getdoserr(){return eax&0xFFFF; }

 void multiplexint(DWORD func)
  {eax=func; rmint(0x2F);}

 //handy pointer setting functions
 inline void setdsdx(void *p) {ds=((DWORD)p)>>4; edx=((DWORD)p)&0xF;}
 inline void setdssi(void *p) {ds=((DWORD)p)>>4; esi=((DWORD)p)&0xF;}
 inline void setesbx(void *p) {es=((DWORD)p)>>4; ebx=((DWORD)p)&0xF;}
 inline void setesdi(void *p) {es=((DWORD)p)>>4; edi=((DWORD)p)&0xF;}
};

#endif