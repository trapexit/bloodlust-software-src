#include "volbatch.h"

//data from a95 vol
struct A95VOL:volbatch
{
 struct PALETTE *pal; //default palette
 struct IMG *cursor; //cursor
 struct FONT *font; //first font
 struct IMG *checkmark; //checkmark
 struct IMG *umark,*dmark,*lmark,*rmark; //arrows for scroll bar
 struct IMG *about; //buddy's head
 struct IMG *shadowsel; //dumb shadow

 struct IMG *play,*stop,*playlooped,*active; //control icons

 struct IMG *xmark;

 virtual int size();
};
extern A95VOL a95vol;

extern struct IMG *shadow;



