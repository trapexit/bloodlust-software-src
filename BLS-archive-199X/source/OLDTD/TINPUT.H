extern "C" {
#include "grip.h"
}
void RefreshInput();
void TerminateGrip();

extern "C" {
   void __cdecl ReadAnalogJoyPos(int *,int);
   int  __cdecl ReadAnalogJoyButtons();
 }

typedef struct keymap {

char ul,u, ur;
char l,r;
char dl,d,dr;

char punch[3]; //punch and kick keys
char kick[3];

} keymap;
 


typedef struct input {
public:
int type; //0-none 1-Joy1 2-Joy2 3-Keyboard 4-keyb2 5-gravis gamepad 6-GRiP
unsigned char stat; //Current status of the input device
                //bits (3-0) UDRL flags  5&4 new press of but 1&0
char but;  //Current press status of buttons
char oldbut; //Previous status of buttons
int oldkey;
char cycle; //Time to read next input

//analog joystick center, min max thresholds
point center;            //joy center pos
point min,max;           //ul and dr position thresholds
point joy;               //current position

GRIP_SLOT grslot;

//gravis 4 button double press things
unsigned int butdur[4]; //time when button will be activated
unsigned char triggered; //whether or not been triggered yet

keymap k;

void CenterJoy();
void Trigger(unsigned char i,int pl);  //i==button that was triggered (letting go or time out)
void Init(char t); //Type of input
void ReadPosition(int d);
int  ReadButt(int pl);
void Reset();

} input;

