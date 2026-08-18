void init_keyboard();
void terminate_keyboard();
void set_keyboard_func(int (*kfunc)());

//whether or not a keybint is in use...
extern volatile char kbint;

//last kbscan read
extern volatile char kbscan;

#define KB_SHIFT 1
#define KB_CTRL  2
#define KB_ALT   4
//status of the control/alt/shift keys
extern volatile char kbstat;

//list of all keys
extern volatile char keydown[128];

//whether or not to store keyscans in local queue
extern int keyqueue;
//whether or not to call bios instead
extern int keybios;
//whether or not to allow repeating
extern int keyrepeat;

void pushkey(char kbscan);
char waitkey();
char getkey();
int keyhit();

char scan2ascii(char s);


