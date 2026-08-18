#include <dos.h>
#include <i86.h>
#include <conio.h>

#include "tkeyb.h"

extern "C" {
void __cdecl SetDPMIVector(void far *vector,int num);
void __cdecl far *GetDPMIVector(int num);
void __cdecl SwitchStack();
void __cdecl SwitchBack();
void __cdecl loades();
}


extern void (interrupt far *oldkeybintvector)(void);

//static unsigned char x;

void (*keyboardhandler)(void)=0;  //pointer to our handler

extern int dooldvect,dodpmi;
void interrupt KEYBOARDIRQ()
{
//SwitchStack();
loades();    
//*((char *)(0xA0000+320*199+160))=2;      
//_enable();
//_disable();
_enable();

//while(inp(0x64)&0x2);
//outp(0x64,0xAD);
   
/*x=inp(0x61);
x|=0x80;
outp(0x61,x); */

(*keyboardhandler)();
//quit=1;

/*x=inp(0x61);
x&=~0x80;
outp(0x61,x);*/


//while(inp(0x64)&0x2);
//outp(0x64,0xAE);

//_enable();
_disable();

//*((char *)(0xA0000+320*199+160))=0;
/*
if (dooldvect)
{
(*oldintvector)();
while (kbhit()) getch();
}
*/
//SwitchBack();
outp(0x20,0x20);

}


void InitKeyboard(void (*kfunc)())
{
 keyboardhandler=kfunc;

_disable();
//oldintvector=(void interrupt (far *)())GetDPMIVector(9);
//SetDPMIVector((void far *)KEYBOARDIRQ,9);
oldkeybintvector = _dos_getvect(9);
_dos_setvect(9, KEYBOARDIRQ);
_enable();
}




void TerminateKeyboard()
{
_disable();
//SetDPMIVector((void far *)oldintvector,9);
_dos_setvect(9, oldkeybintvector);
_enable();
}    



