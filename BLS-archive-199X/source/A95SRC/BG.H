#ifndef _BG_
#define _BG_

#include "bgdef.h"

//represents chunk of space
class bgchunk
{
 int numi; //number of images in this chunk
 int *idx; //array of indices to bgi for each image
 int firstfg; //index of first fg image
 public:
 void create(bgobject *bg,int x,int y, int xw,int yw); //create chunk
 void destroy();

 void drawbg(bgobject *bg,char *dest,int x,int y);
 void drawfg(bgobject *bg,char *dest,int x,int y); 
 #ifdef ANIMATOR
 bgimage *hittest(bgobject *bg,int x,int y,int mx,int my);
 #endif
};



//object representing active instance of a background def
class bgobject
{
 public:
 class objectspace *osp; //object space that this background is in

 uchar bgnum; //number of bg
 class bgdef *bd; //background definition

 //position (<<16)
 int x,y;   //scroll position
 int dx,dy; //delta

 int xw,yw; //width and height of bg

 int updated;
 
 uchar active:1;  //is background activated?


 //bg mapping
 lrect ext; //extent of the background
 lrect *bgiext; //extents of all images
 void calcextent(); //calculate extents of all images

 int cxlen,cylen; //width and height of bg in chunks
 bgchunk *c;   //2-dimensional arrayarray of chunks
 void createchunks();
 void drawchunkbg(char *dest,int cx,int cy);
 void drawchunkfg(char *dest,int cx,int cy);

 class surface *bg; //secondary bg buffer

 //game stuff
 class object *track; //what object is background tracking?

 //member functions
 bgobject(bgdef *tbd,int tx,int ty);
 virtual ~bgobject();

 void activate();
 void deactivate();

 void drawscr(char *dest,int sx,int sy,int x,int y); //draw one scr
 void drawbgscr(char *dest); //draw distant scr's

 void refreshbg(char *dest);
 void drawbg(char *dest); //draw background
 void drawfg(char *dest); //draw foreground

 void move(int tx,int ty) {dx+=tx; dy+=ty;} //move deltas
 int scroll(); //use deltas to scroll

 void tick();

 bgline *getbasey(int tx,int ty,int &by);

 void drawbglines(char *dest);
 bgline *selbgline;

 int r[3]; //rendering counts
 
 #ifdef ANIMATOR
 int emode; //editing modo

 #define BGE_NONE   0
 #define BGE_ACTIVE 1
 #define BGE_IMAGES 2
 #define BGE_LINES  3
 #define BGE_OBJECTS 4

 bgimage *hittest(int mx,int my);
 bgline *linehittest(int mx,int my);
 #endif

};

#endif
