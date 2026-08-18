#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <direct.h>
#include <dos.h>

void SetCDVolume(unsigned int x);
#define NC 12


//#define REVOL
#define CE

//#include "sb2.h"
#include "tgraph.h"
#include "tinput.h"
#include "time.h"
#include "tkeyb.h"

//#include "ll_comm.h"
#include "tipx.h"
#include "tpacket.h"


extern int BMAC,black,spin;
extern int cdplay;
void PlayCDTrack(int x);
void StopCD();
void ChatScreen();

typedef struct vector
{
public:
 int x,y;      //position of vector
 int vx, vy;   //velocity (high word)
 int ax, ay;   //acceleration (high word)

 void Tick();
 void Reset(int nx, int ny);
} vector;

void vector::Reset(int nx, int ny)
{
 x=nx<<16; y=ny<<16;
 vx=0; vy=0;
 ax=0; ay=0;
}

void vector::Tick()
{
vx+=ax; vy+=ay;
x +=vx;  y+=vy;
if (x<0)   x=(320<<16);
if (x>= (320<<16)) x=0;
}


/*
extern "C"
{
void InitCOM(int addr,int port,int vector,int IRQ,void (*kfunc)());
void TerminateCOM();
void SetCOMFunc(void (*kfunc)());
void SendByte(int x);
}
extern int BASE;
extern volatile char databyte,dataready;
extern void ModemFunc();
                                          */

unsigned int imageX(char *i);
unsigned int imageY(char *i);

extern bground bg;

extern int chatstat; //not chatting
extern int chatptr;
extern char chatstr[128];
extern char chatrecvstr[128];
extern int chatrecvstat;

static char cmsg[50];
static int cmsgdur;


extern int sestat;
int uglypalette=0;      //0-none 1-single 2-mixed

extern int  messagedur;
extern char *message,msg[60];



extern char *screen;
extern char *background;

extern volatile char kbscan;
extern volatile int quit;
int copyprotect;

extern char *font,*font2,*font3,*font4,*font5,*font6;


extern int death;
extern char *cnlist[];

color pal1[256],pal2[256];

//extern char *cmf,*musicinst,*musicdata;

int ReadRawFile(char **m,int h);

extern volatile unsigned int  uu;
extern int sbstat;
extern playerstat p[2];
extern int game,debug,kbstat;

extern cfg *options;
extern int fstate;
int fadelevel,oldfadelevel;

void UpdatePalette();

int volumize;

int gigerlevel,oldgiger,dgiger;

int b2x,b3x;

char *cmf;
SOUND vocname[NC];
char *bio[NC][6];
char *smark[2];
char *corner;



int m[]={0,6,5,3,1,4,2,7, 8,11,10,9,  12,13,14,15,  16,17,18,19};


SOUND voc1,voc2,voc3,voc4;

extern int cstat,strobe;

int ch[2];     //Actual number character
int pch[2];    //Position of character targeter
int ostat[2];


int bioexist[NC];  //if a certain character exists
int bgexist[NC];   //if a certain background exists


//void PlayMixedSoundEffect(VOC far *se,int priority);
void ReadSelectFiles();
void SelectTick();
void DrawSmark(int,int);
void DrawBioImage(char *src,char *dest,int o);
void Fade(int);
void DrawGraphImage(char *src,char *dest,int o);
void UpdatePalette2();

int sshake;

int chlock[2];
int lockconfirm;

//unsigned long uu;


int fadeout,done,fadein;

void Lock(int i)
{
 if (chlock[i]) return;    //we are already locked, fuck it

 if (bioexist[ch[i]]) //lock if exist only
    {
     chlock[i]=1;     //locked on the character
     PlayModalSoundEffect(&voc2);
     sshake=20;
     if (chlock[i^1] && ch[0]==ch[1])   options->color[i]=1;
    }
}

//extern void DrawSoundCheck();


void Grey(char *s,int xw,int yw)
{
 int i,j,k;
 for (i=0; i<yw; i++)
 {
  for (j=0; j<xw; j++,s++)
         {
          k=(pal2[*s].r+pal2[*s].g+pal2[*s].b)*16/(256*3);
          *s=31-4-k;
         }
  s+=320-j;
 }
}


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
                  *s=31-4-k;
                  s++;
                 }
                i-=a;
         }
 }
}



}

extern int frenchy;
void DrawStringClip(char *font,char *dest,unsigned char *s,int x, int y);





//check to see if a particular file exists
int existfile(char *s)
{
int h;    
if (_dos_open(s,O_RDONLY | O_BINARY,&h)) return(0);
_dos_close(h);
return(1);
}    


void CheckExistence()
{
  for (int i=0; i<NC; i++)
        {
         char s[20];
         strcpy(s,cnlist[i]);
         strcat(s,".vol");
         if (existfile(s))  {/*printf(s);*/ bioexist[i]=1; }
             else          bioexist[i]=0;

/*         strcpy(s,bgnlist[i]);
         strcat(s,".vol");
         if (existfile(s)) { bgexist[i]=1;}
             else          bgexist[i]=0;*/
        } 
}    


typedef struct code
{
int ptr[2]; //current direction that has been reached (start with 0)
char codetrue[2];
char d[]; //the directions for the code, 100 ends

void Update(int p, char pstat);
} code;    

#define NUMCODES 11
#define L 1
#define R 2
#define U 4
#define D 8
        //                       DURL

code stainecode={{0,0}, {0,0}, {D,R,R,R,U,L,L,L,D,U,R,R,R,D,L,D,L,U,L,D,100}};
code portalcode={{0,0}, {0,0}, {D,R,D,R,U,R,U,L,L,L,R,R,R,D,D,100}};
code deathmodecode={{0,0}, {0,0}, {R,R,R,L,L,L,R,R,R,L,L,L,R,R,R,R,R,R,R,R,R,R,100}};
code spincode={{0,0}, {0,0}, {U,D,L,R,U,D,L,R,U,D,L,R,U,D,L,R,U,D,L,R,U,D,L,R,100}};
code uglycode={{0,0}, {0,0}, {D,U,D,U,D,U,D,U,D,U,R,R,R,L,L,L,R,R,R,R,100}};
code strobecode={{0,0}, {0,0}, {R,R,R,R,R,R,R,R,R,R,L,L,L,L,L,L,L,L,L,L,U,D,100}};
code buddycode={{0,0}, {0,0}, {U,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,100}};
code ravagecode={{0,0}, {0,0}, {D,D,R,D,D,R,D,U,R,D,100}};

code guillotinecode={{0,0}, {0,0}, {D,U,D,D,U,D,R,L,R,L,D,U,R,U,U,D,D,100}};
code reapercode={{0,0}, {0,0}, {R,R,R,L,L,L,R,R,R,U,D,U,D,L,R,U,R,D,100}};
code edcode={{0,0}, {0,0}, {L,R,L,R,L,R,L,R,L,R,U,U,U,D,U,100}};


code *codes[NUMCODES]= { &portalcode,&stainecode, &deathmodecode, &strobecode, &uglycode, &spincode, &buddycode, &ravagecode,&guillotinecode,&reapercode,&edcode  };
  

void code::Update(int p, char pstat)
{
 if (pstat) //if something is pressed
    if (pstat==d[ptr[p]])
       {ptr[p]++;  if (d[ptr[p]]==100) codetrue[p]=1;}
    else ptr[p]=0;
}    



int tbx,tby;

vector vb3,vb2,vb1,vbf;
vector vbjerk;



//STUFF FOR NETWORK/MODEM
#define SSCONNECT 0x0
#define SSCONFIRM 0x1
#define SSCHANGE  0x2
#define SSLOCK    0x3
#define SSQUIT    0x4


void sschange()
  {
     pch[1]=((packet *)prbuf)->data;  ch[1]=m[pch[1]]; //set their cursor pos
     PlayModalSoundEffect(&voc1);
  } //change

void sslock()
  {
     pch[1]=((packet *)prbuf)->data;  ch[1]=m[pch[1]]; //set cursor position
     if (!bioexist[ch[1]]) //doesn't exist, tell them that
       {  
         SendStandardPacket(SSCONFIRM,0); //nope, doesn't exist
         sprintf(cmsg,"Missing character file: %s.vol",cnlist[ch[1]]);
         cmsgdur=1000;
       } 
      else //confirm it
       {
        SendStandardPacket(SSCONFIRM,1); //okay, its been locked
        strcpy(cmsg,"**Received Lock**");
        cmsgdur=100;
        Lock(1);
       } 
  } //lock

void ssquit() {quit=1;}  

void ssconfirm()
 {
  if (((packet *)prbuf)->data)
    {
     Lock(0); //they acked it
     strcpy(cmsg,"**Lock Acknowledged**");
     lockconfirm=1;
     cmsgdur=100;
    } else
    {
     strcpy(cmsg,"Your opponent does not have that character installed.");
     cmsgdur=1000;
     chlock[0]=0; //remove lock
    }
       
 }  

void ssconnect()
{

multimaster=((packet *)prbuf)->data^1; //we are opposite of type sent     
multiconnected=1; 
packetrecvs=packetsends=packeterror=0;

if (!multimaster) 
 SendStandardPacket(SSCONNECT,multimaster);       //tell them our status


SetPacketFunc(SSCHANGE,sschange);
SetPacketFunc(SSLOCK,sslock);
SetPacketFunc(SSCONFIRM,ssconfirm);
SetPacketFunc(SSQUIT,ssquit);
  

} //we are connected!

void Nofile(char *s);

void SelectScreen()
{
int i,j; 

for (i=0,j=0; i<NC; i++)
  j+=bioexist[i];

if (!j) Nofile("No character files installed.");  

 //clear backgrounds
 Solid(background,64000,0);
 MemoryCopy((char *)0xA0000,background,64000);

 //read data files
 ReadSelectFiles();

  //Set up background
  kbscan=quit=0; //cstat=0;

  p[0].d=p[1].d=0;
  p[0].in.Reset(); p[1].in.Reset();
  pch[0]=0; pch[1]=3; if (multiplay) pch[1]=0;        //position of cursors
  ch[0]=m[pch[0]]; ch[1]=m[pch[1]];  //cooresponding character
  uu=0;
  messagedur=0; 
  

  ostat[0]=0;   ostat[1]=0;          //old joystick position
  chlock[0]=chlock[1]=0;         //whether or not pointer is locked in
  lockconfirm=0; //whether or not the lock needs confirmation
  if (!multiplay) lockconfirm=1;
  fadeout=0; fadein=1; done=0;       //tell to fade in...
  sshake=0;

  cmsgdur=0;

  options->color[0]=0;
  options->color[1]=0;

    //select characters if computer is playing them
  for (i=0; i<2; i++)
        if (p[i].ctype==3)   //if computer character, keep it locked on its player
         {
                ch[i]=options->pnum[i];
                for (pch[i]=0; m[pch[i]]!=ch[i]; pch[i]++);
                chlock[i]=1; //fully locked in baby
         }

  oldfadelevel=-1; gigerlevel=0; oldgiger=-1; dgiger=1;
  fadelevel=0;
  UpdatePalette2();

  quit=0;


  //set up background
  vb3.Reset(0,0);  vb3.vx=(1<<16)/4; //far
  vb2.Reset(0,0);  vb2.vx=(1<<16)/2; //middle
  vb1.Reset(163,0);  vb1.ax=(1<<16)/100; //background //159
  vb1.vx=0; //(1<<16)/5/2;

  vbf.Reset(150,0);


  if (multiplay)
   {
    multiconnected=0;
    multimaster=1;
    SetPacketFunc(SSCONNECT,ssconnect);
   }

  //Set up our timer
  if (cdplay) PlayCDTrack(15);
   else
  if (musicstat) start_music(bg.music,1);
  timerhandler=SelectTick;


  do
  {
 UpdatePalette2();

 //Draw background
 bg.scrx=vb1.x>>16; bg.scry=vb1.y>>16;
 PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, bg.scrx, 0, vb2.x>>16,   0,  vb3.x>>16,  0,  0,   0, bg.ylist1a, bg.ylist2);
 PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,screen, bg.scrx, 0, vb2.x>>16,  100, vb3.x>>16, 100,  0,  100, bg.ylist1b, bg.ylist2);


//Draw People
 if (bioexist[ch[0]])
     {
       DrawBioImage(bio[ch[0]][(uu&32) ? 1 : 0],screen,0);

       int w=GetStringWidth(font2,(unsigned char *)cnlist[ch[0]]);
       DrawString(font2,screen+97*320+160-bg.scrx+(7+149-w)/2,(unsigned char *)cnlist[ch[0]],320);

       DrawGraphImage(bio[ ch[0] ][3],screen,0);
     }
 if (bioexist[ch[1]])
    {
       DrawBioImage(bio[ch[1]][(uu&32) ? 0 : 1],screen,1);

       int w=GetStringWidth(font2,(unsigned char *)cnlist[ch[1]]);
       DrawString(font2,screen+97*320+160-bg.scrx+(170+312-w)/2,(unsigned char *)cnlist[ch[1]],320);
       DrawGraphImage(bio[ ch[1] ][3],screen,1);
    }

DrawImage(corner,screen,(160-bg.scrx-1),188,0);
DrawImage(corner,screen,(160-bg.scrx)+320-imageX(corner),188,2);
          

//Draw bg images
series *bp;
/*=bg.bgs;
 for (i=0; i<=bg.nums; i++,bp++)
  {
        bgframe *f=(bgframe *)bp->frameptr;
        if (f->index!=0xFF && bp->numframes)
          DrawImage(bg.bgimagelist[f->index],screen,f->x-bg.scrx,f->y-bg.scry,0);
  }
  */


//Draw foreground images
//      bg.DrawFGImages(screen);
tbx=(vbf.x)>>16;
tby=(vbf.y)>>16;
if ((sshake&2)) if (random(2)) tbx+=2; else tbx-=2;
if (!(sshake&4)) tby+=2;
bp=bg.bgfs;
 for (i=0; i<=bg.numfs; i++,bp++)
  {
        bgframe *f=(bgframe *)bp->frameptr;
        if (f->index!=0xFF && bp->numframes)
                //       DrawImage(bg.bgimagelist[f->index],screen,0,0,0);
  DrawImage(bg.bgimagelist[f->index],screen,f->x-tbx,f->y-tby,0);
  }
//draw people little bios
  for (i=0; i<4; i++)
        {
         int x=160-tbx+100+i*30+1;
          if (bioexist[m[i]])
                 DrawImage(bio[ m[i]   ][2],screen,x,4-tby, 0);
          if (bioexist[m[i+4]])
                 DrawImage(bio[ m[i+4] ][2],screen,x,32-tby,0);
         if (i==1 || i==2) if (bioexist[m[i+8]])
                 DrawImage(bio[ m[i+8] ][2],screen,x,60-tby,0);
        }

//Draw selection marks
if (!multiplay || multiconnected)
 if (uu&8)
   {
    DrawSmark(1,1^chlock[1]);
    DrawSmark(0,0);
   } else
   {
    DrawSmark(0,1^chlock[0]);
    DrawSmark(1,0);
   }

/*    char s[30];
  sprintf(s,"TRIG %5X",p[0].in.triggered);
  DrawString(font,screen+100*320+10,s,320);
  sprintf(s,"BUT0 %5X",p[0].in.butdur[0]);
  DrawString(font,screen+120*320+10,s,320);
  sprintf(s,"BUT1 %5X",p[0].in.butdur[1]);
  DrawString(font,screen+140*320+10,s,320);
  sprintf(s,"STAT %5X",p[0].in.stat);
  DrawString(font,screen+156*320+10,s,320);
  */

RefreshInput();

//DO MODEM PLAY STUFF
//COM ----------------------------------------------------
if (multiplay && !multiconnected)
{
 if (uu&64)  DrawString(font2,screen+5+80*320,(unsigned char *)"WAITING FOR OPPONENT",320);
 if (!(uu&63)) SendStandardPacket(SSCONNECT,multimaster);       //we wanna be master
}
 else
 //move around cursors
 for (i=0; i<2; i++)
   if (!chlock[i] && p[i].ctype==1) //if not locked on yet and we are local controlled...
         {
          //Move cursor around
          p[i].in.ReadPosition(0);
          p[i].in.ReadButt(i);

          if (p[i].in.stat!=ostat[i])    //If control has moved
                {

                 int s=p[i].in.stat; s&=~ostat[i];
                 int och=pch[i];
                 if (ch[i]<8  || ch[i]==10 || ch[i]==11)
                 {
                 if (s&2 && ((pch[i]<8 && (pch[i]&3)!=3) || (pch[i]>=8 && pch[i]<=9))) pch[i]++; //Right
                 if (s&1 && ((pch[i]<8 && (pch[i]&3)!=0) || pch[i]>=10)) pch[i]--; //Left
                 if (s&4 && (pch[i]>=4 )   ) pch[i]-=4;  //Up
                 if (s&8 && (pch[i]<8) && !(pch[i]>3 && ((pch[i]&3)==0 || (pch[i]&3)==3)) ) pch[i]+=4; //down
                 }
                 ch[i]=m[pch[i]];

                  //if they pushed a button (and it exists)
                 if (s&(16+32) && bioexist[ch[i]])
                    {
                     Lock(i); 
                    }

                  //make the movement sound
                 if (och!=pch[i])
                  {
                     PlayModalSoundEffect(&voc1);
                       
                     if (multiconnected) ///send new character position 
                       SendStandardPacket(SSCHANGE,pch[i]);
                  }

                 //check movement againts code
                 for (int j=0; j<NUMCODES; j++)
                  {
                   codes[j]->Update(i,p[i].in.stat);
                   if (codes[j]->codetrue[i]) //if  accomplished
                    {
                     int domessage=0;
                     if (j==0) {pch[i]=11; ch[i]=9;}
                     if (j==1) {pch[i]=8; ch[i]=8;}
                     if (j==2) {death^=1; domessage=death+1; message="DEATH MODE ";}
                     if (j==3) {strobe^=1; black=strobe; domessage=strobe+1; message="STROBE ";}
                     if (j==4) {uglypalette^=1; domessage=uglypalette+1; message="NUCLEAR MODE ";}
                     if (j==5) {spin^=1; domessage=spin+1; message="VOMIT MODE ";}
                     if (j==6) {pch[i]=12; ch[i]=12;}
                     if (j==7) {pch[i]=13; ch[i]=13;}
                     if (j==8) {pch[i]=14; ch[i]=14;}
                     if (j==9) {pch[i]=15; ch[i]=15;}
                     if (j==10) {pch[i]=16; ch[i]=16;}


                      if (domessage)
                       {
                        strcpy(msg,message);
                        strcat(msg,domessage==1 ? "OFF" : "ON");
                        message=msg;
                        messagedur=100;
                       }
                     
                     
                     codes[j]->codetrue[i]=0;
                    }
                  }
                 ostat[i]=p[i].in.stat;
                }

          p[i].in.stat&=0xF;
         }


//DO LOCK STUFF
 //if has been locked but not confirmed, send lock packet
if (multiplay) 
 if (chlock[0] && !lockconfirm)
  {
   if (!(uu&31))  SendStandardPacket(SSLOCK,pch[0]);//send at intervals
   strcpy(cmsg,"**Sending Lock**");
   cmsgdur=100;
  }  




/*
if (multimaster) DrawString(font2,screen+55*320+100,(unsigned char *)"MASTER",320);
   else DrawString(font2,screen+55*320+100,(unsigned char *)"SLAVE",320);

if (multiconnected) DrawString(font,screen+105*320+0,(unsigned char *)"CONNECTED",320);


char s[30];
sprintf(s,"TICKS%5d",uu);
DrawString(font,screen+170*320+200,(unsigned char *)s,320);

sprintf(s,"PS%5d",packetsends);
DrawString(font,screen+40*320+200,(unsigned char *)s,320);
sprintf(s,"PR%5d",packetrecvs);
DrawString(font,screen+55*320+200,(unsigned char *)s,320);
sprintf(s,"PE%5d",packeterror);
DrawString(font,screen+70*320+200,(unsigned char *)s,320);

sprintf(s,"PE0%5d",pe[0]);
DrawString(font,screen+100*320+200,(unsigned char *)s,320);
sprintf(s,"PE1%5d",pe[1]);
DrawString(font,screen+115*320+200,(unsigned char *)s,320);
sprintf(s,"PE2%5d",pe[2]);
DrawString(font,screen+130*320+200,(unsigned char *)s,320);
sprintf(s,"PE3%5d",pe[3]);
DrawString(font,screen+145*320+200,(unsigned char *)s,320);
*/

if (messagedur)
  {
        DrawString(font2,screen+160*320+160-GetStringWidth(font2,(unsigned char *)message)/2,(unsigned char *)message,320);
        messagedur--;
  }


if (chatstat) //draw chat
 {
  char s[49];   
  DrawStringSP(font4,screen+175*320+2,(unsigned char *)"SEND:",320);
  memcpy(s,chatstr+((chatptr-48)<0 ? 0 : (chatptr-48)),48);  s[48]=0;
  DrawStringSP(font4,screen+175*320+30,(unsigned char *)s,320);
  DrawStringSP(font4,screen+175*320+30+GetStringWidthSP(font4,(unsigned char *)s),(unsigned char *)"_",320);
 }

if (chatrecvstat) //someone said something to us
 {
  int sp=0;
  char s[55];
  char *dest=screen+55*320+15;
  while (chatrecvstr[sp])
   {
    int lp=0;  //characters on line
    while (isspace(chatrecvstr[sp])) sp++;
    for (; lp<54 && chatrecvstr[sp+lp]; lp++) s[lp]=chatrecvstr[sp+lp]; //scan to end of a line or nullchar
    if (lp==54) while (lp>0 && !isspace(s[lp])) lp--; s[lp]=0;
    DrawStringSP(font5,dest,(unsigned char *)s,320);
    sp+=lp; dest+=320*9;
   } 
 } 

//standard msges
if (cmsgdur) 
 DrawStringSP(font6,screen+184*320+160-GetStringWidthSP(font6,(unsigned char *)cmsg)/2,(unsigned char *)cmsg,320);


//Copy to screen
 MemoryCopy((char *)(0xA0000+2*320),screen,320*195);


 if (quit || (chlock[0] && chlock[1] && lockconfirm) ) {fadeout=1;fadein=0;}

 if (ipxinstalled)
  {
   while (RecvIPXPacket());
    RelinquishIPX();
  }


}  while(!done);

if (multiconnected && quit) //if we quit
   SendStandardPacket(SSQUIT,0);


if (multiplay)
   {
    if (ipxinstalled)
      {
       WaitIPXPacket();
      } 

    multiconnected=0;
    ResetPacket();
   }


timerhandler=0;

if (cdplay) StopCD();
 else
if (musicstat) stop_music();

UpdatePalette2();

if (sbstat==2) StopVoice();


//set to hidden character
//if (codetrue&1) ch[0]=12;
//if (codetrue&2) ch[1]=12;


//set player characters to what was picked
options->pnum[0]=ch[0];
options->pnum[1]=ch[1];
for (i=0; i<2; i++)
  if (p[i].ctype==3)
         if (ch[i]==ch[i^1]) options->color[i]=options->color[i^1]^1;

if (quit!=1) quit=0;

//Free up memory
free(voc1.soundptr);
free(voc2.soundptr);
free(corner);
for (i=0; i<2; i++) free (smark[i]);
for (i=0; i<NC; i++)
 if (bioexist[i])
         for (j=0; j<4; j++)
          if (bio[i][j]) free(bio[i][j]);
bg.KillBackground();

}




void UpdatePalette2()
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
  if (!fadeout)
  {
  LoadPalette(pal2,0,16*6);          //0->16*16
  LoadPalette(pal2,16*6+1,14*16-(16*6+1));     //16*6+1->14*16
  LoadPalette(pal2,15*16,16);
  }  else LoadPalette(pal2,0,256);

  oldfadelevel=fadelevel;
 }


if (!fadeout && oldgiger!=gigerlevel)
 {
  unsigned int  fl=gigerlevel/2+16;
  unsigned char *p1=(unsigned char *)(pal1+14*16);
  unsigned char *p2=(unsigned char *)(pal2+14*16);

  for (i=0; i<16*3; i++,p2++,p1++)
        *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);

  p1=(unsigned char *)(pal1+6*16);
  p2=(unsigned char *)(pal2+6*16);
  fl^=31;
  for (i=0; i<3; i++,p2++,p1++)
        *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);



  WaitVSync();
  LoadPalette(pal2,14*16,16);
  LoadPalette(pal2,6*16,1);

  oldgiger=gigerlevel;
 }


}


extern int cominstalled;

void  SelectTick()
{
 uu++;


if (cominstalled)  PacketComFunc();

if (chatrecvstat)   chatrecvstat--;


if (sshake) sshake--;

vb3.Tick();
vb2.Tick();
vb1.Tick();
// if ((vb1.vx)>=(1<<16)/5) vb1.ax=-abs(vb1.ax);
// if ((vb1.vx)<=-(1<<16)/5) vb1.ax=abs(vb1.ax);
 if ((vb1.x)> (170<<16)) {vb1.ax=-abs(vb1.ax);}
 if ((vb1.x)< (150<<16)) {vb1.ax= abs(vb1.ax);}
 if (vb1.vx<-(1<<16)/5)  vb1.vx=-(1<<16)/5;
 if (vb1.vx> (1<<16)/5)  vb1.vx=(1<<16)/5;

//vbf.Tick();
// if ((vbf.x)>=(160)<<16 ) vbf.ax=-abs(vbf.ax);
// if ((vbf.x)<=(160)<<16 ) vbf.ax=abs(vbf.ax);

series *bp=bg.bgfs;
 for (int i=0; i<=bg.numfs; i++,bp++)  bp->Tick();
//bp=bg.bgs;
// for (i=0; i<=bg.nums; i++,bp++)  bp->Tick();

 if (fadein)  if  (fadelevel<32) fadelevel++; else fadein=0;
 if (fadeout) if  (fadelevel) fadelevel--; else done=1;


 gigerlevel=uu&63;
 if (uu&64) gigerlevel^=63;
 gigerlevel>>=1;

 if (cmsgdur) cmsgdur--;
}



void Fade(int d)
{
int i,j;

if (d>0) j=0; else j=50;

for (; j>=0 && j<=50; j+=d)
{
 for (i=0; i<256; i++)
        {
         pal2[i].r=pal1[i].r*j/50;
         pal2[i].g=pal1[i].g*j/50;
         pal2[i].b=pal1[i].b*j/50;
        }
 delay(10);
 WaitVSync();
 LoadPalette(pal2,0,256);
}

}








//7 149     142
//169 311   142


void DrawBioImage(char *src,char *dest,int o)
{
int x,y;
int xlen,ylen,xst,yst,clip;

clip=0; //clip flag
xst=yst=0;
xlen=(int) (unsigned char)src[1];
ylen=(int) (unsigned char)src[2];

y=200-ylen; if (y<96) y=(96+200-ylen)/2;

int lb,rb;
if (!o)
 {       //convert everything to bg coords, then make rel to screen
  lb=160+ -1                      - bg.scrx;
  rb=160+ 141                     - bg.scrx;
  if (lb<0) lb=0;     if (rb<=0) return;   //check bounds
  if (rb>320) rb=320; if (lb>=320) return;

  x=160 + (0+142)/2 -  xlen/2     - bg.scrx;
  if (x+xlen>=rb) {xlen=rb-x; clip=1; }
  if (x<lb)  { xst=lb-x; clip=1; x=lb; xlen-=xst;}

  if (y+ylen>=200) {ylen=200-y; clip=1; if (y>=200) return;}
  if (y<96) { yst=96-y; clip=1; if (y+ylen<96) return; y=96; ylen-=yst;}
 }
  else
 {       //convert everything to bg coords, then make rel to screen
  lb=160+ 177                     - bg.scrx;
  rb=160+ 320                     - bg.scrx;
  if (lb<0) lb=0;     if (rb<=0) return;   //check bounds
  if (rb>320) rb=320; if (lb>=320) return;

  x=160+(177+320)/2 - xlen/2      - bg.scrx;
  if (x+xlen>=rb) {xlen=rb-x; clip=1; }
  if (x<lb)  { xst=lb-x; clip=1;  x=lb; xlen-=xst;}

  if (y+ylen>=200) {ylen=200-y; clip=1; }
  if (y<96) { yst=96-y; clip=1; y=96; ylen-=yst;}
 }

if (clip==0) PutImageFlip256(src,dest+(320*y)+x,320,o<<1);
 else  PutImageFlipClip256(src,dest+(320*y)+x,320,xst,yst,xlen,ylen,o<<1);
}

//58  259     68

void DrawGraphImage(char *src,char *dest,int o)
{
int x,y;
int xlen,ylen;

xlen=(int) (unsigned char)src[1];
ylen=(int) (unsigned char)src[2];

y=73-ylen; if (y<0) y=0;

if (!o) x=(160-bg.scrx)+58-7-xlen/2;
else    x=(160-bg.scrx)+259+7-xlen/2;
                                                                                                        //o^=1;
//PutImageFlip256(src,dest+(320*y)+x,320,o<<1);
DrawImage(src,dest,x,y, o<<1);
}









void DrawSmark(int c,int f)
{
 int x=160-tbx+1;
 int y;
 if (pch[c]>=12 || p[c].ctype==3) return;

 if (!c)
        {
          if (pch[0]<4) {x+=99+pch[0]*30; y=0;}
else if (pch[0]<8) {x+=99+pch[0]*30-4*30; y=28;}
         else           {x+=99+pch[0]*30-8*30; y=28*2;}
        }
 else
        {
         if (pch[1]<4) {x+=99+pch[1]*30; y=3;}
 else if (pch[1]<8)       {x+=99+pch[1]*30-4*30; y=31;}
 else {x+=99+pch[1]*30-8*30; y=31+28;}
        }

 if (!f)  DrawImage(smark[c], screen,x,y-tby, 0);
//      else  PutImageSolid(smark[c], screen+x+y*320,320, 0);

}


int lastsize;

char *ReadVolFile(int h)
{
unsigned int bytes;
unsigned  size;
char *p;
  _dos_read(h,&size,4,&bytes);
  p=(char *)malloc(size);
  _dos_read(h,p,size,&bytes);
lastsize=size;  
return(p);
 }

void ReadVolSound(int h,SOUND *s)
{
unsigned bytes;
_dos_read(h,&s->soundsize,4,&bytes);
if (s->soundsize)
  {
        s->soundptr=(signed char *)malloc(s->soundsize);
        _dos_read(h,s->soundptr,s->soundsize,&bytes);
   s->soundsize-=30 ;
  } else s->soundptr=0;
}


char *ReadFile(char *name)
{
 int h;
 unsigned int bytes;
 char *t;

 if (!_dos_open(name,O_RDONLY | O_BINARY,&h))
 {
  lastsize=filelength(h);
 t=(char *)malloc(lastsize);
 _dos_read(h,t,lastsize,&bytes);
 _dos_close(h);
 } else {t=NULL; lastsize=0; Nofile(name);}

 return(t);
}




void ReadSelectFiles()
{
  int i,j;
  int h;
  unsigned int bytes;
  
  unsigned int size;

  #ifdef REVOL
  if (_dos_open("select.vol",O_BINARY | O_RDONLY,&h))
        {
         int sh;
         char *m;
         ModeText();
         printf("revolumizing select.vol....\n");
         _dos_creat("select.vol",0,&sh);

/*       _dos_open("\\ss\\vs.raw",O_RDONLY | O_BINARY,&h);
         size=ReadRawFile(&smark[0],h); _dos_close(h);
         _dos_write(sh,&size,4,&bytes);
         printf("-adding vs.raw %d\n",size);
         _dos_write(sh,smark[0],size,&bytes);*/

         m=ReadFile("\\ss\\versus.rvc");
         _dos_write(sh,m,lastsize,&bytes);
         printf("-adding versus.rvc %d\n",lastsize);

/*       _dos_open("\\ss\\select.raw",O_RDONLY | O_BINARY,&h);
         _dos_read(h,background,filelength(h),&bytes);
         _dos_write(sh,background,64000,&bytes);
         _dos_close(h);
         printf("-adding select.raw %d\n",64000);

*/       _dos_open("\\ss\\select.lbm",O_RDONLY | O_BINARY,&h);
         lseek(h,0x30,SEEK_SET);
         _dos_read(h,pal1,256*3,&bytes);  _dos_close(h);
         for (i=0; i<256; i++) { pal1[i].r>>=2;   pal1[i].g>>=2;   pal1[i].b>>=2; }
         _dos_write(sh,pal1,256*3,&bytes);

         _dos_open("\\ss\\1p.raw",O_RDONLY | O_BINARY,&h);
         size=ReadRawFile(&smark[0],h); _dos_close(h);
         printf("-adding 1p.raw %d\n",size);
         _dos_write(sh,&size,4,&bytes);
         _dos_write(sh,smark[0],size,&bytes);

         _dos_open("\\ss\\2p.raw",O_RDONLY | O_BINARY,&h);
         size=ReadRawFile(&smark[1],h); _dos_close(h);
         printf("-adding 2p.raw %d\n",size);
         _dos_write(sh,&size,4,&bytes);
         _dos_write(sh,smark[1],size,&bytes);

         _dos_open("\\ss\\corner.raw",O_RDONLY | O_BINARY,&h);
         size=ReadRawFile(&corner,h); _dos_close(h);
         printf("-adding corner.raw %d\n",size);
         _dos_write(sh,&size,4,&bytes);
         _dos_write(sh,corner,size,&bytes);

/*       _dos_open("\\ss\\select.mid",O_BINARY | O_RDONLY,&h);
         size=filelength(h);
         cmf=(char *)farmalloc(size);
         _dos_read(h,cmf,size,&bytes); _dos_close(h);
         _dos_write(sh,&size,4,&bytes);
         _dos_write(sh,cmf,size,&bytes);
         farfree(cmf);*/

        voc1.soundptr=(signed char *)ReadFile("\\ss\\change.rvc");
        _dos_write(sh,voc1.soundptr,lastsize,&bytes);
        printf("-adding boom1.rvc %d\n",lastsize);
        free((char *)voc1.soundptr);

        voc1.soundptr=(signed char *)ReadFile("\\ss\\lock.rvc");
        printf("-adding boom2.rvc %d\n",lastsize);
        _dos_write(sh,voc1.soundptr,lastsize,&bytes);
        free((char *)voc1.soundptr);

        printf("done.\n");
        kbscan=0; while (!kbscan);
        Mode256();

 _dos_close(sh);
        }  else _dos_close(h);
  #endif


//  ModeText();
//  printf("opening\n");

  bg.ReadBackground("selectbg.vol",0);

 if (_dos_open("select.vol",O_BINARY | O_RDONLY,&h)) Nofile("select.vol");

//  unsigned short size;
//  _dos_read(h,&size,4,&bytes); //Skip versus
//  lseek(h,size,SEEK_CUR);
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);

//  _dos_read(h,background,64000,&bytes);
  _dos_read(h,pal1,256*3,&bytes);

  smark[0]=ReadVolFile(h);
  smark[1]=ReadVolFile(h);

  corner=ReadVolFile(h);
//  cmf=ReadVolFile(h);
  ReadVolSound(h,&voc1);
  ReadVolSound(h,&voc2);

  _dos_close(h);


//  ModeText();
//  printf("reading character bio pics...\n");
  for (i=0; i<NC; i++)
   if (bioexist[i])
        {
         char s[20];
         strcpy(s,cnlist[i]);
         strcat(s,".vol");

         if ( /*((1<<i)&BMAC) &&*/ !_dos_open(s,O_RDONLY | O_BINARY,&h))
          {
            char key[20];
           _dos_read(h,key,12,&bytes);
            if (stricmp(key,"MIDGETPOWER")) Nofile(s);

                unsigned int size;
                for (j=0; j<5; j++)
                 {
                   _dos_read(h,&size,4,&bytes);
                   lseek(h,size,SEEK_CUR);
                 }
                for (j=0; j<4; j++)
                 {
                   _dos_read(h,&size,4,&bytes);
                   bio[i][j]=NULL;
                        if (size)
                         {
                          bio[i][j]=(char *)malloc(size);
                          _dos_read(h,bio[i][j],size,&bytes);
                         }
                 }

                if ((cstat&(1<<i)))
                  GreyImage(bio[i][2]);
                _dos_close(h);
          } 

}

//printf("done\n");
//kbscan=0; while (!kbscan);
//Mode256();
}



SOUND versus;
int namey[2];
int versusy;
series *svs,*sp[2];
point  svsdisp,spdisp[2];



void VersusTick()
{
 uu++;
 svs->Tick();
 if (sp[0]) sp[0]->Tick();
 if (sp[0]!=sp[1])
  if (sp[1])  sp[1]->Tick();

 if (uu<32) fadelevel=uu;

 if (uu>32 && namey[0]>173) namey[0]-=3;
 if (uu>160 && namey[1]>173) namey[1]-=3;
 //if (!(uu&1))
 if (bg.scrx<320) bg.scrx++;

 if (sbstat==2)
 {
 if (uu==32)  PlayMixedSoundEffect(&vocname[0],8);
 if (uu==120) PlayMixedSoundEffect(&versus,8);
 if (uu==190) PlayMixedSoundEffect(&vocname[1],8);

 }

 if (uu>300 && uu<=332) fadelevel=332-uu;
 }


void DrawBioImage2(char *src,char *dest,int y,int o)
{
int x;
int xlen,ylen,xst,yst,clip;

clip=0; //clip flag
xst=yst=0;
xlen=(int) (unsigned char)src[1];
ylen=(int) (unsigned char)src[2];

y-=ylen;

if (!o)
 {
  x=70-xlen/2;
  if (x+xlen>=150) {xlen=150-x; clip=1; }
  if (x<0)  { xst=-x; clip=1;  x=0; xlen-=xst;}
 }
  else
 {
  x=160+90-xlen/2;
  if (x+xlen>=320) {xlen=320-x; clip=1; }
  if (x<170)  { xst=170-x; clip=1; x=170; xlen-=xst;}
 }

if (clip==0) PutImageFlip256(src,dest+(320*y)+x,320,o<<1);
 else  PutImageFlipClip256(src,dest+(320*y)+x,320,xst,yst,xlen,ylen,o<<1);
}





void VersusScreen(int p1,int c1,int p2,int c2)
{
int h;
unsigned int bytes;


int i,j;

char *bio[2][2];

/*
static SOUND bgtest={0,0};

if (!bgtest.soundsize)
{
bgtest.soundptr=(signed char *)ReadFile("test.rvc");
bgtest.soundsize=lastsize;
} */

//PlayMixedSoundEffect(&bgtest,8);

//start_sound(&bgtest, 3, 8, 255, 0);

Solid(background,64000,0);
MemoryCopy((char *)0xA0000,background,64000);

//return;
//if (volumize) return;

//for (i=14; i<189; i++)
// for (j=0; j<320; j++)
//      background[i*320+j]=(i-14)*16/175+9*16;

bg.ReadBackground("versus.vol",0);

for (i=0; i<2; i++)
 {
  char s[20];
  if (!i) strcpy(s,cnlist[p1]);
         else strcpy(s,cnlist[p2]);
  strcat(s,".vol");

  if (!_dos_open(s,O_RDONLY | O_BINARY,&h))
         {
          unsigned size;
          lseek(h,12,SEEK_CUR);
          ReadVolSound(h,&vocname[i]);

                for (j=0; j<4; j++)
                 {
                        _dos_read(h,&size,4,&bytes);
                        lseek(h,size,SEEK_CUR);
                 }
          for (j=0; j<2; j++)
                bio[i][j]=ReadVolFile(h);

          if ((!i && c1) || (i && c2))
          {
                for (j=2; j<6; j++)
                {
                 unsigned  size;
                 _dos_read(h,&size,4,&bytes);
                 lseek(h,size,SEEK_CUR);
                }
                lseek(h,15,SEEK_CUR);
                char *map=(char *)malloc(256);
                _dos_read(h,map,256,&bytes);
                ColorMapImage(bio[i][0],(unsigned char *)map);
                ColorMapImage(bio[i][1],(unsigned char *)map);
                free(map);
          }
          _dos_close(h);
        }
 }

// unsigned  size;
if (_dos_open("select.vol",O_BINARY | O_RDONLY,&h)) Nofile("select.vol");
ReadVolSound(h,&versus);
_dos_close(h);

//MemoryCopy(pal1,bg.pal,256*3);

//lseek(h,64000,SEEK_CUR);
//_dos_read(h,pal1,256*3,&bytes);

uu=0;
bg.scrx=0; bg.scry=10;

namey[0]=290; namey[1]=290;

bgframe *f;
//find loc for vs
svs=&bg.bgs[0];
f=(bgframe *)svs->frameptr;
svsdisp.x=-f->x+160-imageX(bg.bgimagelist[f->index])/2;
svsdisp.y=-f->y+110-imageY(bg.bgimagelist[f->index])/2;

sp[0]=sp[1]=0;
//find loc for p1
if (p1<14)
{
sp[0]=&bg.bgs[1+p1];
f=(bgframe *)sp[0]->frameptr;
spdisp[0].x=-f->x+ 70-imageX(bg.bgimagelist[f->index])/2;
spdisp[0].y=-180   ; //-imageY(bg.bgimagelist[f->index])/2;
}

if (p2<14)
{
//find loc for p2
sp[1]=&bg.bgs[1+p2];
f=(bgframe *)sp[1]->frameptr;
spdisp[1].x=-f->x+250-imageX(bg.bgimagelist[f->index])/2;
spdisp[1].y=-180   ; //-imageY(bg.bgimagelist[f->index])/2;
}


oldfadelevel=-1;fadelevel=0;
UpdatePalette2();


kbscan=0;
  p[0].in.Reset(); p[1].in.Reset();

//Set up our timer
//      contse=currse=0;
timerhandler=VersusTick;


do
{
//MemoryCopy(screen,background,64000);
bg.DrawBackground(screen);

i=(uu&32)/32;
 DrawBioImage2(bio[0][i],screen,135,0);
 DrawBioImage2(bio[1][i^1],screen,135,1);

// DrawImage(name[0],screen,namex[0],namey[0],0);//namex[0],namey[0],0);
// DrawImage(name[1],screen,namex[1],namey[1],0);//namex[1],namey[1],0);
if (sp[0])
{
 f=(bgframe *)sp[0]->frameptr;
 DrawImage(bg.bgimagelist[f->index],screen,f->x+spdisp[0].x,f->y+spdisp[0].y+namey[0],0);
}
if (sp[1])
{ 
 f=(bgframe *)sp[1]->frameptr;
 DrawImage(bg.bgimagelist[f->index],screen,f->x+spdisp[1].x,f->y+spdisp[1].y+namey[1],0);
}
 int vsx, vsy;
 f=(bgframe *)svs->frameptr;
 vsx=f->x+svsdisp.x;
 vsy=f->y+svsdisp.y;

 if (uu>120)
  {
        if (uu>140) DrawImage(bg.bgimagelist[f->index],screen,vsx,vsy,0);
         else
          PutImageD2(bg.bgimagelist[f->index],screen+vsx+vsy*320,320,(141-uu)*256);
  }





MemoryCopy((char *)(0xA0000+14*320),screen+10*320,320*175);
UpdatePalette2();

RefreshInput();
if ((p[0].in.ReadButt(0)|p[1].in.ReadButt(1))&(~0xF))  kbscan=1;
if (uu<301 && kbscan) uu=301;

} while (uu<332);

fadelevel=0;
UpdatePalette2();

timerhandler=0;

if (sbstat==2) StopVoice();
//farfree(bgtest.soundptr);

bg.KillBackground();
free(bio[0][0]); free(bio[0][1]);
free(bio[1][0]); free(bio[1][1]);
free(vocname[0].soundptr);
free(vocname[1].soundptr);
free(versus.soundptr);
}



char *insult[][4]=
{
 { //Mojumbo 0
  "I am sorry me hitting you/so hard make you die.",
  "Yuk, you taste bad./Skin stick in Mojumbo teeth. ",
  "Mmmmm, you look even /tastier when you dead.",
  "Good thing Mojumbo stomach/full or you be Mojumbo lunch.",
  },
 {//Jinsoku 1
  "I may be blind.../but who is still standing?",
  "You may find that the/blades on my forearms hurt/when they sever your limbs.",
  "Now you too know darkness.",
  "Where did you go?!/Surely you didn't die yet.",
 },
 {//Savage 2
  "I've had harder times taking/a shit than I did killing you.",
  "Get back up, you worthless FUCK!/Get back up!",
  "What's all that brown/stuff on the floor? Oh yeah!/That's all the shit I beat outta ya.",
  "Roses are red. Violets are blue./Now that you're dead,/I'm gonna piss on you.",
 },
 {//Pierre 3
  "The crimson blood matches the/purple hues of your face so perfectly.",
  "I had to kill you!/You were much too ugly to paint.",
  "A good painting takes me/longer than you did.",
  "Good, you're bleeding/ I was running low on red paint.",
 },
 {//Spice 4
  "That's typical.../They always go unconscious on me.",
  "My pimp hits harder than you!!",
  "Damn! Broke a nail.",
  "Too bad I didn't lose.../I like lying flat on my back.",
 },
 {//Vlad 5
  "Look at all the blood on the/ground. What a waste!",
  "Carpathian sheep are harder/prey than you.",
  "I get hungrier and hungrier/the more I see your blood fly.",
  "Those who know not the taste of blood/know not the taste of victory.",
 },
 {//Asylum 6
  "That was fun./ I have to go potty now.",
  "That was too quick.../I have a harder time fighting myself.",
  "We thought you put up/a good fight. Didn't we?/Yes, we did!",
  "Look mommy!/No arms!",
 },
 {//Chi 7
  "Wa ta ta ta ta ta...wa ta.",
  "Oooh hoo wa ta.",
  "Killing you brings me great honor/in my Chinese clan, and now I/can call myself a true warrior.",
  "Oooh ta ta ta woo ta wa ta."
 },
 {//Staine 8
  "Another dies in the/name of Ratchkar!",
  "Time has been cleansed/by your death.",
  "You never had a chance/against a member of the Takar!",
  "The structure has been preserved.",
 },
 {//Portal 9
  "Your death would please me/if I were not devoid of/human emotion.",
  "Another portal set aflame.../another life extinguished.",
  "Ahhh....amputation.../labotomy...  vivisection.../yet, the bloodlust continues.",
  "The pages of time are written/in your blood. Now the book is closed.",
 },

 {//LAZ 10
  "I read the message in your bottle,/and it said you was gonna die!",
  "Your skull opens easier than/a coconut.",
  "Good. Now I can use your/body as a raft.",
  "Good. Now I can use your/body as an S.O.S. sign.",
  },

 {//Ug 11
  "Ug maybe not so smart/but Ug kill you anyway.",
  "What wrong? No want to/play with Ug no more?",
  "After fight Ug need take bath.../hygiene very important to Ug.",
  "Ug pound you into ground so/hard you be fossil.",
 },

{//buddy 12
  "We can't understand what the fuck/this tard is saying.",
  "You know what, I squeezed the life/outta you like you were/a choking victim.",
  "My..my, my..my my mom could/ kick your ass.",
  "I was talking to my mom and she said,/hey buddy, why you out there/fighting again?! You should be on/the tardwagon.",
 },

{//ravage 13
  "There are many forms of death/and I have shown you but one.",
  "Roses are red. Violets are blue./Now that I'm a Takar,/I'm gonna piss on you.",  
  "I used to think I was strong./Now that the surgeon rebuilt me,/I am immortal.",
  "You look like someone who wants/some all-you-can-eat-chili./You're a growing chili boy.",
 },

{//guillotine 14
  "There are many forms of death/and I have shown you but one.",
  "Roses are red. Violets are blue./Now that I'm a Takar,/I'm gonna piss on you.",  
  "I used to think I was strong./Now that the surgeon rebuilt me,/I am immortal.",
  "You look like someone who wants/some all-you-can-eat-chili./You're a growing chili boy.",
 },

{//reaper 15
  "There are many forms of death/and I have shown you but one.",
  "Roses are red. Violets are blue./Now that I'm a Takar,/I'm gonna piss on you.",  
  "I used to think I was strong./Now that the surgeon rebuilt me,/I am immortal.",
  "You look like someone who wants/some all-you-can-eat-chili./You're a growing chili boy.",
 },
 
{//ed 16
  "There are many forms of death/and I have shown you but one.",
  "Roses are red. Violets are blue./Now that I'm a Takar,/I'm gonna piss on you.",  
  "I used to think I was strong./Now that the surgeon rebuilt me,/I am immortal.",
  "You look like someone who wants/some all-you-can-eat-chili./You're a growing chili boy.",
 }
 
 

};


int docontscr,contsec,contsecdur,continueresult;

void InsultTick()
{
 uu++;

 if (uu<32) fadelevel=uu;
  if (fadeout) if  (fadelevel) fadelevel--; else done=1;

if (!(uu&1))
{
bg.scrx+=1;
if (bg.scrx>320) bg.scrx=0;
}

/*
gigerlevel=uu&63;
if (uu&64) {gigerlevel^=63; }
gigerlevel>>=1;
*/


 if (docontscr)
  {
        if ((uu&1) && fadelevel>20) fadelevel--;

   if (p[0].in.stat&0xF) contsecdur--;
        if ( (--contsecdur)<=0)
         if (contsec) {contsec--; contsecdur=192;}
        else {done=1; contsecdur=400; continueresult=0;}
  }
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

extern char *eimagelist[256];
extern move elist[];
extern unsigned char *eframe;


void InsultScreen(int winner,  int p1,int c1,int p2,int c2)
{

SOUND insultvoc;

int h;
unsigned int bytes;

int i;

char *bio[2][2];



Solid(background,64000,0);
MemoryCopy((char *)0xA0000,background,64000);


bg.ReadBackground("selectbg.vol",0);
for (i=1; i<100; i++) bg.ylist1a[i]=bg.ylist1a[0];

unsigned size;
_dos_open("select.vol",O_BINARY | O_RDONLY,&h);
_dos_read(h,&size,4,&bytes); //Skip versus
lseek(h,size,SEEK_CUR);
_dos_read(h,pal1,256*3,&bytes);
_dos_close(h);

uu=0;
bg.scrx=0; bg.scry=10; 

oldfadelevel=-1;fadelevel=0; gigerlevel=0; oldgiger=-1; dgiger=1;
UpdatePalette2();
fadeout=0;



//return;
int inum; //find insult to say
do
{
inum=random(4);
} while (!*insult[options->pnum[winner]][inum]);

//read char bios
for (i=0; i<2; i++)
 {
  char s[20];
  if (!i) strcpy(s,cnlist[p1]);
      else strcpy(s,cnlist[p2]);
  strcat(s,".vol");

  if (!_dos_open(s,O_RDONLY | O_BINARY,&h))
         {
          unsigned int size;
          lseek(h,12,SEEK_CUR);

          for (int j=0; j<5; j++)
          {
                if (winner==i && j==inum+1)
                        ReadVolSound(h,&insultvoc);
                 else
                 { //Skip this voc
                        _dos_read(h,&size,4,&bytes);
                        lseek(h,size,SEEK_CUR);
                 }
          }


          if (winner==i)
          {
          bio[i][0]=ReadVolFile(h); //get both frames
          bio[i][1]=ReadVolFile(h);

          if ((!i && c1) || (i && c2))
          {
                for (j=2; j<6; j++)
                {
                 unsigned  size;
                 _dos_read(h,&size,4,&bytes);
                 lseek(h,size,SEEK_CUR);
                }
                lseek(h,15,SEEK_CUR);
                char *map=(char *)malloc(256);
                _dos_read(h,map,256,&bytes);
                ColorMapImage((bio[i][0]),(unsigned char *)map);
                ColorMapImage((bio[i][1]),(unsigned char *)map);
                free(map);
           }

          }
                else
          {
                for (j=0; j<4; j++)
                {
                 _dos_read(h,&size,4,&bytes);
                 lseek(h,size,SEEK_CUR);
                }
                bio[i][0]=ReadVolFile(h); //get both frames
                if ((!i && c1) || (i && c2)) //see if we need to color map
                 {
                        for (j=5; j<6; j++)
                        {
                         unsigned  size;
                         _dos_read(h,&size,4,&bytes);
                         lseek(h,size,SEEK_CUR);
                        }
                  lseek(h,15,SEEK_CUR);
                  char *map=(char *)malloc(256);
                  _dos_read(h,map,256,&bytes);
                  ColorMapImage(bio[i][0],(unsigned char *)map);
                  free(map);
                }
          }

          _dos_close(h);
         }  else Nofile(s);
 }



done=0;
timerhandler=InsultTick;

PlayModalSoundEffect(&insultvoc);
quit=0;

continueresult=1;
docontscr=0;

kbscan=0;

int leaver=0;

p[0].in.stat=0;

do
{
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, 0, gigerlevel/4  ,  bg.scrx,  0,  0,   0+2, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, 0, 100+gigerlevel/4, bg.scrx, 100,  0,  100+2, bg.ylist1a, bg.ylist2);

if (winner==0)
 {
  if (uu&32) DrawBioImage2(bio[0][0],screen,135,0);
    else     DrawBioImage2(bio[0][1],screen,135,0);
  DrawBioImage2(bio[1][0],screen,135,1);
 }   else
 {
  if (uu&32) DrawBioImage2(bio[1][0],screen,135,1);
    else     DrawBioImage2(bio[1][1],screen,135,1);
  DrawBioImage2(bio[0][0],screen,135,0);
 } 


 //Draw words
char *s=insult[!winner ? p1 : p2][inum];
char line[40];
unsigned dest=137*320;
do
{
 int i=0;
 while (*s!='/' && *s) {line[i++]=toupper(*s); s++;}
 line[i]=0; if (*s) s++;

 DrawStringSP(font5,screen+dest+160-GetStringWidthSP(font5,(unsigned char *)line)/2,(unsigned char *)line,320);
 DrawStringSP(font5,background+dest+160-GetStringWidthSP(font5,(unsigned char *)line)/2,(unsigned char *)line,320);
 dest+=13*320;
} while(*s);


 RefreshInput();
 p[0].in.ReadButt(0);

  //If still playing insult
 if (!docontscr)
 {
  if (p[0].in.stat&(16+32)) kbscan=2;
  if (kbscan) {uu=551; if (sbstat==2) StopVoice(); }

  if ( (sbstat!=2 && uu>550) || (sbstat==2  && !sound_playing(2)) || kbscan)
        if (game!=2) {fadeout=1;quit=0; } //If no need cont, bye
         else {docontscr=1; contsec=9; contsecdur=192; quit=kbscan=0; uu=300;}
 }

 if (docontscr)
  {
        char *b="CONTINUE? -";
        b[10]=contsec+'0';
        if (uu&32)
         DrawString(font2,screen+70+100*320,(unsigned char *)b,320);


        if (!quit)
         { if (kbscan || (p[0].in.stat&(16+32))  ) {fadeout=1; docontscr=0; continueresult=1;} }
        else
         { continueresult=0; fadeout=1; docontscr=0; }

  }


 UpdatePalette();

  MemoryCopy((char *)(0xA0000+14*320),screen+10*320,320*175);


} while (!done);

timerhandler=0;
StopVoice();

if (!continueresult)
{
// #ifndef DEMO
 cstat=0;
// #else
// cstat=0xFF-(1<<3)-(1<<6);
// #endif
 fstate=0; quit=3;}

free(bio[0][0]);
free(bio[1][0]);
if (!winner) free(bio[0][1]);
 else free(bio[1][1]);

free(insultvoc.soundptr);
bg.KillBackground();
//rbaseptr=orb;
}







int curl; //current letter that is dropping
int lx[13]={1,25,41,72,101,125,145,169,192,220,247,273,294};
int ly[13];
int tshake;


int py;
int pn;       //0-none 1, 2

int drawpermback;

int sky;
effect skull;

extern bground bg;

int tstage;
//0 idle at beginning
//1 start/options
//2 going down
//3 1p/2p
//4 done



void TitleTick()
{
 uu++;

 if (uu<32) fadelevel=uu;
  if (fadeout)
         if  (fadelevel) { if  (!(uu&15) || quit || tstage==5) fadelevel--; }
                 else done=1;

 if (curl<=12)
  {
        ly[curl]+=3;
        if (ly[curl] >=12)
         {
          curl++; ly[curl]=-80; tshake=10;
         /* if (random(2))
                PlayModalSoundEffect(&voc1); else
                PlayModalSoundEffect(&voc2);*/
         }
  }

  series *bp;  //bg.nums
  int i;
  bp=&bg.bgs[0];   for (i=0; i<bg.nums; i++,bp++)  bp->Tick();

 b2x+=2; if (b2x>=320) b2x=0;
 b3x-=1; if (b3x<0) b3x=319;


switch (tstage)
{
 case 1: //start/options
         //make it rise
        if (py>152) { py-=2; if (py<=152) {
         PlayModalSoundEffect(&voc1); tshake=25;} }

        skull.Tick();
        if (pn==1 && sky>0)  {sky--; if (sky==0) {PlayModalSoundEffect(&voc1);  tshake=20;}}
        if (pn==2 && sky<22) {sky++; if (sky==22) {PlayModalSoundEffect(&voc1); tshake=20;}}
  break;

 case 2: //Move down
        if (py<240) py+=2;        else tstage=3;
  break;

 case 3: //1p/2p
        if (py>152) { py-=2; if (py<=152) {
         PlayModalSoundEffect(&voc1); tshake=25;} }

        skull.Tick();
        if (pn==1 && sky>0)  {sky--; if (sky==0) {PlayModalSoundEffect(&voc2); tshake=20;}}
        if (pn==2 && sky<22) {sky++; if (sky==22) {PlayModalSoundEffect(&voc2);tshake=20;}}
  break;

}



 if (tshake) tshake--;

// if (sbstat==2)
// if (uu==550)
//  PlayMixedSoundEffect(voc3,10);
}





int TitleScreen()
{
int i;
int h;


if (ipxinstalled) return(2);


char *cs="COPYRIGHT 1996 BLOODLUST SOFTWARE";


tshake=0;
Solid((char *)0xA0000,64000,0);

bg.ReadBackground("titlebg.vol",0);


//REVOLUMIZE TITLE
#ifdef REVOL
 if (_dos_open("title.vol",O_BINARY | O_RDONLY,&h))
        {
         int sh;
         _dos_creat("title.vol",0,&sh);

         for (i=1; i<=13; i++)
         {        //Timeslaughter
              unsigned bytes;
                char * m;
                char *s="\\ss\\l??.raw"; s[5]='0'+i/10; s[6]='0'+i%10;
                _dos_open(s,O_RDONLY | O_BINARY,&h);
                int size=ReadRawFile(&m,h); _dos_close(h);
          _dos_write(sh,&size,4,&bytes);
          _dos_write(sh,m,size,&bytes);
         }

         char *vocs[3]={"\\ss\\boom1.rvc","\\ss\\boom2.rvc","\\ss\\laugh.rvc"};
         for (i=0; i<3; i++)
          {
              unsigned bytes;
                voc1.soundptr=(signed char *)ReadFile(vocs[i]);
//              _dos_write(sh,&lastsize,4,&bytes);
                _dos_write(sh,voc1.soundptr,lastsize,&bytes);
                free((char *)voc1.soundptr);
          }

         _dos_close(sh);
        } else _dos_close(h);
#endif


//ModeText();
//printf("opening titlevol...\n");

//Read........
if (_dos_open("title.vol",O_BINARY | O_RDONLY,&h))  Nofile("title.vol");

char *letters[13];
for (i=0; i<13; i++) letters[i]=ReadVolFile(h);
ReadVolSound(h,&voc1);
ReadVolSound(h,&voc2);
ReadVolSound(h,&voc3);
_dos_close(h);

//kbscan=0; while (!kbscan);
//Mode256();




//char *skull[5];
//for (i=0; i<5; i++)  skull[i]=ReadVolFile(h);


skull.e=(unsigned char *)(eframe+elist[27].firstframe);
skull.currframe=0; skull.numframes=elist[27].numframes;
skull.en=27; skull.d=0;
skull.dur=((frametype *)skull.e)->dur;
skull.tcx=skull.tcy=0;
skull.maxy=999;




uu=0;  done=0;

oldfadelevel=-1;fadelevel=0;
UpdatePalette();
//LoadPalette(pal1,0,256);

//Set up our timer
fadeout=0; done=0;
kbscan=0; quit=0;




pn=0;
curl=0;

tstage=0;

for (i=0; i<=12; i++) ly[i]=-200;



char *w[4];;
CreateFontImage256(font2,&w[0],(unsigned char *)"SLAUGHTER");
CreateFontImage256(font2,&w[1],(unsigned char *)"OPTIONS");
CreateFontImage256(font2,&w[2],(unsigned char *)"1 PLAYER");
CreateFontImage256(font2,&w[3],(unsigned char *)"2 PLAYERS");

//drawpermback=-1;

kbscan=quit=0;
p[0].in.Reset(); p[1].in.Reset();


bg.scrx=0; bg.scry=0;
b2x=0; b3x=0;

int result=0;
timerhandler=TitleTick;

//if (musicstat)

//bg.bgs[13].currframe=1;
//bg.bgs[13].frameptr=(unsigned char *)(bg.bgframes+bg.bgs[13].firstframe)+sizeof(bgframe);

if (cdplay) PlayCDTrack(7);
 else
if (musicstat) start_music(bg.music,1);

do
{
// bg.DrawBackground(screen);

  char *d=screen;
  if (!tshake || uu&4) d+=2*320;

  PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,d, 0, 0, b2x,10/2, b3x,10/4,        0,0, bg.ylist1a, bg.ylist2);
  PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,d, 0, 0, b2x,10/2+90, b3x,10/4+90,  0, 100, bg.ylist1b, bg.ylist2);
  PutBackground(bg.bg1b,d,0,bg.bg2y-100,0,bg.bg2y,bg.ylist1b);

  bg.DrawBGImages(d);


 for (i=0; i<=12; i++)
  DrawImage(letters[i],screen,lx[i],ly[i],0);
/*

if (kbscan)
 {
   bg.bgs[13].currframe++;
         if (bg.bgs[13].currframe>=bg.bgs[13].numframes)
          {bg.bgs[13].currframe=0; bg.bgs[13].frameptr=(unsigned char *)(bg.bgframes+bg.bgs[13].firstframe);    
              } else bg.bgs[13].frameptr+=sizeof(bgframe);
  
  kbscan=0;
 }
*/
 
RefreshInput();
 p[0].in.ReadButt(0);
 p[0].in.ReadPosition(0);


int trigger=(kbscan==57 || kbscan==28 || (p[0].in.stat&(16+32))) && !quit;
if (!fadeout)
{
 switch(tstage)
 {
  case 0:       if (trigger) {tstage=1; py=240; pn=1;} break;
  case 1:
  case 2:
                DrawImage(w[0],screen,180,py,0);
                DrawImage(w[1],screen,180,py+22,0);
                skull.y=py+sky+11; skull.x=165;
                skull.Draw();

         if (tstage==1)
         {
                if (p[0].in.stat&4 && pn>1) pn--;
                if (p[0].in.stat&8 && pn<2) pn++;

                if (trigger)
                  {
                        PlayModalSoundEffect(&voc1);
                        tshake=205;
                        if (pn==1) tstage=2;
                          else
                                {
                                 tstage=5;
                                 fadeout=1;
                                 result=3; //options
                                }
                  }
          }
                break;

  case 3:
                DrawImage(w[2],screen,185,py,0);
                DrawImage(w[3],screen,185,py+22,0);
                 skull.y=py+sky+11; skull.x=171;
                skull.Draw();

                if (p[0].in.stat&4 && pn>1) pn--;
                if (p[0].in.stat&8 && pn<2) pn++;

                if (trigger)
                  {
                        PlayModalSoundEffect(&voc1);
                        PlayModalSoundEffect(&voc3);
                        tshake=205;
                        tstage=4;
                        fadeout=1;
                        result=pn;
                        if (cdplay) StopCD();
                         else
                        if (musicstat) stop_music();

                  }
                break;
  }

}



 kbscan=0; p[0].in.stat=0;



//char v[20];
//if ( (((bgframe *)bg.bgs[13].frameptr)->index)!=0xFF)
//{
//sprintf(v,"%d %d",(int)imageX(bg.bgimagelist[((bgframe *)bg.bgs[13].frameptr)->index]),(int)imageY(bg.bgimagelist[((bgframe *)bg.bgs[13].frameptr)->index])    );
//DrawString(font2,screen+90*320,(unsigned char *)v,320);
//}
 MemoryCopy((char *)(0xA0000+5*320),screen+5*320,190*320);
UpdatePalette();

 if (quit) 
  if (tstage!=3) fadeout=1;
                 else {tstage=1; quit=0; kbscan=0; pn=1;}

 if (uu>1350 && !tstage) fadeout=1;

} while (done==0);

if (cdplay) StopCD();
 else
if (musicstat) stop_music();


kbscan=0;


if (sbstat==2)
 {StopVoice();}


 timerhandler=0;

//if (musicstat) stop_music();
//baseptr=oldptr;

free(voc1.soundptr);
free(voc2.soundptr);
free(voc3.soundptr);
bg.KillBackground();
for (i=0; i<13; i++) free(letters[i]);
free(w[0]);
free(w[1]);
free(w[2]);
free(w[3]);



if (!tstage) result=-2;  
if (quit)    result=-1; //leave game

return(result); //1,2 mean 1player game,2player game, 3 is options 

}



void FadeTick()
{
 uu++;

 if (fadein)
  if (fadelevel<32) fadelevel++;
    else {done=1; fadein=0;}
 
  if (fadeout)
     if  (fadelevel) fadelevel--; 
     else {done=1; fadeout=0;}
}


int direction;
int pillary;

void BioTick()
{
 uu++;

// if (uu<32) fadelevel=uu;
  if (fadeout) if  (fadelevel) fadelevel--; else done=1;

bg.scrx+=direction;
//if (bg.scrx>320) bg.scrx=0;

sp[0]->Tick();
if (uu>100 && namey[0]<50) namey[0]+=1;
if (uu==140)  PlayMixedSoundEffect(&vocname[0],8);

if (pillary>0) pillary-=4;

}


void DrawBigWord(char *s,int x, int y)
{
DrawString(font2,screen+y*320+x-GetStringWidth(font2,(unsigned char *)s),
    (unsigned char *)s,320);
DrawString(font2,screen+(y-12)*320+x+1,
    (unsigned char *)".",320);
DrawString(font2,screen+(y-5)*320+x+1,
    (unsigned char *)".",320);


}
    

char *ages[]=
{"33","92","??","26", //0-1-2-3
 "16","592","29","45", //4-5-6-7
 "FOREVER","36",  "68","25"}; //8-9-10-11
char *origins[]=
{
 "Bahon, Haiti",
 "Hokkaido, Japan",
 "DeathValley",
 "Paris, France",
 "NY, NY",
 "Walachia, Romania",
 "Sydney, Australia",
 "Cheng, China",
 "Khadshar",
 "Liverpool, England",
 "Carribean",
 "Neander Valley",
};

char *ctimes[]=
{
 "1752 AD",
 "1327 AD",
 "2189 AD",
 "1528 AD",
 "1997 AD",
 "1450 AD",
 "2043 AD",
 "500 BC",
 "ALL",
 "2650 AD",
 "1956 AD",
 "50002 BC",
};


char *cstyles[]=
{
 "Voodoo",
 "Ainu-Kai",
 "Cage Fighting",
 "Art Of Death",
 "Bitch",
 "Impalement",
 "Insanity",
 "Braided Blade",
 "Ratchkar",
 "Pissed Off",
 "Stranded Crab",
 "Clubba Clubba"
};


char *cdesc[]=
{
"Pass the salt.",
"Too bad he's blind...",
"Mr. Potty Mouth.",
"So much paint...so little time!",
"Blowing the competition away.",
"You would look delightful on a stick.",
"They are coming to take me away... haha!",
"Seen any flying monkeys?",
"So the little mortal wants to play our game!",
"You bastards! My eye!",
"I'd consider having that amputated...",
"Ug not stupid, Ug just not very stupid.",
};

#define WX 205


int GetLetterWidth(char *font,unsigned char s);

int WordLength(char *font,char *s)
{
int x=0;
while (*s!=' ' && *s)
 {x+=GetLetterWidth(font,*s)-2; s++; }
return(x); 
  
}    


void BioScreen(int c)
{
int i,h;
unsigned bytes;
if (!bioexist[c] || !((1<<c)&BMAC)) return;

//Solid((char *)0xa0000,64000,0);

char s[20];    //get name of character vol
strcpy(s,cnlist[c]);  
strcat(s,".vol");


if (!_dos_open(s,O_RDONLY | O_BINARY,&h))
 {
   unsigned int size;
   lseek(h,12,SEEK_CUR);
   ReadVolSound(h,&vocname[0]);
  
   for (int j=1; j<5; j++)
     {
      _dos_read(h,&size,4,&bytes);
      lseek(h,size,SEEK_CUR);
     }
   for (j=0; j<2; j++)  bio[0][j]=ReadVolFile(h);
   _dos_close(h); 
 } else return;


bg.ReadBackground("versus.vol",0);


uu=0;
bg.scrx=0; bg.scry=10; direction=1;

oldfadelevel=-1;fadelevel=32;
UpdatePalette2();
fadeout=0;

kbscan=0; quit=0;
p[0].in.Reset(); p[1].in.Reset();
done=0;

sp[0]=&bg.bgs[1+c];
bgframe *f=(bgframe *)sp[0]->frameptr;
spdisp[0].x=-f->x+ 70-imageX(bg.bgimagelist[f->index])/2;
spdisp[0].y=-180   ; //-imageY(bg.bgimagelist[f->index])/2;

namey[0]=-90; pillary=174;


timerhandler=BioTick;

do
{

//bg.DrawBackground(screen);

PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 95, 0, bg.scrx%320,   0,  (bg.scrx%640)/2,  0,  0,   0, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,screen, 95, 0, bg.scrx%320,  100, (bg.scrx%640)/2, 100,  0,  100, bg.ylist1b, bg.ylist2);

PutBackground(bg.bg1a,screen,320-95,0,0,0,bg.ylist1a);
PutBackground(bg.bg1b,screen,320-95,0,0,100,bg.ylist1b);

i=(uu&32)/32;
//DrawBioImage2(bio[0][i],screen,155,0);
DrawImage(bio[0][i],screen,60-imageX(bio[0][i])/2,165-imageY(bio[0][i]),0);

DrawBigWord("AGE",   WX,35);
DrawStringSP(font5,screen+39*320+WX+10,(unsigned char *)ages[c],320);

DrawBigWord("ORIGIN",WX,55);
DrawStringSP(font5,screen+59*320+WX+10,(unsigned char *)origins[c],320);

DrawBigWord("TIME",  WX,75);
DrawStringSP(font5,screen+79*320+WX+10,(unsigned char *)ctimes[c],320);

DrawBigWord("STYLE", WX, 95);
DrawStringSP(font5,screen+ 99*320+WX+10,(unsigned char *)cstyles[c],320);


f=(bgframe *)sp[0]->frameptr;
DrawImage(bg.bgimagelist[f->index],screen,f->x+spdisp[0].x,f->y+spdisp[0].y+namey[0],0);


char *s=cdesc[c]; //get string
int x=160-(GetStringWidthSP(font4,(unsigned char *)s))/2;
int y=167;
if (x<0) x=0;
DrawStringSP(font5,screen+ y*320+x,(unsigned char *)s,320);




MemoryCopy((char *)0xA0000+14*320+320*pillary,screen+10*320,320*(175-pillary));
UpdatePalette2();

RefreshInput();
if ((p[0].in.ReadButt(0)|p[1].in.ReadButt(1))&(~0xF))  fadeout=1;
if (uu>700) fadeout=1;
if (quit) fadeout=1;

} while (!done);

timerhandler=0;
if (sbstat==2) StopVoice();

free(vocname[0].soundptr);
free(bio[0][0]);
free(bio[0][1]);

bg.KillBackground();


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


extern int bloodlevel;



void OptionsTick()
{
 uu++;

// if (uu<32) fadelevel=uu;
if (fadeout)  if  (fadelevel) fadelevel--; else done=1;

bg.scrx+=direction;
//if (bg.scrx>320) bg.scrx=0;

if (pillary>0) pillary-=4;

skull.Tick();
}

void DrawLevel(int x, int y, int l)
{
skull.y=y;
for (; l>0; l--,x+=14)
 {
  skull.x=x;
  skull.Draw();
 } 

}    

void OptionsScreen(int overlay)
{
int i;
int h;
unsigned bytes;

if (!overlay)
{
 Solid((char *)0xa0000,64000,0);
 bg.ReadBackground("versus.vol",0);
} else
{
 MemoryCopy(background,(char *)0xa0000,64000);
}    

if (!overlay)
{
 uu=0;
bg.scrx=0; bg.scry=10; direction=1;

oldfadelevel=-1;
UpdatePalette2();
}

fadeout=0;fadelevel=32;
kbscan=0; quit=0;
p[0].d=0; p[1].d=0;
p[0].in.Reset(); p[1].in.Reset();
done=0;


pillary=174;

skull.e=(unsigned char *)(eframe+elist[27].firstframe);
skull.currframe=0; skull.numframes=elist[27].numframes;
skull.en=27; skull.d=0;
skull.dur=((frametype *)skull.e)->dur;
skull.tcx=skull.tcy=0;
skull.maxy=999;


if (_dos_open("select.vol",O_BINARY | O_RDONLY,&h))  Nofile("select.vol");

int size;
  _dos_read(h,&size,4,&bytes); //Skip versus
  lseek(h,size,SEEK_CUR);

//  _dos_read(h,background,64000,&bytes);
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


timerhandler=OptionsTick;

int level[5];
level[0]=options->difficulty;
level[1]=options->soundvolume;
level[2]=options->musicvolume;
level[3]=options->bloodlevel;
level[4]=options->gamespeed;

int l=0;
char *lwords[]={"DIFFICULTY","SOUND VOL","MUSIC VOL","BLOODLEVEL","GAME SPEED"};
int lmax[5]={7,7,7,7,3};

int ostat=0;


if (!overlay)
 if (cdplay) PlayCDTrack(7);
  else
 if (musicstat) start_music(bg.music,1);

do
{

//bg.DrawBackground(screen);
if (!overlay)
{
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 95, 0, bg.scrx%320,   0,  (bg.scrx%640)/2,  0,  0,   0, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,screen, 95, 0, bg.scrx%320,  100, (bg.scrx%640)/2, 100,  0,  100, bg.ylist1b, bg.ylist2);

PutBackground(bg.bg1a,screen,320-95,0,0,0,bg.ylist1a);
PutBackground(bg.bg1b,screen,320-95,0,0,100,bg.ylist1b);
} else MemoryCopy(screen,background,64000);


for (i=0; i<5; i++)
 {
  int x=185,y=25+i*25-bg.scry;
     
  if (i!=l || uu&32)
  DrawBigWord(lwords[i],x-15, y);
  if (i!=4) DrawLevel(x,y,level[i]);
    else
     {
      char *s[]={"SLOW","NORMAL","FASTER","FASTEST"};
      DrawStringSP(font,screen+(y+3)*320+x,(unsigned char *)s[level[i]],320);
     }
 }
 

  

//DrawBigWord("EXIT", 200,155);
if (l!=5 || uu&32)
  DrawString(font2,screen+155*320+120,(unsigned char *)"EXIT",320);


if (!overlay)
{
  MemoryCopy((char *)0xA0000+14*320+320*pillary,screen+10*320,320*(175-pillary));
  UpdatePalette2();
}
 else
  MemoryCopy((char *)0xA0000,screen,64000); 
  


RefreshInput();
p[0].in.ReadButt(0);
p[0].in.ReadPosition(0);

if (ostat!=p[0].in.stat)
{
 if (p[0].in.stat&4 && l>0) {l--;PlayMixedSoundEffect(&voc2,8);} else
 if (p[0].in.stat&8 && l<5) {l++;PlayMixedSoundEffect(&voc2,8);} else
 {
   if (p[0].in.stat&1 && l<5 && level[l]>0)
    {level[l]--; PlayMixedSoundEffect(&voc1,8);} else
   if (p[0].in.stat&2 && l<5 && level[l]<lmax[l])
    {level[l]++; PlayMixedSoundEffect(&voc1,8);}
  switch (l)
   {
    case 1: //sound level
      if (sbstat) set_sound_volume(32*level[1]+31);
      break;
    case 2: //music level
      if (cdplay) SetCDVolume(32*level[2]+31);
       else
      if (musicstat)
       set_music_volume(32*level[2]+31);
      
      break;
    case 3:
      bloodlevel=options->bloodlevel;
     break;
   }
 }   

 if (l==5 && p[0].in.stat&(16+32)) quit=1;

 
 p[0].in.stat&=15;
 ostat=p[0].in.stat;


} 


if (quit) {if (!overlay) fadeout=1; else done=1;}

} while (!done);

timerhandler=0;

if (!overlay)
{
if (sbstat==2) StopVoice();
if (cdplay) StopCD();
 else
if (musicstat) stop_music();
}

if (!overlay)
 bg.KillBackground();



free(voc1.soundptr);
free(voc2.soundptr);

options->difficulty=level[0];
options->soundvolume=level[1];
options->musicvolume=level[2];
options->bloodlevel=level[3];
options->gamespeed=level[4];

 _dos_creat("time.cfg",0,&h);
 lseek(h,0,SEEK_SET);
 _dos_write(h,options,sizeof(cfg),&bytes);
 _dos_close(h);


}

int ReadRawFile(char **m,int h);

/*
void MakeFont()
{
find_t   f;
int done;
unsigned short int size;
char *t;
unsigned int bytes;


unsigned short int sizes[256];
char *imgs[256];

unsigned short int index[256];

unsigned short int ii=256*2;

for (int j=0; j<256; j++)
 {sizes[j]=0; index[j]=0;}

Mode256();
 int h,fh;

_dos_creat("f.vol",0,&fh);
_dos_write(fh,index,256*2,&bytes);


done=_dos_findfirst("*.raw",0,&f);
while (!done)
{ //Search list for file
 printf("%s\n",f.name);

 _dos_open(f.name,O_BINARY | O_RDONLY,&h);
 size=ReadRawFile(&t,h);
 _dos_close(h);

//draw it
 DrawImage(t,(char *)0xA0000,0,0,0);

 unsigned int key=getch();

 index[key]=ii; //store index
 ii+=size;
 sizes[key]=size;
 imgs[key]=t;    //store blah

 _dos_write(fh,t,size,&bytes);
 
 done=_dos_findnext(&f);
}

ModeText();
lseek(fh,0,SEEK_SET);
_dos_write(fh,index,256*2,&bytes);
_dos_close(fh);

}
*/


