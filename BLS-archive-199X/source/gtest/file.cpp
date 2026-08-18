#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"

#ifdef __BORLANDC__
#include <dos.h>
#include <dir.h>
#endif

#ifdef __WATCOMC__
#include <dos.h>
#include <direct.h>
#endif

#ifdef _MSC_VER
#include <windows.h>
#include <direct.h>
#endif

#ifdef DOS
#include "dos\doslfn.h"
#endif


void enumdir(char *path, DIRFUNCPTR func,void *context,int type)
{
 #ifdef WIN95
 #ifdef __BORLANDC__
 ffblk ff;
 int done=findfirst(path,&ff,(type==DE_DIR) ? FA_DIREC : 0);
 while (!done)
  {
   if (type!=DE_DIR || (ff.ff_attrib&FA_DIREC))
     if (!func(ff.ff_name,context)) break;
   done=findnext(&ff);
  }
 #endif
 #ifdef __WATCOMC__
 find_t ff;
 int done=_dos_findfirst(path,(type==DE_DIR) ? _A_SUBDIR : 0,&ff);
 while (!done)
  {
   if (type!=DE_DIR || (ff.attrib&_A_SUBDIR))
     if (!func(ff.name,context)) break;
   done=_dos_findnext(&ff);
  }
 _dos_findclose(&ff);
 #endif

 #ifdef _MSC_VER
 WIN32_FIND_DATA ff;
 HANDLE h=FindFirstFile(path,&ff);
 if (h==INVALID_HANDLE_VALUE) return;
 do
 {
  if ((type==DE_DIR &&  (ff.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) ||
      (type!=DE_DIR && !(ff.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)))
    if (!func(ff.cFileName,context)) break;
 } while (FindNextFile(h,&ff));

 FindClose(h);
 #endif
 #endif

 #ifdef DOS
 lfn_find ff;
 unsigned h=lfn_findfirst(path,(type==DE_DIR) ? _A_SUBDIR : 0,&ff);
 if (!h) return;
 do
 {
  if (type!=DE_DIR || (ff.attrib&_A_SUBDIR))
    if (!func(ff.filename,context)) break;
 } while (lfn_findnext(h,&ff));
 lfn_findclose(h);
 #endif
}



void enumdrives(DRIVEFUNCPTR func,void *context)
{
 char oldpath[128];
 getcurrentdir(oldpath);
 unsigned currentdrive,numdrives;
 #ifdef __BORLANDC__
 currentdrive=getdisk();
 numdrives=setdisk(currentdrive)+1;
 #endif
 #ifdef __WATCOMC__
 _dos_getdrive(&currentdrive);
 _dos_setdrive(currentdrive,&numdrives);
 #endif
 #ifdef _MSC_VER
 numdrives=_getdrives();
 #endif
 if (numdrives>26) numdrives=26;

 setcurrentdir(oldpath);

 for (int i=0; i<numdrives; i++)
  func(i,"",context);
}

//0=A 1=B
void setcurrentdrive(int drivenum)
{
 #ifdef __BORLANDC__
 setdisk(drivenum);
 #endif
 #ifdef __WATCOMC__
 unsigned numdrives;
 _dos_setdrive(drivenum+1,&numdrives);
 #endif
 #ifdef _MSC_VER
 _chdrive(drivenum+1);
 #endif
}

//0=A 1=B
int getcurrentdrive()
{
 #ifdef __BORLANDC__
 return getdisk();
 #endif
 #ifdef __WATCOMC__
 unsigned currentdrive;
 _dos_getdrive(&currentdrive);
 return currentdrive-1;
 #endif
 #ifdef _MSC_VER
 return _getdrive()-1;
 #endif
}


void getcurrentdir(char *d)
{
 getcwd(d,128);
}
void setcurrentdir(char *d)
{
 #ifdef WIN95
 chdir(d);
 #endif
 #ifdef DOS
 lfn_chdir(d);
 #endif
};

int getfilesize(char *filename)
{
 struct stat sb;
 stat(filename,&sb);
 return sb.st_size;
}


static char temp[128],temp2[128];

//resolves a directory and filename to a path
// ex "C:\nes" + "x" + ".nes" = "C:\nes\x.nes"
char *resolvepath(char *dir,char *name,char *ext)
{
 _makepath(temp,0,dir,name,ext);
 return temp;
}

int absolutepath(char *dir,char *abspath)
{
 return _fullpath(abspath,dir,128) ? 1 : 0;
}

char *extractname(char *path)
{
 for (int i=strlen(path); i>0; i--)
  if (path[i]=='\\' || path[i]==':') {i++; break;}
 return path+i;
}

//sees if a file exists
int fileexists(char *path)
{
 struct stat sb;
 #ifdef WIN95
 return !stat(path,&sb);
 #endif
 #ifdef DOS
 char shortpath[256];
 lfn_getshortname(path,shortpath);
 return !stat(shortpath,&sb);
 #endif
}

void getshortpath(char *longpath,char *shortpath)
{
 #ifdef WIN95
 strcpy(shortpath,longpath);
 #else
 lfn_getshortname(longpath,shortpath);
 #endif
}


//---------------------------------------------
//---------------------------------------------

//opens readonly
int FILEIO::open(char *filename)
{
 if (h) close(); //already open
 _fmode=O_BINARY;
 #ifdef WIN95
 h=::open(filename,O_RDONLY|O_BINARY);
 #endif
 #ifdef DOS
 h=lfn_open(filename);
 #endif
 if (h==-1) {h=0; return -1;} //error
 return 0;
}

//opens read/wr
int FILEIO::open_rdwr(char *filename)
{
 if (h) close(); //already open
 _fmode=O_BINARY;
 #ifdef WIN95
 h=::open(filename,O_RDWR|O_BINARY);
 #endif
 #ifdef DOS
 h=lfn_openrdwr(filename);
 #endif
 if (h==-1) {h=0; return -1;} //error
 return 0;
}



//create
int FILEIO::create(char *filename)
{
 if (h) close();
 _fmode=O_BINARY;
 #ifdef WIN95
 #ifdef __WATCOMC__
 h=creat(filename,S_IWRITE|S_IREAD);
 #endif
 #ifdef __BORLANDC__
 h=_rtl_creat(filename,0);
 #endif
 #ifdef _MSC_VER
 h=creat(filename,S_IWRITE|S_IREAD);
 #endif
 #endif

 #ifdef DOS
 h=lfn_create(filename);
 #endif

 if (h==-1) {h=0; return -1;} //error
 return 0;
}

void FILEIO::close()
{
 if (!h) return;
 #ifdef WIN95
 ::close(h);
 #endif
 #ifdef DOS
 lfn_close(h);
 #endif
 h=0;
}

int FILEIO::read(void *t,unsigned size)
{
 if (!h) return -1;
 #ifdef WIN95
 return ::read(h,t,size)<size ? -1 : 0 ;
 #endif
 #ifdef DOS
 unsigned bytes=0;
 _dos_read(h,t,size,&bytes);
 return bytes<size ? -1 : 0;
 #endif
}

int FILEIO::write(void *t,unsigned size)
{
 if (!h) return -1;
 #ifdef WIN95
 return ::write(h,t,size)<size ? -1 : 0;
 #endif
 #ifdef DOS
 unsigned bytes;
 return _dos_write(h,t,size,&bytes);
 #endif
}

unsigned FILEIO::size()
{
 return h ? filelength(h) : 0;
}

unsigned FILEIO::getpos()
{
 return h ? lseek(h,0,SEEK_CUR) : 0;
}

void FILEIO::setpos(unsigned p)
{
 if (h) lseek(h,p,SEEK_SET);
}





