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

extern int idletime,specialtime;
void OptionsScreen();

#define NUMGUYSOUND 17
#define REVOL 1

#define VERSION "0.78"

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

#include "tipx.h"
#include "tpacket.h"
#include "tchat.h"

#include "nogtext.h"

#define MAXMARKS 40

int pcount[8]={0,0,0,0,0,0,0,0};
int prcount[8]={0,0,0,0,0,0,0,0};

extern "C" int ipxlisten(int);

int cstat,cleft;
int roundcount=2;

int phit[2]={0,0};

//ipx packets
typedef struct connectpacket
{
unsigned char type;
unsigned data;
} connectpacket;

typedef struct playerpacket
{
unsigned char type;
unsigned short frame;
short x,y;
} playerpacket;

typedef struct ballpacket
{
unsigned char type;
short x,y;
} ballpacket;

typedef struct hitpacket
{
unsigned char type;
vector b;
} hitpacket;    

typedef struct hitack
{
unsigned char type;
} hitack;    


connectpacket connectp={0,0};
playerpacket playerp={1,0,0,0};
ballpacket ballp={2,0,0};
hitpacket   hitp;
hitack      hitackp={4};

hitack      lossp={5};
hitack      lossackp={6};

hitack      doneposep={7};
hitack      doneposeackp={8};

void PlayCinema(char *cinfile);
void ResetDifficulty();
char fuckthisshit;
netplayer *ChatScreen();
void PlaySE(unsigned char se,char type,char pl,int xc); //type 0-swish 1-hit
void PlayGame();
void Keyboard();
char *ReadFile(char *);
void TickUpdate();
void GetConfig();
void InitializeEffects();
void KillEffects();
void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p);
void FunkyFont(char *f,unsigned char a,unsigned char b);
void SelectScreen();
int TitleScreen();
//Graphics
char *font,*font2,*font3,*font4,*font5,*font6;
char *textfont[4];
char *screen,*background;

static unsigned timeout;

void ReceivePackets();


//translates pnum to bgseries
int ptobg[]={5,1,7,3,6,4,0,2};

//translates pnum to bgseries scream
int ptoscream[]={9,11,13,12,14,15,8,10};

//translates cursor to pnum
int ctop[]={0,1,3,5,6,7,4,2};

//BAckground
bground bg;
int bg3scrx;
color pal1[256],pal2[256];
int fadelevel, oldfadelevel,fadein,fadeout;
int gigerlevel=0,oldgiger=-1,dgiger;

effect effects[MAXMARKS*2];
int stickyblood=1;
int electrocute=0,electrocutedur;

//words on screen
int  messagedur,framecheck=0,drawp=1,draww=1,drawall=1,black=0;
char *message,msg[60];
int debug=0;

//Sound
int sbstat=0;
int musicstat=0;
int nodomusic=0,nodosound=0,doawe32=0;

SOUND sysse[16];
SOUND sse[2][7];

SOUND headguysound[30];

void InitHeadGuySound();
void TerminateHeadGuySound();

int drawpriority=0;
//System
volatile char kbscan;
volatile int quit=0,pause=0,kbstat=0,done=0;
volatile int kbint=0;
volatile unsigned int uu,su,fu;
void (interrupt far *oldkeybintvector)(void);
int printtime;

//Multiiplayer stuff
int multiinstalled=0; //flag to tell whether or not multiplayer capability is on 
int multiplay=0;      //flag to tell whether or not we are playing multiplayer this GAME
int multimaster=0;    //master/slave relationship
int multiconnected;   //whether or not a connection has been established in game
int ready;            //should the game proceed? always true for local 

netplayer *multiopponent=0; //our opponent for the game

char ipxmaster=0;
int ipxoffset=0; //ipx socket address
int sendhit,sendloss,senddonepose;

int doipx=0;
int dotalk=0;

//players
int gamestat,gameover,winner;
int sz,z;
int newframe;
playerstat p[2];
playerstat ball,chopper;

int picktime;
int pselect[2];
int plock[2];

int score[2];
int wins=0,losses=0;
int ballpain;

char *cnlist[]=
 {"KLUBBOR", //0
   "FETUS", //1
   "HENRY", //2
   "GURDIP", //3
   "ED", //4
   "SINAMMON", //5
   "BUDDY", //6
   "GONZOLES", //7
   "", //8
   "", //9
   "HEAD", //10

   };

//movement

void vector::FlipX()
{
p.x=(320<<16)-p.x;    
v.x=-v.x;
a.x=-a.x;
}
    
void vector::Reset(lpoint acc,lpoint dec, lpoint max)
{
 p.x=(boundmin.x+boundmax.x)/2;
 p.y=(boundmin.y+boundmax.y)/2; 
 v.x=0; v.y=0;
 a=acc; d=dec; mv=max;
 bounce=0;
}

void vector::Tick(int stat,int clip)
{
if (v.x>0) {v.x-=d.x; if (v.x<0) v.x=0;}
if (v.x<0) {v.x+=d.x; if (v.x>0) v.x=0;}
if (v.y>0) {v.y-=d.y; if (v.y<0) v.y=0;}//allways deccelerate
if (v.y<0) {v.y+=d.y; if (v.y>0) v.y=0;}

if (stat&2) v.x+=a.x;
if (stat&1) v.x-=a.x;
if (stat&8) v.y+=a.y;
if (stat&4) v.y-=a.y;

if (v.x)
{
 if (v.x>0) {if (p.x<=boundmax.x) p.x+=v.x; if (v.x>mv.x) v.x=mv.x;}  //max velocity slow down
    else    {if (p.x>=boundmin.x) p.x+=v.x; if (v.x<-mv.x) v.x=-mv.x;}  //max velocity slow down
}
if (!clip)
{
 if (p.x<boundmin.x)
   {p.x+=0x10000; if (p.x>=boundmin.x) {p.x=boundmin.x; v.x=0;}}
 if (p.x>boundmax.x)
   {p.x-=0x10000; if (p.x<=boundmax.x) {p.x=boundmax.x; v.x=0;}}
}

if (v.y)
{
 p.y+=v.y;
 if (v.y>0)
   {if (v.y>mv.y)  {v.y-=0x500; if (v.y<mv.y) v.y=mv.y;} }  //max velocity
    else
   {if (v.y<-mv.y) {v.y+=0x500; if (v.y>mv.y) v.y=-mv.y;}}  //max velocity
//if (!clip)
{ 
 if (p.y<boundmin.y)  {p.y=boundmin.y;  v.y=0;}
 if (p.y>boundmax.y)  {p.y=boundmax.y; v.y=0;}
} 
}
}


int vector::Bounce()
{
int bounced=0;
if (a.x) //if acceleration
 {v.x+=a.x;
  if (a.x<0) {a.x+=0x500; if (a.x>0) a.x=0;}
  if (a.x>0) {a.x-=0x500; if (a.x<0) a.x=0;}
 }
  

if (v.x)
{
 p.x +=v.x;

 if (v.x>0)
   {if (v.x>mv.x) v.x=mv.x;
    if (v.x>0x50000) v.x-=0x800;
       }  //max velocity
    else
   {if (v.x<-mv.x) v.x=-mv.x;
    if (v.x<-0x50000) v.x+=0x800;
    }  //max velocity
 
/* if (p.x<boundmin.x) {p.x=boundmin.x; v.x=-v.x; a.x=-a.x; bounced|=1;}
 if (p.x>boundmax.x) {p.x=boundmax.x; v.x=-v.x; a.x=-a.x; bounced|=2;}
*/
}
if (v.y)
{
 p.y+=v.y;
 if (v.y>0) {if (v.y>mv.y) v.y=mv.y;}  //max velocity
    else    {if (v.y<-mv.y) v.y=-mv.y;}  //max velocity
 
 if (p.y<boundmin.y)  {p.y=boundmin.y; v.y=-v.y; bounced|=4;}
 if (p.y>boundmax.y)  {p.y=boundmax.y; v.y=-v.y; bounced|=8;}
}

return(bounced);
}


//default movement
lpoint accel[]=
 {
   {0x0B00,0x0B00}, //k
   {0x1200,0x1200},//fetus
   {0x5D00,0x5650},//hhh
   {0x1B00,0x1B00},//gurdip
   {0x1a00,0x1a00},//ed
   {0x1B00,0x1B00},//sinammon
   {0x1100,0x1100},//buddy
   {0x1000,0x1000},//gonzo
   {0x1B00,0x1B00},
   {0x1B00,0x1B00},
   {0x1B00,0x1B00},
 };
 
lpoint deccel[]=
 {
  {0x0600,0x0600},
  {0x0700,0x0700},
  {0x5200,0x5200},//hhh
  {0x0C00,0x0C00}, //gurdip
  {0x1300,0x1300},//ed
  {0x0C00,0x0C00},//sin
  {0x0200,0x0200}, //buddy
  {0x0500,0x0500},
 };
 
lpoint maxvelocity[]=
 {
  {0x10000,0x10000},
  {0x20000,0x20000},
  {0x16000,0x16000}  ,
  {0x1A000,0x1A000},
  {0x12000,0x12000}, //ed
  {0x10000,0x1B000}, //sin
  {0x14000,0x14000},//buddy
  {0x1B000,0x1B000}, //gonzo
 };

lpoint boundminimum[]=
 {
   {(-27<<16),(40<<16)},
   {(-12<<16),(20<<16)},
   {(-35<<16),(20<<16)}, //hhh
   {(-18<<16),(20<<16)},
   {(-24<<16),(20<<16)}, //ed
   {(-18<<16),(20<<16)}, //sin
   {(-29<<16),(20<<16)}, //buddy
   {(-8<<16),(20<<16)}, //gonzo
   {(-18<<16),(20<<16)},
 } ;

lpoint boundmaximum[]=
 {
   {(-12<<16),(180<<16)},
   {(-5<<16),(190<<16)},
   {(-10<<16),(190<<16)},//hhh
   {(-2<<16),(190<<16)}, //gr
   {(-2<<16),(190<<16)}, //ed
   {(-2<<16),(190<<16)},//sin
   {(-9<<16),(190<<16)},//buddy
   {(155<<16),(190<<16)}, //gonzo
   {(-2<<16),(190<<16)},
 } ;
 

//amount gained per tap, amount lost per bash, super=128
int penergy[][3]=
 {
  {30,20,120},
  {30,20,40},
  {30,25,25}, //hhh
  {30,25,25},  //gurdip
  {30,25,25}, //ed
  {30,25,25}, //sin
  {30,35,40},//buddy
  {30,10,25}, //gonzo

  {30,25,25},
  {30,25,25},
  {30,25,25},
 };


//Misc
unsigned bytes;
int h;
cfg *options;
int i,j,k;

int random(int x)
{
 return((x*rand())/32768);
}

#define BW 64

int barsize[2];

void DrawBar0()
{
int len=barsize[0]/2;
char *dest=screen+25+7*320;

memset(dest,0,BW-len); //draw black
memset(dest+BW-len,2,len); //draw colored
dest+=320;
memset(dest,0,BW-len); //draw black
memset(dest+BW-len,2,len); //draw colored
dest+=320;
memset(dest,0,BW-len); //draw black
memset(dest+BW-len,2,len); //draw colored

}    

void DrawBar1()
{
int len=barsize[1]/2;
char *dest=screen+320-25-BW+7*320;

memset(dest,2,len); //draw colored
memset(dest+len,0,BW-len); //draw black
dest+=320;
memset(dest,2,len); //draw colored
memset(dest+len,0,BW-len); //draw black
dest+=320;
memset(dest,2,len); //draw colored
memset(dest+len,0,BW-len); //draw black

}    



//*************************
char *doplaycin=0;

void main(int argc, char *argv[])
{
    
printf("\n      Noggin Knockers 2 Alpha V%s\n",VERSION);
printf("      Copyright 1996 Bloodlust Software \n\n");

for (i=1; i<argc; i++)
 {
  if (!stricmp(argv[i],"nomusic"))
    { printf("-music disabled\n"); nodomusic=1;}
  if (!stricmp(argv[i],"nosound"))
   {printf("-digital audio disabled...\n");  nodosound=1;}
  if (!stricmp(argv[i],"ipx"))  doipx=1;
  if (!stricmp(argv[i],"talk"))  dotalk=1;
  if (!stricmp(argv[i],"bimalkpaul"))  ipxmaster=1;

  if (!stricmp(argv[i],"playcin"))
     {doplaycin=argv[i+1];}
  if (!stricmp(argv[i],"strode"))     {debug=1;}
  
 }
GetConfig();

//randomize from timer
srand(*((unsigned int *)(0x6c+0x400)) );
/*
if (doipx)
 {
  printf("-initializing ipx...");
  if (!InitIPX()) printf("successful.\n");
   else {printf("failed\n"); return;}
  multiinstalled=1;
  multiopponent=0;
 
 }*/


//Initialize sb stuff
sbstat=0;
if (!nodosound)
if (options->sbon)
 {
  cprintf("-initializing sb voice....");
  sbstat=2;
  if (!init_sb(options->sbport, options->sbirq, options->sbdma, options->sbdma16))
         {
                printf("\nERROR:  Error initializing sound card!\n");
                printf("Check your soundblaster settings\n");
                return;
         }
  init_mixing();
  printf("done\n");
  set_sound_volume(32*options->soundvolume+31);

 }

//Initialize Timer
cprintf("-initializing timer...");
InitializeTimers();
SetTimerSpeed(100);
printf("done\n");

//initialize sb music
/*
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
    set_music_volume(32*options->musicvolume+31);

   } 
}
 */


//set options

//set up input devices
cprintf("-initializing input devices...");
p[0].in.Init(0); p[1].in.Init(1);
printf("done\n");


cprintf("-initializing keyboard...");
InitKeyboard((void ( *)())Keyboard);
printf("done\n");


cprintf("-allocating screen buffers...");
background=(char *)malloc(65000);
screen=(char *)malloc(100000);
screen+=20000;
printf("done\n");

cprintf("-initializing fonts...");
font=ReadFile("font.vol");    //ugly blue font
font2=ReadFile("font2.vol");  //big red font
font3=ReadFile("font.vol");  
font4=ReadFile("font3.vol"); //peachy font
font5=ReadFile("font3.vol"); //white font for cinematics
FunkyFont(font5,0xca,0xff);
FunkyFont(font5,0xdb,0x0);

font6=ReadFile("font3.vol"); //gold looking font for screen
FunkyFont(font6,0xca,0xcd);
FunkyFont(font6,0xdb,0xdf);


textfont[1]=ReadFile("font3.vol"); //icer font
//FunkyFont(textfont[1],0xca,8*16+8       );
FunkyFont(textfont[1],0xca,8*16+9       );
FunkyFont(textfont[1],0xdb,0);
textfont[2]=ReadFile("font3.vol"); //ethan font
FunkyFont(textfont[2],0xca,0xff);
FunkyFont(textfont[2],0xdb,0x0);
textfont[3]=ReadFile("font3.vol"); //stumpy font
FunkyFont(textfont[3],0xca,3*16+2);
FunkyFont(textfont[3],0xdb,0);
printf("done\n");

cprintf("-initializing effects...");
InitializeEffects();
printf("done\n");

//cprintf("-initializing head sound effects...");
//InitHeadGuySound();
//printf("done\n");

cprintf("-initializing head...");

ball.Initialize(10,0);
printf("done\n");
chopper=ball;
ball.ctype=4; //ball is different than everything
chopper.ctype=5;

p[0].po=&p[1]; p[1].po=&p[0];

roundcount=options->roundcount+3;
//roundcount=2;

Mode256();

if (doplaycin)
  {
  PlayCinema(doplaycin);
  goto fuckthis;
  }

convptr=0; //no conversation yet

PlayCinema("logo.vol");
PlayCinema("ncred.vol");

//full loop
do
{

int choice;
do
{
choice=TitleScreen();
//choice=0;
if (choice==-1) goto fuckthis;

//printf("done with titlescreen\n");
if (choice==0)
{ //1 player
 p[0].ctype=1; p[1].ctype=3;
}

if (choice==1)
{//2 player
 p[0].ctype=1; p[1].ctype=1;
}

if (choice==2)
{
 OptionsScreen();
} 
} while(choice==2);



/*if (multiinstalled)
{
 multiopponent=ChatScreen();
 if (!multiopponent)  goto fuckthis;
 multiplay=1; p[1].ctype=2; //remote player

 IPXGameMode((char *)&multiopponent->a.nodeadd,ipxoffset);  //set up ipx for games
}*/


cstat=0;
cleft=8;

pselect[0]=0; pselect[1]=3;
plock[0]=0; plock[1]=0;

//tournament loop
do
{
quit=0;


//if (!multiinstalled)  //select the characters
  SelectScreen();
if (quit) {quit=0; break;}

p[0].Initialize(ctop[pselect[0]],0);  //load the characters
p[1].Initialize(ctop[pselect[1]],0);


bg.ReadBackground("nogginbg.vol",0);

ResetDifficulty();
score[0]=score[1]=0;
PlayGame();
StopVoice();

bg.KillBackground();

p[1].Kill();
p[0].Kill();


if (quit==2) //somebody won
 {
  if (p[winner^1].ctype==1) //human lost
   {  }
  else  //computer lost
   {plock[0]=1; plock[1]=0;
    cstat|=(1<<p[winner^1].type);
    cleft--;
   }

  plock[winner^1]=0; 
  plock[winner]=1;
 }

if (cleft==0)
{
 char s[30];
 sprintf(s,"end%d.vol",p[0].type+1);
 PlayCinema(s);
 quit=1; //play ending
}  

} while (quit!=1); //continue till esc is pushed

/*if (multiinstalled) //go back to chatscreen
{
  quit=0;
  if (score[0]>score[1]) wins++;
 if (score[0]<score[1]) losses++;
} */     

quit=0;

} while (!quit && !dotalk);

fuckthis:


ModeText();

printf("Copyright (C) 1996 Bloodlust Software. All rights Reserved\n");

ball.Kill();

cprintf("-terminating effects...");
KillEffects();
printf("done\n");

cprintf("-terminating keyboard...");
TerminateKeyboard();
printf("done\n");

/*
if (musicstat==2)
 {
  cprintf("-terminating music...");
  done_music();
  printf("done\n");  
 }
*/

cprintf("-terminating timers...");
TerminateTimers();
printf("done\n");

TerminateGrip();


if (sbstat==2)
{
  cprintf("-terminating sb voice...");
  shutdown_mixing();
  shutdown_sb();
  printf("done\n");
}

if (ipxinstalled)
{
 cprintf("-terminating ipx...");
 TerminateIPX();
 printf("done\n"); 
} 

//printf("%d\n",fuckthisshit);

}

//*******

void DrawText()
{
if (!draww) return;
if (quit) return;
if (!options->talking) return;

if (!convptr) //no conversation selected yet
 {
  convptr=&conversations[0]; //start at first conversation
  currconv=0; //0th conversation
  numconvdone=0; //no conversations done

  for (int i=0; conversations[i]; i++)  convdone[i]=0;
  numconv=i; //get total number of conversations
  textptr=0;               //nothing spoken yet
  parsetext=1;             //parse new text
 }

if (textptr) //there is some text to be written
 {
  if (person) //if it is not a delay..
   {
    char *dest=textdest;
    for (int line=0; textline[line][0]; line++,dest+=9*320)
      DrawStringSP(textfont[person],dest+textcenter[line],(unsigned char *)textline[line],320);
   }
  if (uu>texttimeout) parsetext=1; //do next text
 }


if (parsetext) //parsenew text
 {
  parsetext=0;
  if (!textptr)  //if no conversation started yet
    textptr=*convptr; //first of conversation
   else         //conversation in progress...
      textptr++; //go to next text
    
  person=textptr->person; //person talking
  if (person==DONE)  //done with this conversation
     {
      if (gamestat==4) {person=DELAY; texttimeout=0xFFFFFFFF; return;}   
      int i=0;
      numconvdone++; //one more conversation done
      convdone[currconv]++; //we did this conversation
      textptr=0; parsetext=1;  //restart this conv up
      
      if (numconvdone>=numconv) //if we've done them all
           { convptr=0; currconv=0;}  //restart
        else     //find a not done conversation
         do
         {
          currconv=random(numconv); i++;
         } while(convdone[currconv] && i<20);
      convptr=&conversations[currconv];
      return;  
     }

      //go to next conversation
  if (person!=DELAY) //if we aren't delaying, then parse the next
    {
     int line=0;
     char *s=textptr->text; //beggining of text
     int words=0;
     
     while (*s && line<9)
     {
      char *d=textline[line]; //dest line

      while (isspace(*s)) s++;  //scan till first character
      for (int lw=0; lw<textwidth[person] && *s; s++) //scan to end of a line or nullchar
        if ( !(isspace(*s) && isspace(*(s-1))) )
         {
            *d=*s;  //store each char of line
            if (isspace(*d)) words++; //count words
            lw+=GetLetterWidth(textfont[person],(unsigned char )*s); //increase width of line
            d++;
         }  

      if (*s)
       while (lw>0 && !isspace(*s)) //backtrack to last word
         {lw-=GetLetterWidth(textfont[person],(unsigned char )*s); *d=0; d--; s--;}
       *d=0;
      textcenter[line]=textpos[person].x-lw/2; 
      line++; //next line   
     } 
     textline[line][0]=0;
        
     textdest=screen+textpos[person].y*320;
     if (textptr->delay) texttimeout=textptr->delay;      else
      {
       texttimeout=55*words;  //delay time for text
       if (texttimeout<400) texttimeout=300;
      }
//      texttimeout/=10;
     texttimeout+=uu;
    }  else texttimeout=uu+textptr->delay;  //delay time 
 } 


}    

//**********************************

lpoint start[]={ {-15<<16,100<<16}, {330<<16,100<<16}, {160<<16,100<<16} };
lpoint ballinit[]={{0,0},{0x0001,0x0001},{0xA0000,0x20000},{-30<<16,8<<16},{350<<16,192<<16}};

void PlayGame()
{
barsize[0]=barsize[1]=0;    
for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;
p[0].Reset(); p[1].Reset();
chopper.Reset();
ball.Reset(); ball.drawme=0;


p[0].x=-20;  p[1].x=340;  ball.x=160;
p[0].y=100; p[1].y=100;   ball.y=100;
p[0].d= 0;  p[1].d=1;     ball.d=0;

p[0].v.boundmin=boundminimum[p[0].type]; p[0].v.boundmax=boundmaximum[p[0].type];
p[0].v.Reset(accel[p[0].type],deccel[p[0].type],maxvelocity[p[0].type]);

p[1].v.boundmin=boundminimum[p[1].type]; p[1].v.boundmax=boundmaximum[p[1].type];
p[1].v.boundmin.x=(320<<16)-boundmaximum[p[1].type].x;
p[1].v.boundmax.x=(320<<16)-boundminimum[p[1].type].x;
p[1].v.Reset(accel[p[1].type],deccel[p[1].type],maxvelocity[p[1].type]);

ball.v.boundmin=ballinit[3]; ball.v.boundmax=ballinit[4];
ball.v.Reset(ballinit[0],ballinit[1],ballinit[2]);

p[0].in.Reset(); p[1].in.Reset(); 
newframe=0;

done=0;
kbscan=0; quit=0;
oldfadelevel=-1; fadelevel=0; fadein=1; fadeout=0;
UpdatePalette();

electrocute=0;
gamestat=-2; //pregame
gameover=0;


//multplayer shit
if (!dotalk) ready=1;                         //flag to tell whether or not to proceed in game
multimaster=1;
if (multiplay)      //flag that tells whether or not we are playing multiplayer
 {
  multiconnected=0; ready=0;       
  multimaster=1;
  packetrecvs=packetsends=packeterror=0;
  sendhit=0;
 } 


su=1; uu=1;
bg.scrx=160; bg3scrx=0;

//textptr=0;
texttimeout=0;

SetTimerSpeed(100);
timerhandler=TickUpdate;

do
{
if (p[0].ctype==1) p[0].in.ReadPosition(0);
if (p[1].ctype==1) p[1].in.ReadPosition(0);


//before the game...
//wait until ready (computer's synched)
//start up chopper, make sure ball is erased
if (gamestat==-2)
  if (ready)
   {
    chopper.Reset(); ball.drawme=0;
    chopper.d=random(2);
    if (chopper.d) chopper.x=160-random(80);
              else chopper.x=160+random(80);
    if (multiplay)
      {chopper.d=multimaster; chopper.x=160;
      }             
    chopper.y=250;
    ball.x=160; //look towards center
//    if (sbstat)
//   start_sound(headguysound[random(17)],10);
//       start_sound(&headguysound[random(17)], 2, 255, 0);
    gameover=0;
    gamestat++;
   } 

//wait in this state until ball has been released
if (gamestat==-1) //pregame
 {
 }

//the game is actually playing
//gamestat==0

//this is the state after both players have done their win/loss pose
if (gamestat==3)
 {
   if (score[0]>=roundcount)
    {
     gamestat++; winner=0;
     convptr=&winconversations[random(NUMWINCONV)];
     parsetext=1; textptr=0; currconv=0;

     if (p[1].ctype==3 && cleft==1)  //human won all game!
       convptr=&endingtextptr;

     
    } else
   if (score[1]>=roundcount)
    {
     gamestat++; winner=1;
    }
    else gamestat=-2;

    if (gamestat==4)
     if (p[winner].ctype==3) //if winner is computer
      {
       convptr=&loseconversations[random(NUMLOSECONV)];
       parsetext=1; textptr=0; currconv=0;
      }  else
      {
       convptr=&winconversations[random(NUMWINCONV)];
       parsetext=1; textptr=0; currconv=0;
      }      
     
   kbscan=0; ball.d=0;
 }

if (!black)
{

if (drawall)
{
//bg.DrawBackground(screen);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen,bg.scrx, 0, 0, gigerlevel/4  ,  bg3scrx,  0,  0,   0, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,screen,bg.scrx, 0, 0, 100+gigerlevel/4, bg3scrx, 100,  0,  100, bg.ylist1b, bg.ylist2);
bg.DrawFloorBG(screen,160);
}
//bg.DrawBGImages(screen);




//draw background movement things
if (electrocute!=1)
{
if (!(p[0].in.stat&(12))) //centered
  bg.DrawBGSeries(screen,0);
 else
  if (p[0].in.stat&8) bg.DrawBGSeries(screen,2); else
   bg.DrawBGSeries(screen,4);

if (person==1) //icer talk
 {
  if (gameover) bg.DrawBGSeries(screen,12);
   else
   {
  if (ball.x<70) bg.DrawBGSeries(screen,14);   else
  if (ball.x>140) bg.DrawBGSeries(screen,16); else
      bg.DrawBGSeries(screen,12);
   }
 } else
 {             //icer no talk
  if (!gameover)
  {
  if (ball.x<70) bg.DrawBGSeries(screen,8);   else
  if (ball.x>140) bg.DrawBGSeries(screen,10);
  }
 }
} else
{
 bg.DrawBGSeries(screen,6);
 bg.DrawBGSeries(screen,24);
}

if (electrocute!=2)
{
if (!(p[1].in.stat&(12))) //centered
  bg.DrawBGSeries(screen,1);
 else
  if (p[1].in.stat&8) bg.DrawBGSeries(screen,3); else
   bg.DrawBGSeries(screen,5); 

if (person==2) //ethan talk
 {
  if (gameover) bg.DrawBGSeries(screen,13); else
    { 
  if (ball.x>320-70) bg.DrawBGSeries(screen,17); else   
  if (ball.x<320-140) bg.DrawBGSeries(screen,15); else
   bg.DrawBGSeries(screen,13);
    }
 } else  //ethan no talk
 {
  if (!gameover)
  {
  if (ball.x>320-70) bg.DrawBGSeries(screen,11);   
  if (ball.x<320-140) bg.DrawBGSeries(screen,9);
  }
 }
} else
{
  bg.DrawBGSeries(screen,7);
  bg.DrawBGSeries(screen,25);  
}

if (!electrocute)
if (person==3) //stump talk
{
  if (ball.x>190) bg.DrawBGSeries(screen,23); else   
  if (ball.x<120) bg.DrawBGSeries(screen,22); else
   bg.DrawBGSeries(screen,21);
} else //no stump talk
{
  if (ball.x>190) bg.DrawBGSeries(screen,20); else   
  if (ball.x<120) bg.DrawBGSeries(screen,19); else
   bg.DrawBGSeries(screen,18);
}


} else
Solid(screen,64000,0);


char s[30];
//draw behind effects

if (drawall)
{

for (effect *eptr=&effects[0]; eptr<&effects[MAXMARKS]; eptr++) eptr->Draw();

if (drawp)
if (ready)
{
if (chopper.drawme) chopper.DrawFrame();

if (!ball.freeze) if (ball.drawme) ball.DrawFrame();

if (drawpriority)
{
p[0].DrawFrame();
p[1].DrawFrame();
} else
{
p[1].DrawFrame();
p[0].DrawFrame();
}    

if (ball.freeze) if (ball.drawme) ball.DrawFrame();

projectilestat *pp;
sz=0; for (pp=&p[0].ps[0]; pp<&p[0].ps[PMAX]; pp++) pp->DrawProjectile();
sz=1; for (pp=&p[1].ps[0]; pp<&p[1].ps[PMAX]; pp++) pp->DrawProjectile();
}



//draw noggin text
if (!black) DrawText();

if (pause) DrawString(font2,screen+90*320+85,(unsigned char *)"GAME PAUSED",320);

//we are displaying win/lose conversation
if (gamestat==4)
 {
   char s[50];
   sprintf(s,"%s WINS!",cnlist[p[winner].type]);
   DrawString(font2,screen+34*320+160-GetStringWidth(font2,(unsigned char *)s)/2,(unsigned char *)s,320);
   strcpy(s,"PRESS BUTTON TO CONTINUE");
   if (uu&32)   DrawStringSP(font5,screen+56*320+160-GetStringWidthSP(font6,(unsigned char *)s)/2,(unsigned char *)s,320);
   if ((p[0].in.but) || (p[1].in.but)) quit=2;
 }


//draw front effects
for (eptr=&effects[MAXMARKS]; eptr<&effects[MAXMARKS*2]; eptr++) eptr->Draw();
}


if (framecheck) //framecheck status
{
if (multiplay)
{
if (multiconnected) DrawStringSP(font4,screen+155*320+0,(unsigned char *)"CONNECTED",320);
if (ready) DrawStringSP(font4,screen+165*320+0,(unsigned char *)"READY",320);   
if (multimaster) DrawStringSP(font4,screen+175*320,(unsigned char *)"MASTER",320);
   else DrawStringSP(font4,screen+175*320,(unsigned char *)"SLAVE",320);
sprintf(s,"PS%5d",packetsends);
DrawStringSP(font4,screen+50*320+250,(unsigned char *)s,320);
sprintf(s,"PR%5d",packetrecvs);
DrawStringSP(font4,screen+65*320+250,(unsigned char *)s,320);
sprintf(s,"PE%5d",packeterror);
DrawStringSP(font4,screen+80*320+250,(unsigned char *)s,320);

sprintf(s,"BYTES%5d",pbytes);
DrawStringSP(font4,screen+95*320+250,(unsigned char *)s,320);

sprintf(s,"%5d BPS",pbytes*speed/uu);
DrawStringSP(font4,screen+106*320+250,(unsigned char *)s,320);

for (int i=0; i<8; i++)
 {
sprintf(s,"%05d:%d:%05d",pcount[i],i,prcount[i]);
DrawStringSP(font4,screen+i*9*320,(unsigned char *)s,320);
        
 }
 
}

//sprintf(s,"%d BallY %d sendhit %d",gamestat,ball.v.p.x,ball.v.p.y,sendhit);
//DrawStringSP(font4,screen+90*320,(unsigned char *)s,320);
sprintf(s,"CM%d CF%d NF%d s%d sdur%d idletime%d st%d",p[0].cm,p[0].currframe,p[0].numframes,p[1].strategy,p[1].sdur,idletime,specialtime);
DrawStringSP(font4,screen+90*320,(unsigned char *)s,320);
//sprintf(s,"p0energy%d p1energy%d",p[0].energy,p[1].energy);
//DrawStringSP(font4,screen+100*320,(unsigned char *)s,320);

 sprintf(s,"%5d FPS",su*speed/uu);
DrawStringSP(font4,screen+135*320+250,(unsigned char *)s,320);

screen[319+p[1].desty*320]=4;

}
// sprintf(s,"p0hit%d p1hit%d",phit[0],phit[1]);
//DrawStringSP(font4,screen+155*320+200,(unsigned char *)s,320);


//IPX STUFF*******************************************
//do com connection

if (multiplay)
{
if (!ready)
 DrawString(font2,screen+5+80*320,(unsigned char *)"WAITING FOR OPPONENT",320);

//RelinquishIPX();

}

sprintf(s,"%02d",score[0]);
DrawStringSP(font2,screen+12*320+37,(unsigned char *)s,320);
sprintf(s,"%02d",score[1]);
DrawStringSP(font2,screen+12*320+320-37-GetStringWidth(font2,(unsigned char *)s),(unsigned char *)s,320);
DrawBar0();
DrawBar1();


UpdatePalette();
//MemoryCopy((char *)(0xA0000+2*320),screen+2*320,320*192);

MemoryCopy((char *)(0xA0000),screen,320*200);

if (quit) fadeout=1;




su++;

} while (!done);

if (gamestat==4)
 {
     textptr=donetext;
     parsetext=1; person=DONE;
     gamestat=5;
     quit=2;
 }


multiplay=0;
if (multiinstalled)
{
  WaitIPXPacket();
  ResetIPX();
}  

timerhandler=0;

}    




void DrawName(int n)
{
n=n;
//DrawStringSP(font4,screen+(3+(n&3)*80+40)+320*(1+(n>>2)*100+90)-GetStringWidthSP(font4,(unsigned char *)clnlist[n])/2,(unsigned char *)clnlist[ctop[n]],320);
/*DrawString(font2,screen+x+122*320-GetStringWidth(font2,(unsigned char *)cnlist[n])/2,(unsigned char *)cnlist[n],320);  
if (n==2)
 DrawStringSP(font4,screen+x+117*320-65,(unsigned char *)"Hard Hat",320);  
if (n==4)
 DrawStringSP(font4,screen+x+117*320-55,(unsigned char *)"Cannibal",320);  
if (n==5)
 DrawStringSP(font4,screen+x+117*320-55,(unsigned char *)"Mistress",320);  

if (n==6)
 DrawStringSP(font4,screen+x+137*320-5,(unsigned char *)"the Tard",320);  
if (n==7)
 DrawStringSP(font4,screen+x+137*320-25,(unsigned char *)"the Wonder Midget",320);  */

 
}    


SOUND scream[8][4];
int screamer=0,screamerdur=500;
int oldscreamer=-1;
int doscream=-1;


void PlayScream()
{
int loop=30;
int x;

if (cleft==0) return;
if (doscream==-1)
do
{
 x=random(8);
} while ( (cstat&(1<<x)) || (cleft>1 && oldscreamer==x) );// && --loop>0);
else x=doscream;

doscream=-1;
screamer=x;

if (!sbstat) return;

loop=60;
do
{
  x=random(3);
  loop--;
} while(!scream[screamer][x].soundptr && loop>0);

if (loop!=0)
 start_sound(&scream[screamer][x], 10, 255, 0);
else {screamer=-1; }

}
    
void SelectTick()
{
uu++;

picktime--;

if (screamer==-1) //no one screaming
 if (screamerdur>0) screamerdur--;   //count down to next scream
 else  { screamerdur=200; PlayScream();} //make somoen scream
else //someone screaming
 if (sbstat)
  {if (!sound_playing(10)) {oldscreamer=screamer; screamer=-1; screamerdur=90+random(80);}}
 else
  if (screamerdur>0) screamerdur--; //count down to done scream
   else {oldscreamer=screamer; screamer=-1; screamerdur=50+random(50);}
  

//fading in and out
if (fadein)
 {if (fadelevel<32) fadelevel++; else fadein=0;}
else
if (fadeout)
 {if (fadelevel>0) fadelevel--; else done=1;}
gigerlevel=uu&63;
if (uu&64) {gigerlevel^=63; }
gigerlevel>>=1;

//scroll background
bg3scrx+=1;
if (bg3scrx>320) bg3scrx=0;


series *bp;  //bg.nums
int i;
bp=&bg.bgs[0];   for (i=0; i<=bg.nums; i++,bp++)  bp->Tick();

}    
extern int lastsize;
char *ReadVolFile(int h);

int ReadRawFile(char **m,int h);

char *smark[2];

void drawsmark(int p)
{
 DrawImage(smark[p],screen,3+(pselect[p]&3)*80,1+(pselect[p]>>2)*100,0);
}    

/*
char *cnlist[]=
 {"KLUBBOR", //0
   "FETUS", //1
   "HENRY", //2
   "GURDIP", //3
   "ED", //4
   "SINAMMON", //5
   "BUDDY", //6
   "GONZOLES", //7
*/

void GreyImage(char *s)
{
int wx,wy; //width and height of image
int k;
wx=(int)(unsigned char)s[1];
wy=(int)(unsigned char)s[2];

s+=4+wy+wy; ///s points to image

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F;  i-=a; }
  else         //opaque
         {
          for (int b=a; b>0; b--)
                 {
                  k=(pal1[*s].r+pal1[*s].g+pal1[*s].b)*16/(256*3);
                  *s=31-5-k;
                  s++;
                 }
                i-=a;
         }
    }
   }
}


void GreySeries(int i)
{
 series *bp=&bg.bgs[i];
 if (!bp->numframes) return;

bgframe *f=(bgframe *)(bg.bgframes+bg.bgs[i].firstframe);   ;
for (int j=0; j<bp->numframes; j++)
 { GreyImage(bg.bgimagelist[f->index]);
     f++;}
}    
void Nofile(char *);
void SelectScreen()
{
int i;
int h;

//ModeText();



//read smarks
//printf("reading select2...\n");
if (_dos_open("select2.vol",O_BINARY|O_RDONLY,&h))
 Nofile("select2.vol");
 
/* {  //revol it
  _dos_creat("select2.vol",0,&h);
   char *t;
   unsigned lastsize;
   int th;

   _dos_open("TEMP\\s1.raw",O_RDONLY|O_BINARY,&th);
   lastsize=ReadRawFile(&t,th); _dos_close(th);
   _dos_write(h,&lastsize,4,&bytes);
   _dos_write(h,t,lastsize,&bytes);
   free(t);

   _dos_open("TEMP\\s2.raw",O_RDONLY|O_BINARY,&th);
   lastsize=ReadRawFile(&t,th); _dos_close(th);
   _dos_write(h,&lastsize,4,&bytes);
   _dos_write(h,t,lastsize,&bytes);
   free(t);
    _dos_close(h);
    _dos_open("select2.vol",O_BINARY|O_RDONLY,&h);
 }*/

//printf("reading smark[0]...\n");
smark[0]=ReadVolFile(h);
//printf("reading smark[1]...\n");
smark[1]=ReadVolFile(h);
_dos_close(h);

//printf("done.\n");

//read vocs
//cprintf("reading select3...");
SOUND voc1,voc2; 
if (_dos_open("select3.vol",O_BINARY | O_RDONLY,&h))  Nofile("select3.vol");
int size;
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);
  lseek(h,256*3,SEEK_CUR);
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);
  ReadVolSound(h,&voc1);
  ReadVolSound(h,&voc2);
  _dos_close(h);
//printf("done.\n");


//cprintf("reading select...");
bg.ReadBackground("select.vol",0);
//printf("done\n");
//for (i=1; i<100; i++) bg.ylist1a[i]=bg.ylist1a[0];

//cprintf("clearing bgfloor...");
memset(bg.bgfloor,0,bg.bgfloorsize);
//printf("done\n");

//ModeText();
if (sbstat)
for (int i=0; i<8; i++)
  {
   char s[20];
   strcpy(s,cnlist[i]);
   strcat(s,".VOL");
 

   for (int j=0; j<4; j++)
     scream[i][j].soundptr=0;

   if (!_dos_open(s,O_RDONLY | O_BINARY,&h))
      {
           unsigned size;
          lseek(h,12,SEEK_CUR);
          //ReadVolSound(h,&vocname[i]);
          _dos_read(h,&size,4,&bytes);
          lseek(h,size,SEEK_CUR);
          
          for (j=0; j<4; j++)
            ReadVolSound(h,&scream[i][j]);
         _dos_close(h);
      }
  //   printf("read sound %s\n",s);
//   printf("%d: %X %X %X %X\n",i,scream[i][0].soundptr,scream[i][1].soundptr,scream[i][2].soundptr,scream[i][3].soundptr);
  }

//cstat=0xFF; cleft=0;
for (i=0; i<8; i++)
  if (cstat&(1<<i))
    {GreySeries(ptobg[i]);   GreySeries(ptoscream[i]);}





picktime=20;
int pskip[2];
pskip[0]=8+random(10);
pskip[1]=8+random(10);


//printf("starting select...(hit any key)\n");
//kbscan=0; while (!kbscan);
//Mode256();

done=0;
kbscan=0; quit=0;
oldfadelevel=-1; fadelevel=0; fadein=1; fadeout=0;
UpdatePalette();
memset(screen,0,64000);
memset((char *)0xa0000,0,64000);

SetTimerSpeed(100);
su=1; uu=1;
bg.scrx=0; bg3scrx=0;
p[0].d=p[1].d=0;
p[0].in.stat=0; p[1].in.stat=0;
int ostat[2];

screamer=-1;
screamerdur=80;

if ((p[0].ctype==3 && !plock[0])|| (p[1].ctype==3 && !plock[1])) screamerdur=3000;
if (p[0].ctype==1 && plock[0]) picktime=150;

timerhandler=SelectTick;

do
{

RefreshInput();
 for (i=0; i<2; i++)
  if (!plock[i])
   if (p[i].ctype==1) //if not locked on yet and we are local controlled...
         {
          //Move cursor around
          p[i].in.ReadPosition(0);
          p[i].in.ReadButt(i);
          if (p[i].in.stat!=ostat[i])    //If control has moved
                {
                 int s=p[i].in.stat; s&=~ostat[i];
                 int och=pselect[i];
                 if (s&1) pselect[i]=(pselect[i]&4)|((pselect[i]-1)&3);
                 if (s&2) pselect[i]=(pselect[i]&4)|((pselect[i]+1)&3);
                 
                 if (s&4) pselect[i]&=3;  //Up
                 if (s&8) pselect[i]|=4; //down
                  //if they pushed a button (and it exists)
                 if (s&(16+32))
                  {plock[i]=1; PlayMixedSoundEffect(&voc2,8);}
                 ostat[i]=p[i].in.stat;
                 if (och!=pselect[i])  PlayMixedSoundEffect(&voc1,8);
                }

          p[i].in.stat&=0xF;
         }
  else
  if (p[i].ctype==3) //computer
   if (picktime<0)
   {
     picktime=10;
     pselect[i]++;
     pselect[i]&=7;
     if (--pskip[i]<=0 && !(cstat&(1<<ctop[pselect[i]])) )
      {
       plock[i]=1;  //PlayMixedSoundEffect(&voc2,8);
//       start_sound(&voc2, 1, 40, 0);
//       screamer=ctop[pselect[i]]; screamerdur=50;
   screamerdur=50;
   doscream=ctop[pselect[i]];
//       if (sbstat)// && scream[screamer][0].soundptr)
//         start_sound(&scream[screamer][0], 10, 255, 0);
      }
     else //PlayMixedSoundEffect(&voc1,8);
      if (sbstat) start_sound(&voc1, 1, 40, 0);
   }
         

PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen,0, 0, gigerlevel/2, 0 /*gigerlevel/4 */ ,  bg3scrx,  0,  0,   0, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,screen,0, 0, gigerlevel/2, 100 /*+gigerlevel/4  */, bg3scrx, 100,  0,  100, bg.ylist1b, bg.ylist2);
bg.DrawFloorBG(screen,160);
//memset(screen,0,64000);

//DrawName(80,pselect[0]);
//DrawName(80+160,pselect[1]);

for (i=0; i<8; i++) //cycle through all pnums
 if (i!=screamer) bg.DrawBGSeries(screen,ptobg[i]);

if (plock[0])
 {
  drawsmark(0);
  if (plock[1] || (uu&8)) drawsmark(1);
 } else
if (plock[1])
 {
  drawsmark(1);
  if (plock[0] || (uu&8)) drawsmark(0);
 } else
 {
  if (uu&8) drawsmark(0);
   else drawsmark(1);
 }  


/*DrawName(pselect[0]);
if (pselect[0]!=pselect[1])
 DrawName(pselect[1]);
*/

if (screamer!=-1)
  bg.DrawBGSeries(screen,ptoscream[screamer]);


UpdatePalette();
MemoryCopy((char *)(0xA0000),screen,320*200);

if (plock[0] && plock[1] && screamer==-1 && doscream==-1) fadeout=1;
if (quit) fadeout=1;

su++;
} while (!done);


timerhandler=0;
StopVoice();
free(voc1.soundptr); free(voc2.soundptr);
if (sbstat) stop_sound(10);
free(smark[0]); free(smark[1]);
if (sbstat)
 {
  for (int i=0; i<8; i++)
   for (int j=0; j<4; j++)
    if (scream[i][j].soundptr) free(scream[i][j].soundptr);
 }
bg.KillBackground();
}    


void SendPackets()
{
//IPX SHIT
if (uu>timeout)
  {multiconnected=0; //disconnect
      ready=0;}    

//try and connect if we aren't connected    
if (!ready)
 {
  if (!(uu&3))
   SendIPXPacketFixed(0,(char *)&connectp,sizeof(connectp));       //we wanna be master
 }
 else
if (sendhit)
 {
//  if (sendhit==1) {CancelIPXFixed(); sendhit++;}
  hitp.type=3;
  SendIPXPacketFixed(6,(char *)&hitp,sizeof(hitp));
 }
  else
if (sendloss)
 {
//  if (sendloss==1) {CancelIPXFixed(); sendloss++;} 
  SendIPXPacketFixed(7,(char *)&lossp,sizeof(lossp));
 }
 else
if (senddonepose)
 {
//  if (senddonepose==1) {CancelIPXFixed(); senddonepose++;}
  SendIPXPacketFixed(8,(char *)&doneposep,sizeof(doneposep));
 } else  //send player stuff
 {
  SendIPXPacketFixed(1,(char *)&playerp,sizeof(playerp));
//  if (ball.d)
//   SendIPXPacketFixed(2,(char *)&ballp,sizeof(ballp));
 } 
}    


void ReceivePackets()
{
        unsigned char *fp;
//read chars...........
int x;
while ( (x=RecvIPXPacket())!=0 )
 {
   timeout=uu+500;
   char *ipd=(char *)&ip[x].data; //address of data
   prcount[ipd[0]]++;
   switch (ipd[0]) //packet type
    {
     case 0: //someone wants to connect
      multimaster=((packet *)ipd)->data^1; //we are opposite of type sent     
      multiconnected=1; ready=1;   //we are connected and ready
      if (multimaster)
      {
       connectp.data=multimaster;
       SendIPXPacketFixed(0,(char *)&connectp,sizeof(connectp));
      }
     break;
      case 1:
        p[1].x=320-((playerpacket *)ipd)->x;
        p[1].y=((playerpacket *)ipd)->y;
        fp=(((playerpacket *)ipd)->frame+p[1].frame);
        if (fp!=p[1].frameptr) //new frame!
         {
          p[1].frameptr=fp;
          p[1].GetRect();
          PlaySE( ((frametype *)fp)->se,0,1,0 );
         }
      break;
      case 2:
        if (ball.d) break; //only use outgoing ball shit
        ball.x=320-((ballpacket *)ipd)->x;
        ball.y=((ballpacket *)ipd)->y;
      break;
      case 3:  //hit thing
      //  if (!ball.d)  //only use when ball is going away
        {
         ball.v=((hitpacket *)ipd)->b; //store the vector
         ball.v.FlipX();
         ball.d=1; //coming to us now
        }
       hitackp.type=4; 
       SendIPXPacketFixed(3,(char *)&hitackp,sizeof(hitackp)); //send ack of this
      break;
      case 4: //if acknowledged hit, stop sending
       sendhit=0;
      break;
      case 5: //the other dude lost
        if (!gamestat) //if during game
          {
           p[0].cm=3; p[0].newmove=1;  //make us WIN
           score[0]++;
           gamestat=1;                 //after game...
           electrocute=2; electrocutedur=150;
           {ball.v.v.x=ball.v.v.y=0;}
          }
         lossackp.type=6;
        SendIPXPacketFixed(4,(char *)&lossackp,sizeof(lossackp)); //send ack of this
       break;
      case 6: //ack loss
        sendloss=0; //stop sending loss packets
       break;
      case 7: //the other dude is done his posing
         if (gamestat==2) //if we've done posing, then carry on
                   gamestat=3;
         doneposeackp.type=8;  
         if (gamestat>=2)
          SendIPXPacketFixed(5,(char *)&doneposeackp,sizeof(doneposeackp)); //send ack of this
       break;
      case 8:
        senddonepose=0;
       break;
    

    }
  ipxlisten(x*sizeof(ipxpacket));
 }
}    


void TickUpdate()
{
int i;
if (pause) return;

uu++;

if (multiplay && !(uu&7))
{
 SendPackets();
 RelinquishIPX();
 ReceivePackets();
}    


//fading in and out
if (fadein)
 {if (fadelevel<32) fadelevel++; else fadein=0;}
  else
if (fadeout)
 {if (fadelevel>0) fadelevel--; else done=1;}
gigerlevel=uu&63;
if (uu&64) {gigerlevel^=63; }
gigerlevel>>=1;



 // make bloody effects
for (effect *eptr=effects; eptr<&effects[MAXMARKS*2]; eptr++)
         if (eptr->e) eptr->Tick();

//scroll background
bg3scrx+=1;
if (bg3scrx>320) bg3scrx=0;

if (p[0].energy!=barsize[0])
  {
   if (p[0].energy<barsize[0]) barsize[0]--;
   if (p[0].energy>barsize[0]) barsize[0]++;
  }

if (p[1].energy!=barsize[1])
  {
   if (p[1].energy<barsize[1]) barsize[1]--;
   if (p[1].energy>barsize[1]) barsize[1]++;
  }


series *bp;
bp=bg.bgs;
for (i=0; i<=bg.nums; i++,bp++)  bp->Tick();

if (electrocutedur) electrocutedur--;
 else electrocute=0;

if (ready)// && !sendhit) //dont do anything during hit sends
{
z=0;    
for (i=0; i<PMAX; i++) p[0].ps[i].AdvanceProjectile();
z=1;
for (i=0; i<PMAX; i++) p[1].ps[i].AdvanceProjectile();
    
z=0; p[0].Update();
z=1; p[1].Update();

z=2; if (ball.drawme) ball.Update();
z=3; if (chopper.drawme) chopper.Update();
}

RefreshInput();
p[0].in.ReadButt(0);
p[1].in.ReadButt(0);


}  


int playerstat::CreateProjectile(int ex, int ey)
{
 for (projectilestat *pp=(projectilestat *)&ps; pp<&ps[PMAX]; pp++)
  if (!pp->pframeptr)
    {
     pp->pframeptr   =&frame[moves[20].firstframe];
     pp->pnumframes  =moves[20].numframes; pp->ptype=type;
     pp->pcurrframe  =0; pp->shake=0;
     pp->pdur=((frametype *)pp->pframeptr)->dur;
     pp->pd=d; pp->px=ex;  pp->py=ey; pp->pstat=0;

     register image *i=(image *) (pp->pframeptr+sizeof(frametype));
     while (i->index!=0xFF) i++;
     pp->parectptr=(rect *)(((unsigned char *)i)+3); //Skip Vuln and attack

     return(1);
    }
return(0);
}    


#define DONEATTACK 1

void vector::SlowXA(int r)
{
if (v.x<=0) a.x+=r;
     else a.x-=r;
}

void vector::SpeedXA(int r)
{
if (v.x>=0) a.x+=r;
     else a.x-=r;
}

void vector::SlowXV(int r)
{
if (v.x<=0) v.x+=r;
     else v.x-=r;
}

void vector::SpeedXV(int r)
{
if (v.x>=0) v.x+=r;
     else v.x-=r;
}


void vector::SlowYV(int r)
{
if (v.y<=0) v.y+=r;
     else v.y-=r;
}

void vector::SpeedYV(int r)
{
if (v.y>=0) v.y+=r;
     else v.y-=r;
}
 

void TapHit(playerstat *p,playerstat *b,int dy)
{
if (!p->controlball) p->energy+=penergy[p->type][0];
 if (p->energy>128) p->energy=128;
 
p=p;
int dvy=-dy*0x400-b->v.v.y/16;

//same signs, so decrease x
if ((dvy>0 && b->v.v.y>0) || (dvy<0 && b->v.v.y<0))
 {
  b->v.SpeedXA(abs(dvy>>4)+0x650);
 } else b->v.SpeedXA(abs(dvy>>3)+0x800);
b->v.v.y+=dvy;

//b->v.SpeedXV(abs(p->v.v.x)/2);
//b->v.SpeedYV(p->v.v.y/2);

}    

void SpecialMove(playerstat *p,playerstat *b,int dy)
{
     p=p; dy=dy;

switch(p->type)
{
case 0:    
        b->v.SpeedXA(0x2500);
        b->v.v.y+=0x2500;  //reduce y movement
        b->v.v.y/=2;
       break;
case 1:
    break;
case 2: //hard hat contact
     p->cm=8; p->newmove=1;
     if (!p->d) ball.v.v.x=0x20000;
      else ball.v.v.x=-0x20000;
     ball.v.v.y=0;
    break;

case 5: //sinammon
  if (p->cm==6)
   {if (ball.v.v.y>0) ball.v.v.y=-ball.v.v.y; ball.v.v.y-=0x5000;}
   
  if (p->cm==8)
   {if (ball.v.v.y<0) ball.v.v.y=-ball.v.v.y; ball.v.v.y+=0x5000;}
  

 break;    
}
    
        
}    


void SuperMove(playerstat *p,playerstat *b,int dy)
{
//p->energy=0;    
p=p; dy=dy;

switch(p->type)
{
case 0:    
        b->v.SpeedXA((0x3000<<3)+0x1000);
        b->v.v.y+=0x3000;  //reduce y movement
        b->v.v.y/=4;
        break;

case 4:
      b->freeze=10; //stuck
      p->cm=8; //caught ball
      p->newmove=1;

   break;        
}

}    


int HitCheck(playerstat *p,playerstat *b)
{
 if (!(*p->arectptr) || !(*b->vrectptr)) return(0);
 if (p->stat&DONEATTACK) return(0);
 if (gamestat) return(0);
 if (p->d==b->d)// && multiplay)
 return(0);

rect *arptr=(rect *) (p->arectptr+1); //player attack
rect *vrptr=(rect *) (b->vrectptr+1); //ball defend
rect ar,vr;

//attack rect
ar.y1=arptr->y1+p->y; ar.y2=arptr->y2+p->y;
if (!p->d)
  {ar.x1=p->x+arptr->x1; ar.x2=p->x+arptr->x2;} else
  {ar.x1=p->x-arptr->x2; ar.x2=p->x-arptr->x1;}
//if (!p->d)
//  {ar.x1=p->x+arptr->x2; ar.x2=p->x+arptr->x2;} else
//  {ar.x1=p->x-arptr->x2; ar.x2=p->x-arptr->x2;}


//Calc vuln rect
vr.y1=vrptr->y1+b->y; vr.y2=vrptr->y2+b->y;
if(!b->d)
 {vr.x1=b->x+vrptr->x1; vr.x2=b->x+vrptr->x2;} else
 {vr.x1=b->x-vrptr->x2; vr.x2=b->x-vrptr->x1;}



//See if rect conflict
if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) )
 {
  b->stuck=0;
  if (p->po->cm==8) //if bulldozing
    {p->po->cm=9; p->po->newmove=1;}
  //make ball reflect
  if (p->d!=b->d)  //not facing same direction
   {
    b->d^=1;
    b->v.v.x=-b->v.v.x; //turn around
    b->v.a.x=-b->v.a.x; 
   }
  if (p->ctype==2) //computer player
    return(1);

  p->stat|=DONEATTACK;

  //push player back a bit
  if (!b->d) p->v.v.x-=b->v.v.x/2;
        else p->v.v.x-=b->v.v.x/2;
   
  //make it hit top/bottom of rectangles
//  if (b->v.v.y>0 && (vr.y1<ar.y1)) b->v.v.y=-b->v.v.y; else //going down 
//  if (b->v.v.y<0 && (vr.y2>ar.y2)) b->v.v.y=-b->v.v.y; else //going up
   //place ball at the end of the attack rectangle
  if (!p->d)
    { b->v.p.x+=(ar.x2-vr.x1)<<16;}
  else
    { b->v.p.x+=(ar.x1-vr.x2)<<16; }

 int dy=(ar.y2+ar.y1)/2; //center of attack, y coordinate
 dy-=b->y;    //find relative y location 

 b->freeze=0;

//make blood on power hit
 if (p->cm==6 || (p->type==5 && p->cm==8)) //special move
 {
  GenerateEffect(25,b->x,b->y,0,0,p->d,random(2));
  for (int i=0; i<15; i++)
   GenerateEffect(16,b->x,b->y,random(20),0,p->d^1,random(2));
  for (i=0; i<5; i++)
  {
   GenerateEffect(10,b->x,b->y,random(20),0,p->d^1,random(2));
   GenerateEffect(18,b->x,b->y,random(20)-10,random(20)-10,p->d^1,random(2));
   GenerateEffect(25,b->x,b->y,random(30)-15,random(30),p->d^1,random(2));
  }
  GenerateEffect(1,b->x,b->y,0,0,p->d,1);
  ballpain+=3;

  SpecialMove(p,b,dy);  //execute special move on ball
 }
 else
  if (p->cm==7) //sSUPER move
 {
  for (int i=0; i<25; i++)
   GenerateEffect(16,b->x,b->y,random(20),0,p->d^1,random(2));
  for (i=0; i<10; i++)
  {
   GenerateEffect(10,b->x,b->y,random(20),0,p->d^1,random(2));
   GenerateEffect(18,b->x,b->y,random(20)-10,random(20)-10,p->d^1,random(2));
  }
  GenerateEffect(25,b->x,b->y,random(30)-15,random(30),p->d^1,random(2));
  GenerateEffect(2,b->x,b->y,0,0,p->d,1);     
  ballpain+=6;
  SuperMove(p,b,dy);  //execute special move on ball

 } else
 {
  GenerateEffect(0,b->x,b->y,0,0,p->d,1);     
  if (!p->dizzydur) {p->cm=5; p->newmove=1;}
  ballpain+=1;
  TapHit(p,b,dy);  //just tap hit it
 }
  
  //make scream sound
  PlaySE( (8|(random(4)+1))<<4  ,1,2,b->x );

  //make ouch move
  b->cm=21; b->newmove=1;
  b->pain=ballpain/8;

  if (multiplay)
   {
    hitp.b=b->v; //send over complete vector
    sendhit=1; //send condition of this hit to other player
               //dont do anything until it has been acknowledged
   }

  p[0].desty=-1; p[1].desty=-1;            
  return(1);
 } 

return(0);  

}    



int HitPlayerCheck(playerstat *p,playerstat *b)
{
 if (!(*p->arectptr) || !(*b->vrectptr)) return(0);
 if (p->stat&DONEATTACK) return(0);

rect *arptr=(rect *) (p->arectptr+1); //player attack
rect *vrptr=(rect *) (b->vrectptr+1); //player defend
rect ar,vr;

//attack rect
ar.y1=arptr->y1+p->y; ar.y2=arptr->y2+p->y;
if (!p->d)
  {ar.x1=p->x+arptr->x1; ar.x2=p->x+arptr->x2;} else
  {ar.x1=p->x-arptr->x2; ar.x2=p->x-arptr->x1;}
//Calc vuln rect
vr.y1=vrptr->y1+b->y; vr.y2=vrptr->y2+b->y;
if(!b->d)
 {vr.x1=b->x+vrptr->x1; vr.x2=b->x+vrptr->x2;} else
 {vr.x1=b->x-vrptr->x2; vr.x2=b->x-vrptr->x1;}

//See if rect conflict
if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) )
 {
  p->stat|=DONEATTACK;

  //push player back a bit
   
  //make ouch move
  b->cm=0; b->newmove=0;
  if (!p->d)
    { p->v.p.x=(330-70)<<16;
      b->v.p.x=330<<16;
    }
   else
    { p->v.p.x=(70-10)<<16;
      b->v.p.x=-10<<16;
    }
   
  
  
//  b->stuck=1; //they cannot move

  p->cm=21; p->newmove=1;

  return(1);
 } 

return(0);  

}    

//,{0x0001,0x0001},{0xA0000,0x20000},{-30<<16,8<<16},{350<<16,192<<16}};

int ymiss,idletime,specialtime;

void ResetDifficulty()
{
int d=options->difficulty;
int nd=8-d; //negative difficulty
d=d*d; 

ymiss=(nd*nd);
if (p[1].type==5) ymiss+=7;
if (p[1].type==3) ymiss+=10;

idletime=nd*35+10;
specialtime=d*2+10;

}    



//finds Y position for which ball will intersect p's destX position
int FindHitY(playerstat *p, vector *b)
{
if (gamestat!=0) return (20+random(160));    
if (p->cm>3 && p->cm!=23) return(-1);    
static vector t; //store ball vector and play with it
memcpy(&t,b,sizeof(vector));

if (t.v.y==0 || t.v.x==0) //no y movement
  return (t.p.y>>16); //same y coord

//calculate time to get to edge
int time;
rect *arptr=(rect *) (p->arectptr+1); //player attack

int rightedge=p->destx- arptr->x1;
rightedge<<=16;

if (t.p.x>rightedge) return(t.p.y>>16);

if (t.v.x<=0)
 {
  time=((5<<16)-t.p.x)/t.v.x;  //time to get to left edge
  time+=-rightedge/t.v.x; //time to get to right edge
 } else time=(rightedge-t.p.x)/t.v.x;
printtime=time;

if (time<=0) return(101);
if (time>1000) return 100;

while (time>0)
 {
  ///upward motion
  if (t.v.y<0)
           {time-=((8<<16)-t.p.y)/t.v.y; t.p.y=(8<<16);}
     else {time-=((192<<16)-t.p.y)/t.v.y; t.p.y=(192<<16);}
  t.v.y=-t.v.y; //simulate bounce
 }


t.p.y-=time*t.v.y;
t.p.y>>=16;
//t.p.y+=ymiss/2+random(ymiss);
return (t.p.y);//-(arptr->y1+arptr->y2)/2);
}

void playerstat::DoCompSpecial1()
{ 
 switch(type)
  {
   case 1: //fetus
   case 3: //gurdip
   case 4: //ed
   case 7: //gonzo
    if (!ball.d) strategy=0; //coming towards us
     else    {
     desty=po->y;
     if (y>desty-10 || y<desty+10) in.stat|=16; //do move
    }
   break;
   case 0: //bash!
    if (!ball.d && ball.x>260 && ball.y>y-30 && ball.y<y+10) //if in range
      in.stat|=16;
   break; 
   case 5: //whip it
    if (ball.x>240 && ball.x<310) //within x range
     if (ball.y>y+20 && ball.y<y+80 ||
         ball.y>y-80 && ball.y<y-20)  //if in range
      in.stat|=16;
   break;
   case 6: //buddy
    if (!ball.d && ball.x>120)  {strategy=0; in.but=0; break;}
    if (cm<3) in.stat|=16; //do drool
     else
     {
       if (!gamestat) in.but|=1; else break;
     }
     sdur++;
    return;

   case 2: //hhh hand grab
     in.but|=1;
    if (cm<3) //do hando grabo
    {
     if (ball.x>270 && ball.y>y-15 && ball.y<y+15) //if in range
      {in.stat|=16; return;}
    } else //got it in hand...
     {
       in.but|=1;
       if (y<100)
          if (po->y<100) desty=120+random(60);
           else {in.but=0; strategy=0; break;}
        else   
          if (po->y>100) desty=20+random(60);
           else {in.but=0; strategy=0; break;}
       sdur++;
       return;
     }
    
    break;
    
  }
  //if we did the move
if (strategy==0 || (in.stat&16))
 {strategy=0; sdur=idletime; desty=-1;} //time till next move
}    

void playerstat::DoCompSpecial2()
{ 
 switch(type)
  {
    //ed,gonzo,buddy
   case 2: //hhh rock
   case 5: //sinn slave
    if (!ball.d && ball.x>150) strategy=0; //coming towards us
     else    
     {
     desty=po->y;
     if (y>desty-10 || y<desty+10) in.stat|=32; //do move
    }
   break;
   case 0: //super bash!
    if (!ball.d && ball.x>260 && ball.y>y-30 && ball.y<y+10) //if in range
      in.stat|=32;
   break; 
   case 1: //fetus tongue
    if (ball.x>290 && ball.y>y-40 && ball.y<y+0) //if in range
      in.stat|=32;
   break;
   case 3:  //gurdip
   strategy=0; sdur=100;    
   break;
   case 4: //ed hand grab
     in.but|=2;
    if (cm<8) //do hando grabo
    {
     if (ball.x>260 && ball.y>y-30 && ball.y<y) //if in range
      {in.stat|=32; return;}
    } else //got it in hand...
     {
       in.but|=2;
       if (y<100)
          if (po->y<100) desty=120+random(60);
           else {in.but=0; strategy=0; break;}
        else   
          if (po->y>100) desty=20+random(60);
           else {in.but=0; strategy=0; break;}
         
//       if (y<100 && po->y<70) in.stat|=8; else
//       if (y>100 && po->y>130) in.stat|=4;
    //   else {in.but=0; strategy=0; break;}

       sdur++;
       return;
     }
    
    break;
   case 6: //buddy hug
   if (cm<3)
   {
    if (!ball.d) strategy=0; //coming towards us
     else    {
     desty=po->y;
     if (y>desty-10 || y<desty+10) {in.stat|=32; return; } //do move
     break;
    }
   } else
    {
     desty=p->y;
     sdur++;
     return;
    }

    break;
   
  }
  //if we did the move
if (strategy==0 || (in.stat&32))
 {strategy=0; sdur=idletime; desty=-1;} //time till next move
}    

void playerstat::CompAI()
{
  in.stat=0;
  in.but=0;


/*  if (type==7) //if gonzo
   {
    if (gamestat<0) destx=340;
     else   
    if (ball.d || gamestat)
     {
//       if (destx<30) destx=random(150)+150;
     } else
     {
      destx=ball.x+40;
     } 
    if (destx<x) in.stat|=1;
    if (destx>x+10) in.stat|=2;
    destx=random(150)+200;
   
   } else if (x>340) in.stat|=1;
*/
/* if (type==7)
  {
    if (desty==-1)
     destx=180+random(140);
    if (ball.x+55>x) in.stat|=2;
     else
     { 
      if (destx<x-10) in.stat|=1;
      if (destx>x+10) in.stat|=2;
     }

  } else if (x>340) in.stat|=1;
*/
  if (type==7) //make gonzo fly!
   {
     if (desty==-1) destx=180+random(160);
     if (gamestat<0) destx=340;

   } else destx=340; //if (x>340) in.stat|=1;
   
  if (type==3) //gurdip tool disappear
    if (!ball.d && ball.x<150 && (abs(ball.y-y)>50))  in.stat|=32;

//  destx=x;
  if (desty==-1 || !(uu&31))
   desty=FindHitY(this,&ball.v);
//   printtime=desty;

  if (desty!=-1)
  {
   if (y<desty-ymiss) in.stat|=8;
   if (y>desty+10+ymiss) in.stat|=4;
  }

  if (destx<x-5) in.stat|=1;
  if (destx>x+5) in.stat|=2;
  

if (gamestat) return;

 if (controlball) // tool control ball
  {
   if (ball.d)
   {
   in.stat=0;
//   if (ball.y<100)
    if (po->y<100) in.stat|=8; else in.stat|=4; //ball down
//   else
//    if (po->y>100) in.stat|=8; else in.stat|=4; //ball down
    
   in.but|=1;
   }
  } else    

 switch (strategy)
 {
  case 0: //wait to do something
   if (sdur>0) sdur--;  //do special move
    else
    {
     if (energy>=penergy[type][1]) //we can do special move #1
     {
      if (energy>=penergy[type][2]) //we can do special move #2 also
       strategy=random(2)+1; //do random of either
     else strategy=1; //can't do 2, only do one
     }
     else //can't do 1
     {
      if (energy>=penergy[type][2]) //we can do special move #2 only
       strategy=2; //do 2
      else {strategy=0; sdur=idletime; break;} //can't do either
     }
     sdur=specialtime; //time till special move expires
    }
  break;

  case 1: //special move 1
    if (sdur>0) //never got a chance to complete the special move
     {sdur--; DoCompSpecial1();}
    else 
     {strategy=0; sdur=idletime/2;}
    break;  
  case 2: //special move 2
    if (sdur>0) //never got a chance to complete the special move
     {sdur--; DoCompSpecial2();}
    else 
     {strategy=0; sdur=idletime/2;}
    break;  

 }

 
}        


void playerstat::Update()
{
int i;

if (freeze>0)
 {
 x=v.p.x>>16; y=v.p.y>>16;
  
  freeze--;  return;
 }

if (ctype==2)
 {
//    HitCheck(this,&ball);
    return;
 }

if (ctype==3) //computer player
 {
  CompAI();
 }



if (ctype<=3 && po->cm==21) //we are getting hugged!
  {
   if (!po->d) x=po->x+70;
        else x=po->x-70;
   y=po->y;   
   in.stat&=15;
   HitCheck(this,&ball);

   return;
  }


//update our vector
if (ctype==1 || ctype==3) //human player
 if (cm==21) //HIUGGGING!!
 {
  drawpriority=z;
  v.v.y/=2;
  v.Tick(in.stat&12,1);
  v.Tick(po->in.stat&12,1);
  
  x=(v.p.x>>16)+random(5);
  y=v.p.y>>16;
  in.stat&=15;
  if (currframe>=2) cm=0;
 } 
  else
{
 v.Tick(in.stat,*((int *)&((frametype *)frameptr)->dx));
 if (!d)  v.p.x+=*((short int *)&((frametype *)frameptr)->dx)<<8;
    else  v.p.x-=*((short int *)&((frametype *)frameptr)->dx)<<8;
 v.p.y+=*((short int *)&((frametype *)frameptr)->tx)<<8;

if (type==7 && (in.but&2) && energy>0)
 {v.Tick(in.stat,0); v.Tick(in.stat,0); energy--;}

 x=v.p.x>>16; y=v.p.y>>16;
 if (!dizzydur)
 {
  if ((in.stat&16) && (cm<3 || cm==5) && energy>=penergy[type][1] && !controlball)
      {cm=6;  newmove=1; energy-=penergy[type][1];
        if (type==5 && ball.y>y) //cinammon
            cm=8;
        }
  if (type!=7)
  {if ((in.stat&32) && (cm<3 || cm==5) && energy>=penergy[type][2])
   {cm=7;  newmove=1; energy-=penergy[type][2];}  }
   
  
  if (in.stat&8  && cm!=2 && cm<3) {cm=2; newmove=1;} else
  if (in.stat&4  && cm!=1 && cm<3) {cm=1; newmove=1;} else
  if ((in.stat&12)==0  && cm!=0 && cm<3) {cm=0; newmove=1;}
 } else dizzydur--;

  in.stat&=15;

 HitCheck(this,&ball);
 if (type==6 && cm==7)
   HitPlayerCheck(this,po);

 if (multiplay)
 {
  playerp.x=(short)x; playerp.y=(short)y;
  playerp.frame=(unsigned short)(frameptr-frame);
  //SendIPXPacketFixed(1,(char *)&playerp,sizeof(playerp));
 }

//HARD hat HARRY!
if (type==2)
{
 if (cm==8)
  {if (!d) ball.v.p.x=v.p.x+0x390000;
       else ball.v.p.x=v.p.x-0x390000;
   ball.v.p.y=v.p.y+0x50000;
   if (!(in.but&1)) {cm=9; newmove=1; ball.v.v.y=v.v.y/2; }
  }
 if (cm==9)
  {
   if (!d) {if (x<0) {cm=0; newmove=1;}}
     else  {if (x>320) {cm=0; newmove=1;}}
  }
}

//ed grab stuff
if (type==4)
 {
  if (cm==8 || (cm==9 && currframe<3)) //poopie#1
   {
   ball.freeze++;
       
   if ( ((frametype *)frameptr)->parm&256 )
    {
     GetRect();
     unsigned char *t=arectptr+ ((*arectptr)*sizeof(rect))+1;
     if (!d) ball.v.p.x=v.p.x+(*((short *)(t+1))<<16); 
       else  ball.v.p.x=v.p.x-(*((short *)(t+1))<<16);
     ball.v.p.y=v.p.y+ (*((short *)(t+3))<<16);
    }

   }
  if (cm==8)
    if (!(in.but&2) || !energy)
       {cm=9; newmove=1; ball.freeze=0; ball.v.v.y=v.v.y/2; ball.v.v.x=ball.v.v.x*2/3; ball.v.SpeedXV(0x15000); p[z^1].desty=-1; }
     else if (!(uu&7)) energy--;
   
   
 }
  
}
  //   

if (ctype==5)
 {
  if (!tcountx)
    {if (y>198) y--; else tcountx++;}
   else
  if (tcountx<200) tcountx++;
  else y+=2;

   if ( ((frametype *)frameptr)->parm&256 )
    {
     if (gamestat==-1) //ball hasn't been drawn yet
     {
     GetRect();
     unsigned char *t=arectptr+ ((*arectptr)*sizeof(rect))+1;
     p[0].desty=-1; p[1].desty=-1;
     ball.d=chopper.d^1; ballpain=0; ball.pain=0;
     if (!d) ball.x=x+*((short *)(t+1)); 
       else  ball.x=x-*((short *)(t+1));
     ball.y=y+ *((short *)(t+3));
     ball.v.p.x=ball.x<<16; ball.v.p.y=ball.y<<16;
      ball.Reset(); gamestat++;
     if (multimaster)
       {
        if (ball.d) ball.v.v.x=-0x0E000;
         else ball.v.v.x=0x0E000;
        ball.v.v.y=0xE000;
        hitp.b=ball.v; //send over complete vector
        sendhit=1;
       } 

      

  for (int i=0; i<25; i++)
   GenerateEffect(16,ball.x,ball.y,random(20),0,ball.d^1,random(2));
  for (i=0; i<10; i++)
  {
   GenerateEffect(10,ball.x,ball.y,random(20),0,ball.d^1,random(2));
   GenerateEffect(18,ball.x,ball.y,random(20)-10,random(20)-10,ball.d^1,random(2));
  }
 GenerateEffect(25,ball.x,ball.y,random(30)-15,random(30),ball.d^1,random(2));
      
     } 

   }       
 }     

 
if (ctype==4) //if ball
{
// if (!multiplay || d) //if single player, or multi and coming towards local
 {
  int result=0;
  if (!stuck) result=v.Bounce();
   else stuck--;
  x=v.p.x>>16; y=v.p.y>>16;
  if (!gamestat) //check for weener
  {
   if (x>(320+45) && !multiplay) //p0 wins
    {p[0].cm=3; p[0].newmove=1; 
        p[1].cm=4; p[1].newmove=1; gamestat=1;
        score[0]++;
        if (score[0]>=roundcount) gameover=1;
        if (score[1]>=roundcount) gameover=1;     
        
       electrocute=2; electrocutedur=150;
      v.v.x=0; v.v.y=0;
      p[1].dizzydur=p[0].dizzydur=0;
     PlaySE( (8|(5))<<4  ,1,z,x );
//     x=160; v.v.x=160<<16;
//     drawme=0;
     }
   if (x<-45) //p1 wins
    {
     gamestat=1;
     p[0].cm=4; p[0].newmove=1; //make us lose
      electrocute=1; electrocutedur=150; 

     PlaySE( (8|(5))<<4  ,1,z,x ); 
     
     if (multiplay) sendloss=1; //we must have lost
       else {p[1].cm=3; p[1].newmove=1;
      p[1].dizzydur=p[0].dizzydur=0;
//     x=160; v.v.x=160<<16;     
//            p[1].energy+=penergy[p[1].type][2]; if (p[1].energy>128) p[1].energy=128;
           } //force guy to win
     score[1]++;
        if (score[0]>=roundcount) gameover=1;     
        if (score[1]>=roundcount) gameover=1;     
     v.v.x=0; v.v.y=0;     v.a.x=0; v.a.y=0;
//     drawme=0;

    }
  }

  if (result&3) d^=1; //turn ball around
  if (result) PlaySE( (8|(random(4)+1))<<4  ,1,z,x );
  if (result&12) //hit top/bottom
   {
   for (i=0; i<5; i++)
    GenerateEffect(16,x,y,random(20)-10,0,d,random(2));
    GenerateEffect(18,x,y,0,0,d,random(2));
   } 
  if (!(uu&63))
   GenerateEffect(10,x,y,0,0, 0,0);

  int ballspeed=abs(v.v.x)+abs(v.v.y);
  if (ballspeed>0x20000)  
   if (!(uu&31))
    GenerateEffect(18,x,y,0,0, 0,0);
  if (ballspeed>0x30000)  
   if (!(uu&15) && (uu&16) )
    GenerateEffect(18,x,y,0,0, 0,0);
    

  if (multiplay) //send coords
   {
     ballp.x=(short)x; ballp.y=(short)y;
//     SendIPXPacketFixed(2,(char *)&ballp,sizeof(ballp));
   }  
 }

} 

//if (gameover && (cm==3 || cm==4) && currframe==numframes-1 && dur<10)
// {dur=0xFFFFFF; gamestat++;}


if (dur>0 && !newmove) dur--; //animate
 else
  {
   if (!newmove) //increase frame
    { currframe++; frameptr+=((frametype *)frameptr)->size; }

   if (type==3) //tool
    if (cm==7 && currframe==9)
       v.p.y=ball.v.p.y;

   newframe=1;         //We moved to a new frame
   if ( (currframe>=numframes) || newmove)
    {
     if (!newmove) //see if move was completed
      {
       if (ctype==5) drawme=0;
         else   
       if (ctype==4) cm=22; //ball always goes back to headproj
        else
          if (cm>=3) //if not up/down/stance
            {
             if (cm<5 && ctype!=2)
              {
               if (gamestat<3) gamestat++;//if win/loss pose
               senddonepose=1;
              }
             
             if (cm==8) {if (type==4) cm=8; else cm=9;}
              else
             if (dizzydur>0) cm=23; 
               else if (!gameover) cm=0;
                  else gameover|=z+1;
            } 
      }
     frameptr   =&frame[moves[cm].firstframe];
     numframes=moves[cm].numframes;
     currframe  =0; newmove=0;
    } 

   stat&=~(DONEATTACK);     
   dur=((frametype *)frameptr)->dur;
   newmove=0; GetRect();

   if (ctype<=3)
   if ( ((frametype *)frameptr)->parm&256 )
    {
     unsigned char *t=arectptr+ ((*arectptr)*sizeof(rect))+1;
     int tx;
     if (!d) tx=x+*((short *)(t+1)); 
       else  tx=x-*((short *)(t+1));
     int noggin=(*t)>>4;
     if (noggin==6) //projectile
      CreateProjectile(tx, y+*((short *)(t+3)));
    } 
   

   //Make sound effect
   if (gameover<3)
   PlaySE( ((frametype *)frameptr)->se,0,z,x );
  }


}    



void projectilestat::AdvanceProjectile()
{
if (!pframeptr) return;


// phit[z]++;

//sendhit++;
playerstat *b=&p[z^1];

if (!pstat && ptype==6) //buddy
 {
  if (shake<200) //push button
   {if (p[z].in.but&1)
      {if (!pd) px++; else px--;
       shake++; p[z].dur++;
      } else shake=200;
    }
   else
         {
          shake++;
          if (shake>2000)
          { pframeptr   =&p[z].frame[p[z].moves[22].firstframe];
            pcurrframe=0;  pnumframes=p[z].moves[22].numframes;
            pdur=((frametype *)pframeptr)->dur;
            pstat=1;
          }
         }
 }


//hit check

if (!pstat) //0-do hits
{
rect *vrptr, *arptr=(rect *) (parectptr); //player attack
rect ar,vr;

//CHECK FOR HITTING OTHER PLAYER
if (*b->vrectptr && !gamestat)
{
 vrptr=(rect *) (b->vrectptr+1); //opponent defend

 //attack rect
 ar.y1=arptr->y1+py; ar.y2=arptr->y2+py;
 if (!pd)
  {ar.x1=px+arptr->x1; ar.x2=px+arptr->x2;} else
  {ar.x1=px-arptr->x2; ar.x2=px-arptr->x1;}


//see if hitting opponent
if (ptype!=6)
{
 //Calc vuln rect
 vr.y1=vrptr->y1+b->y; vr.y2=vrptr->y2+b->y;
 if(!b->d)
  {vr.x1=b->x+vrptr->x1; vr.x2=b->x+vrptr->x2;} else
  {vr.x1=b->x-vrptr->x2; vr.x2=b->x-vrptr->x1;}

//See if rect conflict
if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) )
 {
   pframeptr   =&p[z].frame[p[z].moves[21].firstframe];
   pcurrframe=0;  pnumframes=p[z].moves[21].numframes;
   pdur=((frametype *)pframeptr)->dur;
   stickx=px-b->x; sticky=py-b->y;
   pstat=1;

//   int dy=(ar.y2+ar.y1)/2-(vr.y2+vr.y1)/2; //center of attack, y coordinate
  
   if (ptype==2) //rock
    {
//     if (ball.y>b->y) //enemy is above
    if (b->y>100)
            {b->v.p.y-=-0x3A0000; b->v.v.y=-0xA000;}
     else  {b->v.p.y+=0x3A0000; b->v.v.y=0xA000;}
     if (!b->d) {b->v.p.x-=0x200000; b->v.v.x=-0x10000;}
           else {b->v.p.x+=0x200000; b->v.v.x=0x10000;}
     b->cm=0; b->newmove=1;
           
    }

   if (ptype==5) //cinn
    {
     if (py>b->y) //enemy is above
            {b->v.p.y-=-0xA0000; b->v.v.y=-0x60000;}
     else  {b->v.p.y+=0xA0000; b->v.v.y=0x60000;}
     if (!b->d) {b->v.p.x-=0x20000; b->v.v.x=-0x1000;}
           else {b->v.p.x+=0x20000; b->v.v.x=0x1000;}
     b->cm=0; b->newmove=1;
           
    }

   if (ptype==7) //gonzo
    {
      b->energy-=35;
      if (b->energy<0) b->energy=0;
    }


   if (ptype==3) //shrink
    {
     b->dizzydur=500; //shrink him
     if (!b->d) b->v.p.x=b->v.boundmax.x;
       else b->v.p.x=b->v.boundmin.x;
     b->cm=23; b->newmove=1;
    }
   if (ptype==4) //cleaver
    {
     if (!pd)  stickx+=20; else stickx-=20;
     if (ball.y>b->y) //enemy is above
            {b->v.p.y-=-0x10000; b->v.v.y=-0x6000;}
     else  {b->v.p.y+=0x10000; b->v.v.y=0x6000;}
     if (!b->d) {b->v.p.x-=0x20000; b->v.v.x=-0x1000;}
           else {b->v.p.x+=0x20000; b->v.v.x=0x1000;}
           
           
    }
 }
}
}



///---------
//see if hitting ball
if (!gamestat)
if (ptype!=2 && ptype!=1) //cleaver
{
 //check for ball hits
 vrptr=(rect *) (ball.vrectptr+1); //ball defend
 //Calc vuln rect
 vr.y1=vrptr->y1+ball.y; vr.y2=vrptr->y2+ball.y;
 if(!ball.d)
  {vr.x1=ball.x+vrptr->x1; vr.x2=ball.x+vrptr->x2;} else
  {vr.x1=ball.x-vrptr->x2; vr.x2=ball.x-vrptr->x1;}


 //See if rect conflict
 if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) )
 {
   pframeptr   =&p[z].frame[p[z].moves[22].firstframe];
   pcurrframe=0;  pnumframes=p[z].moves[22].numframes;
   pdur=((frametype *)pframeptr)->dur;
   stickx=px-ball.x; sticky=py-ball.y;   pstat=2; //2-just bounce off
   if (ptype==4) //cleaver hit ball
    {
     if (pd!=ball.d)  //not facing same direction
           {
            ball.d^=1;
            ball.v.v.x=-ball.v.v.x; //turn around
            ball.v.a.x=-ball.v.a.x;
           }
     ball.v.SpeedXA(0x2000);
     int dy=(ar.y2+ar.y1)/2-ball.y; //center of attack, y coordinate
     ball.v.v.y=-dy*0x1500+ball.v.v.y/2;
     b->desty=-1; ball.stuck=2;
     ball.freeze=0;
    }
   if (ptype==3) // tool hit ball
    {
     stickx=0; sticky=0; //shake=90;
     pstat=3; ball.stuck=0;
     if (b->type==4 && b->cm==8) {b->cm=0; b->newmove=1; ball.freeze=0;}
    }
   if (ptype==7) //gonzo
    {
     if (!pd) ball.v.p.x+=15<<16;
         else ball.v.p.x-=15<<16;
    }

    
   if (ptype==1) // fetus hit ball
    {
     stickx=0; sticky=0; //shake=90;
     ball.stuck=30; //time that ball pauses
      if (b->type==4 && b->cm==8) {b->cm=0; b->newmove=1; ball.freeze=0;}
    }
   if (ptype==5) // sinn
    {
     //stickx=0; sticky=0; //shake=90;
     pstat=3; //ball.stuck=2;
     if (b->type==4 && b->cm==8) {b->cm=0; b->newmove=1; ball.freeze=0;}
     if (pd!=ball.d)  //not facing same direction
           {
            ball.d^=1;
            ball.v.v.x=-ball.v.v.x; //turn around
            ball.v.a.x=-ball.v.a.x;
           }
     ball.v.v.x/=2;      
     ball.v.SpeedXV(0x10000);      
     ball.v.v.y/=3;      
     ball.freeze=5;
    }
    
   if (ptype==6) //buddy
    {
     ball.v.SpeedXA(0x1000);
     int dy=(ar.y2+ar.y1)/2-ball.y; //center of attack, y coordinate
     if (pd!=ball.d)  //not facing same direction
           {
            ball.d^=1;
            ball.v.v.x=-ball.v.v.x; //turn around
            ball.v.a.x=-ball.v.a.x;
           }
     
     ball.v.v.y=-dy*0x1500+ball.v.v.y/2;
     b->desty=-1;
     if (b->type==4 && b->cm==8) {b->cm=0; b->newmove=1;}
    } 
    
    
  }
}


}


//stick to opponent
if (pstat==1)
 {
   //fetus spit
  if ((ptype==1 || ptype==4) && !*((short int *)&((frametype *)pframeptr)->tx)) //spit
     {
      px=b->x+stickx; py=b->y+sticky;
      if (ptype==1)
      {
      b->v.SlowXV((accel[b->type].x-deccel[b->type].x)/2);
      b->v.SlowYV((accel[b->type].y-deccel[b->type].y)/2);

      if (b->v.v.y<0 && shake>=0) {shake=-1; pdur-=20; }//quit=1;}
       else
      if (b->v.v.y>0 && shake<=0) {shake= 1; pdur-=20; }// quit=1;}
     }

    ///ed axe
   if (ptype==4 && !(uu&3)) ///blood stuff
       {
          for (int i=0; i<15; i++)
           GenerateEffect(16,px,py,random(20),0,pd,random(2));
          for (i=0; i<5; i++)
          {
           GenerateEffect(10,px,py,random(20),0,pd,random(2));
           GenerateEffect(18,px,py,random(20)-10,random(20)-10,pd,random(2));
           GenerateEffect(25,px,py,random(30)-15,random(30),pd,random(2));
          }
          GenerateEffect(1,px,py,0,0,pd,1);
          if (b->energy>0) b->energy--;
       }
 
       
     }
 }

//pstat==2   just bounce off

 //stick to ball
if (pstat==3)
{
if (ptype==3) //tool
 {
  px=ball.x+stickx; py=ball.y+sticky;
  p[z].controlball=1;  

  if (p[z].energy && !ball.stuck) { if (!(uu&3)) p[z].energy--;}
     else {p[z].controlball=0; pframeptr=0; }
 
  if (p[z].in.but&1)
   {
    p[z^1].desty=-1;   
    if (p[z].in.stat&1) ball.v.p.x-=0x15000;
    if (p[z].in.stat&2) ball.v.p.x+=0x15000;
    if (p[z].in.stat&4) ball.v.p.y-=0x15000;
    if (p[z].in.stat&8) ball.v.p.y+=0x15000;
    if (ball.v.p.y<8<<16) ball.v.p.y=8<<16;
    if (ball.v.p.y>192<<16) ball.v.p.y=192<<16;
    
   }
 }

if (ptype==5) //cinn
 {
  if (ball.freeze>0)
   {
    if (uu&1)
     {
      if ((p[z].in.stat&4) && py>30) py--;
      if ((p[z].in.stat&8) && py<190) py++;
     } 
      
     if (!pd) ball.v.p.x=(px+10)<<16;
       else ball.v.p.x=(px-10)<<16;
    ball.v.p.y=(py-10)<<16;
    ball.freeze++;
    p[z^1].desty=-1;
   } 


 }

}     

//move projectile
 if (!pd)  ptcx+=*((short int *)&((frametype *)pframeptr)->dx);
    else   ptcx-=*((short int *)&((frametype *)pframeptr)->dx);
 ptcy+=*((short int *)&((frametype *)pframeptr)->tx);

 px+=ptcx>>8; py+=ptcy>>8;  //Move it!!!
 ptcx&=0xFF; ptcy&=0xFF;

if (px>380 || px<-60) { if (pstat==3) p[z].controlball=0; pframeptr=0;  return;}


if (pdur>0) pdur--;
  else
        {
         pcurrframe++;

         if (pcurrframe>=pnumframes)
         {
          //Done projectile--repeat it (repeat standard) 
          if (pstat==0)
          {
                pframeptr   =&p[z].frame[p[z].moves[20].firstframe];
                pcurrframe=0; 
                pdur=((frametype *)pframeptr)->dur;
          } else
          if (pstat==3) //repeat tool animation
          {
                pframeptr   =&p[z].frame[p[z].moves[22].firstframe];
                pcurrframe=0; 
                pdur=((frametype *)pframeptr)->dur;
          } else
          //Finished projectile
          pframeptr=0;

         }  else
         {pframeptr+=((frametype *)pframeptr)->size;
             pdur=((frametype *)pframeptr)->dur;}
             
         PlaySE( ((frametype *)pframeptr)->se,0,z,px );
        }





}    






//----------------------

void playerstat::GetRect()
{
register image *i=(image  *) (frameptr+sizeof(frametype));

while (i->index!=0xFF) i++;

vrectptr=((unsigned char *)i)+1; //Point to first count of v rects
arectptr=vrectptr+ ((*vrectptr)*sizeof(rect))+1; //first count of a rects
}


void playerstat::Reset()
{
    freeze=0;
controlball=0;
stuck=0;
if (ctype!=4) cm=0;
else cm=22;

drawme=1;

dizzydur=0;

frameptr   =&frame[moves[cm].firstframe];
currframe  =0;
numframes=moves[cm].numframes;
dur=((frametype *)frameptr)->dur;


maxenergy=energy=20;
destx=desty=-1;
tcountx=0;
tcounty=0;
pain=0;
strategy=0;
sdur=0;

//frameptr=0
for (int i=0; i<PMAX; i++)
 {ps[i].pframeptr=0; ps[i].pstat=0; }
pexist=0;

ca=cai=0;
//in.stat=0;

GetRect();

}

extern char *eimagelist[256];
extern move elist[];
extern unsigned char *eframe;


void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p)
{


 if (en==16) en=42+random(6);
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
         if (en>=7 && en<=10) eptr->maxy=190;
         if (en>=12 && en<=14) eptr->maxy=190;
         if (en>=42 && en<=47) eptr->maxy=190;
         eptr->maxy+=random(10);

//         if (p) eptr->maxy-=15;


//       if (en==16) eptr->dur+=GetRnd(15)+6;
//       if (en==10) eptr->dur+=GetRnd(25);


         break;
        }

}

static char oldscan=0;
int ki,ka;
int olds;

int keybutton[2][4]=
          {
                {24,26,38,40},
                {16,18,30,32}
          };


void Keyboard()
{
kbscan=inp(0x60);

if (oldscan!=kbscan )
{
        oldscan=kbscan;

        kbint=1;
        //update position of keyboard controls....position
        if (!(kbstat&1)) //Control
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
         //Check key
         if (kbscan==1) quit=1;
        } else   //if control
        {
          if (kbscan==25)
                {
                 pause^=1;        //p
                 if (musicstat) playing=pause^1;
                }
          if (debug)
          {      
          if (kbscan==21) framecheck^=1; //y
          if (kbscan==22) drawp^=1; //u
          if (kbscan==23) draww^=1; //i
          if (kbscan==19) score[0]++; //r
          if (kbscan==20) options->talking^=1; //t
          if (kbscan==24) plock[1]=0; 
          } 
          if (kbscan==48)
           { black^=1;        
           }    


        }
}

/* else //kbscan&0x80
         {
            unsigned char k=kbscan&0x7F;
            //check keyboard buttons
             for (ki=0; ki<2; ki++)
              if (p[ki].in.type>=3 && p[ki].in.type<=4 && !kbstat )
                 {
                        ka=(p[ki].in.type&1)^1;
                        if (k>=keybutton[ka][0] && k<=keybutton[ka][1]) {p[ki].in.but&=~1; }
                        if (k>=keybutton[ka][2] && k<=keybutton[ka][3]) {p[ki].in.but&=~2; }
                 }


         }*/

if (kbscan==0x1D) kbstat|=1;
if (kbscan==0x9D) kbstat&=~1;
if (kbscan&0x80) kbscan=0;

kbint=0;
}

}


void StopVoice()
{
 if (!sbstat) return;
 stop_sound(1);
 stop_sound(2);
}

void UpdatePalette()
{
int i;

if (oldfadelevel!=fadelevel)
 {
  unsigned int  fl=fadelevel;
  unsigned char *p1=(unsigned char *)pal1;
  unsigned char *p2=(unsigned char *)pal2;

  if (fl<32)
         for (i=0; i<256*3; i++,p1++,p2++)  *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);
  else  MemoryCopy(pal2,pal1,256*3);


  WaitVSync();
  LoadPalette(pal2,0,256);

  oldfadelevel=fadelevel;
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


void PlaySE(unsigned char se,char type,char pl,int xc) //type 0-swish 1-hit
{
    xc=xc;
//if (gamestat<0) return;

if (type) se>>=4;
se&=15; if (!(se&7)) return;

playerstat *player=&p[pl];
 if (pl==2) player=&ball;
 if (pl==3) player=&chopper;

SOUND *s;
if (!(se&8)) s=&sse[type][ (se&7) -1];       //It's a standard se
          else    s=&player->se[type][ (se&7) -1];  //It's a player specific se
if (s->soundptr)  PlayMixedSoundEffect(s,8);

}

void PutStickyImage(char *img, int x, int y, int o)
{
int xlen,ylen,xst,yst,clip;
clip=0; //clip flag
xst=yst=0;
xlen=(int) (unsigned char)img[1];
ylen=(int) (unsigned char)img[2];

//y+=random(15)-7;
y-=bg.floory;
x+=bg.scrx; //+(bg.scrx+160-320)*(y+ylen/2)/150;
//x+=(720-640)/2;
if (y+ylen<0) return;
if (y>bg.floorheight-10) return;
if (x+xlen<0 || x>640) return;

if (x+xlen>640) {xlen=640-x; clip=1; }
if (x<0)         {xst=-x; clip=1;  x=0; xlen-=xst;}
if (y<0)         {yst=-y; clip=1;  y=0; ylen-=yst; return; }
if (y+ylen>bg.floorheight) {ylen=bg.floorheight-y; clip=1; if (ylen<0) return;}

//PutImageFlip256(img,bg.bg1raw+(y-bg.bg2y)*640+x+bg.scrx*8/10,640,o);
if (!clip) PutImageFlip256(img,bg.bgfloor+x+y*720,720,o);
 else      PutImageFlipClip256(img,bg.bgfloor+(720*y)+x,720,xst,yst,xlen,ylen,o);
}

void effect::Draw()
{
_disable();
if (!e)
  {
       _enable();
         return;
  }

if (this>=&effects[MAXMARKS] && en==11)
 {
   for (effect *eptr=&effects[0]; eptr<&effects[MAXMARKS] && eptr->e; eptr++);
   if (!eptr->e) *eptr=*this;
   e=0;
   return;
 }
  
int tx=x;
int ty=y-80;
if (en==27) ty+=bg.scry;
int td=d;
image *i=(image *) (e+sizeof(frametype));

_enable();

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
//  drawraw=1;  
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
         //if (len>=12 && len<=14) {en=15; y+=32; }
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



void FuckWithImage(char *s,unsigned char c1,unsigned char c2)
{
int wx,wy; //width and height of image
//int k;
wx=(int)(unsigned char)s[1];
wy=(int)(unsigned char)s[2];

s+=4+wy+wy; ///s points to image

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F;  i-=a; }
  else         //opaque
         {
          for (int b=a; b>0; b--)
                 {
                  if (*s==c1) *s=c2;
                  s++;
                 }
                i-=a;
         }
 }
}
}


void FunkyFont(char *f,unsigned char a,unsigned char b)
{

for (int c=0; c<128; c++)
{
 unsigned int index=((unsigned short *)f)[c];
 if (index)  FuckWithImage(f+index,a,b);
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

extern int lastsize;

void InitHeadGuySound()
{
int h;
if (sbstat!=2) return;

if (_dos_open("sound.vol",O_BINARY | O_RDONLY,&h))
 {
  #ifdef REVOL
   _dos_creat("sound.vol",0,&h);
   chdir("\\nogsnd");
   unsigned int size;
   char s[30];
   for (int i=0; i<NUMGUYSOUND; i++)
        {
         sprintf(s,"SAY%d.RVC",i+1);   
         char *t=ReadFile(s);
         if (t)
          {
           _dos_write(h,t,lastsize,&bytes);
           free(t);
          }
          else
          {
           size=0;
           _dos_write(h,&size,4,&bytes);
          }
         }
  chdir("\\noggin");
  _dos_close(h);
  _dos_open("sound.vol",O_BINARY | O_RDONLY,&h);
  #else
   Nofile("sound.vol");
  #endif 
 }

int i;

for (i=0; i<NUMGUYSOUND; i++)
  ReadVolSound(h,&headguysound[i]);

_dos_close(h);


}




    
