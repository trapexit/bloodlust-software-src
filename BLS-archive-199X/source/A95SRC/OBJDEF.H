#ifndef _OBJDEF_
#define _OBJDEF_

//gets the integer portion of the coordinate
#define INTC(x) ((signed short *)&x)[1]

#include "types.h"

#include "effect.h"

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

struct image *getimgptr()
{
 if (!this) return 0;
 return (struct image *) (((char *)this)+sizeof(frame));
};

class effect *geteffectptr()
{
 if (!this) return 0;
 return (class effect *) (getimgptr()+numimages);
};

frame *next()
{
 return (frame *)(((char *)this)+size);
}

void draw(char *dest,int x,int y,class objectdef *od);
void drawflipx(char *dest,int x,int y,class objectdef *od);

};

//series parms
#define SP_NOSHADOW 1
#define SP_NOGROUND 2
#define SP_NOFALLDOWN 4
#define SP_MOVEDOWN 8

#define SP_SUICIDE 0xFF
#define SP_LASTFOREVER 0xFE

//series header
//describes a sequential series of frames
struct series
{
struct frame *first;   //pointer to first frame in series
int size;      //size of this series in bytes
uchar cf,nf;        //number of frames in this series
int parm;      //parameter

zpoint pos; //starting location for series (optional)
uchar next; //next series to do after theees one
uchar cfunc;  //function that does control (optional)

uchar falldown; //series that should go to if this object falls in this series, 0 for search
signed short energy; //energy of object instantiated in this series, or energy taken off by attack

char name[16];  //name of this series i.e (stance, walk forward)

#ifdef ANIMATOR
void *insertbytes(void *p,int num);
void *deletebytes(void *p,int num);
void insertframe(frame *fptr); //inserts frame before fptr
void deleteframe(frame *fptr);
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
class objectspace *osp; //objectspace that this belongs to
uchar loaded:1;    //is it loaded into memory or not?
uchar readonly:1; //can this be modified?
uchar dirty:1;     //if series is "dirty" or not (needs to be saved)
uchar type:2;   //OBJ? BG?
uchar isvol:1;   //loaded from vol?
uchar volexist:1; //does this object's vol exist?

char refcount;  //number of references to this definition
                //if !loaded and there be references, it must be loaded

//objdef identification
uchar onum;      //ordinal number of this object
char name[16];   //name of this object "JINSOKU"

//images
int numi;         //number of images
struct IMG   **id;       //list of all possible images for reference by imagedefs
void freeimages();

//series list
int nums;   //number of series
struct series   *sd; //series definitions
struct frame    *f; //frame list (big chunk o' series[] and frames)
void freeseries();

//sounds
int numsounds; //number of sounds
struct SOUND   **sounds; //list of all sounds
struct IDirectSoundBuffer **dsounds; //list of all sounds (directsound)

void convertdsounds();
void freesounds();

//load info
unsigned mem; //amount of memory allocated for this object
char volfile[32]; //volume file

//member functions
objectdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile);
~objectdef();

int load(); //reference objdef
void kill(); //dereference objdef
void refreshload(); //check refresh count vs loaded flag, and read if necessary
                    //also updates all objects in osp in accordance with load

virtual int read(); //read objectdef from disk (uses vol)
virtual void freeodf(); //free objectdef
};

#ifdef ANIMATOR
//writable and editable objectdef
//uses series list, need not be loaded from vol
class objectdefw: public objectdef
{
 public:
 defname *imgnames; //list of all image names
 defname *sndnames; //list of all sound names

 char sdf[32];    //series definition file path "char.sdf" (common to most objs)
 char dir[32];   //source directory for all images, move.lst, etc "ug"

 objectdefw(class objectspace *tosp,int tnum,char *tname,char *tvolfile,char *tsdf,char *tdir);

 virtual int read(); //read object from disk (uses serieslist,images, etc)
 virtual void freeodf(); //free object from memory

 virtual void save(); //save to disk
 void volumize();
 virtual void writevol(class volumefile &v); //write vol specific stuff to disk

 int readsdf(char *filename); //read series definition file
 void writesdf(char *filename);

 int  readseries(char *filename); //load series from file
 void writeseries(char *filename); //writes series to disk (the only changeable element)

 int readimages(char *lstfile); //load in images from directory
 int readsounds(char *lstfile); //load in sounds from directory
};
#endif

typedef char CFUNCNAME[16];
typedef CFUNCNAME (*CFUNCLISTPTR)[];

extern int cfuncnum; //num of control functions
extern CFUNCLISTPTR cfuncnames; //control function names





#endif



