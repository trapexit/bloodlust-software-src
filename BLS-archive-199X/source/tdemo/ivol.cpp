#include <direct.h>
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <graph.h>

unsigned total=0,filesnumber=0;;
find_t ff;

char s[30];

void main()
{

int h;
unsigned bytes;


printf("enter dir to volumize: ");
scanf("%s",s);


int vh;
printf("creating volume file...\n");
_dos_creat("C:\\INSTALL.COW",0,&vh);

if (chdir(s)) return;


printf("volumizing %s...\n",s);


if (_dos_findfirst("*.*",_A_NORMAL,&ff)) exit(1);


char key[16]="MIDGETPOWER";
do
{
memcpy(s,ff.name,12);
s[12]=0;

if (_dos_open(s,O_BINARY | O_RDONLY,&h)) continue;
unsigned size=filelength(h);
char *t=(char *)malloc(size);


cprintf("read: %12s size: %8d ",s,size);
_dos_read(h,t,size,&bytes);
_dos_close(h);
total+=size;
filesnumber++;

cprintf("writing.. ");
_dos_write(vh,key,16,&bytes);
_dos_write(vh,ff.name,12,&bytes);
_dos_write(vh,&size,4,&bytes);
_dos_write(vh,t,size,&bytes);
cprintf("done.");
free(t);

printf("freed.\n");

} while (!_dos_findnext(&ff)); 
_dos_close(vh);
printf("\n");
printf("%d bytes written\n",total);
printf("%d files\n",filesnumber);
printf("done.\n");



}
