#ifndef _UUTIMER_
#define _UUTIMER_
extern volatile unsigned ticks;
//cheap timer
class uutimer
{
 public:
 unsigned x;
 unsigned dur; //duration of timer

 void clear() {dur=0; x=0xFFFFFFF;}
 void set(unsigned tdur) {dur=tdur; x=ticks+dur;}
 void reset() {set(dur);}
 int check() {return ticks>=x;}
 uutimer() {clear();}
 uutimer(unsigned tdur) {set(tdur);}
};
#endif

