#include <stdlib.h>
#include <stdio.h>
#include <mem.h>

#include "message.h"

#include "keyb.h"

#include "glib.h"
#include "input.h"
#include "misc.h"

#include "effect.h"
#include "object.h"

#include "file.h"
#include "vol.h"

#include "dd.h"

#include "ddraw.h"

#include "net.h"

#include "netobj.h"

#include "editctrl.h"

volatile unsigned uu=1,su=1;

color *pal;
img *cursor;
FONT *font[10];

char *bg;
img *shadow;

//linked list of all objects
object *o=0;

//linked list of all local objects
localobject *lo=0;

//object definitions
int numodf;
objectdef *odf=0;
extern volatile int needdelete,needload;

//selected object
object *p=0;

//message interface
msgbuffer msg;

//current edit control
editcontrol *edit=0;

extern input inputdevice[2];

int gshakedur=0;
//int gshake[8]={0,1*320,3*320,2*320,-1*320,-0*320,-2*320,0};

int keyboard();

extern char localnodename[],defaultlocalname[];
class localname_edit:public editcontrol
{
 public:
 localname_edit(char *p):editcontrol(p) {};
 virtual int isvalidkey(char key)
  {return (slen<15) && editcontrol::isvalidkey(key); };
 virtual void enter()
  {
   if (!slen) strcpy(localnodename,defaultlocalname);
         else strcpy(localnodename,s);
  };
 virtual void cancel()
  {
   strcpy(localnodename,defaultlocalname);
  };
};



int initgame()
{
 //set up keyboard handler
 set_keyboard_func(keyboard);


 //load up graphics and shit
 volumefile v;
 if (v.open("a32.vol")) return -1;
 pal=(color *)v.read();
 setpalette(pal);
 v.skip();
 cursor=(img *)v.read();
 font[0]=(FONT *)v.read();
 v.skip();
 shadow=(img *)v.read();
 v.close();

 font[1]=(FONT *)malloc(6000); memcpy(font[1],font[0],5986);
 FunkyFont(font[1],0xca,0xff);
 FunkyFont(font[1],0xdb,0x0);

 font[2]=(FONT *)malloc(6000); memcpy(font[2],font[0],5986);
 FunkyFont(font[2],0xca,6*16);
 FunkyFont(font[2],0xdb,0x0);


         
 //read object.bin list
/* FILEIO f;
 if (f.open("object.bin")) return -1;
 numodf=f.readint();
 odf=(objectdef *)f.readalloc(numodf*f.readint());
 f.close();*/
 int *t=(int *)loadresource("#1");
 if (!t) return -1;
 numodf=t[0];
 odf=(objectdef *)&t[2];


 //make instance of player
 addobject(new localobject(&odf[1],0,SCREENX/2,SCREENY-60,0,0,0));
 inputdevice[0].bind(o);

// addobject(new object(&odf[1],0,400,SCREENY-60,0,0,0));

// addobject(new object(&odf[5],0,160,SCREENY-60,0,0,0));
// addobject(new object(&odf[7],0,260,SCREENY-60,0,0,0));

 //initialize network
 initnetwork();

 edit=new localname_edit("Enter player name: ");
 return 0;
}

void terminategame()
{
 terminatenetwork();

 for (int i=0; i<10; i++)
  if (font[i]) free(font[i]);
 free(cursor);
 free(pal);
}



int sendrate=4; //send every 4 ticks

void gametimer()
{
 uu++;

 inputdevice[0].read();

  //tick all playing objects
 for (object *t=o; t; t=t->next)
  if (t->playing)
   {
    t->tick(); //tick the object
    t->in->reset(); //done with the triggered input
    t->updated=0;
   }

 if (networkinstalled)
  {
   static int nextsendtime=0;
   if (uu>=nextsendtime)
   {
    nextsendtime=uu+sendrate;
    netupdate();
   }
  }
}


int getnumobjects()
{
 int num=0;
 for (object *t=o; t; t=t->next) num++;
 return num;
}

int getnumlocal()
{
 int num=0;
 for (localobject *t=lo; t; t=t->lnext) num++;
 return num;
}

int getnumremote()
{
 int num=0;
 for (netnode *n=netnodes; n; n=n->next)
      num+=n->getnumremote();
 return num;
}





extern int PITCH;
extern JOYINFO joypos[2];
int fps;
int sentps=0,recvps=0;

//connection shit
int doconnect=0;
int connectipstrlen=0;
char connectipstr[30];


void loadobjectdefs();
void deleteobjects();


int blah;
void updatescreen()
{
  _disable();
  //load objectdefs as needed
  if (needload) loadobjectdefs();

  //delete objects that need deleting
  if (needdelete) deleteobjects();
  _enable();

  //clear screen
//  memset(screen,1,640*480);     //102

  //calculate fps
  static unsigned lastuu=0;

  if (!(su&127)) //check every 15 secs
   {
    fps=128*100/(uu-lastuu);
    sentps=totalsent *100/(uu-lastuu); totalsent=0;
    recvps=totalrecv *100/(uu-lastuu); totalrecv=0;
    //recvps=blah *100/(uu-lastuu); blah=0;
                                             
    lastuu=uu;
   }
  su++;

  //sort characters
  _disable();
  for (object *t=o; t; t=t->next)
   {
    t->resort();
    if (t->x<0<<16) t->x=0<<16;
    if (t->x>SCREENX<<16) t->x=SCREENX<<16;
   }
  _enable();

  //draw all object shadows
  for (t=o; t; t=t->next) //cycle through each object
   if (t->fptr)  //if frame exists
    if(!(t->cs->parm&SP_NOSHADOW))     //and shadow exists
      DrawImage(shadow,screen,(t->x>>16)-shadow->xw/2,((t->basey+t->z)>>16)-shadow->yw/2,0);

  //draw all objects
  int numobjs=0;
  for (t=o; t; t=t->next)
   {
     t->draw(); //draw the object
     numobjs++;
   }

  //draw mouse cursor
  DrawImage(cursor,screen,mx,my,0);

//  if (doconnect)
//   {
//    dprintf(font[2],20,300,"Connect to ip: %s%c",connectipstr,(uu&32)?'_':' ');
//   }

  if (edit) edit->draw(20,300,2,1);


   //print info
  dprintf(font[2],SCREENX*5/6,10,"fps:%d",fps);
  dprintf(font[2],SCREENX*5/6,20,"obj:%d",getnumobjects());
  dprintf(font[2],SCREENX*5/6,30,"local:%d",getnumlocal());
  dprintf(font[2],SCREENX*5/6,40,"remote:%d",getnumremote());

  dprintf(font[2],SCREENX*5/6,60,"sent cps: %d",sentps);
  dprintf(font[2],SCREENX*5/6,70,"recv cps: %d",recvps);

  dprintf(font[2],SCREENX/2-40,SCREENY-30,"send every %d ticks (F5/F6)",sendrate);

   blah=inputdevice[0].but;
  dprintf(font[2],SCREENX*5/6,100,"%d",blah);


  netnode *n=netnodes;
  for (int y=120; n; n=n->next,y+=10)
   DrawString(font[0],screen,n->getinfo(),20,y);

  //draw message buffer
  msg.draw(SCREENX/2,10);

}


//control for getting ip addresses
class ip_edit:public editcontrol
{
 public:
  ip_edit(char *p):editcontrol(p) {};
  virtual int isvalidkey(char key)
  {
   return isdigit(key) || key=='.';
  };
 virtual void enter()
  {
   netconnect_inet(s);
   cancel();
  };
 virtual void cancel() {};
};


class chatsend_edit:public editcontrol
{
 public:
 chatsend_edit(char *p):editcontrol(p) {};
 virtual int isvalidkey(char key)
  {
   return (slen<79) && editcontrol::isvalidkey(key);
  };
 virtual void enter()
  {
   TCP_chat tcpchat(s);
   tcpchat.sendtoall();
  };
 virtual void cancel() {};
};


int keyboard()
{
 //edit control is active
 if (edit && !(kbscan&0x80))
 {
  if (edit->processkey(kbscan))
   {delete edit; edit=0;}
  return 1;
 }

 if (refreshinputkeyboard()) return 1; //dont add key to queue

 if (kbscan&0x80) return 1;

 char key=scan2ascii(kbscan);
 if (!edit)
 {
  if (kbscan==0x3B) //they want to connect ip
   edit=new ip_edit("Connect to IP: ");
  if (kbscan==0x3D) //they want disconnect
   netdisconnect();
  if (kbscan==0x3C) //they want connect ipx
   netconnect_ipx();

  if (kbscan==0x1C) //enter/send
   edit=new chatsend_edit("Send: ");
   
  if (kbscan==0x3F && sendrate>1) sendrate--;
  if (kbscan==0x40) sendrate++;

   //introduce new object
  if (key>='1' && key<='9')
   {
    int onum=key-'0';
    if (onum<numodf) addobject(new localobject(&odf[onum],0,mx,my,0,0,0));
   }

  //esc/quit
  if (kbscan==1) quitgame();
 }

 return 1;
}








//object maintenence functions
void deleteobjects()
{
  object *t=o;
  while (t)
   {
    object *next=t->next;
    if (t->killme) //delete all flagged objects
     {
      if (p==t) p=NULL;
      delete t;
     }
    t=next;
   }
  needdelete=0;
}

void loadobjectdefs()
 {
  for (int i=1; i<numodf; i++)
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
         if (!t->active) t->startseries(t->csnum); //start series back up (set fptr)
              else t->activate(t->csnum);
      t=next;
     }
     if (!odf[i].loaded) odf[i].refcount=0;
      else odf[i].refcount--; //we didn't meant to increase the refcount
    }
 needload=0;
}







