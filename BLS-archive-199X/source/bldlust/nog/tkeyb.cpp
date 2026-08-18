#include <dos.h>
#include <i86.h>
#include <conio.h>

#include "tkeyb.h"

extern "C" {
void __cdecl loades();
}


extern void (interrupt far *oldkeybintvector)(void);

void (*keyboardhandler)(void)=0;  //pointer to our handler

void interrupt KEYBOARDIRQ()
{
loades();    

_enable();
(*keyboardhandler)();
_disable();

outp(0x20,0x20);
}


void InitKeyboard(void (*kfunc)())
{
 keyboardhandler=kfunc;

_disable();
oldkeybintvector = _dos_getvect(9);
_dos_setvect(9, KEYBOARDIRQ);
_enable();
}




void TerminateKeyboard()
{
_disable();
_dos_setvect(9, oldkeybintvector);
_enable();
}    



