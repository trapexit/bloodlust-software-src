//Copyright(c) 1996 Bloodlust Software All rights reserved
//animator reading file functions
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

#define ANIMATOR

#include "misc.h"
#include "ttimer.h"
#include "keyb.h"

#include "file.h"

#include "glib.h"
#include "motion.h"
#include "input.h"
#include "config.h"
#include "smix.h"
#include "vol.h"


#include "effect.h"
#include "object.h"

#include "bg.h"


extern int numodf;
extern objectdef *odf;

extern int numbgdf;
extern backgrounddef *bgdf;


void ReadObjectNames(char *onf)
{
FILE *f;
f=fopen(onf,"rt");
if (!f) {printf("cannot open object definition file %s\n",onf); exit(1);}

char line[100];
odf=(objectdef *)malloc(256*sizeof(objectdef));  //allocate memory for all odf
memset(odf,0,256*sizeof(objectdef));

numodf=0;

while (fgets(line,100,f)) //read line into memory
{
//   printf("%s\n",line);
static  char name[32],sdf[32],dir[32],volfile[32];
static  int onum;
  if (sscanf(line,"%d %s %s %s %s",&onum,name,sdf,dir,volfile)<5)
   break;//  {printf("error in object list\n%s\n",line);  exit(1);}

  //set up objectdef
  odf[onum].onum=onum; //ordinal number of type
  odf[onum].type=OBJDEF;
  strcpy(odf[onum].name,name);
  strcpy(odf[onum].sdf,sdf);
  strcpy(odf[onum].dir,dir);
  strcpy(odf[onum].volfile,volfile);

  if (onum>numodf) numodf=onum;
} 
numodf++;

odf[0].onum=0;
odf[0].loaded=0;
strcpy(odf[0].name,"NULL");
odf[0].sdf[0]=odf[0].dir[0]=0;


//resize odf
odf=(objectdef *)realloc(odf,numodf*sizeof(objectdef)); 
fclose(f);

{
FILEIO f;
f.create("object.bin");
f.writeint(numodf); 
f.writeint(sizeof(objectdef)); 
f.write(odf,numodf*sizeof(objectdef)); 
f.close();
}
}


void ReadBackgroundNames(char *bnf)
{
FILE *f;
f=fopen(bnf,"rt");
if (!f) {printf("cannot open background definition file %s\n",bnf); exit(1);}

char line[100];
bgdf=(backgrounddef *)malloc(256*sizeof(backgrounddef));  //allocate memory for all odf
memset(bgdf,0,256*sizeof(backgrounddef));

numbgdf=0;

while (fgets(line,100,f)) //read line into memory
{
//   printf("%s\n",line);
  static  char name[32],sdf[32],dir[32],volfile[32];
  static  int onum;
  if (sscanf(line,"%d %s %s %s %s",&onum,name,sdf,dir,volfile)<5)
   break;//  {printf("error in object list\n%s\n",line);  exit(1);}

  //set up objectdef
  bgdf[onum].onum=onum; //ordinal number of type
  bgdf[onum].type=BGDEF;
  strcpy(bgdf[onum].name,name);
  strcpy(bgdf[onum].sdf,sdf);
  strcpy(bgdf[onum].dir,dir);
  strcpy(bgdf[onum].volfile,volfile);

  if (onum>numbgdf) numbgdf=onum;
} 
numbgdf++;

bgdf[0].onum=0;
bgdf[0].loaded=0;
strcpy(bgdf[0].name,"NULL");
bgdf[0].sdf[0]=bgdf[0].dir[0]=0;


//resize odf
bgdf=(backgrounddef *)realloc(bgdf,numbgdf*sizeof(backgrounddef)); 
fclose(f);

{
FILEIO f;
f.create("bg.bin");
f.writeint(numbgdf); 
f.writeint(sizeof(backgrounddef)); 
f.write(bgdf,numbgdf*sizeof(backgrounddef)); 
f.close(); 
}
}





int ReadControlFuncs(char *cfile,char cfnames[256][32])
{
FILE *f;
f=fopen(cfile,"rt");
if (!f) {printf("cannot open control function definition file %s\n",cfile); exit(1);}

char line[100];
do
{
 fscanf(f,"%s",line); //scan each funcname
} while (line[0]!='{'); //scan till first {


for (int i=0; ; i++)
 {
  fscanf(f,"%s",line); //scan each funcname
  if (line[0]=='}') break; //done reading cfuncs
  *strchr(line,',')=0;
  strcpy(cfnames[i],strchr(line,':')+2);
//  printf("%d: %s\n",i,cfnames[i]);
 }

//getch(); 

fclose(f);
return i;
}





int ReadRawFile(char **m,int h)
{
unsigned bytes;    
char header[4];
_dos_read(h,header,4,&bytes);
int x=(int)header[1];
int y=(int)header[2];

int size=y*2+filelength(h);

char *t=(char *)malloc(size);
if (!t)
{
 strcpy(errstr,"out of memory readrawfile\n");
 cleanexit(1);
}

for (int i=0;i<4;i++) t[i]=header[i];
short *ylist=(short *) (t+4);
char *img=t+4+ y*2;

_dos_read(h,img,size-y*2-4,&bytes);
for (i=0; i<y; i++)
{
ylist[i]=(short )(img-t);

int xl=x;
while (xl>0)
 {
  if ( (*img)&0x80) {xl-=(*img)&0x7f; img++;}
         else            {xl-=*img; img+=*img; img++; }
 }
}
*m=t;
return(size);
}


int memused;


//read list of image files
int ReadImageList(char *list,img ***imagedefs,defname **imgnames)
{

 //allocate memory for all of idf
 int numidf=0;
 img **idf=(img **)malloc(256*4);
 if (!idf) exit(1);
 memset(idf,0,256*4);

 //allocate memory for image names
 defname *in=(defname *)malloc(256*sizeof(defname));
 memset(in,0,256*sizeof(defname));


 //open image definition file
 FILE *f=fopen(list,"rt");
 
 if (f)  //read the idf file (if it exists)
 {
  char line[50];
  fgets(line,50,f); //get maximum number of images
  sscanf(line,"%d",&numidf);

  for (int i=0; i<numidf; i++) //read all images
   if (fgets(line,50,f)) //read line into memory
   {
    int num; char name[12];
    if (sscanf(line,"%d %s",&num,name)==2)
     {
      strcpy(in[num],name); //copy name
      if (num>numidf) numidf=num;
     } 
    } else {numidf=i; break;}
   fclose(f); 
  }


 //get lots of imagedefs
 int h;
 find_t   ft;
 int done=_dos_findfirst("*.raw",0,&ft);
 while (!done)
 {
  //get name of file without extension
  char fname[20]; 
  for (int j=0; ft.name[j] && ft.name[j]!='.' && j<8; j++) fname[j]=ft.name[j];
  fname[j]=0; 

  for (j=0; j<numidf; j++) //search list
   if (!stricmp(in[j],fname)) break; //see if we can find it...

  if (j==numidf) //was not found in list
    {
     strcpy(in[j],fname); //make new id
     numidf++;  //added new file
     if (numidf==255) {sprintf(errstr,"too many images\n"); cleanexit(-1);}
    } 
     
  //Read file
  _dos_open(ft.name,O_BINARY | O_RDONLY,&h);
  memused+=ReadRawFile((char **)&idf[j],h);   //read image file
  _dos_close(h);

  //draw it
  DrawImage(idf[j],(char *)0xA0000,0,0,0);

  done=_dos_findnext(&ft);
 }

 //write files to list
 f=fopen(list,"wt");
 fprintf(f,"%d\n",numidf); //number
 for (int i=0; i<numidf; i++)
  if (in[i][0]) fprintf(f,"%d %s\n",i,in[i]);
 fclose(f);

 //return
 *imagedefs=(img **)realloc((char *)idf,numidf*4);;   //store pointer to image defs
 *imgnames=(defname *)realloc((char *)in,numidf*sizeof(defname));;   //store pointer to image names

 return(numidf);  //return idf
}    


//read list of Sound files
int ReadSoundList(char *list,SOUND ***sounddefs,defname **names)
{

 //allocate memory for all of sounds
 int numsounds=0;
 SOUND **s=(SOUND **)malloc(32*4);
 if (!s) exit(1);
 memset(s,0,32*4);

 //allocate memory for names
 defname *n=(defname *)malloc(32*sizeof(defname));
 memset(n,0,32*sizeof(defname));


 //open image definition file
 FILE *f;
 f=fopen(list,"rt");
 
 if (f)  //read the sound file (if it exists)
 {
  char line[50];
  fgets(line,50,f); //get maximum number of images
  sscanf(line,"%d",&numsounds);

  for (int i=0; i<numsounds; i++) //read all images
   if (fgets(line,50,f)) //read line into memory
   {
    int num; char name[12];
    if (sscanf(line,"%d %s",&num,name)==2)
     {
      strcpy(n[num],name); //copy name
      if (num>numsounds) numsounds=num;
     } 
    } else {numsounds=i; break;}
   fclose(f); 
  }


 //get lots of sounddefs
 find_t   ft;
 int done=_dos_findfirst("*.wav",0,&ft);
 while (!done)
 {
  //get name of file without extension
  char fname[20]; 
  for (int j=0; ft.name[j] && ft.name[j]!='.' && j<8; j++) fname[j]=ft.name[j];
  fname[j]=0; 

  for (j=0; j<numsounds; j++) //search list
   if (!stricmp(n[j],fname)) break; //see if we can find it...

  if (j==numsounds) //was not found in list
    {
     strcpy(n[j],fname); //make new id
     numsounds++;  //added new file
     if (numsounds==31) {sprintf(errstr,"too many sounds\n"); cleanexit(-1);}
    } 
     
  //Read file
  s[j]=ReadWavFile(ft.name);
  memused+=s[j]->soundsize;

  done=_dos_findnext(&ft);
 }

 //write files to list
 f=fopen(list,"wt");
 fprintf(f,"%d\n",numsounds); //number
 for (int i=0; i<numsounds; i++)
  if (s[i]) fprintf(f,"%d %s\n",i,n[i]);
 fclose(f);

 //return
 *sounddefs=(SOUND **)realloc((char *)s,numsounds*4);   //store pointer to image defs
 *names=(defname *)realloc(n,numsounds*sizeof(defname));   //store pointer to image defs

 return(numsounds);  //return idf
}    




SCREEN *ReadScreen(char *name)
{
FILEIO f;
f.open(name);
void *a=f.readalloc(64000);
f.close();
return (SCREEN *)a;
}    

//read list of Screen files
int ReadScreenList(char *list, SCREEN ***scr,defname **names)
{

 //allocate memory for all of screens
 int numscr=0;
 SCREEN **s=(SCREEN **)malloc(256*4);
 if (!s) exit(1);
 memset(s,0,256*4);

 //allocate memory for names
 defname *n=(defname *)malloc(256*sizeof(defname));
 memset(n,0,256*sizeof(defname));


 //open image definition file
 FILE *f;
 f=fopen(list,"rt");
 
 if (f)  //read the sound file (if it exists)
 {
  char line[50];
  fgets(line,50,f); //get maximum number of images
  sscanf(line,"%d",&numscr);

  for (int i=0; i<numscr; i++) //read all images
   if (fgets(line,50,f)) //read line into memory
   {
    int num; char name[12];
    if (sscanf(line,"%d %s",&num,name)==2)
     {
      strcpy(n[num],name); //copy name
      if (num>numscr) numscr=num;
     } 
    } else {numscr=i; break;}
   fclose(f); 
  }


 //get lots of sounddefs
 find_t   ft;
 int done=_dos_findfirst("*.scr",0,&ft);
 while (!done)
 {
  //get name of file without extension
  char fname[20]; 
  for (int j=0; ft.name[j] && ft.name[j]!='.' && j<8; j++) fname[j]=ft.name[j];
  fname[j]=0; 

  for (j=0; j<numscr; j++) //search list
   if (!stricmp(n[j],fname)) break; //see if we can find it...

  if (j==numscr) //was not found in list
    {
     strcpy(n[j],fname); //make new id
     numscr++;  //added new file
     if (numscr==255) {sprintf(errstr,"too many screens\n"); cleanexit(-1);}
    } 
     
  //Read file
  s[j]=ReadScreen(ft.name);
  memused+=64000;

  done=_dos_findnext(&ft);
 }

 //write files to list
 f=fopen(list,"wt");
 fprintf(f,"%d\n",numscr); //number
 for (int i=0; i<numscr; i++)
  if (s[i]) fprintf(f,"%d %s\n",i,n[i]);
 fclose(f);

 //return
 *scr=(SCREEN **)realloc((char *)s,numscr*4);   //store pointer to image defs
 *names=(defname *)realloc(n,numscr*sizeof(defname));   //store pointer to image defs

 return(numscr);  //return idf
}    



//save series list to file
void objectdef::SaveSeriesList()
{
 FILEIO f;
 f.create("series.lst");

 f.writeint(0x6969); //write sig

 f.writeint(nums); //number of series
 int seriessize=sizeof(series);
 f.writeint(seriessize);
 
 f.write(sd,nums*sizeof(series)); //write all serieses

 //write all the frames for each series
 for (int i=0; i<nums; i++)
  f.write(sd[i].first,sd[i].size);

 mprintf("%s saved: %d series %d bytes",name,nums,f.size());
 f.close();

 dirty=0;
}    

int objectdef::LoadSeriesList()
{
 unsigned bytes;
 int h;

//ModeText();
//--------------------------------
//read standard series definition
//------------------------------
 FILE *f=fopen(sdf,"rt");
 if (!f) {mprintf("cannot open series definition file %s",sdf);  return -1;}
      
 char line[50];
 nums=-1;
 sd=(series *)malloc(256*sizeof(series));  //allocate memory for all seriess
 memset(sd,0,256*sizeof(series)); //clear series
 while (fgets(line,50,f))
  {
   int i; char n[50],d[50];
   if (sscanf(line,"%s %s %d",d,n,&i)==3 && !stricmp(d,"#define"))
    {
     strcpy(sd[i].name,n);
     if (i>nums) nums=i;
    }

  }
nums++;
fclose(f);

//-----------------------------------------
//read saved series definitions for the object
//--------------------------------------

int tread=0;
   int tnums;
  //read existing list
if (!_dos_open("series.lst",O_BINARY | O_RDONLY,&h))
 {  
     //read signiture
   unsigned int sig;     
   series *t;
   int seriessize;

   _dos_read(h,&sig,4,&bytes);
   if (sig!=0x6969) //if sig not present, old series file
     {
      tnums=sig; //assume tnums=sig then
      seriessize=41; //old series size
     } else //sig present
     {
      _dos_read(h,&tnums,4,&bytes);
      _dos_read(h,&seriessize,4,&bytes);      
     }

   t=(series *)malloc(tnums*sizeof(series));
   for (int a=0; a<tnums; a++) //read all series defs
     _dos_read(h,&t[a],seriessize,&bytes); //read all of stored series definitions
    
     int i,j;

   for (i=0; i<tnums; i++)
    if (!strlen(t[i].name)) break;
   tnums=i;
    
   //go through each saved series
   for (i=0; i<tnums; i++)
   {
    t[i].cf=0;
    //read the frames of the series into memory
    if (t[i].size)
      {
       t[i].first=(frame *)malloc(t[i].size);     //get memory
       _dos_read(h,t[i].first,t[i].size,&bytes);
       tread++;
      }
    
    //find the series character name in the SDF
    for (j=0; j<nums; j++)
         if (!stricmp(t[i].name,sd[j].name)) break; //find series

    //we found a matching series name
    if (j<nums)
      { sd[j]=t[i]; }//copy series to final list that has the same name
     else
    if (i<nums) //it was a part of the standard list
    {
    if (!sd[i].first)
     {
      if (!t[i].size) strcpy(t[i].name,sd[i].name);
      sd[i]=t[i]; //if cooresponding empty numeral series exists...copy it over
     }
     else
    if (t[i].first) free(t[i].first); //if no numeral or character cooresponding one exists, free it
    } else //it was added afterwards
    {
      sd[i]=t[i]; //if cooresponding empty numeral series exists...copy it over
    }
   }
   
   free(t);
   _dos_close(h);
 if (nums<tnums) nums=tnums;
  }


sd=(series *)realloc(sd,nums*sizeof(series));

mprintf("sdfnums%d savednums %d",nums,tnums);

return tread; 
}    




void objectdef::SaveSDF()
{
 FILE *f=fopen(sdf,"wt");

 for (int i=0; i<nums; i++)
   fprintf(f,"#define %s %d\n",sd[i].name,i);
 fclose(f);
}          
