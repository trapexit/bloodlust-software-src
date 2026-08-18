//structure that defines an intersection
struct intersection
{
object *a;
zrect arect;
e_region *ar;

object *b;
zrect brect;
e_region *br;

lpoint p;
int result;

static int (intersection::*rfunclist[])();
#include "\a32\rfunc.h"
};    

//class for active regions
class region {
 public:
 region *prev,*next;
 region **list; //pointer to beginning of list
 
 object *o; //object that this region belongs to
 e_region *r; //this region
 uchar intersect; //bits for how this intersects

 region *platform; //region that we are standing on
 int platformy; //y coordinate of top of platform
 int platwidth; //width of the side intersecting platform
 int platnum; //number of people standing on us

 int width;


 region(object *x,e_region *e);
 ~region();

 region *update(e_region *e,object *o);
 getabsregion(zrect &a); //gets the absolute region
 void checkattack();
 void checkimperm();

 void updateaboveobjects();
 void moveaboveobjects(int tx,int ty,int tz); //x,y,z<<16
 void setplatform(region *newplat,intersection *i);
 
};



extern region *rlist[3];
