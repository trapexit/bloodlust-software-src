#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

extern int sbstat;
extern playerstat p[2];
extern short rsel;
extern int donocin;

void CinTickUpdate();
void ClearScreen(char c);
void ReadImageFiles();


typedef struct event
{
unsigned char type;       //Type of event
unsigned char data;       //Data
short  x,y;    //Position info
} event ;

//TYPE
//0-Delay
//1-Fade in/out
//2-Change BG
//3-Initialize sprite
//4-Kill Sprite

typedef struct simg
{
 unsigned char img;
 unsigned char o;
 short int  x,y;
} simg;

typedef struct sframe
{
 unsigned char inum;
 unsigned char dur;
 short int  dx,dy;
 simg images[];
} sframe;


typedef struct sprite
{
 unsigned char stype;
 sframe *sptr;
 short int cf,nf;
 short int  x,y;
 short int  tcx,tcy;
 unsigned char dur;
 unsigned char playloop;
} sprite;


sprite sprites[15];

typedef struct spriteptr
{
unsigned short int base;
short int numframes;
} spriteptr;

spriteptr slist[50];
short int ssize;
unsigned char *sbuffer;

event *ebuffer;
short int eventnum;
int ce;

int   numfiles;
unsigned int imagesizes[250];
char *imagelist[250];


extern char *screen,*background;

extern char *font,*font2,*font3,*font4,*font5;


///-------------
int playback,playloop;
int oce;

int currbg;
int dur;

int fadestat,dofade; //0-off -on
extern int fadelevel,oldfadelevel;


void UpdatePalette();
void EditSprite(int snum);

int u;

void DrawSprite(sprite *s);

//unsigned int vocs[26];
int playvoc;
unsigned vsize[26];
unsigned vpos[26];
SOUND voc[26];


void CinUpdatePalette();


extern color pal1[256],pal2[256];
extern char *screen,*background;

extern volatile char kbscan;
extern volatile int quit,pause,kbstat;

char *words;

int dowords,wordson;
extern cfg *options;


int dochangepal=0;

extern unsigned int tsCD; //cdrom drive with TS cd, 0 if none exists

void PlayCinema(char *cinfile)
{

unsigned int bytes;

int mh;

int i;

if (donocin) return;

if (sbstat!=2) wordson=1;
if (options->cinword) wordson=1;

//  printf("opening file %d\n",cinfile);
if (_dos_open(cinfile,O_BINARY | O_RDWR,&mh)) //see if it is in the root
 {
  if (!tsCD) return;   
  char cdcinfile[30];
  sprintf(cdcinfile,"%c:\\CIN\\%s",tsCD+'A'-1,cinfile);
     
  if (_dos_open(cdcinfile,O_BINARY | O_RDWR, &mh))  return;
 } 


  _dos_read(mh,pal1,256*3,&bytes);

  _dos_read(mh,&eventnum,2,&bytes);
//  printf("eventnum %d...quit?\n",(int)eventnum);
//  kbscan=quit=0; while (!kbscan); if (quit) return;

  ebuffer=(event *)malloc(eventnum*sizeof(event) );
  _dos_read(mh,ebuffer,eventnum*sizeof(event),&bytes);

  _dos_read(mh,&ssize,2,&bytes);
//  printf("ssize %d...quit?\n",(int)ssize);
//  kbscan=quit=0; while (!kbscan); if (quit) return;

  sbuffer=(unsigned char *)malloc(ssize);
  _dos_read(mh,sbuffer,ssize,&bytes);

  _dos_read(mh,slist,50*sizeof(spriteptr),&bytes);


  words=(char *)malloc(25*100);
  _dos_read(mh,words,25*100,&bytes);

  numfiles=0;
  _dos_read(mh,&numfiles,2,&bytes);
//  printf("reading %d image files\n",(int)numfiles);
  //     kbscan=quit=0; while (!kbscan); if (quit) return;
  for (i=0; i<250; i++) imagelist[i]=0;

  for (i=0; i<=numfiles; i++)
        {
         imagelist[i]=0;
         unsigned int size;
         size=0;
         _dos_read(mh,&size,2, &bytes);
         if (size)
                {
                 imagelist[i]=(char *)malloc(size);
                 _dos_read(mh,imagelist[i],size,&bytes);
                }
         imagesizes[i]=size;       
         //     printf("%d %d %X      ",i,size,imagelist[i]);
        }

int readallvocs=1;
if (!stricmp(cinfile,"intro.vol"))  readallvocs=0;


if (sbstat==2)
  for (i=0; i<26; i++)
     {
       _dos_read(mh,&vsize[i],4,&bytes);
       vpos[i]=lseek(mh,0,SEEK_CUR);
       voc[i].soundptr=0;
       voc[i].soundsize=0;

       if (readallvocs)
       {
        voc[i].soundptr=(signed char *)malloc(vsize[i]); //alloc mem for it
        _dos_read(mh,voc[i].soundptr,vsize[i],&bytes);
        voc[i].soundsize=vsize[i];
       } else lseek(mh,vsize[i],SEEK_CUR);
     }

        //    getch();


//kbscan=0; while (!kbscan);
//Mode256();


  ce=0; dur=0; currbg=-1;
fadestat=0; fadelevel=0; oldfadelevel=-1; dofade=0;
//_fmemset((char far *)MK_FP(0xA000,0),0,64000);

for (i=0; i<15; i++) sprites[i].sptr=0;
UpdatePalette();
Solid(background,64000,0);
MemoryCopy((char *)0xA0000,background,64000);


playback=1; kbscan=0;
int leaver=0;
p[0].in.Reset(); p[1].in.Reset();

          dowords=-1;
          dochangepal=0;

timerhandler=CinTickUpdate;
unsigned int oldspeed=speed;
SetTimerSpeed(100);



playvoc=-1;


do
{

        //Playing
  if (currbg!=-1)
  {
        MemoryCopy(screen,imagelist[currbg],320*120);
        for (sprite *s=sprites; s<&sprites[15]; s++)
         if (s->sptr)    DrawSprite(s);

        MemoryCopy((char *)(0xA0000+40*320),screen,320*120);

        if (dowords!=-1)
         {
  memset(screen+160*320,0,39*320);

char *t=words+dowords*100;
char line[40];
unsigned dest=161*320;
do
{
 int i=0;
 while (*t!='/' && *t) {line[i++]=*t; t++;}
 line[i]=0; if (*t) t++;

 DrawStringSP(font5,screen+dest+160-GetStringWidthSP(font5,(unsigned char *)line)/2,(unsigned char *)line,320);
 dest+=12*320;
} while(*t);
         dowords=-1;
MemoryCopy((char *)(0xA0000+160*320),screen+160*320,320*39);


 }

}




if (playvoc!=-1)
  {
    if (!voc[playvoc].soundptr)
     {
    lseek(mh,vpos[playvoc],SEEK_SET);  //position to the voc
    voc[playvoc].soundptr=(signed char *)malloc(vsize[playvoc]); //alloc mem for it
    _dos_read(mh,voc[playvoc].soundptr,vsize[playvoc],&bytes);
    voc[playvoc].soundsize=vsize[playvoc];
     }
     
    PlayModalSoundEffect(&voc[playvoc]);
    playvoc=-1;
  }


if (dochangepal)
 {
  if (imagesizes[currbg]>320*120)
    MemoryCopy(pal1,imagelist[currbg]+320*120,256*3);
  oldfadelevel=-1;

  dochangepal=0;
 }     
  

  UpdatePalette();
RefreshInput();
 leaver=(p[0].in.ReadButt(0)|p[1].in.ReadButt(1))&(~0xF);

} while (kbscan!=1 && playback && !(leaver));
if (!playback) quit=0; else quit=1;

p[0].in.stat=0;

timerhandler=0;
SetTimerSpeed(oldspeed);

if (sbstat==2)  StopVoice();

 
free(words);
if (sbstat==2)
  for (i=0; i<26; i++)
    if (voc[i].soundsize) free(voc[i].soundptr);
for (i=0; i<=numfiles; i++)
  if (imagelist[i])   {free(imagelist[i]); imagelist[i]=0;}
free(sbuffer);
free(ebuffer);



_dos_close(mh);


}



sprite s;


void DrawSprite(sprite *s)
{
simg *i=& (s->sptr->images[0]);

for (int j=s->sptr->inum; j>0; j--)
{
 DrawImage(imagelist[i->img],screen,s->x+i->x,s->y+i->y,i->o);
 i++;
}

}


void CinTickUpdate()
{
u++;

if (playback)
 {
        while (!dur)
         {
          if (ce>=eventnum)
                 {playback=0; ce=oce;dur=1; currbg=-1;
        fadestat=1; dofade=0; kbscan=1; //getch();
        timerhandler=0;
        break;}
                else
          {
                switch(ebuffer[ce].type)
                {
                 case 0: dur=ebuffer[ce].x; break;
                 case 1: dofade=1; dur=33; break;
                 case 2: currbg=ebuffer[ce].data; dochangepal=1;
                 {
                for (sprite *s=sprites; s<&sprites[15]; s++) s->sptr=0;
                MemoryCopy((char *)(0xA0000+160*320),background,320*39);

          //DrawBar((char far *)MK_FP(0xa000,0),0,0,161,320,39);
          }
         break;
                 case 3:
          {
        sprite *s;
        for (s=sprites; s->sptr; s++);
        short int snum=ebuffer[ce].data&0x7F;
        s->stype=snum;
        s->x=ebuffer[ce].x; s->y=ebuffer[ce].y; s->tcx=0; s->tcy=0;
        s->sptr=(sframe  *)&sbuffer[slist[snum].base];
        s->dur=s->sptr->dur; s->cf=0; s->nf=slist[snum].numframes;
        s->playloop=ebuffer[ce].data&0x80;
         }
        break;
                 case 5:
//        if (sbstat==2 && vocs[ebuffer[ce].data].soundptr)
//         {
//  StopVoice();
//          PlayModalSoundEffect( &vocs[ebuffer[ce].data]);
//         }
 if (sbstat==2 && vsize[ebuffer[ce].data])
   playvoc=ebuffer[ce].data;
                 break;

                 case 6:
        if (!wordson) break;

        dowords=ebuffer[ce].data;

                 break;
                }
                ce++;
          }
         }
        dur--;


        sprite *s;
        for (s=sprites; s<&sprites[15]; s++)
         if (s->sptr)
          {
                s->tcx+=s->sptr->dx; s->x+=s->tcx>>8; s->tcx&=0xFF;
                s->tcy+=s->sptr->dy; s->y+=s->tcy>>8; s->tcy&=0xFF;
                if (s->dur) s->dur--;
                 else
                {
                 s->cf++;
                 if (s->cf>=s->nf)
        {
         s->cf=0;
         if (s->playloop) s->sptr=(sframe *)(sbuffer+slist[s->stype].base);
          else s->sptr=0;
        }
                 else s->sptr=(sframe *) (((char *)s->sptr)+sizeof(sframe)+s->sptr->inum*sizeof(simg));

                 if (s->sptr) s->dur=s->sptr->dur;
                }
          }


        if (dofade)
         {
          if (!fadestat) {fadelevel++; if (fadelevel==31) {fadestat=1; dofade=0;}}
                 else    {fadelevel--; if (fadelevel==0 ) {fadestat=0; dofade=0;}}
         }

 }

}



