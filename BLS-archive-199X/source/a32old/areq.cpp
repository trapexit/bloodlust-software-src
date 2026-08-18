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

#define ANIMATOR


#include "a32.h"

extern int mx,my,mb,mstat;
void refreshmouse();
extern char *screen;
extern SCR *bg;
extern int numodf;
extern objectdef *odf;
extern FONT *font[];
extern IMG *cursor;
extern char *extraname[];



extern lpoint dragrel;


extern void (*timerhandler)();
void timer();
//request an image
int objectdef::RequestImage()
{
wait();

int done=0;

static int basex=0,maxx=320;
int imgnum=-1;
timerhandler=0;

do
{
//mb=readmouse(&mx,&my);
refreshmouse();
memcpy(screen,bg->data(),64000);

int x=basex;
imgnum=-1;
for (int i=0; i<numi; i++)
 {
  int imgwidth=imgnames ? font[0]->getwidth(imgnames[i]) : 32;
  int imgheight=0;

  if (id[i])
  {
   if (imgwidth<id[i]->xw) imgwidth=id[i]->xw;
   imgwidth+=2;
   imgheight=id[i]->yw;
  } 

  if (x>-150 && x<321)
 { 
  if (mx>x && mx<x+imgwidth)
   {
    char s[30];
    if (imgnames)
     DrawCenteredString(font[1],screen,imgnames[i],x+imgwidth/2,145);
    drawrect(screen,14,x,145-imgheight,imgwidth,imgheight);
    imgnum=i;
    sprintf(s,"i%d xw%d yw%d",i,id[i]->xw,id[i]->yw);
    DrawString(font[1],screen,s,5,170);
    dragrel.x=x-mx;
    dragrel.y=(145-imgheight)-my;
    

   } else
    if (imgnames)
     DrawCenteredString(font[0],screen,imgnames[i],x+imgwidth/2,145);

  if (id[i])
   id[i]->draw(screen,x,145-imgheight,0);
 }
  x+=imgwidth;
 }   
maxx=x;



int dir=1,delta=0,offset=mx;
if (mx>160) {offset=320-mx; dir=-1;}

if (!(maxx<300 && basex==0))
if (offset<110){ delta+=1;
if (offset<80) {delta+=1;
if (offset<40) {delta+=1;
if (offset<20) {delta+=2;
if (offset<10) {delta+=2;
if (offset<5) {delta+=2;
               }}}}}}
basex+=dir*delta;               
if (basex>0) basex=0;
if (maxx<300 && basex!=0) basex=basex+300-maxx;

/*
if (mx<110) if (basex<0) basex+=1;
if (mx>210) if (maxx>300) basex-=1;
if (mx<50) if (basex<0) basex+=4;
if (mx>270) if (maxx>300) basex-=4;
if (mx<5) if (basex<0) basex+=15;
if (mx>315) if (maxx>300) basex-=15;
*/

//send to video
cursor->draw(screen,mx,my,0);  //draw mouse
memcpy(video,screen,64000);

if (mstat&1)  done=1;
if (mstat&2) {imgnum=-1; done=1;} //just quit
}while (!done);

timerhandler=timer;

mstat=0;
return(imgnum);
}


int objectdef::RequestSound()
{
if (!sndnames) return -1;    
wait();    
char *bg=(char *)malloc(64000);
int done=0;
memcpy(bg,screen,64000);

static int basey=0,maxy;
int snd;

do
{
//=readmouse(&mx,&my);
refreshmouse();
    
memcpy(screen,bg,64000);


int y=basey;
snd=-1;
for (int i=0; i<numsounds; i++,y+=10)
 if (y>-10 && y<170)
 {
  char s[80];
  sprintf(s,"%s",sndnames[i]);
  if (my>y && my<y+10) snd=i;
  
  if (snd==i)  DrawString(font[1],screen,s,5,y);
         else   DrawString(font[0],screen,s,5,y);
 }   
maxy=y;

if (my<50) if (basey<0) basey+=1;
if (my>150) if (maxy>180) basey-=1;
if (my<20) if (basey<0) basey+=3;
if (my>180) if (maxy>180) basey-=3;

//send to video
cursor->draw(screen,mx,my,0);  //draw mouse
memcpy(video,screen,64000);

if (mstat&1)  done=1;
if (mstat&2) {snd=-1; done=1;} //just quit
}while (!done);

//wait();
mstat=0;
free(bg);
return(snd);
}







void drawscreen(char *s,char *d,int x,int y);

//request an scree
int backgrounddef::RequestScreen()
{
wait();

int done=0;

static int basex=0,maxx=320;
int scrnum=-1,fx;
timerhandler=0;

do
{
refreshmouse();

scrnum=basex/320;
fx=basex%320;
drawscreen((char *)(scr[scrnum]), screen,fx,0);
drawscreen((char *)(scr[scrnum+1]),screen,fx+320,0);
if (mx>fx+320)  scrnum++;

maxx=-(numscreens-1)*320+1;


int dir=1,delta=0,offset=mx;
if (mx>160) {offset=320-mx; dir=-1;}

//if (basex==0))
if (offset<110){ delta+=1;
if (offset<80) {delta+=1;
if (offset<40) {delta+=1;
if (offset<20) {delta+=2;
if (offset<10) {delta+=2;
if (offset<5) {delta+=2;
               }}}}}}
basex+=dir*delta;               
if (basex>0) basex=0;
//if (maxx<300 && basex!=0) basex=basex+300-maxx;
if (basex<maxx) basex=maxx;
dprintf(font[0],0,0,"%d %d",basex,maxx);

//send to video
cursor->draw(screen,mx,my,0);  //draw mouse
memcpy((char *)0xA0000,screen,64000);

if (mstat&1)  done=1;
if (mstat&2) {scrnum=-1; done=1;} //just quit
}while (!done);

timerhandler=timer;

mstat=0;
return(scrnum);
}

