#ifndef MESSAGE_H
#define MESSAGE_H

#define MAXMSG 8
#define MSGTIMEOUT 500

struct MESSAGE {
 unsigned timeout; //time that this message will timeout
 char color; //color of text
 char s[80]; //actual message string
 inline void draw(int x,int y);
 inline void set(char *m,int c);
 inline int update();
};


class msgbuffer
{
 //array of messages
MESSAGE  msg[MAXMSG];
public:

int mhead,mtail; //head/tail of queue

msgbuffer() {mhead=mtail=0;};
void add(char *m,int color=0);
void printf(int color,char *format, ...);
void draw(int x,int y);
void update();
};

extern msgbuffer msg;
#endif








