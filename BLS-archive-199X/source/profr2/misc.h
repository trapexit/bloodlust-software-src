void mprintf(char *format,...);
extern char errstr[80];
void cleanexit(int x);
int query(char *format,...);
int dselprintf(int x,int y,char *format,...);
void wait();

extern volatile unsigned uu;
extern volatile int quit,timerbusy;

extern char msg[100];
extern int msgdur;

