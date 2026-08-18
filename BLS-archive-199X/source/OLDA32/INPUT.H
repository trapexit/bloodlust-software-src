#ifndef INPUT_H
#define INPUT_H

#include "grip.h"

extern "C" {
    void __cdecl initmouse();
    int  __cdecl readmouse(int *x,int*y);
};

extern "C" {
   void __cdecl ReadAnalogJoyPos(int *,int);
   int  __cdecl ReadAnalogJoyButtons();
 }



//input device types
#define ID_NONE   0
#define ID_GRAVIS 1
#define ID_KEY1   2
#define ID_KEY2   3 
#define ID_JOY1   4
#define ID_JOY2   5
#define ID_GRIP1  6
#define ID_GRIP2  7


//input directions
#define ID_RIGHT 0x1
#define ID_LEFT  0x2
#define ID_UP    0x4
#define ID_DOWN  0x8

#define ID_BUT0  0x10
#define ID_BUT1  0x20
#define ID_BUT2  0x40
#define ID_BUT3  0x80


typedef struct keymap
{
char ul,u,ur;
char l,r;
char dl,d,dr;
char button[6]; //6 button keys

int read(unsigned *kstat);
} keymap;


//defines an input device
class input {
private:
char type;     //ID_XXXX

unsigned keystat;  //status of keys pressed
int l,r,u,d; //threshholds
GRIP_SLOT grslot;

//current status of the device
//   DULR  directions and button triggers.
public:
unsigned oldstat; //old status 
unsigned stat;
unsigned but;     //holding status of all the buttons/keys

//the object bound to this device
class object *o;

void init(char type); //initialize
void deinit();
void read(); //read input device
void reset();     //clear input
unsigned getstat(); //get the stat (0 if null device)

//bind/unbind input device to an object
void bind(object *t);
void unbind();

private:
//specific reading funcs
void analog1();
void  analog2();
void  gravis();
void  grip();
void  key();
void  none();

static void (input::*idfunc[])();
};

void refreshinputmain(); //main refresh input
int  refreshinputkeyboard(); 
void refreshinputtimer(); 

// structure containing input device settings, calibrations for each input device.
typedef struct inputdevicesettings
{

//the keymaps for the keyboard inputdevices
keymap km[2]; 


//analog joystick center, min max thresholds
struct
{
int left,right,up,down; //thresholds
} aj[2];

GRIP_SLOT gripslot[2]; //the grip slots for grip1 and 2

} inputdevicesettings;


void TerminateGrip();
extern inputdevicesettings *ids;

#endif    
