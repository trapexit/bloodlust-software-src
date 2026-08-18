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
#include "smix.h"
#include "vol.h"


#include "effect.h"
#include "object.h"

#include "bg.h"

#include "adialog.h"



extern int regionchecks,regionintersects,seriesstarts;
extern int regiontest,rpush,rsolid;
extern lpoint regionpoint;


 void drawarrow(int stat,int x,int y);
void drawintersect(int stat,int x,int y); 

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
int mx,my,mb,mstat=0,mrelstat=0,oldmb=-1; //mouse x,y,buttons
int basex=160,basey=160;
char key=0,kscan=0;

img *arrow[13];

int drawregions=0; //dont draw regions by default


//dialogs  
dialog dlg;


//int speed=100;
int editmotion=0,forcemotionedit=0;

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

//control function names
char cfuncnames[256][32];
int cfuncnum;
char rfuncnames[256][32];
int rfuncnum;

//object definitions
int numodf=0;
objectdef *odf;
volatile int needload=0;   //flag set when object definition needs loading
volatile int needdelete=0; //flag set when object instance needs deleting

//background definitions
int numbgdf=0;
backgrounddef *bgdf;

//list of all objects
object *o=0;

//current background
bgobject *bgobj=0; //none at first


//object that is being edited
object    *p=0; //current object being edited (none)
char      pshit[sizeof(object)];
object    *pshadow=(object *)&pshit; //(0,0,0,0,0,0,0); //shadow of p

//editing mode
enum {EM_IMAGE,EM_REGION,EM_OBJECTS,EM_EFFECT,EM_SERIES} emode=EM_IMAGE;
static byte *doedit=0;

//series editor
series *seriesedit=0;

//image dragging stuff
image *dragimg=0;
lpoint dragrel;

//clipboard
enum {EMPTY,IMAGES,FRAMES,EFFECTS} cliptype=EMPTY;
int clipsize=0; //size in bytes in buffer
int clipnum=0; //number of images/frames in buffer
char clipboard[500];
objectdef *clipobj; //the objectdef that the shit was copied from



int findimage(objectdef *a,objectdef *b,int index)
{
 char *s=a->id[index].name; //get pointer to source image name

 for (int i=0; i<b->numi; i++)
  if (!stricmp(s,b->id[i].name))  return i;
 return index;      //not found
}

void convertimage(image *sptr,image *dptr,objectdef *srcobj,objectdef *destobj,int numimages)
{
 memcpy(dptr,sptr,numimages*sizeof(image)); //copy over images
 if (srcobj==destobj) return;
 
 for (int i=0; i<numimages; i++,sptr++,dptr++) //go through all images
   dptr->index=findimage(srcobj,destobj,sptr->index); //get image number
}    


void convertframe(frame *source,frame *dest,objectdef *srcobj,objectdef *destobj)
{

 memcpy(dest,source,source->size); //copy over frame
 if (srcobj==destobj) return;

 //convert images
 convertimage(source->getimgptr(),dest->getimgptr(),srcobj,destobj,source->numimages);
}

void convertframes(frame *source,frame *dest,objectdef *srcobj,objectdef *destobj,int num)
{
 for (int i=0; i<num; i++)
  {
   convertframe(source,dest,srcobj,destobj);
   dest=(frame *)(((char *)dest)+source->size);   
   source=(frame *)(((char *)source)+source->size);
  }
}



void hline(int x,int y,int xw,char color)
{
 memset(screen+x+y*320,color,xw);
}    


//*******************************************
//***************menu stuff******************
//*******************************************
class menuitem
{
public:
 char *text;     //text for this menu item
 void (*func)(); //function to call
 char key;       //shortcut key
 class menu *submenu;  //submenu

 void draw(int x,int y,int sel); //returns true if selected
 int width(); //finds width of this item
} menuitem;

int menuitem::width()
{
 if (!text) return 5;
 return GetStringWidth(font[0],text)+10;
}

void menuitem::draw(int x, int y, int sel)
{
 DrawString(font[sel],screen,text,x,y);
}    

//menu
class menu
{
public:
class menuitem *msel; //currently selected menu item
int xw,yw; //width/height of menu
char orient; //1horiz 0vertical
class menuitem m[];

void draw(int x,int y);
void drawhoriz(int x,int y);
void getwidthheight();

class menuitem *checkkey(char key);
};
void menu::getwidthheight()
{
 class menuitem *t;
 if (!orient) //vertical
 {
  if (!xw) { for (t=m; t->text; t++) if (t->width()>xw) xw=t->width();  xw+=6;}//find width
  if (!yw) { for (t=m; t->text; t++) yw+=10; }    //find height
 } else
 {
  if (!xw) { for (t=m; t->text; t++) xw+=t->width();  }//find width     
  yw=10;
 }
}
int onmenu=0;
void menu::draw(int x,int y)
{
 if (!this) return;    
 int sel;
 getwidthheight();

 DrawBarMap(screen,SHADOWMAP,x,y,xw,yw);
 msel=0; 
 for (class menuitem *t=m; t->text; t++)
  {
   sel=(my>=y && my<y+10 && mx>x && mx<x+xw && !dragimg && !dlg.active);
   if (sel)
    {
     DrawBar(screen,2,x,y,xw,10);
     t->draw(x+3,y,1);
     if (mb&1) msel=t; //this has been selected
     onmenu=1;
    } else  t->draw(x+3,y,0);
   DrawLetter(font[2],screen,t->key,x+xw-8,y);
   y+=10;  
  }
  
}    

void menu::drawhoriz(int x,int y)
{
 if (!this) return;
 int sel;
 getwidthheight();
 DrawBarMap(screen,SHADOWMAP,x,y,xw,yw);
 for (class menuitem *t=m; t->text; t++)
  {
   sel=(my>=y && my<y+10 && mx>x && mx<x+t->width());
   if (sel) onmenu=1;
   sel=sel && !dragimg && !dlg.active;
   if (sel)
    {
     DrawBar(screen,2,x,y,t->width(),10);
     t->draw(x+3,y,1);
     if (msel)
     {
      if (mb&1) msel=t; //this has been selected
     } else if (mstat&1) msel=t; //this has been selected
    } else  t->draw(x+3,y,0);
   if (msel==t && !dragimg) msel->submenu->draw(x,y+10); 
   x+=t->width(); //horizontal
  }

  //if one of them IS selected
 if (msel)
  {
   if (!(mb&1)) msel=NULL;
//   mb=0; mstat=0;
  }
}    


class menuitem *menu::checkkey(char key)
{
 if (!this) return 0;
 for (class menuitem *t=m; t->text; t++)
  {
   if (t->key==key) return t;
   class menuitem *r=t->submenu->checkkey(key);
   if (r) return r;
  }
return 0; 
}    

//int menustat=0;

extern menu mainmenu;



void drawpixel(int x,int y, char color)
{
 screen[x+y*320]=color;
}    





int random(int x)
{
 return((x*rand())/32768);
}

//reads object names from the object definition file
void ReadObjectNames(char *onf);
void ReadBackgroundNames(char *bnf);

int ReadImageList(char *list,imagedef **imagedefs);
int ReadSoundList(char *list,sounddef **sounddefs);
//int RequestExtra();
int ReadControlFuncs(char *cfile,char cfnames[256][32]);

//int RequestControlFunc();

//requests the user to select an object from a objectdeflist
//int RequestObject();
int DoMenu();
void CheckKeys();


#define BW 12
int DrawButton(int x,int y,char character, int c,int c2,int highlight,char *tooltip)
{
char s[10];    
DrawBar(screen,c,x,y,BW,12);
if (highlight)
{ DrawBar(screen,c2,x+1,y+1,BW-2,12-2);}

 s[0]=character; s[1]=0;
 DrawCenteredString(font[highlight ? 1 : 3],screen,s,x+BW/2,y+2);

//if withing bounds
if (mx>x && mx<x+BW && my>y && my<y+12)
 {
//  DrawString(font[2],screen,tooltip,260,12);
  if  ((mstat&1) && !dragimg) return 1;
 }

return 0;


}

image *object::CheckImageBound()
{
image *found=0;
if (!fptr || killme) return 0;
    
register image *i=fptr->getimgptr();    
for (j=fptr->numimages; j>0; j--) //
 {
  img *a=od->id[i->index].ptr; //get pointer to this image
  if (mx>(x>>16)+i->dispx && my>((y+z)>>16)+i->dispy && mx<(x>>16)+i->dispx+a->xw && my<((y+z)>>16)+i->dispy+a->yw)
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
  dragrel.y=((p->y+p->z)>>16)-my+((int)found->dispy);
  
 }

return(found);     
}    


volatile unsigned uu=0;
volatile unsigned su=1;

//reset shadow to look like last frame of current char
void resetshadow()
{
 if (p && !p->playing)
      
       *pshadow=*p;
     else {pshadow->fptr=0; pshadow->onum=0; pshadow->od=0;}
}


//standard timer tick
void timer()
{
 uu++;
 if (msgdur>0) msgdur--;
 if (gshakedur>0) gshakedur--;

 //refresh timered input
 refreshinputtimer();
 //read device from latches
 inputdevice.read();

regionpoint.x=0;
 //tick all playing objects
 for (object *t=o; t; t=t->next)
 if (t->playing)
 {
  t->tick(); //tick the object
  t->in->reset(); //done with the triggered input
  if (!t->playing && t==p) resetshadow();
 }
  

}    


//standard key function
int keyboard()
{

//refresh the input device if keyb  
if (refreshinputkeyboard() && (editmotion || (p && p->playing))) return 1; //dont add key to queue

    //if key pressed stop playing
if (p && p->playing) //if selected object is playing
 {
  if (kbscan==15) //tab
    return 0;     //add it to queue

   //if certain key hit, stop playing of this selected object 
  if (kbscan==0x1C || kbscan==0x39 || kbscan==1)  //return,space,enter
  {
   if (kbstat&KB_CTRL)
    {
     for (object *t=o; t; t=t->next) t->stop();
    } else p->stop();
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

void M_NEWIMAGE();
void M_DELETEIMAGE();

void refreshmouse()
{
oldmb=mb;
mb=readmouse(&mx,&my);
mstat=mb&(oldmb^0x7); //press triggers
mrelstat=oldmb&(mb^0x7); //release triggers
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
}



void createcolormap(color *pal,unsigned char *map,int r,int g,int b)
{
   
for (i=0; i<256; i++) //do shadow map
 {
  unsigned char tr,tg,tb;
  tr=((unsigned)pal[i].r)*r/8;  tg=((unsigned)pal[i].g)*g/8;  tb=((unsigned)pal[i].b)*b/8;   

  unsigned int closeness=9999,closecolor=0,k;
  for (int j=0; j<256; j++)
    {
      k=abs(tr-pal[j].r)+abs(tg-pal[j].g)+abs(tb-pal[j].b);
      if (k<closeness) {closeness=k; closecolor=j;}
    }
  map[i]=closecolor;
 }
}



int donea32=0;

bgmap testmap;

void bgtest()
{
 int x,y,idx;
 do
 {
  scanf("%d %d %d",&x,&y,&idx);

  testmap.addpoint(x,y,idx);
  testmap.print();
 } while (1);
}    

void drawobjects();

void main(int argc, char *arg[])
{
//bgtest();
//return;
    
getmeminfo();
printf("%dK available\n",MemInfo.NumFreePagesAvail*4);
//printf("object:    %d\n",sizeof(object));
//printf("objectdef: %d\n",sizeof(objectdef));


cfg=load_config("a32.cfg");
if (!cfg) {printf("Error: config file not found. Run a32setup.\n"); exit(1);}

ids=&cfg->ids; //copy over input settings


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

 
//vdiagnose=1;    
v.open("a32.vol");
pal=(color *)v.read();
bg=(char *)v.read();
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


//read arrows
for (i=0; i<13; i++)
{
 arrow[i]=(img *)v.read();
 FuckWithImage((char *)arrow[i],0,15);
}

v.close();
screen=(char *)malloc(70000)+2000;
memset(screen-2000,0,70000);

//create grey map for shadow
createcolormap(pal,SHADOWMAP,6,6,6);

/*
//create region maps
createcolormap(pal,regmap[0],1,8,1);
createcolormap(pal,regmap[1],1,1,8);
createcolormap(pal,regmap[2],8,1,1);
*/



//read control functions
cfuncnum=ReadControlFuncs("cfunc.lst",cfuncnames);
rfuncnum=ReadControlFuncs("rfunc.lst",rfuncnames);

//read object list
ReadObjectNames("object.lst");
ReadBackgroundNames("bg.lst");

//mprintf("%d object definitions. %d control funcs. %dK available.",numodf-1,cfuncnum,MemInfo.NumFreePagesAvail*4);





//start up soundblaster
sbstat=0;
if (cfg->soundcard!=CFG_NOSOUND)
if (!init_sb(cfg->sbport,cfg->sbirq,cfg->sbdma,cfg->sbdma16))
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
      if (odf[j].onum==x) break;
     if (j!=numodf)
      { 
       object *t=new object(&odf[j],-1,basex,0,basey,0,0); //create an instance of the object
       t->startseries(0);
       if (!t->od->loaded) {free(t); break;}
       if (p) t->x=p->x+(40<<16);   

       //add object to beginning of object list
       o->prev=t; t->prev=0; t->next=o; p=o=t;   
       resetshadow();
      }
   }
 }

//start up keyboard handler
set_keyboard_func(keyboard);
SetTimerSpeed(speed);
SetTimerFunc(timer);


donea32=0;

do
{

//load needed objects
if (needload)
 {
  _disable();     
  for (int i=0; i<numodf; i++)
   if (!odf[i].loaded && odf[i].refcount)  //not loaded, but referenced
    {
     odf[i].Read(); //read it into memory
     object *t=o;
     while (t)
     {
      object *next=t->next;
      if (t->od==&odf[i])
      if (!odf[i].loaded) delete t; //if not loaded still, was error, delete all referenced objects
       else //was loaded okay, bind to objectdef
         t->startseries(t->csnum); //start series back up (set fptr)
      t=next;
     }
    }
 _enable();     
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
      if (p==t) p=NULL;
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
refreshinputmain();

//clear screen
memcpy(screen,bg,64000);

object *t;

drawobjects();

//do object switching on mouse click
if ((mstat&1) && !doedit && !dragimg && !onmenu && !dlg.active ) //if mouse button hit
{
 object *clicked=0;
 for (t=o; t; t=t->next)
   if (t!=p) //if not being edited
     if (t->CheckImageBound()) clicked=t; //it was clicked on

 if (clicked)
   {
    p=clicked; resetshadow(); 
    if (p->active) inputdevice.bind(p); //bind input device to new object if it is active
    mstat=0;
   }

}
     






//IF OBJECT SELECTED 
if (p)
{
if (p->fptr) //and frame exists
{

//do editing
if (!mainmenu.msel && !p->playing && !doedit && !onmenu && !dlg.active)
switch (emode)
{
case EM_IMAGE:
   if (!dragimg) //if nothing is dragged
    if (mstat&1) //and a button is clicked...
     {
      dragimg=p->CheckImageBound(); //see if image clicked on
      if (dragimg) p->od->dirty++; //it dirty now
     }

   if (dragimg) //image IS being dragged
    if (mb&1) //while button is held, change img coordinate
      {
       if (!p->d) dragimg->dispx=(short)(dragrel.x+mx-(p->x>>16)); //move image
            else  dragimg->dispx=(short)(dragrel.x-(mx-(p->x>>16))); //move image
       dragimg->dispy=(short)(dragrel.y+my-((p->y+p->z)>>16));
       
       dprintf(font[1],2,170,"%s %ddx %ddy",p->od->id[dragimg->index].name,dragimg->dispx,dragimg->dispy);
      } else dragimg=0; //if button let go, stop dragging

   if (mstat&2)
       if (!dragimg) M_NEWIMAGE(); //choice=M_NEWIMAGE; //new image!
       else M_DELETEIMAGE();

  break;
  
case EM_EFFECT:
/*add new extra perhaps
if ((mstat&2) && !sel)
  {
    choice=M_NEWEXTRA;
    wait();
   }
*/
  break;

}

//draw buttons and change emodes
if (drawstat)
{
if (DrawButton(280,0,emode==0 ? (p->fptr->numimages+'0') : 'I',6*16+15,6*16+6,emode==0,"images")) emode=EM_IMAGE;
if (DrawButton(280+BW*1,0,emode==3 ? (p->fptr->numextra+'0') : 'E',1*16+15,1*16+6,emode==3,"effects")) emode=EM_EFFECT;
if (DrawButton(280+BW*2,0,'S',6*16+15,6*16+6,emode==4,"series")) emode=EM_SERIES;
}
}


//we are editing background
if (p==bgobj) 
{
//draw buttons and change emodes
if (drawstat)
{
 if (DrawButton(280,0,'I',6*16+15,6*16+6,emode==EM_IMAGE,"images")) emode=EM_IMAGE;
 if (DrawButton(280+BW*1,0,'R',1*16+15,1*16+6,emode==EM_REGION,"regions")) emode=EM_REGION;
 if (DrawButton(280+BW*2,0,'O',6*16+15,6*16+6,emode==EM_OBJECTS,"objects")) emode=EM_OBJECTS;
}
}    


}

//check keys
CheckKeys();

//check menu
if ((mrelstat&1) && mainmenu.msel && mainmenu.msel->submenu->msel)
 {
   if (mainmenu.msel->submenu->msel->func) mainmenu.msel->submenu->msel->func();
   mainmenu.msel->submenu->msel=0;
   mainmenu.msel=0;
 }





//draw the status screen edits
//if (o) //there is an object loaded
//{
if (p)
{
 if (!dragimg && !dlg.active && !seriesedit)
 {
  if (((mb&4) || keydown[44]))
   {
    basex=mx; p->x=mx<<16; basey=my; p->z=(my<<16)-p->y; p->updated|=7;
    dprintf(font[1],2,170,"%ddx %ddy %ddz",p->x>>16,p->y>>16,p->z>>16);
   }

  if (keydown[48])
   {
    basex=mx; p->x=mx<<16; p->y=(my<<16)-p->z;
    if ((kbstat&KB_SHIFT)) p->y=0; p->updated|=7;
    dprintf(font[1],2,170,"%ddx %ddy %ddz",p->x>>16,p->y>>16,p->z>>16);
   }
 }
 if (!mainmenu.msel && !dlg.active)
 {
  if (emode==EM_EFFECT && !p->active)    p->DrawExtra();
  if (emode==EM_SERIES)    p->DrawSeries();
 }
 if (drawstat)           p->DrawStatus();
}  else  DrawString(font[0],screen,"No object selected",2,190);
//}  else  DrawString(font[0],screen,"No objects loaded",2,190);

//if (my<10 || mainmenu.msel)
onmenu=0;
if (drawstat)
mainmenu.drawhoriz(0,0);



//draw message at top
if (msgdur && !dragimg)// && !mainmenu.msel)
 DrawString(font[2],screen+170*320,msg,0,0);

//draw the dialog box
dlg.draw();

//draw mouse crusor
if (!dragimg && !doedit && !seriesedit)  DrawImage(cursor,screen,mx,my,0);  


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
dprintf(font[1],280,14,"%dfps",fps);

dprintf(font[1],0,90,"%d %dX:%dY p:%d s:%d",regiontest,regionpoint.x,regionpoint.y,rpush,rsolid);
dprintf(font[1],0,100,"rchecks:%d rinter:%d sstarts:%d",regionchecks,regionintersects,seriesstarts);

_disable();
int numr=0,numa=0,numv=0;;
for (region *r=rlist[R_IMPERM]; r; r=r->next) numr++;
for (r=rlist[R_ATTACK]; r; r=r->next) numa++;
for (r=rlist[R_VULN]; r; r=r->next) numv++;
_enable();

dprintf(font[1],0,110,"numr:%d numa:%d numv:%d",numr,numa,numv);

for (object *t=o; t; t=t->next)
 drawintersect(t->intersect,t->x>>16,(t->y+t->z)>>16);
}








//send to video
if (!gshakedur) MemoryCopy((char *)0xA0000,screen,64000);
           else MemoryCopy((char *)0xA0000,screen+gshake[uu&7],64000);



} while (!donea32);    

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

void drawobjects()
{
object *t;

//sort characters
_disable();
for (t=o; t; t=t->next) t->resort();
_enable();


//draw shadows
for (t=o; t; t=t->next) //cycle through each object
if (t->fptr)  //if frame exists
if(!(t->cs->parm&SP_NOSHADOW))     //and shadow exists
{
 if (t==p)
   DrawImage(shadow,screen,(t->x>>16)-shadow->xw/2,(t->z>>16)-shadow->yw/2,0);
 else
  DrawImageMap(shadow,screen,(t->x>>16)-shadow->xw/2,(t->z>>16)-shadow->yw/2,0);
}
 else //draw dot shadow
{
 if (!p->active)
 if (uu&16) drawpixel(t->x>>16,t->z>>16,3);
}

//draw shadow map character
if (!p->playing && pshadow->od->loaded) {pshadow->drawmap();}


//draw X shadow behind (if even exists)
if (p && emode==EM_EFFECT && !p->active) p->DrawExtraXShadow();

//draw objects
int numobjs=0;
for (t=o; t; t=t->next)
{
 if (!t->active)
 {
  if (t->x<(-20<<16)) t->x=340<<16; //scoot it back if it goes offscreen
  if (t->x>(340<<16)) t->x=-20<<16;
 }

 t->draw(); //draw the object
 if (drawregions) t->DrawRegions();
 
 numobjs++;
}
if (verbose)
 dprintf(font[1],250,140,"numobjs:%d",numobjs);


//draw X in front (if even exists)
if (p && emode==EM_EFFECT && !p->active) p->DrawExtraX();
}    






void dlg_loadobject(objectdef *odefs,int i,void *context)
{
 object *t=new object(&odf[i],-1,basex,0,basey,0,0); //create an instance of the object
 t->startseries(0);
 if (!t->od->loaded) //error creating object
     {free(t); return;} //free it and abort

 //if previous object there
 if (p)  t->x=p->x+(65<<16); 
 if (t->x<(-20<<16)) t->x=340<<16; //scoot it back if it goes offscreen
 if (t->x>(340<<16)) t->x=-20<<16;


 //add object to beginning of object list
 o->prev=t;
 t->prev=0; t->next=o;
 p=o=t;

 resetshadow();
}
void M_LOADOBJECT()
{
 rect r={80,20,320-80,170};
 dlg.init(&r,"Load object",odf,sizeof(objectdef),numodf,0,
    (void *)dlg_objectdef,(void *)dlg_loadobject,0);
}    



void dlg_loadbackground(backgrounddef *bgdefs,int i,void *context)
{
 if (bgobj) delete bgobj;
  //create bgobject
 bgobj=new bgobject(&bgdf[i]);
 p=bgobj;
}

void M_EDITBG() {p=bgobj;}
void M_LOADBG()
{
 rect r={80,20,320-80,170};
 dlg.init(&r,"Load background",bgdf,sizeof(backgrounddef),numbgdf,0,
    (void *)dlg_objectdef,(void *)dlg_loadbackground,0);
}    
void M_VOLUMIZEBG()
{

}    




void dlg_listseries(series *ser,int i,object *context)
{
 context->startseries(i);
}
void M_SERIESLIST()
{
   if (!p || p->playing) return;
   resetshadow();
 char s[50];
 sprintf(s,"Select %s Series",p->od->name);

 rect r={50,15,320-50,170};
 dlg.init(&r,s,p->od->sd,sizeof(series),p->od->nums,p->csnum,
    (void *)dlg_series,(void *)dlg_listseries,p);
}


void M_SAVESDF()
{
  if (!p) return;
  p->od->SaveSDF();
  mprintf("%s saved",p->od->sdf);
}    


void M_KILLOBJECT()
  {
   if (!p) return;
   _disable();
   object *t=p;
   if (p->next) {p=p->next; delete t;}
    else
   if (p->prev) {p=p->prev; delete t;}
    else {delete t; p=o;}
   resetshadow();
   
   _enable();
  }


void M_NEWIMAGE()
  {
      if (!p || p->playing || !p->fptr || dragimg) return;
      emode=EM_IMAGE;
      p->resetfptr();
      i=p->od->RequestImage();
      if (i!=-1)
        {
         _disable();
         image *im=p->fptr->getimgptr()+p->fptr->numimages; //get pointer to image locations
         
         p->fptr->size+=sizeof(image);
         p->fptr->numimages++;          //increase image counter
         im=(image *)p->cs->insertbytes((char *)im,sizeof(image)); //get memory for new image
         p->resetfptr();
         im->index=(unsigned char)i;  //store image index
         im->orient=0;
         im->dispx=0; im->dispy=0;

         dragimg=im; //start draggin it!
         _enable();
        }
  }

void M_DELETEIMAGE()
 {
    if (!p || !dragimg) return;
       resetshadow();
       _disable();
       p->fptr->size-=sizeof(image);
       p->fptr->numimages--;          //decrease image counter
       p->cs->deletebytes((char *)dragimg,sizeof(image)); //free memory for image
       dragimg=0;
       _enable();
       kbscan=key=0;
       mstat=0;

 }  

//       if (!p->d) dragimg->dispx=(short)(dragrel.x+mx-(p->x>>16)); //move image
//            else  dragimg->dispx=(short)(dragrel.x-(mx-(p->x>>16))); //move image
//       dragimg->dispy=(short)(dragrel.y+my-(p->y>>16));

//  if (!d) dragrel.x=(p->x>>16)-mx+((int)found->dispx);
//   else dragrel.x=-((p->x>>16)-mx)+((int)found->dispx);
//  dragrel.y=(p->y>>16)-my+((int)found->dispy);

///     emode=EM_EFFECT;
///     int i=RequestExtra();
///     if (i==-1) return;
///     wait();


void dlg_addextra(char **extraname,int i,object *context)
{
 context->AddExtra(i);
}
  
extern char *extraname[];
int ExtraCount()
{
for (int i=0; extraname[i]; i++);
return i;
}    
void M_NEWEFFECT()
{
  if (!p || p->playing || !p->fptr) return;
  {
    rect r={50,15,320-50,170};
    dlg.init(&r,"Add Effect",extraname,4,ExtraCount(),-1,
      (void *)dlg_extra,(void *)dlg_addextra,p);
      emode=EM_EFFECT;
  }
}

void M_INSERTFRAME()
  {
       if (!p || p->playing) return;
        _disable();
        p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,sizeof(frame));
        p->cs->nf++; p->nf++; //p->resetfptr();
        p->fptr->size=sizeof(frame); //reset frame
        p->fptr->dur=10;
        _enable();
  }

void M_INSERTAFTER()
  {
   if (!p || p->playing || !p->cs->nf) return;
      p->cs->cf++; p->cf++; p->resetfptr();
    M_INSERTFRAME();
     p->cs->cf--; p->cf--; p->resetfptr();
  }

void M_SAVEOBJECT()
 {
  if (!p) return;
  _disable();
  p->od->Save();
  _enable();
 }

void M_QUIT()
 {
  if (dlg.active) {dlg.active=0; return;}
  while (o)
  {
   delete o;
  }
  donea32=1;
 }


void M_ACTIVATEOBJECT()
{
if (!p) return;

//delete all objects created by this one
 for (object *t=o,*next; t; t=next)
   {
    next=t->next;
    if (t->creator==p) delete t;
   }

 inputdevice.bind(p);
 p->in->reset();
 p->activate(0);
 
 mprintf("%s activated",p->od->name);
}


void M_ACTIVATEALL()
{
if (p)
{
 inputdevice.bind(p);
 p->in->reset();
}    

//delete all objects created by other ones
for (object *t=o,*next; t; t=next)
   {
    next=t->next;
    if (t->creator) delete t;
   }


for (t=o; t; t=t->next)
 t->activate(0);

 mprintf("all activated");
}


void M_RESETALL()
{
for (object *t=o; t; t=t->next)
    t->z=160<<16;    
 M_ACTIVATEALL();

 mprintf("all reset");
}



void M_PASTE()
{
 if (!p) return;
 _disable();
  switch (cliptype)
          {
           case EMPTY: break;
           case EFFECTS:
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr->getextraptr(),clipsize); //insert bytes
             p->resetfptr();
             p->fptr->size+=clipsize;
             p->fptr->numextra+=clipnum;
             memcpy((char *)p->fptr->getextraptr(),clipboard,clipsize);
             mprintf("pasted %d effects(s), %d bytes",clipnum,clipsize);

             break;
           case FRAMES:
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,clipsize);
      //       memcpy((char *)p->fptr,clipboard,clipsize);
            convertframes((frame *)clipboard,p->fptr,clipobj,p->od,clipnum);

             p->cs->nf+=clipnum; //insert frame in there
             p->nf=p->cs->nf;
             mprintf("pasted %d frame(s), %d bytes",clipnum,clipsize);
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
             //memcpy((char *)p->fptr->getimgptr(),clipboard,clipsize);
             mprintf("pasted %d image(s), %d bytes",clipnum,clipsize);

             convertimage((image *)clipboard,p->fptr->getimgptr(),clipobj,p->od,clipnum);

            }
             break;
            
          }
 _enable();

} 

void M_FLIPXOBJECT()
{
 if (p) p->d^=1;
}



void M_STOP()
{
 if (!p || !p->playing) return;
 p->stop();
 resetshadow();
}    

void M_PLAY()
{
 if (p && p->fptr) p->play(p->csnum);
}    

void M_PLAYLOOPED()
{
 if (p && p->fptr) p->playlooped(p->csnum);
}    

void M_DELETEFRAME()
{
 if (p && p->fptr && !p->playing)
 {
 _disable();
  p->cs->deletebytes((char *)p->fptr,p->fptr->size);
  p->cs->nf--; p->nf--; 
  if (p->cs->cf>=p->cs->nf && p->cf>0) {p->cs->cf--; p->cf--;}
  p->resetfptr();
 _enable();
 }
}

void M_COPYEFFECTS()
{
if (!p || !p->fptr) return;
        cliptype=EFFECTS;
        clipnum=p->fptr->numextra;
        clipsize=p->fptr->size-sizeof(frame)-sizeof(image)*p->fptr->numimages; //get size of effects
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr->getextraptr(),clipsize);
        mprintf("copied %d effect(s), %d bytes",clipnum,clipsize);
}


void M_COPYIMAGES()
{
if (!p || !p->fptr) return;

if (emode==3) //effect
 { M_COPYEFFECTS(); return;}
        cliptype=IMAGES;
        clipnum=p->fptr->numimages;
        clipsize=clipnum*sizeof(image);
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr->getimgptr(),clipsize);
        mprintf("copied %d image(s), %d bytes",clipnum,clipsize);
}

void M_COPYFRAME()
{
if (!p || !p->fptr) return;
       cliptype=FRAMES;
        clipnum=1;clipsize=p->fptr->size;
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr,clipsize);
        mprintf("copied frame %dbytes",clipsize);
}

void M_COPYSERIES()
{
if (!p || !p->fptr) return;    
    cliptype=FRAMES;
    clipnum=p->cs->nf;
    clipsize=p->cs->size;
    clipobj=p->od;    
    memcpy(clipboard,p->cs->first,clipsize);
    mprintf("copied %s, %d frame(s), %d bytes",p->cs->name,clipnum,clipsize);
}      


void M_VERBOSE() {verbose^=1;}
void M_DRAWSTAT() {drawstat^=1;}

void M_200SPEED() {speed=200; SetTimerSpeed(speed);}
void M_150SPEED() {speed=150; SetTimerSpeed(speed);}
void M_100SPEED() {speed=100; SetTimerSpeed(speed);}
void M_50SPEED() {speed=50; SetTimerSpeed(speed);}
void M_25SPEED() {speed=25; SetTimerSpeed(speed);}


void M_EDITMOTION() {if (!p) return; emode=EM_SERIES; forcemotionedit=1;}

void M_OBJECTINFO()
{
 if (!p) return;
 mprintf("%dseries %dwavs %dimages %dK",p->od->nums,p->od->numsounds,p->od->numi,p->od->mem/1024);
}    

void M_VOLUMIZE()
{
 if (p) p->od->volumize();
}

void M_RENAMESERIES()
{
 if (!p || !p->fptr) return;
 seriesedit=p->cs; //edit this series
}

void M_DRAWREGIONS()
{
 drawregions++;
 drawregions%=3;
}    


void M_ADDNEWSERIES()
{
 if (!p) return;
  //add new series
 p->od->addnewseries();

  //go to it
 p->startseries(p->od->nums-1);
}

void M_DELETELASTSERIES()
{
 if (!p) return;

 p->od->nums--;
 if (p->csnum>=p->od->nums)
  p->startseries(p->od->nums-1);
}


void objectdef::addnewseries()
{
  //add new series at end
 sd=(series *)realloc(sd,(nums+1)*sizeof(series));
  //set to zero
 memset(&sd[nums],0,sizeof(series));

 sprintf(sd[nums].name,"series %d",nums);
 nums++;

}    


//********************************
//********************************
//********************************
//********************************
//********************************
//********************************
menu objectmenu=
{
 NULL, //no menu item selected
 0,0,0, //x,y,vertical
 {
  {"Load",M_LOADOBJECT,'l',0},
  {"Save",M_SAVEOBJECT,'s',0},
  {"Kill",M_KILLOBJECT,'k',0},
  {"Volumize",M_VOLUMIZE,'V',0},  
  {"---",NULL,0,0},
  {"Activate",M_ACTIVATEOBJECT,'a',0},
  {"FlipX",M_FLIPXOBJECT,'x',0},
  {"Info",M_OBJECTINFO,0,0},
  {"Move Base",0,'b',0},  
  {"---",NULL,0,0},
  {"Exit",M_QUIT,'q',0},  
  {0,NULL,0,0},
 }
};

menu bgmenu=
{
 NULL, //no menu item selected
 0,0,0, //x,y,vertical
 {
  {"Load",M_LOADBG,0,0},
  {"Edit",M_EDITBG,0,0},
  {0,NULL,0,0},
 }
};


menu editmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Copy Images",M_COPYIMAGES,'[',0},
  {"Copy Effects",M_COPYEFFECTS,0,0},  
  {"Copy Frame",M_COPYFRAME,'{',0},
  {"Copy Series",M_COPYSERIES,'c',0},
  {"---",NULL,0,0},
  {"Paste",M_PASTE,']',0},
  {0,NULL,0,0},
 }
};

menu seriesmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Play",M_PLAY,'p',0},
  {"Play Looped",M_PLAYLOOPED,'P',0},  
  {"Stop",M_STOP,0,0},
  {"-----",0,0,0},
  {"List",M_SERIESLIST,'m',0},
  {"-----",0,0,0},
  {"Rename",M_RENAMESERIES,'R',0},
  {"Add New",M_ADDNEWSERIES,0,0},    
  {"Delete Last",M_DELETELASTSERIES,0,0},    
  {"Save SDF",M_SAVESDF,0,0},    
  {0,NULL,0,0},
 }
};

menu framemenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"New Image",M_NEWIMAGE,'n',0},
  {"New Effect",M_NEWEFFECT,'e',0},
  {"------",NULL,0,0},
  {"Insert",M_INSERTFRAME,'i',0},
  {"Insert After",M_INSERTAFTER,'I',0},
  {"Delete",M_DELETEFRAME,'D',0},
  {0,NULL,0,0},
 }
};

menu miscmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Toggle Status",M_DRAWSTAT,'h',0},
  {"Toggle Verbose",M_VERBOSE,'v',0},
  {"Toggle DrawRegions",M_DRAWREGIONS,0,0},
  {"------",0,0,0},
  {"ActivateAll",M_ACTIVATEALL,'A',0},
  {"ResetAll",M_RESETALL,'r',0},
  {"------",0,0,0},
  {"200% speed",M_200SPEED,0,0},
  {"150% speed",M_150SPEED,0,0},
  {"100% speed",M_100SPEED,0,0},
  {" 50% speed",M_50SPEED,0,0},
  {" 25% speed",M_25SPEED,0,0},
  {0,NULL,0,0},
 }
};



menu mainmenu=
{
 NULL, //no menu item selected
 0,0,1, //x,y,vertical
 {
  {"Object",NULL,0,&objectmenu},
  {"Bg",NULL,0,&bgmenu},
  {"Edit",NULL,0,&editmenu},
  {"Series",NULL,0,&seriesmenu},
  {"Frame",NULL,0,&framemenu},
  {"Misc",NULL,0,&miscmenu},
  {0,NULL,0,0},
 }
};










///****************************CHECK KEYS
void CheckKeys()
{
key=kscan=0;
if (!keyhit()) return;

kscan=getkey();
key=scan2ascii(kscan);

if (seriesedit)
 {
  int slen=strlen(seriesedit->name);
  
  if (kscan==0xE) //backspace
    if (slen>0)     {seriesedit->name[slen-1]=0; }

  if (isprint(key) && slen<15)
   {
    seriesedit->name[slen]=key;
    seriesedit->name[slen+1]=0;
   }
   
  if (kscan==0x1C || kscan==1) seriesedit=0;

  return;
 }


if (dragimg) //if image being dragged
 {
  if (kscan==83) //delete key, 83
       M_DELETEIMAGE();
     
  if (key=='b')
   if (dragimg>p->fptr->getimgptr())
      {
       image t=*dragimg;
       memmove(dragimg,dragimg-1,sizeof(image) );
       dragimg--; *dragimg=t;
       kbscan=key=0;       
      }
  if (key=='x') {dragimg->orient^=2; kbscan=key=0;}
  if (key=='y') {dragimg->orient^=1;kbscan=key=0;}
 }         


class menuitem *mi=mainmenu.checkkey(key);
if (mi && mi->func)
 {
  mi->func();
  return;
 }

    
if (p && !dlg.active) //if object being edited
 {
  if (kscan==72 && p->csnum>0) {resetshadow(); p->startseries(p->csnum-1);}
  if (kscan==80 && p->csnum<p->od->nums-1) {resetshadow(); p->startseries(p->csnum+1); }
  if (kscan==0x49) {resetshadow(); int t=p->csnum; t-=15; if (t<0) t=0; p->startseries(t);}
  if (kscan==0x51) {resetshadow(); p->csnum+=15; if (p->csnum>=p->od->nums) p->csnum=p->od->nums-1; p->startseries(p->csnum);}  

  if (p->fptr)
  {  
   if (kscan==0x4b && p->cf>0)
        {resetshadow(); p->cf--; p->cs->cf=p->cf; p->resetfptr();}
   if (kscan==0x4d && p->cf<p->nf-1)
       {resetshadow(); p->cf++; p->cs->cf=p->cf; p->resetfptr();}
  }       
 }

 //capitalize
 key=(char)toupper(key);
 if (!dlg.active && key==27) M_QUIT();
// if (key=='H') drawstat^=1;
// if (key=='V') {verbose^=1; mprintf("verbose %s",verbose ? "ON" : "OFF");}

 if (kbstat&KB_CTRL)
  {
   int oldspeed=speed;
   if (key=='-') if (speed>30) speed-=10;
   if (key=='+' || key=='=') if (speed<500) speed+=10;
   if (speed!=oldspeed)
    {
     SetTimerSpeed(speed);
     mprintf("timer speed: %d%%",speed);   
    }
  }      

 if (key=='\t') //switch objects
      {
       if (o)
       {
        if (!p || !o) p=o;
         else
         {
          if (!(kbstat&KB_CTRL)) if (p->next) p=p->next; else p=o;
              else //backtab
             if (p->prev) p=p->prev;
             else {p=o; while (p->next) p=p->next;}
         }
         
        if (p->active) inputdevice.bind(p); //be sure to set its input device if active
       } else p=0;
       resetshadow();
      }
      

if (key>='1' && key<='9')
{
 if (key-'0'<numodf)  
 addobject(new object(&odf[key-'0'],0,mx,my-160,160,0,0)); //create an instance of the object
}    


if (p) //if object being edited
{
 if (p->fptr) //if frames exist      
 {
  if (key==' ') p->effect(); 
 } //p->nf==0


 if (emode==EM_SERIES)
  {
   if (key=='=') p->cs->energy++;
   if (key=='-' && p->cs->energy>0) p->cs->energy--;
  }

} //p
     


}












void objectdef::volumize()
{
 volumefile v;
    //create volfile
 v.create(volfile);
 
 //Go to that object directory   
 char oldpath[30];
 getcwd(oldpath,30);
 if (chdir(dir)) {mprintf("%s dir does not exist",dir); v.close(); return;}

 //write object shit

 v.write(name,strlen(name)+1,V_RAWDATA,"objname");  //write name
 v.writefile("series.lst"); //write series's
 
 v.writelistblock("image.lst",V_IMAGE);
 v.writelistblock("sound.lst",V_SOUND); 

 //change to old dir
 chdir(oldpath);
 mprintf("%s volumized %d bytes",name,filelength(v.h));
 v.close();
 
}    

void objectdef::Save()
{
 if (!loaded) return;

 //Go to that object directory   
 char oldpath[30];
 getcwd(oldpath,30);
 if (chdir(dir)) {mprintf("%s dir does not exist",dir);  return;}
 
 SaveSeriesList();

 if (type==BGDEF) ((backgrounddef *)this)->Save(); //save the extra bg stuff maybe

 chdir(oldpath);

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
if (dirty)  {if (query("Changes were made to %s, save?",name)) Save();}

//free series definitions
if (sd) 
{
 for (int j=0; j<nums; j++) if (sd[j].first) free(sd[j].first);
 free(sd);
 sd=0; nums=0;
 f=0; 
}
//free sound definitions
if (sounds)
{
 for (int j=0; j<numsounds; j++) if (sounds[j].ptr) free(sounds[j].ptr);
 free(sounds);
 sounds=0; numsounds=0;
}

//free image definitions
if (id)
{
 for (int j=0; j<numi; j++) if (id[j].ptr) free(id[j].ptr);
 free(id);
 id=0; numi=0;
}

if (type==BGDEF) ((backgrounddef *)this)->Kill(); //kill the extra bg stuff maybe
  
mem=0; dirty=0;

loaded=0; //clear loaded flag

mprintf("%s killed",name); 
}

extern int memused;
//read object definition from disk    
void objectdef::Read()
{
if (!type) return; //hasn't been filled in    
refcount++; //increase reference count
if (loaded) return; //if already loaded, we dont care
if (timerbusy) {needload++; return;} //we can't load it if we are in the timer, set flag

//Go to that directory   
char oldpath[30];
getcwd(oldpath,30);
if (chdir(dir)) {mprintf("%s dir does not exist",dir);  return;}

//-------read objectdef into memory  
dirty=0; mem=0;
sd=0; nums=0; //no series
id=0; numi=0; //no images
sounds=0; numsounds=0; //no sounds
memused=0;

//read images
numi=ReadImageList("image.lst",&id); 
if (numi==-1) goto error; //if error, break

//read sounds
numsounds=ReadSoundList("sound.lst",&sounds); 

//read series list
if (LoadSeriesList()==-1) goto error; //if error, break

if (type==BGDEF) ((backgrounddef *)this)->Kill(); //kill the extra bg stuff maybe

mem=memused; //store amount of memory used
loaded=1; //set loaded

mprintf("%s loaded",name);


error:
chdir(oldpath);
}    



void object::resetfptr()
{
if (!fptr) return;

_disable();
fptr=cs->first;
if (!fptr) {dur=0; return;}

for (uchar i=0; i<cf; i++)  //skip ahead to frameptr
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
   if (t->onum==::p->onum) //if other object of same type
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
   if (t->onum==::p->onum) //if other object of same type
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
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//if high bit of etype set, extra only done on Hit
// sound

char *extraname[]=
{
 "Sound effect",
 "Ground shake",
 "Object shake",
 "New object",
 "Goto series",
 "Blood",
 "Region", 
 "ChildPosition",
 0
} ;

//size of each extra structure...
int extrasize[]=
{
 sizeof(e_sound),
 sizeof(e_shake),
 sizeof(e_shake),
 sizeof(e_newobject),
 sizeof(e_goto),
 sizeof(e_blood),
 sizeof(e_region), 
 sizeof(e_newobject),  //childpos
 0
};    

int E_SOUND_draw(object *o,e_sound *t,int x,int y,int sel)
{
if (t->sound>=o->od->numsounds) return sizeof(e_sound);
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
     
 dprintf(font[sel],x,y,"%s  intensity: %d-%s",extraname[t->etype&0x7f],t->intensity+1,istr[t->intensity]);
 return(sizeof(e_shake));
}

int E_SHAKE_edit(object *o,e_shake *t)
{
 o=o;
 t->intensity++;
 t->intensity&=3;
 return 1;
}


//Functions for the NEW_OBJECT creation effect
int E_NEWOBJECT_draw(object *o,e_newobject *t,int x,int y,int sel)
{
o=o;
char sername[32]="<not loaded>";
if (odf[t->onum].loaded) strcpy(sername,odf[t->onum].sd[t->s].name);
char *estr=((t->etype&0x7F)==E_NEWOBJECT ? "NewObject" : "ChildPos");
dprintf(font[sel],x,y,"%s: %s %dX:%dY:%dZ %s",estr,odf[t->onum].name,t->pos.x,t->pos.y,t->pos.z,sername);

if (t->pos.d) DrawLetter(font[2],screen,'X',310,y);
return(sizeof(e_newobject));
}

void dlg_e_newobjectseries(series *ser,int i,e_newobject *context)
{
 context->s=i;
}

void dlg_e_newobject(objectdef *odefs,int i, e_newobject *t)
{
 t->onum=i;

 if (!odf[t->onum].loaded) odf[t->onum].Read();
 rect r={50,15,320-50,170};
 dlg.init(&r,"New Object Effect Series",odf[t->onum].sd,sizeof(series),odf[t->onum].nums,t->s,
    (void *)dlg_series,(void *)dlg_e_newobjectseries,t);
}

 
static int dopos=0;
int E_NEWOBJECT_edit(object *o,e_newobject *t)
{ //request object
if (!dopos)
{    
if (!t->onum || (kbstat&KB_CTRL))
{
 rect r={80,50,320-80,160};
 dlg.init(&r,"New Object Effect Type",odf,sizeof(objectdef),numodf,-1,
    (void *)dlg_objectdef,(void *)dlg_e_newobject,t);
 dopos=0;
 return 1;
}

if (kbstat&KB_ALT)
{
 if (!odf[t->onum].loaded) odf[t->onum].Read();
 rect r={50,15,320-50,170};
 dlg.init(&r,"New Object Effect Series",odf[t->onum].sd,sizeof(series),odf[t->onum].nums,t->s,
    (void *)dlg_series,(void *)dlg_e_newobjectseries,t);
 dopos=0;
 return 1;
}
}

if (dopos)
{
 t->pos.x=mx-(o->x>>16);
 if (!(mb&2))
  t->pos.y=my-(o->z>>16)-(t->pos.z);
    else
  t->pos.z=((my-(o->z>>16)-t->pos.y));

 if ((kbstat&KB_SHIFT))if (t->pos.y>0) t->pos.y=0;
 if (kbstat&KB_CTRL) t->pos.d=0xFF;
              else t->pos.d=0;
 if (mstat&1) {dopos=0; return 1;}
} else dopos=1;
return 0; //dont stop editing yet
}




int E_GOTO_draw(object *o,e_goto *t,int x,int y,int sel)
{
dprintf(font[sel],x,y,"Goto series: %s",o->od->sd[t->s].name);
return(sizeof(e_goto));
}
void dlg_e_gotoseries(series *ser,int i,e_goto *context)
{
 context->s=i;
}
int E_GOTO_edit(object *o,e_goto *t)
{
rect r={50,15,320-50,170};
dlg.init(&r,"Effect Goto Series",p->od->sd,sizeof(series),p->od->nums,t->s,
    (void *)dlg_series,(void *)dlg_e_gotoseries,t);
return 1;
}



int E_CHILDPOS_edit(object *o,e_newobject *t)
{ //request object
if (!t->onum || (kbstat&KB_CTRL))
{
 rect r={80,50,320-80,160};
 dlg.init(&r,"Child Pos Type",odf,sizeof(objectdef),numodf,o->onum,
    (void *)dlg_objectdef,(void *)dlg_e_newobject,t);
 dopos=0;
 return 1;
}
if (kbstat&KB_ALT)
{
 rect r={50,15,320-50,170};
 dlg.init(&r,"Child Pos Series",odf[t->onum].sd,sizeof(series),odf[t->onum].nums,t->s,
    (void *)dlg_series,(void *)dlg_e_newobjectseries,t);
 dopos=0;
 return 1;
}
if (dopos)
{
 t->pos.x=mx-(o->x>>16);
 if (!(mb&2))
  t->pos.y=my-(o->z>>16)-t->pos.z;
    else
  t->pos.z=my-(o->z>>16)-t->pos.y;

 if (!(kbstat&KB_SHIFT))
   if (t->pos.y>0) t->pos.y=0;

 if (o->child) //position child
  {
   if (!o->d) o->child->x=o->x+(t->pos.x<<16);
         else o->child->x=o->x-(t->pos.x<<16);
   o->child->y=o->y+(t->pos.y<<16);
   o->child->z=o->z+(t->pos.z<<16);
  }

 if (mstat&1)
 {
  dopos=0;
  return 1;
 }
} else dopos=1;
return 0; //dont stop editing yet
}



int E_BLOOD_draw(object *o,e_blood *t,int x,int y,int sel)
{ //request object
 char *bstr[]={"Squirt","Drip","Explosion"};
 dprintf(font[sel],x,y,"Blood %s %dX:%dY:%dZ",bstr[t->bloodtype],t->pos.x,t->pos.y,t->pos.z);
 return(sizeof(e_blood));
}

int E_BLOOD_edit(object *o,e_blood *t)
{ 
if ((kbstat&KB_CTRL))
{
 t->bloodtype++; t->bloodtype%=3;
 return 1;
}

if (dopos)
{
 t->pos.x=mx-(o->x>>16);
 if (!(mb&2)) t->pos.y=my-(o->z>>16)-t->pos.z;
        else  t->pos.z=my-(o->z>>16)-t->pos.y;
 if ((kbstat&KB_SHIFT)) if (t->pos.y>0) t->pos.y=0;
 
 if (mstat&1) {dopos=0; return 1;}
} else dopos=1;
return 0; //dont stop editing yet
}



void dlg_pickrfunc(char *cfr,int i,e_region *context)
{
 context->rfunc=i;
}
void copyrect(zrect *s,zrect *d)
{
 if (s->p1.x<s->p2.x) {d->p1.x=s->p1.x; d->p2.x=s->p2.x;}
                else {d->p1.x=s->p2.x; d->p2.x=s->p1.x;}
 if (s->p1.y<s->p2.y) {d->p1.y=s->p1.y; d->p2.y=s->p2.y;}
                else {d->p1.y=s->p2.y; d->p2.y=s->p1.y;}
 if (s->p1.z<s->p2.z) {d->p1.z=s->p1.z; d->p2.z=s->p2.z;}
                else {d->p1.z=s->p2.z; d->p2.z=s->p1.z;}
}    
void dlg_e_regionseries(series *ser,int i,e_region *context) {context->afterseries=i;}
int E_REGION_draw(object *o,e_region *t,int x,int y,int sel)
{ //request object
 char *rstr[]={"imperm","vuln","attack"};
 if (t->rtype&0x80) //erase
 dprintf(font[sel],x,y,"%s erase",rstr[t->rtype&0x7f]);
   else
 dprintf(font[sel],x,y,"%s %d:%d:%d::%d:%d:%d rfunc:%s after:%s",rstr[t->rtype],
     t->r.p1.x,t->r.p1.y,t->r.p1.z,
     t->r.p2.x,t->r.p2.y,t->r.p2.z,
     rfuncnames[t->rfunc],o->od->sd[t->afterseries].name);
 return(sizeof(e_region));
}
int E_REGION_edit(object *o,e_region *t)
{ 
if (kbstat&KB_ALT)
{
 rect r={50,15,320-50,170};
 dlg.init(&r,"Region After Series",o->od->sd,sizeof(series),o->od->nums,t->afterseries,
    (void *)dlg_series,(void *)dlg_e_regionseries,t);
 dopos=0;
 return 1;
}

if (kbstat&KB_CTRL)
{
 rect r={50,15,320-50,170};
 dlg.init(&r,"Select Region Function",rfuncnames,32,rfuncnum,t->rfunc,
           (void *)dlg_cfunc,(void *)dlg_pickrfunc,t);
 dopos=1;
 return 1;
}    

if (!dopos && mx<50)
{
 t->rtype++;
 if ( (t->rtype&0x7F)==3)  {t->rtype&=0x80; t->rtype^=0x80;}
 dopos=0;
 if (t->rtype==1)
  if (p->onum<=3) t->rfunc=5; else t->rfunc=7;
 if (t->rtype==2)
  if (p->onum<=3) t->rfunc=4; else t->rfunc=6;
 return 1;
}
if (t->rtype&0x80) return 1; //do nothign for erase


static zrect r; //temp rectangle
if (!dopos)
{ //start rectangle
 r.p1.x=r.p1.y=0; r.p1.z=-5;
 r.p2.x=r.p2.y=0; r.p2.z=5;  
 dopos=1;
}
if (dopos==1) //1st coord
{
 if (mb&2)
  {
    r.p1.z=my-(o->z>>16)-r.p1.y;
  }
 if (mstat&1)
 {
  r.p1.x=mx-(o->x>>16);
  r.p1.y=my-(o->z>>16); //-r.p1.z;
  if ((kbstat&KB_SHIFT)) if (r.p1.y>0) r.p1.y=0;
  dopos=2;
 }
}

if (dopos==2) //second coord
{
 r.p2.x=mx-(o->x>>16);
 if (!(mb&2)) r.p2.y=my-(o->z>>16); //-r.p2.z;
        else  r.p2.z=my-(o->z>>16); //-r.p2.y;
 if ((kbstat&KB_SHIFT)) if (r.p2.y>0) r.p2.y=0;
 if (mrelstat&1)  dopos=0; //released button
} 

 DrawImage(cursor,screen,mx,my,0);    
 copyrect(&r,&t->r);
 return dopos ? 0 : 1;
}


//pointer to functions that draw the specified extra
//returns the size of the extra
int (*extradraw[])(object *,byte *,int,int,int)=
 {
  (int (*)(object *,byte *,int,int,int)) E_SOUND_draw,  //0
  (int (*)(object *,byte *,int,int,int)) E_SHAKE_draw,  //1
  (int (*)(object *,byte *,int,int,int)) E_SHAKE_draw,  //2
  (int (*)(object *,byte *,int,int,int)) E_NEWOBJECT_draw,  //3
  (int (*)(object *,byte *,int,int,int)) E_GOTO_draw,  //4  
  (int (*)(object *,byte *,int,int,int)) E_BLOOD_draw,  //5
  (int (*)(object *,byte *,int,int,int)) E_REGION_draw,  //6  
  (int (*)(object *,byte *,int,int,int)) E_NEWOBJECT_draw,  //7 child pos
  
 };

//pointer to functions that edit the specified extra
//returns 0 to continue editing, 1 to stop
int (*extraedit[])(object *,byte *)=
 {
  (int (*)(object *,byte *)) E_SOUND_edit,  //0
  (int (*)(object *,byte *)) E_SHAKE_edit,  //1
  (int (*)(object *,byte *)) E_SHAKE_edit,  //2
  (int (*)(object *,byte *)) E_NEWOBJECT_edit,  //3
  (int (*)(object *,byte *)) E_GOTO_edit,  //4
  (int (*)(object *,byte *)) E_BLOOD_edit,  //5
  (int (*)(object *,byte *)) E_REGION_edit,  //6    
  (int (*)(object *,byte *)) E_CHILDPOS_edit,  //7
  0,
 };

char rcolor[3]={2,1,4};


void object::DrawRegions()
{
for (int i=2; i>=0; i--)
if ((drawregions==1 && i==0) || (drawregions==2 && i!=0))
 if (r[i])
 {
  rect t;
  if (!d)
  {t.x1=(x>>16)+r[i]->r->r.p1.x; t.x2=(x>>16)+r[i]->r->r.p2.x;}
  else
  {t.x1=(x>>16)-r[i]->r->r.p2.x; t.x2=(x>>16)-r[i]->r->r.p1.x;}
  
  t.y1=r[i]->r->r.p1.y+((y+z)>>16);
  t.y2=r[i]->r->r.p2.y+((y+z)>>16);
  DrawBar(screen,rcolor[r[i]->r->rtype],t.x1,t.y1,t.x2-t.x1,t.y2-t.y1);
 }
}

    
//Extra stuff X's
void object::DrawExtraX()
{
if (!this || !fptr) return;    
byte *t=fptr->getextraptr();   //pointer to first option

for (int n=fptr->numextra; n>0; n--)
 {
   switch(((*t)&0x7F))
   {
    case E_NEWOBJECT:
     {
     e_newobject *e=(e_newobject *)t;
     DrawImage(X,screen,(x>>16)+e->pos.x-X->xw/2,((z+y)>>16)+e->pos.y+(e->pos.z)-X->yw/2,0);
     }
    break;
    case E_BLOOD:
    {
     e_blood *e=(e_blood *)t;
     DrawImage(X,screen,(x>>16)+e->pos.x-X->xw/2,((z+y)>>16)+e->pos.y+(e->pos.z)-X->yw/2,0);
    }
    break;
    case E_REGION: //has an rect
    {
     e_region *e=(e_region *)t;
     if (!(e->rtype&0x80)) //not erase
      {
       rect r;
       r.x1=e->r.p1.x+(x>>16); r.y1=e->r.p1.y+((y+z)>>16);
       r.x2=e->r.p2.x+(x>>16); r.y2=e->r.p2.y+((y+z)>>16);
       DrawBar(screen,rcolor[e->rtype],r.x1,r.y1,r.x2-r.x1,r.y2-r.y1);
      }
    }
    break;
   }
  t+=extrasize[(*t)&0x7F];
 }
}
//Extra stuff X's shadow
void object::DrawExtraXShadow()
{
if (!this || !fptr) return;    
byte *t=fptr->getextraptr();   //pointer to first option

for (int n=fptr->numextra; n>0; n--)
 {
  switch(((*t)&0x7F))
  {
  case E_NEWOBJECT: //has an X
   {
    e_newobject *e=(e_newobject *)t;
    DrawImage(Xshadow,screen,(x>>16)+e->pos.x-Xshadow->xw/2,((z+y)>>16)+(e->pos.z)-Xshadow->yw/2,0);
   }
  break;
  case E_BLOOD: //has an X
   {
    e_blood *e=(e_blood *)t;
    DrawImage(Xshadow,screen,(x>>16)+e->pos.x-Xshadow->xw/2,((z+y)>>16)+(e->pos.z)-Xshadow->yw/2,0);
   }
  break;
  case E_REGION: //has an rect
    {
     e_region *e=(e_region *)t;
     if (!(e->rtype&0x80)) //not erase
      {
       rect r;
       r.x1=e->r.p1.x+(x>>16); r.y1=e->r.p1.z+((y+z)>>16);
       r.x2=e->r.p2.x+(x>>16); r.y2=e->r.p2.z+((y+z)>>16);
       DrawBar(screen,7,r.x1,r.y1,r.x2-r.x1,r.y2-r.y1);
      }
    }
  break;
  }

  
  t+=extrasize[(*t)&0x7F];
 }

}




void drawarrow(int stat,int x,int y)
{
 int a=-1;
 if (stat) 
 if (stat&ID_UP)
   {
     if (stat&ID_LEFT) a=4 ; else
     if (stat&ID_RIGHT) a=5 ; else
         a=0; //UP
   } else
 if (stat&ID_DOWN)
   {
     if (stat&ID_LEFT) a=6 ; else
     if (stat&ID_RIGHT) a=7; else
         a=1; //DOWN
   } else
 if (stat&ID_LEFT) a=2; else
 if (stat&ID_RIGHT) a=3;

if (stat&ID_BUT0) DrawImage(arrow[9],screen,x,y-6,0); else
if (stat&ID_BUT1) DrawImage(arrow[10],screen,x,y-6,0);

//draw arrow
a++; DrawImage(arrow[a],screen,x,y,0);
}    


void drawintersect(int stat,int x,int y)
{
 if (stat&ID_UP) DrawImage(arrow[0+1],screen,x,y-5,0);
 if (stat&ID_DOWN) DrawImage(arrow[1+1],screen,x,y+5,0);
 if (stat&ID_RIGHT) DrawImage(arrow[2+1],screen,x-5,y,0); 
 if (stat&ID_LEFT) DrawImage(arrow[3+1],screen,x+5,y,0);
}    







void dlg_pickcfunc(char *cfr,int i,object *context)
{
 context->cs->cfunc=i;
 if (kbstat&(KB_CTRL|KB_ALT))
  {
   objectdef *od=context->od;
   for (int j=0; j<od->nums; j++)
     od->sd[j].cfunc=i;
  }
}

void dlg_afterseries(char *cfr,int i,object *context)
{
 context->cs->next=i;
}



void object::DrawSeries()
{
if (!this) return;

if (dselprintf(2,35,"NeedFalldown: %s",(cs->parm&SP_NOFALLDOWN) ? " OFF" : "ON")) cs->parm^=SP_NOFALLDOWN;
   
if (dselprintf(2,45,"Shadow: %s",(cs->parm&SP_NOSHADOW) ? " OFF" : "ON")) cs->parm^=SP_NOSHADOW;
if (dselprintf(2,55,"HitGround: %s",(cs->parm&SP_NOGROUND) ? "OFF" : "ON")) cs->parm^=SP_NOGROUND;
    

dprintf(font[0],2,100,"energy: %d",cs->energy);

//draw series after
if (
    (cs->next==0xFF && dselprintf(2,75,"after: suicide")) ||
    (cs->next==0xFE && dselprintf(2,75,"after: lastforever")) ||
    (dselprintf(2,75,"after: %s",od->sd[cs->next].name)) 
   ) 
 {
   if (kbstat&KB_SHIFT)
    {cs->next=0xFF;}
   else
   if (kbstat&KB_ALT)
    {cs->next=csnum;}
   else
   if (kbstat&KB_CTRL)
    {cs->next=0xFE;}
    else
   {
    rect r={50,15,320-50,170};
    dlg.init(&r,"Select AfterSeries",od->sd,sizeof(series),od->nums,cs->next>=0xFE ? 0 : cs->next,
           (void *)dlg_series,(void *)dlg_afterseries,p);
   }
 }

//draw cfuncnames 
if (dselprintf(2,85,"cfunc: %s",cfuncnames[p->cs->cfunc]))
 {
  if (kbstat&KB_SHIFT) p->cs->cfunc=0;
   else
   {
   rect r={50,15,320-50,170};
   dlg.init(&r,"Select Control Function",cfuncnames,32,cfuncnum,p->cs->cfunc,
           (void *)dlg_cfunc,(void *)dlg_pickcfunc,this);
   }
 }

}    

//Extra stuff
void object::DrawExtra()
{
if (!this) return;
//return;
if (doedit)
 {
//  wait();
  if (!extraedit[*doedit] || extraedit[*doedit](this,doedit))
   {doedit=0; return;}
 }

if (!nf || !fptr) return;    
//if (fptr->numextra) 

if (!fptr->numextra) dprintf(font[3],10,12,"No effects");
  else
{
byte *t=fptr->getextraptr();   //pointer to first option
int n=fptr->numextra;          //number of bytes of options

int x=10,y=14;
memset(screen+y*320,7,320);
while (n>0)  //cycle through all options
 {
  int sel=(my>=y && my<y+10);
  if (sel && !doedit && !dlg.active)
    {
     if (mstat&1) //they wanna edit this thing
      {doedit=t; od->dirty=1;}

     if (mstat&2) //right click on selected effect
      {*t^=0x80; mstat=0;} //turn on/off hit thing

     DrawBar(screen,2,0,y+1,320,9);
     if (kbscan==83) //delete key (they wanna delete this baby)
     {
      _disable();
      fptr->numextra--;
      fptr->size-=extrasize[(*t)&0x7F];
      cs->deletebytes((char *)t,extrasize[(*t)&0x7F]); //delete those bytes
      n--;
      _enable();
      od->dirty=1;
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

 if ( (*t)&0x80) DrawLetter(font[2],screen,'H',0,y+2);
  
  t+=extradraw[(*t)&0x7F](this,t,x,y+2,sel ? 1 : 3); //draw thingee;
  
  n--; //one less extra

  y+=10; //go down one line
  memset(screen+y*320,7,320);    
 }
}


if ((mstat&2)  && !doedit && !dlg.active)
  {
   M_NEWEFFECT();
  }

 
}

void object::AddExtra(int i)
{
if (!this) return;    
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
if (i==E_CHILDPOS) ((e_newobject *)t)->onum=onum;

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
//return 0;    
 char s[256];
 char *arg=(char *)&format+sizeof(format);
 vsprintf(s,format,&arg);
 int sel=3;
 if (my>=y && my<y+10 && mx>=x && mx<x+GetStringWidth(font[0],s) && !doedit && !dragimg) sel=1;

 if (sel==1 && (mstat&1)  && !mainmenu.msel && !dlg.active)
   {
//     wait();
    mstat=0;
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

mstat=0;
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


void bgobject::DrawStatus()
{
 DrawString(font[2],screen,od->name,255,190);
}    


//draw full status at bottom of screen
void object::DrawStatus()
{
// int p;

// DrawBarMap(screen,SHADOWMAP,0,180,320,20);
 
 char s[80];
 //draw object name
 DrawString(font[2],screen,od->name,255,190);
 //draw series name
// DrawString(font[2],screen,p->cs->name,2,180);

 //editing this series
 if (seriesedit==cs)
   { //draw cursor
     DrawString(font[1],screen,cs->name,2,180);
     if (uu&8) DrawBar(screen,15,2+GetStringWidth(font[0],cs->name)+1,180,8,8);
   } else
 if (dselprintf(2,180,cs->name) && !playing)
   M_RENAMESERIES();
   
      
 //draw frame
 if (!fptr)
  {
   if (!nf) DrawString(font[0],screen,"No frames",2,190);
     else  DrawString(font[0],screen,"Deleted",2,190); //if there are frames, but fptr=0 it is deleted
  }
  else //there is a frame
   {
    memset(screen+189*320+10,4,nf);
    screen[189*320+10+cf]=0xFF;
       
    DrawString(font[0],screen,"frame:",2,190);
    sprintf(s,"%2d/%2d",cf+1,nf);
    DrawString(font[1],screen,s,40,190);

    #define DXX 80
    static int tiptime=0xFFFFFFFF;
    if (my>190 && mx>80 && mx<260)
     {
      if (uu>tiptime)
       {
        mprintf("Press ',' or '.' to change values.");
        tiptime=0xFFFFFFFF;
       }
     } else tiptime=uu+200;

    short int *dptr=0;
    sprintf(s,"dX:%3.02f",((float)fptr->dx)/256);
    if (mx>DXX && mx<DXX+45)
      {
       DrawString(font[1],screen,s,DXX,190);
       dptr=&fptr->dx;
      } else DrawString(font[0],screen,s,DXX,190);
    
    sprintf(s,"dY:%3.02f",((float)fptr->dy)/256);
    if (mx>DXX+45 && mx<DXX+90)
      {
       DrawString(font[1],screen,s,DXX+45,190);
       dptr=&fptr->dy;
      } else DrawString(font[0],screen,s,DXX+45,190);
    
    sprintf(s,"dZ:%3.02f",((float)fptr->dz)/256);
    if (mx>DXX+90 && mx<DXX+90+45)
      {
       DrawString(font[1],screen,s,DXX+90,190);
       dptr=&fptr->dz;
      } else DrawString(font[0],screen,s,DXX+90,190);

    if (dptr && key)
     {
      if (key=='.') *dptr+=32;  if (key=='>') *dptr+=0x100;
      if (key==',') *dptr-=32; if (key=='<')  *dptr-=0x100;
      if (key=='?') *dptr=0;
    
     }
     
    //print dur
    sprintf(s,"dur:%2d",fptr->dur);
    if (mx>215 && mx<215+45)
      {
       DrawString(font[1],screen,s,215,190);
       if (key)
        {
         if (key=='.') fptr->dur+=1; if (key=='>') fptr->dur+=10;
         if (key==',') fptr->dur-=1; if (key=='<') fptr->dur-=10;
         if (key=='?') fptr->dur=1;
         if (key=='"') fptr->dur=9999;
         if (fptr->dur>65000 || fptr->dur<1) fptr->dur=1;
        }
      } else DrawString(font[0],screen,s,215,190);
    
    if (verbose)
    {
     sprintf(s,"%db",fptr->size);
     DrawString(font[0],screen,s,285,180);
     dprintf(font[0],285,170,"%den",energy);
     if (r[R_IMPERM])
      dprintf(font[0],245,160,"%d:%Xplat",r[R_IMPERM]->platnum,r[R_IMPERM]->platform);
    }
   
   } 

 drawarrow(inputdevice.stat&0xF,260,0);

}    
