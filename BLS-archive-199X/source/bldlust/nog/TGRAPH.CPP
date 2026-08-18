#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>
#include <ctype.h>

#include "tgraph.h"
#include "tinput.h"

int DrawLetter(char *font,char *dest,unsigned char c,int LSW);

extern "C"
{
void __cdecl putimageflip256();
}

unsigned int CompressImage256(char *src, char *dest)
{
unsigned int xlen,ylen,xt,x,y,a,b,i;
char    tcolor;

xlen=(unsigned int) ((unsigned char)src[1]);
ylen=(unsigned int) ((unsigned char)src[2]);
tcolor=src[3];

for (a=0, b=0; a<4; a++, b++) dest[b]=src[a]; // Copy header data

short *ylist=(short *)(dest+b);

a+=ylen*2; b+=ylen*2;

for(y=0; y<ylen; y++)  /* Yloop */
{
 *ylist=b; ylist++;

 xt=xlen;       /* Xloop*/
 do
 {
 if (xt<0x7F) x=xt; else x=0x7F;
 xt-=x;

  do
  {
  if(src[a]==tcolor)    /* Transparent Loop */
         {
          for (i=0; src[a]==tcolor && x>0; a++, i++, x--);
          i |= 0x80;             // Set Transparent flag in run
          dest[b++]=(char) i; // Store data
         }
        else
         {                   /* Data Loop */
          for (i=1; src[a]!=tcolor && x>0; x--, i++) dest[b+i]=src[a++];
          dest[b]=i-1;
          b+=i;
         }
  } while(x);

 } while(xt>0);

}


return(b+1);
}


int DrawString(char *font,char *dest,unsigned char *s,int LSW)
{
int i;
char *d2;

d2=dest;

for(i=0; s[i]; i++)
  d2+=DrawLetter(font,d2,toupper(s[i]),LSW)-1;

return(d2-dest);
}

int DrawStringSP(char *font,char *dest,unsigned char *s,int LSW)
{
int i;
char *d2;

d2=dest;

for(i=0; s[i]; i++)
  d2+=DrawLetter(font,d2,s[i],LSW);

return(d2-dest);
}



int DrawLetter(char *font,char *dest,unsigned char c,int LSW)
{
unsigned int index;

index=((unsigned short *)font)[c];
if (index)
  {
        PutImageFlip256(font+index,dest,LSW,0);
        return((int) ((unsigned char)font[index+1]));
  }
return(6);
}

int GetStringWidth(char *font,unsigned char *s)
{
int i,width;
int index;
for (i=width=0; s[i]; i++)
        {
         index=((unsigned short *)font)[(s[i])];
         if(index)
                 width+=(int) ((unsigned char)font[index+1])-1;
         else width+=6;
        }
return(width);
}

int GetStringWidthSP(char *font,unsigned char *s)
{
int i,width;
int index;
for (i=width=0; s[i]; i++)
        {
         index=((unsigned short *)font)[(s[i])];
         if(index)   width+=(int) ((unsigned char)font[index+1]);
         else width+=6;
        }
return(width);
}
//F9F5
//ax=0 bx=66 cx=0

int GetLetterWidth(char *font, unsigned char s)
{
int width;
int index;

if (s>128 || s<30) return(10);
 index=((unsigned short *)font)[s];
 if(index)  width=(int) ((unsigned char)font[index+1]);
      else width=6;
return(width);
}    

int CreateFontImage256(char *font,char **img,unsigned char *s)
{
int width,height,imagesize;
int i,j;
int index;
//char *temp;

//find width
height=0;
for (i=width=0; s[i]; i++)
        {
         index=((short *)font)[(s[i])];
         if (index)
                {
                 width+=(int) ((unsigned char)font[index+1]);
                 j     =(int) ((unsigned char)font[index+2]); //Get y
                 if (j>height) height=j;
                } else width+=6;
         width-=1;
        }
//printf("%d",height);

width+=1;
height+=1;



imagesize=width*height+4+8 +height*2+50;

//*img=(char far *) farmalloc(imagesize);
//temp=(char far *) farmalloc(imagesize);

char *iptr=(char *)malloc(imagesize);
char *tptr=(char *)malloc(imagesize);


for (i=0; i<imagesize; i++) tptr[i]=0xFF;

tptr[0]=16;
tptr[1]=(char)width; //temp[2]=14;
tptr[2]=height;
tptr[3]=0xFF;


DrawString(font,tptr+4+height*2,s,width);

imagesize=CompressImage256(tptr,iptr);
free(tptr);
*img=iptr;
//(char *)farrealloc(iptr,imagesize);

return(imagesize);
}



