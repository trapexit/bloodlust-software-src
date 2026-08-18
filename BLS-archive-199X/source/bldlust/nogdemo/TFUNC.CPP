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
#include <direct.h>

#define REVOL

extern "C"
{
#include "detect.h"
}
#include "tgraph.h"
#include "tinput.h"
#include "time.h"

void Nofile(char *s)
{
ModeText();
printf("File not found: %s\n",s);
printf("Try and reinstall Noggin Knockers 2...");
exit(1);

}    


extern int lastsize;
void ReadVolSound(int h,SOUND *s);

extern color pal1[256];


extern char *background;

extern cfg *options;
extern int sbstat;
extern char *font;

extern char *screen;
extern unsigned int bytes;



//extern char *cdlist;
extern char *cnlist[];

extern sz;

extern playerstat p[2];

unsigned int imageX(char *i)
{
 return(         (unsigned int)*((unsigned char *) (i+1)) );
}

unsigned int imageY(char *i)
{
 return(         (unsigned int)*((unsigned char *) (i+2)) );
}

int lastsize;

char *ReadVolFile(int h)
{
unsigned int bytes;
unsigned  size=10;
char *p;
//cprintf("   reading size: ");
  _dos_read(h,&size,4,&bytes);
//printf("%d...done\n",size);
//cprintf("   malloc...");
p=(char *)malloc(size);
//printf("done\n");

//cprintf("   reading...");
  _dos_read(h,p,size,&bytes);
//printf("done\n");  
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






extern volatile unsigned int uu;
extern volatile int pause;
void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p);;
unsigned int imageX(char *i);

void playerstat::DrawPain(int i,int x,int y,int o)
{
if (!pain || !painexist) return;
image *z=&pl[i].pi[0]; //get first image pointer

int icnt=8;
while (z->index!=0xFF && icnt)
 {
  if (((z->orient>>2)&3)==pain) 
   {
    int dy=z->dispy;
    if (o&1) dy=-dy+imageY(imagelist[i])-imageY(painimagelist[z->index]);
    
    DrawImage(painimagelist[z->index],screen,x+z->dispx[(o>>1)&1],y+dy,(o^z->orient));

    if (paindur<uu && energy && !(rand()&3) && !pause)
     {
      paindur=uu+(200)/(8/2);
      GenerateEffect(10,x+z->dispx[(o>>1)],y+dy,(imageX(painimagelist[z->index]) )/2,imageY(painimagelist[z->index])+5, 0,0);
     } //17
   }
  
    
  z++; icnt--;
 }

}


void playerstat::DrawFrame()
{

_disable();
if (!numframes)
  {
        _enable();
        return;
  }

int tx=x;
int ty=y-80;
int td=d;

register image *i=(image *) (frameptr+sizeof(frametype));

//if (gamestat==-1 && fontdur<90)
//  {ty+=200-fontdur*2; s-=(20)*320;  }
_enable();

if (pain>3) pain=3;

while ( i->index!=0xFF )
{
 DrawImage(imagelist[i->index],screen,tx+i->dispx[td],ty+i->dispy,i->orient^(td<<1));
// if (bloodlevel>=2)
  DrawPain(i->index,tx+i->dispx[td],ty+i->dispy,i->orient^(td<<1));
        
 i++;
}


}


void projectilestat::DrawProjectile()
{
if (!pframeptr) return; //If no projectile leave
_disable();
int tx=px;
int ty=py-80;
int td=pd;

register image *i=(image *) (pframeptr+sizeof(frametype));
_enable();

while ( i->index!=0xFF )
{
 DrawImage(p[sz].imagelist[i->index],screen,tx+i->dispx[td],ty+i->dispy,i->orient^(td<<1));
// if (bloodlevel>=2)
  p[sz].DrawPain(i->index,tx+i->dispx[td],ty+i->dispy,i->orient^(td<<1));
 i++;
}


}


int ReadRawFile(char **m,int h)
{
char header[4];
_dos_read(h,header,4,&bytes);
int x=(int)header[1];
int y=(int)header[2];

int size=y*2+filelength(h);

char *t=(char *)malloc(size);
for (int i=0;i<4;i++) t[i]=header[i];
short *ylist=(short *) (t+4);
char *img=t+4+ y*2;

_dos_read(h,img,64000,&bytes);


for (i=0; i<y; i++)
{
ylist[i]=img-t;

int xl=x;
while (xl>0)
 {
  if ( (*img)&0x80) {xl-=(*img)&0x7f; img++;}
         else            {xl-=*img; img+=*img; img++; }
 }
}

*m=t;
return(size);
}




  extern char volatile  kbscan;
extern char volatile  quit;



void playerstat::Initialize(int ptype,int c)
{
//int mh;
//char path[20];

int i;

type=ptype;
color=c;


int vh;
char volfile[20];
strcpy(volfile,cnlist[type]);
strcat(volfile,".vol");
if (_dos_open(volfile,O_BINARY | O_RDWR,&vh)) Nofile(volfile);


char key[20];
_dos_read(vh,key,12,&bytes);
if (stricmp(key,"MIDGETPOWER")) Nofile(volfile);

// Get name voc
//unsigned  size;
//_dos_read(vh,&size,2,&bytes);
//namevoc=(VOC *)farmalloc(size);
//_dos_read(vh,namevoc,size,&bytes);
//ReadVolSound(vh,&namevoc);;
//namevoc=(VOC *)realvolread(vh);
//printf("readvoc");

for (i=0; i<5; i++)
{
int size;
_dos_read(vh,&size,4,&bytes);
lseek(vh,size,SEEK_CUR);
}

//Skip Bio stuff
for (i=1; i<=6; i++)
 {
  unsigned  size;
  _dos_read(vh,&size,4,&bytes);
  lseek(vh,size,SEEK_CUR);
 }

_dos_read(vh,charname,15,&bytes);

//printf("1 (%ld) ",baseptr);

//char *map=NULL;
//map=(char *)malloc(256);
//_dos_read(vh,map,256,&bytes);
//free(map);
lseek(vh,256,SEEK_CUR);

//printf("skip map\n");

unsigned int imgindex[256];
unsigned int imgsizes[256];
unsigned int imgsize;

_dos_read(vh,imgindex,256*4,&bytes);
_dos_read(vh,imgsizes,256*4,&bytes);
_dos_read(vh,&imgsize,4,&bytes); //printf("%ld\n",imgsize);

images=(char *)malloc(imgsize);
_dos_read(vh,images,imgsize,&bytes);
for (i=0; i<256; i++)      //Set up image list
  if (imgsizes[i])
        {
         imagelist[i]=images+imgindex[i];
        } else imagelist[i]=0;


//printf("2");
//printf("read most of shit\n");

SOUND *sptr=(SOUND *)se;
for (i=0; i<14; i++)
 {
  ReadVolSound(vh,sptr); 
//  printf("%d:%d ",i,sptr->soundsize);
  sptr++;
 }
//printf("\n");
 

// printf("3");

//Read move stuff
unsigned int framesize=0,tframesize=0;
//printf("reading moves...\n");
_dos_read(vh,moves,sizeof(move)*130,&bytes);
_dos_read(vh,&framesize,2,&bytes); frame=(unsigned char *)malloc(framesize+25); //printf("framesize %d\n",framesize);
//printf("framesize=%d\n",(int)framesize);

_dos_read(vh,frame,framesize,&bytes);
_dos_read(vh,&numjm,2,&bytes);
//printf("numjm=%d\n",(int)numjm);

//for (i=0; i<numjm; i++)
//         _dos_read(vh,&(sm[i].jp),sizeof(joypos)*10,&bytes);  //Read joypos list
  lseek(vh,sizeof(joypos)*10*numjm,SEEK_CUR);
//_dos_read(vh,throwptr,32*4*2,&bytes);
  lseek(vh,32*4*2,SEEK_CUR);
_dos_read(vh,&tframesize,2,&bytes); //tframe=(char *)malloc(tframesize+25); //printf("tframe size %d\n",tframesize);
//printf("tframesize=%d\n",(int)tframesize);


//_dos_read(vh,tframe,tframesize,&bytes);
  lseek(vh,tframesize,SEEK_CUR);
painexist=0; 

//pain
{
unsigned size;
_dos_read(vh,&size,4,&bytes);
painexist=0; 
if (bytes) //pain exists
 {
  painexist=1;
  pl=(painimage *)malloc(size);
  _dos_read(vh,pl,size,&bytes);

  _dos_read(vh,imgindex,256*4,&bytes);
  _dos_read(vh,imgsizes,256*4,&bytes);
  _dos_read(vh,&imgsize,4,&bytes); //printf("%ld\n",imgsize);

  painimages=(char *)malloc(imgsize);
  _dos_read(vh,painimages,imgsize,&bytes);
  for (i=0; i<256; i++)      //Set up image list
   if (imgsizes[i])
         painimagelist[i]=painimages+imgindex[i];
//         if (color) ColorMapImage(painimagelist[i],(unsigned char *)map);
         else
        painimagelist[i]=0;
 }
 
}


//free(map);


_dos_close(vh);

//printf("done\n");
//kbscan=0; while (!kbscan);


//chdir(path);

//Set up frame pointers
Reset();
}






void playerstat::Kill()
{
 int i;
//  free(namevoc.soundptr);
//  free(name);
  free(images);
  for (i=0; i<14; i++)
         if ( ((SOUND *)se)[i].soundptr) free( ((SOUND *)se)[i].soundptr);
  free(frame); //free(tframe);

  if (painexist)
   {
    free(painimages); free(pl); painexist=0;
   }
}


extern int awe32;

void CreateFloorBG(char *bg,int bgy,int bgynum,unsigned short int *ylist);
extern "C" unsigned char SHADOWMAP[];

void bground::ReadBackground(char *dir,int fly)
{
int i;
int vh;

unsigned short size;

//ModeText();
//printf("opening %s..\n",dir);

if (_dos_open(dir,O_BINARY | O_RDONLY,&vh))
 Nofile(dir);

//Palette
_dos_read(vh,pal1,256*3,&bytes);
/*
for (i=0; i<256; i++) //do shadow map
 {
  unsigned char tr,tg,tb;
  tr=((unsigned)pal1[i].r)*3/10;  tg=((unsigned)pal1[i].g)*3/10;  tb=((unsigned)pal1[i].b)*3/10;   

  unsigned int closeness=9999,closecolor=0,k;
  for (int j=0; j<256; j++)
    {
      k=abs(tr-pal1[j].r)+abs(tg-pal1[j].g)+abs(tb-pal1[j].b);
      if (k<closeness) {closeness=k; closecolor=j;}
    }
  SHADOWMAP[i]=closecolor;
 }
*/
//Images
unsigned long imgindex[80];
unsigned short imgsizes[80];
unsigned long imgsize;
_dos_read(vh,imgindex,80*4,&bytes);
_dos_read(vh,imgsizes,80*2,&bytes);
_dos_read(vh,&imgsize,4,&bytes);

//printf("imgsize=%d\n",imgsize);

bgimages=(char *)malloc(imgsize);
//printf("bgimages=%X\n",bgimages);
_dos_read(vh,bgimages,imgsize,&bytes);
//printf("read=%d\n",bytes);

for (i=0; i<80; i++)      //Set up image list
        bgimagelist[i]=bgimages+imgindex[i];

//Get music
_dos_read(vh,&size,2,&bytes);
//cmf=(char *)farmalloc(size);
//_dos_read(vh,cmf,size,&bytes);

//printf("midisize=%d\n",(int)size);
//
if (musicstat)
 {
  int oldpos=lseek(vh,0,SEEK_CUR);
  load_music(vh,&music);
  lseek(vh,oldpos+size,SEEK_SET);
 }
 else lseek(vh,size,SEEK_CUR);

//Read bg.lst
_dos_read(vh,&nums,2,&bytes);  //Read number of series
//printf("nums=%d\n",(int)nums);
_dos_read(vh,&bgs,sizeof(series)*(nums+1),&bytes);  //Read series list
_dos_read(vh,&bgframesize,2,&bytes);
bgframes=(unsigned char *)malloc(bgframesize);
_dos_read(vh,bgframes,bgframesize,&bytes);      //Read frame list

for (i=0; i<nums+1; i++)
 {
  bgs[i].currframe=0;
  bgs[i].frameptr=(unsigned char *)(bgs[i].firstframe+bgframes);
  bgs[i].dur=((bgframe *)bgs[i].frameptr)->dur;
 }
numfs=0;
for (i=0; i<=nums; i++)
 if (bgs[i].parm && bgs[i].numframes) {bgfs[numfs++]=bgs[i]; bgs[i].numframes=0;}
numfs--;


//Read bg
         //Read ylists
ylist1a=(short *)malloc(100*2);
_dos_read(vh,ylist1a,100*2,&bytes);
ylist1b=(short *)malloc(100*2);
_dos_read(vh,ylist1b,100*2,&bytes);
_dos_read(vh,&bg2y,2,&bytes);
ylist2=(short *)malloc(bg2y*2);
_dos_read(vh,ylist2,bg2y*2,&bytes);


        //Read Backgrounds
unsigned short sbg1a,sbg1b,sbg2,sbg3;

_dos_read(vh,&sbg1a,2,&bytes); //printf("sbg1a: %hu\n",sbg1a);
bg1a=(char *)malloc(sbg1a);
_dos_read(vh,bg1a,sbg1a,&bytes);

_dos_read(vh,&sbg1b,2,&bytes); //("sbg1b: %hu\n",sbg1b);
bg1b=(char *)malloc(sbg1b);
_dos_read(vh,bg1b,sbg1b,&bytes);

_dos_read(vh,&sbg2,2,&bytes);  //("sbg2: %hu\n",sbg2);
bg2=(char *)malloc(sbg2);
_dos_read(vh,bg2,sbg2,&bytes);

_dos_read(vh,&sbg3,2,&bytes);  //("sbg3: %hu\n",sbg3);
bg3=(char *)malloc(sbg3);
_dos_read(vh,bg3,sbg3,&bytes);



if (musicstat && awe32)
 {
  if (filelength(vh)>lseek(vh,0,SEEK_CUR)) //if there is something at the end....
   {
     free_music(&music);
     _lseek(vh,2,SEEK_CUR);    
     load_music(vh,&music);
   }  
 }


_dos_close(vh);

int bg1by=((short int *)bg1b)[1];

//CREate floor stuff


if (fly<bg2y) fly=bg2y; //can't be less than transparent stuff;
if (fly==0) fly=bg2y; //can't be less than transparent stuff;

//ModeText();
//printf("%s  bg2y=%d bgynum=%d\n",dir, bg2y,200-bg2y);


floory=fly;           //set floor position
floorheight=bg1by+100-fly;  //vertical height of floor

if (floorheight>0) //we make floor now
{
 if (floory>bg2y)
   ((short int *)bg1b)[1]=floory-100;  //truncate bg1b
 CreateFloorBG(bg1b,floory-100,floorheight,(unsigned short int *)ylist1b);
  
}  else { bgfloor=bgfloorsave=0; bgfloorsize=0; floorheight=0;} 

//kbscan=0; while(!kbscan);
//exit(1);
//Mode256();
}

//stretch s[640] to d[640+l] 
void StretchBGLine(char *s, char *d, int l)
{
d+=(720-640)/2; //goto beggining of d
d-=l;

int length=640+l*2;
int dx=(640<<16)/length;
int countx=0;

for (int i=0; i<length; i++)
 {
  *d=*s;
  d++;
  countx+=dx;
  s+=countx>>16;
  countx&=0xFFFF;
 }
}    

void bground::CreateFloorBG(char *b,int bgy,int bgynum,unsigned short int *ylist)
{
unsigned int bgsize=bgfloorsize=bgynum*720;
bgfloor=(char *)malloc(bgsize+720*20); //5000; //buffer for floor
bgfloorsave=(char *)malloc(bgsize);     //buffer for floor w/o blood

//ModeText();
//printf("bgsize=%d bgy=%d bgynum=%d floory=%d bg2y=%d\n",bgsize,bgy,bgynum,floory,bg2y);
//printf("bg1by=%d",

char *fl=bgfloor; //write floor here
unsigned char a;

int x;
char *s;
for (int i=0; i<bgynum; i++,bgy++) //go through all lines
 {
  s=b+ylist[bgy]; //get readpos
  char line[640]; //make temp line to be stretched
  char *d=line;
  
//  printf("y=%3d s=%4X\n",i,s);
  //FILL LINE WITH BG
  for (x=640; x>0; ) //go through all width
   {
    a=*s; s++;
    if  (a&0x80) //transparent
         { a&=0x7F; d+=a; x-=a; }
    else         //opaque
         {
          x-=a;
          if (x<0) {memcpy(d,s,x+a); break;}
          memcpy(d,s,a);
          d+=a; s+=a; 
         }
   }

  //StretchBGLine(line,fl,(320+160-320)*i/150 );
  MemoryCopy(fl,line,640); //just copy it
  fl+=720;
     
}

MemoryCopy(bgfloorsave,bgfloor,bgfloorsize); //copy no blood version
//Mode256();
return; 
}

//fill dest pointer with 320 bytes of  720 bytes of bg
void drawbgline(char *s,char *d,int x)
{
    MemoryCopy(d,s+x,320);
/* s+=(720-640)/2;    
 if (x<0)
  {
//   MemoryCopy(d,s+640+x,-x);
   MemoryCopy(d,s+x,320);
   return;
  }
 if (x<320)
  {
   MemoryCopy(d,s+x,320);
   return;
  }
 if (x>=320)
  {
   MemoryCopy(d,s+x,320);
//   MemoryCopy(d+640-x,s,x-320);
  }
  */  
}    

void bground::DrawFloorBG(char *d,int bx)
{
char *s=bgfloor;    
d+=floory*320; //goto location
for (int by=0; by<floorheight; by++,d+=320,s+=720)
 drawbgline(s,d,bx);// +(bx+160-320)*by/150 );
}        


/*
void DrawRawBG(char *s, char *d,int bx,int by,int dx, int dy,int bgynum)
{
d+=dx+dy*320; //goto location
for ( ; by<bgynum; by++,d+=320)
 
 MemoryCopy(d,s+by*640+bx ,320);

}        
*/

void bground::DrawBackground(char *d)
{
PutBackgroundTriOverlap(bg1a,bg2,bg3,d, scrx,10, (scrx/2)%320,10/2, (scrx/4)%320,10/4,        0,10, ylist1a, ylist2);
PutBackgroundTriOverlap(bg1b,bg2,bg3,d, scrx, 0, (scrx/2)%320,10/2+90, (scrx/4)%320,10/4+90,  0,100, ylist1b, ylist2);

//  PutBackgroundTriOverlap(bg1a,bg2,bg3,d, scrx,scry, (scrx/2)%320,scry/2, (scrx/4)%320,scry/4,        0,0, ylist1a, ylist2);
//  PutBackgroundTriOverlap(bg1b,bg2,bg3,d, scrx,0, (scrx/2)%320,scry/2+100, (scrx/4)%320,scry/4+100,  0,100-scry, ylist1b, ylist2);
if (floorheight)
{
 if (floory>bg2y)
  PutBackground(bg1b,d,scrx,bg2y-100,0,bg2y,ylist1b);
    
 DrawFloorBG(d,scrx);
} 
}

void bground::DrawBGImages(char *d)
{
 int  i=0;
 for (series *bp=bgs; i<nums; i++,bp++)
  {
        bgframe *f=(bgframe *)bp->frameptr;
        if (f->index!=0xFF && bp->numframes)
                DrawImage(bgimagelist[f->index],d,f->x-scrx,f->y,0);
  } 
}

void bground::DrawBGSeries(char *d,int i)
{
 series *bp=&bgs[i];
 if (!bp->numframes) return;

 bgframe *f=(bgframe *)bp->frameptr;
 if (f->index!=0xFF)
       DrawImage(bgimagelist[f->index],d,f->x-scrx,f->y,0);
}


void bground::DrawFGImages(char *d)
{
 int  i=0;
 for (series *bp=bgfs; i<=numfs; i++,bp++)
  {
        bgframe *f=(bgframe *)bp->frameptr;
if (f->index!=0xFF && bp->numframes)
  switch(bp->parm)
  {
        case 3: DrawImage(bgimagelist[f->index],d,f->x-scrx-(scrx+160-320)*100/150,f->y,0); break;
        case 1: DrawImage(bgimagelist[f->index],d,f->x-scrx -(scrx-160)/4 , f->y,0); break;
        case 2: DrawImage(bgimagelist[f->index],d,f->x-scrx -(scrx-160)/32,f->y,0); break;
  }

  }
}

void bground::KillBackground()
{
free(bgimages);
if (musicstat) free_music(&music);
free(bgframes);
 free(bg1a); free(bg1b);
 free(bg2);  free(bg3);
 free(ylist1a); free(ylist1b); free(ylist2);
 if (bgfloor) {free(bgfloor); free(bgfloorsave);}
 bgfloor=bgfloorsave=0; bgfloorsize=0;
}


char *eimages;
char *eimagelist[256];
unsigned char *eframe;       //Array of frames
move elist[130];             //Array of moves

void KillEffects()
{
free(eimages);
free(eframe);
}

void InitializeEffects()
{
int vh;
int i;
if (_dos_open("misc.vol",O_BINARY | O_RDWR,&vh))
 Nofile("misc.vol");

// Get name voc
unsigned short size;
for (i=0; i<5; i++)
{
_dos_read(vh,&size,2,&bytes);
lseek(vh,size,SEEK_CUR);
}

//Skip Bio stuff
for (i=1; i<=6; i++)
 {
  _dos_read(vh,&size,2,&bytes);
  lseek(vh,size,SEEK_CUR);
 }

//_dos_read(vh,charname,15,&bytes);
lseek(vh,15+256,SEEK_CUR);

unsigned long imgindex[256];
unsigned short imgsizes[256];
unsigned long imgsize;
_dos_read(vh,imgindex,256*4,&bytes);
_dos_read(vh,imgsizes,256*2,&bytes);
_dos_read(vh,&imgsize,4,&bytes); //printf("%ld\n",imgsize);

//printf("imgsize %d\n",imgsize);

eimages=(char *)malloc(imgsize);
_dos_read(vh,eimages,imgsize,&bytes);
for (i=0; i<256; i++)      //Set up image list
  if (imgsizes[i])
        {
         eimagelist[i]=eimages+imgindex[i];
        } else eimagelist[i]=0;

for (i=0; i<14; i++)
 {
  _dos_read(vh,&size,2,&bytes);
  lseek(vh,size,SEEK_CUR);
 }

// printf("3");


//Read move stuff
unsigned short int eframesize;

_dos_read(vh,elist,sizeof(move)*130,&bytes);
_dos_read(vh,&eframesize,2,&bytes);
//printf("eframesize %d\n",eframesize);
eframe=(unsigned char *)malloc(eframesize); //printf("framesize %d\n",framesize);
_dos_read(vh,eframe,eframesize,&bytes);
_dos_close(vh);

//printf("eframe %X eimages %X\n",eframe,eimages);

}

#ifdef REVOL
char *soundfiles[]=
{
 "i1.rvc",
 "i2.rvc",
"i3.rvc",
"i4.rvc",
"i5.rvc",
"i6.rvc",
"i7.rvc",
"h1.rvc",
"h2.rvc",
"h3.rvc",
"h4.rvc",
"h5.rvc",
"h6.rvc",
"h7.rvc",
  "BLOCK.rVC",   //0
  "LAND.rVC",    //1
  "SPLAT.rVC",   //2

  "ROUND.rVC",   //3
  "1.rVC",       //4
  "2.rVC",       //5
  "3.rVC",       //6
  "4.rVC" ,      //7

  "FIGHT.rVC" ,  //8
  "KILL.rVC" ,   //9
  "SLAUGHT.rVC" ,//10
  "BUTCHER.rVC" ,//11

  "WIN.rVC" ,    //12
  "LOSE.rVC" ,   //13

  "DBLKO.rVC" ,  //14
  "TIMEUP.rVC" , //15
 "GUT.rVC" , //16
// "laugh.rVC" , //17
 
  0

  };
#endif

//  VOC *ReadVocFile(char *);
extern SOUND sysse[16];
extern SOUND sse[2][7];

void InitializeSoundEffects()
{
int h;
if (sbstat!=2) return;



if (_dos_open("sound.vol",O_BINARY | O_RDONLY,&h))
 {
  #ifdef REVOL
        _dos_creat("sound.vol",0,&h);
        chdir("\\sound");
        unsigned int size;
  for (int i=0; soundfiles[i]; i++)
        {
         char *t=ReadFile(soundfiles[i]);
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

//_dos_read(h,&sysidx,16*4,&bytes);
int i,j;
for (i=0; i<2; i++)
 for (j=0; j<7; j++)  ReadVolSound(h,&sse[i][j]);

for (i=0; i<=16; i++) ReadVolSound(h,&sysse[i]);

_dos_close(h);


}

void GetConfig()
{
int i;
int h;

//open timeslaughter config file
if (!_dos_open("noggin.cfg",O_BINARY | O_RDWR,&h))
 {  //Already exists
  options=(cfg *)malloc(sizeof(cfg));
  for (i=0; i<sizeof(cfg); i++)  *(((char *)options)+i)=0;
  _dos_read(h,options,filelength(h),&bytes);
  _dos_close(h);
 }
  else //Get config
 {
   printf("Configuration file not found.\n");
   printf("Please run SETUP before playing...\n");
   exit(1);
 }



for (i=0; i<2; i++)
{
p[i].in.center=options->jcenter[i];
p[i].in.min=options->jmin[i];
p[i].in.max=options->jmax[i];
}

}
