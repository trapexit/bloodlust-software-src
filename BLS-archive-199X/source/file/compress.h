#ifndef _COMPRESS_
#define _COMPRESS_

//--------------------------
//bit stream
class BITSTREAM
{
 protected:
 STREAMIO *s;
 
 unsigned char c;
 unsigned char mask;
 int pos;  //pos in stream (in bits)
 int size; //number of bits in stream

 public:
  //position based stuff
  unsigned getsize() {return size;} 
  unsigned getpos() {return pos;} 
  void setpos(unsigned int p)
   {
    pos=p;
    s->setpos(pos>>8);
    c=s->readchar();
    mask=0x80>>(pos&7);
   };
  void reset() {pos=0; s->setpos(0); mask=0;}
 virtual char *getname() {return "BITSTREAM";}

 BITSTREAM(STREAMIO *t):s(t)  {c=0; size=pos=0; mask=0;}
};    

//input bit stream
class BITSTREAMI : public BITSTREAM
{
 public:
  int readbit()
   {
    if (pos>size) return 0;
    if (!mask) {mask=0x80; c=s->readchar();}
    int bit=c&mask;
    mask>>=1; pos++;
    return bit;
   };
 unsigned readbits(int count)
  {
   unsigned bits=0;
   unsigned bmask=1<<(count-1);
   while (bmask)
   {
    if (readbit()) bits|=bmask;
    bmask>>=1;
   }
   return bits;
  }

 void begin() {c=0; mask=0;}
 void end() {c=0; mask=0;}

 BITSTREAMI(STREAMIO *t):BITSTREAM(t) {size=s->getsize()*8; begin();}

 virtual char *getname() {return "BITSTREAMI";}

};    


//output bit stream
class BITSTREAMO : public BITSTREAM
{
 public:
  void writebit(int bit)
   {
    if (bit) c|=mask;
    mask>>=1; pos++;
    if (!mask) {s->writechar(c); c=0; mask=0x80; }
    if (pos>size) size=pos;
   };
   
  void writebits(unsigned bits,int count)
  {
   unsigned bmask=1<<(count-1);
   while (bmask)
   {
    writebit(bits&bmask);
    bmask>>=1;
   }
  }
 void begin() {c=0; mask=0x80;}
 void end() {if (mask!=0x80) s->writechar(c); begin();}
 
 BITSTREAMO(STREAMIO *t):BITSTREAM(t)  {begin();}
 ~BITSTREAMO() {end();}

 virtual char *getname() {return "BITSTREAMO";}
 
};    

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

#define BITS                       15
#define MAX_CODE                   ( ( 1 << BITS ) - 1 )
#define TABLE_SIZE                 35023L
#define TABLE_BANKS                ( ( TABLE_SIZE >> 8 ) + 1 )
#define END_OF_STREAM              256
#define BUMP_CODE                  257
#define FLUSH_CODE                 258
#define FIRST_CODE                 259
#define UNUSED                     -1


//class for LZW compression
class LZW:public STREAMIO
{

 public:
  virtual int read(void *t,unsigned num) {return 0;};
  virtual int write(void *t,unsigned num) {return 0;};
  virtual int writeto (STREAMIO &out,unsigned num) {return 0;};
  virtual int readfrom(STREAMIO &in,unsigned num) {return 0;};

  LZW();
  ~LZW();

  virtual char *getname() {return "LZW";}

 protected:
  unsigned int find_child_node( int parent_code, int child_character );
  unsigned int decode_string( unsigned int offset, unsigned int code );

  struct dictionary {
    int code_value;
    int parent_code;
    char character;
  } *dict[ TABLE_BANKS ];

  char *decode_stack;
  unsigned int next_code;
  int current_code_bits;
  unsigned int next_bump_code;
  int gotstringcode;
  int string_code;

  void InitializeDictionary();
};    


class COMPRESS: public LZW
{
 BITSTREAMO out; //output bitstream

 public:
 int compress(char *t,int num);

 void start();
 void stop();
 
 virtual int write(void *t,unsigned num)
  {return compress((char *)t,num); };

 COMPRESS(STREAMIO *o):out(o) {}
};

class DECOMPRESS: public LZW
{
 BITSTREAMI in; //input bitstream

 public:
 int decompress(STREAMIO *out);
 void start();
 void stop();
 
 int operator >>(STREAMIO &out) {return decompress(&out);}

 DECOMPRESS(STREAMIO *i):in(i) {}
};












#endif
