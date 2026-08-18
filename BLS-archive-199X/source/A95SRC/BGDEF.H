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


//structure for each line definition composing a bg boundary
struct bgline {
 #define BGL_HORIZ 1
 #define BGL_VERT  2

 uchar  type;
 short int x1,x2,y1,y2;

 int getbasey(int tx,int ty);
 int calcy(int tx);


 void draw(class bgobject *bg,char *dest,int color);
 #ifdef ANIMATOR
 void set(int tx1,int ty1,int tx2,int ty2);
 void moverel(int dx,int dy) {x1+=dx; x2+=dx; y1+=dy; y2+=dy;}
 int hittest(int mx,int my);
 #endif
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

 //array of screen boundary lines
 int numbglines;
 struct bgline *bgl;

 #ifdef ANIMATOR
 bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile,char *tsdf,char *tdir);
 #else
 bgdef(class objectspace *tosp,int tnum,char *tname,char *tvolfile);
 #endif
 virtual int read();
 virtual void freeodf(); //free


 #ifdef ANIMATOR
 virtual void save();
 virtual void writevol(class volumefile &v);
 void loadbgimages(char *filename);
 void savebgimages(char *filename);
 void loadscrmap(char *filename);
 void savescrmap(char *filename);
 void loadbglines(char *filename);
 void savebglines(char *filename);

 void resortbgimages();
 bgimage *insertimage(bgimage *t);
 void deleteimage(bgimage *t);

 bgline *insertbgline(bgline *t);
 void deletebgline(bgline *t);
 #endif
};


#endif