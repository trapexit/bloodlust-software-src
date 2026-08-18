#include <stdio.h>
#include <string.h>
#include "message.h"

#include "glib.h"
#include "dd.h"

extern FONT *font[];

inline void MESSAGE::draw(int x,int y)
{
 DrawCenteredString(font[color],screen,s,x,y);
}

inline void MESSAGE::set(char *m,int c)
{
 strcpy(s,m);
 color=c;
 timeout=uu+MSGTIMEOUT;
}

inline int MESSAGE::update()
{
// if (uu>timeout) return 1;
 return 0;
}

void msgbuffer::add(char *m,int color)
{
 msg[mtail].set(m,color);
 mtail++; mtail%=MAXMSG;
 if (mtail==mhead) {mhead++; mhead%=MAXMSG;}
}

void msgbuffer::printf(int color,char *format, ...)
{
 char s[200];
 char *args=(char *)&format+sizeof(format);
 vsprintf(s,format,args);
 add(s,color);
}


void msgbuffer::update()
{
 if (mtail!=mhead && msg[mhead].update())
  {mhead++; mhead%=MAXMSG;}
}

void msgbuffer::draw(int x,int y)
{
 update();
 for (int i=mhead; i!=mtail; i++,i%=MAXMSG,y+=10)
     msg[i].draw(x,y);
}










