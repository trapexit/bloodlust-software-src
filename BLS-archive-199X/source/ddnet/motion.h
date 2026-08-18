//defines a set of input motions that trigger a move in an objectdef
typedef struct motiondef
{
 uchar series;     //series num that this motion triggers (0 for blank)
 uchar parm;       //parameters for the motion
 uchar priority;   //priority of this motion
 char reserved[1];
 unsigned short pos[12]; //list of input positions

#ifdef ANIMATOR
 void Edit();
 void Draw(int x,int y);
#endif
} motiondef;

typedef struct motion //defines an active motion from an objectdef
{
 motiondef *mdef; //pointer to motion definition
 unsigned short *mptr; //current motionpos of this motion
 unsigned timeout; //time that this position will timeout
 uchar buttons;

 void reset();
 int  update(int instat); //update with a changed stat
}  motion;

//can be done during stance/walk/turnaround etc
#define MO_STANCE 1
//can be done during jump
#define MO_JUMP 2
//can be done during run
#define MO_RUN  4
//can be done as a combo move
#define MO_COMBO 8

