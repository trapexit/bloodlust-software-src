#include <i86.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>


#include "glib.h"
#include "smix.h"

#include "file.h"
#include "vol.h"


int iter=100;

int RDTSC(void);
#pragma aux RDTSC = 0x0f 0x31 modify [eax edx] value [eax];


extern "C"
{
int PITCH=320;
int SCREENX=320;
int SCREENY=200;
};



 //offscreen buffer
char *screen;
 //background buffer
char *bg;
 //palette
color *pal;

char *video=(char *)0xA0000;



volatile unsigned su=0;


img **id;
int numi;


int getpixels(img *i);
   


void main(int argc, char *argv[])
{

if (argc>=2)
 { 
  iter=atoi(argv[1]);
 }

    
 //get palette and background
volumefile v;
v.open("a32.vol");
pal=(color *)v.read();
bg=(char *)v.read();
v.close();


screen=(char *)malloc(70000)+2000;
memset(screen-2000,0,70000);
//screen=(char *)0xA0000;



//open volfile
v.open("dis1.vol");
v.skip();
v.skip();
//read images
id=(img **)v.readblock(&numi);
v.close();



//set video mode
Mode256();
LoadPalette(pal,0,256);
memcpy((char *)0xA0000,bg,64000);


//--------------------
_disable();
int cycle1=RDTSC();
 //repeated loop
for (su=0; su<iter; su++)
 {
   for (int i=0; i<numi; i++)
    if (id[i])
    {
     DrawImage(id[i],video,0,0,0);
    }
 }
int cycle2=RDTSC(); 
_enable();

//--------------------
ModeText();

int totalpixels=0;
int totalarea=0;

for (int i=0; i<numi; i++)
 if (id[i])
 {
  totalarea+=id[i]->xw*id[i]->yw; //rectangular area of image
  totalpixels+=getpixels(id[i]); //getpixels(id[i]); //number of actual pixels in this image
 }

printf("IMAGE STATISTICS\n");
printf("images: %d\n",numi);
printf("total area:   %d\n",totalarea); 
printf("total opaque: %d\n",totalpixels);
printf("opacity: %f%%\n",((double)totalpixels)*100/((double)totalarea));
printf("\n");


printf("Iterations: %d\n",su);

int cycles=cycle2-cycle1;
printf("Total cycles: %d\n",cycles);
printf("cycles/iteration: %f\n",((float)cycles)/su);
printf("cycles/image:     %f\n",((float)cycles)/(su*numi));
printf("\n");
printf("cycles/pixel: %f\n",((float)cycles)/(totalpixels*su));
printf("cycles/area:  %f\n",((float)cycles)/(totalarea*su));

}




int getpixels(img *i)
{
int wx,wy; //width and height of image
//int k;
wx=i->xw;
wy=i->yw;

char *s=&i->data[wy*2]; ///s points to image data

unsigned int pixels=0;
for (; wy>0; wy--)
{
for (int i=wx; i>0; )
 {
  unsigned char a=(unsigned char)*s; s++;
  if  (a&0x80) //transparent
         { a&=0x7F;  i-=a; }
  else         //opaque
         {
                 pixels+=a;
                 s+=a;
                i-=a;
         }
 }
}

return pixels;
    
}




char errstr[256];
void cleanexit(int x)
{
 ModeText();    
 if (x<0) printf("ERROR: %s\n",errstr);
 exit(x);
}    
















    
