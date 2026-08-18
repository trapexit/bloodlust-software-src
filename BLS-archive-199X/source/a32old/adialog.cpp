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


extern int mx,my,mb,mstat,mrelstat;
extern char *screen,*bg;
extern FONT *font[];


void drawarrow(int stat,int x,int y);

void dialog::init(rect *pos,char *ttitle,void *tarray,int tsize,int tnum,int selnum,
       void *gsf,void *sf,void *tcontext)
{
 if (active) return;
 x=pos->x1; xw=pos->x2-x;
 y=pos->y1; yw=pos->y2-y;

 strcpy(title,ttitle);
 array=(char *)tarray; size=tsize; num=tnum;
 getstr=(void (*)(char *s,void *element))gsf;
 selfunc=(void (*)(void *array,int num,void *context))sf;

 dblclicktime=0; buttonregion=0;
 selected=selnum; //nothing selected
// if (selected==-1) basey=0;
   basey=-selected*10+20;
 if (basey>0) basey=0;
 context=tcontext;  
 active=1; //fire it up
// mstat=0; mb=0;
}    

void dialog::draw()
{
 if (!active) return;
 int done=0;

 //draw dialog box
// DrawBarMap(screen,SHADOWMAP,x,y+10,xw,yw-20);    

 //find mouse coordinates relative to dialog box
 int cx=mx-x,cy=my-y;
 

 //check region areas of cursor if button is pushed
if ((mstat&5) || (mrelstat&1))
{
 int inside=(cx>0) && (cx<xw) && (cy>0) && (cy<yw); //is inside the box?
 int sel=0;
 if (inside)
 {
  if (cy>yw-10) //is it in the bottom button area?
   if (cx<xw/2) sel=1; //on OKAY
        else    sel=2; //on cancel
  else //it's above bottom
  if (cy>10) //if below titlebar
    if (cx>xw-10) sel=3;//in scroll bar
           else sel=4;     //in list window
 }

 if (mrelstat&1)
  {
   if (sel==1 && buttonregion==1) done=1; //okay
   if (sel==2 && buttonregion==2) done=2;  //cancel
   buttonregion=0; //no button region now
  }
 if (mstat&5) buttonregion=sel;
}

//scroll up and down
if (buttonregion==4) //if we last clicked in the list
 {
  if (cy<10) //scroll up
   {
    if (uu>scrolltime) //time to scroll
     {if (selected>0) {selected--; basey+=10; if (basey>0) basey=0;} scrolltime=uu+2;}
   }

  if (cy>yw-15) //scroll down
   {
    if (uu>scrolltime) //time to scroll
     {if (selected<num-1) {selected++;if  (basey+num*10>yw-20) basey-=10; } scrolltime=uu+2;}
   }
 }

if (selected!=-1)
{
if (kbscan==72)
 {if (selected>0) {selected--; basey+=10; if (basey>0) basey=0;} kbscan=0;}
if (kbscan==80) 
 {if (selected<num-1) {selected++;if  (basey+num*10>yw-20) basey-=10; } kbscan=0;}
if (kbscan==0x1C) done=1;
}
if (kbscan==1) done=1;

 //draw list
 if (array && getstr)
  {
   int ty=basey+10;
   char *tarray=array;
   char s[256];
   for (int i=0; i<num; i++) //go through all elements
   {
/*    if (selected!=-1)
    {
     if (ty<10)   if (selected==i) basey+=10;
     if (ty>yw-20) if (selected==i) basey-=10;
    }*/

    //if inside window, draw it
       
    if (ty>0 && ty<yw-10)
    {
     getstr(s,tarray); //get the string to draw
     if (buttonregion==4 && cy>=ty && cy<ty+10) //if cursor on element
      {
       if (mstat&1) //if just clicked
         {
          if (uu<dblclicktime) {if (selected==i) done=1;}
           else dblclicktime=uu+45;
         }
       if (mstat&4) {selected=i; done=1;}
       if (mb&1) selected=i;
      }

     if (selected==i) //if this one has been selected
      {
       drawrect(screen,2,x,y+ty,xw-9,10);
       dprintf(font[1],x+2,y+ty,s);
      } else dprintf(font[3],x+2,y+ty,s);
    } 
    
    tarray+=size; //go to next element
    ty+=10;       //increase y
   } //done for

  }

 //draw title
 drawrect(screen,0,x,y,xw,10);
 dprintf(font[2],x,y,title);

 //draw okay/cancel stuff
 drawrect(screen,0,x,y+yw-10,xw,10);
 if (mb&1)
 {
  if (buttonregion==1)  drawrect(screen,2,x,y+yw-10,xw/2,10);
  if (buttonregion==2)  drawrect(screen,2,x+xw/2,y+yw-10,xw/2,10);
 }
 font[buttonregion==1 ? 2 : 3]->drawcentered("Ok",screen,x+xw/4,y+yw-10);
 font[buttonregion==2 ? 2 : 3]->drawcentered("Cancel",screen,x+xw*3/4,y+yw-10);
 
 //draw scroll bar
 drawrect(screen,8,x+xw-9,y+10,9,yw-20);
 drawarrow(ID_UP,  x+xw-8,y+8);
 drawarrow(ID_DOWN,x+xw-8,y+yw-18);



// if (mstat&2) done=2;

 //see if we should abort
 if (done)
  {
   active=0;
   if (done==1 && selected!=-1 && selfunc) //call the finish function
    selfunc(array,selected,context);
  }
  
}    






//draw object def in dialog
void dlg_objectdef(char *s,objectdef *e)
{
 sprintf(s,"%d:%s",e->onum,e->name);
}


//draw object def in dialog
void dlg_series(char *s,series *e)
{
 sprintf(s,"%02d:%s",e->nf,e->name);
}

//draw object def in dialog
void dlg_extra(char *s,char **e)
{
 strcpy(s,*e);
}

void dlg_cfunc(char *s,char *e)
{
 strcpy(s,e);
}


