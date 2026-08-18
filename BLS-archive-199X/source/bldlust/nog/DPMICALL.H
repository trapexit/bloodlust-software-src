/*      SMIXB is Copyright 1995 by Ethan Brodsky.  All rights reserved      */

/* лл dpmicall.h лллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

#undef outportb
#undef inportb

long get_limit(short sel);
void set_limit(short sel, unsigned long limit);
long get_base(short sel);

void outportb(unsigned __portid, unsigned char __value);
unsigned char inportb(unsigned __portid);

void *low_malloc(int size, long int *phyaddress, short int *sel, long int *handle);
void low_free(short int sel, long int handle);

typedef struct
  {
    short int sel;
    long  int ofs;
  } FARPTR;


void _dpmi_getvect(unsigned char interruptno, FARPTR *oldisr);
void _dpmi_setvect(unsigned char interruptno, FARPTR isr);
void _setvect(unsigned char interruptno, void *isr);

/* лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */
