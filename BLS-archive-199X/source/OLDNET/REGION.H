#ifndef _REGION_
#define _REGION_

#include "effect.h"

#define R_LEFT    1
#define R_RIGHT   2
#define R_ABOVE   4
#define R_BELOW   8

//active region
class region {
 friend class objectspace;
 friend class object;
 friend int imperm_intersection(region &a,region &b);
 friend int attack_intersection(region &a,region &b);

 region *prev,*next; //linked list
// region **list;
 int type;

 region *platform; //platform we are standing on
 int px,py;    //last coords of platform object
 void setplatform(region *p);
 void clearplatform();

 uchar updated; //has this region moved?

 class objectspace *osp;
 class object *o;   //object

 e_region *e; //e_region that this represents
 lrect r;     //absolute coords of region o+bg+r

 void getabsregion(); //get abs coords of region

 public:
 region(object *x,e_region *t);
 ~region();
 void update(e_region *t);
 void draw(char *dest);
 void checkimperm();
 void checkattack();

 int getwidth() {return r.x2-r.x1;}
 int getheight() {return r.y2-r.y1;}

};

#endif