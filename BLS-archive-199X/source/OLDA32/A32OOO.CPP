//Copyright(c) 1996 Bloodlust Software All rights reserved
//animator 32-bit
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

#include "misc.h"
#include "ttimer.h"
#include "keyb.h"

#include "glib.h"
#include "input.h"
#include "config.h"
extern "C"
{
#include "smix.h"
}
#include "vol.h"


#include "object.h"
#include "effect.h"


char errstr[80];
void cleanexit(int x)
{
TerminateTimers();
terminate_keyboard();
if (sbstat)
{
 shutdown_mixing();
 shutdown_sb();
 sbstat=0;
}

ModeText();    
if (x<0) printf("ERROR: %s\n",errstr);
exit(x);
}


volumefile v;

//Sound stuff
int sbstat;

//standard stuff
int i,j,k;
color *pal;
char *screen,*bg;
img *cursor,*shadow,*X,*Xshadow;
FONT *font[6];
int mx,my,mb,mstat=0,oldmb=-1; //mouse x,y,buttons
int basex=160,basey=160;
char key=0,kscan=0;

//int speed=100;

int verbose=0;

int gshakedur=0;
int gshake[8]={0,1*320,3*320,2*320,-1*320,-0*320,-2*320,0};


char msg[100];
int msgdur=0;
int drawstat=1;

//standard config
config *cfg; 

//input devices
input inputdevice;  //main input device
input inputnone; //null input


//control function names
char cfuncnames[256][32];
int cfuncnum;

//object definitions
int numodf=0;
objectdef *odf;
volatile int needload=0;   //flag set when object definition needs loading
volatile int needdelete=0; //flag set when object instance needs deleting


//list of all objects
object *o=0;

//object that is being edited
object    *p=0; //current object being edited (none)
object    pshadow(0,0,0,0,0,0); //shadow of p

//editing mode
int emode=0;
static byte *doedit=0;


//image dragging stuff
image *dragimg=0;
lpoint dragrel;

//clipboard
enum {EMPTY,IMAGES,FRAMES} cliptype=EMPTY;
int clipsize=0; //size in bytes in buffer
int clipnum=0; //number of images/frames in buffer
char clipboard[500];

//menu stuff
int menustat=0;
char *menustr[]=
 {
  "LoadObject", //0
  "SaveObject", //1
  "KillObject", //2
  "", //3
  "ListSeries", //4
  "NewFrame", //5
  "NewImage", //6
  "NewEffect", //7
  "", //8
  "Quit",       //9
  0
 };
#define M_NEWOBJECT  0
#define M_SAVEOBJECT 1
#define M_KILLOBJECT 2

#define M_SERIESLIST 4
#define M_INSERTFRAME 5
#define M_NEWIMAGE   6
#define M_NEWEXTRA   7
#define M_QUIT 9

#define M_INSERTAFTER 100 


int random(int x)
{
 return((x*rand())/32768);
}

//reads object names from the object definition file
void ReadObjectNames(char *onf);
int ReadImageList(char *dir,char *list,imagedef **imagedefs);
int ReadSoundList(char *dir,char *list,sounddef **sounddefs);
int RequestExtra();
void ReadControlFuncs(char *cfile);
int RequestControlFunc();

//requests the user to select an object from a objectdeflist
int RequestObject();
int DoMenu();
void CheckKeys();


#define BW 16
int DrawButton(int x,int y,int num, int c,int c2,int highlight)
{
char s[10];    
DrawBar(screen,c,x,y,BW,12);
if (highlight)
{ DrawBar(screen,c2,x+1,y+1,BW-2,12-2);}

if (highlight || num)
{
 sprintf(s,"%02d",num);
 DrawCenteredString(font[highlight ? 1 : 3],screen,s,x+BW/2,y+2);
}

return(!dragimg && (mstat&1) && mx>x && mx<x+BW && my>y && my<y+12);
}

image *object::CheckImageBound()
{
image *found=0;
if (!fptr || killme) return 0;
    
register image *i=fptr->getimgptr();    
for (j=fptr->numimages; j>0; j--) //
 {
  img *a=od->id[i->index].ptr; //get pointer to this image
  if (mx>(x>>16)+i->dispx && my>(y>>16)+i->dispy && mx<(x>>16)+i->dispx+a->xw && my<(y>>16)+i->dispy+a->yw)
   found=i; //we found an image baby
  i++;
 }
i--;

if (found) //hey, we found one
 {
  if (this==p && !p->playing) //move to top
  {
  image timg=*found;
  memmove(found,found+1, ((char *)i)-((char *)found) );
  *i=timg;  //move selected image to the LAST image
  found=i;
  }

  if (!d) dragrel.x=(p->x>>16)-mx+((int)found->dispx);
   else dragrel.x=-((p->x>>16)-mx)+((int)found->dispx);
  dragrel.y=(p->y>>16)-my+((int)found->dispy);
  
 }

return(found);     
}    


volatile unsigned uu=0;
volatile unsigned su=1;

//reset shadow to look like last frame of current char
void resetshadow()
{
 if (p && !p->playing) pshadow=*p;
     else {pshadow.fptr=0; pshadow.otype=0; pshadow.od=0;}
}


//standard timer tick
void timer()
{
 uu++;
 if (msgdur>0) msgdur--;
 if (gshakedur>0) gshakedur--;

 //refresh timered input
 refreshinput();
 inputdevice.read();


 //tick all playing objects
 for (object *t=o; t; t=t->next)
 if (t->playing)
 {
  t->tick(); //tick the object

  if (!t->playing && t==p) resetshadow();
 } 
  

}    


//standard key function
int keyboard()
{

//refresh the input device if keyb  
if (inputdevice.read() && p && p->playing) return 1; //dont add key to queue

    //if key pressed stop playing
if (p && p->playing) //if selected object is playing
 {
  if (kbscan==15) //tab
    return 0;     //add it to queue

   //if other key hit, stop playing of this selected object 
  if (!(kbscan&0x80)) 
  {
//   if (p->active) mprintf("%s deactivated",p->od->name);
   p->playing=0; //stop playing
   p->active=0; //no longer active either
   p->shakedur=0;
   p->cs->cf=p->cf;
   resetshadow();
   return 1; //dont add to queue
  }
 }


return 0; //add key to queue
}

void resetallobjects()
{
for (object *t=o; t; t=t->next)
  {
     t->resetfptr(); //reset it
  }   
}

void modetext()
{
ModeText();
}    

void refreshmouse()
{
oldmb=mb;
mb=readmouse(&mx,&my);
mstat=mb&(oldmb^0x3); //triggers
}      



struct meminfo {
 unsigned LargestBlockAvail;
 unsigned MaxUnlockedPage;
 unsigned LargestLockablePage;

 unsigned LinAddrSpace;
 unsigned NumFreePagesAvail;
 unsigned NumPhysicalPagesFree;
 unsigned TotalPhysicalPages;
 unsigned FreeLinAddrSpace;
 unsigned SizeOfPageFile;
 unsigned Reserved[3];
} MemInfo;

void getmeminfo()
{
 union REGS regs;
 struct SREGS sregs;

 regs.x.eax = 0x00000500;
 memset( &sregs, 0, sizeof(sregs) );
 sregs.es = FP_SEG( &MemInfo );
 regs.x.edi = FP_OFF( &MemInfo );
 int386x( 0x31, &regs, &regs, &sregs );
/*         printf( "Largest available block (in bytes): %lu\n", MemInfo.LargestBlockAvail );
         printf( "Maximum unlocked page allocation: %lu\n", MemInfo.MaxUnlockedPage );
         printf( "Pages that can be allocated and locked: %lu\n", MemInfo.LargestLockablePage );
         printf( "Total linear address space including allocated pages: %lu\n", MemInfo.LinAddrSpace );
         printf( "Number of free pages available: %lu\n", MemInfo.NumFreePagesAvail );

         printf( "Number of physical pages not in use: %lu\n", MemInfo.NumPhysicalPagesFree );
         printf( "Total physical pages managed by host: %lu\n",MemInfo.TotalPhysicalPages );
         printf( "Free linear address space (pages): %lu\n", MemInfo.FreeLinAddrSpace );
         printf( "Size of paging/file partition (pages): %lu\n", MemInfo.SizeOfPageFile );*/
}






int choice;
void main(int argc, char *arg[])
{
getmeminfo();
printf("%dK available\n",MemInfo.NumFreePagesAvail*4);

cfg=load_config("c:\\a32\\a32.cfg");
if (!cfg) {printf("Error: config file not found. Run a32setup.\n"); exit(1);}

ids=cfg->ids; //copy over input settings


//setup keyboard
init_keyboard();
keybios=0;
keyrepeat=1;
keyqueue=1;

/*
char k;
do
{
k=waitkey();

printf("%d %c\n",k,scan2ascii(k));
printf("%d\n",kbstat);
}  while (k!=1);

//exit(1);
*/



//setup timer
InitializeTimers();

//setup input devices
initmouse();
inputdevice.init(cfg->pinput[0]); //initialize input
inputnone.init(ID_NONE);

 
//vdiagnose=1;    
v.open("c:\\a32\\a32.vol");
pal=(color *)v.read();
bg=v.read();
cursor=(img *)v.read();
font[0]=(FONT *)v.read();
font[2]=(FONT *)malloc(6000); memcpy(font[2],font[0],5986);
font[3]=(FONT *)malloc(6000); memcpy(font[3],font[0],5986);

FunkyFont(font[0],0xca,0xc6);
FunkyFont(font[0],0xdb,0xd8);
font[1]=(FONT *)v.read();
FunkyFont(font[1],0xca,0xff);
FunkyFont(font[1],0xdb,0x0);

FunkyFont(font[2],0xca,6*16);
FunkyFont(font[2],0xdb,0x0);

FunkyFont(font[3],0xca,0x7);
FunkyFont(font[3],0xdb,0x0);

shadow=(img *)v.read();
X=(img *)v.read();
Xshadow=(img *)v.read();
v.close();
screen=(char *)malloc(70000)+2000;
memset(screen-2000,0,70000);

for (i=0; i<256; i++) //do shadow map
 {
  unsigned char tr,tg,tb;
  tr=((unsigned)pal[i].r)*7/8;  tg=((unsigned)pal[i].g)*7/8;  tb=((unsigned)pal[i].b)*7/8;   

  unsigned int closeness=9999,closecolor=0,k;
  for (int j=0; j<256; j++)
    {
      k=abs(tr-pal[j].r)+abs(tg-pal[j].g)+abs(tb-pal[j].b);
      if (k<closeness) {closeness=k; closecolor=j;}
    }
  SHADOWMAP[i]=closecolor;
 }


//read control functions
ReadControlFuncs("c:\\a32\\cfunc.lst");

//read object list
ReadObjectNames("c:\\a32\\object.lst");
mprintf("%d object definitions. %d control funcs. %dK available.",numodf-1,cfuncnum,MemInfo.NumFreePagesAvail*4);





//start up soundblaster
sbstat=0;


//cfg->soundcard=CFG_NOSOUND;
if (cfg->soundcard!=CFG_NOSOUND)
if (!init_sb(cfg->sbport,cfg->sbirq,cfg->sbdma,cfg->sbdma16))
//if (!init_sb(0x220,5,1,5))
 {
  printf("ERROR:  Error initializing sound card!\n");
  sbstat=0;
 } else
 {
  init_mixing();
  set_sound_volume(255);
  printf("soundblaster initialized successfully\n");
  sbstat=2;
 }





Mode256();
LoadPalette(pal,0,256);
memcpy((char *)0xA0000,bg,64000);





for (int i=1; i<argc; i++)
 {
  int x=atoi(arg[i]);
  if (x)
   {
    for (int j=0; j<numodf; j++)
      if (odf[j].otype==x) break;
     if (j!=numodf)
      { 
       object *t=new object(j,0,basex,basey,0,0); //create an instance of the object
       if (!t->od->loaded) {free(t); break;}
       if (p) t->x=p->x+(70<<16);   

       //add object to beginning of object list
       o->prev=t; t->prev=0; t->next=o; p=o=t;   
       resetshadow();
      }
   }
 }

if (p) p->in=&inputdevice; //be sure to set its input device

//start up keyboard handler
set_keyboard_func(keyboard);
SetTimerSpeed(speed);
SetTimerFunc(timer);


int done=0;
choice=-1;

do
{

//load needed objects
if (needload)
 {
  for (int i=0; i<numodf; i++)
   if (!odf[i].loaded && odf[i].refcount)  //not loaded, but referenced
    {
     odf[i].Read(); //read it into memory
     _disable();     
     object *t=o;
     while (t)
     {
      object *next=t->next;
      if (t->otype==i)
      if (!odf[i].loaded) delete t; //if not loaded still, was error, delete all referenced objects
       else //was loaded okay, bind to objectdef
        {
         t->od=&odf[i]; //bind
         t->startseries(t->csnum); //start series back up (set fptr)
        }
      t=next;
     }
     _enable();     
    }
 needload=0;    
 }

//need to delete object instance
if (needdelete)
 {
  _disable();
  object *t=o;
  while (t)
   {
    object *next=t->next;
    if (t->killme) //delete all flagged objects
     {
      if (p==t)
       {
        if (t->next) p=t->next; else if (t->prev) p=t->prev; else p=NULL;
        if (p) p->in=&inputdevice; 
       }
      delete t;
      resetshadow();
     }
    t=next;
   }
  needdelete=0;
  _enable();
 }    



//refresh all input
refreshmouse();
refreshinput();



//clear screen
memcpy(screen,bg,64000);


//IF OBJECT BEING EDITED
if (p)
{

//choice=-1;

//draw ellipse shadows
for (object *t=o; t; t=t->next)
if (t->fptr)
{
 if (t==p)
   DrawImage(shadow,screen,(t->x>>16)-shadow->xw/2,(t->y>>16)-shadow->yw/2,0);
 else
  DrawImageMap(shadow,screen,(t->x>>16)-shadow->xw/2,(t->y>>16)-shadow->yw/2,0);
}


//draw shadow map character
if (!p->playing)
{//pshadow.x=p->x; pshadow.y=p->y;
 pshadow.drawmap();
}


if (emode==3 && p) //extra edits
     p->DrawExtraXShadow();



//sort characters
_disable();
for (t=o; t; t=t->next) t->resort();
_enable();

//draw character(s)
for (t=o; t; t=t->next)
{
   //scoot it back if it goes offscreen
 if (t->x<(-20<<16)) t->x=340<<16;
 if (t->x>(340<<16)) t->x=-20<<16;

 t->draw();
}

if (emode==3 && p) //extra edits
     p->DrawExtraX();



if (p->nf && p->fptr) //IF THERE ARE FRAMES IN THIS SERIES
{
       //IF WE ARE EDITING IMAGES
if (!menustat)
 if (emode==0) //image editing mode
  {
   if (!dragimg) //NOTHING being dragged
    {if (mstat&1) //triggered
      {
       //check to see if an image in the edited char was clicked on
       if (!p->playing) dragimg=p->CheckImageBound();
       if (dragimg)
         {
           p->od->dirty++; //it's dirty now
         }
             else  //find the "other" object that was clicked on maybe
       for (object *t=o; t && !dragimg; t=t->next)
         if (t!=p) //if not being played
           if (t->CheckImageBound())
             {
              p->in=&inputnone;
              p=t; resetshadow(); 
              p->in=&inputdevice;
             }
      }
      
     }
      else //we are dragging
      if (mb&1)
      {
       if (!p->d) dragimg->dispx=(short)(dragrel.x+mx-(p->x>>16)); //move image
            else  dragimg->dispx=(short)(dragrel.x-(mx-(p->x>>16))); //move image
       
       dragimg->dispy=(short)(dragrel.y+my-(p->y>>16));
       dprintf(font[1],2,160,"%s %ddx %ddy",p->od->id[dragimg->index].name,dragimg->dispx,dragimg->dispy);
      } else dragimg=0;
      
   if (mstat&2) //new image!
     choice=M_NEWIMAGE; 
  }

//someone wants to insert extra
if (!menustat && !p->playing && !doedit)
if (emode==3)
 if (mstat&2)
 {
  choice=M_NEWEXTRA;
  wait();
 }
  

if (drawstat)
{
if (DrawButton(255,188,p->fptr->numimages,6*16+15,6*16+6,emode==0)) emode=0;
if (DrawButton(255+BW,188,p->fptr->numvuln,8*16+15,8*16+6,emode==1)) emode=1;
if (DrawButton(255+BW*2,188,p->fptr->numattack,2*16+15,2*16+6,emode==2)) emode=2;
if (DrawButton(255+BW*3,188,p->fptr->numextra,1*16+15,1*16+6,emode==3)) emode=3;
}

} else if (mstat&2) choice=M_SERIESLIST;

}  else if (mstat&2) choice=M_NEWOBJECT;  



//check keys
CheckKeys();

//do menu choice
if (choice!=-1)
switch(choice)
{
case M_NEWOBJECT:
   {
    int tempo;
    if (!(tempo=RequestObject())) break; //get requested object 

    object *t=new object(tempo,0,basex,basey,0,0); //create an instance of the object
    if (!t->od->loaded) //error creating object
       {free(t); break;} //free it and abort

    if (p) //if previous object there
     { t->x=p->x+(85<<16);  p->in=&inputnone; }

    //add object to beginning of object list
    o->prev=t;
    t->prev=0; t->next=o;
    p=o=t;

    p->in=&inputdevice; //be sure to set its input device

    resetshadow();
   }
   break;

case M_KILLOBJECT:
  {
   if (!p || p->playing) break;
   _disable();
   object *t=p;
   if (p->next) {p=p->next; delete t;}
    else
   if (p->prev) {p=p->prev; delete t;}
    else {delete t; p=o;}
   resetshadow();
   
   if (p && p->active && p->playing) p->in=&inputdevice;
   _enable();
  }
 break;  
case M_SERIESLIST:
   if (!p || p->playing) break;
   resetshadow();
   i=p->od->RequestSeries();
   if (i!=-1) p->startseries(i);
  break;
case M_NEWIMAGE:
      if (!p || p->playing) break;
      emode=0;
      p->resetfptr();
      i=p->od->RequestImage();
      if (i!=-1)
        {
         _disable();
         image *im=p->fptr->getimgptr(); //get pointer to image locations
         
         p->fptr->size+=sizeof(image);
         p->fptr->numimages++;          //increase image counter
         im=(image *)p->cs->insertbytes((char *)im,sizeof(image)); //get memory for new image
         p->resetfptr();
         im->index=(unsigned char)i;  //store image index
         im->orient=0; im->dispx=(short)-(p->x>>16); im->dispy=(short)-(p->y>>16);
         _enable();
        }
     break;

case M_NEWEXTRA: //new extra
    if (!p || !p->nf || p->playing) break;
     {
     emode=3;
     int i=RequestExtra();
     if (i==-1) break;
     wait();
     p->AddExtra(i);
     }
    break;     

case M_INSERTAFTER:
      if (!p || p->playing) break;
        p->cs->cf++; p->cf++; p->resetfptr();
case M_INSERTFRAME:
       if (!p || p->playing) break;
        _disable();
        p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,sizeof(frame));
        p->cs->nf++; p->nf++; //p->resetfptr();
        p->fptr->size=sizeof(frame); //reset frame
        p->fptr->dur=10;
//        resetallobjects();
        _enable();
   break;
case M_SAVEOBJECT:
  if (!p || p->playing) break;
  _disable();
  p->od->SaveSeriesList();
  _enable();
 break;   
case M_QUIT:
//kill all objects
  while (o)
  {
   delete o;
  }
  done=1;
  break; 
}

if (emode==3 && !menustat)
 if (p) p->DrawExtra();

if (msgdur && !menustat)
 DrawString(font[2],screen,msg,0,0);


choice=-1;
if (!dragimg && !doedit) choice=DoMenu();

if (!dragimg && !doedit)
  DrawImage(cursor,screen,mx,my,0);  //draw mouse


if (drawstat)
{
//if we are editing

if (p) //p is current player being edited
{
  
 char s[80];
 //draw object name
 DrawString(font[1],screen,p->od->name,2,170);
 //draw series name
// DrawString(font[2],screen,p->cs->name,2,180);
if (dselprintf(2,180,p->cs->name) && !p->playing)
 choice=M_SERIESLIST;
      
 //draw frame
 if (!p->nf)  DrawString(font[0],screen,"No frames",2,190);   
  else
   {
    memset(screen+189*320+10,2,p->nf);
    screen[189*320+10+p->cf]=0xFF;
       
    DrawString(font[0],screen,"frame:",2,190);
    sprintf(s,"%2d/%2d",p->cf+1,p->nf);
    DrawString(font[1],screen,s,42,190);

    #define DXX 80
    short int *dptr=0;
    sprintf(s,"dX:%3.02f",((float)p->fptr->dx)/256);
    if (mx>DXX && mx<DXX+45)
      {
       DrawString(font[1],screen,s,DXX,190);
       dptr=&p->fptr->dx;
      } else DrawString(font[0],screen,s,DXX,190);
    
    sprintf(s,"dY:%3.02f",((float)p->fptr->dy)/256);
    if (mx>DXX+45 && mx<DXX+90)
      {
       DrawString(font[1],screen,s,DXX+45,190);
       dptr=&p->fptr->dy;
      } else DrawString(font[0],screen,s,DXX+45,190);
    
    sprintf(s,"dZ:%3.02f",((float)p->fptr->dz)/256);
    if (mx>DXX+90 && mx<DXX+90+45)
      {
       DrawString(font[1],screen,s,DXX+90,190);
       dptr=&p->fptr->dz;
      } else DrawString(font[0],screen,s,DXX+90,190);

    if (dptr && key)
     {
      if (key=='.') *dptr+=32;  if (key=='>') *dptr+=0x100;
      if (key==',') *dptr-=32; if (key=='<')  *dptr-=0x100;
      if (key=='?') *dptr=0;
     }
     
    //print dir
    sprintf(s,"dur:%2d",p->fptr->dur);
    if (mx>215 && mx<215+45)
      {
       DrawString(font[1],screen,s,215,190);
       if (key)
        {
         if (key=='.') p->fptr->dur+=1; if (key=='>') p->fptr->dur+=10;
         if (key==',') p->fptr->dur-=1; if (key=='<') p->fptr->dur-=10;
         if (key=='?') p->fptr->dur=1;
         if (p->fptr->dur>65000 || p->fptr->dur<1) p->fptr->dur=1;
        }
      } else DrawString(font[0],screen,s,215,190);
    
    if (verbose)
    {
    sprintf(s,"%db",p->fptr->size);
    DrawString(font[0],screen,s,285,180);
    }
//    sprintf(s,"fptr%d",((char *)p->fptr)-((char *)o->sd[p->cs].first));
//    DrawString(font[0],screen,s,160,180);
   } 
 
//if (key=='B') dprintf(font[1],2,160,"%dx %dy",p->x,p->y); 
} else  DrawString(font[0],screen,"No objects loaded",2,190);
}


//dprintf(font[0],0,60,"%d %d",intcount,voicecount);

//calculate fps
su++;
if (verbose)
{
static unsigned lastuu=0,fps;
if (!(su&127)) //check every 15 secs
 {
  fps=128*speed/(uu-lastuu);
  lastuu=uu;
 }
dprintf(font[1],150,175,"%dfps",fps);
}


//send to video
if (!gshakedur) MemoryCopy((char *)0xA0000,screen,64000);
           else MemoryCopy((char *)0xA0000,screen+gshake[uu&7],64000);



} while (!done);    

SetTimerFunc(0);


ModeText();

if (sbstat)
{
 shutdown_mixing();
 shutdown_sb();
 sbstat=0;
}

TerminateTimers();

terminate_keyboard();
TerminateGrip();
}




///****************************CHECK KEYS
void CheckKeys()
{

key=kscan=0;
if (!keyhit()) return;

kscan=getkey();
key=scan2ascii(kscan);
    
/*   if (playing)
    {
      p->cf=p->cs->cf;
      playing=0; p->resetfptr();
      p->shakedur=0;
    }*/

if (dragimg) //if image being dragged
 {
  if (kscan==83) //delete key, 83
      {
       resetshadow();
       _disable();
       p->fptr->size-=sizeof(image);
       p->fptr->numimages--;          //decrease image counter
       p->cs->deletebytes((char *)dragimg,sizeof(image)); //free memory for image
       dragimg=0;
       _enable();
     }
  if (key=='b')
   if (dragimg>p->fptr->getimgptr())
      {
       image t=*dragimg;
       memmove(dragimg,dragimg-1,sizeof(image) );
       dragimg--; *dragimg=t;
      }
  if (key=='x') dragimg->orient^=2;
  if (key=='y') dragimg->orient^=1;
  return;
 }         
    
if (p) //if object being edited
 {
  if (kscan==72 && p->csnum>0) {resetshadow(); p->startseries(p->csnum-1);}
  if (kscan==80 && p->csnum<p->od->nums-1) {resetshadow(); p->startseries(p->csnum+1); }
  if (kscan==0x4b && p->cf>0)
       {resetshadow(); p->cs->cf--; p->cf--; p->resetfptr();}
  if (kscan==0x4d && p->cf<p->nf-1)
      {resetshadow(); p->cs->cf++; p->cf++; p->resetfptr();}

  if (key=='I' && p->cs->nf>0) choice=M_INSERTAFTER;
  if (key=='i') choice=M_INSERTFRAME;
 }

 //captilize
 key=(char)toupper(key);
 if (key=='L') choice=M_NEWOBJECT;
// if (key=='O') *((char *)0x12345678)=0;
 if (key==27) choice=M_QUIT;
 if (key=='H') drawstat^=1;
 if (key=='V') {verbose^=1; mprintf("verbose %s",verbose ? "ON" : "OFF");}

 if (kbstat&KB_CTRL)
  {
   int oldspeed=speed;
   if (key=='-') if (speed>30) speed-=10;
   if (key=='+' || key=='=') if (speed<500) speed+=10;
   if (speed!=oldspeed)
    {
     SetTimerSpeed(speed);
     mprintf("timer speed: %d",speed);   
    }
  }      

if (p) //if object being edited
{
 if (key=='X') p->d^=1;
 if (key=='M') choice=M_SERIESLIST;
 if (key=='S') choice=M_SAVEOBJECT;
 if (key=='K') choice=M_KILLOBJECT;
 if (key=='U') if (p) mprintf("%X:%X:%X",p->prev,p,p->next);
 if (key==' ' && p->nf) p->effect(); 
 if (key=='B') {basex=mx; p->x=mx<<16; basey=my; p->y=my<<16;}
 if (key=='\t') //switch objects
      {
       if (o)
       {
        p->in=&inputnone; 
        if (p->next) p=p->next;
         else p=o;
        p->in=&inputdevice; //be sure to set its input device
       }
       else p=0;
       resetshadow();
      }


  if (key==']') //paste
         {
          _disable();
          switch (cliptype)
          {
           case EMPTY: break;
           case FRAMES:
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,clipsize);
             memcpy((char *)p->fptr,clipboard,clipsize);
             p->cs->nf+=clipnum; //insert frame in there
             p->nf=p->cs->nf;
             mprintf("copied %d frame(s), %d bytes",clipnum,clipsize);
             break;
           case IMAGES:
            {
             if (!p->nf) break;
             int imgsize=p->fptr->numimages*sizeof(image);
             p->cs->deletebytes((char *)p->fptr->getimgptr(),imgsize); //delete all images
             p->resetfptr();
             p->fptr->size-=imgsize;
             p->fptr->numimages=0; //clipnum;
            
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr->getimgptr(),clipsize);
             p->resetfptr();
             p->fptr->size+=clipsize;
             p->fptr->numimages=clipnum;
             memcpy((char *)p->fptr->getimgptr(),clipboard,clipsize);
             mprintf("pasted %d image(s), %d bytes",clipnum,clipsize);
            }
             break;
            
          }
          _enable();
         }         

      
 if (p->nf) //if frames exist      
 {   
  if (key=='E') choice=M_NEWEXTRA;
  if (key=='A') //activate object
   if (!p->playing)
   {
    p->in=&inputdevice;
    p->playing=1; p->active=1;
    p->startseries(0);     //activate object
    mprintf("%s activated",p->od->name);
   }
  if (key=='P') //start up playing
   if (!p->playing)
    {
     //p->x=basex; p->y=basey;   
     p->playing=1; p->active=0; p->playrepeat=0;                //play it
     if (kbstat&KB_SHIFT) p->playrepeat=1;
     p->startseries(p->csnum);   //start up animation
    }
        
  if (key=='D') //delete frame
   {
    _disable();
    p->cs->deletebytes((char *)p->fptr,p->fptr->size);
    p->cs->nf--; p->nf--; p->resetfptr();
    
    _enable();
   }

/*frames
  if (key=='[') //copy
      { clipsize=p->fptr->size; memcpy(clipboard,(char *)p->fptr,clipsize); }
  if (key==']' && clipsize) //paste
         {
          _disable();
          p->cs->deletebytes((char *)p->fptr,p->fptr->size);
          p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,clipsize);
          memcpy((char *)p->fptr,clipboard,clipsize);
          _enable();
         } */

  if (key=='[') //copy images
      { cliptype=IMAGES;
        clipnum=p->fptr->numimages;
        clipsize=clipnum*sizeof(image);
        memcpy(clipboard,(char *)p->fptr->getimgptr(),clipsize);
        mprintf("copied %d image(s), %d bytes",clipnum,clipsize);
      }

  if (key=='{') //copy frame
      { cliptype=FRAMES;
        clipnum=1;clipsize=p->fptr->size;
        memcpy(clipboard,(char *)p->fptr,clipsize);
        mprintf("copied frame %dbytes",clipsize);
      }

  if (key=='C') //copy whole series
   {
    cliptype=FRAMES;
    clipnum=p->cs->nf;
    clipsize=p->cs->size;
    memcpy(clipboard,p->cs->first,clipsize);
    mprintf("copied %s, %d frame(s), %d bytes",p->cs->name,clipnum,clipsize);
   }      

 } //p->nf==0



} //p
     


}













unsigned menudur=0xFFFFFFFF;
int DoMenu()
{
 
//if (p && p->playing) return -1;
if (!menustat)
{
 if (mx==0 && my<100) //left of screen
 {
   if (menudur==0xFFFFFFFF) menudur=uu+40;
 } else menudur=0xFFFFFFFF;

 if (menudur>uu) menustat=0;
     else menustat=1;
}
else //menu is on
{
if (mx>60 || my>100) {menustat=0;} //menu off
}

if (!menustat) return (choice);    



//draw menu
DrawBarMap(screen,SHADOWMAP,0,0,60,100);

int tempchoice=-1;
for (int i=0,y=0; menustr[i]; i++,y+=10)
 {
  if (my>y && my<y+10)
   {
    DrawString(font[1],screen,menustr[i],0,y); tempchoice=i;
   } else   DrawString(font[0],screen,menustr[i],0,y);
 }   

if ((mstat&1))
 {
  choice=tempchoice;
  menustat=0;
 // wait();
 }


return(choice);
}    




//free memory used by object definition
void objectdef::Kill()
{
if (refcount) refcount--; //decrease reference count
if (!loaded) return;  //if not loaded anyway, we dont care
if (refcount) return; //it's still needed

if (timerbusy) return; //can't kill if in timer


//-----------free object def from memory-------------

//ask for save
if (dirty)  {if (query("Changes were made to %s, save?",name)) SaveSeriesList();}

//free series definitions
if (sd) 
{
 for (int j=0; j<nums; j++) if (sd[j].first) free(sd[j].first);
 free(sd); 
}
//free sound definitions
if (sounds)
{
 for (int j=0; j<numsounds; j++) if (sounds[j].ptr) free(sounds[j].ptr);
 free(sounds); 
}


//free image definitions
if (id)
{
 for (int j=0; j<numi; j++) if (id[j].ptr) free(id[j].ptr);
 free(id); 
}

  
//clear pointers
sd=0; id=0; f=0; sounds=0;
nums=numi=numsounds=0;
mem=0; dirty=0;

loaded=0; //clear loaded flag

mprintf("%s killed",name); 
}

extern int memused;
//read object definition from disk    
void objectdef::Read()
{
refcount++; //increase reference count
if (loaded) return; //if already loaded, we dont care
if (timerbusy) {needload++; return;} //we can't load it if we are in the timer, set flag

//-------read objectdef into memory  
dirty=0; mem=0;
sd=0; nums=0; //no series
id=0; numi=0; //no images
sounds=0; numsounds=0; //no sounds

memused=0;
numi=ReadImageList(dir,"image.lst",&id); //read all images
if (numi==-1) return; //if error, break

numsounds=ReadSoundList(dir,"sound.lst",&sounds); //read all sounds
 
int tread=LoadSeriesList();  //read series list
if (tread==-1) return; //if error, break

mem=memused; //store amount of memory used
loaded=1; //set loaded

mprintf("%s loaded: %d series, %d sounds, %d images, %dK",name,tread,numsounds,numi,memused/1024);

}    



void object::resetfptr()
{
_disable();

fptr=cs->first;
if (!fptr) {dur=0; return;}

for (int i=0; i<cf; i++)  //skip ahead to frameptr
  fptr=(frame *) (((char *)fptr)+fptr->size);
dur=fptr->dur;
_enable();
}    


//inserts num bytes at p
char * series::insertbytes(char *p,int num)
{
if (num<=0) return p;
    
if (!first) //if no memory has been allocated
{
 first=(frame *)malloc(num); ///allocate memory
 memset(first,0,num); //clear memory
 size=num;
 ::p->od->dirty=1;

if (verbose) mprintf("created series with %dbytes",size); 

 return (char *)first;
} 

char *f=(char *)first;  //keep as char *
int pos=p-f;             //find relative position within series list (int)
if (pos<0 || pos>size+1)
   {sprintf(errstr,"invalid insertion ssize:%d pos:%d num:%d fptrsize:%d",size,pos,num,::p->fptr->size); cleanexit(-1);}


//get more memory
f=(char *)realloc(f,size+num);
memmove(f+pos+num,f+pos,size-pos);

if (verbose)
mprintf("inserted %d bytes at %d; size=%d",num,pos,size);

first=(frame *)f;
size+=num;
memset(f+pos,0,num); //clear memory

::p->od->dirty=1;

for (object *t=o; t; t=t->next)
 if (t!=::p) //find objects other than being edited
  {
   if (t->otype==::p->otype) //if other object of same type
     t->resetfptr(); //reset it
  }

return(f+pos);
}


//delete num bytes at p
void series::deletebytes(char *p,int num)
{
if (num<=0) return;
char *f=(char *)first;  //keep as char *
int pos=p-f;             //find relative position within series list (int)
if (!first || num>size || pos<0 || pos>=size)
   {sprintf(errstr,"invalid deletion %d %d",size,pos); cleanexit(-1);}

//get more memory
memmove(f+pos,f+pos+num,size-pos-num);

if (verbose) mprintf("deleted %d bytes at %d; size=%d",num,pos,size);

size-=num;
if (size>0)
 {
  first=(frame *)realloc(f,size+num);
 } else {first=0; free(f); size=0;}
::p->od->dirty=1;


for (object *t=o; t; t=t->next)
 if (t!=::p) //find objects other than being edited
  {
   if (t->otype==::p->otype) //if other object of same type
     t->resetfptr(); //reset it
  }


}


void object::drawmap()
{
if (!nf || !fptr) return;
_disable();
int tx=(x>>16);
int ty=(y+z)>>16;
register image *i=fptr->getimgptr();
int j=fptr->numimages;
_enable();

 if (!d)
 { 
  for (; j>0; j--)
  { 
   DrawImageMap(od->id[i->index].ptr,screen,tx+i->dispx,ty+i->dispy,i->orient);
   i++;
  }
 } else
 { 
  for (; j>0; j--)
  {
   img *iptr=od->id[i->index].ptr;
   DrawImageMap(iptr,screen,tx-i->dispx-iptr->xw,ty+i->dispy,0x2^i->orient);
   i++;
  }
 } 
 
}




//-----------------------------------------
//if high bit of etype set, extra only done on Hit
// sound

char *extraname[]=
{
 "Sound effect",
 "Ground Shake",
 "Object Shake",
 "New Object",
 0
} ;

//size of each extra structure...
int extrasize[]=
{
 sizeof(e_sound),
 sizeof(e_shake),
 sizeof(e_shake),
 sizeof(e_newobject),
 0
};    

int E_SOUND_draw(object *o,e_sound *t,int x,int y,int sel)
{
sounddef *s=&o->od->sounds[t->sound];    
dprintf(font[sel],x,y,"Sound #%02d: %9s %d.%02dsec",t->sound,s->name,s->ptr->soundsize/16000,(s->ptr->soundsize/(16000/100))%100);
return(sizeof(e_sound));
}
int E_SOUND_edit(object *o,e_sound *t)
{
int i=o->od->RequestSound();
if (i==-1) return 1;    
t->sound=(byte)i;
return 1;
}


int E_SHAKE_draw(object *o,e_shake *t,int x,int y,int sel)
{
 o=o;
 char *istr[]={"Weak","Violent","Orgasmic","Epileptic"};
     
 dprintf(font[sel],x,y,"%s  intensity: %d-%s",extraname[t->etype],t->intensity+1,istr[t->intensity]);
 return(sizeof(e_shake));
}

int E_SHAKE_edit(object *o,e_shake *t)
{
 o=o;
 t->intensity++;
 t->intensity&=3;
 return 1;
}


int E_NEWOBJECT_draw(object *o,e_newobject *t,int x,int y,int sel)
{
o=o;
char sername[32]="<none>";
if (odf[t->otype].loaded) strcpy(sername,odf[t->otype].sd[t->s].name);

dprintf(font[sel],x,y,"NewObject: %s %dX:%dY:%dZ %s",odf[t->otype].name,t->pos.x,t->pos.y,t->pos.z,sername);
return(sizeof(e_newobject));
}
static int dopos=0;
int E_NEWOBJECT_edit(object *o,e_newobject *t)
{

if (!dopos)
 if (!(kbstat&KB_SHIFT) && t->otype!=0) {dopos=3; wait(); return 0;}
     else dopos=1;

if (dopos==1)     
 {
  t->otype=RequestObject();
  if (!t->otype) return 1;
  t->s=0;  dopos=2;
  return 0;
 }
if (dopos==2)
{
 odf[t->otype].Read();   
 int ser=odf[t->otype].RequestSeries();
 odf[t->otype].Kill();
 if (ser==-1) {dopos=0; return 1;}
 t->s=ser; //store series
 dopos=3;
 return 0;
}     

if (dopos==3)
{
 t->pos.x=mx-(o->x>>16);
 if (!(mb&2))
  t->pos.z=my-(o->y>>16)-t->pos.y;
    else
  t->pos.y=my-(o->y>>16)-t->pos.z;
 if (!(kbstat&KB_SHIFT))
   if (t->pos.z>0) t->pos.z=0;
 if (mstat&1)
 {
  dopos=0;
  return 1;
 }
} 
return 0; //dont stop editing yet
}




//pointer to functions that draw the specified extra
//returns the size of the extra
int (*extradraw[])(object *,byte *,int,int,int)=
 {
  (int (*)(object *,byte *,int,int,int)) E_SOUND_draw,  //0
  (int (*)(object *,byte *,int,int,int)) E_SHAKE_draw,  //1
  (int (*)(object *,byte *,int,int,int)) E_SHAKE_draw,  //2
  (int (*)(object *,byte *,int,int,int)) E_NEWOBJECT_draw,  //3
  
 };

//pointer to functions that edit the specified extra
//returns 0 to continue editing, 1 to stop
int (*extraedit[])(object *,byte *)=
 {
  (int (*)(object *,byte *)) E_SOUND_edit,  //0
  (int (*)(object *,byte *)) E_SHAKE_edit,  //1
  (int (*)(object *,byte *)) E_SHAKE_edit,  //2
  (int (*)(object *,byte *)) E_NEWOBJECT_edit,  //3  
  0,
 };


//Extra stuff X's
void object::DrawExtraX()
{
byte *t=fptr->getextraptr();   //pointer to first option

for (int n=fptr->numextra; n>0; n--)
 {
  if (t==doedit || (uu&16))
  if (*t==E_NEWOBJECT) //has an X
   {
    e_newobject *e=(e_newobject *)t;
    DrawImage(X,screen,(x>>16)+e->pos.x-X->xw/2,(y>>16)+e->pos.y+e->pos.z-X->yw/2,0);
   }
  t+=extrasize[*t];
 }
}
//Extra stuff X's shadow
void object::DrawExtraXShadow()
{
byte *t=fptr->getextraptr();   //pointer to first option

for (int n=fptr->numextra; n>0; n--)
 {
  if (*t==E_NEWOBJECT) //has an X
   {
    e_newobject *e=(e_newobject *)t;
    DrawImage(Xshadow,screen,(x>>16)+e->pos.x-Xshadow->xw/2,(y>>16)+e->pos.y-Xshadow->yw/2,0);
   }
  t+=extrasize[*t];
 }

}


//Extra stuff
void object::DrawExtra()
{
if (doedit)
 {
//  wait();
  if (!extraedit[*doedit] || extraedit[*doedit](this,doedit))
   {doedit=0; return;}
 }

if (
    (cs->next!=0xFF && dselprintf(2,150,"after: %s",od->sd[cs->next].name)) ||
    (cs->next==0xFF && dselprintf(2,150,"after: suicide"))
   ) 
 {
  int t=od->RequestSeries();
  cs->next=t; //-1==suicide
 }

if (dselprintf(2,160,"cfunc: %s",cfuncnames[p->cs->cfunc]))
 {
  int t=RequestControlFunc();
  if (t!=-1)
   { cs->cfunc=t; controlfunc=object::cfunclist[t];} //get pointer to the control function.
 }


memset(screen+10*320,7,320);

if (!nf || !fptr) return;    

if (!fptr->numextra) return; //dprintf(font[0],2,10,"No extra options");
  else
{
byte *t=fptr->getextraptr();   //pointer to first option
int n=fptr->numextra;          //number of bytes of options

int x=2,y=10;
while (n>0)  //cycle through all options
 {
  int sel=(my>=y && my<y+10);
  if (sel && !doedit)
    {
     if (mstat&1) //they wanna edit this thing
      {
        doedit=t;
      }

     DrawBar(screen,1,0,y+1,320,9);
     if (kbscan==83) //delete key (they wanna delete this baby)
     {
      _disable();
      fptr->numextra--;
      fptr->size-=extrasize[*t];
      cs->deletebytes((char *)t,extrasize[*t]); //delete those bytes
      n--;
      _enable();
      kbscan=0;
      break;
     }
   
    }
    
  if (doedit)
   if (doedit==t)
    {sel=1; //else sel=0;
     memset(screen+y*320+320,4,320*9);    
    }
   else sel=0;
   
  if (!sel)
  t+=extradraw[*t](this,t,x,y+2,3); //draw thingee;
   else
  t+=extradraw[*t](this,t,x,y+2,1); //draw thingee;
  
  n--; //one less extra

  y+=10; //go down one line
  memset(screen+y*320,7,320);    
 }
}



 
}

void object::AddExtra(int i)
{
if (!nf || !fptr) return;
_disable();
fptr->size+=extrasize[i]; //increase frame size
fptr->numextra++;        //increase numextra

cs->insertbytes(((char *)fptr) + fptr->size -extrasize[i],extrasize[i]); //allocate bytes for it..
resetfptr();
byte *t=((byte *)fptr) + fptr->size -extrasize[i];
*t=i;                             //set extra type byte


od->dirty=1;
//doedit=t;
//if (i==E_NEWOBJECT) dopos=1;
//while(readmouse(&mx,&my));
//if (extraedit[*t]) extraedit[*t](this,t);

resetshadow();
_enable();
}    



void mprintf(char *format,...)
{
 char *arg=(char *)&format+sizeof(format);
 vsprintf(msg,format,&arg);
 msgdur=500;
}

//printf with selectability to char *screen
int dselprintf(int x,int y,char *format,...)
{
 char s[256];
 char *arg=(char *)&format+sizeof(format);
 vsprintf(s,format,&arg);
 int sel=3;
 if (my>=y && my<y+10 && mx>=x && mx<x+GetStringWidth(font[0],s) && !doedit && !dragimg) sel=1;

 if (sel==1 && (mstat&1)  && !menustat)
   {
//     wait();
    return 1;
   }
 DrawString(font[sel],screen,s,x,y);
 return 0;
}    

int query(char *format,...)
{
char s[128];

char *arg=(char *)&format+sizeof(format);
vsprintf(s,format,&arg);

char *t=(char *)malloc(64000);
int done=0,choice=0;;
//memcpy(screen,(char *)0xA0000,64000);
do
{
memcpy(t,screen,64000);

//mb=readmouse(&mx,&my);
refreshmouse();

DrawBarMap(t,SHADOWMAP,30,65,320-60,50);

DrawCenteredString(font[0],t,s,160,72);
if (mx<160) choice=1; else choice=0;
DrawCenteredString(font[choice],t,"Yes",80+10,87);
DrawCenteredString(font[choice^1],t,"No",320-(80+10),87);

DrawImage(cursor,t,mx,my,0);  //draw mouse
MemoryCopy((char *)0xA0000,t,64000);

if (keyhit())
 {
  char key=toupper(scan2ascii(getkey()));
  if (key=='Y') {choice=1; done=1;}
  if (key=='N' || key==27) {choice=0; done=1;}
 }

if (mstat) done=1; 
} while(!done);
free(t);

//while(readmouse(&mx,&my));

return choice;
}    


void wait()
{

while (keyhit()) getkey();
do
{
 mb=readmouse(&mx,&my);   
} while (mb);
mstat=0;
}    

