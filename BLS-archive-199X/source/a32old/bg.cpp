//background functions
#include <i86.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define ANIMATOR

#include "a32.h"

#include "ttimer.h"

extern bgobject *bgobj;

#define Z(z)    (z+64)/64
#define invZ(z) 64/(z+64)


int bgy1,bgy2;

int object::getbasey()
{
 if (!fptr) return basey=160<<16;
  
 //if never hits ground (ex bullet) basey is max
 if (cs->parm&SP_NOGROUND)
  {slope=0; return basey=0x7FFF<<16;}
  
 //if no background loaded, default to baseline of y=160
 if (!bgobj)
  {slope=0; return basey=160<<16;}


 //we are already standing on a slope
 if (slope && y==basey) 
  {  //get basey of new slope
   basey=slope->getbasey((x+bgobj->x)>>16,0)<<16;
   if (basey!=0x7FFF<<16) return basey;
  }

 //search basey and slope that this object is over
 slope=bgobj->getbasey(x,y,&basey);
 return basey;

/*
 int x1,x2,y1,y2;
 bgline *slope1,*slope2;
 x1=x2=x;

 //calculate the two x coords of this obj's bounding region
 if (!r[R_IMPERM])
  {
   if (!d)
   {x1-=10<<16; x2+=10<<16;}//make artificial bounding
    else
   {x1-=10<<16; x2+=10<<16;}//make artificial bounding
  } else
  {
   if (!d)
    {
     x1+=r[R_IMPERM]->r->r.p1.x<<16;
     x2+=r[R_IMPERM]->r->r.p2.x<<16;
    } else
    {
     x2-=r[R_IMPERM]->r->r.p1.x<<16;
     x1-=r[R_IMPERM]->r->r.p2.x<<16;
    }    
  }

 //get the y coords
 slope1=bgobj->getbasey(x1,y,&y1);
 slope2=bgobj->getbasey(x2,y,&y2);
 bgy1=y1>>16; bgy2=y2>>16;

 //standing on same slope
 if (slope1==slope2) //both over same slope
  {
   if (!slope1) {slope=0; return basey=0x7FFF<<16;}
   slope=slope1;
   return basey=(y1+y2)/2; //average
  }

 //left edge is hanging off
 if (!slope1)
  {
   slope=slope2;
   return basey=y2;
  }
 //right edge is hanging off
 if (!slope2)
  {
   slope=slope1;
   return basey=y1;
  }
 //one side is on over one slope, other side is over different one
  //find highest slope
 if (y1>y2)
  {
   slope=slope2;
//   if (slope1!=slope2)  return basey=slope->y1<<16;
   return basey=y2;
  } else
  {
   slope=slope1;
//   if (slope1!=slope2)  return basey=slope->y2<<16;
   return basey=y1;
  }
*/
}    



bgline *bgobject::getbasey(int tx,int ty,int *by)
{
 tx=(tx+x)>>16;
 ty=(ty+y)>>16;
 
 int i=((backgrounddef *)od)->numbglines;
 bgline *p=((backgrounddef *)od)->bgl;

 int basey=0x7FFF;
 bgline *bglbase=0;
 for ( ; i>0; i--,p++)
  {
   int newbasey=p->getbasey(tx,ty);
   if (newbasey<basey && newbasey>ty-3)
    {
     basey=newbasey;
     bglbase=p;
    }
  }
 if (by) *by=basey<<16;
 return bglbase;
}


//calculates what the y value of a an x coord will be on this slope
int bgline::calcy(int tx)
{
 return y1+(tx-x1)*(y2-y1)/(x2-x1);
}    

int bgline::getbasey(int tx,int ty)
{
 if (!type) return 0x7FFF;
/* //wall
 if (x1==x2)
  {
   if (tx!=x1) return 0x7FFF;

   if (y1<y2)
    if (ty<y1 || ty>y2) return 0x7FFF; else return y1;
   else
    if (ty<y2 || ty>y1) return 0x7FFF; else return y2;
  }*/

 //check left/right clipping
 if (tx<x1 || tx>x2) return 0x7FFF;

 //return x coordinate then
 return calcy(tx);
}

    


void drawscreen(char *s,char *d,int x,int y)
{
if (x>=320 || y>=200) return;
if (x<=-320 || y<=-200) return;

if (y<0) {s+=-y*320; y+=200;}
   else {d+=y*320; y=200-y;}
if (x<0) {s+=-x; x+=320;}
   else {d+=x; x=320-x;}
if (x<=0) return;
while (y>0)
 {
  memcpy(d,s,x);
  d+=320; s+=320;
  y--;
 }

}    


//************************
//*********bgactiveimage**
//************************
//swaps two objects in order
void swap(bgactiveimage *a,bgactiveimage *b)
{
 if (!a || !b) return;
     
 if (a->prev) a->prev->next=b;
   else bgobj->i=b; //set as first now
 if (b->next) b->next->prev=a;

 a->next=b->next;
 b->prev=a->prev;

 b->next=a;
 a->prev=b;
}

void bgactiveimage::resort()
{
if (prev && (z<prev->z)) //should be moved up in list
 {swap(prev,this); resort(); return;}
 
if (next && (z>next->z)) //should be moved up in list
 {swap(this,next); prev->resort(); return;}
}


//creates an active need-to-be-displayed bgimage from one stored in bgimage list
bgactiveimage::bgactiveimage(int i,bgobject *bgo)
{
o=bgo;
bgimage *b=&((backgrounddef *)o->od)->bgi[i];

//set up image
iptr=o->od->id[b->index]; //get image pointer
orient=b->orient;

z=b->dispz;
x=((b->dispx<<16)-o->x)*Z(z);
y=((b->dispy<<16)-o->y)*Z(z);
//y=(b->dispy<<16)-o->y;

//add to list
o->addactiveimage(this);

idx=i;
o->iarray[i]=this;
}    

void bgobject::addactiveimage(bgactiveimage *x)
{
//find insertion point
for (bgactiveimage *tprev=0,*t=i; t; tprev=t,t=t->next)
  if (t->z>x->z) break;

//x goes after tprev, before t
x->prev=tprev;
x->next=t;
if (tprev) tprev->next=x; else i=x;
if (t)     t->prev=x;
};


bgactiveimage::~bgactiveimage()
{
 o->iarray[idx]=0; //unlink from bg img array
 if (prev) prev->next=next;
 if (next) next->prev=prev;
 if (this==o->i) o->i=next;
}    

//updates an stored array image from it's currently active counterpart
void bgimage::update(bgactiveimage *a)
{
 if (!this) return;
 orient=a->orient;
 dispx=(bgobj->x+a->x*invZ(a->z))>>16; //set x where a->x==0
 dispy=(bgobj->y+a->y*invZ(a->z))>>16; //set y where a->y==0
 dispz=a->z;
}    

#define ROUNDC(x) ((x)&0x8000) ? ((x)>>16)+1 : ((x)>>16)

void bgactiveimage::draw(char *d)
{
 DrawImage(iptr,d,ROUNDC(x),ROUNDC(y),orient);
}

void bgactiveimage::scroll(int dx,int dy)
{
 if (abs(z)<5)
 {
  x-=dx; y-=dy;
 } else
 {
  x-=dx*Z(z);
  y-=dy*Z(z);
 }
 
}

//************************
//*********bgobject*******
//************************


bgobject::bgobject(backgrounddef *bgdef):object(bgdef,-1,0,0,0,0,0)
{
 bg=(char *)malloc(64000);

 dx=0; dy=0; 
 posleft=0; i=0; iarray=0;
 foregroundptr=0;
 moveto(0<<16,0);
 
 playing=1;  active=1;
}    

bgobject::~bgobject()
{
 free();
 ::free(bg);
};

void bgobject::free()
{
 if (!this) return;

 //delete bg active images
 while (i) delete(i);

 //free image array
 if (iarray) ::free(iarray); iarray=0;

 //free pos
 if (posleft) ::free(posleft); posleft=0;
}    

//resets all position structs and images in bg instance
void bgobject::moveto(int px,int py)
{
 if (!this) return;
 _disable();
 x=px; y=py;
 dx=dy=0;

 free();

 //create active image index array
 int numbgimages=((backgrounddef *)od)->numbgimages;
 iarray=(bgactiveimage **)malloc(4*numbgimages);
 memset(iarray,0,4*numbgimages);

 numscanlines=((backgrounddef *)od)->map.numscanlines;
  //create left edge
 posleft=((backgrounddef *)od)->map.createbgpos((x>>16)-1);

 //sweep through screen, instantiate all active objects on screen
 for (int t=0; t<numscanlines; t++)
 {
  dx=0;
  posleft[t].scroll(321,this);
  posleft[t].scroll(-320,this);
 }
  dx=0; 


 for (bgactiveimage *u=i; u; u=u->next) u->resort();
 _enable();
}    

void bgobject::reset()
{
 numscanlines=((backgrounddef *)od)->map.numscanlines;
  //create left edge
 //free pos
 if (posleft) ::free(posleft); posleft=0;
 posleft=((backgrounddef *)od)->map.createbgpos((x>>16));
}    

//draw every single image from array
extern int verbose;
void bgobject::drawall(char *d)
{
 backgrounddef *b=(backgrounddef *)od;
 int j=b->numbgimages;
 bgimage *i=b->bgi;
 int tx=x>>16,ty=y>>16;

 for (; j>0; j--,i++)
  if (i->index!=0xFF)
   DrawImage(b->id[i->index],d,i->dispx-tx,i->dispy-ty,i->orient);

 if (verbose)
  ((backgrounddef *)od)->map.drawgrid(d,x,y); 
}    

extern FONT *font[];
void bgobject::drawbackground(char *d)
{
 backgrounddef *bd=(backgrounddef *)od;

 _enter(6);
 int ix=(x>>19)/320,iy=(y>>19)/200;
 int fx=-((x>>19)%320),fy=-((y>>19)%200);
 drawscreen((char *)(bd->scr[((*bd->scrmap)[ix][iy])]),d,fx,fy);
 drawscreen((char *)(bd->scr[((*bd->scrmap)[ix+1][iy])]),d,fx+320,fy);
 drawscreen((char *)(bd->scr[((*bd->scrmap)[ix][iy+1])]),d,fx,fy+200);
 drawscreen((char *)(bd->scr[((*bd->scrmap)[ix+1][iy+1])]),d,fx+320,fy+200);
// MemoryCopy(d,(char *)(bd->scr[((*bd->scrmap)[0][0])]) ,64000);
 _leave();

 _enter(7);
 for (bgactiveimage *t=i; t && t->z<=0; t=t->next) t->draw(d);
 foregroundptr=t;
 _leave();


 if (verbose)
 {
//  dprintf(font[1],250,130,"numimgs:%d",num);
  bd->map.drawgrid(d,x,y);
 }
}

extern object *p;
void bgobject::drawlines()
{
 backgrounddef *bd=(backgrounddef *)od;

 for (int i=0; i<bd->numbglines; i++)
  bd->bgl[i].draw(1);
 if (p && p->slope )
  p->slope->draw(2);

  dprintf(font[0],0,50,"%d %d",bgy1,bgy2);
}    

int b1,b2,b3;

void bgobject::refreshbackground(char *d)
{
 if (scroll(dx,dy))
  {
   drawbackground(d); //draw directly to screen
   updated=1;
  }
 else
 if (updated) //draw in secondary buffer
  {
   drawbackground(bg);
   memcpy(d,bg,64000);
   updated=0;
  } //copy from secondary buffer
 else memcpy(d,bg,64000); 
//dprintf(font[1],200,130,"b1:%d b2:%d b3:%d",b1,b2,b3);
}    




void bgobject::drawforeground(char *d)
{
  //go through all active images
 _enter(7);
 for (bgactiveimage *t=foregroundptr; t; t=t->next) t->draw(d);
 _leave();
}    



void bgobject::activate()
{
 active=1;
// reset();
}    

void bgobject::stop()
{
 active=0;
}    

void bgobject::activateitem(bgitem a)
{
 if (a.i>=((backgrounddef *)od)->numbgimages || iarray[a.i]) return;//this image is already activated, so fuck it

 bgimage *b=&((backgrounddef *)od)->bgi[a.i];
 if (b->index==0xFF) return;

  //add new active image
 iarray[a.i]=new bgactiveimage(a.i,this);
// mprintf("activate %d %dX,%dY",a.i,b->aimg->x>>16,b->aimg->y>>16);
}


void bgobject::deactivateitem(bgitem a)
{
 if (a.i>=((backgrounddef *)od)->numbgimages || !iarray[a.i]) return;//this image isn't activated, so fuck it
  //deactivate image
 delete iarray[a.i];
 iarray[a.i]=0;
//  mprintf("deactivate %d",a.i);
}




void bgobject::processleft(bgitem a)
{
 if (!dx) {activateitem(a); return;}

 if (dx>0) //we moved right...
  {
   if (a.x) //over a right edge
    deactivateitem(a);
   else
    activateitem(a);
   return;
  }

 if (dx<0) //we moved left...
  {
   if (a.x) //over a right edge
    activateitem(a);
   else deactivateitem(a);
   return;
  }
}    



extern object *o,*pshadow;
int bgobject::scroll(int tx,int ty)
{
 if (!updated) return 0;
 _disable();

  //move our coords
 int oldx=INTC(x); x+=tx; if (x<0) {tx-=x; x=0;} oldx=INTC(x)-oldx;
 int oldy=INTC(y); y+=ty; if (y<0) {ty-=y; y=0;} oldy=INTC(y)-oldy;

  //scroll all active images
 for (bgactiveimage *u=i; u; u=u->next) u->scroll(tx,ty);

  //adjust position of all objects
 for (object *t=o; t; t=t->next)
   if (t->active) {t->x-=tx; t->y-=ty; t->basey-=ty;}
// if (pshadow) {pshadow->x-=tx; pshadow->z-=ty;}

 if (oldx)
 {
   for (int i=0; i<numscanlines; i++)
     posleft[i].scroll(oldx,this);
 }

 //resort active bg images
 for (u=i; u; u=u->next) u->resort();
 
  //reset deltas
 dx=0; dy=0; updated=0;
 _enable();
 return 1;
}    




//*************************
//****bg pos counter stuff*
//************************
void bgpos::start(bgscanline *line)
{
 l=line;
 x=0; y=l->y;
 p=l->p;
 currp=0; count=0;
}    

int  bgpos::scroll(int scrollx,bgobject *b)
{
if (scrollx>0)
 for (;scrollx>0; scrollx--)
  {
   count++; x++;
   while (currp<l->nump && count==p->dx) 
      {
       b->processleft(p->item);
       currp++; p++; count=0;
      }
  }

if (scrollx<0)
 for (;scrollx<0; scrollx++)
  {
   count--; x--;
   while (currp>0 && count==0) 
       {
        currp--; p--; count=p->dx;
        b->processleft(p->item);
       }
  }
return 0; 
}    


void bgpos::moveto(int tx)
{
 while (tx>x)
  {
   count++; x++;
   while (currp<l->nump && count==p->dx) {currp++; p++; count=0;}
  }

 while (tx<x)
  {
   count--; x--;
   while (currp>0 && count==0) {currp--; p--; count=p->dx;}
  }
}

//************************
//*******Map shit*********
//************************

bgpos *bgmap::createbgpos(int tx)
{
if (!numscanlines) return 0; //no lines, no bgpos

bgpos *t=(bgpos *)malloc(numscanlines*sizeof(bgpos));
for (int i=0; i<numscanlines; i++) //fire them up
 {
  t[i].start(&line[i]);
  t[i].moveto(tx);
 }

return t; 
}    

bgscanline *bgmap::findscanline(int y)
{
 for (int i=0; i<numscanlines; i++)
  if (line[i].y==y) return &line[i];
 return 0; 
}

bgscanline *bgmap::addscanline(int y)
{
 bgscanline *l=findscanline(y);
 if (l) return l;

 for (int i=0; i<numscanlines; i++)
  if (line[i].y>=y) break; //insert line at this point

 //scoot over
 numscanlines++;
 line=(bgscanline *)realloc(line,numscanlines*sizeof(bgscanline));
 memmove(&line[i+1],&line[i],(numscanlines-1-i)*sizeof(bgscanline));

 l=&line[i];
 l->y=y; l->nump=0; l->p=0;
 return l; 
}

//delete indexed scanline
void bgmap::deletescanline(int i)
{
 if (i<0 || i>=numscanlines) return;
 line[i].free();
 numscanlines--;
 memmove(&line[i],&line[i+1],(numscanlines-i)*sizeof(bgscanline));
 line=(bgscanline *)realloc(line,numscanlines*sizeof(bgscanline));
 if (!numscanlines) line=0;
}    

void bgmap::free()
{
 for (int i=0; i<numscanlines; i++)
  line[i].free();
 ::free(line);
 line=0; numscanlines=0;
}

void bgmap::addpoint(int x,int y,bgitem a)
{
 if (x<0) x=0; //return; //x=0;
 if (y<0) y=0; //return; //y=0;
 addscanline(y)->addpoint(x,a);
}    

void bgmap::addimage(IMG * i,int idx,int x,int y,int z)
{
 bgitem a;
 a.i=idx;
 a.type=BG_IMG;
  //add 4 points of image
 int x1,x2,y1,y2;
 y1=y-200*invZ(z); y2=y+i->yw*invZ(z);
 x1=x-320*invZ(z); x2=x+i->xw*invZ(z);
 
  
 a.x=0; a.y=0; addpoint(x1,y1,a);
 a.x=1; a.y=0; addpoint(x2,y1,a);
 a.x=0; a.y=1; addpoint(x1,y2,a);
 a.x=1; a.y=1; addpoint(x2,y2,a); 
}    


//deletes all occurences of an item
void bgmap::deleteitem(bgitem a)
{
    //delete all points of item (4 corners)
 for (int i=0; i<numscanlines; i++)
 {
  for (int j=0; j<line[i].nump; j++)
     if (line[i].p[j].item.type==a.type && line[i].p[j].item.i==a.i)
       line[i].deletepoint(j--); //delete this point
      
  if (!line[i].nump) deletescanline(i--); 
 }
}


//************************
//****scanline shit*******
//************************

void bgscanline::addpoint(int x,bgitem a)
{
 for(int t=0; t<nump; t++)
 {
  x-=p[t].dx;
  if (x<=0) {x+=p[t].dx; break;}
 }

 //create new point
 nump++;
 if (!p) p=(bgpoint *)malloc(sizeof(bgpoint));
   else p=(bgpoint *)realloc(p,nump*sizeof(bgpoint));
 memmove(&p[t+1],&p[t],(nump-1-t)*sizeof(bgpoint));


 //store dx
 p[t].dx=x; p[t].item=a;

 if (++t<nump) p[t].dx-=x;
}    

void bgscanline::deletepoint(int i)
{
 if (i<0 || i>=nump) return;
 nump--;
 if (i<nump) p[i+1].dx+=p[i].dx;
 memmove(&p[i],&p[i+1],(nump-i)*sizeof(bgpoint));
 p=(bgpoint *)realloc(p,nump*sizeof(bgpoint));
 if (!nump) p=0;
}    

void bgscanline::free()
{
 if (p) ::free(p);
 p=0; nump=0;
}    

//************************
//************************
//background def shit
//************************
//************************

void bgline::set(int tx1,int ty1,int tx2,int ty2)
{
 if (tx1<tx2)
  {
   x1=tx1; y1=ty1;
   x2=tx2; y2=ty2;
  } else
  {
   x1=tx2; y1=ty2;
   x2=tx1; y2=ty1;
  }
 type=0;
 if (x1!=x2) type|=BGL_HORIZ;
 if (y1!=y2) type|=BGL_VERT;
}

void bgline::draw(int color)
{
 if (!type) return;
 int x=bgobj->x>>16;
 int y=bgobj->y>>16;
 
 //line(x1-x,y1-y,x2-x,y2-y,color);
}    


//creates the map from image array, region array, and object array.
void backgrounddef::generatemap()
{
 map.free(); //clear map first

 for (int i=0; i<numbgimages; i++)
  if (bgi[i].index!=0xFF)
    map.addimage(id[bgi[i].index],i,bgi[i].dispx,bgi[i].dispy,bgi[i].dispz);
 mprintf("bgmap generated %d images %dscanlines",numbgimages,map.numscanlines);
}    

void backgrounddef::clearmap()
{
 map.free();
 bgobj->free();
 mprintf("bgmap freed");
}

void backgrounddef::add(bgline *x)
{
 for (int i=0; i<numbglines; i++)
   if (!bgl[i].type) break;

 if (i==numbglines)
 {
  numbglines++;
  bgl=(bgline *)realloc(bgl,numbglines*sizeof(bgline));
 }

 bgl[i]=*x;

}    

//adds an image to the background array 
bgactiveimage *backgrounddef::add(bgimage *x)
{
// bgobj->free();

 for (int i=0; i<numbgimages; i++)
  if (bgi[i].index==0xFF) break;

 //add to end
 if (i==numbgimages)
 {
  numbgimages++;
  bgi=(bgimage *)realloc(bgi,numbgimages*sizeof(bgimage));
  bgobj->iarray=(bgactiveimage **)realloc(bgobj->iarray,numbgimages*4);
 }

  //store image in array
 bgi[i]=*x; 

   //add new active image
 bgobj->iarray[i]=new bgactiveimage(i,bgobj);

 return  bgobj->iarray[i];
}    

//updates an image from the array in the map
void backgrounddef::update(bgactiveimage *t)
{
 bgitem a;
 a.type=BG_IMG; a.i=t->idx;

 bgimage *b=&bgi[t->idx]; //get pointer to bg image


 map.deleteitem(a); //delete from map
  //add back to map
 map.addimage(t->iptr,t->idx,b->dispx,b->dispy,b->dispz);

 //reset
 bgobj->reset();
}    

//removes an image from the array and the map
void backgrounddef::remove(bgactiveimage *t)
{
 bgitem a;
 a.type=BG_IMG; a.i=t->idx;

 map.deleteitem(a); //delete from map
 bgobj->reset();
// bgobj->moveto(bgobj->x,bgobj->y);    
}    





int ReadScreenList(char *list, SCREEN ***scr,defname **names);

void backgrounddef::Read()
{
 if (type!=BGDEF) return;
 //load extra bg stuff
 numscreens=ReadScreenList("bgscreen.lst",&scr,&screennames); 
 
 FILEIO f;
 if (!f.open("bgimages.map"))
 {
  numbgimages=f.readint();
  f.readint(); //skip size
  bgi=(bgimage *)f.readalloc(numbgimages*sizeof(bgimage));
  f.close();
 }     

 if (!f.open("bgscreen.map"))
  {
   scrmap=(uchar (*)[64][64])f.readalloc(64*64);
   f.close();
  } else
  {
   scrmap=(uchar (*)[64][64])malloc(64*64);
   memset(scrmap,0,64*64);
  }

 if (!f.open("bglines.map"))
 {
  numbglines=f.readint();
  f.readint(); //skip size
  bgl=(bgline *)f.readalloc(numbglines*sizeof(bgline));
  f.close();
 }     




 //generate background map
 generatemap();
}    

void backgrounddef::Save()
{
if (type!=BGDEF) return;

FILEIO f;
f.create("bgimages.map");
f.writeint(numbgimages); 
f.writeint(sizeof(bgimage)); 
f.write(bgi,numbgimages*sizeof(bgimage)); 
f.close();

f.create("bgscreen.map");
f.write(scrmap,64*64);
f.close();

f.create("bglines.map");
f.writeint(numbglines); 
f.writeint(sizeof(bgline)); 
f.write(bgl,numbglines*sizeof(bgline)); 
f.close();


mprintf("%s saved %dbgimages %dbglines",name,numbgimages,numbglines);
}    

void backgrounddef::Kill()
{
 if (type!=BGDEF) return;
 //kill extra bg stuff
 if (bgi) {free(bgi); numbgimages=0;}
 clearmap();

 numscreens=0;
 if (scr) free(scr); scr=0;
 if (screennames) free(screennames); screennames=0;

 if (scrmap) free(scrmap); scrmap=0;

 if (bgl) free(bgl); bgl=0;
}    

void backgrounddef::volumize()
{
}    





//************************
//diagnostic shit
//************************

void drawhline(char *d,int y, char c)
{
 if (y<0 || y>=200) return;
 memset(d+y*320,c,320);
}
void drawvline(char *d,int x, char c)
{
 if (x<0 || x>=320) return;
 d+=x;
 for (int i=0; i<200; i++,d+=320)  *d=c;
}


void bgmap::drawgrid(char *d,int x,int y)
{
 for (int i=0; i<numscanlines; i++)
  {
   drawhline(d,((line[i].y<<16)-y)>>16,2);
   int tx=0;
   for (int j=0; j<line[i].nump; j++)
    {
     tx+=line[i].p[j].dx;
     drawvline(d,((tx<<16)-x)>>16,2);
    }
  }
}    

void bgscanline::print()
{
printf("%dy: ",y);
for (int t=0; t<nump; t++)
 printf("%d:%d ",p[t].dx,p[t].item.i);
printf("\n"); 
}    

void bgmap::print()
{
for (int i=0; i<numscanlines; i++)
  line[i].print(); 
}    



