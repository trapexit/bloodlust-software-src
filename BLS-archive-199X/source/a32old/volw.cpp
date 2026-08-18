//volume creation
#include "r2img.h"
#define SOUNDFREQ 22050

//read an element header
void volumefile::writeheader(int type,int size,char *name)
{
 strcpy(hdr.key,dsl);
 hdr.type=type;
 hdr.size=size;
 if (name) strcpy(hdr.name,name);
  else hdr.name[0]=0;
 f.write(&hdr,sizeof(header));
}    


//creates new volume file
int volumefile::create(char *filename)
{
 if (!strchr(filename,'.')) strcat(filename,".vol");
  
 if (f.create(filename))
  {sprintf(errstr,"creating file %s",filename); cleanexit(-1);}
 
 char name[16];
 GetFileName(filename,name);
 strcpy(volname,name);

 writeheader(V_VOLHEADER,0,volname);
 
 if (vdiagnose)  printf("create: %8s\n",filename);
 return(0);
}    


//writes memory to volume file, storing the type that the file should be accessed as
// i.e. midi, raw, img
void volumefile::write(void *t,unsigned size,char type,char *file)
{
 if (strchr(file,'.')) *strchr(file,'.')=0;
 writeheader(type,size,file);
 f.write(t,size);

 if (vdiagnose)  printf("write:  %8s size: %7d type: %s\n",hdr.name,hdr.size,voltype[hdr.type]);
}

//write file
void volumefile::writefile(char *file)
{
 FILEIO g;
  //open second file
 if (g.open(file))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_RAWDATA,file); return; }

 //read it all
 int size=g.size();
 void *t=g.readalloc(size);
 g.close();
 write(t,size,V_RAWDATA,file);
 free(t);
}

//write midi file
void volumefile::writemidi(char *file)
{
 if (!strchr(file,'.')) strcat(file,".mid");

 FILEIO g;
  //open second file
 if (g.open(file))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_RAWDATA,file); return; }

 //read it all
 int size=g.size();
 void *t=g.readalloc(size);
 g.close();
 write(t,size,V_MIDI,file);
 free(t);
}


//write screen file
void volumefile::writescreen(char *file)
{
 SCR *i=loadscreen(file);
 if (!i)
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_SCREEN,file); return; }

 write(i,i->size,V_SCREEN,file);
 free(i);
}

//write palette from LBM file
void volumefile::writepalette(char *file)
{
 if (!strchr(file,'.')) strcat(file,".lbm");

 FILEIO g;
 if (g.open(file))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_PALETTE,file); return; }

 int size=3*256;

 g.setpos(0x30);
 unsigned char *pal=(unsigned char *)g.readalloc(size);
 g.close();

 for (int i=0; i<size; i++) //adjust palette
   pal[i]>>=2;
 
 write(pal,size,V_PALETTE,file);
 free(pal);
}

/*
#ifndef SOUND
struct SOUND
{
 unsigned int soundsize;
 char soundptr[];
};
#endif 
*/
#include "smix.h"
SOUND *ReadWavFile(char *file)
{
 FILEIO g;
 if (!strchr(file,'.')) strcat(file,".wav");
 
 if (g.open(file))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else return(0);

 unsigned rate,size;
 g.setpos(0x18); //go to sample rate
 rate=g.readint();
 g.setpos(0x28);
 size=g.readint();

 //read data
 SOUND *d=(SOUND *)malloc(size+4);
 d->soundsize=size;
 g.read(&d->soundptr,size);
 g.close();

 if (vdiagnose)  cprintf(".wav samplerate: %5d size: %6d ",rate,size);
 
 if (rate<16000)
  {
   if (vdiagnose) cprintf("expanding to %d...",SOUNDFREQ);
   unsigned finalsize=size*SOUNDFREQ/rate; 
   SOUND *d2=(SOUND *)malloc(finalsize+4);
   d2->soundsize=finalsize;
   unsigned dif=(rate<<10)/SOUNDFREQ;
   for (int i=0,j=0; i<finalsize; i++,j+=dif) //go through every byte in final
      d2->soundptr[i]=d->soundptr[j>>10];
   free(d);  d=d2;
   if (vdiagnose)  printf("size: %6d\n",d->soundsize);
  } else
 if (rate>16000)
  {
   if (vdiagnose)   
   cprintf("contracting to %d...",SOUNDFREQ);
   unsigned finalsize=size*SOUNDFREQ/rate;
   SOUND *d2=(SOUND *)malloc(finalsize+4);
   d2->soundsize=finalsize;
   unsigned dif=(SOUNDFREQ<<10)/rate;
   for (int i=0,j=0; i<size; i++,j+=dif) //go through every byte in final
      d2->soundptr[j>>10]=d->soundptr[i];
   free(d); d=d2; 
   if (vdiagnose) printf("size: %6d\n",d->soundsize);
  } else
  if (vdiagnose) printf("no adjustment.\n");

 for (int i=0; i<d->soundsize; i++)
  d->soundptr[i]-=0x80; //make signed

 return(d); //return SOUND struct
}    


//writes sound file from .wav file
void volumefile::writesound(char *file)
{
 SOUND *d=ReadWavFile(file);
 if (d)
  {
   write(d,d->soundsize+4,V_SOUND,file);
   free(d);
  } else write(0,0,V_SOUND,file); 
}


//write image file (raw)
void volumefile::writeimage(char *file)
{
 IMG *i=loadimage(file);
 if (!i)
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_IMAGE,file); return; }

 write(i,i->size,V_IMAGE,file);
 free(i);
}



//writes all file names with paths in a list as a block
void volumefile::writelistblock(char *list,char type)
{
 char *ptr[256];
 unsigned idx[256];
 unsigned sizes[256];

 unsigned num; //number of items

 //open list
 FILE *l=fopen(list,"rt");
 if (!l) return;

 char line[50];
 fgets(line,50,l); //get maximum number of items
 sscanf(line,"%d",&num);

 for (int i=0; i<num; i++) {idx[i]=sizes[i]=0; ptr[i]=0;}

 for (i=0; i<num; i++) //read all images
  if (fgets(line,50,l)) //read line into memory
   {
    int n; char name[16];
    if (!(sscanf(line,"%d %s",&n,name)==2 && n<num)) continue;

    char *item=0;
    unsigned itemsize=0;
    
    switch(type) //go read it
    {
      case V_SOUND:
        {
         SOUND *d=ReadWavFile(name);
         if (d)  {item=(char *)d; itemsize=d->soundsize+4;}
        }
       break;
      case V_IMAGE:
        {
         IMG *i=loadimage(name); //read the image
         if (i)
         {item=(char *)i; itemsize=i->size;
         i->draw((char *)0xA0000,0,0,0);
         }
        }
      break;
      case V_SCREEN:
        {
         SCR *i=loadscreen(name); //read the image
         if (i) {item=(char *)i; itemsize=i->size;}
        }
      break;
    }

   //item has been read
   ptr[n]=item;
   sizes[n]=itemsize;
  } //if fgets......
    
fclose(l); 
  
  
//now we gotta rewrite this all


//reindex it
unsigned tsize=0;
for (i=0; i<num; i++)
if (ptr[i])
 {
  idx[i]=tsize;
  tsize+=sizes[i];
 }



//set up header
GetFileName(list,hdr.name);
writeheader(V_BLOCK,tsize+4+num*4,hdr.name);

f.writeint(num);
f.write(idx,num*4);

//write actual shit
for (i=0; i<num; i++)
if (ptr[i]) 
 {
  f.write(ptr[i],sizes[i]);
  free(ptr[i]);
 }
  
}    

























