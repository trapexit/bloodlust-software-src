//Copyright(c) 1995 Bloodlust Software All rights reserved
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <conio.h>

#define VERSION "3.3"

#define DEMO

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

#include "ll_comm.h"
#include "tpacket.h"

#define asmcli _disable();
#define asmsti _enable();

#define BL 130
#define BH 9

#define TIMETICK 80
#define ED 40
#define INITIALDIZZY 15
#define DIZZYMAX     30
#define DIZZYMIN     -30

#define MAXMARKS 20

#define DISSOLVESPEED 256


extern "C" void ioSetComFunc(void (*c)());

//tkeyb.asm
void Keyboard();
int ReadRawFile(char **m,int h);

void PutWordImage(int num,char *dest,int starttime);

void PlaySysSE(char se,int x);

char *ReadFile(char *);
void SelectScreen();
void VersusScreen(int p1,int c1,int p2,int c2);
void InsultScreen(int w,int p1,int c1,int p2,int c2);
int TitleScreen();
void MapScreen(int level);
void PlayCinema(char *);
void TickUpdate();
short GetSelector(unsigned short segment);
void GetConfig();
void PlayGame();
void CreateBackground();
void DrawTime();
void InitializeEffects();
void InitializeSoundEffects();
void KillEffects();
void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p);
                                                                //depth 0front 1back

void BioScreen(int c);
void CheckExistence();
void FunkyFont(char *f);

void OptionsScreen(int x);


//PLAYER
int sz,z;
playerstat p[2];
extern char *cnlist[]=
 {"MOJUMBO","JINSOKU","SAVAGE","PIERRE","SPICE","VLAD",
  "ASYLUM","CHI","STAINE","PORTAL","LAZARUS","UG","SMEGMA"};
playerstat *pz;
int drawpriority;
int newframe;
int rndcount[2],rndnumber;
int orndcount[2];
int stopdur;           //Tell to stop after being hit
int slowdown;

int timeleft;
int timetickdur;

extern int bioexist[12];  //if a certain character exists
extern int bgexist[12];   //if a certain background exists




//Graphics
char *font,*font2,*font3;
char *screen,*background;
char *statusbar;


//BAckground
bground bg;
extern color pal1[256];
int bgnum;
char *bgnlist[]={"bg1","bg2","bg3","bg4","bg9","bg6","bg7","bg8","bg12","bg5","bg10","bg11","bg12","bg12"};
int bgdust[]={0,1,0,0,0,0,0,0,0,0,2,0,0,0};
extern int fadelevel,oldfadelevel;
int scrollx,scry320,scrolldx;
int oldscrollx=-1;
int scrdone=0;
int gshakedur,gshake,gstemp;
int pbsize[2]={0,0};  //PHYSICAL length of bar
int cbsize[2]={0,0};  //Current length of bar
int nbsize[2]={0,0};  //Needed length of bar (for incremental decreasing when hit) (timer)
effect effects[MAXMARKS*2];

int bgfloorylist[]=
 {160,160,140,140,150,150,155,158,0,160,160,0,0,0,0};
 
#define DEMOFLOORY 160

int doparticularbg=-1;

//Game
int game,cstat,fstate,demo,gamestat,BMAC;
int debug,config;
int region,MAXENERGY,multi,drawstatus=1;
int scrolling=0;
int vertscroll=1;
int airmove=0,supermove=0,randomteleport=0,headprojectile=0,suction=0,speedup=0,shakeup=0,black=0,strobe=0,fastp1=0,fastp2=0,death=0,offscreen=0;;
int stickyblood=0;
int slide=0;
int statbar=1;
int screensave=0;
int battle;
int bloodlevel=1;
int postwordtype;
int  messagedur;
char *message,msg[60];
int msgnum=-1; 

char postwords[20];
char postwordwidth;
int postse;
//xxx 2 off     2on
//||+-1P/2P     timeup
//|+--Win/lose  dblko

int fontdur;
int fonttype;
char *word[3];
int wordpos[3];
char *prewords[5]=
{
 "ROUND   X",
 "FIGHT  !",
 "KILL  !!",
 "SLAUGHTER  !!!",
 "BUTCHER  !!!"
};

//Sound
int sbstat;
int musicstat;

int sepriority=0;

SOUND sysse[20];
SOUND sse[2][7];
//extern volatile int playing;

//System
volatile char kbscan;
volatile int quit=0,pause=0,kbstat=0;
volatile int kbint=0;
volatile unsigned int uu,su,fu;
//short dataseg;
//short SEGVIDEO;

//Communication stuff
COMM port;
int complay=0;
int cominstalled=0;
int commaster=0;

//Misc
unsigned bytes;
int h;
cfg *options;
int i,j,k;

int m1,m2;
int shit=0;
int framecheck=0;

int takescreenshot=0;
int lastscreenshot=0;



void debugger();

extern unsigned long clockcount;


int frenchy=0;

extern "C" volatile long intcount;

int random(int x)
{
 return((x*rand())/32768);
}

void interrupt FadeTick();

//configuration variables
int dodpmi=0,dooldvect=0,nodotimer=0,nodokeyb=0, docomm=0;
int dodiagnose=0;
int doawe32=0;
int doquick=0;



void wait()
{
 if (!dodiagnose) return;
 do
 {
  printf("press any key... / \r");
  if (kbhit()) break;
  printf("press any key... - \r");
  if (kbhit()) break;
  printf("press any key... \\ \r");
  if (kbhit()) break;
 } while (!kbhit());
 while (kbhit()) getch();
}    

void waitk()
{
    if (!dodiagnose) return;
    kbscan=0;
 do
 {
  printf("press any key... / \r");
  if (kbscan) return;
  printf("press any key... - \r");
  if (kbscan) return;
  printf("press any key... \\ \r");
  if (kbscan) return;
 } while (!kbscan); 
}    



void blatick()
{
uu++;
}


short int pdata=0;
int pdatastat=0;
void pfunc()
{
 pdatastat=1;
 pdata=((packet *)prbuf)->data;
}    


int COMPORT;

//standard com tick
int comticks=0;
void DefaultComTick() {}    

void main(int argc, char *argv[])
{
if (!stricmp(argv[1],"testtimer")) //test
 {
  printf("testing...\n");
  printf("-initializing timer...\n");
  InitializeTimers();
  SetTimerSpeed(100);
  printf("done\n");
  uu=0; timerhandler=blatick;

  do
  {
   printf("%d\r",uu);
  } while (uu<1000);
  printf("\n");
  timerhandler=0; 
  TerminateTimers();
  printf("done timer\n");
  return;
 }     

if (!stricmp(argv[1],"testkey")) //test
 {
  printf("testing...\n");
  printf("-initializing keyboard...\n");
  InitKeyboard((void ( *)())Keyboard);
  printf("done\n");

  quit=0;
  do
  {
   printf("%d\r",kbscan);
  } while (!quit);
  printf("\n");
  TerminateKeyboard();
  printf("done keyboard\n");
  return;
 }     


if (!stricmp(argv[1],"testtimerkey")) //test
 {
  printf("testing...\n");
  printf("-initializing timer &keyb...\n");
  InitializeTimers();
  SetTimerSpeed(100);
  printf("done\n");
  uu=0; timerhandler=blatick;
  printf("-initializing keyboard...\n");
  InitKeyboard((void ( *)())Keyboard);
  printf("done\n");

  do
  {
   printf("%d %d\r",uu,kbscan);
  } while (uu<1000);
  printf("\n");
  timerhandler=0; 
  TerminateTimers();
  printf("done timer\n");
  TerminateKeyboard();
  printf("done keyboard\n");
  return;
 }     

    
    
printf("TimeSlaughter Beta v%s\n\n",VERSION);
printf("Welcome to the Timeslaughter beta....chances are it'll cause discombobulation.\n");
printf("...but try it anyway. Type 'config' on the command line to change the \n");
printf("configuration of the game. Have fun....\n");
printf("                  - Bloodlust 1995\n");
//getch();


//********************************************************************
printf("M.inistilt I.nteractive D.ynamic G.ame E.ngine T.ardprogram\n");
printf("      Copyright 1995 Bloodlust Software \n\n");


int nodomusic=0, nodosound=0;

int allbg,allchar;
#ifdef DEMO
allbg=0; allchar=0;
#else
allbg=1; allchar=1;
#endif
stickyblood=1;
for (i=1; i<argc; i++)
 {
  if (!stricmp(argv[i],"midgetpower"))
    {printf("-debug codes enabled...\n"); debug=1; }
  if (!stricmp(argv[i],"config"))
    {printf("-configuring...\n");  config=1; }
  if (!stricmp(argv[i],"tarddrool"))    frenchy=1;
  if (!stricmp(argv[i],"nomusic"))
    { printf("-music disabled\n"); nodomusic=1;}
  if (!stricmp(argv[i],"nosound"))
   {printf("-digital audio disabled...\n");  nodosound=1;}
  if (!stricmp(argv[i],"awe32"))    doawe32=1;
  if (!stricmp(argv[i],"diagnose"))    dodiagnose=1;
  
//  if (!stricmp(argv[i],"dpmi"))
//    {printf("-using dpmi setvect...\n"); dodpmi=1;}
//  if (!stricmp(argv[i],"oldvect"))
//   {printf("-trying old keyb vector...\n"); dooldvect=1;}
//  if (!stricmp(argv[i],"notimer")) nodotimer=1;
//  if (!stricmp(argv[i],"nokeyb")) nodokeyb=1;
  
  if (!stricmp(argv[i],"allbg")) allbg=1;
  if (!stricmp(argv[i],"allchar")) allchar=1;
  if (!stricmp(argv[i],"com"))
    {
     docomm=1;
     sscanf(argv[i+1],"%d",&COMPORT);
     printf("-COM play enabled PORT %d\n",COMPORT);
    } 
  if (!stricmp(argv[i],"quick")) doquick=1;

  if (!stricmp(argv[i],"nosticky")) stickyblood=0;

 }
GetConfig();
//_clearscreen();
//getch();


srand(*((unsigned int *)(0x6c+0x400)) );



complay=cominstalled=0;
if (docomm)
 {
   int comaddr,comirq;
   printf("testing comm...\n");
   switch(COMPORT)
    {
     case 1:
        comaddr=0x3F8; comirq=4;
        break;
     case 2:
        comaddr=0x2F8; comirq=3;
        break;
     case 3:
        comaddr=0x3E8; comirq=4;
        break;
     case 4:
        comaddr=0x2E8; comirq=3;
        break;
     default: break;   
    }    

  printf("ADDRESS %X IRQ %d\n",comaddr,comirq);
  
  cprintf("initializing port...");
  ioSetComFunc(DefaultComTick);
  port=ioOpenPort(comaddr,comirq);
  if (!port)
       {
            printf("error initalizing port.\n");
            return ;
       }
  printf("done\n");
  ioSetBaud(port, 57600);
  ioSetHandShake(port, DTR | RTS);
  ioSetControl(port, (BITS_8 | STOP_1 | NO_PARITY));
  ioSetMode(port, BYTE_MODE);

  cominstalled=1;

  int quit=0,key=0;   
  do
  {
//     if (kbhit()) key=getch();
   if (kbhit()) {key=getch(); ioWriteByte(port,key);}
   if (ioReadStatus(port)) cprintf("%c",ioReadByte(port));
//   printf("comticks: %d\n",comticks);
  } while (key!='q');
  printf("\n");

  //Initializing packet
  printf("-initializing packets....\n");
  InitPacket();
/*  SetPacketFunc(0,pfunc);

  key=0;
  do
  {
//   printf("sentpackets: %2d recvpackets:%2d errors:%2d\n",packetsends,packetrecvs,packeterror);
   if (kbhit()) {key=getch(); SendStandardPacket(0,(unsigned short)key); };
   if (pdatastat) {pdatastat=0; cprintf("%c",pdata);}
  } while (key!='q');

  TerminatePacket();

  ioClosePort(port);
  return;*/
 }     




//Initialize sb stuff
sbstat=0;

if (!nodosound)
if (options->sbon)
 {
  cprintf("-initializing sb voice....");
  sbstat=2;
  if (!init_sb(options->sbport, options->sbirq, options->sbdma, options->sbdma16))
         {
                cprintf("ERROR:  Error initializing sound card!\n");
                cprintf("Check your soundblaster settings\n");
                return;
         }
//      printf("initmixing\n");
  init_mixing();
  printf("done\n");

// do
// {
//  printf("DMAcount %5d\r",intcount);
// } while (!kbhit()); 

 }

wait();


if (!nodotimer)
{
//Initialize Timer
cprintf("-initializing timer...");
InitializeTimers();
SetTimerSpeed(100);
printf("done\n");
wait();
}


//initialize sb music
musicstat=0;

if (options->sbon && !nodomusic)
 {
  cprintf("-initializing sb music....");
  if (options->sbon==3) doawe32=1;
  musicstat=2;
  if (init_music(ON)) musicstat=0;
   else
   {
    for (i=0; i < 16; i++) set_channelmask(i, 1); //          printf("channelmask");
    set_multiplier(1); //printf("multiplier");
    printf("done\n");
    wait();
   } 
}
 


/*
if (options->sbon)
 {
  if (sixteenbit)
        {
         set_sound_volume(255);
         set_music_volume(200);
        }
         else
        {
         set_sound_volume(200);
         set_music_volume(200);
        }
 } */

set_sound_volume(255);
set_music_volume(255);

 
//getch();
//randomize();


cprintf("-initializing input devices...");
//Set up `      input devices
p[0].in.Init(options->pinput[0]);
p[1].in.Init(options->pinput[1]);
p[0].ctype=1;        //Joystick/keyboard input device
p[1].ctype=3;        //Computer input device
printf("done\n");

wait();


if (!nodokeyb)
{
cprintf("-initializing keyboard...");
InitKeyboard((void ( *)())Keyboard);
printf("done\n");
}

//waitk();
//kbscan=0; while (!kbscan);
//goto fuckthis;

cprintf("-allocating screen buffers...");
background=(char *)malloc(64000);
screen=(char *)malloc(70000);
screen+=2000;
printf("done\n");
//wait();


cprintf("-initializing fonts...");
font=ReadFile("font.vol");
font2=ReadFile("font2.vol");
font3=ReadFile("font.vol");
FunkyFont(font3);
printf("done\n");

//kbscan=0; while (!kbscan);

//waitk();

//getch();

cprintf("-initializing effects...");
InitializeEffects();
printf("done\n");

//printf("-checking for vols...");
CheckExistence();
//printf("done\n");

//check existence of vols
if (!allchar)
{
 for (i=0; i<12; i++)
 {
  if (i!=3 && i!=6) bioexist[i]=0;
 }
}
BMAC=0;
for (i=0; i<12; i++)
 if (bioexist[i]) BMAC|=1<<i;
doparticularbg=-1; 



///quit=0; while (!quit); quit=0;
if (!doquick) { kbscan=0; while (!kbscan); }
//---------------------------------------------
//printf("gonna go to mode256\n");
Mode256();

/*do
{
do {i=random(12); } while (!bioexist[i]);
BioScreen(i);
} while (1);
//goto fuckthis;
*/
if (!doquick && !cominstalled)
{

PlayCinema("logo.vol");
//printf("gonna play the prod\n");


PlayCinema("prod2.vol");
if (quit) goto doneprod;
PlayCinema("prod.vol");
if (quit) goto doneprod;
PlayCinema("prod3.vol");
doneprod:

PlayCinema("intro.vol");

#ifndef DEMO
//PlayCinema("cred.vol");

#endif
}


//WHOLE GAME LOOP
do
{

if (!doquick)
{
do
{
i=TitleScreen();

if (i==3) {OptionsScreen(0);}
} while (i==3);
} else if (kbscan!=1) i=0; else i=2;

demo=0; game=0; battle=1;
if (i==1) { demo=1; p[0].ctype=p[1].ctype=3; cstat=fstate=0; complay=0;} //Play demo
if (i==2) goto fuckthis;  //quit

    



//ACTUAL TOURNAMENT LOOP---------------------
do
{

quit=0;

//Select guys
 if (game!=2)
  {
        for (i=0; i<2; i++)
         if (p[i].ctype==3)
          {
                if ( (cstat&BMAC)==BMAC)  //if we have defeated all base characters
                  {
                        #ifndef DEMO
                         if ( (fstate&BMAC)==BMAC) {options->pnum[i]=8; fstate=0;} //if fatalatied them all, goto staine
                                else           {options->pnum[i]=9; fstate=0;}  //else goto portal
                        #else
                          cstat=0;
                        #endif
                  }
                 else
                do
                {
                 do
                 {
                 options->pnum[i]=random(11);
                 if (options->pnum[i]==9) options->pnum[i]=11;  //9 is portal
                 } while (!bioexist[options->pnum[i]]);
                } while ( (cstat&(1<<options->pnum[i])) );


                options->color[i]=0;
                if (options->pnum[i]==options->pnum[i^1]) options->color[i]=options->color[i^1]^1;
          }

  }


//Selection screen
if (game!=1 && !demo) SelectScreen();

//if (demo) BioScreen(options->pnum[0]);

if (quit) break;


if (!demo) VersusScreen(options->pnum[0],options->color[0],options->pnum[1],options->color[1]);


//Read Background
char bgname[15];  //char name of the background file
if (allbg)      //pick one of all the backgrounds
{
 int ms;
   //determine what the BG should be...
 if (p[0].ctype==p[1].ctype)  ms=options->pnum[1];//ms=random(12);
          else
 if (p[0].ctype==1) ms=options->pnum[1];
              else  ms=options->pnum[0];

if (doparticularbg!=-1) ms=doparticularbg;              
while (!bgexist[ms]) ms=random(12); //find one that works
 strcpy(bgname,bgnlist[ms]);
 strcat(bgname,".vol");
 bgnum=ms;

 bg.ReadBackground(bgname,bgfloorylist[bgnum]);
 scrolldx=0;
 doparticularbg=-1;  
}
else
{
 bg.ReadBackground("bg.vol",DEMOFLOORY);
 scrolldx=0;
 bgnum=1;       //snow
}





//ModeText();
//printf("reading background %s...",bgname);
//ReadBackground
//printf("done.\n");


//ModeText();
//printf("Read background");
//kbscan=0; while (!kbscan);


//Setup up status bar & screen
InitStatusBar();

//ModeText();

if (sbstat==2)
{
//printf("reading se...");

InitializeSoundEffects();
//printf("done.\n");
}

//printf("Read Sound");
//kbscan=0; while (!kbscan);


//Read Characters
//printf("reading player %s...",cnlist[options->pnum[0]]);
p[0].Initialize(options->pnum[0],options->color[0]);
//printf("done.\n");

//printf("reading player %s...",cnlist[options->pnum[1]]);
p[1].Initialize(options->pnum[1],options->color[1]);
//printf("done.\n");
//kbscan=0; while (!kbscan);
//Mode256();
p[0].po=&p[1]; p[1].po=&p[0];




PlayGame();
//debugger();


if (sbstat==2)
 StopVoice();

 battle++;
//ModeText();


//rbaseptr=oldrb;
//printf("p1kill\n");
p[1].Kill();
//printf("p2kill\n");
p[0].Kill();
free(statusbar);
//printf("killbg\n");

bg.KillBackground();
//kbscan=0; while (!kbscan);
//Mode256();

if (sbstat==2)
{
 for (i=0; i<2; i++)
 for (j=0; j<7; j++)
  if(sse[i][j].soundptr) {free(sse[i][j].soundptr); sse[i][j].soundptr=0;}

 for (i=0; i<=15; i++)
  if(sysse[i].soundptr) {free(sysse[i].soundptr); sysse[i].soundptr=0;}
}

//DO INSULT SHIT----------------------------------
if (!demo && quit!=1)
{
for (i=0; i<2; i++)
 if (rndcount[i]==2) //if this player has gotten two wins
  {
       //check if versus mode
   if (p[0].ctype!=3 && p[1].ctype!=3) {game=0; cstat=0;fstate=0;} 
       else
       //check if player won
   if (p[i].ctype==1)  {game=1; cstat|=1<<p[i^1].type; }
       else
       //player lost
       {game=2; fstate=0;}
       
   InsultScreen(i,p[0].type,p[0].color,p[1].type,p[1].color);
   break;         
  }   
 
}    

/*if (rndcount[0]==2 || rndcount[1]==2) //if enough rounds have been played to win
{
 if (p[0].ctype==1 && p[1].ctype==1) {game=0; cstat=0;fstate=0;
  if (rndcount[0]==2) InsultScreen(0,p[0].type,p[0].color,p[1].type,p[1].color); else InsultScreen(1,p[0].type,p[0].color,p[1].type,p[1].color);
 } //versus
        else
 if ((rndcount[0]==2 && p[0].ctype==1) ) {game=1; cstat|=1<<p[1].type;InsultScreen(0,p[0].type,p[0].color,p[1].type,p[1].color);}
         else
 if ((rndcount[1]==2 && p[1].ctype==1) ) {game=1; cstat|=1<<p[0].type; InsultScreen(1,p[0].type,p[0].color,p[1].type,p[1].color);} //Won, goto next guy
        else { game=2;fstate=0; InsultScreen(1,p[0].type,p[0].color,p[1].type,p[1].color);}//Lost
}
*/

//Play ending if necessary
if ((cstat==BMAC) )
 {
  PlayCinema("end.vol");
  cstat=0; game=0;
  quit=3;
 }



if (demo)
 {
// PlayCinema("cred.vol");
 if (quit==1) {BioScreen(options->pnum[1]); quit=1;}
 PlayCinema("intro.vol");
 }


if (quit==1) {cstat=0; game=0; demo=0; quit=3;}
rndcount[0]=rndcount[1]=0; rndnumber=0;


} while (quit!=3);   //--tournament loop


quit=0;
} while (!quit);      //DONE GAME LOOP


//done:

fuckthis:



ModeText();

printf("Copyright (C) 1995 Bloodlust Software All rights Reserved\n");

printf("-terminating...\n");

cprintf("-terminating effects...");
KillEffects();
printf("done\n");

cprintf("-terminating keyboard...");
TerminateKeyboard();
printf("done\n");


if (musicstat==2)
 {
  cprintf("-terminating music...");
  done_music();
  printf("done\n");  
 }

cprintf("-terminating timers...");
TerminateTimers();
printf("done\n");


if (sbstat==2)
{
  cprintf("-terminating sb voice...");
  shutdown_mixing();
  shutdown_sb();
  printf("done\n");
}


if (cominstalled)
   ioClosePort(port);


//while (kbhit()) getch();

}

void DrawName();

extern "C"
{
void _PutImageFlipClip256(char *,char *,unsigned int,int,int,int,int,int);
}

/*
//36
void debugger()
{


//LoadPalette(
for(i=0; i<64000; i++) screen[i]=1;
MemoryCopy((char *)0xA0000,screen,64000);

quit=0; kbscan=0;

int x,y,s,im=0;
s=0;
x=160; y=140;
do
{
for(i=0; i<64000; i++) screen[i]=1;
 DrawImage(p[0].imagelist[im],screen,x,y,0);
MemoryCopy((char *)0xA0000,screen,64000);

if (kbscan)
{
 if (kbscan==32 ) x-=15;
 if (kbscan==33 ) x+=15;

 if (kbscan==34 && im>0) im--;
 if (kbscan==35 ) im++;

 if (kbscan==36) x--;
 if (kbscan==37) x++;
 if (kbscan==38) s-=5;
 if (kbscan==39) s+=5;
 kbscan=0;
}

char s[30];
sprintf(s,"IMAGE %d",im);
DrawString(font2,screen+150*320,(unsigned char *)s,320);

} while (!quit);



}

*/
void PutImageFunky(char *s,char *d,int x,int y);
void DrawSoundCheck();
void ComTick();

int comconnected;
int ready;



//STUFF FOR NETWORK/MODEM
#define TSCONNECT 0x0
#define TSCONFIRM 0x1
#define TSJOY     0x2
#define TSQUIT    0xF
void SendPos();

void tsconnect()
{
commaster=((packet *)prbuf)->data^1; //we are opposite of type sent     
if (!comconnected)
 {
   SendStandardPacket(TSCONNECT,commaster);       //tell them our status
 }
comconnected=1; ready=1; 
} //we are connected!

void tsjoy()
  {
     unsigned short int x=((packet *)prbuf)->data;
        uu++;    
        p[1].kbmove=x>>4;  //kbmove
        x&=0xf;
        if (p[1].kbmove) //if button pushed
         if (p[1].kbmove<=3) x|=16; else x|=32;
        p[1].in.stat=x; 

        if (!commaster) SendPos();

        //Move projectiles
        pz=&p[1];  z=0;
        for (i=0; i<PMAX; i++) p[0].ps[i].AdvanceProjectile();
        pz=&p[0];  z=1;
        for (i=0; i<PMAX; i++) p[1].ps[i].AdvanceProjectile();


        //Move players
        pz=&p[1]; z=0; p[0].Update();
        pz=&p[0]; z=1; p[1].Update();
      
  } //joystick movement

void tsquit()
{
 quit=1;
}   //the other user quit

void tsconfirm() {}  





void PlayGame()
{
Solid((char *)0xA0000,64000,0);

//Set up variables
for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;
scrollx=160*10/8;  scrdone=0;
rndcount[0]=rndcount[1]=0; rndnumber=0;
if (timeleft!=100) timeleft=99; timetickdur=0;
p[0].x= 160-70; p[1].x=160+70;   drawpriority=0;
  p[0].in.Reset(); p[1].in.Reset();
if (p[0].d!=0) {p[0].d=0;if (p[0].in.stat&3) p[0].in.stat^=3; }
if (p[1].d!=1) {p[1].d=1;if (p[1].in.stat&3) p[1].in.stat^=3; }
drawpriority=0; newframe=0;
stopdur=slowdown=0; p[0].kbmove=p[1].kbmove=0;
messagedur=0; msgnum=-1;
sepriority=0;
rndnumber=1;
oldscrollx=-1;

oldfadelevel=-1; fadelevel=0; gamestat=-4;
pause=quit=0;



ready=1;                         //flag to tell whether or not to proceed in game
commaster=1;
if (complay)      //flag that tells whether or not we are playing multiplayer
 {
  comconnected=0; ready=0;       //flag to tell
  commaster=1;

  SetPacketFunc(TSCONNECT,tsconnect);
  SetPacketFunc(TSJOY,tsjoy);
  SetPacketFunc(TSCONFIRM,tsconfirm);
  SetPacketFunc(TSQUIT,tsquit);
 } 
  

//Start playing game
timerhandler=TickUpdate;
su=1; uu=1;



do
{


 for (i=0; i<2; i++)
 if (p[i].ctype==1)
  p[i].in.ReadPosition(p[i].d);

 CreateBackground();
// series *bp;
 projectilestat *pp;

 if (!black) bg.DrawBGImages(screen);

 for (effect *eptr=&effects[MAXMARKS]; eptr<&effects[MAXMARKS*2]; eptr++) eptr->Draw();


 if (!black && ready)
  {
   p[0].DrawShadow();
   p[1].DrawShadow();
  } 


 //Draw Images
if (ready)
if (uu<DISSOLVESPEED && !demo)
{
 if (p[0].ctype!=3) p[0].DrawFrameFunky(uu);
         else p[0].DrawFrame();
if (p[1].ctype!=3)  p[1].DrawFrameFunky(uu);
    else p[1].DrawFrame();

}
  else
 if (drawpriority)
 {
  p[0].DrawFrame();  p[0].DrawAttachEffect();
  p[1].DrawFrame();  p[1].DrawAttachEffect();
 }
  else
 {
  p[1].DrawFrame();  p[1].DrawAttachEffect();
  p[0].DrawFrame();  p[0].DrawAttachEffect();
 }

sz=0; for (pp=&p[0].ps[0]; pp<&p[0].ps[PMAX]; pp++) pp->DrawProjectile();
sz=1; for (pp=&p[1].ps[0]; pp<&p[1].ps[PMAX]; pp++) pp->DrawProjectile();

 for (eptr=&effects[0]; eptr<&effects[MAXMARKS]; eptr++) eptr->Draw();


 //Draw foreground
 if (!black)
  bg.DrawFGImages(screen);

 if (pause) DrawString(font2,screen+90*320+85,(unsigned char *)"GAME PAUSED",320);

 UpdateStatusBar();

if (!demo && drawstatus)
{
 PutStatusBar(statusbar,screen+15*320+scry320);
 DrawName();
 DrawTime();
 }

//PutImageFunky(p[0].imagelist[1],screen,10,50);

if (takescreenshot)
 {
  takescreenshot=0;
  char *sname="shot000.raw";
  i=lastscreenshot++;
  sname[6]='0'+i%10; i/=10;
  if (i) {sname[5]='0'+i%10; i/=10;}
  if (i) {sname[4]='0'+i%10; i/=10;}
  _dos_creat(sname,0,&h);
  _dos_write(h,pal1,sizeof(color)*256,&bytes);
  _dos_write(h,screen,64000,&bytes);
  _dos_close(h);
 }

//Draw words
if (demo)
 {
  if (uu>1500) quit=1;
  if (uu&32) DrawString(font2,screen+90*320+160-GetStringWidth(font2,(unsigned char *)"DEMO")/2,(unsigned char *)"DEMO",320);
 }


//do com connection
if (!ready)
 {
  DrawString(font2,screen+5+80*320,(unsigned char *)"WAITING FOR OPPONENT",320);
  SendStandardPacket(TSCONNECT,commaster);       //we wanna be master
 }      

//char cmstr[10]; 
//sprintf(cmstr,"%d",p[0].cm);
//DrawString(font2,screen+55*320+15+3,cmstr,320);
//sprintf(cmstr,"%d",p[1].cm);
//DrawString(font2,screen+55*320+320-16-GetStringWidth(font2,cmstr),cmstr,320);

if (commaster) DrawString(font2,screen+55*320+100,(unsigned char *)"MASTER",320);
   else DrawString(font2,screen+55*320+100,(unsigned char *)"SLAVE",320);

char s[30];
if (comconnected) DrawString(font,screen+105*320+0,(unsigned char *)"CONNECTED",320);
if (ready) DrawString(font,screen+125*320+0,(unsigned char *)"READY",320);   

sprintf(s,"TICKS%5d",uu);
DrawString(font,screen+170*320+200,(unsigned char *)s,320);

   

if (msgnum!=-1)
 {
  sprintf(msg,"%d",msgnum);
  messagedur=300;
  message=msg;
  msgnum=-1;
 }

if (messagedur)
  {
        DrawString(font2,screen+160*320+160-GetStringWidth(font2,(unsigned char *)message)/2,(unsigned char *)message,320);
        messagedur--;
  }
//if (soundcheck)
// DrawSoundCheck();

if (framecheck)
 {
//  char s[30];
  sprintf(s,"UU %5d",uu);
  DrawString(font,screen+100*320+10,(unsigned char *)s,320);
  sprintf(s,"SU %5d",su);
  DrawString(font,screen+120*320+10,(unsigned char *)s,320);
  sprintf(s,"FPS %5d",su*100/uu);
  DrawString(font,screen+140*320+10,(unsigned char *)s,320);
  sprintf(s,"MR %5d",mspeed);
  DrawString(font,screen+160*320+10,(unsigned char *)s,320);
 }


if (gamestat)
{
if (gamestat==-4 && !quit) gamestat++;

if (gamestat==-3)
 {
  int oldft=fonttype; while (fonttype==oldft) fonttype=random(4);
  (prewords[0])[8]=rndnumber+'0';
  CreateFontImage256(font2,&word[0],(unsigned char *)prewords[0]);
  wordpos[0]=160-((int) (unsigned char)(word[0][1]))/2;
  CreateFontImage256(font2,&word[1],(unsigned char *)prewords[fonttype+1]);
  wordpos[1]=160-((int) (unsigned char)(word[1][1]))/2;

  char b[20];
  sprintf(b,"BATTLE  %d",battle);
  CreateFontImage256(font,&word[2],(unsigned char *)b);
  wordpos[2]=160-((int) (unsigned char)(word[2][1]))/2;
  gamestat++;

  if (bg.bgfloor)
    {
     MemoryCopy(bg.bgfloor,bg.bgfloorsave,bg.bgfloorsize);
    }    

  if (musicstat) start_music(bg.music,1);
 }

if (gamestat==-1)
 {

        PutWordImage(0,screen+wordpos[0]+60*320,30);
        PutWordImage(1,screen+wordpos[1]+84*320,150);
        i=fontdur;
        if (i>80 && i<215)
        {
         if (i<110) i=110-i+2; else
         if (i>185) i=i-185+2; else i=2;
         PutImageD2(word[2],screen+wordpos[2]+108*320,320,i<<8);
        }
 }

if (gamestat==1)
 {
         _disable();
          postse=0;
          postwords[0]=0;
          if (!(postwordtype&4)) //1p/2p wins/loses
                {
                 strcpy(postwords,cnlist[p[postwordtype&1].type]);
                 if (postwordtype&2) {strcat(postwords," LOSES"); postse=13;}
                         else   {strcat(postwords," WINS"); postse=12;}
                } else
                {
                 if (postwordtype&1) {strcpy(postwords,"TIME UP"); postse=15;}
                 if (postwordtype&2) {strcpy(postwords,"DOUBLE K.O."); postse=14;}
                }

          postwordwidth=160-(GetStringWidth(font2,(unsigned char *)postwords))/2;
          gamestat++;
         _enable();
 }

if (gamestat==2)
 {
        DrawString(font2,screen+postwordwidth+60*320,(unsigned char *)postwords,320);
 }

}

if ((p[0].fatality==1 || p[0].fatality==2))
 if ( (uu&64) &&  !p[0].throwstat && !pause)
         DrawString(font2,screen+72*320+scry320+130,(unsigned char *)"KILL!",320);

/*
if (p[1].stat&1)
{
 char s[30];
 sprintf(s,"S %d DX%d DY%d",p[1].strategy,abs(p[1].x-p[0].x),abs(p[1].y-p[0].y));
 DrawString(font,screen+60*320,(unsigned char *)s,320);
} */

// char s[30];
// sprintf(s,"BGX%d",bg.scrx);
// DrawString(font,screen+60*320,(unsigned char *)s,320);


 MemoryCopy((char *)(0xA0000+14*320),screen+10*320+scry320,320*175);
// MemoryCopy((char *)(0xA0000),screen+0,320*200);
 UpdatePalette();

 su++;

} while (!quit);

if (complay)
{
  comconnected=0; ready=0;       //flag to tell
  commaster=1;

  SetPacketFunc(TSCONNECT,tsconnect);
  SetPacketFunc(TSJOY,tsjoy);
  SetPacketFunc(TSCONFIRM,tsconfirm);
  SetPacketFunc(TSQUIT,tsquit);
} 


UpdatePalette();
timerhandler=0;

if (musicstat)
 {
  stop_music();
 }


}


void PutWordImage(int num,char *dest,int starttime)
{
if (starttime>fontdur) return;


if (fontdur-starttime<35)
  PutImageD2(word[num],dest,320,(35-fontdur+starttime)*256);
         else
if (fontdur>245)
  PutImageD2(word[num],dest,320,(fontdur-245)*256);
         else
DrawImage(word[num],dest,0,0,0);

}


#define CONTROL 1
// In air y<basey
#define AIR 2
// 1-In stance 0-In crouch
#define STANCE 4



void SendPos()
{
unsigned short int x=p[0].in.stat&0xF; //keep position
if (p[0].in.stat&(16+32)) //if button
  x|=p[0].kbmove<<4;
SendStandardPacket(TSJOY,x);
}

/*
void ComTick()
{
if (!ioReadStatus(port)) return;
unsigned int x=(unsigned char)ioReadByte(port);
if (x&0x80) //system
 {
   if (x==0xFF) {quit=1;ioSetComFunc(DefaultComTick);  }
 }
else //joystick position
{
uu++;    
p[1].kbmove=x>>4;  //kbmove
x&=0xf;
if (p[1].kbmove) //if button pushed
 if (p[1].kbmove<=3) x|=16; else x|=32;
p[1].in.stat=x; 

if (!commaster) SendPos();

//Move projectiles
pz=&p[1];  z=0;
for (i=0; i<PMAX; i++) p[0].ps[i].AdvanceProjectile();
pz=&p[0];  z=1;
for (i=0; i<PMAX; i++) p[1].ps[i].AdvanceProjectile();


//Move players
pz=&p[1]; z=0; p[0].Update();
pz=&p[0]; z=1; p[1].Update();
}


};
*/

void TickUpdate()
{
int i;
//if (!comconnected ||
if (ready && commaster) uu++;

 //Do multiple sound effects
// ContinueMixedSoundEffect();

if (pause) return;
if (quit) return;

for (i=0; i<2; i++)
 if (p[i].ctype==1)
  p[i].in.ReadButt(i);


 //Play kill, kill kill.....                                    //!currse
 if ((p[0].fatality==1 || p[0].fatality==2) && !p[0].throwstat && (uu&127)==64 && !pause)
   PlaySysSE(9,160);

if (gamestat)
 {
  if (gamestat==-2)  //Fade in
        {fadelevel+=1;
         if (fadelevel>=32)
          {fontdur=0; gamestat++;
                if(demo) {gamestat=0;p[0].stat=STANCE|CONTROL; p[1].stat=STANCE|CONTROL;}
          }
        }
  if (gamestat==-1 && ready)  //pregame shit, wait till ready though
        {
          fontdur++;
                if (fontdur==30) PlaySysSE(3,160);
                if (fontdur==110) PlaySysSE(3+rndnumber,160);
                if (fontdur==190) PlaySysSE(8+fonttype,160);

                if (fontdur>280)   //Done pre game
                {gamestat++; fontdur=0;
                 p[0].stat=STANCE|CONTROL;
                 p[1].stat=STANCE|CONTROL;
                 free(word[0]); free(word[1]); free(word[2]);
                 }
        }
  if (gamestat==2)
 {
        p[0].dur=p[1].dur=30000;
                fontdur++;

                //if (fontdur==110) PlaySysSE(3+rndnumber,160);
                if (postwordtype &4)
                 {
        if (fontdur==30)  { PlaySysSE(postse,160);}
                 }
                else
                 {
        if (fontdur==15)   { PlayMixedSoundEffect(&p[postwordtype&1].namevoc,16);}
        if (fontdur==115) { PlaySysSE(postse,160); }
                 }
                if (fontdur> 190)   //Done postgame
        {gamestat++; fontdur=0;
          if (musicstat) stop_music();
          }
 }
  if (gamestat==3)  //Fade out
        {fadelevel-=1; if (fadelevel<=0)  gamestat++; } //Fade out
  if (gamestat==4)
        {
          ready=1;
          if (complay)
           {
//            ioSetComFunc(DefaultComTick);   
            comconnected=0; ready=0;
            commaster=1;
           } 
            
        scrdone=0;
        if (rndcount[0]==2 || rndcount[1]==2)   quit=2;
        if (rndnumber>=4) { quit=2;}
        rndnumber++;
        p[0].Reset(); p[1].Reset();
        if (p[0].d!=0) {p[0].d=0;if (p[0].in.stat&3) p[0].in.stat^=3; }
        if (p[1].d!=1) {p[1].d=1;if (p[1].in.stat&3) p[1].in.stat^=3; }

        cbsize[0]= 0; cbsize[1]= 0;
        nbsize[0]=BL; nbsize[1]=BL;
        p[0].x= 90; p[1].x=230;
        p[0].dizzy=INITIALDIZZY; p[1].dizzy=INITIALDIZZY;
        drawpriority=0;
        scrollx=160; slowdown=0; gamestat=-4; uu=1;

//      for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;

        if (timeleft!=100) timeleft=99; timetickdur=0;
        }
 }
 else
if (timeleft!=100 && p[0].energy && p[1].energy)
         {
          if (timetickdur) timetickdur--;
                else
                 {
        timetickdur=TIMETICK;
        if (timeleft) timeleft--;
         else
          //Time up
         {
          p[0].stat&=~CONTROL; p[1].stat&=~CONTROL;
          if (p[0].cm==0 || (p[0].cm==101 && p[0].y==basey) )
                if (p[0].energy<=p[1].energy) { p[0].cm=99; p[0].newmove=1;}
          if (p[1].cm==0 || (p[1].cm==101 && p[1].y==basey))
                if (p[1].energy<=p[0].energy) { p[1].cm=99; p[1].newmove=1;}

          if ( (p[0].cm==99 && (p[1].cm==0 || p[1].cm==99)) ||
                         (p[1].cm==99 && (p[0].cm==0 || p[0].cm==99))   )
                {
                if (p[0].energy==p[1].energy) { p[0].energy=p[1].energy=0; nbsize[0]=nbsize[1]=0;}
                  else if (p[0].energy>p[1].energy) {p[1].energy=0; nbsize[1]=0;}
                  else {p[0].energy=0; nbsize[1]=0;}
                }
         }
                 }
         }




 //Check status bars
 if (nbsize[0]!=cbsize[0])
         {if (nbsize[0]>cbsize[0]) cbsize[0]++;
                 else if (uu&1) cbsize[0]--; }
 if (nbsize[1]!=cbsize[1])
         {if (nbsize[1]>cbsize[1]) cbsize[1]++;
                 else if (uu&1) cbsize[1]--; }


if (!comconnected)
//Slow down after fireball/death
 if (slowdown && !stopdur)
  {
        slowdown--;
        if (slowdown>50) {if (slowdown&0x3) return;}
          else {if (slowdown&0x1) return;}
  }

 //Advance background animations
// if (options->bganimation)
 {
  series *bp;
  bp=bg.bgs;   for (i=0; i<=bg.nums; i++,bp++)  bp->Tick();
  bp=bg.bgfs;  for (i=0; i<=bg.numfs; i++,bp++) bp->Tick();
 }

//Tick effects
for (effect *eptr=effects; eptr<&effects[MAXMARKS*2]; eptr++)
         if (eptr->e) eptr->Tick();
if (p[0].eattach.e)
 {
  p[0].eattach.Tick();
  if (p[0].eattach.en==20 && !(uu&31))
        GenerateEffect(16,p[0].x+p[0].eattach.x,p[0].y+p[0].eattach.y,0,-30,random(2),random(2));

 }
if (p[1].eattach.e)
 { p[1].eattach.Tick();
 }


if (!ready) return;


 //Make ground shake
 if (gshakedur)
  {
        gshakedur--;
        gshake=(gshakedur&8)/2;
        if (gshakedur<25) gshake/=2;
        if (!gshakedur) {gshake=0; oldscrollx=-1; }
  } else gshake=0;



//Do scrolling
#define SL 60
#define SR 260
if ((p[0].x<SL || p[1].x<SL) && !(p[0].x>SR-1 || p[1].x>SR-1)) scrolldx-=1; else
if ((p[0].x>SR || p[1].x>SR) && !(p[0].x<SL+1 || p[1].x<SL+1)) scrolldx+=1;

 for (register playerstat *psz=&p[0]; psz<&p[2]; psz++)
  if (psz->shakedur)
        {
          if (abs(psz->shake)>=5)        psz->shake=0;
                else  if (!psz->d) psz->shake-=1; else psz->shake+=1;
          if (!--psz->shakedur) psz->shake=0;

          if (psz->shakedur<22 && !(psz->shakedur&15))
                if (psz->y==basey)  GenerateEffect(31, psz->x,psz->y,-25,0,psz->d,0);
        }

//Guy update
if (newframe) AttackCheck();


//Stop action for hit
if  (stopdur>0) {stopdur--; if (shakeup) gshakedur=50; return;} //Dot(3,0);return;}

if (randomteleport && !(uu&63))
         p[ (uu>>6)&1].x=random(300)+10;


if (!comconnected)
{
//Move projectiles
pz=&p[1];  z=0;
for (i=0; i<PMAX; i++) p[0].ps[i].AdvanceProjectile();
pz=&p[0];  z=1;
for (i=0; i<PMAX; i++) p[1].ps[i].AdvanceProjectile();


//Move players
pz=&p[1]; z=0; p[0].Update();
pz=&p[0]; z=1; p[1].Update();
} else if (commaster) SendPos();

}


int keybutton[2][4]=
          {
                {24,26,38,40},
                {16,18,30,32}
          };


static char oldscan=0;
int ki,ka;
int olds;


extern "C" {
void __cdecl start_dac();
};

void Keyboard()
{


kbscan=inp(0x60);

//return;


if (oldscan!=kbscan)
{
        oldscan=kbscan;

        kbint=1;
        if (!(kbstat&1)) //Control
                for (ki=0; ki<2; ki++)
                 p[ki].in.ReadPosition(p[ki].d);

        if (!(kbscan&0x80))
        {
        //----------------NOT CONTROL
        if (!(kbstat&1))
        {
         for (ki=0; ki<2; ki++)
         if (p[ki].in.type>=3 && p[ki].in.type<=4 && !kbstat )
         {

                ka=(p[ki].in.type&1)^1;
                if (oldscan>=keybutton[ka][0] && oldscan<=keybutton[ka][1]) {p[ki].in.stat|=16; p[ki].kbmove=oldscan-keybutton[ka][0]+1; }
                if (oldscan>=keybutton[ka][2] && oldscan<=keybutton[ka][3]) {p[ki].in.stat|=32; p[ki].kbmove=oldscan-keybutton[ka][2]+1+3; }
         }

        //Check key
          if (kbscan==1) quit=1;
        }

        //----------------CONTROL
        if (kbstat&1) //Control
        {

          if (kbscan==25)
                {
                 pause^=1;        //p
                 if (musicstat) playing=pause^1;
                 
                }

          if (kbscan==20)                  //t
                if (timeleft==100) {timeleft=99; timetickdur=0;}
                  else        {timeleft=100; }

          if (kbscan==21)           start_dac();


          if (kbscan==23) //i
             drawstatus^=1;

          if (kbscan==36) //c
         {
//              messagedur=100;
//              for (i=0; i<2; i++) p[i].in.Init(p[i].in.type);
//              message="JOYSTICK CENTERED";
         }

         if (kbscan>=59 && kbscan<=68) doparticularbg=kbscan-59;
         if (kbscan==87) doparticularbg=10;
         if (kbscan==88) doparticularbg=11;
         


                if (debug)
                {
                  olds=speed;
                  if (kbscan==74 && speed>20)  {speed-=15; message="SLOW DOWN"; messagedur=100;}
                  if (kbscan==78 && speed<500)  {speed+=15; message="SPEED UP"; messagedur=100;}
                  if (kbscan==55)  { speed=100; message="STANDARD SPEED"; messagedur=100;}
                  if (speed!=olds) SetTimerSpeed(speed);

                        if (kbscan==48) {++bloodlevel; message="BLOODIER"; messagedur=100;}//b
                  int domessage=0;
//                if (kbscan==21) {if (p[0].pain>=0) p[0].pain=-1; else p[0].pain=(MAXENERGY-p[0].energy)/ (MAXENERGY/4); message="INVISIBLE P1"; messagedur=100;} //y
//                if (kbscan==22) {if (p[1].pain>=0) p[1].pain=-1; else p[1].pain=(MAXENERGY-p[1].energy)/ (MAXENERGY/4); message="INVISIBLE P2"; messagedur=100;} //u
                  if (kbscan==30) {airmove^=1; domessage=airmove+1; message="AIR MOVES ";}     //a
                  if (kbscan==31) {supermove^=1; domessage=supermove+1; message="SUPER MOVES ";}     //s
                  if (kbscan==33)                  //f
                          {p[0].numjoymoves=p[0].numfm; p[1].numjoymoves=p[1].numfm; message="FATALITY"; domessage=2;}

                  if (kbscan==16) {offscreen^=1; domessage=offscreen+1; message="OFFSCREEN ";}      //q
                  if (kbscan==18) {death^=1; domessage=death+1; message="DEATH MODE ";}      //e
                  if (kbscan==19) {region^=1; domessage=region+1; message="REGIONS ";}      //r
                  if (kbscan==47) {vertscroll^=1; domessage=vertscroll+1; message="VSCROLL ";}  //v
                  if (kbscan==37) {p[1].energy=0; messagedur=100; message="KILL!!!!";}  //k
                  if (kbscan==35) {randomteleport^=1; domessage=randomteleport+1; message="TELEPORT ";}  //h
                  if (kbscan==38) {headprojectile^=1; domessage=headprojectile+1; message="HEADS ";}//l
                  if (kbscan==45) {suction^=1;domessage=suction+1; message="SUCTION ";} //x
                  if (kbscan==44) {speedup^=1;domessage=speedup+1; message="ZOOM ";} //z
                  if (kbscan==49) {shakeup^=1;domessage=shakeup+1; message="SHAKE ";} //n
                  if (kbscan==32) {black^=1;  domessage=black+1; message="DARK ";}  //d
                  if (kbscan==24) {strobe^=1; domessage=strobe+1; message="STROBE ";}//o
                  if (kbscan==51) {fastp1^=1; domessage=fastp1+1; message="FASTP1 ";}//<
                  if (kbscan==52) {fastp2^=1; domessage=fastp2+1; message="FASTP2 ";}//>
                  if (kbscan==53) {takescreenshot=1; message="SHOTTAKEN"; } // /

//                if (kbscan==21)
//                        soundcheck^=1;
                  if (kbscan==22)       {framecheck^=1; su=uu=1;}

                  if (domessage)
                        {
                         strcpy(msg,message);
                         strcat(msg,domessage==1 ? "OFF" : "ON");
                         message=msg;
                         messagedur=100;
                        }
         }    //if debug

 }  //if controll pressed

}

if (kbscan==0x1D) kbstat|=1;
if (kbscan==0x9D) kbstat&=~1;


if (kbscan&0x80) kbscan=0;

kbint=0;

}

}


void series::Tick()
{
if (!numframes) return;

if (dur) dur--;
        else
        {
         currframe++;
         if (currframe>=numframes)
          {currframe=0; frameptr=(unsigned char *)(bg.bgframes+firstframe);    
              } else frameptr+=sizeof(bgframe);
         dur=((bgframe *)frameptr)->dur;
        }
}

int drawraw=0;
void DrawRawBG(char *s, char *d,int bx,int by,int dx, int dy,int bgynum);

void CreateBackground()
{

if (scrolldx)
  {
        scrollx+=scrolldx;
        if (scrollx<0) scrollx=0;
         else
        if (scrollx>800- (800/2)) scrollx=800- (800/2);
         else
          {
                p[0].x-=scrolldx; p[1].x-=scrolldx;
                for (effect *eptr=effects; eptr<&effects[MAXMARKS*2]; eptr++)
                 if (eptr->e && eptr->en!=27) eptr->x-=scrolldx;

                for (projectilestat *pq=&p[0].ps[0]; pq<&p[0].ps[PMAX]; pq++)
                 if (pq->pframeptr) pq->px-=scrolldx;
                for (pq=&p[1].ps[0]; pq<&p[1].ps[PMAX]; pq++)
                 if (pq->pframeptr) pq->px-=scrolldx;
          }
        scrolldx=0;
  }
if (strobe) return;

bg.scrx=scrollx*8/10; //* 720/640
bg.scry=( (p[0].y+p[1].y)/2-basey+80 )/8 ;
//if (!vertscroll || p[0].throwstat) scry=10;

//scry-=gshake;
if (bg.scry<0) bg.scry=0;
scry320=bg.scry*256+bg.scry*64;

//bg.DrawBackground(screen);

//MapBackground(screen);
if (black) Solid(screen,64000,0);
 else
if (bg.scrx!=oldscrollx || su<scrdone)
 {
        if (bg.scrx!=oldscrollx) scrdone=su+3;
        oldscrollx=bg.scrx;

        bg.DrawBackground(screen);
 } else
 if (scrdone<su)
   {
    if (drawraw) bg.DrawFloorBG(background,bg.scrx);   
    MemoryCopy(screen+10*320,background+10*320,320*185);
   } 
  else
  {
        bg.DrawBackground(background);
        MemoryCopy(screen+10*320,background+10*320,320*185);
         //Create saved background
  }


drawraw=0;
}


int GetRnd(int x)
{
return(random(x));
}



#define B1 15
#define B2 173


void InitStatusBar()
{

statusbar=(char *)malloc( (BH+2)*320);

for (i=0; i<(BH+2)*320; i++) statusbar[i]=0;

for (int i=0; i<2; i++)
 {
  nbsize[i]=BL;
  cbsize[i]=0;
  pbsize[i]=0;
 }

DrawBar(statusbar,0,B1,0,BL+2,BH+2);
DrawBar(statusbar,4,B1+1,1,BL,BH);
DrawBar(statusbar,0,B2,0,BL+2,BH+2);
DrawBar(statusbar,4,B2+1,1,BL,BH);

}


int  UpdateStatusBar()
{
int i;
int update=0;


for (i=0; i<2; i++)
 if (cbsize[i]!=pbsize[i]) //We have to update bar
  {
        asmcli;
        int pb=pbsize[i];
        int cb=cbsize[i];
        pbsize[i]=cbsize[i];
        asmsti;

        if (i==0)
         {
          if (pb>cb) DrawBar(statusbar,4,(B1+1+BL)-pb,1,pb-cb,BH);
          if (pb<cb)
                 {
        for ( ; pb<cb; cb--)
         DrawBar(statusbar,64+7-cb*8/(BL+1),(B1+1+BL)-cb, 1,1,BH);
                 }
         }
          else
         {
          if (pb>cb)    DrawBar(statusbar,4,(B2+1)+cb,1,pb-cb,BH);
          if (pb<cb)
        for ( ; pb<cb; pb++)
         DrawBar(statusbar,64+7-pb*8/(BL+1),(B2+1)+pb, 1,1,BH);
         }
        update=1;

//   MemoryCopy((char *)MK_FP(0xA000,60800 ),statusbar,10*320);
  }



return(update);
}


void DrawName()
{
//if (DMAstat) return;
//if (NeedBlock()) return;
//  if (p[0].stat&CONTROL)
  DrawImage(p[0].name,screen,15+3,25+2+bg.scry,0);
//  if (p[1].stat&CONTROL)
  DrawImage(p[1].name,screen,320-13-3-p[1].namex,25+2+bg.scry,0);
}


void DrawTime()
{
if (timeleft!=100)
 if (timeleft>15 || !timeleft || uu&16 || !p[0].energy || !p[1].energy)
  {
        if (statbar==1)
         {
          DrawLetter(font,screen+28*320+151+scry320,timeleft/10+'0',320);
          DrawLetter(font,screen+28*320+161+scry320,timeleft%10+'0',320);
         }

  }
}




void PutStickyImage(char *img, int x, int y, int o)
{
int xlen,ylen,xst,yst,clip;
clip=0; //clip flag
xst=yst=0;
xlen=(int) (unsigned char)img[1];
ylen=(int) (unsigned char)img[2];

y+=random(15)-7;
y-=bg.floory;
x+=bg.scrx+(bg.scrx+160-320)*(y+ylen/2)/150;
x+=(720-640)/2;
if (y+ylen<0) return;
if (y>bg.floorheight-10) return;
if (x+xlen<0 || x>720) return;

if (x+xlen>720) {xlen=720-x; clip=1; }
if (x<0)         {xst=-x; clip=1;  x=0; xlen-=xst;}
if (y<0)         {yst=-y; clip=1;  y=0; ylen-=yst; return; }
if (y+ylen>bg.floorheight) {ylen=bg.floorheight-y; clip=1; if (ylen<0) return;}

//PutImageFlip256(img,bg.bg1raw+(y-bg.bg2y)*640+x+bg.scrx*8/10,640,o);
if (!clip) PutImageFlip256(img,bg.bgfloor+x+y*720,720,o);
 else      PutImageFlipClip256(img,bg.bgfloor+(720*y)+x,720,xst,yst,xlen,ylen,o);

/*if (y>200) return;

int wx,wy; //width and height of image
wx=(int)(unsigned char)img[1];  //width of image
wy=(int)(unsigned char)img[2];  //height of image
img+=4;    ///s points to ylist
short int *imgylist=(short int *)img;
img+=wy*2; ///s points to image

char *s=img;
char *d=bg.bg1raw+(y-bg.bg2y)*640+x+bg.scrx*8/10; //get location
*/

}



extern char *eimagelist[256];
extern move elist[];
extern unsigned char *eframe;



void DrawRawBG(char *s, char *d,int bx,int by,int dx, int dy,int bgynum);

void effect::Draw()
{
asmcli;
if (!e)
  {
        asmsti;
         return;
  }
int tx=x;
int ty=y-80;
if (en==27) ty+=bg.scry;
int td=d;
image *i=(image *) (e+sizeof(frametype));
asmsti;

//tx=0; ty=0;
if (!stickyblood || ((en!=11 || (currframe<4 || (currframe!=numframes-3 && random(10))) ) //puddle
  &&  en!=15))// && (en!=19 || currframe!=numframes-1))
{
  while ( i->index!=0xFF )
  {
   DrawImage(eimagelist[i->index],screen,tx+((signed int) (signed char)(i->dispx[td])),ty+((signed int) (signed char)(i->dispy) ),i->orient^(td<<1));
   i++;
  }
}  
else
{
  drawraw=1;  
  while ( i->index!=0xFF )
  {
   PutStickyImage(eimagelist[i->index],tx+((signed int) (signed char)(i->dispx[td])),ty+((signed int) (signed char)(i->dispy) ),i->orient^(td<<1));
   i++;
  }
}
 

}



void effect::Tick()
{
 int len=en;

 if (!d)  tcx+=*((short int *)&((frametype *)e)->dx);
         else  tcx-=*((short int *)&((frametype *)e)->dx);

  x+=tcx>>8; tcx&=0xFF;
 tcy+=*((short int *)&((frametype *)e)->tx); y+=tcy>>8; tcy&=0xFF;

 if (y>maxy) //See if hit ground
        {
         en=11;
         if (len>=7 && len<=9) if (!d) x-=20; else x+=20;
         if (len>=12 && len<=14) {en=15; y+=32; }
         maxy=999;
        }

        //dec dur
 if (dur) dur--;
  else
  {     //end of frame
        currframe++;
        if (currframe<numframes)
          {e+=((frametype *)e)->size;   dur=((frametype *)e)->dur; }     //Set up next duration
        else
         if (en!=19 || stickyblood)
         {e=0; if (en==27 || en==21 )  len=0;    }//Die
        else {dur=9999; currframe--;}
  }

         //new effect
 if (len!=en)
        {
         e=(unsigned char *)eframe+elist[en].firstframe;
         dur=((frametype *)e)->dur;
         currframe=0; numframes=elist[en].numframes;
        }

}


void playerstat::DrawAttachEffect()
{
asmcli;
if (!eattach.e)
  {
        asmsti;
        return;
  }
int tx=eattach.x;
int ty=eattach.y-80;
int td=eattach.d;
register image *i=(image *) (eattach.e+sizeof(frametype));
asmsti;

while ( i->index!=0xFF )
{
 DrawImage(eimagelist[i->index],screen,x+tx+i->dispx[d],y+ty+i->dispy,i->orient^(td<<1));
 i++;
}
}







void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p)
{

if (en==16) en=42+GetRnd(6);
if (en==28) en+=bgdust[bgnum];
if (en==31) en+=bgdust[bgnum];
if (!elist[en].firstframe) return;



 effect *eptr=effects;
 int i;
  eptr+=MAXMARKS*p;

 for (i=0; i<MAXMARKS; eptr++,i++)
  if (!eptr->e)
        {
         eptr->e=(unsigned char *)eframe+elist[en].firstframe;
         eptr->currframe=0; eptr->numframes=elist[en].numframes;
         eptr->en=en; eptr->d=d;
         if (!d) eptr->x=x+dx; else eptr->x=x-dx;
         eptr->y=y+dy;
         eptr->dur=((frametype *)eptr->e)->dur;

//       if (en==16) eptr->dx=-(GetRnd(0x170)-50);
//              else eptr->dx=eptr->e->dx;
//       if (d)      eptr->dx=-eptr->dx;
         eptr->tcx=eptr->tcy=0;

         eptr->maxy=999;
         if (en>=7 && en<=10) eptr->maxy=6+basey;
         if (en>=12 && en<=14) eptr->maxy=-26+basey;
         if (en>=42 && en<=47) eptr->maxy=5+basey;
         eptr->maxy+=GetRnd(6);

         if (p) eptr->maxy-=15;


//       if (en==16) eptr->dur+=GetRnd(15)+6;
//       if (en==10) eptr->dur+=GetRnd(25);


         break;
        }

}




void PutImageFunky(char *s,char *d,int x,int y,int r)
{
d+=x+y*320;

int wx,wy; //width and height of image
wx=(int)(unsigned char)s[1];
wy=(int)(unsigned char)s[2];

s+=4+wy+wy; ///s points to image

int prob=r*32768/DISSOLVESPEED;

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F; d+=a; i-=a; }
  else         //opaque
         {
          for (int b=a; b>0; b--)
                 {
                  if (rand()<=prob) *d=*s;
                  d++; s++;
                 }
          //MemoryCopy(d,s,a);
          //d+=a; s+=a;
                i-=a;
         }
 }
d-=wx; d+=320;
}

}


void PutImageFunkyFlip(char *s,char *d,int x,int y,int r)
{
d+=x+y*320;

int wx,wy; //width and height of image
wx=(int)(unsigned char)s[1];
wy=(int)(unsigned char)s[2];

s+=4+wy+wy; ///s points to image

d+=wx;

int prob=r*32768/DISSOLVESPEED;

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F; d-=a; i-=a; }
  else         //opaque
         {
          for (int b=a; b>0; b--)
                 {
                  if (rand()<=prob) *d=*s;
                  d--; s++;
                 }
          //MemoryCopy(d,s,a);
          //d+=a; s+=a;
                i-=a;
         }
 }
d+=wx; d+=320;
}

}


void playerstat::DrawFrameFunky(int r)
{

asmcli;
if (!numframes)
  {
      asmsti;
        return;
  }

int tx=x+shake;
int ty=y-80;
int td=d;

register image *i=(image *) (frameptr+sizeof(frametype));

asmsti;

while ( i->index!=0xFF )
{
if ((i->orient&2)^(td<<1))
PutImageFunkyFlip(imagelist[i->index],screen,tx+i->dispx[td],ty+i->dispy,r);
  else
PutImageFunky(imagelist[i->index],screen,tx+i->dispx[td],ty+i->dispy,r);
 i++;
}
}
/*
void DrawSoundCheck()
 {
  char s[30];
  sprintf(s,"CURR %5X",currse);
  DrawString(font,screen+100*320+10,s,320);
  sprintf(s,"CONT %5X",contse);
  DrawString(font,screen+120*320+10,s,320);
  sprintf(s,"DMASTAT %s",DMAstat ? "ON" : "OFF");
  DrawString(font,screen+140*320+10,s,320);
  sprintf(s,"MIXLEN %5X",mixlen);
  DrawString(font,screen+156*320+10,s,320);
//  sprintf(s,"0X22E %X",inportb(0x22e));
  DrawString(font,screen+175*320+10,s,320);
 }
  */


//Play SOUND effect that cancels out all others
void PlayModalSoundEffect(SOUND *se)
{
 if (sbstat!=2 || !se->soundptr) return;
 start_sound(se, 2, 255, 0);
}

void PlayMixedSoundEffect(SOUND *se,int priority)
{
    priority=priority;
 if (sbstat!=2 || !se->soundptr) return;
 start_sound(se, 1, 255,0);

};

void StopVoice()
{
    if (!sbstat) return;
 stop_sound(1);
 stop_sound(2);
}


void PlaySysSE(char se,int x)
{
    x=x;
if (sbstat!=2 || !sysse[se].soundptr) return;
PlayMixedSoundEffect(&sysse[se],16);
}

void PlaySE(unsigned char se,char type,char pl,int xc) //type 0-swish 1-hit
{
    xc=xc;
if (gamestat<0) return;

if (type) se>>=4;
se&=15; if (!(se&7)) return;

SOUND *s;
if (!(se&8)) s=&sse[type][ (se&7) -1];       //It's a standard se
          else    s=&p[pl].se[type][ (se&7) -1];  //It's a player specific se
if (s->soundptr)  PlayMixedSoundEffect(s,8);

}
  
