#ifndef _FILEIO_
#define _FILEIO_

struct datetime
{
 char day,month;
 char hour,minute,second;

 void getdatestr(char *s);
 void gettimestr(char *s);
};

//file input/output wrapper functions
class FILEIO
{
 int h;           //dos handle of this file
 public:
  FILEIO() {h=-1; }
  int open(char *filename);
  int open_rdwr(char *filename);
  FILEIO(char *file) {h=-1; open(file);}
  ~FILEIO() {close();}
  int create(char *filename);
  void close();
  int read(void *t,unsigned size);
  int write(void *t,unsigned size);

 //allocates memory and reads from a file
  void *readalloc(unsigned num)
  {
   void *t=malloc(num);
   if (!t) return 0; //malloc failed
   if (read(t,num)) {free(t); return 0;} //read failed
   return t;
  }

  int readint() {int t=0; read(&t,sizeof(int)); return t;}
  void writeint(int x) {write(&x,sizeof(int));}

  char readchar() {char t=0; read(&t,sizeof(char)); return t;}
  void writechar(char x) {write(&x,sizeof(char));}

  unsigned size(); //returns file size
  unsigned getpos(); //returns file position
  void     setpos(unsigned int p); //sets file position

  void getdatetime(datetime &dt);

  //write RLE
  int read_RLE(void *t,unsigned size);
  int write_RLE(void *t,unsigned size);
};


//function used to enumerate all files in a pathspec
#define DE_FILE  0
#define DE_DIR   1
#define DE_DRIVE 2

typedef int (*DIRFUNCPTR)(char *filename,void *context);
void enumdir(char *path,DIRFUNCPTR func,void *context,int type=DE_FILE);

typedef int (*DRIVEFUNCPTR)(unsigned char drivenum,char *volname,void *context);
void enumdrives(DRIVEFUNCPTR func,void *context);

void setcurrentdrive(int drivenum);
int getcurrentdrive();

void getcurrentdir(char *d);
void setcurrentdir(char *d);

int getfilesize(char *filename);
int fileexists(char *path);

char *resolvepath(char *dir,char *name,char *ext=0);
int absolutepath(char *dir,char *abspath); //1 if successful
char *extractname(char *path);
void getshortpath(char *longpath,char *shortpath);

//directories
extern char romdir[],savedir[],pcxdir[],startupdir[],patchdir[],logdir[];

#endif

