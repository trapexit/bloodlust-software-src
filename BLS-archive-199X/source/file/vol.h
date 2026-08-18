#ifndef _VOL_
#define _VOL_

#include "file.h"

#define VOL_ERR -1
#define VOL_OK 0

#define VOLEXT ".VOL"


//header for each entry in the volume file
struct header
{
char key[4];   //must be "DSL"    
char type;     //type of data
unsigned size; //size of data (uncompressed)
char name[9];  //name of data
};

//structure for open volume file
class volumefile
{
public:
char volname[9]; //name of this file
header hdr;       //last header read/written

static int vdiagnose; //print diagnose messages?
static int vwerror;   //abort on error?
static char *voltype[];
};




//structure for reading from volumefile
class volumeread :public volumefile
{
STREAMIO *f;   //actual file stream
BUFFERI *bin;  //buffered input for file
DECOMPRESS *d; //decompression stream
public:

volumeread();
~volumeread();

//read functions
int open(char *filename); //open volume file
void close();    

int readheader();

void *read();
void  read(void *t);
void **readblock(int &num);
void skip();
};    


//structure for writing to volumefile
class volumewrite:public volumefile
{
STREAMIO *f;   //actual file stream
BUFFERO *bout; //buffered output for file
COMPRESS *c;   //compression stream

public:

volumewrite();
~volumewrite();

//write functions
int create(char *filename);
void close();

void writeheader(int type,int size,char *name);

void writeclose();
void write(void *t,unsigned size,char type,char *name);
void writefile(char *filename);
void writemidi(char *filename);
void writescreen(char *filename);
void writepalette(char *filename);
void writelistblock(char *listfile,char type); 
void writesound(char *filename);
void writeimage(char *filename);

//void writeoldimage(char *filename);
//void writebbmfile(char *filename);
//void writelbmfile(char *filename);
};    





//volume header
#define V_VOLHEADER 0
//pure data
#define V_RAWDATA   1
//img file new type (with ylist)
#define V_IMAGE 2
//16000hz mono signed data
#define V_SOUND 3
//type 0 midi file
#define V_MIDI  4
//color[256]
#define V_PALETTE 5
//uncompressed image
#define V_SCREEN 6
//blockheader + block
#define V_BLOCK 7

//volume header
#define V_VOLEND 0xFF


void GetFileName(char *fullname, char *name);
void GetFileExtension(char *fullname, char *ext);
#endif







