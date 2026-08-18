


#define BG_IMG    0
#define BG_REGION 1
#define BG_OBJECT 2

//structure specifying type of item, corner, and index
struct bgitem {
 ushort i:12; //index into array (depends on type)
 ushort type:2; //type of point this is
 //00-UL 01-UR 10-DL 11-DR
 ushort x:1;
 ushort y:1;
};

//individual point on scan line
struct bgpoint {
 short dx;
 bgitem item; //item at this point
};    

//scan line
struct bgscanline {
 ushort  nump; //number of points
 bgpoint *p;

 short y; //y coord of this line
 void addpoint(int x,bgitem a);
 void deletepoint(int i);
 void scroll(int dx);
 void free(); //clears scan line
 
 void print(); //prints all the points on a line
};    


//position within line, used for advancing and scanning line
struct bgpos {
 int x,y;

 int count; //current count within point
 ushort currp; // current point num
 struct bgpoint *p;   // current point pointer
 struct bgscanline *l; //line that this pos references

 void start(bgscanline *line);
 int  scroll(int dx,class bgobject *b);
 void moveto(int tx);
};

//series of scanlines, sorted by Y
struct bgmap {
 int numscanlines;  
 bgscanline *line; //pointer to array of all scanlines

 bgscanline *findscanline(int y);
 bgscanline *addscanline(int y);
 void deletescanline(int i);

 void addimage(img *i,int idx,int x,int y,int z); //adds an image to the map
 
 bgpos *createbgpos(int tx); //creates an array of bgpos info for this map (for bgobject)

 void addpoint(int x,int y,bgitem a); //adds a single item to the map at x,y
 void deleteitem(bgitem a);
 void free();                          //frees whole map

 //diagnostic shit
 void print(); //prints all the points on a line
 void drawgrid(char *d,int x,int y);
};    


//class for an active background image to be actually drawn on screen
class bgactiveimage {
 public:
 bgactiveimage *prev,*next;

 bgobject *o;
 ushort idx;
 
 img *iptr;      //pointer to image data
 uchar orient;
 int x,y,z;  //screen position

 bgactiveimage::bgactiveimage(int i,bgobject *bgo);
 ~bgactiveimage(); //destructor
 
 void scroll(int dx,int dy);
 void draw(char *d);
 void resort();
};    



//structure for each background image definitin
struct bgimage:public image {
 short dispz; //adds a Z dimension
 void update(bgactiveimage *a);
 
};    

//structure for each line definition composing a bg boundary
struct bgline {
 #define BGL_HORIZ 1
 #define BGL_VERT  2 
 
 uchar  type:2;
 short int x1,x2,y1,y2;

 void set(int tx1,int ty1,int tx2,int ty2);
 void draw(int color);
 int getbasey(int tx,int ty);
 int calcy(int tx);
};    


typedef char SCREEN[64000];

//structure for background definition
struct backgrounddef:public objectdef
{
//map containing position info for all items    
bgmap map;    

//array of images on background
int numbgimages;
bgimage *bgi;

//array of screens on background
int numscreens;
SCREEN **scr;
defname *screennames; //list of all screen names
uchar (*scrmap)[64][64];

//array of screen boundary lines
int numbglines;
bgline *bgl; 

//functions to add/remove images from the array and map
void remove(bgactiveimage *x);
void update(bgactiveimage *x);
bgactiveimage *add(bgimage *x);
void add(bgline *x);

int RequestScreen();

//functions to create the map from the array data
void generatemap();
void clearmap(); 

void Read();  //Read Object into memory(if not loaded)
void Kill();  //Free object from memory
void Save(); //save object
void volumize();
};


//actual object for active backgroun
class bgobject:public object
{
public:
int dx,dy;  //delta x,y

//scan line pointers to bgmap lines
int numscanlines;
bgpos *posleft; //pos for left edge of screen

//list of active images on screen now
bgactiveimage *i;
bgactiveimage **iarray; //array of active images (numbgimages in length)

bgactiveimage *foregroundptr;//pointer to first foreground image

char *bg; //background buffer

void addactiveimage(bgactiveimage *);

//process right edge items
void processright(bgitem a);
//process left edge items
void processleft(bgitem a);

void activateitem(bgitem a);
void deactivateitem(bgitem a);

bgline *getbasey(int x,int y,int *by=0);

void free();
void reset();
void moveto(int x,int y);

void refreshbackground(char *d);
void drawbackground(char *d);
void drawforeground(char *d);

void tick();
int  scroll(int tx,int ty);

void activate();
void stop();


bgobject(backgrounddef *bg);
virtual ~bgobject();

#ifdef ANIMATOR
void drawlines();
void drawall(char *dest); //draw background by drawing every array image
virtual void DrawStatus();
virtual void drawbuttons();
virtual void mainedit();
bgactiveimage *CheckImageBound();
#endif
};    







