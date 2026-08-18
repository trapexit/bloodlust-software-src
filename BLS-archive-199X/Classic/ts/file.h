//file input/output wrapper functions
typedef class FILEIO
{
 char opened; //flag that tells if it has been opened yet or not
 int error;  //error flag
 int h;           //dos handle of this file
 unsigned bytes; //number of bytes of last read/write
  

 public:
  FILEIO() {opened=0; error=0; }
  int open(char *filename);
  int create(char *filename);
  void close();
  int read(void *t,unsigned size);
  int write(void *t,unsigned size);

  int readint();
  void writeint(int x);

  unsigned size(); //returns file size
  unsigned getpos(); //returns file position
  void     setpos(unsigned int p); //sets file position
  
};

