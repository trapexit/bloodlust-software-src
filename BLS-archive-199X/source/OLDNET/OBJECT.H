#ifndef _OBJECT_
#define _OBJECT_

#include "objspace.h"
#include "bg.h"

#include "types.h"
#include "objdef.h"
#include "input.h"


//object update
#define OU_X 1
#define OU_Y 2
#define OU_Z 4
#define OU_FRAME 8
#define OU_SERIES 16
#define OU_ALL 31

//instance of objectdef
//position, frame, dur
class  object
{
typedef int (object::*CONTROLFUNCPTR)(int);
friend class objectdef;

public:
class objectspace *osp; //object space that this object is in (manditory)

//linking vars
class object *prev,*next; //next object

class object *creator; //who created this?

//type vars
uchar onum;     //ordinal number of this object (manditory)
class objectdef *od; //pointer to object definition (manditory)

//series
uchar  csnum;    //ordinal number current series
struct series *cs;      //current series displayed "0-STANCE,1-WALK"
CONTROLFUNCPTR controlfunc; //pointer to control function for this series (0 for none)

//frame
struct frame *fptr;   //pointer to current frame of this object
int dur;      //duration of this frame
uchar cf,nf;   //current frame, number of frames

//position relative to bg (<<16)
int x;  //current position (left-right)
int y;  // (up-down in air)
int z;  //(forward-backward)
int basey; //y coordinate of the background floor underneath the object
char d;       //direction 0-right 1-left


//get coords relative to ul of screen
int relx() {return osp->bg ? (x-osp->bg->x) : x; }
int rely() {return osp->bg ? (y-osp->bg->y) : y; }
int relz() {return z; }
int relbasey() {return osp->bg ? (basey-osp->bg->y) : basey;}
//set coords relative to ul of screen
void setrelx(int tx) {x=osp->bg ? (tx+osp->bg->x) : tx;}
void setrely(int ty) {y=osp->bg ? (ty+osp->bg->y) : ty;}
void setrelz(int tz) {z=tz;}


//flags
uchar updated:5; //set if object has moved or frame has changed   (k---fzyx)
uchar active:1; //whether or not the whole object is playing and active
uchar playing:1; //whether or not the single series is playing
uchar playrepeat:1; //to repeat this series or not
uchar killme:1; //flag to tell whether or not this object should be destroyed
uchar lastforever:1; //flag to tell whether to last forever
uchar invincible:1; //flag to tell if it is invincible
uchar net:1; //is this a netobject?
uchar layer;    //current layer to display 0-3

uchar intersect; //total intersections currently
class region *r[3];

//misc
signed short energy;

//input device
class input *in; //input device for the object (0 for none)

//possible trajectory of object
class trajectory *traj;

struct bgline *floor,*wall,*ceiling;
int doclimbwall(int instat);
int dohangceiling(int instat);
void forcefloor();

//------------------------------------------------

//member functions
object(objectdef *objdef,int s,int tx,int ty,int tz,int td, object *c);
virtual ~object();

virtual object *spawn(int objnum,int series,int tx,int ty,int tz, int td);

//series func
virtual int startseries(int seriesnum); //start series from the beginning
int startseries(char *seriesname); //start series with character name
int findseries(char *seriesname);

//drawing functions
void draw(char *dest);    //draw object at x,y
void drawshadow(char *dest);
//void drawmap(); //draw object shadowed

//object play control funcs
void play(int csnum); //plays series and stops
void playlooped(int csnum); //plays series and repeats
void activate(int csnum); //activates object
void stop();               //stops playing
void kill();


virtual void tick();  //tick
virtual int  advanceframe();
virtual void effect(); //instantiates all of the non hit extras for this frame
struct frame *resetfptr(); //sets fptr to cf position
void sound(int num); //play this sound ordinal
void flip();
void move(int tx,int ty,int tz); //move object
void moveabs(int tx, int ty, int tz);
void boundcheck();

//background interaction
void falldown();
int hitground(int groundy);
int getbasey(); //sets the basey from the objects current position
int hitwallleft();
int hitwallright();
int hitceiling();

void resort(); //resorts object in linked list based on z-order

//checks to see if this object intersects any other objects' regions
//int getregion(int rtype,zrect &a);
//void checkattack();
//void checkimperm();

int  loseenergy(int e); //makes this object lose energy

//static control functions
#include "c:\a32\cfunc.h"

static CONTROLFUNCPTR cfunclist[];

struct image *hittest(int x,int y);
#ifdef ANIMATOR
void framebackward();
void frameforeward();
void seriesup();
void seriesdown();

void insertframe();
void insertframeafter();
void deleteframe();
void deleteeffect(class effect *e);
void dirty();
void addnewseries();
void print();
#endif
};


//defines a trajectory that can be attached to an object
//to modify its movement...ex, for jumps or randomdur
class trajectory
{
 protected:
 class object *o; //object that this trajectory controls
 public:
 trajectory(object *to):o(to)
  {
   if (!o) return;
   if (o->traj) delete o->traj; //replace
   o->traj=this; //link
  }
 virtual ~trajectory() {if (o) o->traj=0;} //unlink
 virtual void tick()=0; //tick
 virtual void move(int)=0; //move object based on trajectory
};

//trajectory based on jump
class jumptrajectory:public trajectory
{
 protected:
 public:
 //keeps track of jumping series for trajectory
 struct frame *fptr;
 int dur;
 uchar cf,nf,csnum;

 jumptrajectory(object *to); //create jump trajectory based on object
 virtual ~jumptrajectory();
 virtual void tick();
 virtual void move(int);
};

//trajectory of basic shaking
class shaketrajectory:public trajectory
{
 static int oshake[8];
 int dur;
 public:
 shaketrajectory(int tdur,object *to):trajectory(to),dur(tdur) {}
 virtual void tick() {dur--; if (dur<=0) delete this;}
 virtual void move(int instat) {o->moveabs(oshake[dur&7]<<16,0,0);}
};


//trajectory based on movement
class movetrajectory:public trajectory
{
 protected:
 int dx,dy,dz;
 int dur;

 public:
 movetrajectory(object *to,int tx,int ty,int tz,int tdur)
  :trajectory(to),dur(tdur)
  { dx=tx; dy=ty; dz=tz; }; //create move trajectory
 virtual void tick();
 virtual void move(int);
};


#endif


