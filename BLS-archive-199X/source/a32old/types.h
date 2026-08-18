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



