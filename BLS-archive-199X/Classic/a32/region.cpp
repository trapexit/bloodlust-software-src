//region and collision intersections
#include "region.h"
#include "bg.h"
#include "objspace.h"
#include "object.h"

#include "r2img.h"
#include "dd.h"
#include "message.h"

static char rcolor[3]={2,9*16+1,4};
void region::draw(char *dest)
{
 if (!osp->bg)
  drawrect(screen,rcolor[type],r.x1+osp->x1,r.y1+osp->y1,r.x2-r.x1,r.y2-r.y1);
 else
  drawrect(screen,rcolor[type],r.x1+osp->x1-(osp->bg->x>>16),r.y1+osp->y1-(osp->bg->y>>16),r.x2-r.x1,r.y2-r.y1);
}

void region::update(e_region *t)
{
 if (e!=t)
  {
   e=t; //set region
   getabsregion();
  }
}

//region constructor
region::region(object *x,e_region *t):o(x),e(0),osp(x->osp),platform(0)
{
 update(t);
 type=e->type;

  //link to chain
// list=&osp->rlist[e->type];
 prev=0; next=osp->rlist[type];
 if (next) next->prev=this;
 osp->rlist[type]=this;

 //bind to object
 o->r[type]=this;
}

//region destructor
region::~region()
{
  //unlink from list
 if (prev) prev->next=next; else osp->rlist[type]=next;
 if (next) next->prev=prev;

 //unbind from object
 o->r[type]=0;

 //update all other regions that have us as platform
 if (type==R_IMPERM)
   for (region *t=osp->rlist[R_IMPERM]; t; t=t->next)
    if (t->platform==this) t->clearplatform();
}


extern int blah;
void region::getabsregion()
{
 //get abs coords of object
/* if (osp->bg)
 {
  r.x1=r.x2=(o->x+osp->bg->x)>>16;
  r.y1=r.y2=(o->y+osp->bg->y)>>16;
 } else
 {
  r.x1=r.x2=o->x>>16;
  r.y1=r.y2=o->y>>16;
 }*/
  r.x1=r.x2=o->x>>16;
  r.y1=r.y2=o->y>>16;

 //add region boundaries
 if (!o->d) {r.x1+=e->r.p1.x; r.x2+=e->r.p2.x;}
       else {r.x1-=e->r.p2.x; r.x2-=e->r.p1.x;}
 r.y1+=e->r.p1.y; r.y2+=e->r.p2.y;
 updated++;
}

//sees if two regions intersect
inline int intersect(lrect &a, lrect &b)
{
   //see if NO intersection
 if (b.x1>a.x2 || a.x1>b.x2 ||
     b.y1>a.y2 || a.y1>b.y2) return 0;
  //must be some intersection
 return 1;
}

//get rect of intersection
inline void getintersectionrect(lrect &a,lrect &b,lrect &i)
{
 i.x1=(a.x1<b.x1) ? b.x1 : a.x1;
 i.x2=(b.x2<a.x2) ? b.x2 : a.x2;
 i.y1=(a.y1<b.y1) ? b.y1 : a.y1;
 i.y2=(b.y2<a.y2) ? b.y2 : a.y2;
}

//get center of a rectangle
inline void getcenter(lrect &r,lpoint &p)
{
 p.x=(r.x1+r.x2)/2;
 p.y=(r.y1+r.y2)/2;
}

// msg.printf(2,"%d,%d-%d,%d",r.x1,r.y1,r.x2,r.y2);

//get relative orientation of a to b
inline int getorient(lrect &a,lrect &b,lrect &i)
{
 if ((i.x2-i.x1)>(i.y2-i.y1)) //if xw is greater than yw
  {
   if (a.y1<=b.y1) return R_ABOVE;
   if (a.y2>=b.y2) return R_BELOW;
  } else
  {
   if (a.x1<=b.x1) return R_LEFT;
   if (a.x2>=b.x2) return R_RIGHT;
  }
 return 0;
}



void region::clearplatform()
{
 platform=0;
 updated++;
}

void region::setplatform(region *p)
{
 if (p!=platform)
  {
   platform=p;
   px=platform->o->x;
   py=platform->o->y;
  }
}




int imperm_intersection(region &a,region &b)
{
 lrect i;
 int o;

 //get rect of intersection
 getintersectionrect(a.r,b.r,i);
 //get orientation of intersection
 o=getorient(a.r,b.r,i);

 int xw=i.width();  if (xw>5) xw=5; xw<<=16;
 int yw=i.height(); //if (yw>10) yw=10; yw<<=16;

 switch (o)
 {
  case R_RIGHT:
    if (yw<20) break;
    if ((a.e->getflags()&R_PUSH) && (b.e->getflags()&R_PUSH)) break;

    //if ((a.o->z>>16)==(b.o->z>>16))
    a.o->z=b.o->z+1;
    a.o->moveabs(xw>>1,0,0);
    b.o->moveabs(-(xw>>1),0,0);
  break;
  case R_LEFT:
    if (yw<20) break;
    if ((a.e->getflags()&R_PUSH) && (b.e->getflags()&R_PUSH)) break;

    //if ((a.o->z>>16)==(b.o->z>>16))
    a.o->z=b.o->z-1;
    a.o->moveabs(-(xw>>1),0,0);
    b.o->moveabs(xw>>1,0,0);
  break;
  case R_ABOVE:
    if (!(b.e->getflags()&R_STANDONTOPABLE))
     { o&=~R_ABOVE; break;}
    a.setplatform(&b);

    //if ((a.o->z>>16)==(b.o->z>>16))
    a.o->z=b.o->z+1;
    a.o->hitground(a.o->y-(yw<<16));
//    a.o->hitground((b.o->y>>16)+b.e->r.y1-a.e->r.y2 );
//    if (!a.osp->bg) a.o->hitground(b.r.y1<<16);
//               else a.o->hitground((b.r.y1<<16)-a.osp->bg->y);
   break;
 }
 return o;
}



int attack_intersection(region &a,region &b)
{
 //see if valid attack...
 if (!(a.e->getflags()&b.e->getflags())) return 0;

 if (a.o==b.o) return 0; //same object can't hit itself

 //get rect of intersection
 lrect r;
 getintersectionrect(a.r,b.r,r);

 //get center point of intersection
 lpoint p;
 getcenter(r,p);

 //---------------------
 //doeffects
// msg.printf(2,"attack");
 //what they do to each other
 e_region *ae=a.e,*be=b.e; //store regions
 object *ao=a.o,*bo=b.o;

 if (ae->getflags()&R_FORCEFACE)
  {
   if (bo->x>ao->x) bo->d=1; else bo->d=0;
  } else bo->d=ao->d^1; //flip victim

 ae->hitremotetrigger(bo,&p); //attacker's remote (loseenergy)
 be->hitremotetrigger(ao,&p); //victim's remote

 be->hitlocaltrigger(bo,&p); //victim's local (onenergy)
 ae->hitlocaltrigger(ao,&p); //attacker's local (suicide)

 return 1;
}







void region::checkimperm()
{
 if (platform) // && (platform->updatedo->updated&7))
  {
   o->moveabs((platform->o->x-px),(platform->o->y-py),0);
   px=platform->o->x;
   py=platform->o->y;
      
   if (o->updated&7) getabsregion();
  }

 if (!updated) return; //region hasn't moved

 blah++;
 o->intersect=0;
 for (region *t=osp->rlist[R_IMPERM]; t; t=t->next)
   if (t!=this && intersect(r,t->r))
      o->intersect|=imperm_intersection(*this,*t);  //intersection occurred

 if (platform && !(o->intersect&R_ABOVE)) //not above anything
          clearplatform();    //no platform
 updated=0;
}


void region::checkattack()
{
 if (!updated) return; //region hasn't moved

 region *tnext;
 for (region *t=osp->rlist[R_VULN]; t; t=tnext)
  {
   tnext=t->next;
   if (t!=this && intersect(r,t->r))
      if (attack_intersection(*this,*t)) break;  //intersection occurred
  }
 updated=0;
}

















