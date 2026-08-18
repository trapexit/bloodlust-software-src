#include <i86.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "r2img.h"

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


void mode256();
#pragma  aux mode256 =  \
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



 //offscreen buffer
char *screen;
 //background buffer
char *bg;
 //palette
PALETTE *pal;

char *video=(char *)0xA0000;



volatile unsigned su=0;


IMG **id;
int numi;


int getpixels(IMG *i);
   


void main(int argc, char *argv[])
{

if (argc>=2)
 { 
  iter=atoi(argv[1]);
 }

    
 //get palette and background
volumefile v;
v.open("a32.vol");
pal=(PALETTE *)v.read();
bg=(char *)v.read();
v.close();


screen=(char *)malloc(70000)+2000;
memset(screen-2000,0,70000);
//screen=(char *)0xA0000;


printf("opening dis1.vol...\n");

//open volfile
v.open("dis1.vol");
v.skip();
v.skip();
//read images
id=(IMG **)v.readblock(&numi);
v.close();





//set video mode
mode256();
pal->set(0,256);
memcpy(video,bg,64000);


//--------------------
_disable();
int cycle1=RDTSC();
 //repeated loop
for (su=0; su<iter; su++)
 {
   for (int i=0; i<numi; i++)
   {
     //printf("drawing image %d...\n",i);
    if (id[i])  id[i]->draw(video,0,0,0);
    //getch();
    //printf("image %d: ptr=%p type=%d xw=%d yw=%d\n",i,id[i],id[i]->type,id[i]->xw,id[i]->yw);
   }
 }
int cycle2=RDTSC(); 
_enable();

//--------------------
modetext();

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







int getpixels(IMG *i)
{
 unsigned char *s;
 unsigned int runlength;
 int *yd=i->ydisp();
 int cnt=0;

 for (int y=0; y<i->yw; y++)
 {
  s=((unsigned char *)i)+yd[y]; //get pointer to start of rle line
   
  for (int x=i->xw; x>0; ) //line
   {

   //transparent
   runlength=*s; s++;
   x-=runlength;
   if (x<=0) break;
   //opaque
   runlength=*s; s++;
 
   for ( ; runlength>0 && x>0; s++,runlength--,x--)   cnt++;
   
  }
 }
return cnt;
}    





char errstr[256];
void cleanexit(int x)
{
 modetext();    
 if (x<0) printf("ERROR: %s\n",errstr);
 exit(x);
}    
















    
