#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>


#include "file.h"

typedef unsigned char byte;
typedef unsigned short word;

//dos graphics
void mode13h();
#pragma  aux mode13h =  \
  "mov eax,13h"            \
  "int 10h"               \
  parm   []               \
  modify [eax];

void modetext();
#pragma  aux modetext =  \
  "mov eax,3h"            \
  "int 10h"               \
  parm   []               \
  modify [eax];


char *video=(char *)0xA0000;



#define XW 8
#define YW 8

#define ROMTILESIZE 8
#define COLOR 4

struct tile
{
 char b[YW][XW];

 void draw(char *dest,int x,int y)
 {
  dest+=x+y*320;
  for (y=0; y<YW; y++,dest+=320)
   for (x=0; x<XW; x++) dest[x]=b[y][x];
 }

 void convert(byte *t)
 {
  memset(b,0,XW*YW);
  for (int y=0; y<YW; y++)
  {
   for (int x=0; x<XW; x++)
   {
    byte d=0;
    byte z=0x80>>(x&7);

    if (t[0x000000]&z) d|=1;
    if (t[0x080000]&z) d|=2;
    if (t[0x100000]&z) d|=4;
    if (t[0x180000]&z) d|=8;

	t+=2;

    b[y][x]=d;
   }
  }
 }

};



//word *vrom;
byte *vrom;
void readvrom(char *name,byte *t)
{
 FILEIO f;
 if (f.open(name)) exit(1);
 int size=f.size();
 if (size>0x80000) size=0x80000;
 f.read(t,size);
 f.close();
 return;
}




#define MAXTILES 8192
tile t[MAXTILES];
void maketiles()
{
 for (int i=0; i<MAXTILES; i++)
    t[i].convert(vrom+i*ROMTILESIZE+0x000000);

}





struct color {char r,g,b;};
color pal[256];
void loadpal(color *c,int start,int num)
{
 outp(0x3C8,0);
 for (int i=0; i<num; i++)
 {
  outp(0x3C9,c[i].r>>2);
  outp(0x3C9,c[i].g>>2);
  outp(0x3C9,c[i].b>>2);
 }
}
void setpalette(int p)
{
 for (int i=0; i<(1<<COLOR); i++)
  {
   int j=i;
   if (p) j^=(1<<COLOR)-1;
   pal[i].r=j<<(8-COLOR);
   pal[i].g=j<<(8-COLOR);
   pal[i].b=j<<(8-COLOR);
  }
 loadpal(pal,0,256);
}








void main(int argc,char *arg[])
{
 mode13h();

 vrom=(byte *)malloc(0x200000);
 memset(vrom,0,0x200000);



 readvrom("tkrom.1",vrom+0x180000);
 
 /*readvrom("tkgrom.2",vrom+0x080000);
 readvrom("tkgrom.3",vrom+0x100000);
 readvrom("tkgrom.4",vrom+0x180000);
*/


 setpalette(0);
 memset(video,0,64000);

 vrom+=0x000;


 maketiles();

 int done=0;
 int base=0; //(320*200)/(YW*XW)*16; //0;
 do
 {
 int i=base;
 int x,y;
 for (y=0; (y+YW)<200; y+=YW)
  for (x=0; (x+XW)<320; x+=XW,i++)
   t[i&(MAXTILES-1)].draw(video,x,y);

  switch(getch())
  {
   case 'q': done=1; break;
   case 'z': base-=(320*200)/(YW*XW); break;
   case 'x': base+=(320*200)/(YW*XW); break;
   case 'a': base--; break;
   case 's': base++; break;
   case 'b': vrom++; maketiles(); break;
   case 'p':
   {
    static pal=0;
    pal++; pal&=3;
    setpalette(pal);
   }
   break;
  };

 } while (!done);

 modetext();

 printf("base: %X", base);
}








 //clear screen set pallete
// for (int i=0; i<4; i++)
//  {pal[i].r=i<<6; pal[i].g=i<<6; pal[i].b=i<<6;}
//  {pal[i].r=0<<4; pal[i].g=0<<4; pal[i].b=(i<<4)^15;}

// for (int i=0; i<256; i++)
//  {pal[i].r=i; pal[i].g=i; pal[i].b=i;}
/*
 void convert(word *t,word *u)
 {
  memset(b,0,64);
  for (int y=0; y<8; y++)
 {

   b[y][0]=(*t>>12)&0xF;
   b[y][1]=(*t>> 8)&0xF;
   b[y][2]=(*u>>12)&0xF;
   b[y][3]=(*u>> 8)&0xF;
   b[y][4]=(*t>> 4)&0xF;
   b[y][5]=(*t>> 0)&0xF;
   b[y][6]=(*u>> 4)&0xF;
   b[y][7]=(*u>> 0)&0xF;

   b[y][0]=(*t>>12)&0xF;
   b[y][1]=(*u>>12)&0xF;
   b[y][2]=(*t>> 8)&0xF;
   b[y][3]=(*u>> 8)&0xF;
   b[y][4]=(*t>> 4)&0xF;
   b[y][5]=(*u>> 4)&0xF;
   b[y][6]=(*t>> 0)&0xF;
   b[y][7]=(*u>> 0)&0xF;

   t+=1; u+=1;
  }
 }
*/


/*
 void convert(byte *t)
 {
  memset(b,0,64);
  for (int y=0; y<8; y++)
 {
   b[y][0]=(*t>>4)&0xF;
   b[y][1]=(*t>>0)&0xF; t++;
   b[y][2]=(*t>>4)&0xF;
   b[y][3]=(*t>>0)&0xF; t++;
   b[y][4]=(*t>>4)&0xF;
   b[y][5]=(*t>>0)&0xF; t++;
   b[y][6]=(*t>>4)&0xF;
   b[y][7]=(*t>>0)&0xF; t++;
   t+=8;
  }
 }      */

/*
 void convert(byte *t)
 {
  memset(b,0,64);
  for (int y=0; y<8; y++)
  {
   for (int x=0; x<8; x++)
   {
    byte d=0;
    byte z=0x80>>x;
    if (t[0x000000]&z) d|=1;
    if (t[0x080000]&z) d|=2;
    if (t[0x100000]&z) d|=4;
    if (t[0x180000]&z) d|=8;
    b[y][x]=d;
   }
   t+=2;
  }
 }
*/

/*
 void convert(byte *t)
 {
  memset(b,0,XW*YW);
  for (int y=0; y<YW; y++)
  {
   for (int x=0; x<XW; x++)
   {
    byte d=0;
    byte z=0x80>>(x&7);
//    if (t[0x000000]&z) d|=15;
    if (t[0x000000]&z) d|=1;
    if (t[0x000010]&z) d|=2;
    if (t[0x000020]&z) d|=4;
    if (t[0x000030]&z) d|=8;
    if (t[0x000040]&z) d|=16;
    if (t[0x000050]&z) d|=32;
    if (t[0x000060]&z) d|=64;
    if (t[0x000070]&z) d|=128;
    b[y][x]=d;
    if (z==1) t+=1;
   }
  }
 }
  */




/*
 void convert(byte *t)
 {
  memset(b,0,XW*YW);
  for (int y=0; y<YW; y++)
  {
   for (int x=0; x<8; x++)
   {
    byte d=0;
    byte z=0x80>>(x&7);
    if (t[0x000000]&z) d|=1;
    if (t[0x080000]&z) d|=2;
    if (t[0x100000]&z) d|=4;
    if (t[0x180000]&z) d|=8;
    b[y][x]=d;
   }

   for ( ; x<XW; x++)
   {
    byte d=0;
    byte z=0x80>>(x&7);
    if (t[0x000001]&z) d|=1;
    if (t[0x080001]&z) d|=2;
    if (t[0x100001]&z) d|=4;
    if (t[0x180001]&z) d|=8;
    b[y][x]=d;
   }
   t+=2;

  }
 }
*/
 /*
 void convert(byte *t)
 {
  memset(b,0,64);
  for (int y=0; y<YW; y++)
 {
   b[y][0]=*t; t++;
   b[y][1]=*t; t++;
   b[y][2]=*t; t++;
   b[y][3]=*t; t++;
   b[y][4]=*t; t++;
   b[y][5]=*t; t++;
   b[y][6]=*t; t++;
   b[y][7]=*t; t++;
  }
 } */
 /*
 void convert(byte *t)
 {
  memset(b,0,64);
  for (int y=0; y<YW; y++)
  {
   for (int x=0; x<XW; x++)
   {
    byte d=0;
    if (x==8) t++;
    byte z=0x80>>(x&7);
    if (t[0x000000]&z) d|=1;
    if (t[0x000008]&z) d|=2;
    if (t[0x000010]&z) d|=4;
    if (t[0x000018]&z) d|=8;
    if (t[0x000020]&z) d|=16;
    if (t[0x000028]&z) d|=32;
    if (t[0x000030]&z) d|=64;
    if (t[0x000038]&z) d|=128;
    b[y][x]=d;
   }
   t+=1;
  }
 } */

