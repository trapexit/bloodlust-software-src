#include "smix.h"

#include "types.h"
#include "file.h"
#include "r2img.h"
#include "font.h"
#include "vol.h"
#include "misc.h"
#include "keyb.h"

#include "input.h"

#include "effect.h"
#include "object.h"
#include "amenu.h"
#include "adialog.h"

#include "bg.h"


extern FONT *font[6];

extern int mx,my,mb,mstat,mrelstat,oldmb; //mouse x,y,buttons

//dialogs  
extern dialog dlg;

//list of all objects
extern object *o;

extern int numodf;
extern struct objectdef *odf;
extern int numbgdf;
extern struct backgrounddef *bgdf;


//current background
extern class bgobject *bgobj; //none at first

extern IMG *cursor;
//object that is being edited
extern object *p; //current object being edited (none)

void resetshadow();

//series editor
extern series *seriesedit;

typedef enum {EM_IMAGE,EM_REGION,EM_OBJECTS,EM_EFFECT,EM_SERIES} EMODE; 
extern EMODE emode;
extern byte *doedit;

//image dragging stuff
extern image *dragimg;
extern lpoint dragrel;

extern int onmenu;
extern menu mainmenu;


extern char cfuncnames[256][32];
extern int cfuncnum;
extern char rfuncnames[256][32];
extern int rfuncnum;





typedef enum {EMPTY,IMAGES,FRAMES,EFFECTS} CLIPTYPE;
extern CLIPTYPE cliptype;
extern int clipsize; //size in bytes in buffer
extern int clipnum; //number of images/frames in buffer
extern char clipboard[500];
extern objectdef *clipobj; //the objectdef that the shit was copied from

extern char key,kscan;
extern int donea32;

extern input inputdevice[2];
extern int verbose,drawstat;

extern IMG *cursor,*shadow,*X,*Xshadow;

extern char *screen,*video;
extern unsigned int speed;

inline void DrawString(FONT *f,char *s,char *str,int x,int y)
{f->draw(str,s,x,y);}
inline void DrawCenteredString(FONT *f,char *s,char *str,int x,int y)
{f->drawcentered(str,s,x,y);}
inline void DrawLetter(FONT *f,char *s,char c,int x,int y)
{f->draw(c,s,x,y);}

inline void DrawImage(IMG *i,char *d,int x,int y,int o)
{i->draw(d,x,y,o);}


