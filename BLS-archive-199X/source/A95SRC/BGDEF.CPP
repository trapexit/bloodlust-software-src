#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>


#include "file.h"
#include "message.h"

#include "objdef.h"
#include "bgdef.h"

#include "r2img.h"
#include "vol.h"


#ifdef ANIMATOR
bgdef::bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile,char *tsdf,char *tdir)
 :objectdefw(tosp,tnum,tname,tvolfile,tsdf,tdir)
#else
bgdef::bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile)
 :objectdef(tosp,tnum,tname,tvolfile)
#endif
{
 numbgimages=0; bgi=0;
 numscr=0; scr=0; screennames=0;
 scrmap=0;
 numbglines=0; bgl=0;
}

void bgdef::freeodf()
{
 if (bgi) free(bgi); bgi=0; numbgimages=0;

 if (scr)
 {
  if (isvol)
  for (int i=0; i<numscr; i++)
     if (scr[i]) free(scr[i]);
  free(scr);
 }
 scr=0; numscr=0;
 if (screennames) free(screennames); screennames=0;

 if (scrmap) free(scrmap); scrmap=0;

 if (bgl) free(bgl); bgl=0;
 numbglines=0;

 #ifdef ANIMATOR
 objectdefw::freeodf();
 #else
 objectdef::freeodf();
 #endif
}

#ifndef ANIMATOR
//read from bgvol
int bgdef::read()
{

//read it from volumefile
mem=0;
volumefile v;
//open volfile
if (v.open(volfile)) {msg.printf(5,"error: cannot open %s",volfile);return 0;}

//read volname
char vname[32];
v.read(vname);
if (strcmp(vname,name))
  { msg.printf(5,"error: corrupt %s",volfile); v.close(); return 0;}

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

//read screens
scr=(SCR **)v.readblock(numscr);
mem+=v.hdr.size;

//read bgimages
v.readheader();
numbgimages=v.f.readint();
v.f.readint(); //skip size
bgi=(bgimage *)v.f.readalloc(numbgimages*sizeof(bgimage));

//read bglines
v.readheader();
numbglines=v.f.readint();
v.f.readint(); //skip size
bgl=(bgline *)v.f.readalloc(numbglines*sizeof(bgline));

//read bgscrmap
v.readheader();
scrmap=(uchar (*)[SCRMAPXW][SCRMAPYW])v.f.readalloc(SCRMAPXW*SCRMAPYW);

v.close();

//msg.printf(2,"%d %p",numscr,scr);
//msg.printf(1,"numbgl=%d numbgi=%d",numbglines,numbgimages);
msg.printf(1,"%s loaded from %s %dKB",name,volfile,mem/1024);
loaded=1; //set loaded
readonly=1;
isvol=1;
return 1;
}
#endif







