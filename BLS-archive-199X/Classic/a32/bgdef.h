#ifndef _BGDEF_
#define _BGDEF_

#include "objdef.h"

//structure for each image of background
struct bgimage: public image
{
 short dispz; //z dimension

 void calcextent(lrect &r,class bgobject *bg); //calculate the visual extent of this image
 void draw(struct IMG **,char *dest,int x,int y);

 #ifdef ANIMATOR
 int hittest(struct IMG **,int x,int y,int mx,int my);
 #endif
};


//structure for each line definition composing a bg object-boundary
struct bgline {
 #define BGL_FLOOR    1
 #define BGL_WALLLEFT 2
 #define BGL_WALLRIGHT 3
 #define BGL_CEILING   4

 uchar  type;
 short int x1,x2,y1,y2;

 int getbasey(int tx,int ty);
 int calcx(int tx);
 int calcy(int tx);
 int wallleftintersect(lrect &r);
 int wallrightintersect(lrect &r);
 int ceilingintersect(lrect &r);

 int getslope() {return ((y2-y1)<<16)/(x2-x1);}
 void draw(class bgobject *bg,char *dest,int color);
 #ifdef ANIMATOR
 void set(int tx1,int ty1,int tx2,int ty2);
 void moverel(int dx,int dy) {x1+=dx; x2+=dx; y1+=dy; y2+=dy;}
 int hittest(int mx,int my);
 #endif
};


//structure defining a scroll boundary
struct boundary:public bgline
{
 //boundary types
 #define BGB_TOP    1
 #define BGB_BOTTOM 2
 #define BGB_LEFT   3
 #define BGB_RIGHT  4

 int intersect(lrect &r); //does boundary intersect this rect?
 int extent(lrect &r); //does the extent intersect the rect

 void draw(class bgobject *bg,char *dest);
 #ifdef ANIMATOR
 void set(int tx1,int ty1,int tx2,int ty2);
 int hittest(int mx,int my);
 #endif
};


//object pos
struct bgobjpos
{
 short x,y,z;
 uchar onum;
 uchar s;
 uchar d;

 void set(class object *o); //set from object
 class object *create(class objectspace *osp); //create in objectspace
};


#define SCRMAPXW 64
#define SCRMAPYW 64

#ifdef ANIMATOR
#define OBJECTDEF objectdefw
#else
#define OBJECTDEF objectdef
#endif

class bgdef:public OBJECTDEF
{
 public:
 //array of all images in background
 int numbgimages;
 bgimage *bgi; //array of images

 //array of screens on background
 int numscr;
 struct SCR **scr;
 defname *screennames; //list of all screen names
 int readscreens(char *lstname);
 uchar (*scrmap)[SCRMAPXW][SCRMAPYW];

 //array of floor lines
 int numbglines;
 struct bgline *bgl;

 //array of scroll boundary lines
 int numboundary;
 struct boundary *bnd;

 //array of stored bg object positions
 int numbgobjpos;
 struct bgobjpos *bop;

 #ifdef ANIMATOR
 bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile,char *tsdf,char *tdir);
 #else
 bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile);
 #endif
 virtual int read();
 virtual void freeodf(); //free

 void instantiateobjposlist();


 #ifdef ANIMATOR
 virtual void save();
 virtual void writevol(class volumefile &v);
 void loadbgimages(char *filename);
 void savebgimages(char *filename);
 void loadscrmap(char *filename);
 void savescrmap(char *filename);
 void loadbglines(char *filename);
 void savebglines(char *filename);
 void loadboundaries(char *filename);
 void saveboundaries(char *filename);
 void loadobjects(char *filename);
 void saveobjects(char *filename);

 void resortbgimages();
 bgimage *insertimage(bgimage *t);
 void deleteimage(bgimage *t);

 bgline *insertbgline(bgline *t);
 void deletebgline(bgline *t);

 boundary *insertboundary(boundary *l);
 void deleteboundary(boundary *l);
 #endif
};


#endif
