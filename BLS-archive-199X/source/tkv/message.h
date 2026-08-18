#ifndef MESSAGE_H
#define MESSAGE_H

#define MAXMSG 1024

#include "uutimer.h"

struct MESSAGE {
 char color; //color of text
 char *s;

 MESSAGE() {s=0;}
 void draw(int x,int y);
 void drawpopup(int x,int y);
 void set(char *m,int c);
};


class msgbuffer
{
 //array of messages
MESSAGE  msg[MAXMSG];
char supress; //suppress messages?

public:
int num; //number of msgs in queue

int updated;

#ifdef WIN95
//listbox capturing
unsigned hwndmsg;
void setcapture(unsigned h);
void capturemsg(char *m);
#endif


//persistant message
MESSAGE *lastmessage;
uutimer lmtimer;
void setlastmessage(MESSAGE *m);
void drawlastmessage();
void clearlastmessage() {lastmessage=0;}


void setsuppress(char x) {supress=x;}

msgbuffer() {num=0; updated=0; supress=0; lastmessage=0;}
virtual void add(char *m,int color=0);
void __cdecl printf(char *format, ...);
void __cdecl cprintf(int color,char *format, ...);
void __cdecl error(char *format,...);
void draw(int x,int y,int first,int numtodraw);
void dump(char *filename);
void msgbuffer::drawlast(int x,int y,int n) {draw(x,y,num-n,n);}

};

extern msgbuffer msg;


class msgbufferwrap:public msgbuffer
{
 int width;
 public:
 msgbufferwrap(int _width):width(_width) {}

 virtual void add(char *m,int color=0); //split up message based on width
};

extern msgbufferwrap netchat;



#endif








