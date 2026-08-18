class timer
{
private:
TIMER *prev,*next; //linked list

char enabled; //is the timer enabled? default is enabled
int difcnt; //number of times this timer should be called
            //for every 
int cnt;    //fractional counter maintained to tell when
            //this timer should be called
void updatecnt(int irqfreq);            
    
int freq;     //frequency of the timer (ticks/sec)
void (far *func)(); //function to be called each tick

//hardware timer functions
static void IRQfreq(int x); //set frequency
static void IRQinit();  //initialize irq vector
static void IRQterminate(); //removes irq vector
static void interrupt __loadds TIMERIRQ();
static char IRQinstalled; //is the hardware irq installed?

static void updateIRQ(); //updates IRQ as necessary

static timer *tlist; //linked list of all timers
 
public:
timer(int f,void (far *tfunc)()); //constructor
~timer();   

void setfreq(int f);
void setfunc(void (far *tfunc)());

void enable() {enabled=1;};
void disable() {enabled=0;};

void tick();
};


extern timer *biostimer; //bios timer


