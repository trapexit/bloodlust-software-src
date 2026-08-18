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

extern img *cursor;
//object that is being edited
extern object *p; //current object being edited (none)

void resetshadow();

//series editor
extern series *seriesedit;

extern enum {EM_IMAGE,EM_REGION,EM_OBJECTS,EM_EFFECT,EM_SERIES} emode;
extern byte *doedit=0;

//image dragging stuff
extern image *dragimg;
extern lpoint dragrel;

extern int onmenu;
extern menu mainmenu;


extern char cfuncnames[256][32];
extern int cfuncnum;
extern char rfuncnames[256][32];
extern int rfuncnum;






extern enum {EMPTY,IMAGES,FRAMES,EFFECTS} cliptype;
extern int clipsize; //size in bytes in buffer
extern int clipnum; //number of images/frames in buffer
extern char clipboard[500];
extern objectdef *clipobj; //the objectdef that the shit was copied from

extern char key,kscan;
extern int donea32;

extern input inputdevice[2];
extern int verbose,drawstat;

extern img *cursor,*shadow,*X,*Xshadow;

