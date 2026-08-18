extern char *video;   // Pointer to video memory
extern char *screen; // Pointer to virtual screen

extern int mx,my,mb;
extern volatile int timerbusy;
extern volatile unsigned uu,su;

extern int SCREENX,SCREENY;

void mprintf(char *format, ...);
void setpalette(color *pal);

void _disable();
void _enable();

void quitgame();
void *loadresource(char *name);

