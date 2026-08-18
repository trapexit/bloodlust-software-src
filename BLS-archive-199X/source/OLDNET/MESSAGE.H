#ifndef MESSAGE_H
#define MESSAGE_H

#define MAXMSG 256
#define MSGTIMEOUT 500

struct MESSAGE {
 char color; //color of text
 char *s;

 MESSAGE() {s=0;}
 void draw(int x,int y);
 void set(char *m,int c);
};


class msgbuffer
{
 //array of messages
MESSAGE  msg[MAXMSG];

public:
int num; //number of msgs in queue

int updated;

msgbuffer() {num=0; updated=0;}
void add(char *m,int color=0);
void __cdecl printf(int color,char *format, ...);
void __cdecl error(char *format,...);
void draw(int x,int y,int first,int numtodraw);
};

extern msgbuffer msg;



#endif








