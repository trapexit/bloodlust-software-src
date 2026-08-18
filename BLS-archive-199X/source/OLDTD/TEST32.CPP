//Copyright(c) 1995 Bloodlust Software All rights reserved
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <direct.h>
#include <graph.h>

unsigned int imageX(char *i);
extern "C"
{
#include "gfxmisc.h"
void gfxNewRotate(char *image, char *screen, int angle);
}

#define VERSION "1.00 DEMO"

#define DEMO

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

#include "ll_comm.h"
#include "tipx.h"
#include "tpacket.h"

#include "tchat.h"

#include "cdrom.h"


#define asmcli _disable();
#define asmsti _enable();

#define BL 130
#define BH 9

#define TIMETICK 80
#define ED 40
#define INITIALDIZZY 15
#define DIZZYMAX     30
#define DIZZYMIN     -30

#define MAXMARKS 40

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
//void FunkyFont(char *f);
void FunkyFont(char *f,unsigned char a,unsigned char b);

void OptionsScreen(int x);


//PLAYER
int sz,z;
playerstat p[2];
extern char *cnlist[]=
 {"","","SAVAGE","","","",
  "ASYLUM","","","","","","","","","","","","","",""};
playerstat *pz;
int drawpriority;
int newframe;
int rndcount[2],rndnumber;
int orndcount[2];
int stopdur;           //Tell to stop after being hit
int slowdown;

int timeleft;
int timetickdur;

extern int bioexist[];  //if a certain character exists
extern int bgexist[];   //if a certain background exists




//Graphics
char *font,*font2,*font3,*font4,*font5,*font6;
char *screen,*background;
char *statusbar;


//BAckground
bground bg;
extern color pal1[256];
int bgnum;   //character number 5 would use bg number 9, bgnum==5
int bgdust[]={0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0};
extern int fadelevel,oldfadelevel;
int scrollx,scry320,scrolldx;
int oldscrollx=-1;
int scrdone=0;
int gshakedur,gshake,gstemp;
int pbsize[2]={0,0};  //PHYSICAL length of bar
int cbsize[2]={0,0};  //Current length of bar
int nbsize[2]={0,0};  //Needed length of bar (for incremental decreasing when hit) (timer)
effect effects[MAXMARKS*2];
int sendeffect=1;


int bgfloorylist[]=
 {160,160,140,140,150,150,155,158,0,160,160,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
#define DEMOFLOORY 160

int doparticularbg=-1;

int doextract=0;
//Game
int game,cstat,fstate,demo,gamestat,BMAC,playgame,dooptions=0,lastwinner;
int debug,config;
int region,MAXENERGY,drawstatus=1;
int scrolling=0;
int vertscroll=1;
int airmove=0,supermove=0,randomteleport=0,headprojectile=0,suction=0,speedup=0,shakeup=0,black=0,strobe=0,fastp1=0,fastp2=0,death=0,offscreen=0;;
int stickyblood=0;
int slide=0,showcombo=0;
int statbar=1;
int screensave=0;
int battle;
int bloodlevel=1;
int spin=0,tilt=0,tiltlevel=0;
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
int cdinstalled=0,cdplay=0;

int sepriority=0;

SOUND sysse[20];
SOUND sse[2][7];
//extern volatile int playing;

//2-12sec
//8-0sec
//14-15secs
//1-0
//13-0
//4-0
//3-:40
//11-0
//6-2:53
//9-2:28
//12-0
//5-0
//7-:44

int cdtracks[]={2,8,14,1,13,4,3,11,6,9,12,5,11,11,11,11,11};

int cdstartpos[]=
 {
  0, //track 0
  0, //track 1
  12, //track 2
  40, //track 3
  0, //track 4
  0, //track 5
  2*60+53, //track 6
  0, //44, //track 7
  0, //track 8
  2*60+28, //track 9
  0, //track 10
  0, //track 11
  0, //track 12
  0, //track 13
  15, //track 14
  0, //track 15
  0, //track 16
  0,
 };


//System
volatile char kbscan;
volatile int quit=0,pause=0,kbstat=0;
volatile int kbint=0;
volatile unsigned int uu,su,fu;
//short dataseg;
//short SEGVIDEO;

//Multiiplayer stuff
int multiinstalled=0; //flag to tell whether or not multiplayer capability is on 
int multiplay=0;      //flag to tell whether or not we are playing multiplayer this GAME
int multimaster=0;    //master/slave relationship
int multiconnected;   //whether or not a connection has been established in game
int ready;            //should the game proceed? always true for local 

netplayer *multiopponent=0; //our opponent for the game

char ipxmaster=0;
int ipxoffset=0; //ipx socket address

//comport stuff
int cominstalled=0;  //if COM driver is installed
COMM port;            //port for com  
int COMPORT;          //number for com port, COM1,COM2, etc
void DefaultComTick() {}    


  //chat stuff
int chatstat=0; //not chatting
int chatptr;
char chatstr[128];
char chatrecvstr[128];
int chatrecvstat=0;
int simulkey=0;

//combo display
char *comboname[]=
 {
  "No combo","Blah Combo", //0 and 1
  "Power Combo", //2
  "Clubbering Combo", //3
  "Fistesses Combo", //4
  "Sgt Toochie Combo", //5
  "Zowie Nipple Combo", //6
  "Chili Boy Combo", //7
  "Drunk Indian Combo", //8
  "Mongoloid Combo", //9
  "Strode Combo", //10
  "Tardwagon Combo", //11
  "Mongoloid Combo", //12
  "Hairy Fat Guy Combo", //13
  "Touch My PeePee Combo", //14
  "Sardu Combo", //15
  "Alien Manholer Combo", //16
  "Mr. Round Combo", //17
  "Touch My PooPoo Combo", //18
  "Ralphus Combo", //19
  "Ministilt Combo", //20
  0,0,0,0,0,0,0
 };



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
int dodpmi=0,dooldvect=0,nodotimer=0,nodokeyb=0, docom=0,doipx=0;
int dodiagnose=0;
int doawe32=0,docdrom=0;
int doquick=0;
char *doplaycin=0;
int donocin=0;



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

extern "C" int ipxlisten(int);
short int pdata=0;
int pdatastat=0;
void pfunc()
{
 pdatastat=1;
 pdata=((packet *)prbuf)->data;
}    
void tschat();

void MakeFont();
netplayer *ChatScreen();

//extern unsigned int itu,imu,iuu;

unsigned tsCD=0; //the drive containing the ts cd
int firstCDtrack,lastCDtrack;

//#define USECDSTART

void PlayCDTrack(int x)
{
 cd_stop_audio();   
 cd_set_track (x+1);
 if (cdrom_data.error&0x8000) return;
 unsigned long startpos=cdrom_data.track_position;
 unsigned long endpos=startpos+get_track_length(x+1);

 #ifdef USECDSTART
  int sp=cdstartpos[x]; //get place within track
  startpos+=(sp%60)*75;
  startpos+=(sp/60)*4500;
 #endif 

 cd_seek(startpos);
 cd_play_audio(startpos,endpos);
}


void StopCD()
{
   
 cd_stop_audio();
// cd_lock(UNLOCK); 
}

struct volumeinfo cd_vol;

void SetCDVolume(unsigned int v)
{
cd_vol.volume0 = v;
cd_vol.volume1 = v;
cd_vol.volume2 = v;
cd_vol.volume3 = v;
cd_set_volume (&cd_vol);

}    


extern "C" {
void InitDosError();
void TerminateDosError();
}    



void main(int argc, char *argv[])
{
_clearscreen(_GCLEARSCREEN);
printf("TimeSlaughter Final V%s NS\n\n",VERSION);

//getch();


//********************************************************************
printf("M.inistilt I.nteractive D.ynamic G.ame E.ngine T.ardprogram\n");
printf("      Copyright 1996 Bloodlust Software \n\n");

InitDosError();


int nodomusic=0, nodosound=0;


int allbg,allchar,domidi=0,fakecd=0;
allbg=0; allchar=0;
stickyblood=1;
for (i=1; i<argc; i++)
 {
  if (!stricmp(argv[i],"midgetpower"))
    {debug=1; }
  if (!stricmp(argv[i],"nomusic"))
    { printf("-music disabled\n"); nodomusic=1;}
  if (!stricmp(argv[i],"nosound"))
   {printf("-digital audio disabled...\n");  nodosound=1;}
  if (!stricmp(argv[i],"diagnose"))    dodiagnose=1;

  if (!stricmp(argv[i],"playcin") && debug)
     {doplaycin=argv[i+1];}
  if (!stricmp(argv[i],"nocin"))
     {donocin=1;}

  if (!stricmp(argv[i],"nokey"))
     {nodokeyb=1;}

  if (!stricmp(argv[i],"soundextract"))
     {doextract=1;}


  if (!stricmp(argv[i],"com"))
    {
     docom=1;
     sscanf(argv[i+1],"%d",&COMPORT);
     printf("-com play enabled COM%d\n",COMPORT);
    }
  if (!stricmp(argv[i],"ipx"))  doipx=1;
    
  if (!stricmp(argv[i],"quick")) doquick=1;

  if (!stricmp(argv[i],"nosticky")) stickyblood=0;

  if (!stricmp(argv[i],"gagreflex"))  ipxmaster=1;  

    
  if (!stricmp(argv[i],"cdrom")) docdrom=1;
  if (!stricmp(argv[i],"fakecd"))
   {
    sscanf(argv[i+1],"%d",&fakecd);
   }   

//  if (!stricmp(argv[i],"midi")) domidi=1;
 }
GetConfig();
//_clearscreen();
//getch();

srand(*((unsigned int *)(0x6c+0x400)) );
    

multiplay=cominstalled=0;
//Initialize sb stuff
sbstat=0;

if (!nodosound)
if (options->sbon)
 {
  cprintf("-initializing sb voice....");
  sbstat=2;
  if (!init_sb(options->sbport, options->sbirq, options->sbdma, options->sbdma16))
         {
                printf("failed.\nERROR:  Error initializing sound card!\n");
                printf("Check your soundblaster settings\n");
                return;
         }
//printf("initmixing\n");
  init_mixing();
  set_sound_volume(32*options->soundvolume+31);
  printf("done\n");
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
if (options->sbon && !nodomusic && options->musicsource)
 {
  cprintf("-initializing sb music....");
  if (options->sbon==3) doawe32=1;
  musicstat=2;
  if (init_music(ON)) musicstat=0;
   else
   {
    for (i=0; i < 16; i++) set_channelmask(i, 1); //printf("channelmask");
    set_multiplier(1); //printf("multiplier");
    set_music_volume(32*options->musicvolume+31);
    printf("done\n");
    wait();
   }
}
 


//set options
bloodlevel=options->bloodlevel;



//set up input devices
cprintf("-initializing input devices...");
p[0].in.Init(0);
p[1].in.Init(1);
p[0].ctype=1;        //Joystick/keyboard input device
p[1].ctype=3;        //Computer input device
printf("done\n");

wait();


if (!nodokeyb && !domidi)
{
cprintf("-initializing keyboard...");
InitKeyboard((void ( *)())Keyboard);
printf("done\n");
}

//waitk();
//kbscan=0; while (!kbscan);
//goto fuckthis;

//cprintf("-allocating screen buffers...");
background=(char *)malloc(65000);
screen=(char *)malloc(100000);
screen+=15000;
//printf("done\n");
//wait();


//cprintf("-initializing fonts...");
font=ReadFile("font.vol");    //ugly blue font
font2=ReadFile("font2.vol");  //big red font
font3=ReadFile("font.vol");  

font4=ReadFile("font3.vol"); //peachy font
//FunkyFont(font4,0xca,8*16+11);
//FunkyFont(font4,0xdb,8*16+12);


font5=ReadFile("font3.vol"); //white font for cinematics
FunkyFont(font5,0xca,0xff);
FunkyFont(font5,0xdb,0x0);

font6=ReadFile("font3.vol"); //gold looking font for screen
FunkyFont(font6,0xca,0xcd);
FunkyFont(font6,0xdb,0xdf);
//FunkyFont(font6,0xca,8*16+13);
//FunkyFont(font6,0xdb,8*16+14);
//printf("done\n");



//cprintf("-initializing effects...");
InitializeEffects();
//printf("done\n");


//printf("-checking for vols...");
CheckExistence();
//printf("done\n");

//check existence of vols
BMAC=0;
for (i=0; i<12; i++)
 if (bioexist[i]) BMAC|=1<<i;
BMAC&=( (1<<2)+(1<<6) ); 

doparticularbg=-1; 



gfxInitTables();

//if (domidi) Midi();


///quit=0; while (!quit); quit=0;
//printf("***PRESS ANY KEY***\n");
//if (!doquick) { kbscan=0; while (!kbscan); }
//---------------------------------------------
//printf("gonna go to mode256\n");
Mode256();


if (doplaycin)
  {
  PlayCinema(doplaycin);
  goto fuckthis;
  }

/*do
{
do {i=random(12); } while (!bioexist[i]);
BioScreen(i);
} while (1);
//goto fuckthis;
*/
if (!doquick && !multiinstalled)
{

PlayCinema("logo.vol");

//printf("gonna play the prod\n");
PlayCinema("prod2.vol");
if (quit) goto doneprod;
PlayCinema("prod.vol");
if (quit) goto doneprod;
PlayCinema("prod3.vol");
doneprod:

//PlayCinema("cred.vol");

PlayCinema("intro.vol");

}


//WHOLE GAME LOOP
do
{


if (multiopponent) {free(multiopponent); multiopponent=0;}

//Call title screen and figger out what to do
dotitle:
switch(TitleScreen())
 {
  case -1: //quit
    goto fuckthis;

  case -2: //user didn't do anything, do a demo
   demo=1; p[0].ctype=p[1].ctype=3; cstat=fstate=0; multiplay=0; //Play demo
    break;

  case 1: //1 player game
   cstat=0; fstate=0; game=0; demo=0; battle=1;
   p[0].ctype=1; multiplay=0; //player one is human controlled, no multiplayer
   p[1].ctype=3;              //player two is computer controlled
  break;

  case 2: //2 player game
   cstat=0; fstate=0; game=0; demo=0; battle=1;
   p[0].ctype=1; p[1].ctype=1; //humans
   multiplay=0;
     
   if (multiinstalled) //if there is distant multiplay installed
    {
     p[1].ctype=2; multiplay=1; //2 player is distant
     if (ipxinstalled) //if ipx, go to chat screen
      {
        multiopponent=ChatScreen();
        if (!multiopponent)  goto fuckthis; //dotitle; //no opponent, no game
        IPXGameMode((char *)&multiopponent->a.nodeadd,ipxoffset);  //set up ipx for games
      } 
    }
  break;

  case 3: //Options screen
   OptionsScreen(0);
   bloodlevel=options->bloodlevel;
   goto dotitle;
 }



  

lastwinner=0;


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
                    if (allchar && bioexist[9])  //if portal exists, go to him
                     {
                         if ( (fstate&BMAC)==BMAC) {if (options->pnum[i]!=8) PlayCinema("ciny.vol"); options->pnum[i]=8; fstate=0;} //if fatalatied them all, goto staine
                                else           {options->pnum[i]=9; fstate=0;}  //else goto portal
                     }  else cstat=0;
                  }
                 else  ///else pick a base character
                do
                {
                 do
                 {
                 options->pnum[i]=random(10);
                 if (options->pnum[i]==8) options->pnum[i]=10;  //8 is staine
                 if (options->pnum[i]==9) options->pnum[i]=11;  //9 is portal
                 } while (!bioexist[options->pnum[i]]);
                } while ( (cstat&(1<<options->pnum[i])) );


                options->color[i]=0;
                if (options->pnum[i]==options->pnum[i^1]) options->color[i]=options->color[i^1]^1;
          }

  }


//Selection screen
if (game!=1 && !demo) SelectScreen();


if (quit) break;


if (!demo) VersusScreen(options->pnum[0],options->color[0],options->pnum[1],options->color[1]);


//Read Background
char bgname[15];  //char name of the background file
{
 bg.ReadBackground("bg.vol",DEMOFLOORY);
 scrolldx=0;
 bgnum=0;       //snow
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
//cprintf("reading player %s...",cnlist[options->pnum[0]]);
p[0].Initialize(options->pnum[0],options->color[0]);
//printf("done.\n");

//cprintf("reading player %s...",cnlist[options->pnum[1]]);
p[1].Initialize(options->pnum[1],options->color[1]);
//printf("done.\n");
//kbscan=0; while (!kbscan);
//Mode256();
p[0].po=&p[1]; p[1].po=&p[0];



playgame=1;
PlayGame();
playgame=0;
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

if (quit==1) game=0;

//DO INSULT SHIT----------------------------------
if (!demo && quit!=1)
{
for (i=0; i<2; i++)
 if (rndcount[i]==2) //if this player has gotten two wins
  {
       //check if versus mode
   if (p[0].ctype!=3 && p[1].ctype!=3) {game=0; lastwinner=i+1; cstat=0;fstate=0;}  
       else
       //check if player won
   if (p[i].ctype==1)  {game=1; cstat|=1<<p[i^1].type; lastwinner=0;} //1=player won
       else
       //player lost
       {game=2; fstate=0; lastwinner=0;} //2=player lost
       
   InsultScreen(i,p[0].type,p[0].color,p[1].type,p[1].color);
   break;         
  }   
}    


//Play ending if necessary

if (!allchar) //demo
{
if ((cstat==BMAC) )
 {
  PlayCinema("end.vol");
  cstat=0; game=0;
  quit=3;
 }
} else  //game
{
if ((game==1) && ((p[0].type==9 && p[0].ctype==3) || (p[1].type==9 && p[1].ctype==3)) )
 {

  PlayCinema("cinx.vol");

  char s[30];
  sprintf(s,"cin%d.vol",p[0].type+1);
  PlayCinema(s);

  cstat=fstate=0;; game=0; quit=3;
 }
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

timerhandler=0;
musichandler=0;


ModeText();

printf("Copyright (C) 1996 Bloodlust Software. All rights Reserved\n");

printf("\n");
//cprintf("-terminating effects...");
KillEffects();
//printf("done\n");

//cprintf("-terminating keyboard...");
TerminateKeyboard();
//printf("done\n");


if (musicstat==2)
 {
  //cprintf("-terminating music...");
  done_music();
  //printf("done\n");  
 }

if (cdinstalled) StopCD(); //cd_lock(UNLOCK); 

//cprintf("-terminating timers...");
TerminateTimers();
//printf("done\n");

TerminateGrip();


if (sbstat==2)
{
//  cprintf("-terminating sb voice...");
  shutdown_mixing();
  shutdown_sb();
//  printf("done\n");
}

if (cominstalled)
{
 cprintf("-terminating COM...");
 ioClosePort(port);
  printf("done\n"); 
} 

if (ipxinstalled)
{
 cprintf("-terminating ipx...");
 TerminateIPX();
 printf("done\n"); 
} 

printf("You look like someone who would like some all-you-can-eat-chili.\n");
printf("Looks like you're a chili growin' boy.\n");

printf("\n");
printf("Check the ORDER.TXT file for information on ordering, or\n");
printf("go to our www page on the internet: \n");
printf("     http://www.southwind.net/~bldlust \n\n");






//while (kbhit()) getch();

}



void playerstat::ResetCombo()
{
combometer=0;

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



//STUFF FOR NETWORK/MODEM
#define TSCONNECT 0x6
#define TSSCROLL  0x7
#define TSEFFECT  0x8
#define TSQUIT    0x9
#define TSPOS     0xA
#define TSFRAME   0xB
#define TSCHAT    0xC
#define TSATTACK  0xD
#define TSKEY     0xE
#define TSPUSHOPB 0xF


void SendPos();

void tspushopb()
{
 p[0].x-=(int)(signed short)((packet *)prbuf)->data;
}

void tsscroll()
{
//    pause=1;
 scrolldx=((int)((packet *)prbuf)->data)-scrollx;
} 




void tskey()
{
simulkey=((packet *)prbuf)->data;
Keyboard();
simulkey=0;
}    

void tsquit()
{
 quit=1;
}   //the other user quit


void tspos()
{
 p[1].ModemPos(); 
}    

void tsframe()
{
 p[1].ModemMove();
}    
/*
void tsattack()
{
//    pause=1;
int fierceness=    ((attackpacket *)prbuf)->fierce;
p[0].energy=((attackpacket *)prbuf)->energy;
nbsize[0]=p[0].energy*BL/MAXENERGY;
if (nbsize[0]<0) nbsize[0]=0;


p[0].cm=((attackpacket *)prbuf)->cm; 
p[0].stat=(p[0].stat&(~255)) | ((attackpacket *)prbuf)->bstat;
p[1].stat=(p[1].stat&(~255)) | ((attackpacket *)prbuf)->astat;
p[0].frameptr=p[0].frame+p[0].moves[p[0].cm].firstframe;


p[0].newmove=1;
p[0].wallsound=0; //Reset wallsound
p[0].throwvuln=200;

p[0].shakedur=6+7*fierceness;   //make opponent shake
stopdur=5+8*fierceness;        //stop action
}    */

/*
void tsattack()
{
// pause=1;
int fierceness=    ((attackpacket *)prbuf)->fierce%3;
int mark=((attackpacket *)prbuf)->fierce;
int xmark=(int)((attackpacket *)prbuf)->xmark+160;
int ymark=180-(int)((attackpacket *)prbuf)->ymark;

GenerateEffect(mark,xmark,ymark,0,0,p[0].d,0);

p[1].energy=((attackpacket *)prbuf)->energy;
nbsize[1]=p[1].energy*BL/MAXENERGY;
if (nbsize[1]<0) nbsize[1]=0;


p[0].stat=((attackpacket *)prbuf)->astat;
if (p[0].cm!=((attackpacket *)prbuf)->cma)
  {
   p[0].cm=((attackpacket *)prbuf)->cma; 
   p[0].frameptr=p[0].frame+p[0].moves[p[0].cm].firstframe;
  }
p[1].stat=((attackpacket *)prbuf)->bstat;
p[1].cm=((attackpacket *)prbuf)->cmb; 
p[1].frameptr=p[1].frame+p[1].moves[p[1].cm].firstframe;


//pause=1;

p[1].newmove=1;
p[1].wallsound=0; //Reset wallsound
p[1].throwvuln=200;

p[1].shakedur=6+7*fierceness;   //make opponent shake
stopdur=5+8*fierceness;        //stop action

} */

extern int remoteattack;
void tsattack()
{
 _disable();   
 p[0].cm=((attackpacket *)prbuf)->cma;
 p[0].frameptr=((attackpacket *)prbuf)->frameptra+p[0].frame;
 p[0].stat=((attackpacket *)prbuf)->stata;
 p[0].GetRect();


 p[1].cm=((attackpacket *)prbuf)->cmb;
 p[1].frameptr=((attackpacket *)prbuf)->frameptrb+p[1].frame;
 p[1].stat=((attackpacket *)prbuf)->statb;
 p[1].GetRect();

 remoteattack=1;
 AttackCheck();
 remoteattack=0;
_enable();

}

void tschat()
{
strcpy(chatrecvstr,multiopponent->name);
strcat(chatrecvstr,": ");
strcat(chatrecvstr,((chatpacket *)prbuf)->s);
chatrecvstat=500;
}

/*
void tseffect()
{
int se=sendeffect;
sendeffect=0;
GenerateEffect((unsigned int)((effectpacket *)prbuf)->t,((int)((effectpacket *)prbuf)->x)+160,(int)((effectpacket *)prbuf)->y,0,0,0,0);
sendeffect=1;

}    */

int soldx=-1,soldy=-1;

void SendPos()
{
if (soldx==p[0].x && soldy==p[0].y) return;

SendPosPacket(p[0].x-160,180-p[0].y);
soldx=p[0].x; soldy=p[0].y;
}

int soldcm=-1,soldfp=-1;

void SendFrame()
{
int fp=p[0].frameptr-p[0].frame;    
if (fp==soldfp && p[0].cm==soldcm) return;

SendFramePacket(p[0].cm,fp,p[0].stat);

soldfp=fp; soldcm=p[0].cm;

}    




void tsconfirm() {}  


void tsconnect()
{
multimaster=((packet *)prbuf)->data^1; //we are opposite of type sent     
multiconnected=1; ready=1;
packetrecvs=packetsends=packeterror=0;

if (!multimaster) 
 SendStandardPacket(TSCONNECT,multimaster);       //tell them our status
if (multimaster)
 {
  p[0].x= 160-70; p[1].x=160+70;
  p[0].d=0; p[1].d=1;
 } else
 {
  p[0].x= 160+70; p[1].x=160-70;
  p[0].d=1; p[1].d=0;
 }

  SetPacketFunc(TSSCROLL,tsscroll);
  SetPacketFunc(TSQUIT,tsquit);
  SetPacketFunc(TSPOS,tspos);
  SetPacketFunc(TSFRAME,tsframe);
  SetPacketFunc(TSATTACK,tsattack);
  SetPacketFunc(TSKEY,tskey);
  SetPacketFunc(TSPUSHOPB,tspushopb);
  SetPacketFunc(TSCHAT,tschat);
 


} //we are connected!


extern int pe[4];
extern int ptot,ptime;


int CreateFontImageSP256(char *font,char **img,unsigned char *s);

void PlayGame()
{
Solid((char *)0xA0000,64000,0);

//Set up variables
for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;
scrollx=160*10/8;  scrdone=0;
rndcount[0]=rndcount[1]=0; rndnumber=0;
if (timeleft!=100) timeleft=99; timetickdur=0;

p[0].x= 160-70; p[1].x=160+70;

p[0].combostat=p[1].combostat=0;


drawpriority=0;
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
multimaster=1;
if (multiplay)      //flag that tells whether or not we are playing multiplayer
 {
  multiconnected=0; ready=0;       //flag to tell
  multimaster=1;
  ResetPacket();
  SetPacketFunc(TSCONNECT,tsconnect);
  packetrecvs=packetsends=packeterror=0;
 } 
  

//Start playing game
if (!multiplay) SetTimerSpeed(90+options->gamespeed*10);
 else SetTimerSpeed(100);
timerhandler=TickUpdate;
su=1; uu=1;


do
{
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
  DrawString(font2,screen+5+80*320,(unsigned char *)"WAITING FOR OPPONENT",320);

if (framecheck)
{

char s[30];

sprintf(s,"%5d TICKS",uu);
DrawStringSP(font4,screen+120*320+250+scry320,(unsigned char *)s,320);

sprintf(s,"%5d FPS",su*speed/uu);
DrawStringSP(font4,screen+135*320+250+scry320,(unsigned char *)s,320);



if (multiplay)
{
if (multiconnected) DrawStringSP(font4,screen+105*320+0+scry320,(unsigned char *)"CONNECTED",320);
if (ready) DrawStringSP(font4,screen+125*320+0+scry320,(unsigned char *)"READY",320);   
if (multimaster) DrawStringSP(font4,screen+145*320+scry320,(unsigned char *)"MASTER",320);
   else DrawStringSP(font4,screen+145*320+scry320,(unsigned char *)"SLAVE",320);
    
sprintf(s,"PS%5d",packetsends);
DrawStringSP(font4,screen+50*320+250+scry320,(unsigned char *)s,320);
sprintf(s,"PR%5d",packetrecvs);
DrawStringSP(font4,screen+65*320+250+scry320,(unsigned char *)s,320);
sprintf(s,"PE%5d",packeterror);
DrawStringSP(font4,screen+80*320+250+scry320,(unsigned char *)s,320);

sprintf(s,"BYTES%5d",pbytes);
DrawStringSP(font4,screen+95*320+250+scry320,(unsigned char *)s,320);

sprintf(s,"%5d BPS",pbytes*speed/uu);
DrawStringSP(font4,screen+106*320+250+scry320,(unsigned char *)s,320);

}

sprintf(s,"P0X%3d P0Y%3d P0CM%3d S%2X PTOT%d PTIME%d",p[0].x,p[0].y,p[0].cm,p[0].stat,ptot,ptime);
DrawStringSP(font4,screen+50*320+scry320,(unsigned char *)s,320);
   
sprintf(s,"P1X%3d P1Y%3d P1CM%3d S%2X STR%2X",p[1].x,p[1].y,p[1].cm,p[1].stat,p[1].strategy);
DrawStringSP(font4,screen+70*320+scry320,(unsigned char *)s,320);


if (p[0].painexist)
 {
   sprintf(s,"PAIN %d",p[0].pain);
   DrawStringSP(font4,screen+90*320+scry320,(unsigned char *)s,320);
 }  
if (p[1].painexist)
 {
   sprintf(s,"PAIN %d",p[1].pain);     
   DrawStringSP(font4,screen+250+90*320+scry320,(unsigned char *)s,320);
 }


sprintf(s,"MAXE0 %d MAXE1 %d",p[0].maxenergy,p[1].maxenergy);
DrawStringSP(font4,screen+0+100*320+scry320,(unsigned char *)s,320);
}


if (showcombo)
{
if (p[0].combometer!=p[0].oldcombometer) //see if there was a change in combo levels
{
 if (p[0].combometer>=2) //its a combo to be shown
  {
   if (p[0].comboline1) {free(p[0].comboline1); p[0].comboline1=0;}
   if (p[0].comboline2) {free(p[0].comboline2); p[0].comboline2=0;}   
   
   char s[30];
   sprintf(s,"%d HITS!",p[0].combometer);
   CreateFontImageSP256(font2,&p[0].comboline1,(unsigned char *)s);
   CreateFontImageSP256(font6,&p[0].comboline2,(unsigned char *)comboname[p[0].combometer]);
   if (!p[0].combostat)
       p[0].combox=32;
    p[0].combostat=2; //start counting
  }
 p[0].oldcombometer=p[0].combometer;  
}
if (p[0].combostat) //draw it
{
//  DrawImage(p[0].comboline1,screen,p[0].combox,60+bg.scry,0);
//  DrawImage(p[0].comboline2,screen,p[0].combox,69+bg.scry,0);
  PutImageD2(p[0].comboline1,screen+12+54*320+scry320,320,p[0].combox*256);
  PutImageD2(p[0].comboline2,screen+17+77*320+scry320,320,p[0].combox*256);  
  if (p[0].combostat>200) p[0].combostat=0;
}    

if (p[1].combometer!=p[1].oldcombometer) //see if there was a change in combo levels
{
 if (p[1].combometer>=2) //its a combo to be shown
  {
   if (p[1].comboline1) {free(p[1].comboline1); p[1].comboline1=0;}
   if (p[1].comboline2) {free(p[1].comboline2); p[1].comboline2=0;}   
   
   char s[30];
   sprintf(s,"%d HITS!",p[1].combometer);
   CreateFontImageSP256(font2,&p[1].comboline1,(unsigned char *)s);
   CreateFontImageSP256(font6,&p[1].comboline2,(unsigned char *)comboname[p[1].combometer]);
   if (!p[1].combostat)
    { 
      p[1].combox=32; //-imageX(p[0].comboline2)-10; }
    }
  p[1].combostat=2; //start counting
  }
 p[1].oldcombometer=p[1].combometer;  
}
if (p[1].combostat) //draw it
{
//  DrawImage(p[0].comboline1,screen,p[0].combox,60+bg.scry,0);
//  DrawImage(p[0].comboline2,screen,p[0].combox,69+bg.scry,0);
  PutImageD2(p[1].comboline1,screen+12+54*320+320-12-imageX(p[1].comboline1)+scry320,320,p[1].combox*256);
 PutImageD2(p[1].comboline2,screen+17+77*320+320-12-imageX(p[1].comboline2)+scry320,320,p[1].combox*256);  
  if (p[1].combostat>200) p[1].combostat=0;
}    
}


if (chatstat) //draw chat
 {
  char s[49];   
  DrawStringSP(font4,screen+175*320+scry320+2,(unsigned char *)"SEND:",320);
  memcpy(s,chatstr+((chatptr-48)<0 ? 0 : (chatptr-48)),48);  s[48]=0;
  DrawStringSP(font4,screen+175*320+scry320+30,(unsigned char *)s,320);
  DrawStringSP(font4,screen+175*320+scry320+30+GetStringWidthSP(font4,(unsigned char *)s),(unsigned char *)"_",320);
 }

if (chatrecvstat) //someone said something to us
 {
  int sp=0;
  char s[55];
  char *dest=screen+55*320+scry320+15;
  while (chatrecvstr[sp])
   {
    int lp=0;
    while (isspace(chatrecvstr[sp])) sp++;
    for (; lp<54 && chatrecvstr[sp+lp]; lp++) s[lp]=chatrecvstr[sp+lp]; //scan to end of a line or nullchar
    if (lp==54) while (lp>0 && !isspace(s[lp])) lp--; s[lp]=0;
    DrawStringSP(font4,dest,(unsigned char *)s,320);
    sp+=lp; dest+=320*9;
  
   } 
 } 



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

  if (bg.bgfloor)
    {
     MemoryCopy(bg.bgfloor,bg.bgfloorsave,bg.bgfloorsize);
    }
  oldscrollx=-1;
  if (cdplay) PlayCDTrack(cdtracks[bgnum]);
   else
  if (musicstat) start_music(bg.music,1);

  gamestat++;
  
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
                 if (postwordtype&2) {strcpy(postwords,"DOUBLE KILL"); postse=14;}
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

if (spin)
{
 gfxNewRotate(screen+10*320+scry320, (char *)(0xA0000+10*320), uu&255);
} else
if (tilt)
{
 tiltlevel=(p[0].x-160 +p[1].x-160)*16/160-1;
 if (tiltlevel<0) tiltlevel=255+tiltlevel;
 
 gfxNewRotate(screen+10*320+scry320, (char *)(0xA0000+10*320), tiltlevel);
} else MemoryCopy((char *)(0xA0000+14*320),screen+10*320+scry320,320*175);

 UpdatePalette();

/*
if (dooptions)
 {
  OptionsScreen(1);
  timerhandler=TickUpdate;
  dooptions=0; quit=0; oldscrollx=-1;
  
  if (cdplay) PlayCDTrack(cdtracks[bgnum]);
    else if (musicstat) start_music(bg.music,1);
  bloodlevel=options->bloodlevel;
  SetTimerSpeed(80+options->gamespeed*20);
  p[0].ResetDifficulty();
 }
*/
 su++;

//RelinquishIPX(); 

} while (!quit);


if (multiplay)
{
  SendStandardPacket(TSQUIT,0);
  
  multiconnected=0; ready=0;       //flag to tell
  multimaster=1;

 ResetPacket();
 SetPacketFunc(TSCHAT,tschat);

} 


UpdatePalette();
timerhandler=0;

if (cdplay) StopCD();
 else
if (musicstat) stop_music();


   if (p[0].comboline1) {free(p[0].comboline1); p[0].comboline1=0;}
   if (p[0].comboline2) {free(p[0].comboline2); p[0].comboline2=0;}   
   if (p[1].comboline1) {free(p[1].comboline1); p[1].comboline1=0;}
   if (p[1].comboline2) {free(p[1].comboline2); p[1].comboline2=0;}   


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



   

void TickUpdate()
{
int i;
//if (!multiconnected ||
//if (ready &&
//if (!multiplay)
 uu++;

if (multiplay)
{
if (uu>packettime+500)
  {multiconnected=0; //disconnect
      ready=0;
   ResetPacket();
   SetPacketFunc(TSCONNECT,tsconnect);
  }    
if (ipxinstalled) while (RecvIPXPacket());
if (cominstalled) PacketComFunc();

if (!ready && !(uu&31))
  SendStandardPacket(TSCONNECT,multimaster);       //we wanna be master
}   
 

if (pause) return;
if (quit) return;



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
          if (cdplay) StopCD();
              else
          if (musicstat) stop_music();
          }
 }
  if (gamestat==3)  //Fade out
        {fadelevel-=1; if (fadelevel<=0)  gamestat++; } //Fade out
  if (gamestat==4)
        {
          ready=1;
          if (multiplay)
           {
//            ioSetComFunc(DefaultComTick);   
            multiconnected=0; ready=0;
            multimaster=1;
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
        if (!multimaster) {p[0].x=320-p[0].x; p[1].x=320-p[1].x;}
        
        p[0].dizzy=INITIALDIZZY; p[1].dizzy=INITIALDIZZY;
        drawpriority=0;
        scrollx=160*10/8;; slowdown=0; gamestat=-4; uu=1; su=1;

      for (i=0; i<MAXMARKS*2; i++)
       if (effects[i].en!=27) effects[i].e=NULL;

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


//if (!multiconnected)
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
if (!multiplay ) //|| multimaster)
{
 if ((p[0].x<SL || p[1].x<SL) && !(p[0].x>SR-1 || p[1].x>SR-1)) scrolldx-=1; else
 if ((p[0].x>SR || p[1].x>SR) && !(p[0].x<SL+1 || p[1].x<SL+1)) scrolldx+=1;
}
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
//if (!multiplay &&
if (newframe) AttackCheck();

if (p[0].combostat>=2) {p[0].combostat++; if (p[0].combox>1) p[0].combox--;}
if (p[1].combostat>=2) {p[1].combostat++; if (p[1].combox>1) p[1].combox--;}

//Stop action for hit
if  (stopdur>0) {stopdur--;  if (shakeup) gshakedur=50;}
else
{
if (randomteleport && !(uu&63))
         p[ (uu>>6)&1].x=random(300)+10;


//Move projectiles
if (!multiplay)
{
pz=&p[1];  z=0;
for (i=0; i<PMAX; i++) p[0].ps[i].AdvanceProjectile();
pz=&p[0];  z=1;
for (i=0; i<PMAX; i++) p[1].ps[i].AdvanceProjectile();
}

}

RefreshInput();
for (i=0; i<2; i++)
 if (p[i].ctype==1)
 {
  p[i].in.ReadButt(i);
  p[i].in.ReadPosition(p[i].d);
 }



//Move players
pz=&p[1]; z=0; p[0].Update();
pz=&p[0]; z=1; p[1].Update();


if (multiplay)
{
 if (chatrecvstat)  chatrecvstat--;
 if (!(uu&255)) {soldcm=-1; soldx=-1;}
 SendPos();
 SendFrame();
}

}


static char oldscan=0;
int ki,ka;
int olds;



void (interrupt far *oldkeybintvector)(void);

void Keyboard()
{

if (simulkey)
  {
   kbscan=simulkey;
   goto controlkey;
  }
  else kbscan=inp(0x60);

//return;


if (chatstat && !simulkey)
 {
   if (kbscan==0x1D) return;
   
   if (kbscan==1) {chatstat=0;} //quit chat
         else
   if (kbscan==14) {if (chatptr>0) {chatptr--; chatstr[chatptr]=0;} }
        else 
   if (kbscan!=28)  //if not enter
       {
           (*oldkeybintvector)();
         if (kbhit())
            {
             int x=getch();
             if (x)
               {             
                if (isprint(x) && chatptr<128)
                 {   
                  chatstr[chatptr]=x;
                  chatptr++; chatstr[chatptr]=0;
                 }
               }  else {getch();} 
            } 
       } else
       {
        ///send goes here
        SendChatPacket(chatstr);
        chatstat=0;
       }
  return;     
 }    
          

if (oldscan!=kbscan )
{
        oldscan=kbscan;

        kbint=1; //tell functions that we are in keyb handler

        //update position of keyboard controls....position
        if (!(kbstat&1) && !chatstat) //Control
        {
          p[0].in.ReadPosition(p[0].d);
          p[0].in.ReadButt(0);
          p[1].in.ReadPosition(p[1].d);
          p[1].in.ReadButt(1);
        }       

        if (!(kbscan&0x80))
        {
   
        if (!(kbstat&1))     //----------------NOT CONTROL
        {

          if (!chatstat) //not chatting
          {
            //Check key
             if (kbscan==1) quit=1;

            if (multiplay && multiconnected)   
             if (kbscan==28) {chatstat=1; chatptr=0; chatstr[0]=0; p[0].in.Reset(); p[0].in.stat=0;} //enter
             
          }
         if (debug && !playgame)
         {
          if (kbscan>=59 && kbscan<=68) doparticularbg=kbscan-59;
          if (kbscan==87) doparticularbg=10;
          if (kbscan==88) doparticularbg=11;
         }     
           
        }

        //----------------CONTROL
        if (kbstat&1 && playgame && ready) //Control
        {
          controlkey:  
          int domessage=0;

          if (kbscan==25)
                {
                 pause^=1;        //p
                 if (musicstat) playing=pause^1;
                 if (!simulkey && multiplay) SendStandardPacket(TSKEY,kbscan);
                }

          if (kbscan==20)                  //t
                if (timeleft==100) {timeleft=99; timetickdur=0; message="TIME "; domessage=2;}
                  else        {timeleft=100; message="TIME "; domessage=1;}


          if (kbscan==23) //i
             { drawstatus^=1; domessage=drawstatus+1; message="STATUS "; }


                if (debug)
                {
                  olds=speed;
                  if (kbscan==74 && speed>20)  {speed-=15; message="SLOW DOWN"; messagedur=100;} //-
                  if (kbscan==78 && speed<500)  {speed+=15; message="SPEED UP"; messagedur=100;} //+
                  if (kbscan==55)  { speed=100; message="STANDARD SPEED"; messagedur=100;}      //*
                  if (speed!=olds)
                   {SetTimerSpeed(speed);if (!simulkey && multiplay) SendStandardPacket(TSKEY,kbscan);}

                  if (kbscan==30) {airmove^=1; domessage=airmove+1; message="AIR MOVES ";}     //a
                  if (kbscan==31) {supermove^=1; domessage=supermove+1; message="SUPER MOVES ";}     //s
                  if (kbscan==33)                  //f
                          {p[0].numjoymoves=p[0].numfm; p[1].numjoymoves=p[1].numfm; message="FATALITY"; domessage=2;}

                  if (kbscan==16) {offscreen^=1; domessage=offscreen+1; message="OFFSCREEN ";}      //q
                  if (kbscan==17) {tilt^=1; domessage=tilt+1; message="TILT ";} //i
                  if (kbscan==18) {death^=1; domessage=death+1; message="DEATH MODE ";}      //e
                  if (kbscan==19) {region^=1; domessage=region+1; message="REGIONS ";}      //r
                   //20 t time on/off
                  if (kbscan==21) framecheck^=1; //y
                  if (kbscan==22) {spin^=1; domessage=spin+1; message="SPIN ";} //u
                   //23 infobar
                  if (kbscan==24) {strobe^=1; domessage=strobe+1; message="STROBE ";}//o
                   //25 pause
 
                  if (kbscan==32) {black^=1;  domessage=black+1; message="DARK ";}  //d
                  if (kbscan==35) {randomteleport^=1; domessage=randomteleport+1; message="TELEPORT ";}  //h
                  if (kbscan==37) {p[1].energy=0; messagedur=100; message="KILL!!!!";}  //k
                  if (kbscan==38) {headprojectile^=1; domessage=headprojectile+1; message="HEADS ";}//l

                  if (kbscan==44) {speedup^=1;domessage=speedup+1; message="ZOOM ";} //z
                  if (kbscan==45) {suction^=1;domessage=suction+1; message="SUCTION ";} //x
                  if (kbscan==47) {vertscroll^=1; domessage=vertscroll+1; message="VSCROLL ";}  //v
                  if (kbscan==48) {++bloodlevel; message="BLOODIER"; messagedur=100;}//b
                  if (kbscan==49) {showcombo^=1;domessage=showcombo+1; message="COMBOWORDS ";} //n
                  if (kbscan==50) {PlaySysSE(2,0 );} //m

                  if (kbscan==51) {fastp1^=1; domessage=fastp1+1; message="FASTP1 ";}//<
                  if (kbscan==52) {fastp2^=1; domessage=fastp2+1; message="FASTP2 ";}//>
                  if (kbscan==53) {takescreenshot=1; message="SHOTTAKEN"; } // /
         }    //if debug

      if (domessage)
       {
        if (!simulkey && multiplay) SendStandardPacket(TSKEY,kbscan);
        strcpy(msg,message);
        strcat(msg,domessage==1 ? "OFF" : "ON");
        message=msg;
        messagedur=100;
       }
         

 }  //if controll pressed

}

if (kbscan==0x1D) kbstat|=1;
if (kbscan==0x9D) kbstat&=~1;


if (kbscan&0x80) kbscan=0;

kbint=0;
simulkey=0;
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
        if (scrollx<0) {scrolldx+=-scrollx; scrollx=0;}
        if (scrollx>400) {scrolldx+=400-scrollx ; scrollx=400;}

    // if (multimaster) SendStandardPacket(TSSCROLL,scrollx);   
        
          {
                p[0].x-=scrolldx;
                if (!multiplay) p[1].x-=scrolldx;
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

bg.scry-=gshake;
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



//760
#define FW 760

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
x+=(FW-640)/2;


if (y+ylen<0) return;
if (y>bg.floorheight-10) return;
if (x+xlen<0 || x>FW) return;

if (x+xlen>FW) {xlen=FW-x; clip=1; }
if (x<0)         {xst=0-x; clip=1;  x=0; xlen-=xst;}
if (y<0)         {yst=-y; clip=1;  y=0; ylen-=yst; return; }
if (y+ylen>bg.floorheight) {ylen=bg.floorheight-y; clip=1; if (ylen<0) return;}

//PutImageFlip256(img,bg.bg1raw+(y-bg.bg2y)*640+x+bg.scrx*8/10,640,o);
if (!clip) PutImageFlip256(img,bg.bgfloor+x+y*FW,FW,o);
 else      PutImageFlipClip256(img,bg.bgfloor+(FW*y)+x,FW,xst,yst,xlen,ylen,o);

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

 int i;
 effect *eptr=effects;
 if (p) eptr+=MAXMARKS;

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
          if (en>=42 && en<48) eptr->dur+=random(10);
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


/*
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
int doline=(rand()<=prob);
    
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F; d+=a; i-=a; }
  else         //opaque
         {
          if (doline) MemoryCopy(d,s,a);
          d+=a; s+=a;
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
 int doline=(rand()<=prob);
 for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F; d-=a; i-=a; }
  else         //opaque
         {
          if (doline)   
            for (int b=a; b>0; b--)  { *d=*s; d--; s++;   }
          else {d-=a; s+=a;}
          i-=a;
         }
 }
 
d+=wx; d+=320;
}

}
*/

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


int playbusy=0;

//Play SOUND effect that cancels out all others
void PlayModalSoundEffect(SOUND *se)
{
 if (!playbusy)
 {
  playbusy=1;
  if (sbstat!=2 || !se->soundptr) return;
  start_sound(se, 2, 255, 0);
  playbusy=0;
 } 
}

void PlayMixedSoundEffect(SOUND *se,int priority)
{
 if (!playbusy)
 {
  playbusy=1;
  priority=priority;
  if (sbstat!=2 || !se->soundptr) return;
  start_sound(se, 1, 255,0);
  playbusy=0;
 } 
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
  
