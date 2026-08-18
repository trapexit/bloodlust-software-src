//volume creation
#include "\source\test\tgraph.h"

//creates new volume file
int volumefile::create(char *filename)
{
 if (!strchr(filename,'.')) strcat(filename,".vol");
  
 if (_dos_creat(filename,0,&h))
  {sprintf(errstr,"creating file %s",filename); cleanexit(-1);}
 
 char name[16];
 GetFileName(filename,name);

 strcpy(hdr.key,dsl);
 hdr.type=V_VOLHEADER; hdr.size=0;
 strcpy(hdr.name,name);
 _dos_write(h,&hdr,sizeof(header),&bytes);
 strcpy(volname,hdr.name);

 if (vdiagnose)
  printf("create: %8s\n",filename);

 return(0);
}    


//writes memory to volume file, storing the type that the file should be accessed as
// i.e. midi, raw, img
void volumefile::write(void *t,unsigned size,char type,char *file)
{
    
strcpy(hdr.key,dsl);    
hdr.type=type;
hdr.size=size;
if (file) GetFileName(file,hdr.name);
 else hdr.name[0]=0;
_dos_write(h,&hdr,sizeof(header),&bytes);
_dos_write(h,t,size,&bytes);

if (vdiagnose)
  printf("write:  %8s size: %7d type: %s\n",hdr.name,hdr.size,voltype[hdr.type]);
}

//write file
void volumefile::writefile(char *file)
{
 int th;
 unsigned size;
 if (_dos_open(file,O_BINARY | O_RDONLY,&th))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_RAWDATA,file); return; }
 size=filelength(th);

 char *t=(char *)malloc(size);
 _dos_read(th,t,size,&bytes);
 _dos_close(th);
 write(t,size,V_RAWDATA,file);
 free(t);
}

//write midi file
void volumefile::writemidi(char *file)
{
 int th;
 unsigned size;
 if (_dos_open(file,O_BINARY | O_RDONLY,&th))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_MIDI,file); return; }
   
 size=filelength(th);

 char *t=(char *)malloc(size);
 _dos_read(th,t,size,&bytes);
 _dos_close(th);
 write(t,size,V_MIDI,file);
 free(t);
}


//write screen file
void volumefile::writescreen(char *file)
{
 if (!strchr(file,'.')) strcat(file,".raw");
    
 int th;
 unsigned size;
 if (_dos_open(file,O_BINARY | O_RDONLY,&th))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_SCREEN,file); return; }
 size=filelength(th);

 char *t=(char *)malloc(size);
 _dos_read(th,t,size,&bytes);
 _dos_close(th);
 write(t,size,V_SCREEN,file);
 free(t);
}

//write palette from LBM file
void volumefile::writepalette(char *file)
{
 int th;
 unsigned size;
 if (!strchr(file,'.')) strcat(file,".lbm");
 
 if (_dos_open(file,O_BINARY | O_RDONLY,&th))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else {write(0,0,V_PALETTE,file); return; }

 size=sizeof(color)*256;

 color *pal=(color *)malloc(size);
 lseek(th,0x30,SEEK_SET);
 _dos_read(th,pal,256*3,&bytes); _dos_close(th);

 for (int i=0; i<256; i++) //adjust palette
  { pal[i].r>>=2;   pal[i].g>>=2;   pal[i].b>>=2; }
 
 write(pal,size,V_PALETTE,file);
 free(pal);
}

SOUND *ReadWavFile(char *file)
{
 int th;
 unsigned bytes;
 if (!strchr(file,'.')) strcat(file,".wav");
 
 if (_dos_open(file,O_BINARY | O_RDONLY,&th))
   if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
    else return(0);

 unsigned rate,size;
 lseek(th,0x18,SEEK_SET); //go to sample rate
 _dos_read(th,&rate,4,&bytes);
 lseek(th,0x28,SEEK_SET); //go to size
 _dos_read(th,&size,4,&bytes);

 //read data
 SOUND *d=(SOUND *)malloc(size+4);
 d->soundsize=size;
 _dos_read(th,d->soundptr,size,&bytes);
 _dos_close(th);
if (vdiagnose)
 cprintf(".wav samplerate: %5d size: %6d ",rate,size);
 
 if (rate<16000)
  {
   if (vdiagnose)   
   cprintf("expanding to 16000...");
   unsigned finalsize=size*16000/rate; 
   SOUND *d2=(SOUND *)malloc(finalsize+4);
   d2->soundsize=finalsize;
   unsigned dif=(rate<<10)/16000;
   for (int i=0,j=0; i<finalsize; i++,j+=dif) //go through every byte in final
      d2->soundptr[i]=d->soundptr[j>>10];
   free(d);  d=d2;
   if (vdiagnose)   
   printf("size: %6d\n",d->soundsize);
  } else
 if (rate>16000)
  {
   if (vdiagnose)   
   cprintf("contracting to 16000...");
   unsigned finalsize=size*16000/rate;
   SOUND *d2=(SOUND *)malloc(finalsize+4);
   d2->soundsize=finalsize;
   unsigned dif=(16000<<10)/rate;
   for (int i=0,j=0; i<size; i++,j+=dif) //go through every byte in final
      d2->soundptr[j>>10]=d->soundptr[i];
   free(d); d=d2; 
   if (vdiagnose)   
   printf("size: %6d\n",d->soundsize);
  } else
  if (vdiagnose)   
    printf("no adjustment.\n");

 for (int i=0; i<d->soundsize; i++)
  d->soundptr[i]-=0x80; //make signed

 return(d); //return SOUND struct
}    


//writes 16000hz sound file from .wav file
void volumefile::writesound(char *file)
{
 SOUND *d=ReadWavFile(file);
 if (d)
  {
   write(d,d->soundsize+4,V_SOUND,file);
   free(d);
  } else write(0,0,V_SOUND,file); 
}


int ReadImageFile(char **m,char *file)
{
if (!strchr(file,'.')) strcat(file,".raw");

int h;
unsigned bytes;
if (_dos_open(file,O_BINARY | O_RDONLY,&h)) {*m=0; return 0;}


char header[4];
_dos_read(h,header,4,&bytes);
int x=(int)header[1];
int y=(int)header[2];

int size=y*2+filelength(h);

char *t=(char *)malloc(size);
for (int i=0;i<4;i++) t[i]=header[i];
short *ylist=(short *) (t+4);
char *imgptr=t+4+ y*2;

_dos_read(h,imgptr,filelength(h)-4,&bytes);
_dos_close(h);

for (i=0; i<y; i++)
{
ylist[i]=imgptr-t;

int xl=x;
while (xl>0)
 {
  if ( (*imgptr)&0x80) {xl-=(*imgptr)&0x7f; imgptr++;}
         else            {xl-=*imgptr; imgptr+=*imgptr; imgptr++; }
 }
}

*m=t;

return(size);
}





//write image file (raw)
void volumefile::writeimage(char *file)
{
 char *t;
 unsigned size=ReadImageFile(&t,file);
 if (!size)
  if (vwerror) {sprintf(errstr,"opening file %s",file); cleanexit(-1);}
            else {write(0,0,V_IMAGE,file); return; }
 
 write(t,size,V_IMAGE,file);
 free(t);
}



//writes all file names with paths in a list as a block
void volumefile::writelistblock(char *list,char type)
{
 char *ptr[256];
 unsigned idx[256];
 unsigned sizes[256];

 unsigned num; //number of items

 //open list
 FILE *f=fopen(list,"rt");
 if (!f) return;

 char line[50];
 fgets(line,50,f); //get maximum number of items
 sscanf(line,"%d",&num);

 for (int i=0; i<num; i++) {idx[i]=sizes[i]=0; ptr[i]=0;}

 for (i=0; i<num; i++) //read all images
  if (fgets(line,50,f)) //read line into memory
   {
    int n; char name[16];
    if (!(sscanf(line,"%d %s",&n,name)==2 && n<num)) continue;

    char *item=0;
    unsigned itemsize=0;
    
    switch(type) //go read it
    {
     case V_RAWDATA:
         {
          int th; unsigned bytes;
          if (!_dos_open(name,O_BINARY | O_RDONLY,&th))
           {  
            itemsize=filelength(th);
            item=(char *)malloc(itemsize);
            _dos_read (th,item,itemsize,&bytes);
            _dos_close(th);
           } 
         }
         break;
      case V_SOUND:
        {
         strcat(name,".WAV");
         SOUND *d=ReadWavFile(name);
         if (d)  itemsize=d->soundsize+4;
         item=(char *)d;
        }
       break;
      case V_IMAGE:
        strcat(name,".RAW");
        itemsize=ReadImageFile((char **)&item,name); //read the image
        break;
    }

   //item has been read
   ptr[n]=item;
   sizes[n]=itemsize;
  } //if fgets......
    
fclose(f); 
  
  
//now we gotta rewrite this all


//reindex it
unsigned tsize=0;
for (i=0; i<num; i++)
 {
  idx[i]=tsize;
  tsize+=sizes[i];
 }



//set up header
strcpy(hdr.key,dsl);    
hdr.type=V_BLOCK;
GetFileName(list,hdr.name);
hdr.size=tsize+4+num*4;
_dos_write(h,&hdr,sizeof(header),&bytes);

_dos_write(h,&num,4,&bytes);
_dos_write(h,idx,num*4,&bytes);

//write actual shit
for (i=0; i<num; i++)
 _dos_write(h,ptr[i],sizes[i],&bytes);

for (i=0; i<num; i++)
   if (ptr[i]) free(ptr[i]);

    
}    


































//block format:
//unsigned num;       //number of elements
//unsigned index[num] //index into block for each element
//char block[];

//writes all file names with paths in a range as a block
void volumefile::writerangeblock(char *path,int num,char type) //1,2,3,...num
{
//set up header
strcpy(hdr.key,dsl);    
hdr.type=V_BLOCK;
GetFileName(path,hdr.name);

if (!strchr(path,'.'))
{
 if (type==V_SOUND) strcat(path,".wav");
}

unsigned *index=(unsigned *)malloc(num);
for (int i=0; i<num; i++) index[i]=0;

unsigned size=0; //size of block
char file[40];
int th;

unsigned oldpos=lseek(h,0,SEEK_CUR);
lseek(h,4+4*num+sizeof(header),SEEK_CUR); //skip header stuff for now

if (vdiagnose)
 printf("writing %d range elements %s...\n",num,path);

for (i=0; i<num; i++)
 {
  unsigned s;
  sprintf(file,path,i+1); //get file name
  switch(type) //go read it
   {
    case V_RAWDATA:
          if (!_dos_open(file,O_BINARY | O_RDONLY,&th))
           {  
            s=filelength(th);
            char *t=(char *)malloc(s);
            _dos_read (th,t,s,&bytes); _dos_close(th);
            _dos_write(h, t,s,&bytes);
            free(t);
           }
         break;
    case V_SOUND:
         SOUND *d=ReadWavFile(file);
         if (d)
          {
            s=d->soundsize+4;
            index[i]=size+num*4+4;
            _dos_write(h,d,s,&bytes);
            free(d);
          }
       break;
  }

  index[i]=size+num*4+4;
  size+=s; //advance index

  if (vdiagnose) printf("%11s  size: %7d index: %7d\n",file,s,index[i]);
 }    

unsigned endpos=lseek(h,0,SEEK_CUR);
lseek(h,oldpos,SEEK_SET);

printf("writing block size: %d\n",size);
hdr.size=size+4+4*num;
_dos_write(h,&hdr,sizeof(header),&bytes);  //write header
_dos_write(h,&num,4,&bytes);                //write num elements
        _dos_write(h,index,4*num,&bytes);           //write index to elements
free(index);

lseek(h,endpos,SEEK_SET);

}


