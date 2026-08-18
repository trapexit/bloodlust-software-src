#include <string.h>
#include <stdlib.h>

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif


#include "bg.h"
#include "r2img.h"
#include "font.h"
#include "dd.h"
#include "objspace.h"
#include "input.h"
#include "message.h"

#include "object.h"

#include "a32.h"

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
 b[1]=b[2]=b[3]=b[4]=0;

 #ifdef ANIMATOR
 emode=BGE_NONE;
 etype=1;
 selbgline=0;
 #endif

 if (!bd->loaded) bd->read();
 if (bd->loaded)
  {
   osp->bg=this;
   activate();
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

 xw=osp->width(); yw=osp->height();

 calcextent(); //calculate visual extent of all images
 createchunks();
 if (bgiext) free(bgiext); bgiext=0; //free extent list

 bg=new surface(xw,yw);

 #ifdef ANIMATOR
 r[0]=r[1]=r[2]=0;
 #endif

 b[1]=b[2]=b[3]=b[4]=0;
 dx=dy=-1;
 boundcheck();
 scroll();
 updated=1;
 active=1;

// msg.printf(1,"cxlen=%d cylen=%d",cxlen,cylen);
}

void bgobject::deactivate()
{
 if (!active) return;

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
  bgchunk *x=getchunkptr(tx,ty);
  if (x) return x->hittest(this,tx,ty,mx,my);
 }

return 0;
}

inline int bgline::hittest(int mx,int my)
{
 switch(type)
 {
  case BGL_FLOOR:
  case BGL_CEILING:
   if (mx>=x1 && mx<=x2)
    if (y1<y2) return (my>=y1 && my<=y2);
         else return (my>=y2 && my<=y1);
   else return 0;

  case BGL_WALLLEFT:
   if (my>=y1 && my<=y2)
    if (x1<x2) return (mx>=x1-10 && mx<=x2);
          else return (mx>=x2-10 && mx<=x1);
   else return 0;

  case BGL_WALLRIGHT:
   if (my>=y1 && my<=y2)
    if (x1<x2) return (mx>=x1 && mx<=x2+10);
          else return (mx>=x2 && mx<=x1+10);
   else return 0;
 };

 return 0;
}
bgline *bgobject::linehittest(int mx,int my)
{
 int tx=x>>16,ty=y>>16;
 //test every line
 for (int j=0; j<bd->numbglines; j++)
  if (bd->bgl[j].hittest(mx+tx,my+ty))  return &bd->bgl[j];
return 0;
}



inline int boundary::hittest(int mx,int my)
{
 switch(type)
 {
  case BGB_BOTTOM:
    return (mx>=x1 && mx<=x2 && my>=y1 && my<=y1+10);
  case BGB_TOP:
    return (mx>=x1 && mx<=x2 && my>=y1-10 && my<=y1);
  case BGB_LEFT:
    return (mx>=x1-10 && mx<=x1 && my>=y1 && my<=y2);
  case BGB_RIGHT:
    return (mx>=x1 && mx<=x1+10 && my>=y1 && my<=y2);
 };
 return 0;
}
boundary *bgobject::boundaryhittest(int mx,int my)
{
 int tx=x>>16,ty=y>>16;
 //test every boundary
 for (int j=0; j<bd->numboundary; j++)
  if (bd->bnd[j].hittest(mx+tx,my+ty))  return &bd->bnd[j];
return 0;
}


#endif


//------------------------------------------------
//extent

//calculate visual extents of this image in bg
void bgimage::calcextent(lrect &r,bgobject *bg)
{
 int xw=bg->osp->width();
 int yw=bg->osp->height();
 IMG *i=bg->bd->id[index];

 #ifdef PARALLAX
 r.x1=(dispx-xw)*invZx(dispz); r.x2=(dispx+i->xw)*invZx(dispz);
 r.y1=(dispy-yw)*invZy(dispz); r.y2=(dispy+i->yw)*invZy(dispz);
 #else
 r.x1=dispx-xw; r.x2=dispx+i->xw;
 r.y1=dispy-yw; r.y2=dispy+i->yw;
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
 int x2=x1+xw,y2=y1+yw;

 //find images in chunk
 numi=0; idx=(int *)malloc(1024*sizeof(int));
 lrect *r=bg->bgiext; //go through every single extent
 for (int j=0; j<bg->bd->numbgimages; j++,r++)
   if (x1<=r->x2 && x2>=r->x1 && y1<=r->y2 && y2>=r->y1) //chunk contains image extent...
     idx[numi++]=j;
 idx=(int *)realloc(idx,numi*sizeof(int));

 for (firstfg=0; firstfg<numi && bg->bd->bgi[idx[firstfg]].dispz<=0; firstfg++);

 //find bglines in chunk
 numbglines=0; bgl=(int *)malloc(1024*sizeof(int));
 numbgwalls=0; bgw=(int *)malloc(1024*sizeof(int));
 bgline *l=bg->bd->bgl;
 for (j=0; j<bg->bd->numbglines; j++,l++)
  switch (l->type)
  {
   case BGL_FLOOR:
     if (x1<=l->x2 && x2>=l->x1 && y1<=max(l->y1,l->y2))
       bgl[numbglines++]=j;
     break;
   case BGL_WALLLEFT:
   case BGL_WALLRIGHT:
    if (y1-32<=l->y2 && y2+32>=l->y1 && x1-32<=max(l->x1,l->x2) && x2+32>=min(l->x1,l->x2))
       bgw[numbgwalls++]=j;
    break;
   case BGL_CEILING:
    if (x1-32<=l->x2 && x2+32>=l->x1 && y1-32<=max(l->y1,l->y2) && y2+32>=min(l->y1,l->y2))
       bgw[numbgwalls++]=j;
     break;
  }

 bgl=(int *)realloc(bgl,numbglines*sizeof(int));
 bgw=(int *)realloc(bgw,numbgwalls*sizeof(int));
}


void bgchunk::destroy()
{
 if (idx) free(idx); idx=0;
 if (bgl) free(bgl); bgl=0;
 if (bgw) free(bgw); bgw=0;
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

int boundary::intersect(lrect &r)
{
 if (!this) return 0;
 switch (type)
 {
  case BGB_LEFT:
  case BGB_RIGHT:
   return (x1>r.x1 && x1<r.x2 && y1<r.y2 && y2>r.y1);
  case BGB_TOP:
  case BGB_BOTTOM:
   return (y1>r.y1 && y1<r.y2 && x1<r.x2 && x2>r.x1);
 }
 return 0;
}


int boundary::extent(lrect &r)
{
 if (!this) return 0;
 switch (type)
 {
  case BGB_LEFT:  return (x1<r.x2 && y1<r.y2 && y2>r.y1);
  case BGB_RIGHT: return (x1>r.x1 && y1<r.y2 && y2>r.y1);
  case BGB_TOP:   return (y1<r.y2 && x1<r.x2 && x2>r.x1);
  case BGB_BOTTOM:return (y1>r.y1 && x1<r.x2 && x2>r.x1);
 }
 return 0;
}

//returns the closer of two boundaries
boundary *closerboundary(boundary *a,boundary *b)
{
 if (!a) return b;
 if (!b) return a;
 switch (a->type)
 {
  case BGB_LEFT:  return (a->x1>b->x1) ? a : b;
  case BGB_RIGHT: return (a->x1<b->x1) ? a : b;
  case BGB_TOP:   return (a->y1>b->y1) ? a : b;
  case BGB_BOTTOM:return (a->y1<b->y1) ? a : b;
 }
 return a;
}


/*
void bgobject::boundcheck()
{
 int txw=xw<<16,tyw=yw<<16;

 lrect r; //dimensions of background
 if (dx<0) {r.x1=(x+dx)>>16; r.x2=(x+txw)>>16;}
     else  {r.x1=(x)>>16; r.x2=(x+dx+txw)>>16;}
 if (dy<0) {r.y1=(y+dy)>>16; r.y2=(y+tyw)>>16;}
     else  {r.y1=(y)>>16; r.y2=(y+dy+tyw)>>16;}

 boundary *t[5]={0,0,0,0,0};
 boundary *b=bd->bnd;
 for (int i=0; i<bd->numboundary; i++,b++) //go through all boundaries
   if (b->intersect(r)) t[b->type]=b; //hitting a boundary

 if (t[BGB_LEFT])  {if (dx<0) dx=(t[BGB_LEFT]->x1<<16)-x;} else
 if (t[BGB_RIGHT]) {if (dx>0) dx=(t[BGB_RIGHT]->x1<<16)-x-txw;}
 if (t[BGB_TOP])   {if (dy<0) dy=(t[BGB_TOP]->y1<<16)-y;} else
 if (t[BGB_BOTTOM]) {if (dy>0) dy=(t[BGB_BOTTOM]->y1<<16)-y-tyw;}
}
*/

void bgobject::boundcheck()
{
 int txw=xw<<16,tyw=yw<<16;

 lrect r; //dimensions of background
 if (dx<0) {r.x1=(x+dx)>>16; r.x2=(x+txw)>>16;}
     else  {r.x1=(x)>>16; r.x2=(x+dx+txw)>>16;}
 if (dy<0) {r.y1=(y+dy)>>16; r.y2=(y+tyw)>>16;}
     else  {r.y1=(y)>>16; r.y2=(y+dy+tyw)>>16;}

 //go through all boundaries
 for (int i=1; i<=4; i++)
  if (!b[i] || !b[i]->extent(r))
    { //find new boundary for this
     b[i]=0;
     boundary *t=bd->bnd;
     for (int j=0; j<bd->numboundary; j++,t++) //go through all boundaries
         if (t->type==i && t->extent(r))
           b[i]=closerboundary(b[i],t);
//     if (b[i]) msg.printf(2,"boundary found");
    }
// if (b[BGB_BOTTOM])
//  msg.printf(2,"bottom at %d",b[BGB_BOTTOM]->y1);

 if (b[BGB_LEFT]->intersect(r))   {if (dx<0) dx=(b[BGB_LEFT]->x1<<16)-x;} else
 if (b[BGB_RIGHT]->intersect(r))  {if (dx>0) dx=(b[BGB_RIGHT]->x1<<16)-x-txw;}
 if (b[BGB_TOP]->intersect(r))    {if (dy<0) dy=(b[BGB_TOP]->y1<<16)-y;} else
 if (b[BGB_BOTTOM]->intersect(r)) {if (dy>0) dy=(b[BGB_BOTTOM]->y1<<16)-y-tyw;}
}


int bgobject::scroll()
{
 if (!dx && !dy) return 0; //no updating

 //check against boundaries
 if (active) boundcheck();

 if (!dx && !dy) return 0; //no updating

 //move bg
 x+=dx; y+=dy;

 //move all objects
// for (object *t=osp->o; t; t=t->next)
//    if (t->active) {t->x-=dx; t->y-=dy; t->basey-=dy;}
 dx=dy=0;
 return 1;
}

void bgobject::tick()
{
 #ifdef ANIMATOR
 track=active ? inputdevice[0]->o : 0;
 #endif

 //do some scrolling....
 if (track)
  {
   int d;
   d=(track->relx()>>16)-0; //distance from left edge
   int instat= track->in ? track->in->stat : 0;

// if (track->d) d-=xw*16/32;
//   if (instat&ID_LEFT) d-=xw*16/32;
   if (instat&ID_LEFT) d-=100;
   if (d<0) d=0;
   if (d<80) dx-=(80-d)*(4<<16)/80;

   d=xw-(track->relx()>>16); //distance from right edge
// if (!track->d) d-=xw*16/32;
//   if (instat&ID_RIGHT) d-=xw*16/32;
   if (instat&ID_RIGHT) d-=100;
   if (d<0) d=0;
   if (d<80) dx+=(80-d)*(4<<16)/80;

   d=(track->rely()>>16)-0; //distance from top
//   if (instat&ID_UP) d-=yw*8/16;
   if (instat&ID_UP) d-=100;
   if (d<0) d=0;
   if (d<50) dy-=(50-d)*(4<<16)/50;

   d=yw-(track->rely()>>16); //distance from bottom edge
//   if (instat&ID_DOWN) d-=yw*8/16;
   if (instat&ID_DOWN) d-=100;
   if (d<0) d=0;
   if (d<50) dy+=(50-d)*(4<<16)/50;
  }

 //kill objects that have fallen
 if (!(uu&255) && b[BGB_BOTTOM])
  {
   int ty=(b[BGB_BOTTOM]->y1+100)<<16; //find lower bounds
   for (object *t=osp->o; t; t=t->next)
     if (t->y>ty)
      {
//       msg.printf(1,"%s fell to its death",t->od->name);
       t->kill();
      }
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
 switch(type)
 {
  case BGL_WALLRIGHT:
     y+=a95vol.lmark->yw/2;
     a95vol.lmark->draw(dest,x1-x,y1-y);
     a95vol.lmark->draw(dest,x2-x,y2-y);
    break;
  case BGL_WALLLEFT:
     x+=a95vol.rmark->xw;
     y+=a95vol.rmark->yw/2;
     a95vol.rmark->draw(dest,x1-x,y1-y);
     a95vol.rmark->draw(dest,x2-x,y2-y);
    break;
 };
}


inline void boundary::draw(bgobject *bg,char *dest)
{
 if (!type) return;
 int x=bg->x>>16;
 int y=bg->y>>16;
 line(dest,x1-x,y1-y,x2-x,y2-y,4);
 switch (type)
 {
  case BGB_BOTTOM:
     x+=a95vol.umark->xw/2;
     a95vol.umark->draw(dest,x1-x,y1-y);
     a95vol.umark->draw(dest,(x1+x2)/2-x,y1-y);
     a95vol.umark->draw(dest,x2-x,y1-y);
    break;
  case BGB_TOP:
     x+=a95vol.dmark->xw/2;
     y+=a95vol.dmark->yw;
     a95vol.dmark->draw(dest,x1-x,y1-y);
     a95vol.dmark->draw(dest,(x1+x2)/2-x,y1-y);
     a95vol.dmark->draw(dest,x2-x,y1-y);
    break;
  case BGB_RIGHT:
     y+=a95vol.lmark->yw/2;
     a95vol.lmark->draw(dest,x1-x,y1-y);
     a95vol.lmark->draw(dest,x1-x,(y1+y2)/2-y);
     a95vol.lmark->draw(dest,x1-x,y2-y);
    break;
  case BGB_LEFT:
     x+=a95vol.rmark->xw;
     y+=a95vol.rmark->yw/2;
     a95vol.rmark->draw(dest,x1-x,y1-y);
     a95vol.rmark->draw(dest,x1-x,(y1+y2)/2-y);
     a95vol.rmark->draw(dest,x1-x,y2-y);
    break;

 }
}





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


//returns pointer to the chunk containing bg coords(x,y)
bgchunk *bgobject::getchunkptr(int x,int y)
{
 int cx=(x-ext.x1);//CHUNKXW;
 if (cx<0) return 0;
 int cy=(y-ext.y1);//CHUNKYW;
 if (cy<0) return 0;
 cx/=CHUNKXW; cy/=CHUNKYW;
 if (cx>=0 && cy>=0 && cx<cxlen && cy<cylen) //clip chunk
       return &c[cy*cxlen+cx];
 return 0;
}

inline void bgobject::drawscr(char *dest,int sx,int sy,int x,int y)
{
 if (sx>=0 && sy>=0 && sx<SCRMAPXW && sy<SCRMAPYW)
    bd->scr[(*bd->scrmap)[sx][sy]]->draw(dest,x,y);
// bd->scr[0]->draw(dest,x,y);
}

void bgobject::drawbgscr(char *dest)
{
// return ;
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
 if (!dest) return;
 if (!bd->loaded) return;
 int tx=x>>16,ty=y>>16;

 drawbgscr(dest);

 //draw every single image
 if (!active)
 {
  bgimage *i=bd->bgi;
  int j=bd->numbgimages;
  for (; j>0; j--,i++) i->draw(bd->id,dest,tx,ty);
 } else
 {
  bgchunk *x=getchunkptr(tx,ty);
  if (x) x->drawbg(this,dest,tx,ty);
/*  font[0]->printf(55,65,"bgimgs:  %d",x->numi);
  font[0]->printf(55,75,"bglines: %d",x->numbglines);
  font[0]->printf(55,85,"bgwalls: %d",x->numbgwalls);
*/
 }
}


void bgobject::drawfg(char *dest)
{
 if (!bd->loaded) return;
 int tx=x>>16,ty=y>>16;


 if (!active)
 {
 } else
 {
  bgchunk *x=getchunkptr(tx,ty);
  if (x) x->drawfg(this,dest,tx,ty);
 }
}

int bglinecolor[]={0,1,5,5,6};
void bgobject::drawbglines(char *dest)
{
 for (int i=0; i<bd->numbglines; i++)
   bd->bgl[i].draw(this,dest,bglinecolor[bd->bgl[i].type]);
 if (selbgline) selbgline->draw(this,dest,2);
}

void bgobject::drawboundary(char *dest)
{
 for (int i=0; i<bd->numboundary; i++)  bd->bnd[i].draw(this,dest);
}



extern int alwaysredrawbg,showbglines;
void bgobject::refreshbg(char *dest)
{
 if (!bd->loaded) return;
 if (!active) // || alwaysredrawbg)
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

   if (!bg->blt(dest,osp->x1,osp->y1)) updated=0; //blt didn't work
     else updated=-1;
  }

  //draw lines
#ifdef ANIMATOR
  if (emode==BGE_LINES || showbglines) drawbglines(dest);
  if (!active || showbglines) drawboundary(dest);
#else
  if (showbglines) drawbglines(dest);
#endif


// font[0]->printf(55,55,"%d,%d",x>>16,y>>16);
// font[0]->printf(55,55,"%d %d %d",r[0],r[1],r[2]);
}



//-------------------------------
//basey crap

#define MAXBASEY 0x7FFF



//calculates what the y value of a an x coord will be on this slope
int bgline::calcy(int tx)
{
 return y1+(tx-x1)*(y2-y1)/(x2-x1);
}

//calculates what the x value of a an y coord will be on this slope
int bgline::calcx(int ty)
{
 return x1+(ty-y1)*(x2-x1)/(y2-y1);
}


int bgline::wallleftintersect(lrect &r)
{
 if (y1>r.y2 || r.y1>y2) return 0;
 return x1>=r.x1 && x1<=r.x2;
// return x1<x2 ? (x1<=r.x2 && r.x1<=x2) : (x2<=r.x2 && r.x1<=x1);
}

int bgline::wallrightintersect(lrect &r)
{
 if (y1>r.y2 || r.y1>y2) return 0;
 return x1>=r.x1 && x1<=r.x2;
// return x1<x2 ? (x1<=r.x2 && r.x1<=x2) : (x2<=r.x2 && r.x1<=x1);
}




int bgline::ceilingintersect(lrect &r)
{
 int tx=(r.x1+r.x2)/2;

// if (x1>r.x2 || r.x1>x2) return 0;
 if (x1>tx || tx>x2) return 0;
// if (y1<y2 ? (y1>r.y2 || r.y1>y2) : (y2>r.y2 || r.y1>y1)) return 0;
 if (y1<y2)
  {
   if (y1>r.y2) return 0;
   if (y2<r.y1) return 0;

  } else
  {
   if (y2>r.y2) return 0;
   if (y1<r.y1) return 0;
  }

 int ty=calcy(tx);
 if (ty<r.y1) return 0; //we're below the line
 if (ty>(r.y1+r.y2)/2) return 0;

 return 1;
 /*
 int ty1=calcy(r.x1);
 int ty2=calcy(r.x2);
 if (r.y1>ty1 && r.y1>ty2) return 0;
 if (r.y2<ty1 && r.y2<ty2) return 0;
 return 1;*/
}


inline int bgline::getbasey(int tx,int ty)
{
 //check left/right clipping
 if (tx<x1 || tx>x2) return MAXBASEY;

 //return x coordinate then
 return calcy(tx);
}


//see if rectangle intersects a particular wall
bgline *bgobject::hitwallleft(lrect &r)
{
 if (!active)
 {
  int i=bd->numbglines;
  bgline *p=bd->bgl;
  for ( ; i>0; i--,p++)
    if (p->type==BGL_WALLLEFT && p->wallleftintersect(r)) return p;
 } else
 {
  bgchunk *x=getchunkptr(r.x1,r.y2);
  if (x)
   for (int i=0; i<x->numbgwalls; i++)
     {
      bgline *p=&bd->bgl[x->bgw[i]];
      if (p->type==BGL_WALLLEFT && p->wallleftintersect(r)) return p;
     }
 }
 return 0;
}

bgline *bgobject::hitwallright(lrect &r)
{
 if (!active)
 {
  int i=bd->numbglines;
  bgline *p=bd->bgl;
  for ( ; i>0; i--,p++)
    if (p->type==BGL_WALLRIGHT && p->wallrightintersect(r)) return p;
 } else
 {
  bgchunk *x=getchunkptr(r.x2,r.y2);
  if (x)
   for (int i=0; i<x->numbgwalls; i++)
     {
      bgline *p=&bd->bgl[x->bgw[i]];
      if (p->type==BGL_WALLRIGHT && p->wallrightintersect(r)) return p;
     }
 }
 return 0;
}


bgline *bgobject::hitceiling(lrect &r)
{
 if (!active)
 {
  int i=bd->numbglines;
  bgline *p=bd->bgl;
  for ( ; i>0; i--,p++)
    if (p->type==BGL_CEILING && p->ceilingintersect(r)) return p;
 } else
 {
  bgchunk *x=getchunkptr(r.x1,r.y1);
  if (x)
   for (int i=0; i<x->numbgwalls; i++)
     {
      bgline *p=&bd->bgl[x->bgw[i]];
      if (p->type==BGL_CEILING && p->ceilingintersect(r))  return p;
     }
 }
 return 0;
}




bgline *bgobject::getbasey(int tx,int ty,int &by)
{
// tx=(tx+x)>>16;
// ty=(ty+y)>>16;
 tx>>=16;
 ty>>=16;

 int basey=MAXBASEY;
 bgline *bglbase=0;

// msg.printf(2,"getbasey %d %d",tx,ty);
 if (!active) //scan all bg lines
  {
   int i=bd->numbglines;
   bgline *p=bd->bgl;
   for ( ; i>0; i--,p++)
    if (p->type==BGL_FLOOR)
    {
     int newbasey=p->getbasey(tx,ty);
     if (newbasey<basey && ty<=newbasey+3) //&& newbasey>ty-3 //&& newbasey<ty)
      {basey=newbasey; bglbase=p;}
    }
  } else //go through lines in chunk
  {
   bgchunk *x=getchunkptr(tx,ty);
   if (x)
    for (int i=0; i<x->numbglines; i++)
      {
       bgline *p=&bd->bgl[x->bgl[i]];
       if (p->type==BGL_FLOOR)
       {
        int newbasey=p->getbasey(tx,ty);
        if (newbasey<basey && ty<=newbasey+3)
         {basey=newbasey; bglbase=p;}
       }
      }
  }

 by=(basey<<16); //-y; //bglbase ? (basey<<16)-y : MAXBASEY<<16;
// by=bglbase ? (basey<<16)-y : MAXBASEY<<16;

 return bglbase;
}


void object::forcefloor()
{
 z=0;
 y=getbasey();
 if (y>=(MAXBASEY<<16)) //y=((osp->bg->y>>16)+osp->height())*5/6;
  setrely((osp->height()*5/6) <<16);
}

int object::getbasey()
{
 //if no background loaded, default to baseline of y=160
 if (/*!active || */!osp->bg) return basey=(osp->height()*5/6)<<16;

 //if never hits ground (ex bullet) basey is max
 if (cs && cs->parm&SP_NOGROUND) {floor=0; return basey=MAXBASEY<<16;}


 //we are already standing on a floor
 if (floor && y==basey)
  {  //get basey of new slope
   basey=floor->getbasey(x>>16,0)<<16;
   if (basey!=MAXBASEY<<16) return basey;
  }

 //search basey and slope that this object is over
 floor=osp->bg->getbasey(x,y,basey);

 if (this==osp->p) osp->bg->selbgline=floor;
 return basey;
}












