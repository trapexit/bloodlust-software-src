//dos long file name platform layer
//Copyright 1997 Bloodlust software
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <direct.h>

#include "dosrmi.h"
#include "doslfn.h"


//do long file names exist?
char lfnexists=0;

//--------------------------------------
// low memory functions

//conventional memory allocation functions
void *low_malloc(int size, short int *sel);
void low_free(short int sel);

//long file name buffers in low memory
char *lfnbuf1;
char *lfnbuf2;

//low memory selectors
short lfnsel1,lfnsel2;


static RMI rmi;


//---------------------------------
// long file name functions


//----------------------
//initialization
void lfn_init()
{
 //msdos 7.0 only
 if (_osmajor<7) {lfnexists=0; return;}

 //create low buffers
 lfnbuf1=(char *)low_malloc(512,&lfnsel1);
 lfnbuf2=(char *)low_malloc(512,&lfnsel2);
 if (!lfnbuf1 || !lfnbuf2) return;
 lfnexists=1;
};


//-------------------------
// file open/create/close

int lfn_open(char *path)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,path);
  rmi.setdssi(lfnbuf1); //file name
  rmi.ebx=O_BINARY|O_RDONLY;
  rmi.edx=1;
  if (!rmi.dosint(0x716C)) return rmi.eax&0xFFFF;
  if (rmi.getdoserr()!=0x7100) return -1;
 }
 int h;
 return !_dos_open(path,O_RDONLY|O_BINARY,&h) ? h : -1;
// return open(path,O_RDONLY|O_BINARY);
};

int lfn_openrdwr(char *path)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,path);
  rmi.setdssi(lfnbuf1); //file name
  rmi.ebx=O_BINARY|O_RDWR;
  rmi.edx=1;
  if (!rmi.dosint(0x716C)) return rmi.eax&0xFFFF;
  if (rmi.getdoserr()!=0x7100) return -1;
 }
 int h;
 return !_dos_open(path,O_RDWR|O_BINARY,&h) ? h : -1;
// return open(path,O_RDWR|O_BINARY);
};

int lfn_create(char *path)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,path);
  rmi.setdssi(lfnbuf1); //file name
  rmi.ebx=O_BINARY|O_RDWR;
  rmi.ecx=0;
  rmi.edx=0x10;
  if (!rmi.dosint(0x716C)) return rmi.eax&0xFFFF;
  if (rmi.getdoserr()!=0x7100)
   {
    rmi.edx=0x2;
    if (!rmi.dosint(0x716C)) return rmi.eax&0xFFFF;
    return -1;
   }
 }
 int h;
 return !_dos_creat(path,0,&h) ? h : -1;
// return creat(path,S_IWRITE|S_IREAD);
};

void lfn_close(int h)
{
 _dos_close(h);
};


//-----------------------
// dir stuff


void lfn_chdir(char *dir)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,dir);
  rmi.setdsdx(lfnbuf1); //file name
  if (!rmi.dosint(0x713B)) return;
  if (rmi.getdoserr()!=0x7100) return;
 }
 chdir(dir);
};



void lfn_getshortname(char *p,char *shortp)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,p);
  rmi.setdssi(lfnbuf1); //long path
  rmi.setesdi(lfnbuf2); //shortpath
  rmi.ecx=1;
  if (!rmi.dosint(0x7160)) {strcpy(shortp,lfnbuf2); return;}
 }
 strcpy(shortp,p);
};


void lfn_getlongname(char *shortp,char *p)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,shortp);
  rmi.setdssi(lfnbuf1); //long path
  rmi.setesdi(lfnbuf2); //shortpath
  rmi.ecx=2;
  if (!rmi.dosint(0x7160)) {strcpy(p,lfnbuf2); return;}
 }
 strcpy(p,shortp);
};



//-----------------------
//findfile

static find_t oldff;

unsigned lfn_findfirst(char *path,int attrib,lfn_find *ff)
{
 if (lfnexists)
 {
  strcpy(lfnbuf1,path);
  rmi.setdsdx(lfnbuf1); //file name
  rmi.setesdi(lfnbuf2); //finddata
  rmi.esi=0;
  rmi.ecx=attrib;

  if (!rmi.dosint(0x714E))
  {
   memcpy(ff,lfnbuf2,sizeof(lfn_find));
   return rmi.eax&0xFFFF; //handle
  }
  if (rmi.getdoserr()!=0x7100) return 0; //failed
 }

 //use old findfirst
 int result=_dos_findfirst(path,attrib,&oldff);
 *ff=oldff; //copy to new find format
 return !result; //returns 0 if done else fake handle
}


unsigned lfn_findnext(unsigned handle,lfn_find *ff)
{
 if (lfnexists)
 {
  memcpy(lfnbuf2,ff,sizeof(lfn_find));
  rmi.setesdi(lfnbuf2); //finddata
  rmi.esi=0;
  rmi.ebx=handle;
  if (!rmi.dosint(0x714F))
  {
   memcpy(ff,lfnbuf2,sizeof(lfn_find));
   return 1; //keep going...
  }
  if (rmi.getdoserr()!=0x7100) return 0; //failed
 }

 //use old findfirst
 int result=_dos_findnext(&oldff);
 *ff=oldff; //copy to new find format
 return !result; //returns 0 if done else fake handle
};



void lfn_findclose(unsigned handle)
{
 if (lfnexists)
 {
  rmi.ebx=handle;
  rmi.dosint(0x71A1);
 }
};




//--------------------------------------------------
//low mem alloc shit

/*
void dosmemalloc(short para, short *seg, short *sel);
#pragma  aux dos_memalloc = \
  "push  ecx"               \
  "push  edx"               \
  "mov   ax, 0100h"         \
  "int   31h"               \
  "pop   ebx"               \
  "mov   [ebx], dx"         \
  "pop   ebx"               \
  "mov   [ebx], ax"         \
  parm   [bx] [ecx] [edx]   \
  modify [ax ebx ecx edx];

void dosmemfree(short sel);
#pragma  aux dosmemfree =  \
  "mov   ax, 0101h"         \
  "int   31h"               \
  parm   [dx]               \
  modify [ax dx];

void  *low_malloc(int size, short int *sel)
  {
    short int seg;
    dosmemalloc((size>>4) + 1, &seg, sel);
    return((char *)(seg << 4));
  }

void low_free(short int sel)
  {
    dosmemfree(sel);
  }*/



