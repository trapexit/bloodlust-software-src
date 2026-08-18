//Copyright(c) 1995 Bloodlust Software All rights reserved
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <graph.h>




volatile int timerbusy=0;

#define HI 0x5F

void Dosound();

extern "C"
{
#include "..\test\detect.h"
}

//#include "glib.h"
#include "input.h"
#include "config.h"

char *hstr[]=
{
  "Defines the input device for player 1.",
  "Defines the input device for player 2.",
  "Allows you to set up the hardware for your sound card.",
  "Exit and do not save the configuration changes.",
  "Save the configuration and exit."
} ;


void DrawConfig();

typedef struct character
{
 unsigned char c;
 unsigned char attr;
} character;

character *video=(character *)0xB8000;

#define SCRX 80
#define SCRY 25


config *cfg; //configuration
inputdevicesettings *idsptr; //input settings

unsigned bytes;
int h;
volatile char kbint;
volatile char kbscan;
volatile unsigned int uu;


character screen[SCRX*SCRY];

extern keymap defkeymap[2];

void DrawLA(unsigned char l,unsigned char a, int x,int y)
{
 screen[y*SCRX+x].c=l;
 screen[y*SCRX+x].attr=a; 
}    


void DrawLetter(unsigned char l, int x,int y)
{
 screen[y*SCRX+x].c=l;
}    

void SetAttribute(unsigned char l, int x,int y)
{
 screen[y*SCRX+x].attr=l;
}

void DrawAHLine(int x, int y,int l,unsigned char a)
{
 for (int i=x; l>0; i++,l--)
  SetAttribute(a,i,y);
}    

void DrawHLine(int x, int y, int l,unsigned char a)
{
 for (int i=x; l>0; i++,l--)
  DrawLA(0xCD,a,i,y);
}    

void DrawVLine(int x, int y, int l,unsigned char a)
{
 for (int i=y; l>0; i++,l--)
  DrawLA(0xD7,a,x,i);
}    


void DrawRect(int x, int y, int xl, int yl,unsigned char a)
{
DrawLA(0xC9,a,x,y); //ul
DrawHLine(x+1,y,xl-1,a);
DrawLA(0xBB,a,x+xl,y); //ur

DrawVLine(x,y+1,yl-1,a);
DrawVLine(x+xl,y+1,yl-1,a);

DrawLA(0xC8,a,x,y+yl); //dl
DrawHLine(x+1,y+yl,xl-1,a);
DrawLA(0xBC,a,x+xl,y+yl); //dr
}


void DrawBlock(int x, int y, int xl, int yl,unsigned char l, unsigned char a)
{
  character *s=&screen[y*SCRX+x];
 for (;yl>0; yl--)
 {
  for (int i=xl; i>0; i--,s++)
    {s->c=l; s->attr=a;}
  s+=SCRX-xl;
 }   
}               


void DrawBox(int x, int y, int xl, int yl,unsigned char a)
{
DrawBlock(x,y,xl,yl,0xFF,a);
DrawRect(x,y,xl,yl,a);
}    

character temp[SCRX*SCRY];

void OpenBox(int x,int y, int xl, int yl,unsigned char a)
{
memcpy(temp,screen,SCRX*SCRY*2);

x+=xl/2; y+=yl/2;
for (int i=0; i<=20; i++)
{
 int txl=xl*i/20;
 int tyl=yl*i/20;
 DrawBox(x-txl/2,y-tyl/2,txl,tyl,a);
 memcpy(video,screen,SCRX*SCRY*2);
 delay(15);
}


}

void DrawString(int x, int y, char *s)
{
while (*s)
{ DrawLetter(*s,x,y); x++; s++;}

}

void DrawCenteredString(int x, int y, char *s)
{
DrawString(x-strlen(s)/2, y,s);
}    


void Flash(int x,int y,int xl, unsigned char a1, unsigned char a2)
{
for (int i=0; i<3; i++)
 {
   delay(80);  
   DrawAHLine(x,y,xl,a1);
   memcpy(video,screen,SCRX*SCRY*sizeof(character));
   delay(80);
   DrawAHLine(x,y,xl,a2);
   memcpy(video,screen,SCRX*SCRY*sizeof(character));
 }  
}


void Doinput(int i);    

int numcddrives;

void cleanexit(int x)
{
_clearscreen(_GCLEARSCREEN);
_settextcursor(0x0607);
exit(x);
}


extern "C" void InitDosError();

void main()
{
int i;
char key;

//InitDosError();
//numcddrives=cdrom_installed(); //check cdrom installation
/*if (numcddrives<=0)
  {printf("ERROR initializing CD-ROM driver.\n");
   exit(1);
  }  
*/


//open timeslaughter config file
cfg=load_config("a32.cfg");
if (!cfg)  //create new config 
 {
  cfg=new_config();
  idsptr=&cfg->ids; //get pointer to ids

  //try and detect sb
  if (detect_settings(&cfg->sbport, &cfg->sbirq, &cfg->sbdma, &cfg->sbdma16))
   {
    cfg->soundcard=CFG_SB;
    if (cfg->sbdma16) cfg->soundcard=CFG_SB16;
   }  else
   { //make guesses
    cfg->sbport=0x220;
    cfg->sbirq=5;
    cfg->sbdma=1;
    cfg->sbdma16=5;
    cfg->soundcard=CFG_NOSOUND; //assume no SB
   }

  cfg->pinput[0]=ID_KEY1;
  cfg->pinput[1]=ID_KEY2;

  //set keymaps to the defaults
  idsptr->km[0]=defkeymap[0];
  idsptr->km[1]=defkeymap[1];
 }

idsptr=&cfg->ids; //get pointer to idsptr

 

_clearscreen(_GCLEARSCREEN);

_settextcursor(0x2000);

for (i=0; i<SCRY*SCRX; i++)
 {screen[i].c=0; screen[i].attr=0;}
for (i=0; i<SCRX; i++)
 {
  DrawLA(  0xE0,0x40,i,13);
  DrawLA(  0xE0,0x40,i,12);
  memcpy(video,screen,SCRX*SCRY*sizeof(character));
  delay(4);
 }

for (i=0; i<12; i++)
 {
  for (int j=0; j<SCRY*SCRX; j++)
   {screen[j].c=0; screen[j].attr=0;}
 
  for (j=0; j<SCRX; j++)
   {
     DrawLA(  0xE0,0x40,j,11-i);
     DrawLA(  0xE0,0x40,j,14+i);
   }
  memcpy(video,screen,SCRX*SCRY*sizeof(character));
  delay(20);
 }      


for (i=0; i<SCRY*SCRX; i++)
 {screen[i].c=0; screen[i].attr=0;}
 
for (i=0; i<SCRX; i++)
 {
  screen[i].c=0xE0;
  screen[i].attr=0x40;
  screen[i+SCRX*24].c=0xE0;
  screen[i+SCRX*24].attr=0x40;
 } 

  
DrawBlock(0,1,80,1,0xFF,0x4F);
DrawString(15,1,"Animator32 Configuration Utility Version 1.00");


character *oldbg=(character *)malloc(SCRX*SCRY*2);
memcpy(oldbg,screen,SCRX*SCRY*sizeof(character));    



OpenBox(20,4,40,9,0x4f);
DrawBox(20,4,40,9,0x4F);
DrawAHLine(0,23,80, HI);
DrawBlock(21,5,39,8,0xFF,0x4F);

DrawCenteredString(40,5,"PICK YOUR PLEASURE");
DrawAHLine(21,5,39, 0x4E);



int done=0;
character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    


int highlight=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

DrawCenteredString(40,7,"Player 1 Input");
DrawCenteredString(40,8,"Player 2 Input");
DrawCenteredString(40,9,"Sound Configuration");
/*DrawCenteredString(40,10,"Show Cinematic Text: ");
DrawString(51,10,cfg->cinword ? "ON" : "OFF");
DrawCenteredString(40,11,"Music Source: ");
DrawString(47,11,cfg->musicsource ? "MIDI" : "CD");*/

DrawCenteredString(40,10,"Exit Without Saving");
DrawCenteredString(40,11,"Save & Exit");
DrawAHLine(21,7+highlight,39,HI);

DrawCenteredString(40,23,hstr[highlight]);
  
memcpy(video,screen,SCRX*SCRY*sizeof(character));

key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<4) highlight++;
  if (key==27) done=1;

 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(21,7+highlight,39,0x4f,HI);
    switch (highlight)
    {
     case 0:
     case 1: Doinput(highlight);
             break;
     case 2: Dosound(); break;        
     case 4:
             save_config("a32.cfg",cfg);
     case 3: done=1;
             break;
    }
   } 
 }         

} while (!done);

_clearscreen(_GCLEARSCREEN);

_settextcursor(0x0607);

printf("Copyright (C) 1996 Bloodlust Software. All rights reserved.\n");
printf("\n");


}




//extern void pause(int i);
extern int joypos[4],joybuttons;
extern char analoginstalled;

//centers joystick i
int DoCenter(int i)
{
char s[100];

ids=*idsptr; //copy over ids


input in; //analog input device
in.init(ID_JOY1+i); //initialize it


//detect it
refreshinput();
if (joypos[i*2]>=1024 && joypos[i*2+1]>=1024) //no joy found
 {
  OpenBox(5,17,70,2,0x4F);
  sprintf(s,"ERROR: Joystick %d not found.",i+1);
  DrawCenteredString(40,18,s);
  memcpy(video,screen,SCRX*SCRY*sizeof(character));
  getch();
  return 0;
 }     


//joystick calibrators
int *x=joypos+i*2;
int *y=joypos+i*2+1; 
int l,r,u,d,cx,cy;

         
OpenBox(5,17,70,2,0x1F);

sprintf(s,"Center joystick %d and press any button.",i+1);
DrawCenteredString(40,18,s);
memcpy(video,screen,SCRX*SCRY*sizeof(character));
do
{
refreshinput();
} while (!kbhit() && !joybuttons);
while (kbhit()) getch();
do {refreshinput();} while (joybuttons);

cx=*x; cy=*y; //get centers....
l=r=cx;  u=d=cy;
l=l*3/4; r=r*5/4;
u=u*3/4; d=d*5/4;


OpenBox(40-10*2,3,10*4+1,12+1,0x0F);
         //21,4 and 39,11
         //center 40,10
         

sprintf(s,"   Swirl joystick %d and press any button when calibrated.   ",i+1);
DrawCenteredString(40,18,s);
memcpy(video,screen,SCRX*SCRY*sizeof(character));

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    



int done=0;
do
{
refreshinput(); //get analog settings    
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

if (*x<l) l=*x; if (*x>r) r=*x;
if (*y<u) u=*y; if (*y>d) d=*y;



int cursorx,cursory;
if (*x<cx) //left of center
        {cursorx=(*x-cx)*20/(cx-l); if (cursorx<-19) cursorx=-19;}
 else  {cursorx=(*x-cx)*20/(r-cx); if (cursorx>19) cursorx=19;}

if (*y<cy) //top of center
       {cursory=(*y-cy)*6/(cy-u); if (cursory<-5) cursory=-5;}
 else  {cursory=(*y-cy)*6/(d-cy); if (cursory>6) cursory=6;}

cursorx+=40; cursory+=9; //center it

screen[cursorx+cursory*SCRX].attr=0x4F; //draw square thing


//set input device thresholds
ids.aj[i].left =(cx-l)/6+l; // 1/4 from l to center
ids.aj[i].right=r-(r-cx)/6; // 1/4 from right to center
ids.aj[i].up  =(cy-u)/6+u; 
ids.aj[i].down=d-(d-cy)/6;

in.reset();
in.read();


if (in.stat&ID_LEFT) DrawString(23,9,"left");
if (in.stat&ID_RIGHT) DrawString(60-7,9,"right");
if (in.stat&ID_UP)    DrawCenteredString(40,5,"up"); 
if (in.stat&ID_DOWN)  DrawCenteredString(40,14,"down");

if (!in.stat) DrawCenteredString(40,9,"center");

//sprintf(s,"%d",in.stat);
//DrawCenteredString(40,9,s);

//sprintf(s,"%d %d",cursorx,cursory);
//DrawCenteredString(40,9,s);

//sprintf(s,"%d %d",*x-cx,*y-cy);
//DrawCenteredString(40,10,s);


memcpy(video,screen,SCRX*SCRY*sizeof(character));

if (kbhit()) done=1;
if (joybuttons) done=1;

} while (!done);
free(bg);

if (kbhit())
 {
  char key=getch();
  if (!key) key=getch();
   else
  if (key==27)  return(0);
 }

while (kbhit()) getch();



idsptr->aj[i].left =(cx-l)/6+l; // 1/4 from l to center
idsptr->aj[i].right=r-(r-cx)/6; // 1/4 from right to center

idsptr->aj[i].up  =(cy-u)/6+u; 
idsptr->aj[i].down=d-(d-cy)/6;


return(1);

}


int DoGrip(int gripnum)
{
 OpenBox(5,4,70,2,0x1F);
 DrawCenteredString(40,5,"Initializing GrIP...");
 memcpy(video,screen,SCRX*SCRY*sizeof(character)); 

 int h;
 unsigned bytes;
 char *gripmem;
 if (_dos_open("grip.vol",O_BINARY | O_RDONLY,&h))
    {
     DrawBox(5,4,70,2,0x1F);
     DrawCenteredString(40,5,"grip.vol not found."); 
     memcpy(video,screen,SCRX*SCRY*sizeof(character));  getch();
     return(0);
    }
  else
  {
   bytes=filelength(h); gripmem=(char *)malloc(bytes);
   _dos_read(h,gripmem,bytes,&bytes);
   _dos_close(h);
  }
 
  if (!GrLink(gripmem, bytes))
    {
     DrawCenteredString(40,5,"ERROR: Unable to link to GrIP library."); 
     memcpy(video,screen,SCRX*SCRY*sizeof(character)); getch();
     return(0);
    }
 free(gripmem);

 if (!GrInitialize()) //initialize grip stuff
  {
   GrUnlink();
   DrawCenteredString(40,5,"Failed To Initialize GrIP."); 
   memcpy(video,screen,SCRX*SCRY*sizeof(character)); getch();
   return(0); //failed
  }
  
 DrawCenteredString(40,5,"GrIP Initialized Successfully");
 memcpy(video,screen,SCRX*SCRY*sizeof(character)); 


OpenBox(18,10,41,7,0x1F);
DrawBox(18,10,41,7,0x1F);

DrawCenteredString(40,11,"Choose GrIP Device");
DrawAHLine(19,11,40, 0x4E);

char device[4][30];
GRIP_BITFIELD slotmap=GrGetSlotMap();

for (GRIP_SLOT i=1; i<=4; i++)
 {
  char s[30];
  if (slotmap&(1<<i)) GrGetProductName(i,s);
      else strcpy(s,"EMPTY");
    
  sprintf(device[i-1],"SLOT %d: %s",i,s);
 
  DrawString(20,12+i,device[i-1]);
 }

memcpy(video,screen,SCRX*SCRY*sizeof(character));
 

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=idsptr->gripslot[gripnum];
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

DrawAHLine(19,13+highlight,40,HI);

memcpy(video,screen,SCRX*SCRY*sizeof(character));

char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<3) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(19,13+highlight,40,HI,0x6f);
    idsptr->gripslot[gripnum]=highlight;
    done=1;
   } 
 }         

} while (!done);

free(bg);


// Shutdown the GrIP system
GrShutdown();

// Disconnect from the GrIP library
GrUnlink();
 
    
return(1); 
}



char *mapnames[]={
     "Up-left", //0
     "Up",
     "Up-right",
     "Left",
     "Right",
     "Down-left", //5
     "Down",
     "Down-Right",
     "Button 1", //8
     "Button 2", //9
     "Button 3", //1-
     "Button 4", //11
     "Button 5", //12
     "Button 6", //13
};

typedef struct keyname {
    char scancode;
    char *name;
} keyname ;

keyname keynames[]=
{
  {0x48, "Up Arrow"},
  {0x50, "Down Arrow"},
  {0x4B, "Left Arrow"},
  {0x4D, "Right Arrow"},
  {0x47, "Home"},
  {0x4F, "End"},
  {0x49, "Page Up"},
  {0x51, "Page Down"},
  {0x52, "Insert"},
  {0x53, "Delete"},
  {0xE0, "\\"},
  {0x37, "*"},
  {0x4A, "-"},
  {0x4E, "+"},

  {0x29, "`"},
  {0x02, "1"},
  {0x03, "2"},
  {0x04, "3"},
  {0x05, "4"},
  {0x06, "5"},
  {0x07, "6"},
  {0x08, "7"},
  {0x09, "8"},
  {0x0A, "9"},
  {0x0B, "0"},
  {0x0C, "-"},
  {0x0D, "-"},

  {0x1a, "["},
  {0x1b, "]"},
  {0x27, ";"},
  {0x28, "'"},
  {0x2b, "\\"},
  {0x33, ","},
  {0x34, "."},
  {0x35, "/"},

  {0x10, "q"},
  {0x11, "w"},
  {0x12, "e"},
  {0x13, "r"},
  {0x14, "t"},
  {0x15, "y"},
  {0x16, "u"},
  {0x17, "i"},
  {0x18, "o"},
  {0x19, "p"},

  {0x1E, "a"},
  {0x1F, "s"},
  {0x20, "d"},
  {0x21, "f"},
  {0x22, "g"},
  {0x23, "h"},
  {0x24, "j"},
  {0x25, "k"},
  {0x26, "l"},

  {0x2c, "z"},
  {0x2d, "x"},
  {0x2e, "c"},
  {0x2f, "v"},
  {0x30, "b"},
  {0x31, "n"},
  {0x32, "m"},

  {0,"<none>"}
};


keymap defkeymap[2]=
{
 {
  0x47,0x48,0x49,
  0x4B,0x4D,
  0x4F,0x50,0x51,

  0x18,0x19,0x1a,
  0x26,0x27,0x28
 },  

 {
  0x13,0x14,0x15,
  0x21,0x23,
  0x2F,0x30,0x31,

  0x10,0x11,0x12,
  0x1E,0x1F,0x20 
 },  
};    

char *getkeyname(char key)
{
for (int i=0; keynames[i].scancode; i++)
 if (keynames[i].scancode==key) break;

return(keynames[i].name);
}    
         
void changekey(keymap *k,int num)
{
char s,oldscan=inp(0x60);
s=oldscan;
do
{
 oldscan=s;
 s=inp(0x60);
}
while (s==oldscan || (s&0x80));

if (s!=1) ((char *)k)[num]=s;
while(kbhit()) getch();

}    
     

void DrawKey(int x, int y,int num, keymap *k)
{
char s[40];

char key=((char *)k)[num];
sprintf(s,"%12s: %s",mapnames[num],getkeyname(key));
DrawString(x,y,s);
}    



void DoKeyMap(int mapnum)
{
//int mapnum=cfg->pinput[j]-3; //0 or 1
keymap k=idsptr->km[mapnum];
    
OpenBox(20,2,40,20,0x1F);
DrawBox(20,2,40,20,0x1F);

DrawCenteredString(40,23,"Press Enter to change the desired key mappings.");


char *s="Keyboard X Key Map"; s[9]=mapnum+'1';
DrawCenteredString(40,3,s);
DrawAHLine(21,3,39, 0x4E);
DrawCenteredString(40,5,"Save Keyboard Settings");
DrawCenteredString(40,6,"Restore Default Settings");

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=0;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

for (int i=0; i<=13; i++) DrawKey(23,8+i,i,&k);

DrawAHLine(21,5+highlight,39,HI);

memcpy(video,screen,SCRX*SCRY*sizeof(character));

char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) {highlight--; if (highlight==2) highlight=1;}
  if (key==80 && highlight<16) {highlight++; if (highlight==2) highlight=3;}
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    if (highlight<2)
    Flash(21,5+highlight,39,HI,0x6f);
    if (highlight==0) //SAVE
      {idsptr->km[mapnum]=k; done=1;}
     else
    if (highlight==1)
     {k=defkeymap[mapnum];}
    else //wanna changeum keyum
    {
     DrawAHLine(21,5+highlight,39,0x1F);
     DrawAHLine(37,5+highlight,3,0xCF);
     DrawString(37,5+highlight,"                  ");
     DrawString(37,5+highlight,"???");
     memcpy(video,screen,SCRX*SCRY*sizeof(character));

     changekey(&k,highlight-3); //change it
     if (highlight<16) highlight++; else highlight=0;
    }     
       

   }
 }

} while (!done);

free(bg);    

}



void Doinput(int j)
{
OpenBox(25,10,30,11,0x1F);
DrawBox(25,10,30,11,0x1F);

char *s="Player X"; s[7]=j+'1';
DrawCenteredString(40,11,s);
DrawAHLine(26,11,29, 0x4E);
DrawCenteredString(40,13,"None");
DrawCenteredString(40,14,"Keyboard 1 ");
DrawCenteredString(40,15,"Keyboard 2");
DrawCenteredString(40,16,"Joystick 1");
DrawCenteredString(40,17,"Joystick 2");
DrawCenteredString(40,18,"Gravis Gamepad");
DrawCenteredString(40,19,"GrIP GamePort");


character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int i2menu[]={0,5,1,2,3,4,6,6};
int highlight=i2menu[cfg->pinput[j]];
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

DrawAHLine(26,13+highlight,29,HI);

memcpy(video,screen,SCRX*SCRY*sizeof(character));

char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<6) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(26,13+highlight,29,HI,0x6f);
    switch(highlight)
    {
     case 0: cfg->pinput[j]=ID_NONE;done=1; break;
     case 1: cfg->pinput[j]=ID_KEY1; DoKeyMap(0); done=1; break;
     case 2: cfg->pinput[j]=ID_KEY2; DoKeyMap(1); done=1; break;
     case 3: if (!DoCenter(0)) break; cfg->pinput[j]=ID_JOY1; done=1; break;
     case 4: if (!DoCenter(1)) break; cfg->pinput[j]=ID_JOY2; done=1; break;
     case 5: if (!DoCenter(0)) break; cfg->pinput[j]=ID_GRAVIS; done=1; break;
     case 6: if (!DoGrip(j)) break; cfg->pinput[j]=ID_GRIP1; done=1; break;
    }
    if (cfg->pinput[j]==cfg->pinput[j^1]) cfg->pinput[j^1]=ID_NONE;
//    done=1;
   } 
 }         

} while (!done);

free(bg);
}    



int Doaddress()
{
OpenBox(3,4,15,7,0x1F);
DrawBox(3,4,15,7,0x1F);

DrawCenteredString(10,5,"IO Port");
DrawAHLine(4,5,14,0x4E);
DrawCenteredString(10,7,"220h");
DrawCenteredString(10,8,"240h");
DrawCenteredString(10,9,"260h");
DrawCenteredString(10,10,"280h");

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=(cfg->sbport-0x220)/0x20;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));
DrawAHLine(4,7+highlight,14,HI);
memcpy(video,screen,SCRX*SCRY*sizeof(character));


char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<3) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(4,7+highlight,14,HI,0x6f);
    cfg->sbport=highlight*0x20+0x220;
    done=1;
    free(bg);
    return(1);
   } 
 }         

} while (!done);
free(bg);
return(0);


}    


int DoIRQ()
{
OpenBox(22,4,15,7,0x1F);

DrawCenteredString(29,5,"IRQ");
DrawAHLine(23,5,14,0x4E);
DrawCenteredString(29,7,"3");
DrawCenteredString(29,8,"5");
DrawCenteredString(29,9,"7");
DrawCenteredString(29,10,"9");

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=(cfg->sbirq-3)/2;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));
DrawAHLine(23,7+highlight,14,HI);
memcpy(video,screen,SCRX*SCRY*sizeof(character));


char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<3) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(23,7+highlight,14,HI,0x6f);
    cfg->sbirq=highlight*2+3;
    done=1;
    free(bg);
    return(1);
   } 
 }         

} while (!done);
free(bg);
return(0);
}    


int DoDMA()
{
OpenBox(40,4,15,7,0x1F);

DrawCenteredString(47,5,"DMA");
DrawAHLine(41,5,14,0x4E);
DrawCenteredString(47,7,"0");
DrawCenteredString(47,8,"1");
DrawCenteredString(47,9,"2");
DrawCenteredString(47,10,"3");

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=cfg->sbdma;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));
DrawAHLine(41,7+highlight,14,HI);
memcpy(video,screen,SCRX*SCRY*sizeof(character));


char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<3) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(41,7+highlight,14,HI,0x6f);
    cfg->sbdma=highlight;
    done=1;
    free(bg);
    return(1);
   } 
 }         

} while (!done);
free(bg);
return(0);
}    


int DoDMA16()
{
OpenBox(60,4,15,6,0x1F);

DrawCenteredString(67,5,"DMA16");
DrawAHLine(61,5,14,0x4E);
DrawCenteredString(67,7,"5");
DrawCenteredString(67,8,"6");
DrawCenteredString(67,9,"7");

character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=cfg->sbdma16;
if (highlight<5) highlight=5;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));
DrawAHLine(61,7-5+highlight,14,HI);
memcpy(video,screen,SCRX*SCRY*sizeof(character));


char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>5) highlight--;
  if (key==80 && highlight<7) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(61,7+highlight-5,14,HI,0x6f);
    cfg->sbdma16=highlight;
    done=1;
    free(bg);
    return(1);
   } 
 }         

} while (!done);
free(bg);
return(0);
}    


void Dosound()
{
OpenBox(10,10,60,8,0x1F);
DrawBox(10,10,60,8,0x1F);

DrawCenteredString(40,11,"Sound Hardware");
DrawAHLine(11,11,59, 0x4E);
DrawCenteredString(40,13,"No sound");
DrawCenteredString(40,14,"8-bit Sound Blaster or compatible");
DrawCenteredString(40,15,"16-bit Sound Blaster or compatible");
DrawCenteredString(40,16,"Sound Blaster AWE32");


character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    

int highlight=cfg->soundcard;
int done=0;
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));

DrawAHLine(11,13+highlight,58,HI);

memcpy(video,screen,SCRX*SCRY*sizeof(character));

char key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<3) highlight++;
 }  else
 {
  if (key==27) done=1;
  if (key==13 || key==' ') //enter
   {
    Flash(11,13+highlight,59,HI,0x6f);
    if (!highlight || Doaddress())
     if (!highlight || DoIRQ())
     if (!highlight || DoDMA())
     if (highlight<2 || DoDMA16())
    {
     cfg->soundcard=highlight;
     if (highlight<2) cfg->sbdma16=0;
     done=1;
    }
     
   } 
 }         

} while (!done);

free(bg);

}    


/*
 for (int x=0; x<=16; x++)
  for (int y=0; y<=16; y++)
    if (x || y)
     {
       
         if (!y)
          {
           if (x<=10) DrawLetter(x+'0'-1,x,y);
              else   DrawLetter(x-10+'A'-1,x,y);
          }
          else 
         if (!x)
          {
           if (y<=10) DrawLetter(y+'0'-1,x,y);
              else   DrawLetter(y-10+'A'-1,x,y);
          }  else DrawLetter(y*16+x,x,y);  
     }
*/         


    
