#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "glib.h"
#include "dd.h"
#include "keyb.h"

extern FONT *font[];

#include "editctrl.h"


editcontrol::editcontrol(char *p)
{
 strcpy(prefix,p);
 s[0]=0;
 slen=0;
}

void editcontrol::draw(int x,int y,int prefontnum,int fontnum)
{
// dprintf(font[fontnum],x,y,"%s%s%c",prefix,s,(uu&32)?'_':' ');
 DrawString(font[prefontnum],screen,prefix,x,y);
 x+=GetStringWidth(font[prefontnum],prefix);
 dprintf(font[fontnum],x,y,"%s%c",s,(uu&32)?'_':' ');
}


void editcontrol::addchar(char c)
{
 s[slen++]=c;
 s[slen]=0;
}

void editcontrol::backspace()
{
 if (slen>0) slen--;
 s[slen]=0;
}

int editcontrol::processkey(char kbscan)
{
 if (kbscan==0xE)  {backspace(); return 0;} //backspace
 if (kbscan==0x1C) {enter(); return 1;}  //enter
 if (kbscan==1)    {cancel(); return 1;}//esc

 char key=scan2ascii(kbscan);
 if (isvalidkey(key)) addchar(key);
 return 0;
}

int editcontrol::isvalidkey(char key)
{
 return isprint(key);
}









