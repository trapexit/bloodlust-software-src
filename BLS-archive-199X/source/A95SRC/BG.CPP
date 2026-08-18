#include <string.h>
#include <stdlib.h>

#include "bg.h"
#include "r2img.h"
#include "font.h"
#include "dd.h"
#include "objspace.h"
#include "input.h"
#include "message.h"

#include "object.h"

//projection
#define Zx(z)    (z+64)/64
#define invZx(z) 64/(z+64)

#define Zy(z)    (z+128)/128
#define invZy(z) 128/(z+128)


#define CHUNKXW 64
#define CHUNKYW 64

#define PARALLAX


bgobject::bgobject(bgdef *tbd,int tx,int ty)
 :bgnum(tbd->onum),bd(tbd),x(tx<<16),y(ty<<16)
{
 osp=bd->osp; //objectspace
 dx=dy=0;

 updated=0; active=0;
 track=0;

 bgiext=0;
 c=0; cxlen=cylen=0;
 bg=0;

 r[0]=r[1]=r[2]=0;

 #ifdef ANIMATOR
 emode=BGE_NONE;
 selbgline=0;
 #endif

 if (!bd->loaded) bd->read();
 if (bd->loaded)
  {
   activate();
   osp->bg=this;
  }
}

bgobject::~bgobject()
{
 osp->bg=0;
 deactivate();
 bd->kill();
}


void bgobject::activate()
{
 if (active) deactivate();
 calcextent(); //calculate visual extent of all images
 createchunks();

 xw=osp->width(); yw=osp->height();
 bg=new surface(xw,yw);

 #ifdef ANIMATOR
 r[0]=r[1]=r[2]=0;
 #endif
 updated=1;
 active=1;
}

void bgobject::deactivate()
{
 if (!active) return;

 if (bgiext) free(bgiext); bgiext=0; //free extent list

 //free chunks
 if (c)
  {
   for (int i=0; i<cxlen*cylen; i++) c[i].destroy();
   free(c);
  }
 c=0; cxlen=cylen=0;

 //free secondary bg buffer
 if (bg) delete bg; bg=0;

 active=0;
}


//--------------------------------------------------------
//hittesting
#ifdef ANIMATOR
inline int bgimage::hittest(IMG **id,int tx,int ty,int mx,int my)
{
 if (index==0xFF) return 0;

 int x1,x2,y1,y2;
 #ifdef PARALLAX
 x1=dispx-tx*Zx(dispz); x2=x1+id[index]->xw;
 y1=dispy-ty*Zy(dispz); y2=y1+id[index]->yw;
 #else
 x1=dispx-tx; x2=x1+id[index]->xw;
 y1=dispy-ty; y2=y1+id[index]->yw;
 #endif

 return (mx>=x1 && mx<x2 && my>=y1 && my<y2);
}

bgimage *bgchunk::hittest(bgobject *bg,int x,int y,int mx,int my)
{
 bgdef *bd=bg->bd;
 for (int i=numi-1; i>=0; i--)
  if (bd->bgi[idx[i]].hittest(bd->id,x,y,mx,my))
        return &bd->bgi[idx[i]];
 return 0;
}

bgimage *bgobject::hittest(int mx,int my)
{
// if (active) return 0;
 //find coordinates relative to our base
 int tx=x>>16,ty=y>>16;

 //test every image
 if (!active)
 {
  for (int j=bd->numbgimages-1; j>=0; j--)
   if (bd->bgi[j].hittest(bd->id,tx,ty,mx,my))  return &bd->bgi[j];
 } else //test only chunk
 {
  int cx=(tx-ext.x1);  if (cx<0) return 0;
  int cy=(ty-ext.y1);  if (cy<0) return 0;
  cx/=CHUNKXW; cy/=CHUNKYW;
  if (cx>=0 && cy>=0 && cx<cxlen && cy<cylen) //clip chunk
  return c[cy*cxlen+cx].hittest(this,tx,ty,mx,my);
 }

return 0;
}

inline int bgline::hittest(int mx,int my)
{
 if (mx>=x1 && mx<=x2)
  if (y1<y2) return (my>=y1 && my<=y2);
        else return (my>=y2 && my<=y1);
 else return 0;
}
bgline *bgobject::linehittest(int mx,int my)
{
 int tx=x>>16,ty=y>>16;
 //test every line
 for (int j=0; j<bd->numbglines; j++)
  if (bd->bgl[j].hittest(mx+tx,my+ty))  return &bd->bgl[j];
return 0;
}
#endif


//------------------------------------------------
//extent

//calculate visual extents of this image in bg
void bgimage::calcextent(lrect &r,bgobject *bg)
{
 int sxw=bg->osp->width();
 int syw=bg->osp->height();
 IMG *i=bg->bd->id[index];

 #ifdef PARALLAX
 r.x1=(dispx-sxw)*invZx(dispz); r.x2=(dispx+i->xw)*invZx(dispz);
 r.y1=(dispy-syw)*invZy(dispz); r.y2=(dispy+i->yw)*invZy(dispz);
 #else
 r.x1=dispx-sxw; r.x2=dispx+i->xw;
 r.y1=dispy-syw; r.y2=dispy+i->yw;
 #endif
}

void bgobject::calcextent()
{
 if (bgiext) free(bgiext); //free old array if exists

 if (!bd->numbgimages) return;

 bgiext=(lrect *)calloc(bd->numbgimages,sizeof(lrect));

  //find extents of every single image
 for (int i=0; i<bd->numbgimages; i++)
  if (bd->bgi[i].index!=0xFF)
   bd->bgi[i].calcextent(bgiext[i],this);

   //find total extents
 ext=bgiext[0];
 for (i=1; i<bd->numbgimages; i++)
  {
   if (bgiext[i].x1<ext.x1) ext.x1=bgiext[i].x1;
   if (bgiext[i].x2>ext.x2) ext.x2=bgiext[i].x2;
   if (bgiext[i].y1<ext.y1) ext.y1=bgiext[i].y1;
   if (bgiext[i].y2>ext.y2) ext.y2=bgiext[i].y2;
  }
// msg.printf(2,"bg extent: (%d,%d)(%d,%d)",ext.x1,ext.y1,ext.x2,ext.y2);
}


//-------------------------------------------------------
//chunking

void bgchunk::create(bgobject *bg,int x1,int y1, int xw,int yw)
{
 numi=0;
 idx=(int *)malloc(1024*sizeof(int));

 int x2=x1+xw,y2=y1+yw;

 //go through every single extent
 lrect *r=bg->bgiext;
 for (int j=0; j<bg->bd->numbgimages; j++,r++)
   if (x1<=r->x2 && x2>=r->x1 && y1<=r->y2 && y2>=r->y1) //chunk contains image extent...
     idx[numi++]=j;

 idx=(int *)realloc(idx,numi*sizeof(int));

 for (firstfg=0; firstfg<numi && bg->bd->bgi[idx[firstfg]].dispz<=0; firstfg++);
}


void bgchunk::destroy()
{
 if (idx) free(idx);
}



void bgobject::createchunks()
{
 cxlen=((ext.x2-ext.x1)/CHUNKXW)+1; //number of chunks horiz
 cylen=((ext.y2-ext.y1)/CHUNKYW)+1; //number of chunks vert

 //allocate array of bg chunks
 c=(bgchunk *)malloc(cxlen*cylen*sizeof(bgchunk));

 //create each chunk now
 for (int y=0; y<cylen; y++)
  for (int x=0; x<cxlen; x++)
   c[y*cxlen+x].create(this,ext.x1+x*CHUNKXW,ext.y1+y*CHUNKYW,CHUNKXW,CHUNKYW);

// msg.printf(2,"cxlen=%d cylen=%d",cxlen,cylen);
}



//--------------------------------------------------------
//scrolling

int bgobject::scroll()
{

 if (dx) {x+=dx; if (x<0) {dx-=x; x=0;}}
 if (dy) {y+=dy; if (y<0) {dy-=y; y=0;}}

 if (!dx && !dy) return 0; //no updating

 //move all objects
 for (object *t=osp->o; t; t=t->next)
    if (t->active) {t->x-=dx; t->y-=dy; t->basey-=dy;}
 dx=dy=0;
 return 1;
}

void bgobject::tick()
{
// if (!active) return;
 #ifdef ANIMATOR
 track=active ? inputdevice[0]->o : 0;
 #endif
 if (track)
  {
   int d;
   d=(track->x>>16)-0; //distance from left edge
   if (d<90) {dx-=1<<16;
   if (d<50) {dx-=1<<16;
   if (d<20) {dx-=1<<16;}}}

   d=osp->width()-(track->x>>16); //distance from right edge
   if (d<90) {dx+=1<<16;
   if (d<50) {dx+=1<<16;
   if (d<20) {dx+=1<<16;}}}

   d=(track->y>>16)-0; //distance from top
   if (d<70) {dy-=1<<16;
   if (d<40) {dy-=1<<16;
   if (d<10) {dy-=1<<16;}}}

   d=osp->height()-(track->y>>16); //distance from bottom edge
   if (d<50) {dy+=1<<16;
   if (d<35) {dy+=1<<16;
   if (d<15) {dy=(15-d)<<16;}}}
  }
}





//--------------------------------------------------------
//drawing


inline void bgline::draw(bgobject *bg,char *dest,int color)
{
 if (!type) return;
 int x=bg->x>>16;
 int y=bg->y>>16;

 line(dest,x1-x,y1-y,x2-x,y2-y,color);
}

int bgimagesdrawn=0;
inline void bgimage::draw(IMG **id,char *dest,int x,int y)
{
 if (index!=0xFF)
 {
  #ifdef PARALLAX
  if (abs(dispz)<5)
         id[index]->draw(dest,dispx-x,dispy-y,orient);
    else id[index]->draw(dest,dispx-x*Zx(dispz),dispy-y*Zy(dispz),orient);
  #else
    id[index]->draw(dest,dispx-x,dispy-y,orient);
  #endif
  bgimagesdrawn++;
 }
}

inline void bgchunk::drawbg(bgobject *bg,char *dest,int x,int y)
{
 bgdef *bd=bg->bd;
 for (int i=0; i<firstfg; i++)
   bd->bgi[idx[i]].draw(bd->id,dest,x,y);
}

inline void bgchunk::drawfg(bgobject *bg,char *dest,int x,int y)
{
 bgdef *bd=bg->bd;
 for (int i=firstfg; i<numi; i++)
   bd->bgi[idx[i]].draw(bd->id,dest,x,y);
}


inline void bgobject::drawchunkbg(char *dest,int x,int y)
{
 int cx=(x-ext.x1);//CHUNKXW;
 if (cx<0) return;
 int cy=(y-ext.y1);//CHUNKYW;
 if (cy<0) return;
 cx/=CHUNKXW; cy/=CHUNKYW;
 if (cx>=0 && cy>=0 && cx<cxlen && cy<cylen) //clip chunk
   c[cy*cxlen+cx].drawbg(this,dest,x,y);
}

inline void bgobject::drawchunkfg(char *dest,int x,int y)
{
 int cx=(x-ext.x1);//CHUNKXW;
 if (cx<0) return;
 int cy=(y-ext.y1);//CHUNKYW;
 if (cy<0) return;
 cx/=CHUNKXW; cy/=CHUNKYW;
 if (cx>=0 && cy>=0 && cx<cxlen && cy<cylen) //clip chunk
   c[cy*cxlen+cx].drawfg(this,dest,x,y);
}





inline void bgobject::drawscr(char *dest,int sx,int sy,int x,int y)
{
 if (sx>=0 && sy>=0 && sx<SCRMAPXW && sy<SCRMAPYW)
    bd->scr[(*bd->scrmap)[sx][sy]]->draw(dest,x,y);
// bd->scr[0]->draw(dest,x,y);
}

void bgobject::drawbgscr(char *dest)
{
 int tx=x>>19,ty=y>>19;
 for (int iy=ty/200,fy=-(ty%200); fy<SCREENY; fy+=200,iy++)
  for (int ix=tx/320,fx=-(tx%320); fx<SCREENX; fx+=320,ix++)
   drawscr(dest,ix,iy,fx,fy);

/*
int left=(x>>19)-320,top=(y>>19)-200;
int right=left+SCREENX,bottom=top+SCREENY;
for (int y=top; y<bottom; y+=200)
 for (int x=left; x<right; x+=320)
     drawscr(dest,x/320,y/200,x,y-top);*/
}



void bgobject::drawbg(char *dest)
{
 if (!bd->loaded) return;
 int tx=x>>16,ty=y>>16;

 drawbgscr(dest);


 //draw every single image
 if (!active)
 {
  bgimage *i=bd->bgi;
  int j=bd->numbgimages;
  for (; j>0; j--,i++) i->draw(bd->id,dest,tx,ty);
 } else drawchunkbg(dest,tx,ty);  //draw only chunk
}


void bgobject::drawfg(char *dest)
{
 if (!bd->loaded) return;
 int tx=x>>16,ty=y>>16;

 bgimagesdrawn=0;

 if (!active)
 {

 } else drawchunkfg(dest,tx,ty);  //draw only chunk

// font[0]->printf(55,75,"fgimgs: %d",bgimagesdrawn);
}

void bgobject::drawbglines(char *dest)
{
 for (int i=0; i<bd->numbglines; i++)  bd->bgl[i].draw(this,dest,1);
 if (selbgline) selbgline->draw(this,dest,2);
}


extern int alwaysredrawbg,showbglines;
void bgobject::refreshbg(char *dest)
{
 if (!bd->loaded) return;
 bgimagesdrawn=0;

 if (!active || alwaysredrawbg)
 {
  scroll();
  drawbg(dest);
  updated=0;
 }

 else
 if (scroll()) //did we move?
  {
   drawbg(dest); //draw directly to screen
   updated=5;
   r[0]++;
  }
 else
 if (updated>0) //keep drawing direct
  {
   drawbg(dest); //draw directly to screen
   updated--;
   r[0]++;
  }
  else
  {
   if (updated==0) //refresh secondary buffer
   {
    drawbg(bg->lock()); //draw to secondary buffer
    bg->unlock();
    r[1]++;
   } else r[2]++;

   bg->blt(dest,osp->x1,osp->y1);
   updated=-1;
  }

  //draw lines
#ifdef ANIMATOR
  if (emode==BGE_LINES || showbglines) drawbglines(dest);
#else
  if (showbglines) drawbglines(dest);
#endif  



// font[0]->printf(55,55,"%d %d %d",r[0],r[1],r[2]);
// font[0]->printf(55,65,"bgimgs: %d",bgimagesdrawn);
}



//-------------------------------
//basey crap

#define MAXBASEY 0x7FFF


//calculates what the y value of a an x coord will be on this slope
inline int bgline::calcy(int tx)
{
 return y1+(tx-x1)*(y2-y1)/(x2-x1);
}

inline int bgline::getbasey(int tx,int ty)
{
 if (!type) return MAXBASEY;
 //check left/right clipping
 if (tx<x1 || tx>x2) return MAXBASEY;

 //return x coordinate then
 return calcy(tx);
}


bgline *bgobject::getbasey(int tx,int ty,int &by)
{
 tx=(tx+x)>>16;
 ty=(ty+y)>>16;

 int i=bd->numbglines;
 bgline *p=bd->bgl;

 int basey=MAXBASEY;
 bgline *bglbase=0;
 for ( ; i>0; i--,p++)
  {
   int newbasey=p->getbasey(tx,ty);
   if (newbasey<basey && ty<=newbasey+3) //&& newbasey>ty-3 //&& newbasey<ty)
    {
     basey=newbasey;
     bglbase=p;
    }
  }

 by=(basey<<16)-y; //bglbase ? (basey<<16)-y : MAXBASEY<<16;
 return bglbase;
}




int object::getbasey()
{
 //if no background loaded, default to baseline of y=160
 if (/*!active || */!osp->bg) return basey=(osp->height()*5/6)<<16;

 //if never hits ground (ex bullet) basey is max
 if (cs && cs->parm&SP_NOGROUND) {slope=0; return basey=MAXBASEY<<16;}


 //we are already standing on a slope
 if (slope && y==basey)
  {  //get basey of new slope
   basey=slope->getbasey((x+osp->bg->x)>>16,0)<<16;
   if (basey!=MAXBASEY<<16)
    {
     basey-=osp->bg->y;
     //if (basey-(3>>16)>y)
     return basey;
    }
  }

 //search basey and slope that this object is over
 slope=osp->bg->getbasey(x,y,basey);

 if (this==osp->p) osp->bg->selbgline=slope;
 return basey;
}












