//routines for configuration file
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

#include "glib.h"
#include "input.h"
#include "config.h"


config *new_config()
{
config *t=(config *)malloc(sizeof(config));
memset(t,0,sizeof(config));

t->version=CONFIGVERSION;
return t;
}

config *load_config(char *file)
{

 int h;
 unsigned bytes;
 if (_dos_open(file,O_BINARY|O_RDONLY,&h)) return 0;

 config *t=(config *)malloc(sizeof(config));
 if (!t) return 0;
 memset(t,0,sizeof(config));

 _dos_read(h,t,sizeof(config),&bytes);
 _dos_close(h);
 
 return(t); 
}    


void save_config(char *file,config *t)
{
 int h; unsigned bytes;
 _dos_creat(file,0,&h);

 _dos_write(h,t,sizeof(config),&bytes);
 _dos_close(h);
}    
