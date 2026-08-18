typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;


//short point
typedef struct point { short int x,y;} point;

//long point
struct lpoint  {int x,y;};

//short 3d point
struct zdpoint
{
 short int x;
 signed char z;
 uchar d;
 short int y;
} ;

struct zpoint {short int x,z,y;};

//long point
typedef struct lzpoint  {int x,y,z;} lzpoint;

//rectangle (short)
typedef struct rect
{
signed short int x1,y1,x2,y2;
} rect;

//3d rectangle (short)
typedef struct zrect
{
 zpoint p1,p2;
} zrect;


//color as part of palette
typedef struct color
{
unsigned char   r,g,b;
} color;

//actual image struct for DrawImage()
typedef struct img
{
char type;        //not used
unsigned char xw,yw;
char transparent; //not used
char data[];
} img;    



//timg256.asm
extern "C"
{
void __cdecl PutImageFlip256(char *,char *,int LSW,int o);
void __cdecl PutImageFlipClip256(char *, char *,int LSW,int XC,int YC,int XT,int YT,int o);

void __cdecl DrawImage(img *src,char *dest,int x,int y,int o);
void __cdecl DrawImageMap(img *src,char *dest,int x,int y,int o);
void __cdecl PutImageD2(char *,char *,int,int);
void __cdecl PutImageSolid(char *,char *,int,long);

void __cdecl ColorMapImage(char *img,unsigned char *map);
void __cdecl MemoryCopy(void *,void *,unsigned int);

void __cdecl WaitVSync();
void __cdecl Solid(char *,int,int);
void __cdecl LoadPalette(color *, int, int);
void __cdecl DrawBar(char *b, int color, int x,int y,int xw,int yw);
void __cdecl DrawBarMap(char *b, unsigned char *map, int x,int y,int xw,int yw);

}

//indices used for DrawImageMap
extern "C" unsigned char SHADOWMAP[256];


//changing video modes
extern "C"
{
void __cdecl Mode256();
void __cdecl ModeText();
}



//FONT stuff
typedef struct FONT {
  unsigned short idx[256];
  char data[];
} FONT;


//FONT drawing functions
void DrawCenteredString(FONT *font,char *dest,char *s,int x,int y);
void DrawString(FONT *font,char *dest,char *s,int x,int y);
int GetStringWidth(FONT *font,char *s);
int DrawLetter(FONT *font,char *dest,char c,int x,int y);
int GetLetterWidth(FONT *font, char s);
void FunkyFont(FONT *font,unsigned char a,unsigned char b);
void dprintf(FONT *font,int x,int y,char *format,...);
void FuckWithImage(char *s,unsigned char c1,unsigned char c2);

extern char *screen;


void line(int XStart, int YStart, int XEnd, int YEnd, int Color);


int random(int x);
