#ifndef _FILEIO_
#define _FILEIO_
//file input/output wrapper functions
class FILEIO
{
 int h;           //dos handle of this file
 unsigned bytes; //number of bytes of last read/write
  

 public:
  char opened; //flag that tells if it has been opened yet or not
  int error;  //error flag
 
  FILEIO() {opened=0; error=0; }
  int open(char *filename);
  FILEIO(char *file) {opened=0; error=0; open(file);}
  ~FILEIO() {close();}
  int create(char *filename);
  void close();
  int read(void *t,unsigned size);
  int write(void *t,unsigned size);

 //allocates memory and reads from a file
  void *readalloc(unsigned size);

  int readint();
  void writeint(int x);

  unsigned size(); //returns file size
  unsigned getpos(); //returns file position
  void     setpos(unsigned int p); //sets file position
  
};

#endif

