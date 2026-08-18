#include <stdlib.h>
#include <dos.h>
#include <ctype.h>


#include "keyb.h"



int  (*keyboardhandler)(void)=0;  //pointer to keyboard handler

int keyqueue=1; //whether or not to store keys in the queue

int checkkbstatdown(char k);
int checkkbstatup(char k);

volatile char kbscan=0;
volatile char kbstat=0;
volatile char keydown[128];

void wm_keydown(char k)
{
 //store scan as last key hit
 kbscan=k;

 //store keydown status
 keydown[k]=1;

 if (!checkkbstatdown(kbscan)) //check shift/alt/ctrl
 {
  if (keyboardhandler)  //call handler
   { //0=add key to cache, 1=dont add key to cache
     if (!(*keyboardhandler)() && keyqueue) //if key should be added
        pushkey(k); //push key
   } else if (keyqueue) pushkey(k); //push key
 }
}


void wm_keyup(char k)
{
 //store scan as last key hit
 kbscan=k|0x80;

 //store keydown status
 keydown[k]=0;

 if (!checkkbstatup(k)) //check shift/alt/ctrl
  if (keyboardhandler)  (*keyboardhandler)();
}


int checkkbstatdown(char k)
{
 if (k==42 || k==54) { kbstat|=KB_SHIFT; return 1;}
 if (k==29)  { kbstat|=KB_CTRL; return 1;}
 if (k==56)  { kbstat|=KB_ALT; return 1;}
 return 0;
}

int checkkbstatup(char k)
{
 if (k==42 || k==54) { kbstat&=~KB_SHIFT; return 1;}
 if (k==29)  { kbstat&=~KB_CTRL; return 1;}
 if (k==56)  { kbstat&=~KB_ALT;  return 1;}
 return 0;
}


volatile char kbscanbuf[16];
volatile int  kbhead=0,kbtail=0;

//stores a scan code in the buffer
void pushkey(char kbscan)
{
 kbscanbuf[kbtail]=kbscan; //store at tail
 kbtail++; kbtail&=15;
}


//gets next scan code from buffer
char getkey()
{
if (kbhead==kbtail) return 0;

char kb=kbscanbuf[kbhead];
kbhead++; kbhead&=15;
return kb;
}

int keyhit()
{
return (kbtail!=kbhead);
}

//waits for next scan code from buffer
char waitkey()
{
while (!keyhit());

char kb=kbscanbuf[kbhead];
kbhead++; kbhead&=15;
return kb;
}


void set_keyboard_func(int (*kfunc)())
{
 keyboardhandler=kfunc;
}



char s2a[]=
{
0,27,49,50,51,52,53,54,55,56,57,48,45,61,8,9,113,119,101,114,116,121,117,105,111,
112,91,93,13,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118, //47
98,110,109,44,46,47,0,42,0,32,128,129,130,131,132,133,134,135,136,137,0,0,0,0,0,45, //74
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

char s2ashift[]=
{
0,27,33,64,35,36,37,94,38,42,40,41,95,43,8,9, 81, 87, 69, 82, 84, 89, 85, 73, 79,
 80,123,125,13,0,65, 83, 68, 70, 71, 72, 74, 75, 76,58,34,126,0,124,90, 88,67, 86, //47
66, 78, 77,60,62,63,0,42,0,32,128,129,130,131,132,133,134,135,136,137,0,0,0,0,0,45, //74
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//converts a scancode to ascii character
char scan2ascii(char s)
{
 if (kbstat&KB_SHIFT)  return s2ashift[s];
 return s2a[s];
}
