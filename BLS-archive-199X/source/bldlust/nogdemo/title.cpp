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

#include "tgraph.h"
#include "tinput.h"
#include "time.h"

extern playerstat p[2];
int b2x,b3x;
extern volatile char kbscan;
extern volatile int quit;
extern char *screen;

extern volatile unsigned int  uu;
extern int sbstat;
extern bground bg;


extern int fadelevel,oldfadelevel;

extern void UpdatePalette();
extern int fadeout,done,fadein;

extern int lastsize;
SOUND stumpsay[6];

int buttonpush;

void SayStump(int x)
{

if (!sbstat) return;
if (sound_playing(9)) return; //stop_sound(9); // return;
start_sound(&stumpsay[x], 9, 255, 0);

}    


int showchoice;
void TitleTick()
{
 uu++;

if (uu==90) SayStump(0);
if (uu==200)
{showchoice=1;
}

if (buttonpush) buttonpush--;

 if (uu<32) fadelevel=uu;
   else
  if (fadeout)
      if  (fadelevel>0) {  fadelevel--; }
         else done=1;

if (!(uu&3)) { b2x+=1; if (b2x>=320) b2x=0; }
if (!(uu&7)) { b3x+=1; if (b3x>=320) b3x=0; }

series *bp;  //bg.nums
int i;
bp=&bg.bgs[0];   for (i=0; i<bg.nums; i++,bp++)  bp->Tick();

if (!(uu&7))
{
 ((bgframe *)(bg.bgs[5].frameptr))->y^=1;         
 ((bgframe *)(bg.bgs[6].frameptr))->y^=1;         
} 
}


//1 talking
//2 button up
//3 1-2p options
//4 cursor
//5 button down
//6 noggin
//7 noggin

void Nofile(char *s);
int TitleScreen()
{

Solid((char *)0xA0000,64000,0);

bg.ReadBackground("nogtitle.vol",200);


int h;
unsigned bytes;
if (_dos_open("titlesnd.vol",O_BINARY|O_RDONLY,&h))
/* {  //revol it
  _dos_creat("titlesnd.vol",0,&h);
   unsigned int size;
   char s[30];
   for (int i=0; i<6; i++)
        {
         sprintf(s,"VOC\\VOC%d.RVC",i+1);   
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
    _dos_close(h);
    _dos_open("titlesnd.vol",O_BINARY|O_RDONLY,&h);
 }*/
 Nofile("titlesnd.vol");
 

for (int i=0; i<5; i++)
 ReadVolSound(h,&stumpsay[i]);
_dos_close(h); 
/*
ModeText();
for (i=0; i<6; i++)
 printf("%d\n",stumpsay[i].soundsize);
kbscan=0; while(!kbscan);
Mode256(); 
*/


uu=0;  done=0;
p[0].in.Reset(); p[1].in.Reset();
p[0].in.but=0; p[1].in.but=0; 
p[0].in.stat=0; p[1].in.stat=0; 

oldfadelevel=-1;fadelevel=0;
UpdatePalette();

//Set up our timer
fadeout=0; done=0;
kbscan=0; quit=0;

kbscan=quit=0;

bg.scrx=0; bg.scry=0;
b2x=0; b3x=0;

int result=0;

int choice=0;

int oldstat=0;
int talkleave=0;
buttonpush=0;
showchoice=0;

timerhandler=TitleTick;

do
{

  char *d=screen;

  PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,d, 0, 0, b2x,10/2, b3x,10/4,        0,0, bg.ylist1a, bg.ylist2);
  PutBackgroundTriOverlap(bg.bg1b,bg.bg2,bg.bg3,d, 0, 0, b2x,10/2+90, b3x,10/4+90,  0, 100, bg.ylist1b, bg.ylist2);

//  bg.DrawBGImages(screen);



if (buttonpush) bg.DrawBGSeries(screen,4);
 else bg.DrawBGSeries(screen,1);  


//options bgs
if (showchoice)
{
bg.DrawBGSeries(screen,2);  
bg.DrawBGSeries(screen+choice*25*320,3);
}

//noggin knockers words
bg.DrawBGSeries(screen,5);  
bg.DrawBGSeries(screen,6);  

///stump talk
if (sbstat && sound_playing(9))
 bg.DrawBGSeries(screen,0);
 

RefreshInput();

p[0].in.ReadButt(0);
p[0].in.ReadPosition(0);
int trigger=0; 

if (showchoice)
{
trigger=(kbscan==57 || kbscan==28 || (p[0].in.stat&(16+32))) && !quit;
p[0].in.stat&=15;
int dir=p[0].in.stat&oldstat;
oldstat=p[0].in.stat^0xFFFF;
kbscan=0;

if (dir&4) {if (choice>0) {choice--; buttonpush=30;} SayStump(choice+1);}
if (dir&8) {if (choice<2) {choice++; buttonpush=30;} SayStump(choice+1);}
} else p[0].in.stat=0;

if (talkleave==0) if (trigger) {talkleave++; buttonpush=200; showchoice=0;}
if (talkleave==1) //on our way out
 if (!sbstat || choice==2) talkleave=3; //just quit if no sound
  else
  if (!sound_playing(9)) //wait till no sound playing
    {SayStump(4); talkleave++;}

if (talkleave==2) if (!sound_playing(9)) talkleave++;
if (talkleave==3) fadeout=1;
  
   

        
  MemoryCopy((char *)(0xA0000+5*320),screen,195*320);
  UpdatePalette();
  
 if (quit) fadeout=1;

} while (!done);

if (quit) choice=-1;

timerhandler=0;
if (sbstat) stop_sound(9);

//ModeText();
//cprintf("freeing stumpsounds...");
for (i=0; i<5; i++) free(stumpsay[i].soundptr);
//printf("done\n");

fadelevel=0;
UpdatePalette();

//cprintf("freeing titlebackground...");
bg.KillBackground();
//printf("done\n");

return(choice);
}


extern int talking;
effect skull;

SOUND voc1,voc2;
extern cfg *options;

extern char *font, *font2,*font3;

void DrawBigWord(char *s,int x, int y)
{
DrawString(font2,screen+y*320+x-GetStringWidth(font2,(unsigned char *)s),
    (unsigned char *)s,320);
DrawString(font2,screen+(y-12)*320+x+1,
    (unsigned char *)".",320);
DrawString(font2,screen+(y-5)*320+x+1,
    (unsigned char *)".",320);
}


void OptionsTick()
{
 uu++;

// if (uu<32) fadelevel=uu;
if (fadeout)  if  (fadelevel) fadelevel--; else done=1;

bg.scrx+=1; //direction;
//if (bg.scrx>320) bg.scrx=0;


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

extern int roundcount;
extern move elist[];
extern unsigned char *eframe;

extern void UpdatePalette2();

void Nofile(char *);

void OptionsScreen()
{
int i;
int h;
unsigned bytes;

 Solid((char *)0xa0000,64000,0);
 bg.ReadBackground("select.vol",0);

for (i=1; i<100; i++) bg.ylist1a[i]=bg.ylist1a[0];
//bg.bg1a[bg.ylist1a[0]+1]=-0x70;
//bg.bg1a[bg.ylist1a[0]+2]=-0x70;
//bg.bg1a[bg.ylist1a[0]+3]=-0x70;
bg.bg1a[4]=0xFF;
bg.bg1a[5]=0xFF;
bg.bg1a[6]=0xFF;
bg.bg1a[7]=0xFF;
bg.bg1a[8]=0xFF;

 uu=0;
bg.scrx=160; bg.scry=10; 

oldfadelevel=-1;
UpdatePalette2();


fadeout=0;fadelevel=32;
kbscan=0; quit=0;
p[0].d=0; p[1].d=0;
p[0].in.Reset(); p[1].in.Reset();
done=0;


skull.e=(unsigned char *)(eframe+elist[27].firstframe);
skull.currframe=0; skull.numframes=elist[27].numframes;
skull.en=27; skull.d=0;
skull.dur=((frametype *)skull.e)->dur;
skull.tcx=skull.tcy=0;
skull.maxy=999;


if (_dos_open("select3.vol",O_BINARY | O_RDONLY,&h))  Nofile("select3.vol");

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
level[3]=options->talking;
level[4]=options->roundcount;

int l=0;
char *lwords[]={"DIFFICULTY","SOUND VOL","MUSIC VOL","TALKING","ROUND WIN"};
int lmax[5]={7,7,7,1,22};

int ostat=0;

/*
if (!overlay)
 if (cdplay) PlayCDTrack(7);
  else
 if (musicstat) start_music(bg.music,1);
*/
do
{

//bg.DrawBackground(screen);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, bg.scrx%320,   0,  (bg.scrx%640)/2,  0,  0,   0, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, bg.scrx%320,  100, (bg.scrx%640)/2, 100,  0,  100, bg.ylist1a, bg.ylist2);

//PutBackground(bg.bg1a,screen,320-95,0,0,0,bg.ylist1a);
//PutBackground(bg.bg1b,screen,320-95,0,0,100,bg.ylist1b);


for (i=0; i<5; i++)
 {
  int x=185,y=22+i*25-bg.scry;
     
  if (i!=l || uu&32)
  DrawBigWord(lwords[i],x-15, y);
  if (i==4) 
    {
      char s[10];
      sprintf(s,"%d POINTS",level[i]+3);
      DrawStringSP(font,screen+(y+3)*320+x,(unsigned char *)s,320);
     }
   else
   if (i==3)
    {
     DrawStringSP(font,screen+(y+3)*320+x,(unsigned char *)(level[i] ? "ON" : "OFF"),320); 

    }
  else DrawLevel(x,y,level[i]);    
 }
 

  

//DrawBigWord("EXIT", 200,155);
if (l!=5 || uu&32)
  DrawString(font2,screen+135*320+120,(unsigned char *)"EXIT",320);


  MemoryCopy((char *)0xA0000+30*320,screen+10*320,320*147);
  UpdatePalette2();
  


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
//      if (cdplay) SetCDVolume(32*level[2]+31);
//       else
      if (musicstat)
       set_music_volume(32*level[2]+31);
      
      break;
   }
 }   

 if (l==5 && p[0].in.stat&(16+32)) quit=1;

 
 p[0].in.stat&=15;
 ostat=p[0].in.stat;


} 


if (quit) fadeout=1; 

} while (!done);

timerhandler=0;

if (sbstat==2) StopVoice();
/*if (cdplay) StopCD();
 else
if (musicstat) stop_music();
*/


bg.KillBackground();

fadelevel=0;
UpdatePalette();

free(voc1.soundptr);
free(voc2.soundptr);

options->difficulty=level[0];
options->soundvolume=level[1];
options->musicvolume=level[2];
options->talking=level[3];
options->roundcount=level[4];
roundcount=options->roundcount+3;
 _dos_creat("noggin.cfg",0,&h);
 lseek(h,0,SEEK_SET);
 _dos_write(h,options,sizeof(cfg),&bytes);
 _dos_close(h);


}

    
