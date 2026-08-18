//Copyright(c) 1996 Bloodlust Software All rights reserved
//volume file manager
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <direct.h>

#include <windows.h>
#include <dsound.h>

#include "vol.h"


extern char errstr[];
extern void cleanexit(int x);

char dsl[4]="DSL";

//error handling portion  0=abort, 1=revolumize
int erroraction=0; //action to be taken if there is an error opening file

//0-ignore, return 0     1-abort
int vwerror=1;      //action to be taken while writing file

int vdiagnose=0;

char *voltype[]=
{
 "header",
 "rawdata",
 "image",
 "sound",
 "midi",
 "palette",
 "screen",
 "block"
 
};



//included for writing only
#include "volw.cpp"


//opens volume file
int volumefile::open(char *filename)
{
 if (!strchr(filename,'.')) strcat(filename,".vol");

 if (_dos_open(filename,O_BINARY | O_RDONLY,&h))
  {return -1;}

 char name[16];
 GetFileName(filename,name); //get name of file
 _dos_read(h,&hdr,sizeof(header),&bytes); //get header

 if (strcmp(hdr.key,dsl) && hdr.type!=V_VOLHEADER || stricmp(hdr.name,name)) //invalid header
  {sprintf(errstr,"corrupt volume file %s",name); close(); cleanexit(-1);}
 strcpy(volname,hdr.name);

 if (vdiagnose)
  printf("open:   %8s\n",filename);
 return(0); 
}    

//resets to beginning of file
void volumefile::reset()
{
 lseek(h,0,SEEK_SET);
 _dos_read(h,&hdr,sizeof(header),&bytes); //get header

 if (strcmp(hdr.key,dsl) && hdr.type!=V_VOLHEADER) //invalid header
  {sprintf(errstr,"corrupt volume file while resetting"); close(); cleanexit(-1);}
}    

//closes volume file
void volumefile::close()
{
 _dos_close(h);
 if (vdiagnose)
  printf("close:  %8s\n\n",volname);
}    



//reads single file from volume file, allocs memory for it
void *volumefile::read()
{
if (_dos_read(h,&hdr,sizeof(header),&bytes) || strcmp(hdr.key,dsl))
 {sprintf(errstr,"corrupt volume file %s",volname); cleanexit(1);}

if (!bytes) return 0; //end of file

 void *t=0;
 if (hdr.size)
  {
   t=(void *)malloc(hdr.size); //get memory
   if (!t) {sprintf(errstr,"out of memory. volume: %s  file: %s  bytes: %d",volname,hdr.name,hdr.size); cleanexit(-2);}

   _dos_read(h,t,hdr.size,&bytes);
    if (hdr.size!=bytes) {sprintf(errstr,"reading volume %s bytes: %d size: %d",volname,bytes,hdr.size); cleanexit(-3);}
  }

 if (vdiagnose)
  printf("read:   %8s size: %7d type: %s\n",hdr.name,hdr.size,voltype[hdr.type]);
 return(t);
}



//reads single file from volume file to pointer
void volumefile::read(void *t)
{
if (_dos_read(h,&hdr,sizeof(header),&bytes) || strcmp(hdr.key,dsl))
 {sprintf(errstr,"corrupt volume file %s",volname); cleanexit(1);}

if (!bytes) return; //end of file

 if (hdr.size)
  {
   _dos_read(h,t,hdr.size,&bytes);
    if (hdr.size!=bytes) {sprintf(errstr,"reading volume %s bytes: %d size: %d",volname,bytes,hdr.size); cleanexit(-3);}
  }

 if (vdiagnose)
  printf("read:   %8s size: %7d type: %s\n",hdr.name,hdr.size,voltype[hdr.type]);
}



void ** volumefile::readblock(int *num)
{
if (_dos_read(h,&hdr,sizeof(header),&bytes) || strcmp(hdr.key,dsl))
 {sprintf(errstr,"corrupt volume file %s",volname); cleanexit(1);}

if (!bytes) return 0; //end of file

 void **t=0;
 if (hdr.size)
  {
   _dos_read(h,num,4,&bytes); //get number of items
   if (*num)
   {
   t=(void **)malloc(hdr.size-4); //get memory
   if (!t) {sprintf(errstr,"out of memory. volume: %s  file: %s  bytes: %d",volname,hdr.name,hdr.size); cleanexit(-2);}

   _dos_read(h,t,hdr.size-4,&bytes);
    if (hdr.size-4!=bytes) {sprintf(errstr,"reading volume %s bytes: %d size: %d",volname,bytes,hdr.size); cleanexit(-3);}
    for (int i=0; i<*num; i++) //adjust indices
     ((unsigned *)t)[i]+=((unsigned)t)+ (*num)*4;
   }
  }

 if (vdiagnose)
  printf("read:   %8s size: %7d type: %s numelements: %d\n",hdr.name,hdr.size,voltype[hdr.type],*num);

 return(t);
}    

/*
block *volumefile::readblock()
{
if (_dos_read(h,&hdr,sizeof(header),&bytes) || strcmp(hdr.key,dsl))
 {sprintf(errstr,"corrupt volume file %s",volname); cleanexit(1);}

 block *t=0;
 if (hdr.size)
  {
   t=(block *)malloc(hdr.size); //get memory
   if (!t) {sprintf(errstr,"out of memory. volume: %s  file: %s  bytes: %d",volname,hdr.name,hdr.size); cleanexit(-2);}

   _dos_read(h,t,hdr.size,&bytes);
    if (hdr.size!=bytes) {sprintf(errstr,"reading volume %s bytes: %d size: %d",volname,bytes,hdr.size); cleanexit(-3);}
    for (int i=0; i<t->num; i++) //adjust indices
     t->d[i]=(void *)(((unsigned)t)+((unsigned)t->d[i]));
  }

 if (vdiagnose)
  printf("read:   %8s size: %7d type: %s numelements: %d\n",hdr.name,hdr.size,voltype[hdr.type],t ? t->num : 0);
 return(t);
}    
*/
//skips ahead
void volumefile::skip()
{
if (_dos_read(h,&hdr,sizeof(header),&bytes) || strcmp(hdr.key,dsl))
 {sprintf(errstr,"corrupt volume file %s",volname); cleanexit(1);}

if (!bytes) return; //end of file

lseek(h,hdr.size,SEEK_CUR);
if (vdiagnose)
  printf("skipped:%8s size: %5d type: %2s\n",hdr.name,hdr.size,voltype[hdr.type]);

}    



void GetFileName(char *fullname, char *name)
{
 for (int i=0; fullname[i] && fullname[i]!='.'; i++); //find period
 int p=i; //period location
 for ( ; i>0 && fullname[i]!='\\'; i--); //find beginning of name
 if (i>0) i++;
 for (int j=0; i<p; i++,j++) name[j]=fullname[i]; //copy name
 name[j]=0;
}

void GetFileExtension(char *fullname, char *ext)
{
 for (int i=0; fullname[i] && fullname[i]!='.'; i++); //find period
 if (!fullname[i]) {ext[0]=0; return;} //no period found
 i++;
 for (int j=0; j<3; j++,i++) ext[j]=fullname[i];
 ext[j]=0;
}



