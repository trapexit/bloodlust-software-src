#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
//#include <sbcvoice.h>
//#include <sbcmusic.h>

#include "graph.h"
#include "ems.h"


void Save();

typedef struct rect
{
int x1,y1,x2,y2;
};


typedef struct image
{
unsigned char index;    //Index into image pointer array
unsigned char orient;   //Orientation yx
char          dispx[2]; //Relative x location for image
char          dispy;    //Relative y location
};

typedef struct image2
{
unsigned char index;    //Index into image pointer array
unsigned char orient;   //Orientation yx
int           dispx[2]; //Relative x location for image
int           dispy;    //Relative y location
};

typedef struct def
{
 char se;
 int  parm;
 unsigned char energy;
};

typedef struct frametype   //Primary part of frame structure
{
int   size;	  //size of this frame
unsigned char shadow; //Index for shadow
unsigned char dur;    //duration in 1/100 of seconds
char se;              //Sound effect to play
int  parm;            //Parameters for current attack/vuln
unsigned char  energy;      //Energy taken off by attack
char dx,dy;       //Movement x,y
char tx,ty;       //Time per movement x,y
//     image img[];      //list of images in frame
//     rect  [];   vulnerable regions
//     rect  [];   attack regions

};

unsigned char frame[25000];       //Frame array
// Frames are stored in the following manner:
//     int dur;  duration in 1/100 of seconds
//     char attacktype;
//     int  energy;      Energy taken off by attack
//     int  dx,dy;       Movement x,y
//
//     image [];   list of images in frame
//     rect  [];   vulnerable regions
//     rect  [];   attack regions
int    framesize;


unsigned char tframe[10000];       //Frame array
int    tframesize;

typedef struct move
{
unsigned int   firstframe;       //Index into frame array to first frame
unsigned char  numframes;        //Number of frames in move
};

extern "C"
{
void putimageflip256();
void PutImageFlipClip256(char far *, char far *, int LSW,int XC,int YC,int XT, int YT,int o);

void InitFixedFont();
void DrawFixedChar(int c,char far *dest);
}

extern "C"
{
void InitTimer();
void TerminateTimer();
void StartTimer();
void StopTimer();
void SetTimerFunc(void (*func)());
void SetCountRate(unsigned);
void TimerSpeed(unsigned);
}
void TickUpdate();

//extern int baseptr;

void DrawBar(char far *b, char color, int x,int y,int xw,int yw);
void ClearScreen(char c);
void DrawFixedString(char *s,char far *dest);

void DrawShadow(char *f, int x, int y);
void DrawFrame(char *f, int x, int y, memptr *imagelist);
void DrawStatusBar();

void DrawImage2(char far *src,char far *dest,int x, int y,int o);
void PutImageFlip256(char far *src, char far *dest, int LSW,int O);
void ReadImageFiles();
void ReadImageFiles2(char *dir);


int ReadMouse(int *x, int *y);
int MouseBound(int x1,int x2,int y1,int y2);
image * ImageMouseBoundCheck();

int RequestMove();
void DrawOptions();

void AdjustMoves(char *p,int d,char *);
void CreateFrameIndex();

#define NM 130

char *frameindex[NM];

point framemove[NM];


move   moves[NM];

int currmove;  //Index in moves[] of current move in playback

unsigned char *frameptr; //Pointer to frame[] of current frame on screen

int currframe;   //Number of current frame on screen
int totalframes; //Number of frames in current move

int tcurrframe;

//THROW______________--******************************************
char *cdlist;
char *cnlist;
int cnum;

int ch;

int dothrow=0; //0-not 1-looking  2-loaded
int throwee;

unsigned char *tmp;

unsigned char *tframeptr; //Pointer to frame[] of current frame on screen
int throwptr[4][32];
char *tframeindex[100];
long oldems;



char *movenames[]= //Names of the moves that the character does
{
"Stance",                   //0
"Crouch",                   //1
"Stance->Crouch",           //2
"Crouch->Stance",           //3

"Walk Forward",             //4
"Walk Backward",            //5

"HighBlock",                //6
"Low Block",                //7

"Standing  Punch1",         //8
"Standing  Punch2",         //9
"Standing  Punch3",         //10
"Standing  Kick1",          //11
"Standing  Kick2",          //12
"Standing  Kick3",          //13

"Crouching Punch1",         //14
"Crouching Punch2",         //15
"Crouching Punch3",         //16
"Crouching Kick1",          //17
"Crouching Kick2",          //18
"Crouching Kick3",          //19

"Projectile     ",          //20
"Projectile Hit ",          //21
"Head Projectile",           //22
"Afterslam",           //23

"Jump Up",                  //24
"Jump Forward",             //25
"Jump Backward",            //26
"Aerial Punch1",            //27
"Aerial Punch2",            //28
"Aerial Punch3",            //29
"Aerial Kick1",             //30
"Aerial Kick2",             //31
"Aerial Kick3",             //32


"Stance->HighPain1",        //33
"Stance->HighPain2",        //34
"Stance->HighPain3",        //35
"Stance->Low Pain1",        //36
"Stance->Low Pain2",        //37
"Stance->Low Pain3",        //38
"Crouch->Pain1",            //39
"Crouch->Pain2",            //40
"Crouch->Pain3",            //41

"Ground->Dead",             //42
"Ground->Trip",             //43
"------------",             //44
"Aerial->Pain",             //45
"Dead  ->Stance",           //46

"Dizzyness",                //47

"Turnaround",               //48
"Crouch Turn",              //49

"Aerial Up Punch1",         //50
"Aerial Up Punch2",         //51
"Aerial Up Punch3",         //52
"Aerial Up Kick1",          //53
"Aerial Up Kick2",          //54
"Aerial Up Kick3",          //55

"Stance->HighBlock",        //56
"HighBlock->Stance",        //57
"Stance->LowBlock",         //58
"LowBlock->Stance ",        //59
"HighBlock Hit1",           //60
"HighBlock Hit2",           //61
"HighBlock Hit3",           //62
"LowBlock  Hit1",           //63
"LowBlock  Hit2",           //64
"LowBlock  Hit3",           //65

"Close S Punch1",         //66
"Close S Punch2",         //67
"Close S Punch3",         //68
"Close S Kick1",          //69
"Close S Kick2",          //70
"Close S Kick3",          //71
"Close C Punch1",         //72
"Close C Punch2",         //73
"Close C Punch3",         //74
"Close C Kick1",          //75
"Close C Kick2",          //76
"Close C Kick3",          //77


"Special Move #1",        //78
"Special Move #2",        //79
"Special Move #3",        //80
"Special Move #4",        //81
"Special Move #5",        //82
"Special Move #6",        //83
"Special Move #7",        //84
"Special Move #8",        //85
"Hundred Hands 1",        //86
"Hundred Hands 2",        //87

"Winning Pose #1",        //88
"Winning Pose #2",        //89
"Inactivity     ",        //90
"Final Falling  ",        //91

"After throw    ",        //92

"Throw1         ",        //93
"Throw2         ",        //94
"Aerial Throw   ",        //95
"Fatality       ",        //96

"Fall forward   ",        //97
"Fall w/o Head  ",        //98
"Fall w/Headfly ",        //99

"Bounce Off     ",        //100

"Freeze         ",        //101


0
};




def defs[]=
{
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},

 {0,0,0},
 {0,0,0},

 {0,0,0},
 {0,0,0},

 {0x11,128+64+32+ 16+8 +0+ 0+1, 5},  //SP1
 {0x22,128+64+32+ 16+8 +0+ 0+1,10},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x41,128+64+32+ 16+8 +0+ 0+1, 5},  //SK1
 {0x52,128+64+32+ 16+8 +0+ 0+1,10},
 {0x63,128+64+32+ 16+8 +0+ 0+1,15},

 {0x11,000+64+32+ 16+8 +0+ 0+0, 5},  //CP1
 {0x22,000+64+32+ 16+8 +0+ 0+0,10},
 {0x33,000+64+32+ 16+8 +0+ 0+0,15},
 {0x41,000+64+32+ 00+8 +0+ 0+0, 5},  //CK1
 {0x52,000+64+32+ 00+8 +0+ 0+0,10},
 {0x63,000+64+32+ 00+8 +0+ 2+0,15},

 {0x22,128+64+32+ 16+8 +0+ 0+1,10},  //Closep2
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x52,128+64+32+ 16+8 +0+ 0+1,10},  //Closek2
 {0x63,128+64+32+ 16+8 +0+ 0+1,15},

 {0,0,0},
 {0,0,0},
 {0,0,0},

 {0x11,128+64+32+ 16+0 +0+ 0+0, 5},  //AP1
 {0x22,128+64+32+ 16+0 +0+ 0+1,10},
 {0x33,128+64+32+ 16+0 +0+ 0+1,15},
 {0x41,128+64+32+ 16+0 +0+ 0+0, 5},  //AK1
 {0x52,128+64+32+ 16+0 +0+ 0+1,10},
 {0x63,128+64+32+ 16+0 +0+ 0+1,15},

 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},


 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},

 {0,0,0},

 {0,0,0},
 {0,0,0},

 {0x11,128+64+32+ 16+0 +0+ 0+0, 5},  //AUP1
 {0x22,128+64+32+ 16+0 +0+ 0+1,10},
 {0x33,128+64+32+ 16+0 +0+ 0+1,15},
 {0x41,128+64+32+ 16+0 +0+ 0+0, 5},  //AUK1
 {0x52,128+64+32+ 16+0 +0+ 0+1,10},
 {0x63,128+64+32+ 16+0 +0+ 0+1,15},

 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},

 {0x11,128+64+32+ 16+8 +0+ 0+1, 5},  //SP1
 {0x22,128+64+32+ 16+8 +0+ 0+1,10},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x41,128+64+32+ 16+8 +0+ 0+1, 5},  //SK1
 {0x52,128+64+32+ 16+8 +0+ 0+1,10},
 {0x63,128+64+32+ 16+8 +0+ 0+1,15},

 {0x11,000+64+32+ 16+8 +0+ 0+0, 5},  //CP1
 {0x22,000+64+32+ 16+8 +0+ 0+0,10},
 {0x33,000+64+32+ 16+8 +0+ 0+0,15},
 {0x41,000+64+32+ 00+8 +0+ 0+0, 5},  //CK1
 {0x52,000+64+32+ 00+8 +0+ 0+0,10},
 {0x63,000+64+32+ 00+8 +0+ 2+0,15},


 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},
 {0x33,128+64+32+ 16+8 +0+ 0+1,15},

 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},
 {0,0,0},


};


char far *ssound[2][7];
char far *hsound[2][7];

typedef struct imagefile {char filename[8];};
imagefile *filelist;
int   numfiles;
memptr imagelist[256];
memptr shadowlist[256];


//Throwstuff
int   tnumfiles;
imagefile *tfilelist;
memptr timagelist[256];


int mh,h;
unsigned size,bytes;


char path[40];


color pal[256];


char far *screen;

memptr background;

char far *t;
char far *cursor;

int cx,cy,b;
int mstat; //Current mouse stat 0-idle 1-dragging
image *dragimg; //Image being dragged
point dragrel;  //Relative mouse pos from ul of image

rect  rg;
rect *rgptr;

int i,j,k;
int o;


int updatestat;

int isattack;


point pos[256];


int editstat=0; //0-delay 1-movex 2-movey

int e=0; //0-graphic 1-vulnerable 2-attack 3-options

int pain=0;
int RequestImage(memptr *imagelist,imagefile *filelist,int numfiles);


char key;

unsigned char clipboard[1024];     //Place for copied '['
unsigned char *clipptr=0;            //Pointer to copied frame
int clipsize=0;


int basex,basey; //Base where character should start from
int x,y;         //Current location of character


int playloop=0;
int playback=0;
int dur=0;
int tcountx=0;
int tcounty=0;

int slowdown=0;
int slowflag=0;

long shadowmem;

void InsertBytes(char *p,int c,char *s)
    {
      if (s<tframe)   memmove(p+c,p,framesize-(p-frame));
       else           memmove(p+c,p,tframesize-(p-tframe));
      AdjustMoves(p,c,s);
    }
void DeleteBytes(char *p,int c,char *s)
    {
      if (s<tframe) memmove(p,p+c,framesize-(p-frame));
       else           memmove(p,p+c,tframesize-(p-tframe));
      AdjustMoves(p,-c,s);
    }


typedef struct joypos
{
 unsigned char pos;
 int min;
 int max;
};

joypos  jp[10][10];


joypos *ujp;

void DrawJoyMove();



void CopyDelay(int m1,int m2)
{
unsigned char *t1=frame+moves[m1].firstframe;
unsigned char *t2=frame+moves[m2].firstframe;

for (int i=0; i<moves[m1].numframes && i<moves[m2].numframes; i++)
 {
  ((frametype *)t2)->dur=((frametype *)t1)->dur;
  ((frametype *)t2)->tx=((frametype *)t1)->tx;
  ((frametype *)t2)->dx=((frametype *)t1)->dx;
  ((frametype *)t2)->ty=((frametype *)t1)->ty;
  ((frametype *)t2)->dy=((frametype *)t1)->dy;

  t1+=((frametype *)t1)->size;
  t2+=((frametype *)t2)->size;
 }

}



/*
void AdjustLinks(unsigned char *f,int a)
{

int i,j,k;

for (i=0; i<NM; i++)
 if (moves[i].numframes)
 {
  unsigned char *t=frame+moves[i].firstframe;
  for (j=0; j<moves[i].numframes; j++)
    {
     if ( ((frametype *)t)->parm&8192)
      {
       if ( *((int *)(t+sizeof(frametype))) > f )
	*((int *)(t+sizeof(frametype)))+=a;


      }



     t+=((frametype *)t)->framesize;
    }


 }



}

  */

void main(int argc, char *argv[])
{

if (!_dos_open("\\misc\\char.lst",O_BINARY | O_RDWR,&ch))
 {  //Already exists
  _dos_read(ch,&cnum,2,&bytes);
  cdlist=(char *)malloc(cnum*15); //Dirs
  cnlist=(char *)malloc(cnum*15); //Name
  _dos_read(ch,cdlist,cnum*15,&bytes);
  _dos_read(ch,cnlist,cnum*15,&bytes);
 }

if (argc) ch=atol(argv[1]);
if (!argc || ch<1 || ch>cnum+1)
 {
  for (i=0; i<cnum; i++)
    printf(" %2d. %s\n",i+1,cnlist+i*15);
  printf("\nEnter character number: ");
  scanf("%d",&ch);
 }
ch--;


printf("Reading %s....\n",cnlist+ch*15);

InitEMSMem();
randomize();
printf ("Conventional memory available: %ld\n",farcoreleft());
printf ("Expanded memory available:     %ld\n",EMSTotal());


//Init mouse
asm{
mov ax,0
int 33h
mov ax,4
mov cx,160
mov dx,100
int 33h

mov ax,0Fh
mov cx,4
mov dx,16
int 33h

//mov ax,07h
//mov cx,0
//mov dx,80
//int 33h
}

InitFixedFont();

ReadLBMFile256(&cursor,pal,"c:\\misc\\cursor.bbm");

_dos_open("c:\\misc\\animator.raw",O_RDONLY | O_BINARY,&h);
t=(char far *)farmalloc(64000);
_dos_read(h,t,64000,&bytes);
_dos_close(h);

//background.pages=4;
//background.ptr=(char far *)MK_FP(0,0);
//baseptr+=64000;
/*AllocEMSMem(64000,&background);
MemoryCopy(GetPtr(background),t,64000);
farfree(t);*/
background.ptr=t; background.pages=0;



screen=(char far *)farmalloc(64000);

//Save original path
getcurdir(0,path+1); path[0]='\\';



chdir("c:\\shadow");
printf("\nReading shadows...\n");
ReadImageFiles();
MemoryCopy(shadowlist,imagelist,256*sizeof(memptr) );
free(filelist);


chdir(cdlist+ch*15);


if (!_dos_open("move.lst",O_BINARY | O_RDWR,&mh))
 {
 if (!stricmp(argv[2],"fix"))
  {
  printf("Opening move description file MOVE.LST...\n");
  _dos_read(mh,&moves,sizeof(move)*100,&bytes);  //Read move list
  _dos_read(mh,&framesize,2,&bytes);
  _dos_read(mh,&frame,framesize,&bytes);	//Read frame list
  }
 else
  {
  printf("Opening move description file MOVE.LST...\n");
  _dos_read(mh,&moves,sizeof(move)*NM,&bytes);  //Read move list
  _dos_read(mh,&framesize,2,&bytes);
  _dos_read(mh,&frame,framesize,&bytes);	//Read frame list
  }

 }
  else
 {
  printf("Creating move description file MOVE.LST...\n");
  _dos_creat("move.lst",0,&mh);
  framesize=1; frame[0]=0;
  for (i=0; i<NM; i++)
      {
       moves[i].firstframe=0;
       moves[i].numframes=0;
      }
  _dos_write(mh,&moves,sizeof(move)*NM,&bytes);
  _dos_write(mh,&framesize,2,&bytes);
  _dos_write(mh,&frame,framesize,&bytes);
 }

_dos_read(mh,&jp,sizeof(joypos)*10*10,&bytes);  //Read joypos list
if (!bytes)
 {
  for (i=0; i<100; i++)
      {
       jp[0][i].pos=0;
       jp[0][i].min=0;
       jp[0][i].max=0;
      }
  _dos_write(mh,&jp,sizeof(joypos)*100,&bytes);
 }

_dos_read(mh,throwptr,32*4*2,&bytes);  //Read throwptr
if (!bytes)
 {
  for (i=0; i<32; i++)
      {
       throwptr[0][i]=0;
       throwptr[1][i]=0;
       throwptr[2][i]=0;
       throwptr[3][i]=0;
      }
  _dos_write(mh,throwptr,32*4*2,&bytes);
 }

_dos_read(mh,&tframesize,2,&bytes);  //Read throw frames
if (!bytes)
 {
  tframesize=1;
  _dos_write(mh,&tframesize,2,&bytes);
  _dos_write(mh,tframe,tframesize,&bytes);
  printf("Creating tframe...\n");
 }
 else _dos_read(mh,tframe,tframesize,&bytes);



if (!stricmp(argv[2],"nothrow"))
 {
  tframesize=1;
  for (i=0; i<4; i++)
//  if (!moves[93+i].numframes)
   for (j=0; j<32; j++)
   throwptr[i][j]=0;

 }





shadowmem=EMSTotal()-EMSFree()-64000;

printf("\nReading images...\n");

ReadImageFiles();

printf("Memory used by images:  %ld \n",((EMSTotal()-EMSFree()-64000-shadowmem)) /1024);
printf("Memory used by frame buffer: %d bytes\n",framesize);
printf("Memory used by throw buffer: %d bytes\n",tframesize);

k=0;
int l=0;
int r=0;


if (!stricmp(argv[2],"nopain") )
for (i=0; i<NM; i++)
 if (moves[i].numframes)
   {
     frameptr=&frame[moves[i].firstframe];
     for (j=0; j<moves[i].numframes; j++)
      {
       image *m=(image *) (frameptr+sizeof(frametype));
       while (m->index!=0xFF)
	   {
	     if (m->orient>>2)
	     {
	      DeleteBytes((char *)m,sizeof(image),&frame[moves[i].firstframe]);
	      ((frametype *)frameptr)->size-=sizeof(image);
	     }
	     else m++;
	   }

       frameptr+=((frametype *)frameptr)->size;
      }
   }



for (i=0; i<NM; i++)
 if (moves[i].numframes)
   {
     frameptr=&frame[moves[i].firstframe];
     for (j=0; j<moves[i].numframes; j++)
      {
       image *m=(image *) (frameptr+sizeof(frametype));

       while (m->index!=0xFF)
	   { k++; m++;  }


       unsigned char *rt= ((unsigned char *)m) +1;
       r+=*rt;
       rt+=(*rt)*sizeof(rect)  +1;
       r+=*rt;
       rt+=(*rt)*sizeof(rect)  +1;

       if (!stricmp(argv[2],"fix"))
	if ( ((frametype *)frameptr)->parm&256)
	 *rt>>=1;

       frameptr+=((frametype *)frameptr)->size;
       l++;
      }
   }

if (!stricmp(argv[2],"fix") || !stricmp(argv[2],"fix2"))
for (i=0; i<4; i++)
 for (j=0; j<32; j++)
  if (throwptr[i][j])
    {
     unsigned char *fp=frame+moves[i+93].firstframe;
     unsigned char *tp=tframe+throwptr[i][j];

     for (int u=0; u<moves[i+93].numframes; u++)
     {
      if ( ((frametype*)fp)->parm&16)
      {
       image *m=(image *) (tp+sizeof(frametype));

       while (m->index!=0xFF)
	   m++;

       unsigned char *rt= ((unsigned char *)m) +1;
       rt+=(*rt)*sizeof(rect)  +1;
       rt+=(*rt)*sizeof(rect)  +1;

       if ( ((frametype *)tp)->parm&256) *rt>>=1;

       tp+=((frametype*)tp)->size;
      }
      fp+=((frametype*)fp)->size;
     }
    }



printf("%5d images %5d bytes\n",k,k*sizeof(image));
printf("%5d frames %5d bytes\n",l,l*(sizeof(frametype)+3));
printf("%5d rects  %5d bytes\n",r,r*sizeof(rect));


oldems=baseptr;

getch();


Mode256();

LoadPalette(pal,0,256);
MemoryCopy(screen,GetPtr(background),64000);

//MemoryCopy((char far *)MK_FP(0xA000,0),screen,64000);

basex=160; basey=160;

o=0;

mstat=0;


//framesize=1;

currmove=RequestMove();

x=basex; y=basey;

//Set up frame pointers
CreateFrameIndex();

frameptr   =&frame[moves[currmove].firstframe];
currframe  =0;
totalframes=moves[currmove].numframes;


updatestat=1;


InitTimer();
SetTimerFunc(TickUpdate);
SetCountRate(18*256/100);
TimerSpeed(1193180L/100);
StartTimer();


do
{

if (kbhit())
{
key=getch();


if (key=='z')
 {
  unsigned char *t=frame+moves[currmove].firstframe;
  int tdur=0,tmove=0;

  for (i=0; i<moves[currmove].numframes; i++)
   {
    tdur+=((frametype *)t)->dur;
    tmove+=((frametype *)t)->dx*((frametype *)t)->dur/((frametype *)t)->tx;
    t+=((frametype *)t)->size;
   }

  char s[20];
  sprintf(s,"TDUR=%d",tdur);
  DrawFixedString(s,(char far *)MK_FP(0xA000,160*320+20) );
  sprintf(s,"TMOVE=%d",tmove);
  DrawFixedString(s,(char far *)MK_FP(0xA000,170*320+20) );
  getch();
 }

if (key=='Z')
 {
  for (i=33; i<=35; i++)
   {
    CopyDelay(i,i+3);
    CopyDelay(i,i+6);
   }
  for (i=60; i<=63; i++)
    CopyDelay(i,i+3);
 }

/*
if (key=='f')
 {
  unsigned char *f=frame+moves[currmove].firstframe;

  for (i=0; i<moves[currmove].numframes; i++)
   {
    image *m=(image *) (f+sizeof(frametype));
    while (m->index!=0xFF )
    {
     m->dispx[0]=-m->dispx[0]- ( (unsigned int)(unsigned char) *(GetPtr(imagelist[m->index])+1) );
     m->orient^=2;
     m++;
    }

 if (((frametype *)f)->parm&256 && ((frametype *)f)->parm&512)
  {
   unsigned char *t=f+sizeof(frametype);
   while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
   t+=1+(*t *sizeof(rect));  //Skip vuln
   t+=1+(*t *sizeof(rect));  //Skip attack

   *((int *)(t+1))=-*((int *)(t+1));
  }


    f+=((frametype *)f)->size;
   }


  if (currmove>=93 && currmove<=96)
   for (j=0; j<32; j++)
    if (throwptr[currmove-93][j])
    {
     unsigned char *fp=frame+moves[currmove].firstframe;
     unsigned char *tp=tframe+throwptr[currmove-93][j];


     for (i=0; i<moves[currmove].numframes; i++)
     {
      if ( ((frametype*)fp)->parm&16)
      {
       image *m=(image *) (tp+sizeof(frametype));
       while (m->index!=0xFF )
       {
	int tmp=m->dispx[1];
	m->dispx[1]=m->dispx[0];
	m->dispx[0]=tmp;
	m->orient^=2;
	m++;
       }
       *((int *)(&((frametype *)tp)->dx))=-*((int *)(&((frametype *)tp)->dx));
       if (((frametype *)tp)->parm&256 && ((frametype *)tp)->parm&512)
	  {
	   unsigned char *t=tp+sizeof(frametype);
	   while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
	   t+=1+(*t *sizeof(rect));  //Skip vuln
	   t+=1+(*t *sizeof(rect));  //Skip attack

	   *((int *)(t+1))=-*((int *)(t+1));
	  }
       tp+=((frametype*)tp)->size;
      }
      fp+=((frametype*)fp)->size;
     }


    }
 }
*/


if (key=='*')
   slowdown^=1;

if (key==13 && playback)
 {
  playloop=0;
  playback=0;
  updatestat=1;
  x=basex; y=basey;
 }

if (dothrow!=1)
{
if (key==75 && currframe>0)
      {
       currframe--;
       frameptr=frameindex[currframe];
       updatestat=1;

       if (dothrow==2) tframeptr=tframeindex[currframe];
      }
if (key==77 && currframe<totalframes-1)
      {
       currframe++;
       frameptr=frameindex[currframe];
       updatestat=1;
       if (dothrow==2) tframeptr=tframeindex[currframe];
       }
}


if (key=='[' && !(((frametype *)frameptr)->parm&8192) )  // Copy
   {
     if (dothrow==2)
       {
     image *t=(image *) (tframeptr+sizeof(frametype));
     clipsize=0;
     while (t->index!=0xFF) {t++; clipsize+=sizeof(image); }

     MemoryCopy(clipboard,tframeptr+sizeof(frametype),clipsize);
     clipptr=0;
       }
    else
    {

     image *t=(image *) (frameptr+sizeof(frametype));
     clipsize=0;
     while (t->index!=0xFF) {t++; clipsize+=sizeof(image); }

     MemoryCopy(clipboard,frameptr+sizeof(frametype),clipsize);
     clipptr=frameptr+sizeof(frametype);
    }
   }

if (key=='[' && (((frametype *)frameptr)->parm&8192) )  // Unlink
  {
   ((frametype *)frameptr)->size-=2;
   DeleteBytes(frameptr+sizeof(frametype),2,frame+moves[currmove].firstframe);
//   ((frametype *)frameptr)->parm&=~8192;
  }


if (key==']' && clipsize)
{
 if (dothrow==2)
 {
  image *t=(image *) (tframeptr+sizeof(frametype));
  int a=0;
  while (t->index!=0xFF) {t++; a+=sizeof(image); }
  DeleteBytes(tframeptr+sizeof(frametype),a,tframe+throwptr[currmove-93][throwee]);

  InsertBytes(tframeptr+sizeof(frametype),clipsize,tframe+throwptr[currmove-93][throwee]);
  MemoryCopy (tframeptr+sizeof(frametype),clipboard,clipsize);

  ((frametype *)tframeptr)->size+=-a + clipsize;
 }
 else
 {
 /*
  if (!(((frametype *)frameptr)->parm&8192))
   { //LINK
    image *t=(image *) (frameptr+sizeof(frametype)); //Delete old images
    int a=0;
    while (t->index!=0xFF) {t++; a+=sizeof(image); }
    ((frametype *)frameptr)->size+=-a;
    DeleteBytes(frameptr+sizeof(frametype),a,frame+moves[currmove].firstframe);

    ((frametype *)frameptr)->size+=2;
    InsertBytes(frameptr+sizeof(frametype),2,frame+moves[currmove].firstframe);
    MemoryCopy (frameptr+sizeof(frametype),clipptr,2);


    ((frametype *)frameptr)->parm|=8192;
   } else MemoryCopy (frameptr+sizeof(frametype),clipptr,2);*/

     image *t=(image *) (frameptr+sizeof(frametype));
     int a=0;
     while (t->index!=0xFF) {t++; a+=sizeof(image); }

     ((frametype *)frameptr)->size+=-a;
     DeleteBytes(frameptr+sizeof(frametype),a,frame+moves[currmove].firstframe);


     ((frametype *)frameptr)->size+=clipsize;
     InsertBytes(frameptr+sizeof(frametype),clipsize,frame+moves[currmove].firstframe);
     MemoryCopy (frameptr+sizeof(frametype),clipboard,clipsize);
 }



CreateFrameIndex();
}





if (key=='}' && clipsize && !dothrow && !(((frametype *)frameptr)->parm&8192))  //COPY
  {
     image *t=(image *) (frameptr+sizeof(frametype));
     int a=0;
     while (t->index!=0xFF) {t++; a+=sizeof(image); }

     ((frametype *)frameptr)->size+=-a;
     DeleteBytes(frameptr+sizeof(frametype),a,frame+moves[currmove].firstframe);


     ((frametype *)frameptr)->size+=clipsize;
     InsertBytes(frameptr+sizeof(frametype),clipsize,frame+moves[currmove].firstframe);
     MemoryCopy (frameptr+sizeof(frametype),clipboard,clipsize);


     CreateFrameIndex();
  }






if (mstat && !(((frametype *)frameptr)->parm&8192))
 {
  if (key==83)
   {
    if (dothrow!=2)
     {
      ((frametype *)(frameptr))->size-=sizeof(image);
      DeleteBytes((char *)dragimg,sizeof(image),frame+moves[currmove].firstframe);
     }
   else
     {
      ((frametype *)(tframeptr))->size-=sizeof(image);
      DeleteBytes((char *)dragimg,sizeof(image),tframe+throwptr[currmove-93][throwee]);
     }

    dragimg=0; mstat=0;
    //Adjust frame ptr array
    CreateFrameIndex();
   }

  if (key=='x') dragimg->orient^=2;
  if (key=='y') dragimg->orient^=1;

  if (key=='B')
    {
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
     if (dragimg>(image *) (frameptr+sizeof(frametype)) )
       {
	image t=*dragimg;
	MemoryCopy(dragimg,dragimg-1,sizeof(image) );
	dragimg--;
	*dragimg=t;
       }
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
    }

  if (key=='b')
    {
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
     while(dragimg>(image *) (frameptr+sizeof(frametype)) )
       {
	image t=*dragimg;
	MemoryCopy(dragimg,dragimg-1,sizeof(image) );
	dragimg--;
	*dragimg=t;
       }
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
    }



 }

if (!mstat)
 { //WERE NOT DRAGGING
  if (key=='d' && totalframes && dothrow!=2)
   {
     //Delete throwee frame
     if (currmove>=93 && currmove<=96)
     if ( ((frametype *)frameptr)->parm&16) //If one exists
      {
       i=currmove-93;
       for (j=0; j<32; j++)
	if (throwptr[i][j])
	 {
	  char *a=&tframe[throwptr[i][j]];
	  for (int l=0; l<currframe; l++)
	   if ( ((frametype *)frameindex[l])->parm&16) //If one exists, skip it
		  a+=((frametype *)a)->size;
	  DeleteBytes(a,((frametype *)a)->size,tframe+throwptr[i][j]);
	  if (!totalframes) throwptr[i][j]=0;
	 }
      }

     //Delete real frame
     DeleteBytes(frameptr,((frametype *)(frameptr))->size,frame+moves[currmove].firstframe);

     totalframes--;		//One less frame
     moves[currmove].numframes--;

     if (!totalframes)
      {
       moves[currmove].firstframe=0;
       frameptr=frame;
      }



     //Adjust frame ptr array
     if (totalframes && currframe>totalframes-1) currframe--;
     CreateFrameIndex();
     if (totalframes)
      {
       frameptr=frameindex[currframe];
       tframeptr=tframeindex[currframe];
      }
     updatestat=1;
   }



  if (key=='I' && totalframes && dothrow!=2)
   { frameptr+=((frametype *)(frameptr))->size;	}

  if (toupper(key)=='I' && dothrow!=2)
   { //Insert frame

    if (!totalframes) //Create new move
     {
      moves[currmove].firstframe=framesize;
      moves[currmove].numframes=0;
      frameptr=&frame[framesize];

      framesize+=sizeof(frametype)+3;
     }
     else
    InsertBytes(frameptr,sizeof(frametype)+3,frame+moves[currmove].firstframe);

    //Set up new frame
    ((frametype *)frameptr)->size =sizeof(frametype)+3; //Set size
    ((frametype *)frameptr)->shadow=0; //Set shadow
    ((frametype *)frameptr)->dur  =20; //Set Delay
    ((frametype *)frameptr)->parm   =defs[currmove].parm; //Set Dx
    ((frametype *)frameptr)->se     =0; //Set Dx
    ((frametype *)frameptr)->energy   =defs[currmove].energy; //Set Dx
    ((frametype *)frameptr)->dx   =0; //Set Dx
    ((frametype *)frameptr)->dy   =0; //Set Dy
    ((frametype *)frameptr)->tx   =1; //Set tx
    ((frametype *)frameptr)->ty   =1; //Set ty

    *(frameptr+sizeof(frametype)+0)=0xFF; //Set image term
    *(frameptr+sizeof(frametype)+1)=0x00; //Set vulnerable number
    *(frameptr+sizeof(frametype)+2)=0x00; //Set attack number

    totalframes++;		//One more move
    moves[currmove].numframes++;

    //Adjust frame ptr array
    CreateFrameIndex();
    frameptr=frameindex[currframe];
    tframeptr=tframeindex[currframe];

    updatestat=1;
  }

  if (key=='m' && dothrow!=2)
   {
    currmove=RequestMove();
      while (ReadMouse(&cx,&cy));

    CreateFrameIndex();
    frameptr   =&frame[moves[currmove].firstframe];
    currframe  =0;
    totalframes=moves[currmove].numframes;
    updatestat=1;
     basex=x=160;
     basey=y=160;

    if (e>3) e=3;

    dothrow=0; throwee=0;

   }

  if (key=='s')   Save();

  if (key=='u')    y-=5;

  if (key=='v') CreateFrameIndex();

//THROW_********************************************************
 if (currmove>=93 && currmove<=96)
  {
   if (key=='t')
    {
     dothrow++;

     if (dothrow>2) {dothrow=0;

	 for (j=0; j<4; j++)  //finish up
	  if (throwptr[j][throwee])
	  {
   char *tp=&tframe[throwptr[j][throwee]];
   char *fp=frame+moves[j+93].firstframe;
   for (i=0; i<moves[j+93].numframes; i++)
   {
    if ( ((frametype*)fp)->parm&16)
      {
       image *m=(image *) (tp+sizeof(frametype));
       while (m->index!=0xFF )
       {
	m->dispx[1]=-m->dispx[0]- ( (unsigned int)(unsigned char) *(GetPtr(timagelist[m->index])+1) );
	m++;
       }
       tp+=((frametype*)tp)->size;
      }
    fp+=((frametype*)fp)->size;
   }
	  }

	  throwee=0;
     }

     if (dothrow==2) //Read char
      {
       if (tfilelist) free(tfilelist);
       baseptr=oldems;

       ReadImageFiles2(cdlist+throwee*15);


       if (!throwptr[currmove-93][throwee])  //need to make him frames
	{
	 throwptr[currmove-93][throwee]=tframesize;
	 tframeptr=tframe+tframesize;
	 for (i=0; i<totalframes; i++)
	  if ( ((frametype *)frameindex[i])->parm&16 )
	   {
	    InsertBytes(tframeptr,sizeof(frametype)+3,tframe+throwptr[currmove-93][throwee]);
	    //Set up new frame
	    ((frametype *)tframeptr)->size =sizeof(frametype)+3; //Set size
	    ((frametype *)tframeptr)->shadow=0; //Set shadow
	    ((frametype *)tframeptr)->dur  =((frametype *)frameindex[i])->dur; //Set Delay
	    ((frametype *)tframeptr)->parm   =0; //Set
	    ((frametype *)tframeptr)->se     =0; //Set
	    ((frametype *)tframeptr)->energy   =0; //Set Dx
	    *((int *)(&((frametype *)tframeptr)->dx))   =0; //Set x in relation  to thrower
	    *((int *)(&((frametype *)tframeptr)->tx))   =0; //Set y in relation  to thrower

	    *(tframeptr+sizeof(frametype)+0)=0xFF; //Set image term
	    *(tframeptr+sizeof(frametype)+1)=0x00; //Set vulnerable number
	    *(tframeptr+sizeof(frametype)+2)=0x00; //Set attack number

	    tframeptr+=sizeof(frametype)+3;
	   }
	}


       CreateFrameIndex();
       tframeptr=tframe+throwptr[currmove-93][throwee];
      }

    }


   if (dothrow==1) ///Selecting
    {
     if (key==75 && throwee>0) throwee--;
     if (key==77 && throwee<cnum-1) throwee++;

     if (key=='#')
       {
	tframesize=throwptr[currmove-93][throwee];
	throwptr[currmove-93][throwee]=0;
       }

    }

   if (dothrow==2 && ((frametype *)frameptr)->parm&16 )
    {
     if (key=='l')
     {
      *((int *)(&((frametype *)tframeptr)->dx))=cx-basex;
      *((int *)(&((frametype *)tframeptr)->tx))=0;
     }
     if (key=='L')
     {
      *((int *)(&((frametype *)tframeptr)->dx))=cx-basex;
      *((int *)(&((frametype *)tframeptr)->tx))=cy-basey;
     }

    }


  } else dothrow=0;







 if (e==0 && dothrow!=2)
 {

  if (key>='1' && key<='5') editstat=key-'1';

  if (key=='-')
     {
      frametype *f=(frametype *)frameptr;

      if (editstat==0 && f->dur>1) f->dur--;
      if (editstat==1 && f->dx >-99) f->dx --;
      if (editstat==2 && f->tx >1) f->tx --;

      if (editstat==3 && f->dy >-99) f->dy --;
      if (editstat==4 && f->ty >1) f->ty --;
      updatestat=1;
     }

  if (key=='+')
     {
      frametype *f=(frametype *)frameptr;

      if (editstat==0 && f->dur<255) f->dur++;
      if (editstat==1 && f->dx < 99) f->dx ++;

      if (editstat==2 && f->tx<99) f->tx ++;

      if (editstat==3 && f->dy < 99) f->dy ++;
      if (editstat==4 && f->ty<99) f->ty ++;

      updatestat=1;
     }
 }

 if (e==3 &&  ( ((frametype *)frameptr)->parm&16 || dothrow!=2) )
  {
    if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

    if (key=='-' && ((frametype *)frameptr)->energy>0) ((frametype *)frameptr)->energy--;
    if (key=='+' && ((frametype *)frameptr)->energy<200) ((frametype *)frameptr)->energy++;


   unsigned char *t=frameptr+sizeof(frametype);
   while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
   t+=1+(*t *sizeof(rect));  //Skip vuln
   t+=1+(*t *sizeof(rect));  //Skip attack
   int a=(*t)&15;

   if (key==',' && a>0) a--;
   if (key=='.' && a<15) a++;
   *t&=~15;
   *t|=a;

     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

  }

  if (key=='P') playloop=1;
  if (tolower(key)=='p')
   {
    currframe=0; frameptr=frameindex[currframe];  tframeptr=tframeindex[currframe];
    dur=((frametype *)frameptr)->dur;
    tcountx=tcounty=0;

    for (i=0; i<totalframes; i++)
       {
	framemove[i].x=((int) ((frametype *)frameindex[i])->dx)*256/((frametype *)frameindex[i])->tx;
	framemove[i].y=((int) ((frametype *)frameindex[i])->dy)*256/((frametype *)frameindex[i])->ty;
       }
    playback=1;
   }

  if (key=='b')
    {
     basex=x=cx;
     basey=y=cy;
    }
 if (key=='r')
    {
     x=basex; y=basey;
    }

 if (key=='h')
   {
    pain++; pain&=3;
   }

 if (key=='c' && totalframes==0)
   {
    int i,j,k;
    char *p;

    i=RequestMove();
    moves[currmove].numframes=totalframes=moves[i].numframes;
    moves[currmove]=moves[i];
    currframe  =0;

    for (j=k=0, p=&frame[moves[i].firstframe]; k<totalframes; k++)
     {
      j+=((frametype *)p)->size;
      p+=((frametype *)p)->size;
     }

    memmove(frame+framesize,frame+moves[i].firstframe,j);

    frameptr=&frame[framesize];
    moves[currmove].firstframe=framesize;
    framesize+=j;

    if (currmove>=93 && currmove<=96 && i>=93 && i<=96)
     {
      for (j=0; j<32; j++)
       {
	throwptr[currmove-93][j]=throwptr[i-93][j];
	throwptr[i-93][j]=0;
       }
     }



    CreateFrameIndex();
    updatestat=1;
   }

 if (key=='@')
   for (i=0; i<60; i++)
    if (moves[i].numframes)
    {
     char *f=frame+moves[i].firstframe;

     for(j=0; j<moves[i].numframes; j++)
	{
	  if (j==0) ((frametype *)f)->se =     defs[i].se;
	    else ((frametype *)f)->se =     defs[i].se & (~15);
	 ((frametype *)f)->parm =   defs[i].parm;
	 ((frametype *)f)->energy = defs[i].energy;

	 f+=((frametype *)f)->size;
	}
    }

if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
 if (key=='e' && ((frametype *)frameptr)->parm&256 && ((frametype *)frameptr)->parm&512)
  {
   unsigned char *t=frameptr+sizeof(frametype);
   while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
   t+=1+(*t *sizeof(rect));  //Skip vuln
   t+=1+(*t *sizeof(rect));  //Skip attack

   if (dothrow!=2)
    {
   *((int *)(t+1))=cx-x;
   *((int *)(t+3))=cy-y;
   }
   else
    {
   *((int *)(t+1))=cx-x-*((int *)(&((frametype *)frameptr)->dx));
   *((int *)(t+3))=cy-y-*((int *)(&((frametype *)frameptr)->tx));
   }
  }
if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}


 }

} else key=0;


//if (dothrow==2) getch();

MemoryCopy(screen,GetPtr(background),64000);

if (totalframes)
 {
  DrawShadow(frameptr, x, y );
  if (dothrow==2 && ((frametype *)frameptr)->parm&16) DrawShadow(tframeptr, x+*((int *)(&((frametype *)tframeptr)->dx)), y+*((int *)(&((frametype *)tframeptr)->tx)) );


  if (dothrow==2 && ((frametype *)frameptr)->parm&16)
   {
    if ( ( ((frametype *)frameptr)->parm&4096)) DrawFrame(frameptr, x, y,imagelist );
    DrawFrame(tframeptr, x+*((int *)(&((frametype *)tframeptr)->dx)), y+*((int *)(&((frametype *)tframeptr)->tx)),timagelist );
    if (!( ((frametype *)frameptr)->parm&4096)) DrawFrame(frameptr, x, y,imagelist );
   }
  else DrawFrame(frameptr, x, y,imagelist );


  if (mstat && e>0) //Draw rect
   {
    int x1,y1,x2,y2;

    if (rg.x1>rg.x2) {x1=rg.x2; x2=rg.x1;} else {x1=rg.x1; x2=rg.x2;}
    if (rg.y1>rg.y2) {y1=rg.y2; y2=rg.y1;} else {y1=rg.y1; y2=rg.y2;}

    DrawBar(screen,e==1 ? 1 : 4, x1,y1, x2-x1,y2-y1);
   }

  if (e==3 && (dothrow!=2 || ((frametype *)frameptr)->parm&16) ) DrawOptions();

  if (e==4) DrawJoyMove();


  if (x>330) x-=330;
  if (x<-10) x+=330;
 }

DrawStatusBar();


b=ReadMouse(&cx,&cy);

//If frame exists
//if (dothrow!=2 ||  ((frametype *)frameptr)->parm&16)
{

if (b&1 && totalframes && !playback) //left button
 {
  asm cli;
   if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

  if  (dothrow==2)
   {
    ::x+=*((int *)(&((frametype *)frameptr)->dx));
    ::y+=*((int *)(&((frametype *)frameptr)->tx));
   }


  if (!mstat) //We haven't dragged yet
    {
    //Check dragging
     if (dothrow!=2 || ((frametype *)tframeptr)->parm&16)
       if (e==0)
	{
	 dragimg=ImageMouseBoundCheck(); //See if clicking on an image
	 if (dragimg)
	     {   mstat=1;    }

	 if (MouseBound(266,296,181,189)) editstat=0;
	 if (MouseBound(78,102,190,198))  editstat=1;
	 if (MouseBound(112,134,190,198)) editstat=2;
	 if (MouseBound(151,174,190,198)) editstat=3;
	 if (MouseBound(184,206,190,198)) editstat=4;
	} else
	 if (e<3)
	if (cy<180)
	 { //Make region
	  mstat=1;
	  rg.x1=rg.x2=cx;
	  rg.y1=rg.y2=cy;
	 }

       if (MouseBound(240,320,190,200))
	    {
	     updatestat=1;
	     if ((currmove<78 || currmove>87) && currmove!=96)  {if (++e>3) e=0;}
	      else {if (++e>4) e=0;}
	     while (ReadMouse(&cx,&cy));
	    }
    }
  else
    {
     if (e==0)
      {	 //We're still dragging
       dragimg->dispx[0]=dragrel.x+cx-x;
       dragimg->dispy   =dragrel.y+cy-y;

       pos[dragimg->index].x=dragrel.x+cx;
       pos[dragimg->index].y=dragrel.y+cy;

      }
     else
     if (e<3)
      { rg.x2=cx; rg.y2=cy;}
    }

if  (dothrow==2)
 {::x-=*((int *)(&((frametype *)frameptr)->dx));
  ::y-=*((int *)(&((frametype *)frameptr)->tx));
 }

    if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
   asm sti;
 } else
  if (mstat)               //Release button
   if (e==0)   { mstat=0; dragimg=0; }
  else  //INSERT RECTANGLE
  if (e<3)
  {

   unsigned char *t=frameptr+sizeof(frametype);
   mstat=0;

   //Skip images
   while (*t!=0xFF) t+=sizeof(image);  t++;

   if (e==1)
    {
     rgptr=(rect *) (t+1+ (*t *sizeof(rect)) );
     (*t)++; //Increase rects
    }
   if (e==2)
    {
     t+=1+(*t *sizeof(rect));
     rgptr=(rect *) (t+1+ (*t *sizeof(rect)) );

     (*t)++; //Increase rects
    }


  InsertBytes((char *)rgptr,sizeof(rect),frame+moves[currmove].firstframe);
  ((frametype *)(frameptr))->size+=sizeof(rect);

  if (rg.x2>rg.x1) { rgptr->x1=rg.x1-x; rgptr->x2=rg.x2-x; }
     else          { rgptr->x1=rg.x2-x; rgptr->x2=rg.x1-x; }
  if (rg.y2>rg.y1) { rgptr->y1=rg.y1-y; rgptr->y2=rg.y2-y; }
     else          { rgptr->y1=rg.y2-y; rgptr->y2=rg.y1-y; }

  CreateFrameIndex();

 }

if (b&2 && totalframes && !playback && !mstat)
 if (e==0)  //Get another graphics
 {
  while (ReadMouse(&cx,&cy));

  if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

  if (dothrow!=2) i=RequestImage(imagelist,filelist,numfiles);
   else           i=RequestImage(timagelist,tfilelist,tnumfiles);



	image *z=(image *) (frameptr+sizeof(frametype));
	while (z->index!=0xFF) z++;

	if (dothrow==2)
	InsertBytes((char *)z,sizeof(image),tframe+throwptr[currmove-93][throwee] );
	else
	InsertBytes((char *)z,sizeof(image),frame+moves[currmove].firstframe );


	((frametype *)(frameptr))->size+=sizeof(image);;

	z->index=i;
	z->orient=pain<<2;

	if (pos[i].x==0)
	 {
	  z->dispx[0]=0;
	  z->dispy   =-80;
	 } else
	 {
	  z->dispx[0]=pos[i].x-x;
	  z->dispy   =pos[i].y-y;
	 }
  if (dothrow==2)
 {z->dispx[0]-=*((int *)(&((frametype *)frameptr)->dx));
  z->dispy   -=*((int *)(&((frametype *)frameptr)->tx));
 }


  if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}


	//Adjust frame ptr array
	CreateFrameIndex();
 }
 else
  {
   unsigned char *t=frameptr+sizeof(frametype);
   mstat=0;

   //Skip images
   while (*t!=0xFF) t+=sizeof(image);  t++;

   if (e==2)
    { t+=1+( (*t) *sizeof(rect));  }


   rgptr=(rect *) (t+1);


   DeleteBytes((char *)rgptr,sizeof(rect)* (*t),frame+moves[currmove].firstframe );
   ((frametype *)(frameptr))->size-=sizeof(rect)* (*t);

   *t=0;
   CreateFrameIndex();

  }
}




if (!mstat) DrawImage2(cursor,screen,cx,cy,0);

if (pain)
 {
  switch (pain)
  {
   case 1: DrawFixedString("PAIN1",screen+260+320*170); break;
   case 2: DrawFixedString("PAIN2",screen+260+320*170); break;
   case 3: DrawFixedString("PAIN3",screen+260+320*170); break;
  }
 }

if (dothrow)
 {
  DrawFixedString(cnlist+throwee*15,screen+100+320*170);
 }


MemoryCopy((char far *)MK_FP(0xA000,0),screen,64000);


} while(key!=27);


StopTimer();
TimerSpeed(0xFFFF);
SetCountRate(0x0100);

TerminateTimer();

TerminateEMSMem();


ModeText();

printf("Copyright 1993 Icer Addis.   All rights reserved.\n");

printf("Did you want to save? (Y/N)\n");

key=toupper(getch());

if (key=='Y')
  Save();

 chdir(path);

 for (j=0; j<4; j++)
 {
 for (i=0; i<32; i++)
  printf("%d ",throwptr[j][i]);
 printf("\n");
 }

}

void Save()
{
/*
 for (j=0; j<4; j++)
  for (k=0; k<32; k++)
  if (throwptr[j][k])
  {
   char *tp=&tframe[throwptr[j][k]];
   char *fp=frame+moves[j+93].firstframe;
   for (i=0; i<moves[j+93].numframes; i++)
   {
    if ( ((frametype*)fp)->parm&16)
      {
       image *m=(image *) (tp+sizeof(frametype));
       while (m->index!=0xFF )
       {
	m->dispx[1]=-m->dispx[0]- ( (unsigned int)(unsigned char) *(GetPtr(timagelist[m->index])+1) );
	m++;
       }
       tp+=((frametype*)tp)->size;
      }
    fp+=((frametype*)fp)->size;
   }
  }
  */

   lseek(mh,0,SEEK_SET);
   _dos_write(mh,&moves,sizeof(move)*NM,&bytes);
   _dos_write(mh,&framesize,2,&bytes);
   _dos_write(mh,&frame,framesize,&bytes);

   _dos_write(mh,&jp,sizeof(joypos)*100,&bytes);

   _dos_write(mh,throwptr,32*4*2,&bytes);

   _dos_write(mh,&tframesize,2,&bytes);
   _dos_write(mh,tframe,tframesize,&bytes);


   _dos_write(mh,0,0,&bytes);

}

int u;

void _loadds TickUpdate()
{
u++;

if (playback && (!slowdown || !((slowflag++)&3) )  )
 {
/* if (tcountx) tcountx--; else
   {
    x+=((frametype *)frameptr)->dx;
    tcountx=((frametype *)frameptr)->tx -1;
   }

 if (tcounty) tcounty--; else
   {
    y+=((frametype *)frameptr)->dy;
    tcounty=((frametype *)frameptr)->ty -1;
   }*/

 tcountx+=framemove[currframe].x;
 tcounty+=framemove[currframe].y;

 x+=tcountx>>8; tcountx&=0xFF;
 y+=tcounty>>8; tcounty&=0xFF;


 if (dur && y<=basey) dur--;
  else
  {
   if (y>basey) y=basey;

   currframe++;
   if (currframe>=totalframes)
    {currframe=0; if (!playloop) {playback=0; updatestat=1;} }


   frameptr=frameindex[currframe];
   tframeptr=tframeindex[currframe];
   dur=((frametype *)frameptr)->dur;
  }

 }
}


void AdjustMoves(char *p,int d,char *s)
{
int i;

if (s<tframe)
{
framesize+=d;

for (i=0; i<NM; i++)
  if (&frame[moves[i].firstframe]>=p && (frame+moves[i].firstframe)!=s) //currmove!=i )
      moves[i].firstframe+=d;
}
else
{
tframesize+=d;
for (int j=0; j<4; j++)
 for (i=0; i<32; i++)
  if (&tframe[throwptr[j][i]]>=p && (tframe+throwptr[j][i])!=s) throwptr[j][i]+=d;
}


}

void CreateFrameIndex()
{
int i;
  //Adjust frame ptr array
  frameindex[0]=&frame[moves[currmove].firstframe];
  for (i=1; i<moves[currmove].numframes; i++)
    frameindex[i]=frameindex[i-1]+ ((frametype *)frameindex[i-1])->size;

  if (dothrow==2)
  {
  //Adjust throw frame ptr array
  char *tp=&tframe[throwptr[currmove-93][throwee]];
  for (i=0; i<moves[currmove].numframes; i++)
   {
    tframeindex[i]=tp;
//    if (i)
     if (((frametype *)frameindex[i])->parm&16) tp+=((frametype *)tp)->size;
   }
  }

//  frameptr=frameindex[currframe];

}

void DrawStatusBar()
{
char s[80];


if (!updatestat) return;

char far *bg=GetPtr(background);

DrawBar(bg,27,0,180, 320,20);

if (!totalframes)
{
  sprintf(s,"%-18s",movenames[currmove]);
  DrawFixedString(s,bg+0+320*181);
}
else
if (e==0)
 {
  sprintf(s,"%-18s %2d/%2d   Delay: %3d",
   movenames[currmove],currframe+1,totalframes,((frametype *)frameptr)->dur);
  DrawFixedString(s,bg+0+320*181);

  sprintf(s,"%s  %3d/%3dX %3d/%3dY     GRAPHIC",
   ((frametype *)frameptr)->parm&8192 ? "LINKED" : "      ",
   ((frametype *)frameptr)->dx, ((frametype *)frameptr)->tx,
   ((frametype *)frameptr)->dy, ((frametype *)frameptr)->ty


   );

  DrawFixedString(s,bg+0+320*(181+9) );
 }
else
if (e==1)
 {
  sprintf(s,"%-18s %2d/%2d   Delay: %3d",
   movenames[currmove],currframe+1,totalframes,((frametype *)frameptr)->dur);
  DrawFixedString(s,bg+0+320*181);

  if (dothrow!=2)
    sprintf(s,"                             VULNERABLE");
  else
   {
    sprintf(s,"BASE: %4X                    VULNERABLE",throwptr[currmove-93][throwee]);

   }


  DrawFixedString(s,bg+0+320*(181+9) );
 }
else
if (e==2)
 {
  sprintf(s,"%-18s %2d/%2d   Delay: %3d",
   movenames[currmove],currframe+1,totalframes,((frametype *)frameptr)->dur);
  DrawFixedString(s,bg+0+320*181);

  sprintf(s,"                                 ATTACK");

  DrawFixedString(s,bg+0+320*(181+9) );
 }
else
if (e==3 || e==4)
 {
  sprintf(s,"%-18s %2d/%2d   Delay: %3d",
   movenames[currmove],currframe+1,totalframes,((frametype *)frameptr)->dur);
  DrawFixedString(s,bg+0+320*181);

  if (e==3) sprintf(s,"                                OPTIONS");
   else sprintf(s,"                               JOYSTICK");

  DrawFixedString(s,bg+0+320*(181+9) );
 }



updatestat=0;

}


int MouseBound(int x1,int x2,int y1,int y2)
{
if (cx>=x1 && cx<=x2 && cy>=y1 && cy<=y2) return(1);

return(0);
}

image * ImageMouseBoundCheck()
{

image *i=(image *) (frameptr+sizeof(frametype));

image timg;

image *select;

int done=0;
int x,y;




while (i->index!=0xFF) // && !done)
{
  //Check for boundage

char far *t;
if (dothrow==2) t=GetPtr(timagelist[i->index]);
 else           t=GetPtr(imagelist[i->index]);

if (MouseBound(::x+i->dispx[0],::x+i->dispx[0]+((int)(unsigned char)t[1]),
	       ::y+i->dispy-80,::y+i->dispy-80+((int)(unsigned char)t[2]) )  && ((i->orient>>2)==pain))
 { //Move to front

   dragrel.x=::x+i->dispx[0]-cx;
   dragrel.y=::y+i->dispy   -cy;

   done=1;
   select=i;
 }

  i++;


}

i--;

if (!done) i=NULL;  //None found
   else
    {
     timg=*select;
     memmove(select,select+1,
		((char *)i)-((char *)select) );
//     i=(image *) (frameptr+sizeof(frametype)); //its moved to the first img
     *i=timg;
//      i=select;
    }



return(i);


}


void DrawShadow(char *f, int x, int y)
{
y++;
char far *s=GetPtr(shadowlist[((frametype *)f)->shadow] );
if (s) DrawImage2(s,screen,x-((int)(unsigned char)s[1])/2,basey-((int)(unsigned char)s[2])/2,0);
}

void DrawFrame(char *f, int x, int y, memptr *imagelist)
{
image *i=(image *) (f+sizeof(frametype));

while ( i->index!=0xFF )
{
 if ((i->orient>>2)<=pain)
  if (imagelist[i->index].pages)
  DrawImage2(GetPtr(imagelist[i->index]),screen,x+i->dispx[0],y+i->dispy-80,i->orient);
 i++;
}


((unsigned char *)i)++; //Point to first count of rects


if (e==1)
  {
   rect *r=(rect *)  (  ((char *)i)+1 );

   for (j=*((unsigned char *)i); j>0; j--)
    {
//     DrawBar(screen,1, x,y,10,10);
     DrawBar(screen,1, x+r->x1,y+r->y1,r->x2-r->x1,r->y2-r->y1);
     r++;
    }
  }

if (e>=2)
  {
   rect *r=(rect *) ( ((char *)i)+1+*((unsigned char *)i)*sizeof(rect) +1  );
  isattack=0;
   for (j=*( ((unsigned char *)r)-1); j>0; j--)
    {
      isattack=1;
     DrawBar(screen,4, x+r->x1,y+r->y1, r->x2-r->x1,r->y2-r->y1);
     r++;
    }
 }

 asm cli;
//if (dothrow==2 && f>tframe) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

//if (dothrow!=2 || frameptr>tframe)
if (((frametype *)f)->parm&256 && ((frametype *)f)->parm&512 && u&8)
 {
   unsigned char *t=f+sizeof(frametype);
   while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
   t+=1+(*t *sizeof(rect));  //Skip vuln
   t+=1+(*t *sizeof(rect));  //Skip attack

   int tx=*((int *)(t+1))+x;
   int ty=*((int *)(t+3))-1+y;

   screen[tx+ty*320]=1; ty++; tx--;
   screen[tx+ty*320]=1; tx++;
   screen[tx+ty*320]=1; tx++;
   screen[tx+ty*320]=1; tx--; ty++;
   screen[tx+ty*320]=1;
 }

//if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
asm sti;

}



int RequestImage(memptr *imagelist,imagefile *filelist,int numfiles)
{
static int img=0;

char f[9];

char far *screen2=(char far *)farmalloc(64000);


do
{
MemoryCopy(screen2,GetPtr(background),64000);

if (imagelist[img].pages)
DrawImage2(GetPtr(imagelist[img]),screen2,160-(int)(unsigned char)(GetPtr(imagelist[img])[1])/2,
	      160-(int)(unsigned char)(GetPtr(imagelist[img])[2]),0);

MemoryCopy(f,filelist[img].filename,8); f[8]=0;
DrawFixedString(f,screen2+161*320+160-strlen(f)*4);

b=ReadMouse(&cx,&cy);
DrawImage2(cursor,screen2,cx,cy,0);

MemoryCopy((char far *)MK_FP(0xA000,0),screen2,64000);

if (b&1 && cy<180)
 {
  if (cx<160 &&  img>0) {img--; while (ReadMouse(&cx,&cy)); }
  if (cx>=160 && img<numfiles) {img++; while (ReadMouse(&cx,&cy)); }
 }

if (kbhit())
 {
  key=getch();

  if (key==0)
   {
    key=getch();

  if (key==75 &&  img>0) {img--; }
  if (key==77 && img<numfiles) {img++; }

   }


 }

/*
if (cy<180)
 {
  if (b&1 && img>0) {img--; while (ReadMouse(&cx,&cy)); }
  if (b&2 && img<numfiles) {img++; while (ReadMouse(&cx,&cy)); }
 }*/

} while (!(b&2) || !imagelist[img].pages);   //(!(cy>=180 && b));

while (ReadMouse(&cx,&cy));

farfree(screen2);

return(img);

}



void DrawJoyMove()
{

 int x=0;
 int y=10;

 if (currmove<78) {e=3; return;}
 joypos *cjp=&jp[currmove-78][0];

 if (currmove==96) cjp=&jp[9][0];

 ujp=cjp;

 for (i=0; i<10; i++)
  {

     char s[50];
//   if (cjp->max)
   sprintf(s,"%2d. MIN:%4d MAX:%4d %2sDIR %s %s%s%s%s%s%s",
	 i+1,
	 cjp->min,
	 cjp->max,
	 (cjp->pos&64)  ? "  " : "IN",
	 (cjp->pos&128) ? "BPON " : "BPOFF",
	 cjp->pos&8 ? "D" : "",
	 cjp->pos&4 ? "U" : "",
	 cjp->pos&2 ? "R" : "",
	 cjp->pos&1 ? "L" : "",
	 (cjp->pos&16) ? "B1" : "",
	 (cjp->pos&32) ? "B2" : ""
	 );
//   else    sprintf(s,"%2d. NONE",i+1);

   DrawFixedString(s,screen+y*320+x);

   if (cy>=y && cy<y+10) ujp=cjp;

   cjp++;
   y+=10;
  }

 if (b&1 && cx>22*8 && cx<26*8)
  { ujp->pos^=64;
  while (ReadMouse(&cx,&cy));
  }

 if (b&1 && cx>28*8 && cx<34*8)
  { ujp->pos^=128;
  while (ReadMouse(&cx,&cy));
  }

 if (key=='+')
   {
    if (cx>=9*8 && cx<13*8) ujp->min++;
    if (cx>=17*8 && cx<21*8) ujp->max++;
   }
 if (key=='-')
   {
    if (cx>=9*8 && cx<13*8 && ujp->min>0) ujp->min--;
    if (cx>=17*8 && cx<21*8 && ujp->max>0) ujp->max--;
   }
 if (key=='x')
   {
    if (cx>=9*8 && cx<13*8) ujp->min+=10;
    if (cx>=17*8 && cx<21*8) ujp->max+=10;
   }
 if (key=='z')
   {
    if (cx>=9*8 && cx<13*8 && ujp->min>=10) ujp->min-=10;
    if (cx>=17*8 && cx<21*8 && ujp->max>=10) ujp->max-=10;
   }

 if ((key>='0' && key<='9') || key=='.')
  {
//   ujp->pos&=~63;

   switch(key)
   {
   case '0': ujp->pos^=16; break;
   case '.': ujp->pos^=32; break;
   case '4': ujp->pos^=1; break;
   case '6': ujp->pos^=2; break;
   case '2': ujp->pos^=8; break;
   case '8': ujp->pos^=4; break;

   case '7': ujp->pos^=1+4; break;
   case '9': ujp->pos^=2+4; break;
   case '1': ujp->pos^=1+8; break;
   case '3': ujp->pos^=2+8; break;
   }

  }

}



void DrawOptions()
{
asm cli;
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

     unsigned int parm; parm=((frametype *)frameptr)->parm;
     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
asm sti;

//Write options
int x=10;
int y=10;

DrawFixedString("ENEMY ON GROUND:",screen+y*320+x); x=160;
switch(parm&3)
{
 case 0: DrawFixedString("Low  pain",screen+y*320+x); break;
 case 1: DrawFixedString("High pain",screen+y*320+x); break;
 case 2: DrawFixedString("Trip     ",screen+y*320+x); break;
 case 3: DrawFixedString("Fall down",screen+y*320+x); break;
} y+=8; x=10;
y+=8;


DrawFixedString("ENEMY IN AIR:   ",screen+y*320+x); x=160;
if (parm&4)  DrawFixedString("Falldown",screen+y*320+x);
  else DrawFixedString("Recover  ",screen+y*320+x);


y+=8; x=10;


switch(parm&(1024+2048))
{
 case 0: DrawFixedString("No bounce off",screen+y*320+x); break;
 case 1024: DrawFixedString("Bounce off hit",screen+y*320+x); break;
 case 2048: DrawFixedString("Bounce off block",screen+y*320+x); break;
 case (1024+2048): DrawFixedString("Bounce off h/b",screen+y*320+x); break;
} y+=8; x=10;


DrawFixedString("BLOCKABLE WITH... ",screen+y*320+x); y+=8; x=30;
if (parm&8) DrawFixedString("[X] Low block",screen+y*320+x); else
     DrawFixedString("[ ] Low block",screen+y*320+x);
y+=8;
if (parm&16) DrawFixedString("[X] High block",screen+y*320+x); else
     DrawFixedString("[ ] High block",screen+y*320+x);
y+=8; x=10;

if (parm&4096) DrawFixedString("Throwee is in front",screen+y*320+x); else
     DrawFixedString("Throwee is behind",screen+y*320+x);


y+=8;

DrawFixedString("ATTACKS ENEMY ONLY WHEN IN...",screen+y*320+x); y+=8; x=30;
if (parm&32) DrawFixedString("[X] Crouch",screen+y*320+x); else
     DrawFixedString("[ ] Crouch",screen+y*320+x);
y+=8;
if (parm&64) DrawFixedString("[X] Stance",screen+y*320+x); else
     DrawFixedString("[ ] Stance",screen+y*320+x);
y+=8;
if (parm&128) DrawFixedString("[X] Air",screen+y*320+x); else
     DrawFixedString("[ ] Air",screen+y*320+x);
y+=8;
if (parm&16384) DrawFixedString("[X] DONE ATTACKING",screen+y*320+x); else
     DrawFixedString("[ ] DONE ATTACKING",screen+y*320+x);

y+=8;


char temp[80];
sprintf(temp,"ENERGY: %d",(unsigned int)((frametype *)frameptr)->energy);
DrawFixedString(temp,screen+y*320+x);

y+=16;
DrawFixedString("SOUND",screen+y*320+x); y+=8;
if (!(((frametype *)frameptr)->se &7))
   sprintf(temp,"IMMEDIATE:  NONE");
 else
 if (((frametype *)frameptr)->se &8)
   sprintf(temp,"IMMEDIATE:  SPECIFIC %d",(unsigned int)((frametype *)frameptr)->se&7);
  else  sprintf(temp,"IMMEDIATE:  STANDARD %d",(unsigned int)((frametype *)frameptr)->se&7);
DrawFixedString(temp,screen+y*320+x);

y+=8;
if (!(((frametype *)frameptr)->se & (7<<4) ))
   sprintf(temp,"HIT:        NONE");
 else
 if (((frametype *)frameptr)->se &128)
   sprintf(temp,"HIT:        SPECIFIC %d",(unsigned int) (((frametype *)frameptr)->se&(7<<4))>>4 );
  else  sprintf(temp,"HIT:        STANDARD %d",(unsigned int)(((frametype *)frameptr)->se&(7<<4))>>4 );
DrawFixedString(temp,screen+y*320+x);


y+=8;
if (parm&256)  //Effect on?
  DrawFixedString("Effect ON",screen+y*320+x);
 else  DrawFixedString("Effect OFF",screen+y*320+x);

if (parm &256)
{
  y+=8;
  if (parm&512)  //Effect occurs when...
    DrawFixedString("Effect occurs at specified point",screen+y*320+x); //1-parm 4-point
   else  DrawFixedString("Effect occurs at hit mark",screen+y*320+x); //1-parm


  //Position at effect parm
  unsigned char *t;
   if (dothrow==2) t=tframeptr+sizeof(frametype);
	      else t=frameptr+sizeof(frametype);

  while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
  t+=1+(*t *sizeof(rect));  //Skip vuln
  t+=1+(*t *sizeof(rect));  //Skip attack

  y+=8;
  sprintf(temp,"Percentage: %3d ",( (*t)&15)*100/15 );
  switch((*t)>>4)
   {
    case 0: strcat(temp,"Ground shake"); break;
    case 1: strcat(temp,"Dizzy       ");break;
    case 2: strcat(temp,"Blood Explosion");break;
    case 3: strcat(temp,"Flood"); break;
    case 4: strcat(temp,"Oil Well");break;
    case 5: strcat(temp,"Spew");break;
    case 6: strcat(temp,"Projectile");break;
    case 7: strcat(temp,"Head Fly");break;
    case 8: strcat(temp,"Upward Blood");break;
    case 9: strcat(temp,"Teleport");break;
   }
  DrawFixedString(temp,screen+y*320+x);
}





if (b&1) //&& isattack)
 {
      if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

  if (cy>=10+0*8 && cy<=10+1*8)
      {
       int a=parm&3;
       parm&=~3;
       a++; a&=3;
       parm|=a;
      }
  if (cy>=10+2*8 && cy<10+3*8)   parm^=4;

    if (cy>=10+3*8 && cy<=10+4*8)
      {
       int a=parm&(1024+2048);
       parm&=~(1024+2048);
       a+=1024; a&=(1024+2048);
       parm|=a;
      }


  if (cy>=10+5*8 && cy<10+6*8)   parm^=8;
  if (cy>=10+6*8 && cy<10+7*8)
    {
      parm^=16;
      if (currmove>=93 && currmove<=96 && dothrow!=2)
      {
      if (parm&16)   //We want to add a frame
	{
	 i=currmove-93;
	 for (j=0; j<32; j++)
	 if (throwptr[i][j])
	 {
	  char *a=&tframe[throwptr[i][j]];
	  for (int l=0; l<currframe; l++)
	   if ( ((frametype *)frameindex[l])->parm&16 )
	      a+=((frametype *)a)->size;

	    InsertBytes(a,sizeof(frametype)+3,tframe+throwptr[i][j]);
	    //Set up new frame
	    ((frametype *)a)->size =sizeof(frametype)+3; //Set size
	    ((frametype *)a)->shadow=0; //Set shadow
	    ((frametype *)a)->dur  =((frametype *)frameptr)->dur; //Set Delay
	    ((frametype *)a)->parm   =0; //Set
	    ((frametype *)a)->se     =0; //Set
	    ((frametype *)a)->energy   =0; //Set
	    *((int *)(&((frametype *)a)->dx))   =0; //Set x in relation  to thrower
	    *((int *)(&((frametype *)a)->tx))   =0; //Set y in relation  to thrower

	    *(a+sizeof(frametype)+0)=0xFF; //Set image term
	    *(a+sizeof(frametype)+1)=0x00; //Set vulnerable number
	    *(a+sizeof(frametype)+2)=0x00; //Set attack number
	 }
	}

      if (!(parm&16))   //We want to delete a frame
	{
	 i=currmove-93;
	 for (j=0; j<32; j++)
	 if (throwptr[i][j])
	 {
	  char *a=&tframe[throwptr[i][j]];
	  for (int l=0; l<currframe; l++)
	   if ( ((frametype *)frameindex[l])->parm&16 )
	      a+=((frametype *)a)->size;
	  DeleteBytes(a,((frametype *)a)->size,tframe+throwptr[i][j]);

	 }
	}

      //CreateFrameIndex();
//      frameptr=tframeindex[currframe];
      }

    }

  if (cy>=10+7*8 && cy<10+8*8)   parm^=4096;

  if (cy>=10+9*8 && cy<10+10*8)   parm^=32;
  if (cy>=10+10*8 && cy<10+11*8)   parm^=64;
  if (cy>=10+11*8 && cy<10+12*8)   parm^=128;

  if (cy>=10+12*8 && cy<10+13*8)   parm^=16384;

  if (cy>=10+16*8 && cy<10+17*8)
    {
     int a;

     if (cx>114 && cx<114+8*8) ((frametype *)frameptr)->se^=8;
	else
     {a=((frametype *)frameptr)->se&7; a++; a&=7;
      ((frametype *)frameptr)->se&=~7; ((frametype *)frameptr)->se|=a;}
    }

  if (cy>=10+17*8 && cy<10+18*8)
    {
     int a;
     if (cx>114 && cx<114+8*8) ((frametype *)frameptr)->se^=8<<4;
	 else
     {a=((frametype *)frameptr)->se&(7<<4); a+=1<<4; a&=(7<<4);
      ((frametype *)frameptr)->se&=~(7<<4); ((frametype *)frameptr)->se|=a;}
    }

  //Position at effect parm
  unsigned char *t;
 t=frameptr+sizeof(frametype);
  while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
  t+=1+(*t *sizeof(rect));  //Skip vuln
  t+=1+(*t *sizeof(rect));  //Skip attack


   //Effect on/off
  if (cy>=10+18*8 && cy<10+19*8)
    {
     parm^=256; parm&=~512;
     if (parm&256) //If need to be turned on
      {
       if (dothrow!=2) InsertBytes(t,5,frame+moves[currmove].firstframe);
	 else          InsertBytes(t,5,tframe+throwptr[currmove-93][throwee]);
       ((frametype *)(frameptr))->size+=5;
       *t=15;             //Zero % & type
       *((int *)(t+1))=0;  //Zero point
       *((int *)(t+3))=0;
      } else
      {
       if (dothrow!=2) DeleteBytes(t,5,frame+moves[currmove].firstframe);
	 else          DeleteBytes(t,5,tframe+throwptr[currmove-93][throwee]);
       ((frametype *)(frameptr))->size-=5;
      }
     CreateFrameIndex();

     frameptr=frameindex[currframe];
     tframeptr=tframeindex[currframe];

     if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}
    }


  if (parm&256)
   {
    if (cy>=10+19*8 && cy<10+20*8) parm^=512;  //Occurance

    if (cy>=10+20*8 && cy<10+21*8)
     {
      int a=(*t)>>4;
      if (++a>15) a=0;

      *t&=15;
      *t|=a<<4;
     }
  }

       if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}


  while (ReadMouse(&cx,&cy));

 }

if (dothrow!=2) ((frametype *)frameptr)->parm=parm;
 else ((frametype *)tframeptr)->parm=parm;

//  if (dothrow==2) {tmp=frameptr; frameptr=tframeptr; tframeptr=tmp;}

}

int base=0;

#define HM 22

int RequestMove()
{

int i,maxmoves;


//char far *dest=(char far *)MK_FP(0xA000,10+320*10);

char far *dest;
char far *screen2=(char far *)farmalloc(64000);

DrawBar(GetPtr(background),27,0,180, 320,20);


do
{
dest=screen+10;

//Draw move words on screen
MemoryCopy(screen,GetPtr(background),64000);

i=base;
while (movenames[i] && i<base+HM*2)
{
 DrawFixedString(movenames[i],dest);
 dest+=8*320;  i++;
 if (i==base+HM) dest=dest-(HM*8*320)+160;
}
maxmoves=i;

i=-1;

//getch();

do
{

MemoryCopy(screen2,screen,64000);
b=ReadMouse(&cx,&cy);
DrawImage2(cursor,screen2,cx,cy,0);


//if (b&1)
// {
  if (cy>HM*8) i=-1; //To high/low
   else
     if (cx<170) i=base+(cy)/8; else i=base+HM+(cy)/8;
  if (i>=maxmoves) i=-1;
// }

if (i!=-1)
 {
  char temp[20];
  sprintf(temp,"BASE: %4X",moves[i].firstframe);
  DrawFixedString(temp,screen2+190*320+10);
 }
if ( ! (b&1)) i=-1;

MemoryCopy((char far *)MK_FP(0xA000,0),screen2,64000);

} while (i==-1 && !(b&2) );

if (b&2) {base+=HM*2; if (base>maxmoves) base=0;
//             if (base
	    while (ReadMouse(&cx,&cy));
	     }


}while(b&2);

while (ReadMouse(&cx,&cy));

farfree(screen2);

MemoryCopy(screen,GetPtr(background),64000);

b=0;
return(i);

}



void ReadImageFiles()  //0-img 1-shadow
{
int h,h2,bh;
unsigned bytes;


ffblk   f;
int done;
int size;

int   i,j,k;

char path[20];

//GET OUR FILE LIST
if (_dos_open("file.lst",O_BINARY | O_RDWR,&h) )
  { //Does not exist
   _dos_creat("file.lst",0,&h);
   numfiles=0; filelist=(imagefile *)malloc(sizeof(imagefile));
   done=findfirst("*.raw",&f,0);
   while (!done)
   {
    for (i=0; i<8; i++) filelist[numfiles].filename[i]=0; //Null it!
    for (i=0; f.ff_name[i]!='.'; i++) filelist[numfiles].filename[i]=f.ff_name[i];

    numfiles++; filelist=(imagefile *)realloc(filelist,(numfiles+1)*sizeof(imagefile));
    done=findnext(&f);
   }
  }
else //It does exist
  {
   _dos_read(h,&size,2,&bytes); //get size
   numfiles=size; filelist=(imagefile *)malloc((numfiles+1)*sizeof(imagefile));
   _dos_read(h,filelist,numfiles*sizeof(imagefile),&bytes); //Get image files
  }


//READ OUR FILES
for (i=0; i<256; i++)
   {
    imagelist[i].pages=NULL;
    imagelist[i].ptr=NULL;
   }
done=findfirst("*.raw",&f,0);
while (!done)
{ //Search list for file
 for (i=0; i<13; i++) path[i]=0;
 for (i=0; f.ff_name[i]!='.'; i++) path[i]=f.ff_name[i];
// strcat(path,".BBM");

// printf("Searching list for file %s...",path);

 for (i=0; i<numfiles && memicmp(path,filelist[i].filename,8); i++);

// memcpy(path,f.ff_name,13); path[13]=0;

 if (i>=numfiles) //Add it to our list
   {
    printf("\nAdding new file %s to list...\n",path);
//    for (i=0; i<8; i++) filelist[numfiles].filename[i]=0; //Null it!
//    for (i=0; f.ff_name[i]!='.'; i++) filelist[numfiles].filename[i]=f.ff_name[i];


    memcpy(filelist[numfiles].filename,path,8);   i=numfiles;
    numfiles++; filelist=(imagefile *)realloc(filelist,(numfiles+1)*sizeof(imagefile));
   }

 //Lets read it!
 strcat(path,".RAW");
 printf("Reading file %s...\r",path);

// ReadLBMFile256(&imagelist[i],0,path);  //Read it!!!!!!!!!
 _dos_open(path,O_BINARY | O_RDONLY,&bh);
 t=(char far *)farmalloc(filelength(bh));
 _dos_read(bh,t,64000,&bytes);
 _dos_close(bh);
 AllocEMSMem(bytes,&imagelist[i]);
 MemoryCopy(GetPtr(imagelist[i]),t,bytes);
 farfree(t);



 done=findnext(&f);
}


lseek(h,0,SEEK_SET);
_dos_write(h,&numfiles,2,&bytes); //Write size
_dos_write(h,filelist,numfiles*sizeof(imagefile),&bytes); //Write file list
_dos_write(h,0,0,&bytes); //Truncation
_dos_close(h);

numfiles--;

printf("\n");

}



void ReadImageFiles2(char *dir)
{
int h,h2,bh;
unsigned bytes;



ffblk   f;
int done;
int size;

int   i,j,k;

char path[20];

chdir(dir);


//GET OUR FILE LIST
if (_dos_open("file.lst",O_BINARY | O_RDWR,&h) )
  { //Does not exist
   exit(1);
  }
else //It does exist
  {
   _dos_read(h,&size,2,&bytes); //get size
   tnumfiles=size; tfilelist=(imagefile *)malloc((tnumfiles+1)*sizeof(imagefile));
   _dos_read(h,tfilelist,tnumfiles*sizeof(imagefile),&bytes); //Get image files
  }


//READ OUR FILES
for (i=0; i<256; i++)
   {
    timagelist[i].pages=NULL;
    timagelist[i].ptr=NULL;
   }

done=findfirst("*.raw",&f,0);
while (!done)
{ //Search list for file
 for (i=0; i<13; i++) path[i]=0;
 for (i=0; f.ff_name[i]!='.'; i++) path[i]=f.ff_name[i];
 for (i=0; i<tnumfiles && memicmp(path,tfilelist[i].filename,8); i++);

 //Lets read it!
 strcat(path,".RAW");

 _dos_open(path,O_BINARY | O_RDONLY,&bh);
 t=(char far *)farmalloc(filelength(bh));
 _dos_read(bh,t,64000,&bytes);
 _dos_close(bh);

 AllocEMSMem(bytes,&timagelist[i]);
 MemoryCopy(GetPtr(timagelist[i]),t,bytes);
 farfree(t);

 done=findnext(&f);
}

tnumfiles--;



}


void exitprog(int x, char * msg)
{
printf(msg);
exit(x);
}





void PutImageFlip256(char far *src, char far *dest,int lsw, int orient)
{
asm{
push	ds
push	es
push	si
push	di
lds	si,src
les	di,dest
mov     bx,lsw
mov 	dx,orient
}
putimageflip256();	/*Call putimage */

asm{
pop	di
pop	si
pop	es
pop	ds
}
}


void DrawBar(char far *b, char color, int x,int y,int xw,int yw)
{
char far *dptr;

if (x+xw>=320) {xw=320-x; if (x>=320) return;}
if (y+yw>=200) {yw=200-y; if (y>=200) return;}

if (x<0)  { xw+=x; x=0; if (xw<0) return; }
if (y<0)  { yw+=y; y=0; if (yw<0) return; }



dptr=b+x+ (y*320);

asm les di,dptr

for (;yw>0; yw--)
{
asm{
mov cx,xw
mov al,color
rep stosb
add di,320
sub di,xw
   }
}
}

void ClearScreen(char c)
{
asm{
push di
mov ax,0A000h
mov es,ax
xor di,di
mov ah,c
mov al,c
mov cx,32000d
rep stosw
pop di
}

}


void DrawFixedString(char *s,char far *dest)
{
int i=0;

while (s[i]) {DrawFixedChar(s[i],dest); dest+=8; i++;}

}

int ReadMouse(int *x, int *y)
{
int b,xt,yt;

asm{
mov ax,3
int 33h
mov b,bx
mov xt,cx
mov yt,dx
}

*x=xt/2; *y=yt;
return(b);
}


void DrawImage2(char far *src,char far *dest,int x, int y,int o)
{
//if (!src) return;
//return;

int xlen,ylen,xst,yst,clip;

clip=0; //clip flag
xst=yst=0;
xlen=(int) (unsigned char)src[1];
ylen=(int) (unsigned char)src[2];

if (x+xlen>=320) {xlen=320-x; clip=1; if (x>=320) return;}
if (y+ylen>=200) {ylen=200-y; clip=1; if (y>=200) return;}

if (x<0)  { xst=-x; clip=1; if (x+xlen<0) return; }
if (y<0)  { yst=-y; clip=1; if (y+ylen<0) return; }

if (clip==0) PutImageFlip256(src,dest+(320*y)+x,320,o);
 else  PutImageFlipClip256(src,dest+(320*y)+x,320,xst,yst,xlen,ylen,o);
}

/*
     //Insert throwee frame
     if (currmove>=93 && currmove<=96)
      {
       //for (i=0; i<3; i++)
       i=currmove-93;
	for (j=0; j<32; j++)
	if (throwptr[i][j])
	 {
	  char *a=&tframe[throwptr[i][j]];
	  for (int l=0; l<currframe; l++) a+=((frametype *)a)->size;
	  if (key=='I') a+=((frametype *)a)->size;

	    InsertBytes(a,sizeof(frametype)+3,tframe+throwptr[i][j]);
	    //Set up new frame
	    ((frametype *)a)->size =sizeof(frametype)+3; //Set size
	    ((frametype *)a)->shadow=0; //Set shadow
	    ((frametype *)a)->dur  =((frametype *)frameptr)->dur; //Set Delay
	    ((frametype *)a)->parm   =defs[currmove].parm; //Set
	    ((frametype *)a)->se     =0; //Set
	    ((frametype *)a)->energy   =defs[currmove].energy; //Set Dx
	    *((int *)(&((frametype *)a)->dx))   =0; //Set x in relation  to thrower
	    *((int *)(&((frametype *)a)->tx))   =0; //Set y in relation  to thrower

	    *(a+sizeof(frametype)+0)=0xFF; //Set image term
	    *(a+sizeof(frametype)+1)=0x00; //Set vulnerable number
	    *(a+sizeof(frametype)+2)=0x00; //Set attack number
	 }
      }
  */