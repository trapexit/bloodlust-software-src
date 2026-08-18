//object definition functions
#ifdef WIN95
#include <process.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <dir.h>


//#define MULTITHREAD

#include "r2img.h"
#include "objdef.h"
#include "objspace.h"
#include "object.h"

#include "file.h"
#include "message.h"

#include "dd.h"

#include "vol.h"

#include "guiroot.h"
//----------------------------------------------------

//objdef constructor
objectdef::objectdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile)
 :osp(tosp),onum(tnum)
{
 loaded=0; readonly=0; dirty=0; isvol=0;
 type=OBJDEF;
 refcount=0;
 strcpy(name,tname);

 numi=0; id=0;
 nums=0; sd=0; f=0;
 numsounds=0; sounds=0;
 mem=0;
 strcpy(volfile,tvolfile);

 FILEIO f;
 volexist=!f.open(volfile);
 f.close();
}

//destructor
objectdef::~objectdef() { if (loaded) freeodf();}

void objectdef::freeimages()
{
 if (id)
  {
   if (!isvol)
     for (int i=0; i<numi; i++)
      if (id[i]) free(id[i]);
   free(id);
  }
 numi=0;  id=0;
}

void objectdef::freesounds()
{
 if (sounds)
  {
   if (!isvol)
     for (int i=0; i<numsounds; i++)
      if (sounds[i]) free(sounds[i]);
   free(sounds);
  }
 #ifdef WIN95
 //free secondary directsound buffers
 if (dsounds)
  {
   for (int i=0; i<numsounds; i++)
      if (dsounds[i]) freesound(dsounds[i]);
   free(dsounds);
  }
 #endif

 numsounds=0;  sounds=0; dsounds=0;
}

void objectdef::freeseries()
{
 if (isvol)
  {
   free(f); //just free frame
  } else
  if (sd)
  {
   for (int i=0; i<nums; i++)
    if (sd[i].first) free(sd[i].first);
   free(sd);
  }
 nums=0; sd=0; f=0;
}

//free all used memory
void objectdef::freeodf()
{
 loaded=0; 
 freeimages();
 freeseries();
 freesounds();
 dirty=0; isvol=0;
 mem=0;
 msg.printf(1,"%s freed",name);
}

#ifdef __WATCOMC__
#define _USERENTRY
#endif

#ifdef _MSC_VER
#define _USERENTRY
#endif


#ifdef WIN95
void _USERENTRY loadobjdefthread(void *pvoid)
{
 objectdef *odf=(objectdef *)pvoid;
// msg.printf(2,"load %s thread started",odf->name);
 odf->refreshload();
// msg.printf(2,"thread ended");
}
#endif

int objectdef::load()
{
if (!type) return 0; //hasn't been filled in
refcount++; //increase reference count
if (loaded) return 1; //if already loaded, we dont care

#ifdef DOS
if (timerbusy) {osp->needload++; return 0;}
return read();
#endif

#ifdef WIN95

//start up thread
#ifdef MULTITHREAD
#ifdef __BORLANDC__
_beginthread(loadobjdefthread,0,(void *)this);
#endif
#ifdef __WATCOMC__
_beginthread(loadobjdefthread,0,4096,(void *)this);
#endif
return 0;
#else
//single thread
return read();
#endif

#endif

}

void objectdef::kill()
{
 if (refcount) refcount--; //decrease reference count
 if (!loaded) return;  //if not loaded anyway, we dont care
 if (refcount) return; //it's still needed

 if (timerbusy) return; //can't kill if in timer
// freeodf();
}


//read from volfile (not implemented yet)
int objectdef::read()
{
//read it from volumefile
mem=0;
volumefile v;
//open volfile
if (v.open(volfile))
  {msg.error("error: cannot open %s",volfile);return 0;}

//read volname
char vname[32];
v.read(vname);
if (strcmp(vname,name))
  { msg.error("error: corrupt %s",volfile); v.close(); return 0;}


//read images
id=(IMG **)v.readblock(numi);
mem+=v.hdr.size;


//read sounds
sounds=(SOUND **)v.readblock(numsounds);
mem+=v.hdr.size;


#ifdef WIN95
//convert sounds to directsounds
convertdsounds();
#endif

//parse series list   f[0]=sig f[1]=nums f[2]=seriessize
f=(frame *)v.read(); //get series list
mem+=v.hdr.size;
int *t=(int *)f;
nums=t[1];  //number of series
int ssize=t[2]; //seriessize
sd=(series *)&t[3]; //actual seriesess

char *frameptr=(char *)&sd[nums]; //get pointer to first frame
for (int i=0; i<nums; i++)
   if (sd[i].size)
    {
      sd[i].first=(frame *)frameptr; //set framepointer
      frameptr+=sd[i].size; //advance it
    } else sd[i].first=0; //no first

v.close();

//msg.printf(2,"%d nums",nums);

msg.printf(1,"%s loaded from %s %dKB",name,volfile,mem/1024);
loaded=1; //set loaded
readonly=1;
isvol=1;
return 1;
}


void objectdef::refreshload()
{
 if (!loaded && refcount)  //not loaded, but referenced
   {
    read(); //read it into memory

    for (object *t=osp->o; t; t=t->next)
     if (t->od==this)
      if (!loaded) t->kill(); //if not loaded still, was error, delete all referenced objects
       else //was loaded okay, bind to us
       {
        if (!t->active) t->startseries(t->csnum); //start series back up (set fptr)
                  else t->activate(t->csnum);   //activate it up
        if (!t->creator) t->forcefloor(); // || t->y==SETBASEY) t->y=t->getbasey();

        if (t==osp->p) root->refresh(GUIRFR_OBJECT,osp->p); //new object now
        //msg.printf(3,"%s bound",name);
       }

    if (!loaded) refcount=0;
   }
}





void frame::draw(char *dest,int x,int y,objectdef *od)
{
 if (!this) return;

 image *i=getimgptr();

 for (int j=numimages; j>0; j--,i++)
   od->id[i->index]->draw(dest,x+i->dispx,y+i->dispy,i->orient);
}

void frame::drawflipx(char *dest,int x,int y,objectdef *od)
{
 if (!this) return;

 image *i=getimgptr();
 for (int j=numimages; j>0; j--,i++)
   {
    IMG *iptr=od->id[i->index];
    iptr->draw(dest,x-i->dispx-iptr->xw,y+i->dispy,0x2^i->orient);
   }
}

















