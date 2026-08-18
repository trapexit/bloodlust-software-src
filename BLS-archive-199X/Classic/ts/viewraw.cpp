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
#include <graph.h>


#include "tgraph.h"


int ReadRawFile(char **m,int h)
{
unsigned bytes;
char header[4];
_dos_read(h,header,4,&bytes);
int x=(int)header[1];
int y=(int)header[2];

int size=y*2+filelength(h);

char *t=(char *)malloc(size);
for (int i=0;i<4;i++) t[i]=header[i];
short *ylist=(short *) (t+4);
char *img=t+4+ y*2;

_dos_read(h,img,64000,&bytes);


for (i=0; i<y; i++)
{
ylist[i]=img-t;

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

color pal[256];

void main(int argc, char *argv[])
{
if (argc<2) return;

char *t;
unsigned bytes;
int h;
char name[30];
strcpy(name,argv[1]);
for (int j=0; name[j] && name[j]!='.'; j++);
if (!name[j]) strcat(name,".raw");
if (_dos_open(name,O_BINARY | O_RDONLY,&h)) return;


ReadRawFile(&t,h);
_dos_close(h);

_dos_open("c:\\misc\\cursor.bbm",O_RDONLY | O_BINARY,&h);
lseek(h,0x30,SEEK_SET);
_dos_read(h,pal,256*3,&bytes);
_dos_close(h);
for (int i=0; i<256; i++)
 { pal[i].r>>=2;   pal[i].g>>=2;   pal[i].b>>=2; }



Mode256();
LoadPalette(pal,0,256);
DrawImage(t,(char *) 0xA0000,0,0,0);
getch();
ModeText();


}
