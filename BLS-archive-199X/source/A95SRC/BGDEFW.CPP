#include <stdlib.h>
#include <dir.h>
#include <string.h>

#include "file.h"
#include "message.h"

#include "objdef.h"
#include "bgdef.h"

#include "r2img.h"


void bgdef::loadscrmap(char *filename)
{
 FILEIO f;
 if (!f.open(filename))
  {
   scrmap=(uchar (*)[SCRMAPXW][SCRMAPYW])f.readalloc(SCRMAPXW*SCRMAPYW);
   f.close();
  } else scrmap=(uchar (*)[SCRMAPXW][SCRMAPYW])calloc(SCRMAPXW*SCRMAPYW,1);
}

void bgdef::savescrmap(char *filename)
{
 FILEIO f;
 if (!scrmap) return;
 f.create(filename);
 f.write(scrmap,SCRMAPXW*SCRMAPYW);
 f.close();
}

void bgdef::loadbglines(char *filename)
{
 FILEIO f;
 if (!f.open(filename))
 {
  numbglines=f.readint();
  f.readint(); //skip size
  bgl=(bgline *)f.readalloc(numbglines*sizeof(bgline));
  f.close();
 }
}

void bgdef::savebglines(char *filename)
{
if (!bgl) return;
FILEIO f;
f.create(filename);
f.writeint(numbglines);
f.writeint(sizeof(bgline));
f.write(bgl,numbglines*sizeof(bgline));
f.close();
}

void bgdef::loadbgimages(char *filename)
{
 FILEIO f;
 //read bg image list
 if (!f.open(filename))
 {
  numbgimages=f.readint();
  f.readint(); //skip size
  bgi=(bgimage *)f.readalloc(numbgimages*sizeof(bgimage));
  f.close();
  resortbgimages();
 }
}


void bgdef::savebgimages(char *filename)
{
 if (!bgi) return;
 FILEIO f;
 f.create(filename);
 f.writeint(numbgimages);
 f.writeint(sizeof(bgimage));
 f.write(bgi,numbgimages*sizeof(bgimage));
 f.close();
}


int bgdef::read()
{
 FILEIO f;
 //go to object directory
 char oldpath[30];
 getcwd(oldpath,30);
 if (chdir(dir)) {msg.printf(5,"error: %s dir does not exist",dir);  return 0;}

 loadbgimages("bgimages.map");
 loadscrmap("bgscreen.map");
 loadbglines("bglines.map");

 //read individual bg components
 if (!readimages("image.lst")) {msg.printf(5,"error: reading images"); freeodf(); return 0;} else
 if (!readscreens("bgscreen.lst")) {msg.printf(5,"error: reading scr's"); freeodf(); return 0;} else
 if (!readsounds("sound.lst")) {msg.printf(5,"error: reading sounds"); freeodf(); return 0;} else
 if (!readseries("series.lst")) {msg.printf(5,"error: reading series list"); freeodf(); return 0;} else
  { //successful
  readonly=0;
  loaded=1;
  msg.printf(1,"%s loaded: %dimgs %dscrs %dsnds %dK",name,numi,numscr,numsounds,mem/1024);
 }
 msg.printf(1,"%d bgimages %d bglines",numbgimages,numbglines);

 chdir(oldpath);
 return loaded;
}




void bgdef::save()
{
 if (!loaded) return;

 //go to bg directory
 char oldpath[30];
 getcwd(oldpath,30);
 if (chdir(dir)) {msg.printf(5,"error: %s dir does not exist",dir);  return;}

 //save bgimages
 savebgimages("bgimages.map");
 savescrmap("bgscreen.map");
 savebglines("bglines.map");

 msg.printf(1,"%s saved: %d bgimages %d lines",name,numbgimages,numbglines);

 chdir(oldpath);
}

#include "vol.h"
void bgdef::writevol(class volumefile &v)
{
 v.writelistblock("bgscreen.lst",V_SCREEN);

 v.writefile("bgimages.map");
 v.writefile("bglines.map");
 v.writefile("bgscreen.map");
}


//-------------------------------------------
#ifdef __WATCOMC__
#define _USERENTRY
#endif

int _USERENTRY bgimgcmp(bgimage *a,bgimage *b)
{
 if (a->index==0xFF) return 1;
 if (b->index==0xFF) return -1;

 if (a->dispz==b->dispz) return 0;
 if (a->dispz< b->dispz) return -1;
   else return 1;
}

void bgdef::resortbgimages()
{
 if (!bgi) return;

 qsort(bgi,numbgimages,sizeof(bgimage),
   (int (_USERENTRY *)(const void *, const void *))bgimgcmp);
  //shave off 0xff images
 while (bgi[numbgimages-1].index==0xFF)
   numbgimages--;

}




void bgdef::deletebgline(bgline *l)
{
 int index=l-bgl;
 if (index<0 || index>=numbglines) return; //out of range

 memmove(&bgl[index],&bgl[index+1],(numbglines-index)*sizeof(bgline));

 numbglines--;
 bgl=(bgline *)realloc(bgl,numbglines*sizeof(bgline));
}


bgline *bgdef::insertbgline(bgline *l)
{
 int index=numbglines;  //insertion

 numbglines++;
 bgl=(bgline *)realloc(bgl,numbglines*sizeof(bgline));

 memmove(&bgl[index+1],&bgl[index],(numbglines-index-1)*sizeof(bgline));
 bgl[index]=*l;
 return &bgl[index];
}

void bgline::set(int tx1,int ty1,int tx2,int ty2)
{
 if (tx1<tx2)
  {
   x1=tx1; y1=ty1;
   x2=tx2; y2=ty2;
  } else
  {
   x2=tx1; y2=ty1;
   x1=tx2; y1=ty2;
  }
}



void bgdef::deleteimage(bgimage *i)
{
 int index=i-bgi;
 if (index<0 || index>=numbgimages) return; //out of range

 memmove(&bgi[index],&bgi[index+1],(numbgimages-index)*sizeof(bgimage));

 numbgimages--;
 bgi=(bgimage *)realloc(bgi,numbgimages*sizeof(bgimage));
}

bgimage *bgdef::insertimage(bgimage *i)
{
 for (int index=0; index<numbgimages; index++)
   if (i->dispz<bgi[index].dispz) break;

 numbgimages++;
 bgi=(bgimage *)realloc(bgi,numbgimages*sizeof(bgimage));


 memmove(&bgi[index+1],&bgi[index],(numbgimages-index-1)*sizeof(bgimage));
 bgi[index]=*i;
 return &bgi[index];
}



//------------------------------------------------------------

//------------------------------------------------------
//enum dir func
static int scrload(char *name,bgdef *bdf)
{
 char fname[20];
 //get name without extension
 strcpy(fname,name);
 *strchr(fname,'.')=0;

 for (int i=0; i<bdf->numscr; i++)
   if (!stricmp(bdf->screennames[i],fname)) break; //we found it

  if (i==bdf->numscr) //was not found
   {
    strcpy(bdf->imgnames[bdf->numscr++],fname);
    if (bdf->numscr==255) {msg.printf(5,"error: too many scr's\n"); return 0;}
   }
  //read screen
 char scrname[16];
 strcpy(scrname,fname);
 strcat(scrname,".scr");

 //just read it
 bdf->scr[i]=loadscreen(scrname);
 bdf->mem+=bdf->scr[i]->size;

 return 1; //continue
}
defname *readlistfile(char *lstfile,int &num);
void writelistfile(char *lstfile,defname *n,int num);
int  bgdef::readscreens(char *lstname)
{
 scr=(SCR **)calloc(256,4);
 if (!scr) return 0;

  //read all screen names
 screennames=readlistfile(lstname,numscr);
 if (!screennames) return 0;

 //read all screens in directory
 enumdir("*.scr",(DIRFUNCPTR)scrload,this);

 //rewrite screen names
 writelistfile(lstname,screennames,numscr);

  //reallocate
 screennames=(defname *)realloc(screennames,numscr*sizeof(defname));
 scr=(SCR **)realloc(scr,numscr*4);
 return 1;
}








