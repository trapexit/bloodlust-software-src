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



int findimagesize(char *img)
{
char *s=img;
int wx=(int)(unsigned char)s[1];
int wy=(int)(unsigned char)s[2];
printf("%3dxw %3dyw ",wx,wy);

s+=4; ///s points to image

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F;  i-=a; }
  else         //opaque
         {
            s+=a; 
            i-=a;
         }
 }
}

return ((int)s- (int)img);
}




void main(int argc, char *argv[])
{
if (argc<2) return;

unsigned bytes;
int h;
if (_dos_open(argv[1],O_BINARY | O_RDONLY,&h)) return;

unsigned size=filelength(h);
printf("opened file %s...\n",argv[1]);
printf("size: %d \n", size);
char *t=(char *)malloc(size+10);
_dos_read(h,t,size,&bytes);
_dos_close(h);


char path[30];
strcpy(path,argv[1]);
for (int j=0; path[j]!='.' && path[j]; j++);
path[j]=0;
printf("making directory %s\n",path);
mkdir(path);
chdir(path);

int imgs=0;
//read it all
for (int i=0; i<size; i++) //go through all file
 {
  if (t[i]==0x10 && t[i+3]==0xFF)
  {
   imgs++;
   printf("img%3d offset %5X:  ",imgs+1,i);
   unsigned imgsize=findimagesize(&t[i]);
   printf(" size %d\n",imgsize);

   char filename[30];
   sprintf(filename,"%d.raw",imgs+1);
   _dos_creat(filename,0,&h);
   _dos_write(h,t+i,imgsize,&bytes);
   _dos_close(h);
  }
 }     



printf("%d images found\n",imgs);


}    
