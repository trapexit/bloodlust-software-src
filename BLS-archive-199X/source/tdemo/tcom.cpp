#include <dos.h>
#include <i86.h>
#include <conio.h>


extern "C" {
void __cdecl SetDPMIVector(void far *vector,int num);
void __cdecl far *GetDPMIVector(int num);
}


void interrupt COMIRQ();

static void (interrupt far *oldintvector)(void);
void (*comhandler)(void)=0;  //pointer to our handler


int comirq,comintvector;  //irq for com handler
int commaskport; 
int comrotateport;
char comstopmask,comstartmask;


int comport; //base ADDR for com read/write
int comintenable;
int comintident;
int comlinecontrol;
int commodemcontrol;
int comlinestatus;
int commodemstatus;




void InitCom(void (*cfunc)(), int port, int irq)
{
comhandler=cfunc;

// PIC1 
comirq=irq;
comintvector  = 0x08 + irq;
comrotateport = 0x20;
commaskport   = 0x21;
comstopmask  = 1 << (comirq % 8);
comstartmask = ~comstopmask;

//com ports
comport=port;
comintenable=port+1;
comintident=port+2;
comlinecontrol=port+3;
commodemcontrol=port+4;
comlinestatus=port+5;
commodemstatus=port+6;




_disable();
outp(commaskport, (inp(commaskport) | comstopmask));
oldintvector = _dos_getvect(comintvector);
_dos_setvect(comintvector, COMIRQ);
_enable();
}



void TerminateCom()
{
_disable();
outp(commaskport, (inp(commaskport) | comstopmask));
_dos_setvect(comintvector, oldintvector);
_enable();
}    



void interrupt COMIRQ()
{
   
if (comhandler) (*comhandler)();


outp(0xA0,0xA0);
outp(0x20,0x20);
}

