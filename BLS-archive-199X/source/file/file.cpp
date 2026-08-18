#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"


int STREAMIO::writeto(STREAMIO &out,unsigned num)
{
 void *t=malloc(num);
 if (!t) return 0;
 num=read(t,num);
 printf("%s writeto %s num=%d\n",getname(),out.getname(),num);
 num=out.write(t,num);
 free(t);
 return num;
}    


char *STREAMIO::printinfo(char *s)
{
 sprintf(s,"%s:%s pos=%d size=%d",getname(),isopened() ? "open" : "unopen",
    getpos(),getsize());
 return s;
};


char *MEMORYIO::printinfo(char *s)
{
 sprintf(s,"%s:%s b=%p pos=%d size=%d",getname(),isopened() ? "open" : "unopen",
    b,p,size);
 return s;
};

char *BUFFERIO::printinfo(char *s)
{
 sprintf(s,"%s:%s b=%p pos=%d size=%d",getname(),isopened() ? "open" : "unopen",
    m.b,m.p,m.size);
 return s;
};


//---------------------------------------------
//---------------------------------------------

int FILEIO::open(char *filename)
{
 if (h) close(); //already open   

 h=::open(filename,O_RDONLY|O_BINARY);
 if (h==-1) {h=0; return -1;} //error
 return 0;
}

int FILEIO::create(char *filename)
{
 if (h) close();

 h=::open(filename,O_WRONLY|O_CREAT|O_TRUNC|O_BINARY,0);
 if (h==-1) {h=0; return -1;} //error
 return 0;
}


void FILEIO::close()
{
 if (!h) return;
 ::close(h);
 h=0;
}    

int FILEIO::read(void *t,unsigned size)
{
 if (!h) return 0;
 return ::read(h,t,size);
}

int FILEIO::write(void *t,unsigned size)
{
 if (!h) return 0;
 return ::write(h,t,size);
}

unsigned FILEIO::getsize()
{
 return h ? filelength(h) : 0;
}

unsigned FILEIO::getpos()
{
 return h ? lseek(h,0,SEEK_CUR) : 0;
}

void FILEIO::setpos(unsigned p)
{
 if (h) lseek(h,p,SEEK_SET);
}



//---------------------------------------------
//---------------------------------------------

void MEMORYIO::close()
{
 if (alloc) if (b) free(b);
 b=0;
 size=p=0;
}    

int MEMORYIO::open(void *t,int tsize)
{
 if (b) close(); //close if already open
 b=(char *)t;
 size=tsize;
 p=0; //position at beginning of memory
 alloc=0;
 return 0;
}    

int MEMORYIO::create(int tsize)
{
 if (b) close();
 size=tsize;
 b=(char *)malloc(size);
 p=0;
 alloc=1;
 return 0;
}

int MEMORYIO::read(void *t,unsigned num)
{
 if (!b || !num) return 0;
 if (p+num>=size)
  {
   if (p>=size) return 0;
   num=size-p; //past end of file?
  }
 memcpy(t,b+p,num);
 p+=num;
 return num;
}

int MEMORYIO::write(void *t,unsigned num)
{
 if (!b || !num) return 0;
 if (p+num>=size) //write past end of file?
  {
//   size=p+num; //resize file
//  b=(char *)realloc(b,p+num); //realloc
//   printf("realloc %d\n",size);
   if (p>=size) return 0;
   num=size-p;
  }

 memcpy(b+p,t,num);
 p+=num;
// if (p>size) size=p;
 return num;
}    




int MEMORYIO::writeto(STREAMIO &out,unsigned num)
{
 if (!b) return 0;
 if (p+num>=size) //past end of file?
  {
   if (p>=size) return 0;
   num=size-p; 
  }

 int numwritten=out.write(b+p,num);
 p+=numwritten;
 return numwritten;
}    


int MEMORYIO::readfrom(STREAMIO &in,unsigned num)
{
 if (p+num>=size) //write past end of file?
  {
 //   b=(char *)realloc(b,p+num); //realloc
//    printf("realloc %d\n",p+num);
   if (p>=size) return 0;
   num=size-p;
  }

 int numread=in.read(b+p,num);
 p+=numread;
// if (p>size) size=p;
// printf("%p memoryio::readfrom p=%d size=%d numread=%d\n",this,p,size,numread);
 return numread;
}    



int STREAMIO::operator <<(class MEMORYIO &in)
     {in.reset(); return in.writeto(*this,in.getsize());}     

int STREAMIO::operator >>(class MEMORYIO &out)
     {reset(); return out.readfrom(*this,getsize());}




//---------------------------------
//---------------------------------


int CLIPSTREAM::read(void *t,unsigned num)
    {
     int pos=getpos();
     if (pos+num>end)
      {
       if (pos>end) return 0;
       num=end-pos;
      };
     return p->read(t,num);
    };
    
int CLIPSTREAM::write(void *t,unsigned num)
    {
     int pos=getpos();
     if (pos+num>end)
      {
       if (pos>end) return 0;
       num=end-pos;
      };
     return p->write(t,num);
    };


void CLIPSTREAM::open(STREAMIO *o,int tstart,int tsize)
   {
    p=o;
    start=tstart;
    end=tstart+tsize;
    int psize=p->getsize();
    
    if (start>psize) start=psize;
    if (end>psize) end=psize;
    reset();
   };

