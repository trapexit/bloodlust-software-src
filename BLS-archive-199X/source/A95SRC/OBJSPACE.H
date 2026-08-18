
#ifndef _OBJSPACE_
#define _OBJSPACE_
//class for object workspace
#include "guirect.h"

extern class objectspace *objspace; //current object space


#ifdef ANIMATOR
#define OBJECTDEF objectdefw
#else
#define OBJECTDEF objectdef
#endif

class objectspace:public GUIcontents
{
 #ifdef ANIMATOR
 class objectmove *moving; //are we moving an object with the mouse?
 class epane *ep;  //effect being edited
 #endif

 class object  *o; //linked list of objects in this space

 int updated;
 int needdelete;
 int needload;

 public:
 int maximized;
 int xw,yw; //size of objectspace (nonmaximized)

 int numodf;        //number of objects defined
 class OBJECTDEF **odf; //object definitions

 int numbgdf;    //number of backgrounds defined
 class bgdef **bgdf; //bg definitions

 class object  *p; //currently selected object (for editing)

 class bgobject *bg; //currently shown background

 class region *rlist[3]; //regions of objectspace

 friend class object;
 friend class objectdef;
 friend class bgobject;

 //constructor/destructor
 objectspace(char *objfilename,char *bgfilename,int txw,int tyw); //object file of objspace
 virtual ~objectspace();

 //objectdef maintenence
 void refreshobj();
 void refreshneedload();
 void refreshneeddelete();
 void readobjbin(char *filename);
 void readbgbin(char *filename); 

 //object
 void tick();
 virtual void draw(char *dest);


 #define SETBASEY 0x8000000
 class object *newobject(int num,int x,int y,int z,int d);
 class object *newobject(char *name,int x,int y,int z,int d);
 class bgobject *newbgobject(int num);
 class bgobject *newbgobject(char *name);


 object *addobject(class object *a);
 void removeobject(class object *a);
 void swap(object *a,object *b);

 //-----------------------------------------------
 //gui related
 virtual void restore();
 virtual void maximize();
 void resize(int xw,int yw);
 void moveobjectsrel(int rx,int ry);

 #ifdef ANIMATOR
 virtual int acceptfocus() {return 1;}
 virtual void losefocus() { return; } //workspace never loses focus
 virtual GUIrect *click(class mouse &m);
 virtual int drag(class mouse &m);
 virtual int release(class mouse &m);
 virtual int keyhit(char kbscan,char key);
 virtual char *getname() {return "objectspace";};
 virtual void receivefocus();
 #endif
 static void open(char *objlist,char *bglist,int xw,int yw);

 #ifdef ANIMATOR
 //current selected object control functions
 void neweditableobject(int onum,int activate);
 void neweditablebg(int onum);
 void play();
 void stop();
 void kill();
 void playlooped();
 void activate();
 void flipx();
 void insertframe();
 void insertframeafter();
 void deleteframe();
 void resetodf(int onum);
 void cleanup();

 int readobjectdefs(char *objfilename);
 int readbgdefs(char *bgfilename);

 void writeobjbin(char *filename,int num,class objectdef **odf);

 void seteffectpane(class epane *tep) {ep=tep;}
 #endif

};


#endif


