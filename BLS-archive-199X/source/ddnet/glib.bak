#include "glib.h"
#include "stdio.h"

extern char *screen;


int GetLetterWidth(char *font, char s)
{
int width;
int index;

 index=((unsigned short *)font)[s];
 if(index)  width=( (img *) (((char *)font)+index) )->xw;
      else width=6;
return(width);
}    



int DrawLetter(FONT *font,char *dest,char c,int x,int y)
{
if (c<30) return(0);

register unsigned int index=font->idx[(unsigned char)c];
if (index)
  {
   DrawImage((img *) (((char *)font)+index),dest,x,y,0);
   return(  ((img *) (((char *)font)+index))->xw);
  }
return(6);
}



int GetStringWidth(FONT *font,char *s)
{
int index;
for (int i=0,width=0; s[i]; i++)
        {
         index=font->idx[(unsigned char)(s[i])];
         if(index) width+=( (img *) (((char *)font)+index) )->xw;
              else width+=6;
        }
return(width);
}


void DrawString(FONT *font,char *dest,char *s,int x,int y)
{
for(int i=0; s[i]; i++)
   x+=DrawLetter(font,dest,s[i],x,y);
}

void DrawCenteredString(FONT *font,char *dest,char *s,int x,int y)
{
x-=GetStringWidth(font,s)/2;    
for(int i=0; s[i]; i++)
   x+=DrawLetter(font,dest,s[i],x,y);
}


//printf w/font to char *screen
void dprintf(FONT *font,int x,int y,char *format,...)
{
 static char s[256];
 char *arg=((char *)&format)+sizeof(format);
 vsprintf(s,format,arg);
 DrawString(font,screen,s,x,y);
}    





void FuckWithImage(char *s,unsigned char c1,unsigned char c2)
{
int wx,wy; //width and height of image
//int k;
wx=(int)(unsigned char)s[1];
wy=(int)(unsigned char)s[2];

s+=4+wy+wy; ///s points to image

for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F;  i-=a; }
  else         //opaque
         {
          for (int b=a; b>0; b--)
                 {
                  if (*s==c1) *s=c2;
                  s++;
                 }
                i-=a;
         }
 }
}
}


void FunkyFont(FONT *f,unsigned char a,unsigned char b)
{

for (int c=0; c<128; c++)
{
 unsigned int index=f->idx[(unsigned char)(c)];
 if (index)  FuckWithImage((((char *)f)+index),a,b);
}

}











