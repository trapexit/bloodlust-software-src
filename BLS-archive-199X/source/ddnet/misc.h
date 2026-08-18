extern char errstr[80];
void cleanexit(int x);
int query(char *format,...);
int dselprintf(int x,int y,char *format,...);
void wait();

extern volatile unsigned uu;
extern volatile int quit,timerbusy;

#ifndef MESSAGE_H
 #include "message.h"
#endif
extern msgbuffer msg;

