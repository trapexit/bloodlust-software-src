//Copyright(c) 1995 Bloodlust Software All rights reserved
#include <direct.h>
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <graph.h>

#include "cdrom.h"

#define NOCDCHECK

#define HI 0x5F

extern "C" {
void InitDosError();
void TerminateDosError();

}    


typedef struct character
{
 unsigned char c;
 unsigned char attr;
} character;

character *video=(character *)0xB8000;

#define SCRX 80
#define SCRY 25


unsigned bytes;
int h;
volatile int kbint;
volatile char kbscan;
volatile unsigned int uu;

int cdcheck=2;

character screen[SCRX*SCRY];

void DrawLA(unsigned char l,unsigned char a, int x,int y)
{
 screen[y*SCRX+x].c=l;
 screen[y*SCRX+x].attr=a; 
}    


void DrawLetter(unsigned char l, int x,int y)
{
 screen[y*SCRX+x].c=l;
}    

void SetAttribute(unsigned char l, int x,int y)
{
 screen[y*SCRX+x].attr=l;
}

void DrawAHLine(int x, int y,int l,unsigned char a)
{
 for (int i=x; l>0; i++,l--)
  SetAttribute(a,i,y);
}    

void DrawHLine(int x, int y, int l,unsigned char a)
{
 for (int i=x; l>0; i++,l--)
  DrawLA(0xCD,a,i,y);
}    

void DrawVLine(int x, int y, int l,unsigned char a)
{
 for (int i=y; l>0; i++,l--)
  DrawLA(0xD7,a,x,i);
}    


void DrawRect(int x, int y, int xl, int yl,unsigned char a)
{
DrawLA(0xC9,a,x,y); //ul
DrawHLine(x+1,y,xl-1,a);
DrawLA(0xBB,a,x+xl,y); //ur

DrawVLine(x,y+1,yl-1,a);
DrawVLine(x+xl,y+1,yl-1,a);

DrawLA(0xC8,a,x,y+yl); //dl
DrawHLine(x+1,y+yl,xl-1,a);
DrawLA(0xBC,a,x+xl,y+yl); //dr
}


void DrawBlock(int x, int y, int xl, int yl,unsigned char l, unsigned char a)
{
  character *s=&screen[y*SCRX+x];
 for (;yl>0; yl--)
 {
  for (int i=xl; i>0; i--,s++)
    {s->c=l; s->attr=a;}
  s+=SCRX-xl;
 }   
}               


void DrawBox(int x, int y, int xl, int yl,unsigned char a)
{
DrawBlock(x,y,xl,yl,0xFF,a);
DrawRect(x,y,xl,yl,a);
}    

character temp[SCRX*SCRY];

void OpenBox(int x,int y, int xl, int yl,unsigned char a)
{
memcpy(temp,screen,SCRX*SCRY*2);

x+=xl/2; y+=yl/2;
for (int i=0; i<=20; i++)
{
 int txl=xl*i/20;
 int tyl=yl*i/20;
 DrawBox(x-txl/2,y-tyl/2,txl,tyl,a);
 memcpy(video,screen,SCRX*SCRY*2);
 delay(15);
}
}

void DrawString(int x, int y, char *s)
{
while (*s) {DrawLetter(*s,x,y); x++; s++;}
}

void DrawCenteredString(int x, int y, char *s)
{
DrawString(x-strlen(s)/2, y,s);
}    


void Flash(int x,int y,int xl, unsigned char a1, unsigned char a2)
{
for (int i=0; i<3; i++)
 {
   delay(80);  
   DrawAHLine(x,y,xl,a1);
   memcpy(video,screen,SCRX*SCRY*sizeof(character));
   delay(80);
   DrawAHLine(x,y,xl,a2);
   memcpy(video,screen,SCRX*SCRY*sizeof(character));
 }  
}


unsigned int sourcedrive;
unsigned int numdrives;
unsigned int dspace[32]; //space of all drives
unsigned int dletter[32]; //letter of all drives (1-A, 0-LAST)
unsigned int dcdrom[32]; //is cdrom?
int dptr;
unsigned int numcddrives;

unsigned installdrive;
char installpath[100];

int Install(unsigned int);



unsigned olddrive;
char  oldpath[100];

void cleanexit(int x)
{
_dos_setdrive(olddrive,&numdrives);
chdir(oldpath);

_clearscreen(_GCLEARSCREEN);
_settextcursor(0x0607);
exit(x);
}

int vh;

unsigned smallinstall=0;

void main(int argc, char *argv[])
{
unsigned int i;
char key;

InitDosError();

argc=argc;
numcddrives=cdrom_installed(); //check cdrom installation
//printf("%d\n",numcddrives);
#ifndef NOCDCHECK
if (numcddrives<=0)
  {printf("ERROR initializing CD-ROM driver.\n");
   exit(1);
  }  
#endif

_clearscreen(_GCLEARSCREEN);

_settextcursor(0x2000);

for (i=0; i<SCRY*SCRX; i++)
 {screen[i].c=0; screen[i].attr=0;}
for (i=0; i<SCRX; i++)
 {
  DrawLA(  0xE0,0x40,i,13);
  DrawLA(  0xE0,0x40,i,12);
  memcpy(video,screen,SCRX*SCRY*sizeof(character));
  delay(4);
 }

for (i=0; i<12; i++)
 {
  for (int j=0; j<SCRY*SCRX; j++)
   {screen[j].c=0; screen[j].attr=0;}
 
  for (j=0; j<SCRX; j++)
   {
     DrawLA(  0xE0,0x40,j,11-i);
     DrawLA(  0xE0,0x40,j,14+i);
   }
  memcpy(video,screen,SCRX*SCRY*sizeof(character));
  delay(20);
 }      


for (i=0; i<SCRY*SCRX; i++)
 {screen[i].c=0; screen[i].attr=0;}
 
for (i=0; i<SCRX; i++)
 {
  screen[i].c=0xE0;
  screen[i].attr=0x40;
  screen[i+SCRX*24].c=0xE0;
  screen[i+SCRX*24].attr=0x40;
 } 

  
DrawBlock(0,1,80,1,0xFF,0x4F);
DrawString(22,1,"TimeSlaughter Installer Version 1.01");
DrawAHLine(0,23,80, HI);  //bottom purple line


character *oldbg=(character *)malloc(SCRX*SCRY*2);
memcpy(oldbg,screen,SCRX*SCRY*sizeof(character));    


OpenBox(10,4,60,2,0x4f);
DrawCenteredString(40,5,"Searching for available hard drive space...");
memcpy(video,screen,SCRX*SCRY*sizeof(character));
sourcedrive=argv[0][0]-'A'+1;
_dos_getdrive((unsigned *)&olddrive);
getcwd(oldpath,100);
_dos_setdrive(olddrive,&numdrives);

//_clearscreen(_GCLEARSCREEN);
diskfree_t dtable;
for (i=3,dptr=0; i<numdrives; i++) //go from C: to lastdrive:
{
 dcdrom[dptr]=0; //assume not cdrom
 for (int j=0; j<numcddrives; j++)  //see if it is a CDROM
   if (cddrives[j]==i) dcdrom[dptr]=1;
/* if (dcdrom[dptr]) //if this IS a cdrom
  {
   cdsetdrive(i-1); //go to that cd drive
   cd_get_audio_info();
  }  else cdrom_data.error=0;*/
 if (/*!(cdrom_data.error&0x8000) &&*/ !_dos_getdiskfree(i,&dtable))
 {
  dletter[dptr]=i;
  dspace[dptr]=dtable.avail_clusters*dtable.bytes_per_sector*dtable.sectors_per_cluster;
  dptr++;    
 }
} 
dletter[dptr]=0; //last drive
//getch();

//see if source drive === cdrom drive
for (i=0; i<dptr; i++)
 if (dletter[i]==sourcedrive) //find source dptr
 {
  #ifndef NOCDCHECK 
  if (!dcdrom[i] || dspace[i]!=0) //if not cdrom, uh oh
   {
     cdcheck++;
     OpenBox(12,10,54,2,0x4f);
     DrawCenteredString(40,11,"ERROR: Source drive is not CD-ROM.");
     memcpy(video,screen,SCRX*SCRY*sizeof(character));
     getch();
     cleanexit(1);
   } else cdcheck--;
  #else
  cdcheck--; 
  #endif 
  _dos_setdrive(sourcedrive,&numdrives); //go to source drive
  find_t ff; //search thing
  if (_dos_findfirst("\\*.*",_A_VOLID,&ff) || stricmp(ff.name,"TS") || _dos_open("\\install.cow",0,&vh)) //find all labels
   {
     cdcheck++;
     OpenBox(12,10,54,2,0x4f);
     DrawCenteredString(40,11,"ERROR: Incorrect CD in drive.");
     memcpy(video,screen,SCRX*SCRY*sizeof(character));
     getch();
     cleanexit(1);
    } else cdcheck--;
  break;  
 }



if (cdcheck) cleanexit(1); 

int highlight=0;
fuckshitfuck:

//------------
//Ask for installation now
DrawBox(10,3,60,4,0x4f);
DrawCenteredString(40,4,"Select the hard drive to install to.");
DrawCenteredString(40,5,"The installation requires approximately 25MB of HD space.");
sprintf(installpath,"Source CD drive: %c",sourcedrive+'A'-1);
DrawCenteredString(40,6,installpath);



OpenBox(15,9,50,2+dptr,0x4f);

for (i=0; i<dptr; i++)
 {
  char s[50];
  if (!dcdrom[i]) sprintf(s,"Drive: %c   %6u.%02uMB free",dletter[i]+'A'-1,dspace[i]/1048576,(dspace[i]%1048576)*100/1048576);
   else sprintf(s,"Drive: %c             CD-ROM",dletter[i]+'A'-1);
  DrawCenteredString(40,10+i,s);
 }
DrawCenteredString(40,10+i,"Exit Without Installing"); 

memcpy(video,screen,SCRX*SCRY*sizeof(character));


int done=0;
character *bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));    



do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));


DrawAHLine(16,10+highlight,49,HI);

char s[50];
if (highlight<dptr)
 if (!dcdrom[highlight]) sprintf(s,"Install TimeSlaughter to drive %c:",dletter[highlight]+'A'-1);
   else strcpy(s,"");
else strcpy(s,"Exit without installing TimeSlaughter.");
 
DrawCenteredString(40,23,s);
  
memcpy(video,screen,SCRX*SCRY*sizeof(character));

key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<dptr) highlight++;
  if (key==27) done=1;

 }  else
 {
  if (key==27) cleanexit(0);
  if (key==13 || key==' ') //enter
   {
    if (!dcdrom[highlight]) Flash(16,10+highlight,49,0x4f,HI);
    if (highlight<dptr)
       {
        if (!dcdrom[highlight])
         {installdrive=dletter[highlight]; done=1; }
       }  else cleanexit(0); 
   } 
 }         

} while (!done);
free(bg);

memcpy(screen,oldbg,SCRX*SCRY*sizeof(character));


//full or not
//------------
//Ask for installation now
DrawBox(10,3,60,5,0x4f);
DrawCenteredString(40,4,"Select the type of install.");
DrawCenteredString(40,6,"Full install (requires 31MB)");
DrawCenteredString(40,7,"Small install (requires 19MB)");

memcpy(video,screen,SCRX*SCRY*sizeof(character));
done=0;
bg=(character *)malloc(SCRX*SCRY*2);
memcpy(bg,screen,SCRX*SCRY*sizeof(character));
highlight=0;   
do
{
memcpy(screen,bg,SCRX*SCRY*sizeof(character));


DrawAHLine(16,6+highlight,49,HI);

if (!highlight) 
DrawCenteredString(40,23,"Install everything including all cinematics.");
 else
DrawCenteredString(40,23,"Leave cinematics on CD-ROM, requires CD-ROM to be inserted during game."); 
  
memcpy(video,screen,SCRX*SCRY*sizeof(character));

key=toupper(getch());
if (!key)
 {
  key=getch();
  if (key==72 && highlight>0) highlight--;
  if (key==80 && highlight<1) highlight++;
 }  else
 {
  if (key==27) done=-1;
  if (key==13 || key==' ') //enter
   {
    Flash(16,6+highlight,49,0x4f,HI);
    done=highlight+1;
   } 
 }         

} while (!done);
free(bg);

memcpy(screen,oldbg,SCRX*SCRY*sizeof(character));
if (done==-1) goto fuckshitfuck;
smallinstall=done-1;


//ask for directory
OpenBox(5,10,70,2,0x4f);


done=0; //get path
DrawCenteredString(40,23,"Type in the directory to install to and press ENTER.");
sprintf(installpath,"%c:\\TS",installdrive+'A'-1);

DrawAHLine(10,11,60, 0x1F);  //bottom purple line

_settextcursor(0x0007);
int len=strlen(installpath);
do
{

for (int i=10+11*80; i<75+11*80; i++)
  screen[i].c=0;
DrawString(7,11,installpath);
_settextposition(12,8+len);

memcpy(video,screen,SCRX*SCRY*sizeof(character));
key=getch();
if (!key) {key=getch();}
 else
 {
  key=toupper(key);
  if (key==8) //backspace
   if (len>3)  {installpath[len-1]=0; len--;}
  if (isprint(key) && !isspace(key) && len<65)
    {installpath[len]=key; len++; installpath[len]=0;}
  if (key==27) done=1;
  if (key==13)
   {
    if (chdir(installpath))
     {if (!mkdir(installpath)) done=1;}
    else done=1; //already exists 
   }
 }

}while(!done);
_settextcursor(0x2000);
memcpy(screen,oldbg,SCRX*SCRY*sizeof(character));
if (key==27) goto fuckshitfuck;


//free(oldbg);

int result=Install(installdrive);

//reset screen
_clearscreen(_GCLEARSCREEN);
_settextcursor(0x0607);

if (result==-2)
 {printf("ERROR: Unable to write to disk. Make sure you have enough disk space.\n"); exit(1);}
if (result==-1)
 { 
  printf("User Terminated Install.\n");
  exit(1);
 }
if (result==-3)
 {printf("ERROR: Corrupted data file!\n"); exit(2);}
if (result==-4)
 {printf("ERROR: Invalid directory name.\n"); exit(1);} 
if (result==-5)
 {printf("ERROR: Out of memory.\n"); exit(1);}
_dos_close(vh); 


unlink("time.cfg");
spawnl(P_WAIT,"setup.exe",NULL);
}



int Install(unsigned letter)
{
//#ifndef NOCDCHECK
//if (dspace[letter]) return(-3);
//#endif

if (cdcheck) return(-3);
char s[50];
DrawBox(5,4,70,2,0x4f);
sprintf(s,"Installing to %s",installpath);
DrawCenteredString(40,5,s);

OpenBox(0,16,79,3,0x4f);
DrawAHLine(1,18,78, 0x0F);  //bottom purple line


//default dir is destination
_dos_setdrive(letter,&numdrives);
if (chdir(installpath)) return(-4);


unsigned vsize=filelength(vh);

unsigned size;

char fkey[20];
char *t;

int h;
char fname[20];
fname[12]=0;

//t=(char *)malloc(1700000);

//return(0);
do
{
if (_dos_read(vh,fkey,16,&bytes)) return(-3); //corrupt
if (bytes<16) break;
if (strcmp(fkey,"MIDGETPOWER")) return(-3);

_dos_read(vh,fname,12,&bytes);
for (int i=0; fname[i] && i<12; i++) s[i]=fname[i];
for (; i<13; i++) s[i]=' '; s[i]=0;
DrawString(2,17,s);
int barlen=lseek(vh,0,SEEK_CUR)*78/vsize;
DrawAHLine(1,18,barlen, 0x1F);  //bottom purple line
memcpy(video,screen,SCRX*SCRY*sizeof(character));

_dos_read(vh,&size,4,&bytes); //get size

int docopy;
docopy=1;

if (smallinstall) //if small install, check file name
 {
  if (toupper(fname[0])=='C' && toupper(fname[1])=='I' && toupper(fname[2])=='N') //skip
    docopy=0; //dont copy this one
  if (toupper(fname[0])=='P' && toupper(fname[1])=='R' && toupper(fname[2])=='O') //skip
    docopy=0; //dont copy this one

  if (!stricmp(fname,"intro.vol")) docopy=0;
 }

if (docopy)
{
 t=(char *)malloc(size);
 if (!t) return(-5);
 _dos_read(vh,t,size,&bytes);

 if (_dos_creat(fname,0,&h)) return(-2); //unable to write
 _dos_write(h,t,size,&bytes);
 if (bytes!=size) return(-2);
 _dos_close(h);
 free(t);
} else
{
 lseek(vh,size,SEEK_CUR);
}     


if (kbhit())
 {
  char key=getch();
  if (key==27) return(-1); //user aborted
 }

} while(1);


DrawAHLine(1,18,78, 0x1F);  //bottom purple line
return(0); //success


}


    
