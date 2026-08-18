//gets the integer portion of the coordinate
#define INTC(x) ((signed short *)&x)[1]



//definition for image in a frame
struct image
{
uchar index;    //Index into image definition array (256 images total)

uchar orient:2; //Orientation yx
uchar layer:2;  //layer number of this image
uchar exclusive:1; //does it stay on this layer only?

signed short  dispx;    //Relative x location for image
signed short  dispy;    //Relative y location
};

//frame header
//describes state of object in one frame
struct frame  
{
//basic fields    (10 bytes)
unsigned short size;   //size of this frame in bytes
unsigned short dur;    //duration in ticks
short dx,dz,dy;            //x,y,z movement per tick

//count of appendices (4 bytes)
char numextra;    //number of extra options
char reserved[2];
char numimages;   //number of images

//image[numimages]
//byte[numextra]

image *getimgptr();
byte *getextraptr();
};

//series parms
#define SP_NOSHADOW 1
#define SP_NOGROUND 2
#define SP_NOFALLDOWN 4
#define SP_MOVEDOWN 8


//series header
//describes a sequential series of frames
struct series
{
frame *first;   //pointer to first frame in series
int size;      //size of this series in bytes
uchar cf,nf;        //number of frames in this series
int parm;      //parameter

zpoint pos; //starting location for series (optional)
uchar next; //next series to do after theees one
uchar cfunc;  //function that does control (optional)

uchar falldown; //series that should go to if this object falls in this series, 0 for search
signed short energy; //energy

char name[16];  //name of this series i.e (stance, walk forward)    

#ifdef ANIMATOR
char *insertbytes(char *p,int num);
void deletebytes(char *p,int num);
#endif
};


typedef char defname[9];

#define OBJDEF 1
#define BGDEF  2


//object class definition
//object contains a list of series, image defs, and frame list
//ex: axe, reaper, guil
struct objectdef
{
public:    
uchar loaded:1;    //is it loaded into memory or not? 
uchar readonly:1; //can this be modified?                
uchar dirty:1;     //if series is "dirty" or not (needs to be saved)
uchar type:2;   //OBJ? BG?

char refcount;  //number of references to this definition
                //if !loaded and there be references, it must be loaded

uchar onum;      //ordinal number of this object
char name[16];   //name of this object "JINSOKU"

int numi;         //number of images
img      **id;       //list of all possible images for reference by imagedefs
defname  *imgnames; //list of all image names


int nums;   //number of series
series   *sd; //series definitions
frame    *f; //frame list (big chunk o' frames)

int numsounds; //number of sounds
SOUND   **sounds; //list of all sounds
defname *sndnames; //list of all sound names

unsigned mem; //amount of memory allocated for this object

void Read();     //Read Object into memory from dir(if not loaded)
void ReadVol();  //Read Object into memory from vol(if not loaded)
void Kill();  //Free object from memory
void Save(); //save object
void volumize();

#ifdef ANIMATOR
char sdf[32];    //series definition file path "\def\char.sdf" (common to most objs)
char dir[32];   //source directory for all images, move.lst, etc "\ug"
char volfile[32]; //volume file

int  LoadSeriesList();
void SaveSeriesList();

int RequestSeries();
int RequestImage();
int RequestSound();

void addnewseries();
void SaveSDF();
#endif
};    


#include "region.h"

//instance of objectdef
//position, frame, dur
class  object
{
public:
//linking vars
object *prev,*next; //next object

object *creator; //who created this?
//object *parent,*child; //parent/child relationship

//type vars    
uchar onum;     //ordinal number of this object
objectdef *od; //pointer to object definition (NULL if not loaded)

//frame vars
frame *fptr;   //pointer to current frame of this object
int dur;      //duration of this frame
uchar cf,nf;   //current frame, number of frames

//current regions
region *r[3];
struct bgline *slope; //bgline thats under this object

//position vars (<<16)
int x;  //current position (left-right)
int y;  // (up-down in air) 
int z;  //(forward-backward)
int basey; //y coordinate of the background floor underneath the object
char d;       //direction 0-right 1-left

uchar updated:5; //set if object has moved or frame has changed   (k---fzyx)
uchar active:1; //whether or not the whole object is playing and active
uchar playing:1; //whether or not the single series is playing
uchar playrepeat:1; //to repeat this series or not
uchar killme:1; //flag to tell whether or not this object should be destroyed
uchar lastforever:1; //flag to tell whether to last forever
uchar invincible:1; //flag to tell if it is invincible
uchar layer;    //current layer to display 0-3

uchar intersect; //total intersections currently

//series vars
uchar  csnum;    //ordinal number current series
series *cs;      //current series displayed "0-STANCE,1-WALK"
int (object::*controlfunc)(int); //pointer to control function for this series (0 for none)

//misc
signed short energy;
uchar shakedur; //shake duration

//input device
input *in; //input device for the object (0 for none)

//keeps track of jumping series for trajectory
//jumping stuff
frame *jumpptr; //0 if not jump-trajectory needed
int jumpdur;
uchar jumpcf,jumpnf,jumpcsnum;


//funcs
object(objectdef *objdef,int s,int tx,int ty,int tz,int td, object *c);
virtual ~object();

//void reset(); //resets all object stuff
int startseries(int seriesnum); //start series from the beginning
int startseries(char *seriesname); //start series with character name
int findseries(char *seriesname);

void draw();    //draw object at x,y to screen
void drawmap(); //draw object shadowed

void play(int csnum); //plays series and stops
void playlooped(int csnum); //plays series and repeats

void activate(int csnum); //activates object
void stop();               //stops playing
void kill();

void sound(int num); //play this sound ordinal

void tick();  //tick
int  advanceframe();
void effect(); //instantiates all of the non hit extras for this frame
void hiteffect(int x,int y,int z); //instantiates all of the hit extras at absolute point
void move(int tx,int ty,int tz); //move object
void moveabs(int tx, int ty, int tz);
void falldown();
int getbasey(); //sets the basey from the objects current position
int hitground(int groundy);

void resort(); //resorts object in linked list based on y-val

int  startjumpseries(int seriesnum);
void resumejump();

//checks to see if this object intersects any other objects' regions
int getregion(int rtype,zrect &a);
void checkattack();
void checkimperm();

int  loseenergy(int e); //makes this object lose energy

//Effect functions
int E_SOUND_do(e_sound *t);
int E_GROUNDSHAKE_do(e_shake *t);
int E_OBJECTSHAKE_do(e_shake *t);
int E_GOTO_do(e_goto *t);
int E_BLOOD_do(e_blood *t);
int E_NEWOBJECT_do(e_newobject *t);
int E_REGION_do(e_region *t);
int E_DIR_do(e_dir *t);
static int (object::*extrado[])(byte *t);


//animator only
#ifdef ANIMATOR
virtual void DrawStatus();
void DrawSeries();
void DrawExtra();
void DrawExtraX();
void DrawExtraXShadow();
void AddExtra(int i);
image *CheckImageBound();
void resetfptr(); //sets fptr to cf position
void DrawRegions();

virtual void drawbuttons();
virtual void mainedit();

uchar  oldcsnum;  //ordinal number saved series
#endif

//static control functions
#include "\a32\cfunc.h"

static int (object::*cfunclist[])(int);
} ;


void addobject(object *);
