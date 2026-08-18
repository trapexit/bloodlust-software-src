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


#define VERSION "0.6"

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

#include "tipx.h"
#include "tpacket.h"
#include "tchat.h"

#define MAXMARKS 40

int pcount[8]={0,0,0,0,0,0,0,0};

extern "C" int ipxlisten(int);

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

//Graphics
char *font,*font2,*font3,*font4,*font5,*font6;
char *screen,*background;

static unsigned timeout;

void ReceivePackets();


//BAckground
bground bg;
color pal1[256],pal2[256];
int fadelevel, oldfadelevel,fadein,fadeout;
int gigerlevel,oldgiger,dgiger;

effect effects[MAXMARKS*2];
int stickyblood=0;

//words on screen
int  messagedur,framecheck=0;
char *message,msg[60];

//Sound
int sbstat;
int musicstat;
int nodomusic=0,nodosound=0,doawe32=0;

SOUND sysse[16];
SOUND sse[2][7];


//System
volatile char kbscan;
volatile int quit=0,pause=0,kbstat=0,done=0;
volatile int kbint=0;
volatile unsigned int uu,su,fu;
void (interrupt far *oldkeybintvector)(void);

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

//players
int gamestat;
int sz,z;
int newframe;
playerstat p[2];
playerstat ball;

int score[2];
int wins=0,losses=0;

char *cnlist[]=
 {"KLUBBOR", //0
   "", //1
   "", //2
   "", //3
   "", //4
   "", //5
   "", //6
   "", //7
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

void vector::Tick(int stat)
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
 p.x +=v.x;
 if (v.x>0) {if (v.x>mv.x) v.x=mv.x;}  //max velocity slow down
    else    {if (v.x<-mv.x) v.x=-mv.x;}  //max velocity slow down
       
 if (p.x<boundmin.x) {p.x=boundmin.x; v.x=0;}
 if (p.x>boundmax.x) {p.x=boundmax.x; v.x=0;}

}
if (v.y)
{
 p.y+=v.y;
 if (v.y>0)
   {if (v.y>mv.y) v.y=mv.y;}  //max velocity
    else
   {if (v.y<-mv.y) v.y=-mv.y;}  //max velocity
 
 if (p.y<boundmin.y)  {p.y=boundmin.y;  v.y=0;}
 if (p.y>boundmax.y)  {p.y=boundmax.y; v.y=0;}
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
 { {0x0B00,0x0B00}
 };
 
lpoint deccel[]=
 { {0x0600,0x0600}
 };
 
lpoint maxvelocity[]=
 { {0x12000,0x12000}
 };

lpoint boundminimum[]=
 {
   {(-22<<16),(40<<16)}
 } ;

lpoint boundmaximum[]=
 {
   {(-15<<16),(180<<16)}
 } ;
 



//Misc
unsigned bytes;
int h;
cfg *options;
int i,j,k;

int random(int x)
{
 return((x*rand())/32768);
}



//*************************

void main(int argc, char *argv[])
{
    
printf("\n      Noggin Knockers 2 Beta V%s\n",VERSION);
printf("      Copyright 1996 Bloodlust Software \n\n");

for (i=1; i<argc; i++)
 {
  if (!stricmp(argv[i],"nomusic"))
    { printf("-music disabled\n"); nodomusic=1;}
  if (!stricmp(argv[i],"nosound"))
   {printf("-digital audio disabled...\n");  nodosound=1;}
  if (!stricmp(argv[i],"ipx"))  doipx=1;
  if (!stricmp(argv[i],"bimalkpaul"))  ipxmaster=1; 
 }
GetConfig();

//randomize from timer
srand(*((unsigned int *)(0x6c+0x400)) );

if (doipx)
 {
  printf("-initializing ipx...");
  if (!InitIPX()) printf("successful.\n");
   else {printf("failed\n"); return;}
  multiinstalled=1;
  multiopponent=0;
 
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
  init_mixing();
  printf("done\n");
 }

//Initialize Timer
cprintf("-initializing timer...");
InitializeTimers();
SetTimerSpeed(100);
printf("done\n");

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
   } 
}
 


//set options
set_sound_volume(32*options->soundvolume+31);
set_music_volume(32*options->musicvolume+31);

//set up input devices
cprintf("-initializing input devices...");
p[0].in.Init(0); p[1].in.Init(1);
printf("done\n");


cprintf("-initializing keyboard...");
InitKeyboard((void ( *)())Keyboard);
printf("done\n");


cprintf("-allocating screen buffers...");
background=(char *)malloc(65000);
screen=(char *)malloc(70000);
screen+=2000;
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

printf("done\n");

cprintf("-initializing effects...");
InitializeEffects();
printf("done\n");

p[0].Initialize(0,0);
p[1].Initialize(0,0);
ball.Initialize(10,0);


Mode256();

//full loop
do
{

//assume both human player
p[0].ctype=1; p[1].ctype=1;
ball.ctype=4; //ball is different than everything

if (multiinstalled)
{
 multiopponent=ChatScreen();
 if (!multiopponent)  goto fuckthis;
 multiplay=1; p[1].ctype=2; //remote player

 IPXGameMode((char *)&multiopponent->a.nodeadd,ipxoffset);  //set up ipx for games
}



bg.ReadBackground("nogginbg.vol",200);
//for (i=1; i<100; i++) bg.ylist1a[i]=bg.ylist1a[0];

p[0].po=&p[1]; p[1].po=&p[0];
score[0]=score[1]=0;

PlayGame();


StopVoice();


bg.KillBackground();

if (multiinstalled) //go back to chatscreen
{
      quit=0;
   if (score[0]>score[1]) wins++;
   if (score[0]<score[1]) losses++;
}      


} while (!quit);

fuckthis:
ModeText();

printf("Copyright (C) 1996 Bloodlust Software. All rights Reserved\n");


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



}

//**********************************

lpoint start[]={ {-15<<16,100<<16}, {330<<16,100<<16}, {160<<16,100<<16} };
lpoint ballinit[]={{0,0},{0x0001,0x0001},{0xA0000,0x20000},{-30<<16,8<<16},{350<<16,192<<16}};

void PlayGame()
{
for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;
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

gamestat=-2; //pregame

//multplayer shit
ready=1;                         //flag to tell whether or not to proceed in game
multimaster=1;
if (multiplay)      //flag that tells whether or not we are playing multiplayer
 {
  multiconnected=0; ready=0;       
  multimaster=1;
  packetrecvs=packetsends=packeterror=0;
  sendhit=0;
 } 



SetTimerSpeed(100);
timerhandler=TickUpdate;
su=1; uu=1;
bg.scrx=160;

do
{
if (p[0].ctype==1) p[0].in.ReadPosition(0);
if (p[1].ctype==1) p[1].in.ReadPosition(0);

//bg.DrawBackground(screen);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen,160, 0, 0, gigerlevel/4  ,  bg.scrx,  0,  0,   0+2, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen,160, 0, 0, 100+gigerlevel/4, bg.scrx, 100,  0,  100+2, bg.ylist1a, bg.ylist2);

char s[30];



//draw behind effects
for (effect *eptr=&effects[0]; eptr<&effects[MAXMARKS]; eptr++) eptr->Draw();

if (ready)
{
p[0].DrawFrame();
p[1].DrawFrame();
//if (ball.tcounty<700)
//  ball.DrawFrame();
}

//draw front effects
for (eptr=&effects[MAXMARKS]; eptr<&effects[MAXMARKS*2]; eptr++) eptr->Draw();


if (framecheck) //framecheck status
{
sprintf(s,"GameStat %d BallX %d BallY %d sendhit %d",gamestat,ball.x,ball.y,sendhit);
DrawStringSP(font4,screen+90*320,(unsigned char *)s,320);

}


//IPX STUFF*******************************************
//do com connection

if (multiplay)
{
if (!ready)
 DrawString(font2,screen+5+80*320,(unsigned char *)"WAITING FOR OPPONENT",320);

}

sprintf(s,"%02d",score[0]);
DrawStringSP(font2,screen+12*320+30,(unsigned char *)s,320);
sprintf(s,"%02d",score[1]);
DrawStringSP(font2,screen+12*320+320-30-GetStringWidth(font2,(unsigned char *)s),(unsigned char *)s,320);


UpdatePalette();
MemoryCopy((char *)(0xA0000+2*320),screen+2*320,320*192);

if (quit) fadeout=1;

if (gamestat==-2)
 if (ready)
   {
    ball.x=160;
    ball.y=100;
    ball.Reset();
    gamestat++;
   }

if (gamestat==-1) //pregame
 {
    kbscan=0; ball.v.v.x=0xC000; ball.v.v.y=0xC000;
    ball.d=0;
    gamestat++;

    if (multiplay) {ball.d=multimaster; if (ball.d) ball.v.FlipX();}
 }

if (gamestat==3)
 {
   kbscan=0;
   ball.v.Reset(ballinit[0],ballinit[1],ballinit[2]);
   ball.d=0;
   gamestat=-2;
 }



} while (!done);


multiplay=0;
if (multiinstalled)
{
  WaitIPXPacket();
  ResetIPX();
}  

timerhandler=0;

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
          PlaySE( ((frametype *)fp)->se,0,1,0 );
         }
      break;
      case 2:
        if (ball.d) break; //only use outgoing ball shit
        ball.x=320-((ballpacket *)ipd)->x;
        ball.y=((ballpacket *)ipd)->y;
      break;
      case 3:  //hit thing
       if (!ball.d)  //only use when ball is going away
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
uu++;


//fading in and out
if (fadein)
 {if (fadelevel<32) fadelevel++; else fadein=0;}

if (fadeout)
 {if (fadelevel>0) fadelevel--; else done=1;}
gigerlevel=uu&63;
if (uu&64) {gigerlevel^=63; }
gigerlevel>>=1;

 // make bloody effects
for (effect *eptr=effects; eptr<&effects[MAXMARKS*2]; eptr++)
         if (eptr->e) eptr->Tick();

//scroll background
bg.scrx+=1;
if (bg.scrx>320) bg.scrx=0;

if (ready && !sendhit) //dont do anything during hit sends
{
z=0; p[0].Update();
z=1; p[1].Update();

//z=2; ball.Update();
}

RefreshInput();
p[0].in.ReadButt(0);
p[1].in.ReadButt(0);
}  


#define DONEATTACK 1

void vector::SlowX(int r)
{
if (v.x<=0) a.x+=r;
     else a.x-=r;
}

void vector::SpeedX(int r)
{
if (v.x>=0) a.x+=r;
     else a.x-=r;
}

 

void TapHit(playerstat *p,playerstat *b,int dy)
{
    p=p;
int dvy=-dy*0x200;

//same signs, so decrease x
if ((dvy>0 && b->v.v.y>0) || (dvy<0 && b->v.v.y<0))
 {
  b->v.SlowX(abs(dvy>>4));
 } else b->v.SpeedX(abs(dvy>>4));

b->v.v.y+=dvy;

}    

void SpecialMove(playerstat *p,playerstat *b,int dy)
{
    p=p; dy=dy;
int dvy=0x2500;
b->v.SpeedX(abs(dvy));


b->v.v.y+=dvy;  //reduce y movement
b->v.v.y/=2;
}    


void SuperMove(playerstat *p,playerstat *b,int dy)
{
        p=p; dy=dy;
int dvy=0x3000;
b->v.SpeedX((abs(dvy)<<3)+0x1000);

b->v.v.y+=dvy;  //reduce y movement
b->v.v.y/=4;
}    


int HitCheck(playerstat *p,playerstat *b)
{
 if (!(*p->arectptr) || !(*b->vrectptr)) return(0);
 if (p->stat&DONEATTACK) return(0);

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
  p->stat|=DONEATTACK;

  //make ball reflect
  if (p->d!=b->d)  //not facing same direction
   {
    b->d^=1;
    b->v.v.x=-b->v.v.x; //turn around
    b->v.a.x=-b->v.a.x; 
   }

  //push player back a bit
  if (!b->d) p->v.v.x-=b->v.v.x/2;
        else p->v.v.x-=b->v.v.x/2;
   
  //make it hit top/bottom of rectangles
  if (b->v.v.y>0 && (vr.y1<ar.y1)) b->v.v.y=-b->v.v.y; else //going down 
  if (b->v.v.y<0 && (vr.y2>ar.y2)) b->v.v.y=-b->v.v.y; else //going up
   //place ball at the end of the attack rectangle
  if (!p->d)
    { b->v.p.x+=(ar.x2-vr.x1)<<16;}
  else
    { b->v.p.x+=(ar.x1-vr.x2)<<16; }

 int dy=(ar.y2+ar.y1)/2; //center of attack, y coordinate
 dy-=b->y;    //find relative y location 

//make blood on power hit
 if (p->cm==6) //special move
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

  SuperMove(p,b,dy);  //execute special move on ball

 } else
 {
  GenerateEffect(0,b->x,b->y,0,0,p->d,1);     
  p->cm=5; p->newmove=1;
  TapHit(p,b,dy);  //just tap hit it
 }
  
  //make scream sound
  PlaySE( (8|(random(4)+1))<<4  ,1,2,b->x );

  //make ouch move
  b->cm=1; b->newmove=1;

  if (multiplay)
   {
    hitp.b=b->v; //send over complete vector
    sendhit=1; //send condition of this hit to other player
               //dont do anything until it has been acknowledged
   }            
  return(1);
 } 

return(0);  

}    



void playerstat::Update()
{
int i;
    
//update our vector
if (ctype==1) //human player
{
 v.Tick(in.stat); x=v.p.x>>16; y=v.p.y>>16;

 if ((in.stat&16) && cm<3) {cm=6;  newmove=1;}
 if ((in.stat&32) && cm<3) {cm=7;  newmove=1;}
 in.stat&=15;

 if (in.stat&8  && cm!=2 && cm<3) {cm=2; newmove=1;} else
 if (in.stat&4  && cm!=1 && cm<3) {cm=1; newmove=1;} else
 if ((in.stat&12)==0  && cm!=0 && cm<3) {cm=0; newmove=1;}

 HitCheck(this,&ball);

 if (multiplay)
 {
  playerp.x=(short)x; playerp.y=(short)y;
  playerp.frame=(unsigned short)(frameptr-frame);
//  SendIPXPacketFixed(1,(char *)&playerp,sizeof(playerp));
  
 }
}

if (ctype==2) //computer player
 {

  return;
 }
 
if (ctype==4) //if ball
{
/* if (tcounty==0) if (y>200) y++; else tcounty++;
    else
 if (tcounty<500)  tcounty++;
    else
  if (tcounty<700)   {y++; tcounty++;}
 */
} 




if (dur && !newmove) dur--; //animate
 else
  {
   if (!newmove) //increase frame
    { currframe++; frameptr+=((frametype *)frameptr)->size; } 

   newframe=1;         //We moved to a new frame

   if ( (currframe>=numframes) || newmove)
    {
     if (!newmove) //see if move was completed
      {
       if (ctype==4) cm=0; //ball always goes back to stance
        else
          if (cm>=3) //if not up/down/stance
            {
             if (cm<5 && ctype!=2)
              {gamestat++;//if win/loss pose
               senddonepose=1;
              }
             cm=0;
            } 
      }
     frameptr   =&frame[moves[cm].firstframe];
     numframes=moves[cm].numframes;
     currframe  =0; newmove=0;
    } 

   stat&=~(DONEATTACK);     
   dur=((frametype *)frameptr)->dur;
   newmove=0; GetRect();

   //Make sound effect
   PlaySE( ((frametype *)frameptr)->se,0,z,x );
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
cm=0;
throwstat=0;

frameptr   =&frame[moves[cm].firstframe];
currframe  =0;
numframes=moves[cm].numframes;
dur=((frametype *)frameptr)->dur;
tcountx=0;
tcounty=0;


maxenergy=energy=5;
fatality=0;

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
         if (en>=7 && en<=10) eptr->maxy=195;
         if (en>=12 && en<=14) eptr->maxy=195;
         if (en>=42 && en<=47) eptr->maxy=195;
         eptr->maxy+=random(6);

         if (p) eptr->maxy-=15;


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
        if (!(kbstat&1)) //Control
             for (ki=0; ki<2; ki++)
                 p[ki].in.ReadPosition(0);

        if (!(kbscan&0x80))
        {
   
        if (!(kbstat&1))     //----------------NOT CONTROL
        {
            //check keyboard buttons
             for (ki=0; ki<2; ki++)
              if (p[ki].in.type>=3 && p[ki].in.type<=4 && !kbstat )
                 {
                        ka=(p[ki].in.type&1)^1;
                        if (oldscan>=keybutton[ka][0] && oldscan<=keybutton[ka][1]) {p[ki].in.stat|=16; p[ki].kbmove=oldscan-keybutton[ka][0]+1; p[ki].in.but|=1; }
                        if (oldscan>=keybutton[ka][2] && oldscan<=keybutton[ka][3]) {p[ki].in.stat|=32; p[ki].kbmove=oldscan-keybutton[ka][2]+1+3; p[ki].in.but|=2; }
                 }
            //Check key
             if (kbscan==1) quit=1;
        } else   //if control
        {
          if (kbscan==21) framecheck^=1; //y

        }
}

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

playerstat *player=&p[pl]; if (p[pl].ctype==4) player=&ball;

SOUND *s;
if (!(se&8)) s=&sse[type][ (se&7) -1];       //It's a standard se
          else    s=&player->se[type][ (se&7) -1];  //It's a player specific se
if (s->soundptr)  PlayMixedSoundEffect(s,8);

}


void effect::Draw()
{
_disable();
if (!e)
  {
       _enable();
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
/*else
{
//  drawraw=1;  
  while ( i->index!=0xFF )
  {
   PutStickyImage(eimagelist[i->index],tx+((signed int) (signed char)(i->dispx[td])),ty+((signed int) (signed char)(i->dispy) ),i->orient^(td<<1));
   i++;
  }
}*/
 

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

/*
 if (!multiplay || d) //if single player, or multi and coming towards local
 {
  int result=v.Bounce();
  x=v.p.x>>16; y=v.p.y>>16;
  if (!gamestat) //check for weener
  {
   if (x>345 && !multiplay) //p0 wins
    {p[0].cm=3; p[0].newmove=1; p[1].cm=4; p[1].newmove=1; gamestat=1; score[0]++;}
   if (x<-25) //p1 wins
    {
     gamestat=1;
     p[0].cm=4; p[0].newmove=1; //make us lose
     if (multiplay) sendloss=1; //we must have lost
       else
       {p[1].cm=3; p[1].newmove=1; } //force guy to win
     score[1]++;  
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
  if (abs(v.v.x)>0x10000)  
   if (!(uu&31))
    GenerateEffect(18,x,y,0,0, 0,0);

  if (multiplay) //send coords
   {
     ballp.x=(short)x; ballp.y=(short)y;
//     SendIPXPacketFixed(2,(char *)&ballp,sizeof(ballp));
   }  
 }


//IPX SHIT
if (multiplay)
{
if (uu>timeout)
  {multiconnected=0; //disconnect
      ready=0;}    

//try and connect if we aren't connected    
if (!ready)// && !(uu&31))
  SendIPXPacketFixed(0,(char *)&connectp,sizeof(connectp));       //we wanna be master
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
  if (ball.d)
   SendIPXPacketFixed(2,(char *)&ballp,sizeof(ballp));
 } 

RelinquishIPX();

ReceivePackets();
}

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
sprintf(s,"%05d:%d ",pcount[i],i);
DrawStringSP(font4,screen+i*9*320,(unsigned char *)s,320);

 }
 
}


 */
 
