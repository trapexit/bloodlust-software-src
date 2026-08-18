//object definition functions
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>


#include "r2img.h"
#include "objdef.h"

#include "file.h"
#include "message.h"
#include "dd.h"

#include "sound.h"

#include "vol.h"

//-----------------------------
//   series editable functions
//--------------------------

//inserts num bytes at p
void *series::insertbytes(void *p,int num)
{
if (num<=0) return p;

if (!first) //if no memory has been allocated
{
 first=(frame *)malloc(num); ///allocate memory
 memset(first,0,num); //clear memory
 size=num;
// msg.printf(3,"created series with %dbytes",size);
 return (void *)first;
}

char *f=(char *)first;  //keep as char *
int pos=((char *)p)-f;             //find relative position within series list (int)
if (pos<0 || pos>size+1) {msg.printf(5,"invalid insertion ssize:%d pos:%d num:%d",size,pos,num); return p;}

//get more memory
f=(char *)realloc(f,size+num);
memmove(f+pos+num,f+pos,size-pos);


first=(frame *)f;
size+=num;
//msg.printf(3,"inserted %d bytes at %d; size=%d",num,pos,size);

memset(f+pos,0,num); //clear memory
return (void *)(f+pos); //return it
}



//delete num bytes at p
void *series::deletebytes(void *p,int num)
{
if (num<=0) return p;
char *f=(char *)first;  //keep as char *
int pos=((char *)p)-f;             //find relative position within series list (int)
if (!first || (pos+num)>size || pos<0 || pos>=size)
   {msg.printf(5,"invalid deletion %d %d",size,pos); return p;}

memmove(f+pos,f+pos+num,size-pos-num);


size-=num;
//msg.printf(3,"deleted %d bytes at %d; size=%d",num,pos,size);

if (size>0)
 {
  f=(char *)realloc(f,size+num);
  first=(frame *)f;
  return (void *)(f+pos);
 } else {first=0; free(f); size=0; return 0;}
}



void series::insertframe(frame *fptr)
{
 fptr=(frame *)insertbytes(fptr,sizeof(frame));
 nf++;
 fptr->size=sizeof(frame);
 fptr->dur=10;
}


void series::deleteframe(frame *fptr)
{
 fptr=(frame *)deletebytes(fptr,fptr->size);
 nf--;
}





//------------------------------------------------
//     objectdefw editable odf functions
//------------------------------------------------

objectdefw::objectdefw(class objectspace *tosp,int tnum,char *tname,char *tvolfile,char *tsdf,char *tdir)
 :objectdef(tosp,tnum,tname,tvolfile)
{
 imgnames=sndnames=0;
 strcpy(sdf,tsdf);
 strcpy(dir,tdir);
}


void objectdefw::freeodf()
{
 if (imgnames) free(imgnames); imgnames=0;
 if (sndnames) free(sndnames); sndnames=0;
 objectdef::freeodf();
}

void objectdefw::save()
{
 if (!loaded) return;
 if (readonly) return;

 //go to object directory
 char oldpath[30];
 getcwd(oldpath,30);
 if (chdir(dir)) {msg.printf(5,"error: %s dir does not exist",dir);  return;}

 //save series
 writeseries("series.lst");

 msg.printf(1,"%s saved: %d series",name,nums);

 chdir(oldpath);

}

//read object from disk, images, sounds, and series list components
int objectdefw::read()
{
//go to object directory
char oldpath[30];
getcwd(oldpath,30);
if (chdir(dir)) {msg.printf(5,"error: %s dir does not exist",dir);  return 0;}

//read individual components
if (!readimages("image.lst")) {msg.printf(5,"error: reading images"); freeodf(); return 0;} else
if (!readsounds("sound.lst")) {msg.printf(5,"error: reading sounds"); freeodf(); return 0;} else
if (!readseries("series.lst")) {msg.printf(5,"error: reading series list"); freeodf(); return 0;} else
 { //successful
  readonly=0;
  loaded=1;
  isvol=0;
  msg.printf(1,"%s loaded: %dimgs %dsnds %dK",name,numi,numsounds,mem/1024);
 }

chdir(oldpath);
return loaded;
}

//read names from a *.lst file
defname *readlistfile(char *lstfile,int &num)
{
 num=0;
 //array of names
 defname *n=(defname *)calloc(256,sizeof(defname));
 if (!n) return 0;

  //open list file
 FILE *f=fopen(lstfile,"rt");
 if (!f) return n;

 //read the file (if it exists)
 static  char line[50];
 fgets(line,50,f); //get maximum number of names
 sscanf(line,"%d",&num);

 static  int x;
 static char name[12];
 while (fgets(line,50,f)) //read line into memory
   if (sscanf(line,"%d %s",&x,name)==2)
     {
      strcpy(n[x],name); //copy name
      if (x>num) num=x;
     }

fclose(f);

return n;
}

void writelistfile(char *lstfile,defname *n,int num)
{ //write files to list
 FILE *f=fopen(lstfile,"wt");
 fprintf(f,"%d\n",num); //number
 for (int i=0; i<num; i++)
  if (n[i][0]) fprintf(f,"%d %s\n",i,n[i]);
 fclose(f);
}


//------------------------------------------------------------------
#include "sound.h"
static int soundload(char *name,objectdefw *odf)
{
 char fname[20];
 //get name without extension
 strcpy(fname,name);
 *strchr(fname,'.')=0;

 for (int i=0; i<odf->numsounds; i++)
   if (!stricmp(odf->sndnames[i],fname)) break; //we found it

  if (i==odf->numsounds) //was not found
   {
    strcpy(odf->sndnames[odf->numsounds++],fname);
    if (odf->numsounds==255) {msg.printf(5,"error: too many sounds\n"); return 0;}
   }
   //read image
  odf->sounds[i]=ReadWavFile(name);
  odf->mem+=odf->sounds[i]->soundsize;
 return 1;
}

int  objectdefw::readsounds(char *lstname)
{
 sounds=(SOUND **)calloc(256,4);
 if (!sounds) return 0;

  //read all sound names
 sndnames=readlistfile(lstname,numsounds);
 if (!sndnames) return 0;

 //read all sounds in directory
 enumdir("*.wav",(DIRFUNCPTR)soundload,this);

 //write all sound names
 writelistfile(lstname,sndnames,numsounds);

  //reallocate
 sndnames=(defname *)realloc(sndnames,numsounds*sizeof(defname));
 sounds=(SOUND **)realloc(sounds,numsounds*4);

 #ifdef WIN95
 convertdsounds();
 #endif
 return 1;
}


#include <sys\stat.h>
unsigned gettime(char *file)
{
 struct stat s;
 if (stat(file,&s)) return 0;
 return (unsigned)s.st_mtime;
}

//------------------------------------------------------
//enum dir func
static int imageload(char *name,objectdefw *odf)
{
 char fname[20];
 //get name without extension
 strcpy(fname,name);
 *strchr(fname,'.')=0;

 for (int i=0; i<odf->numi; i++)
   if (!stricmp(odf->imgnames[i],fname)) break; //we found it

  if (i==odf->numi) //was not found
   {
    strcpy(odf->imgnames[odf->numi++],fname);
    if (odf->numi==255) {msg.printf(5,"error: too many images\n"); return 0;}
   }
   //read image
 char r2name[16];
 strcpy(r2name,fname);
 strcat(r2name,".r2");

 char bbmname[16];
 strcpy(bbmname,fname);
 strcat(bbmname,".bbm");


 unsigned bbmtime=gettime(bbmname);
 unsigned r2time=gettime(r2name);
 if (r2time<bbmtime) //must be re-rawed
  {
   msg.printf(3,"%s rawed",bbmname);
   odf->id[i]=ReadLBMFile(bbmname)->compress(255);
   odf->id[i]->write(r2name);
  } else
  { //just read it
   odf->id[i]=loadimage(r2name);
  }

/*
 #ifdef READBBM
 odf->id[i]=ReadLBMFile(name)->compress(255);
 #else
 odf->id[i]=loadimage(name);
 #endif
  */
 odf->mem+=odf->id[i]->size;
 return 1; //continue
}
int  objectdefw::readimages(char *lstname)
{
 id=(IMG **)calloc(256,4);
 if (!id) return 0;

  //read all image names
 imgnames=readlistfile(lstname,numi);
 if (!imgnames) return 0;

 //read all images in directory
// #ifdef READBBM
// enumdir("*.bbm",(DIRFUNCPTR)imageload,this);
 enumdir("*.r2",(DIRFUNCPTR)imageload,this);


 //rewrite images names
 writelistfile(lstname,imgnames,numi);

  //reallocate
 imgnames=(defname *)realloc(imgnames,numi*sizeof(defname));
 id=(IMG **)realloc(id,numi*4);
 return 1;
}


//---------------------------------------------------------------------

int objectdefw::readsdf(char *filename)
{
 nums=0;
 FILE *f=fopen(filename,"rt");
 if (!f) {msg.printf(5,"cannot open %s",filename);  return 0;}

 char line[50];
 sd=(series *)calloc(256,sizeof(series));  //allocate memory for all seriess
 while (fgets(line,50,f))
  {
   int i; char n[50],d[50];
   if (sscanf(line,"%s %s %d",d,n,&i)==3 && !stricmp(d,"#define"))
    {
     strcpy(sd[i].name,n); //copy name over
     if (i>=nums) nums=i+1;
   }
  }
 fclose(f);
 return 1;
}

void objectdefw::writesdf(char *filename)
{
 FILE *f=fopen(filename,"wt");
 for (int i=0; i<nums; i++)
   fprintf(f,"#define %s %d\n",sd[i].name,i);
 fclose(f);
}


int  objectdefw::readseries(char *lstname)
{
 //read series definition
 if (!readsdf(sdf)) return 0;

 //read saved list
 FILEIO f;
 if (f.open(lstname)) return 0;
 if (f.readint()!=0x6969) {msg.printf(5,"ERROR: invalid series.lst"); return 0;}

 int tnums=f.readint(); //number of saved series
 int tsize=f.readint(); //size of each series

 series *t=(series *)f.readalloc(tnums*tsize);
 if (!t) {msg.printf(5,"ERROR: invalid series.lst"); return 0;}

  //go through each saved series
 for (int i=0; i<tnums; i++)
  {
   //read it from disk
   t[i].first=(frame *)f.readalloc(t[i].size);

   //find the series name in the sdf
   for (int j=0; j<nums; j++)
    if (!stricmp(t[i].name,sd[j].name)) break;

   //Correlate saved series with sdf
  //we found a matching series name
    if (j<nums) { sd[j]=t[i]; }//copy series to final list that has the same name
     else
    if (i<nums) //it was a part of the standard list
    {
     if (!sd[i].first)
      {
       if (!t[i].size) strcpy(t[i].name,sd[i].name);
       sd[i]=t[i]; //if cooresponding empty numeral series exists...copy it over
      }
     else    if (t[i].first) free(t[i].first); //if no numeral or character cooresponding one exists, free it
    } else //it was added afterwards
      sd[i]=t[i]; //copy it over
  }
 free(t); //free temp
 f.close();

 //realloc
 if (nums<tnums) nums=tnums;
 sd=(series *)realloc(sd,(nums+1)*sizeof(series));
 return 1;
}

void objectdefw::writeseries(char *filename)
{
 FILEIO f;
 f.create(filename);

 f.writeint(0x6969); //write sig

 f.writeint(nums); //number of series
 int seriessize=sizeof(series);
 f.writeint(seriessize);

 f.write(sd,nums*sizeof(series)); //write all serieses

 //write all the frames for each series
 for (int i=0; i<nums; i++)
  f.write(sd[i].first,sd[i].size);

 f.close();

 dirty=0;
}




//-----------------------------------------------
//volumize


void objectdefw::writevol(volumefile &v)
{
 v.writefile("series.lst"); //write series's
}


void objectdefw::volumize()
{
 if (readonly) return;


 volumefile v;
    //create volfile
 v.create(volfile);

 //Go to that object directory
 char oldpath[50];
 getcwd(oldpath,50);
 if (chdir(dir)) {msg.printf(5,"%s dir does not exist",dir);/* v.close(); */return;}

 //write object shit
 v.write(name,strlen(name)+1,V_RAWDATA,"objname");  //write name

 //write images
 v.writelistblock("image.lst",V_IMAGE);

  //write sounds
 v.writelistblock("sound.lst",V_SOUND);

 //write vol crap
 writevol(v);

 //change to old dir
 chdir(oldpath);
 msg.printf(2,"%s volumized %d bytes",name,v.f.size());
 v.close();
}





