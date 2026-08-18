#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <errno.h>
#include <malloc.h>


#include "file.h"


//returns 0 on successful open
int FILEIO::open(char *filename)
{
// if (opened) return -2;
 h=0; bytes=0;
 if (_dos_open(filename,O_BINARY | O_RDWR,&h))
     {error=errno; return -1;}
 opened=1; 
 return 0;
}

//returns 0 on successful create
int FILEIO::create(char *filename)
{
// if (opened) return -2;
 h=0; bytes=0;
 if (_dos_creat(filename,0,&h))
     {error=errno; return -1;}
 opened=1;
 return 0;
}

void FILEIO::close()
{
 if (!opened) return;
 bytes=0;
 _dos_close(h);
 opened=0;
}    

int FILEIO::read(void *t,unsigned size)
{
 if (!opened) return -2;
 if (_dos_read(h,t,size,&bytes)) {error=errno;  return -1;}
 if (size!=bytes) return -1;
 return 0;
}

int FILEIO::write(void *t,unsigned size)
{
 if (!opened) return -2;
 if (_dos_write(h,t,size,&bytes)) error=errno;
 if (size!=bytes) return -1;
 return 0;
}


void *FILEIO::readalloc(unsigned size)
{
 if (!opened) return 0;
 void *t=malloc(size);
 if (!t) return 0;

 if (read(t,size)) {free(t); return 0;} //read failed
 return t;
}



//reads one integer
int FILEIO::readint()
{
 int t=0;
 read(&t,sizeof(int));
 return t;
}
//write one integer
void FILEIO::writeint(int x)
{
 write(&x,sizeof(int));
}


unsigned FILEIO::size()
{
 if (!opened) return 0;
 return filelength(h);
}

unsigned FILEIO::getpos()
{
 if (!opened) return -1;
 return lseek(h,0,SEEK_CUR);
}

void FILEIO::setpos(unsigned p)
{
 if (!opened) return ;
 lseek(h,p,SEEK_SET);
}















