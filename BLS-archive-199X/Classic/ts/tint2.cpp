        
#include "tgraph.h"
#include "tinput.h"
#include "time.h"
#include "tpacket.h"


#define LB 40
#define RB 280

#define TIMETICK 50
#define ED 80

#define HOLDDUR 15

#define MAXMARKS 20s

#define INITIALDIZZY 15
#define DIZZYMAX     40
#define DIZZYMIN     -30


#define HHMIN   60
#define HHMAX   105
#define HHINC   28

#define BL 130
#define BH 9

#define B1 15
#define B2 173

#define abs(x) ((x)>=0 ? (x) : -(x))


extern int bloodlevel;
extern volatile unsigned int uu;
extern unsigned long setime;
extern int contdisp;     //disp within voc
extern int speedup,fastp1,fastp2;
extern int sepriority;
extern playerstat p[2];
extern volatile int pause,quit;
extern int region,musicstat;
extern int scrolling;
extern int vertscroll;
extern int airmove,supermove;
extern int slide;
extern int statbar;
extern int screensave;
extern int battle;
extern int gshakedur,gshake,gstemp;

extern int  msgnum;

extern int drawpriority,scrdone;
extern int newframe,headprojectile,offscreen;
extern int rndcount[2];
extern int orndcount[2];
extern int stopdur;           //Tell to stop after being hit
extern int slowdown,death;

extern int timeleft;
extern int timetickdur;

extern int gamestat;
extern int fadelevel,oldfadelevel;
extern int fontdur;
extern int rndnumber;
extern int fonttype;
extern int cbsize[2];  //Current length of bar
extern int nbsize[2];  //Needed length of bar (for incremental decreasing when hit) (timer)
extern char postwordwidth;
extern int postse;
extern int postwordtype;
extern long  prewordbaseptr;
extern SOUND postvocname;
extern SOUND sysse[];
extern int MAXENERGY;
extern effect effects[];
//extern int ebasey[];
extern unsigned char *eframe;        //Array of frames
extern move elist[];
int hithurt[9]=  //Amount successive hits take off
 {
        95,
        85,
        80,
        65,
        60,
        60,
        50,
        50,
        50,
 };


void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p);
void PlaySysSE(char se, int xc);
void PlaySE(unsigned char se,char type,char pl,int xc); //type 0-swish 1-hit
int GetRnd(int x);

// Able to do moves, walk, etc
// cm<8 ||  cm>=24 && cm<=26 || cm==48 || cm==49 || cm>=56 && cm<=59
#define CONTROL 1
// In air y<basey
#define AIR 2
// 1-In stance 0-In crouch
#define STANCE 4
// 1-blocking 0-not blocking
#define BLOCK 8
// If hit (in pain or block)
#define BEENHIT 16
// If we need to push other player backwards
#define PUSHOPB 32
// If we are doing an attacking move that needs to be blocked
#define ATTACKING 64
// If we need to get up after this move
#define NEEDGETUP 256
// If attack has been done in current frame
#define DONEATTACK 128

extern int z;
extern playerstat *pz;

void playerstat::GetRect()
{
register image *i=(image  *) (frameptr+sizeof(frametype));

while (i->index!=0xFF) i++;

vrectptr=((unsigned char *)i)+1; //Point to first count of v rects
arectptr=vrectptr+ ((*vrectptr)*sizeof(rect))+1;
}

int tcm[2];


void AttackCheck()
{
int a,b;
int i,j;

rect *arptr;
rect *vrptr;

rect ar,vr;


newframe=0;

//If noone attacking... bye bye
if ( !(*(p[0].arectptr)) && !(*(p[1].arectptr)) ) return;

//p[0].GetRect();
//p[1].GetRect();


tcm[0]=p[0].cm;
tcm[1]=p[1].cm;


for (a=0; a<2; a++)
 if (p[a^1].ctype!=2)
 {
  b=a^1;

  playerstat *pa=&p[a];
  playerstat *pb=&p[b];


  i=(int)(unsigned char) *(pa->arectptr); //Get rect counts
  j=(int)(unsigned char) *(pb->vrectptr);

//  if (i>5) i=0; if (j>5) j=0;
//  if (i>5) {i=0;quit=1; return;}
//  if (j>5) {j=0; quit=1; return;}

  //Check for a attacking and b defending
  if ( (!i || !j || pa->stat&DONEATTACK)) continue;


  arptr=(rect *) (pa->arectptr+1);

  for (; i>0; i--)
  {
        //Calc attack rect

        ar.y1=arptr->y1+pa->y; ar.y2=arptr->y2+pa->y;
        if(!pa->d)
          {ar.x1=pa->x+arptr->x1; ar.x2=pa->x+arptr->x2;} else
          {ar.x1=pa->x-arptr->x2; ar.x2=pa->x-arptr->x1;}

        vrptr=(rect *) (pb->vrectptr+1);
        for (; j>0; j--)
         {
          //Calc vuln rect
          vr.y1=vrptr->y1+pb->y; vr.y2=vrptr->y2+pb->y;
          if(!pb->d)
                {vr.x1=pb->x+vrptr->x1; vr.x2=pb->x+vrptr->x2;} else
                {vr.x1=pb->x-vrptr->x2; vr.x2=pb->x-vrptr->x1;}

          //See if rect conflict
          if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) && ( !(pa->stat&DONEATTACK)) )
                {
                int m=((frametype *)pa->frameptr)->parm;

                //check for OK attack
                if   ( (m&(128+64+32))==(128+64+32)    ||
                 (
                  (m&128 || !(pb->y<basey) ) &&
                  (m&64  || !(pb->stat&STANCE) ) &&
                  (m&32  ||   pb->stat&STANCE)    )  )
                 {
//LET'S ATTACK HIM!!!!!!!!!!!!!-----------------------------------
        stopdur=20;

        pb->newmove=1;
        pa->dur++;

        pb->wallsound=0; //Reset wallsound

        int xmark=((ar.x1>vr.x1 ? ar.x1 : vr.x1)+(ar.x2<vr.x2 ? ar.x2 : vr.x2))/2;
        int ymark=((ar.y1>vr.y1 ? ar.y1 : vr.y1)+(ar.y2<vr.y2 ? ar.y2 : vr.y2))/2;


        //Hit

         //Determine fierceness of move
        tcm[b]=2;
          if (pa->cm>=27 && pa->cm<=32) tcm[b]=(pa->cm%3);  //Store fierceness of move
         else
          if (pa->cm<66) tcm[b]=((pa->cm-8)%3);   //Store fierceness of move
         else
          if (pa->cm<78) tcm[b]=((pa->cm-66)%3);   //Store fierceness of move

//       else
//        if ( (pa->cm|1)==87) tcm[b]=2;

        pb->throwvuln=200;

         //Determine b's pain move
         pb->shakedur=6+7*tcm[b];
         stopdur=5+8*tcm[b];
         int mark=tcm[b];

         z=b;
//       if (pb->NeedBlock() && (pb->stat&CONTROL) && !(pb->stat&AIR)) pb->stat|=BLOCK;// else pause=1;
         if ((pb->in.stat&1) && (pb->stat&CONTROL) && !(pb->stat&AIR)) pb->stat|=BLOCK;// else pause=1;

         int elose=0;

         if (m&16 && (pb->stat&BLOCK && pb->stat&STANCE) ) //If he blocks high
                 {pb->stat|=PUSHOPB; tcm[b]+=60;
                  PlaySysSE(0,xmark); mark+=3;
                  if (pa->cm>=78) elose=((frametype *)pa->frameptr)->energy/16+1;
                  if (m&2048 && !(pa->stat&BEENHIT)) {tcm[a]=100; pa->newmove=1; pa->jumpptr=0;}
                 }

                else
         if (m&8 &&  (pb->stat&BLOCK && !(pb->stat&STANCE)) ) //If he blocks low
                 {pb->stat|=PUSHOPB; tcm[b]+=63;
                  PlaySysSE(0,xmark); mark+=3;
                  if (pa->cm>=78) elose=((frametype *)pa->frameptr)->energy/16+1;
                  if (m&2048 && !(pa->stat&BEENHIT) ) {tcm[a]=100; pa->newmove=1; pa->jumpptr=0;}
                 }
                else
          {
                mark+=0; //hitmark
                pb->blockholddelay=0;
                elose=((frametype *)pa->frameptr)->energy;

                int diz=tcm[b]*5+5;

                if (pa->cm>=78) pb->stat|=STANCE;

                //Do successive hits
                if (!pa->fatality)
                {
                elose=elose*hithurt[pa->hitcnt[pa->cm]]/100;
                if (pa->hitcnt[pa->cm]<8) pa->hitcnt[pa->cm]++;

                if (elose<diz) diz=elose; if (!diz) diz=1;
                elose=elose* (pb->energy+ED)/ ((MAXENERGY)+ED);
                if (!elose) elose++;
                }

                pb->dizzy +=diz;
                if (pa->dizzy>0) pa->dizzy -=diz/2;


                PlaySE( ((frametype *)pa->frameptr)->se,1,a,xmark );
                switch(m&3) //He's hit
                {
                  case 0: pb->stat|=PUSHOPB; if (pb->stat&STANCE) tcm[b]+=36; else tcm[b]+=39; break; //Low pain
                  case 1: pb->stat|=PUSHOPB; if (pb->stat&STANCE) tcm[b]+=33; else tcm[b]+=39; break; //High pain
                  case 2: pb->stat&=~PUSHOPB; tcm[b]=43; pb->wallsound=1;if (pa->fatality) {pa->fatality=0; pb->fatality=3;} pb->stat|=NEEDGETUP; pb->shakedur=0;break; //Trip
                  case 3: pb->stat&=~PUSHOPB; tcm[b]=42; pb->wallsound=1;if (pa->fatality) {pa->fatality=0; pb->fatality=3;} pb->stat|=NEEDGETUP; pb->shakedur=0;break; //Fall down
                }
                pb->stat&=~BLOCK;

                if (m&1024 && !(pa->stat&BEENHIT)) {tcm[a]=100;pa->newmove=1;pa->jumpptr=0;}
          }

        if (timeleft)
        {
        pb->energy-=elose;
        nbsize[b]=pb->energy*BL/MAXENERGY;
        if (nbsize[b]<0) nbsize[b]=0;
        if (pb->energy<=0)
                {
                 pb->stat&=~NEEDGETUP;
                 pb->energy=0; if (mark>=3) mark-=3;
                 if (!pb->fatality)
                  {
                 //     if (pb->eattach.e && pb->eattach.en==21) pb->eattach.e=0;
                        tcm[b]=91; pb->dizzydur=0; pa->dizzydur=0; pb->dizzy=pa->dizzy=0;
                        slowdown=1000; pb->stat&=~PUSHOPB; pb->wallsound=1;
                        if (pa->cm!=3) pa->cm=0; pa->stat&=~CONTROL;  pb->shakedur=0;

                        //pa->dizzy=60;
                        //if (pb->fatality) {pb->fatality=3; pa->fatality=0; slowdown=0; }
                  }  //else pause=1;
                }
         if (pb->pain>=0) pb->pain=(MAXENERGY-pb->energy)/ (MAXENERGY/4);
        }


        pb->stat&=~(CONTROL+ATTACKING+DONEATTACK); //Lose control & turnaround
        pb->stat|=BEENHIT;
        drawpriority=a;

        //GENERATE HITMARKS---------------------------------

        if ( !(mark>=3 && mark<=5))
         if ((m&256  &&  !(m&512)) )
         {
          unsigned char *t=pa->arectptr+ ((*pa->arectptr)*sizeof(rect))+1;

          if ( ((*t)&15)>=GetRnd(16) ) //Do we need to generate it?
          {
                int etype=( ((*t)>>4)  );

                pa->PEffect(etype,xmark,ymark);
                GenerateEffect(22 +mark,xmark+  (pa->d ? -10 : 10),ymark,0,0 ,pa->d,0);

                //GenerateEffect(7+etype ,xmark,ymark,0,0 ,pa->d,0);
                //Falling drips
                for (int l=mark*bloodlevel+2; l>0; l--)
                 GenerateEffect(16,xmark,ymark,GetRnd(20)-10,GetRnd(10)-5,pa->d,GetRnd(2));
//              for (l=mark*1*bloodlevel+2; l>0; l--)
//               GenerateEffect(16,xmark,ymark,GetRnd(20)-10,GetRnd(10)-5,pa->d^1,GetRnd(2));
                GenerateEffect(18 ,xmark,ymark,0,0 ,pa->d,0);
                //if ( (m&3)==1 )
          }
         } else
          {
                if (!pb->energy) mark=2;
         GenerateEffect(22+mark ,xmark+ (pa->d ? -10 : 10),ymark,0,0 ,pa->d,0);

                GenerateEffect(7+mark ,xmark,ymark,0,0 ,pa->d,0);
                for (int l=mark*bloodlevel+1; l>0; l--)
                 GenerateEffect(16,xmark,ymark,GetRnd(20)-10,GetRnd(10)-5 ,pa->d,GetRnd(2));
                if (!pb->energy) GenerateEffect(18 ,xmark,ymark,0,0 ,pa->d,0);
                //if ( (m&3)==1 )

          }
        GenerateEffect(mark,xmark,ymark,0,0,pa->d,0);


        //If opponent is in the air
        if (basey-pb->y>0 && pb->energy  && tcm[b]!=43)
          {
                if (!(m&4)) {tcm[b]=45; pb->stat&=~NEEDGETUP;  pb->shakedur=0;}  //Jumping pain
                 else   { tcm[b]=42; pb->wallsound=1;pb->stat|=NEEDGETUP; pb->shakedur=0;} //Jumping dead
                pb->stat&=~PUSHOPB;
          }

          //Remove dizzy from opponent
        if (!pa->fatality)
                {pb->dizzydur=0;if (pb->eattach.e && pb->eattach.en==21) pb->eattach.e=0; if (death) pa->numjoymoves=pa->numjm;}

        if ((!pb->d && pb->x>pa->x) || (pb->d && pb->x<pa->x) )
         {pb->d=pa->d^1;   if (pb->in.stat&3) pb->in.stat^=3; }


        if (!(pa->stat&BEENHIT)) pa->stat|=DONEATTACK;
        pb->jumpptr=0;

        if (pb->dizzy>DIZZYMAX)   //See if guy's hit to dizzy
                 if (!pb->fatality)
                  {           //Make 'em dizzy
                        if (tcm[b]!=43)tcm[b]=42;
                        pb->stat&=~(PUSHOPB+BLOCK+CONTROL);
                        pb->stat|=STANCE+NEEDGETUP; pb->dizzydur=50+pb->energy*280/MAXENERGY; //pb->dizzy=0;
                        pb->dizzy=DIZZYMIN;  pb->shakedur=0; if (death) pa->numjoymoves=pa->numfm;
                  }
                else          //Fatality
                  if (tcm[b]>=33 && tcm[b]<=35) //If high pain
                {tcm[b]=99; pb->fatality=3; pa->fatality=0; slowdown=0; stopdur=0; pb->dizzy=0; pb->dizzydur=0;}
                else { tcm[b]=91; pb->dizzy=0; pb->dizzydur=0; pb->stat&=~(PUSHOPB+BLOCK+CONTROL); slowdown=0; pb->wallsound=1; pb->fatality=3; pa->fatality=0;}

        break;
        }



                }

          if (pa->stat&DONEATTACK) break;
          vrptr++;
         }
        if (pa->stat&DONEATTACK) break;
        arptr++;
  }
 }

 p[0].cm=tcm[0]; if (p[0].newmove && p[0].stat&BEENHIT) p[0].frameptr=p[0].frame+p[0].moves[tcm[0]].firstframe;
 p[1].cm=tcm[1]; if (p[1].newmove && p[1].stat&BEENHIT) p[1].frameptr=p[1].frame+p[1].moves[tcm[1]].firstframe;


}


void playerstat::Move()
{
 if (!jumpptr)
  {
        if (!d) tcountx+=*((short int *)&((frametype *)frameptr)->dx);
         else   tcountx-=*((short int *)&((frametype *)frameptr)->dx);
        tcounty+=*((short int *)&((frametype *)frameptr)->tx);
  }
 else
  {
        if (!d) tcountx+=*((short int *)&((frametype *)jumpptr)->dx)+*((short *)&((frametype *)frameptr)->dx);
         else   tcountx-=*((short int *)&((frametype *)jumpptr)->dx)+*((short *)&((frametype *)frameptr)->dx);
        tcounty+=*((short int *)&((frametype *)jumpptr)->tx)+*((short int *)&((frametype *)frameptr)->tx);
  }

 int dx=tcountx>>8; tcountx&=0xFF;
 int dy=tcounty>>8; tcounty&=0xFF;

 if (speedup) {dx<<=1; dy<<=1;}

 if (dx|dy)  newframe=1;

 y+=dy;

 //Check for location conflict  42 45
 if ((abs(pz->x-(x+dx))<32 && abs(pz->y-(y+dy))<35) && !throwstat && !offscreen)
  {
        int td=d;
        if (x>pz->x) td=1;
        if (x<pz->x) td=0;


        //Make players move away from each other
        if (!dx) if (td) dx+=4; else dx-=4;

        if ((td && dx<0) || (!td && dx>0) )
          {
                x+=dx/2;        //Move player
                pz->x+=dx; //Push other player

                //If we are pushing other guy off the screen..
                if (pz->x<LB)  { pz->x=LB;  x-=dx; if (x<LB+32) x+=2;}
                if (pz->x>RB)  { pz->x=RB;  x-=dx; if (x>RB-32) x-=2;}
          }  else   x+=dx;        //Move player

  if (x<LB) x=LB;
  if (x>RB) x=RB;

  } else

  //Just move ourselves only
         {
          x+=dx;
         if (!offscreen)
         {
          if (x<LB) {x=LB; if (stat&PUSHOPB) pz->x-=dx; if (wallsound) {PlaySysSE(2,x);wallsound=0;}}
          if (x>RB) {x=RB; if (stat&PUSHOPB) pz->x-=dx; if (wallsound) {PlaySysSE(2,x);wallsound=0;}}
         } else
          {
                if (x<-10) x=330;
                if (x>330) x=-10;

          }

    }

}


int playerstat::NeedBlock()
{
/*
 if (ctype==3 && !blockholddelay)
  {
   in.stat=0;

   if (!GetRnd(10)) in.stat=1;

  }*/


 if (! (in.stat & 1)) {blockholddelay=0;/*stat&=~BLOCK;*/return(0);}

 if ( ((pz->stat&ATTACKING) && (abs(x-pz->x)<180) && !(pz->stat&DONEATTACK))  || pz->pexist)
  {
          blockholddelay=30;
          return(1);
 }

 if (blockholddelay>0) return(1);


 return(0);
}

int specialmove::checkpos(int a,int b,int charge)
{
 if (!charge)
  {
   if (a&(16+32))
    {if ( (a&63)== (b|bhold) ) return(1);}    //If need us to push buttons
          else
    {
          if (curstat&128) {bhold|=b&(16+32); }   //Save buttons pushed BPON
          if ((a&15)==(b&15)) return(1);
    }
  }//If charging
  else {bhold=0; if (a&b&63) return(1); }

 return(0);

}

int specialmove::Check(unsigned char s)
{
 if (!jpp)  //We haven't even done 1st position yet
 {
  jpmax=1; curstat=0; bhold=0;
  if (checkpos(jp[0].pos,s,jp[0].min))  //If we moved to 1st pos
          {
      jpp=&jp[1];        //Point to next position required
                jpmin=uu+jp[0].min;    //timeleft that this pos must be held
                if (!jp[0].min) jpmax=uu+jp[0].max; else jpmax=0;   //timeleft that next pos must be achieved
      curstat= jp[0].pos; //Store current status
     }
 }
  else
 {
   if (checkpos(curstat,s,(jpp-1)->min)) //If we are still in this position
                { if (jpmax && jpmax<=(int)uu) jpp=0;  } //We failed over max
        else   //We changed positions
    {
                if (checkpos(jpp->pos,s,jpp->min)) //If we are at next pos

         if (jpmin<=(int)uu)  //If we are over minimum
           {  //Next pos
            jpmin=uu+jpp->min;    //timeleft that this pos must be held
                 if (!jpp->min) jpmax=uu+jpp->max; else jpmax=0;   //timeleft that next pos must be achieved
            curstat= jpp->pos; //Store current status

                 jpp++; if (!jpp->max) {jpp=0; bhold=0; return(1);}  //We finished
           } else jpp=0; //If we are not over minimum then we failed

      else //We moved somewhere else other than required
      {
                 if (!jpmax) jpmax=uu+(jpp-1)->max;

                 if ((curstat&64)) jpp=0; //We moved somewere else and violated direct
        else if (jpmax<=(int)uu)  jpp=0; //Else if over timeleft
      }
         }

 }

return(0);
}

void playerstat::PEffect(int type, int ex, int ey)
{

int j;
switch(type)
{
case 6:  //Projectile
case 7:
                {
                 if (type&1 && cm>97)  //Make bloody effect
                 {
                  eattach.e=elist[20].firstframe+eframe; eattach.en=20; eattach.d=d;
                  eattach.currframe=0; eattach.numframes=elist[20].numframes;
                  eattach.x=ex-x; eattach.y=ey-y;
                  eattach.dur=((frametype *)eattach.e)->dur;
                  eattach.tcx=eattach.tcy=0; eattach.maxy=300;

                 }

                 if (headprojectile) type=1;

                 for (projectilestat *pp=ps; pp<&ps[PMAX]; pp++)
                  if (!pp->pframeptr)
                        {
                         pp->pframeptr   =&frame[moves[20+(type&1)*2].firstframe];
                         pp->pnumframes  =moves[20+(type&1)*2].numframes; pp->ptype=type&1;
                         pp->pcurrframe  =0;
                         pp->pdur=((frametype *)pp->pframeptr)->dur;
                         if (!throwstat) pp->pd=d; else pp->pd=d^1;
                         pp->pstat=1; 
                         pp->px=ex;  pp->py=ey;
                         {
                          register image *i=(image *) (pp->pframeptr+sizeof(frametype));
                          while (i->index!=0xFF) i++;
                          pp->parectptr=(rect *)(((unsigned char *)i)+3); //Skip Vuln and attack
                         }
                         break;
                        }
                }
                break;

case 4: //OIL
        //GenerateEffect(20,ex-x,ey-y,0,0,d,0);
        //slowdown=1000;

                  eattach.e=eframe+elist[20].firstframe; eattach.en=20; eattach.d=d;
                  eattach.currframe=0; eattach.numframes=elist[20].numframes;
                  eattach.x=ex-x; eattach.y=ey-y;
                  eattach.dur=((frametype *)eattach.e)->dur;
                  eattach.tcx=eattach.tcy=0; eattach.maxy=300;

/*      eattach.e=elist[20]; eattach.en=20; eattach.d=d;
        eattach.pos.x=ex-x; eattach.pos.y=ey-y;
        eattach.dur=elist[20]->dur;

        eattach.dx=0; eattach.tcx=eattach.tcy=0; eattach.maxy=300;
  */
        break;

case 1: //Dizzyness
        if (!eattach.e)
        {
/*      eattach.e=elist[21]; eattach.en=21; eattach.d=d;
        eattach.pos.x=ex-x; eattach.pos.y=ey-y;
        eattach.dur=elist[21]->dur;
        eattach.dx=0; eattach.tcx=eattach.tcy=0; eattach.maxy=300;
  */
                  eattach.e=eframe+elist[21].firstframe; eattach.en=21; eattach.d=d;
                  eattach.currframe=0; eattach.numframes=elist[21].numframes;
                  eattach.x=ex-x; eattach.y=ey-y;
                  eattach.dur=((frametype *)eattach.e)->dur;
                  eattach.tcx=eattach.tcy=0; eattach.maxy=300;

        }
        break;

                 //FLOOD
case 3: GenerateEffect(19,ex,ey,0,0,d,1);
                 break;

                 //Blood
case 2: GenerateEffect(18,ex,ey,0,0,d,0);
                 for (j=0; j<5*bloodlevel; j++)
        {
         GenerateEffect(16,ex,ey-15,GetRnd(30),GetRnd(30),0,GetRnd(2));
         GenerateEffect(16,ex,ey-15,-GetRnd(30),GetRnd(30),1,GetRnd(2));
        }
                 break;

                 //Blood
case 8: GenerateEffect(25,ex,ey,0,0,d,0);
                 for (j=0; j<2*bloodlevel; j++)
        {
         GenerateEffect(16,ex,ey-15,GetRnd(30),GetRnd(30),0,GetRnd(2));
         GenerateEffect(16,ex,ey-15,-GetRnd(30),GetRnd(30),1,GetRnd(2));
        }
                 break;

                 //SPEW
case 5: GenerateEffect(12,ex,ey,0,0,d,0);
                 break;

                 //TELEPORT
case 9:
        if (!d) if (pz->x<265) x=pz->x+75; else x=pz->x-75;
          else  if (pz->x>75)  x=pz->x-75; else x=pz->x+75;

         //Turn around
         if ( ((d && x<pz->x-20) || (!d && x>pz->x+20)) && (!(stat&BLOCK) || pz->y==basey) )
         { if (! (in.stat&8)) cm=48; else cm=49;  d^=1;if (in.stat&3) in.stat^=3;  }

                 break;

//case 10:      //freeze
//        pz->cm=101; pz->newmove=1;
//        break;

case 10:
   GenerateEffect(28,ex,ey,0,0,d,0);
 break;        


case 0: gshakedur=50;
        break;



}

}


void  projectilestat::AdvanceProjectile()
{
 if (!pframeptr) return;


 playerstat *pa=&p[z];
 playerstat *pb=&p[z^1];

 //See if hitting opponent
 if (pstat==1 && (ptype==0 || headprojectile) && (pz->energy || pz->fatality==2))
  {
         rect ar,vr;

         int j=(int) *(pb->vrectptr);


         ar.y1=parectptr->y1+py;
         ar.y2=parectptr->y2+py;
         if(!pd)
          {ar.x1=px+parectptr->x1; ar.x2=px+parectptr->x2;} else
          {ar.x1=px-parectptr->x2; ar.x2=px-parectptr->x1;}


         rect *vrptr=(rect *) (pb->vrectptr+1);
         for (; j>0; j--)
         {
          //Calc vuln rect
          vr.y1=vrptr->y1+pb->y;
          vr.y2=vrptr->y2+pb->y;
          if(!pb->d)
                {vr.x1=pb->x+vrptr->x1; vr.x2=pb->x+vrptr->x2;} else
                {vr.x1=pb->x-vrptr->x2; vr.x2=pb->x-vrptr->x1;}

          //See if rect conflict
          if (!(ar.x1>vr.x2 || ar.x2<vr.x1 || ar.y1>vr.y2 || ar.y2<vr.y1) )
                {
                int m=((frametype *)pframeptr)->parm;

                //check for OK attack
                if   ( (m&(128+64+32))==(128+64+32)    ||
                 (
                  (m&128 || !(pb->y<basey) ) &&
                  (m&64  || !(pb->stat&STANCE) ) &&
                  (m&32  ||   pb->stat&STANCE)    )  )
                 {
//      stopdur=5;

        pb->newmove=1;
        pb->wallsound=0;

        //Determine b's pain move
        pb->shakedur=6+5*2;
        pb->throwvuln=200;

                 /*     z^=1;
        if (pb->NeedBlock() && pb->stat&CONTROL && !(pb->stat&AIR)) pb->stat|=BLOCK;
        z^=1;*/
         if ((pb->in.stat&1) && (pb->stat&CONTROL) && !(pb->stat&AIR)) pb->stat|=BLOCK;// else pause=1;

        int elose;

        if (m&16 && ((pb->stat&BLOCK) && pb->stat&STANCE) ) //If he blocks high
                 { pb->cm=62; PlaySysSE(0,px); elose=((frametype *)pframeptr)->energy/16+1;}
                else
        if (m&8 && ((pb->stat&BLOCK) && !(pb->stat&STANCE)) ) //If he blocks low
                 { pb->cm=65; PlaySysSE(0,px); elose=((frametype *)pframeptr)->energy/16+1; }
                else
          {
                elose=((frametype *)pframeptr)->energy;

                if (!pa->fatality)
                {
                //Do successive hits
                elose=elose*hithurt[pa->hitcnt[20]]/100;
                if (pa->hitcnt[20]<8) pa->hitcnt[20]++;

                pb->dizzy+=elose;

                elose=elose* (pb->energy+ED)/ ((MAXENERGY)+ED);

                if (!elose) elose++;
                }

                slowdown=90;
                pb->blockholddelay=0;
                PlaySE( ((frametype *)pframeptr)->se,1,z,px );

                pa->PEffect(8,px,py);

                if (m&1024)
                        {
                         if (pb->cm==101 || pb->dizzydur)
                 {
                  if (pb->y<basey) pb->cm=42;
                          else   pb->cm=42;
                  pb->wallsound=1; pb->stat|=NEEDGETUP;
                  pb->dizzydur=0; pb->dizzy=0;
                 }
                         else {pb->cm=101; pb->shakedur=55; pb->stat&=~BEENHIT; }
                         elose=0;
                        }
                 else
                switch(m&3) //He's hit
                {
                  case 0: pb->cm=36+2; break; //Low pain
                  case 1: pb->cm=33+2; break; //High pain
                  case 2: pb->cm=43;pb->wallsound=1; pb->stat|=NEEDGETUP;break; //Trip
                  case 3: pb->cm=42;pb->wallsound=1; pb->stat|=NEEDGETUP;break; //Fall down
                }

                pb->dizzydur=0;if (pb->eattach.e && pb->eattach.en==21) pb->eattach.e=0;
                if (death) pa->numjoymoves=pa->numjm;

          }

        pb->stat&=~PUSHOPB;

        if (timeleft && pa->energy)
        {
        pb->energy-=elose;
        nbsize[z^1]=pb->energy*BL/MAXENERGY;
        if (nbsize[z^1]<0) nbsize[z^1]=0;
        if (pb->energy<=0)
                {
                 pb->energy=0; pb->stat&=~NEEDGETUP;
                 
                 if (pb->eattach.e && pb->eattach.en==21) pb->eattach.e=0;
                 pb->cm=91; pb->dizzydur=0; pa->dizzydur=0; pb->dizzy=pa->dizzy=0;
                 slowdown=1000; pb->wallsound=1;
                 if (pa->cm!=3) pa->cm=0; pa->stat&=~CONTROL;

                 if (pa->fatality) {pb->fatality=3; pa->fatality=0; slowdown=0;}
                }
        if (pb->pain>=0) pb->pain=(MAXENERGY-pb->energy)/ (MAXENERGY/4);
        }

        pb->stat&=~(CONTROL); //Lose control & turnaround
        pb->stat|=BEENHIT;
        drawpriority=z;

        //If opponent is in the air
        if (basey-pb->y>0 && pb->energy>0 && pb->cm!=101)
                if (!(m&4)) {pb->cm=45;pb->stat&=~NEEDGETUP;}  //Jumping pain
                 else     {  pb->cm=42; pb->wallsound=1; pb->stat|=NEEDGETUP;} //Jumping dead


        pb->jumpptr=0;


        pframeptr   =&pa->frame[pa->moves[21].firstframe];
        pcurrframe=0;  pnumframes=pa->moves[21].numframes;
        pdur=((frametype *)pframeptr)->dur;

        if (ptype==1) {pframeptr=0;
        GenerateEffect(3,px,py,0,0,pa->d,0);
         
        }
        pstat=2;




        if (pb->d!=pd^1)
         {pb->d=pd^1;  if (pb->in.stat&3) pb->in.stat^=3; }

        if (pb->dizzy>DIZZYMAX)   //See if guy's hit to dizzy
                 if (!pb->fatality)
                        {           //Make 'em dizzy
                  if (pb->cm!=43) pb->cm=42;
                        pb->stat&=~(PUSHOPB+BLOCK+CONTROL);
                        pb->stat|=STANCE+NEEDGETUP; pb->dizzydur=80+pb->energy*280/MAXENERGY;
                        pb->dizzy=DIZZYMIN; if (death) pa->numjoymoves=pa->numfm;
                  }
                else          //Fatality
                  if (pb->cm>=33 && pb->cm<=35) //If high pain
                {pb->cm=99; pb->fatality=3; pa->fatality=0; slowdown=0; stopdur=0; pb->dizzy=0; pb->dizzydur=0;}
                else { pb->cm=91; pb->dizzy=0; pb->dizzydur=0; pb->stat&=~(PUSHOPB+BLOCK+CONTROL); slowdown=0; pb->wallsound=1; pb->fatality=3; pa->fatality=0;}




        break;
                 }

                }


          vrptr++;
         }
  } //DONE




 if (!pd)  ptcx+=*((short int *)&((frametype *)pframeptr)->dx);
         else   ptcx-=*((short int *)&((frametype *)pframeptr)->dx);
 ptcy+=*((short int *)&((frametype *)pframeptr)->tx);

 px+=ptcx>>8; py+=ptcy>>8;  //Move it!!!
 ptcx&=0xFF; ptcy&=0xFF;


 if (pdur && py<=basey+10) pdur--;
  else
        {
         if (py>basey+10)
           {py=basey+10; slowdown=0;
            if (ptype || pz->fatality) PlaySysSE(16,py);
           }
         PlaySE( ((frametype *)pframeptr)->se,0,z,px );

         pcurrframe++;

         if (pcurrframe>=pnumframes)
         {
          //Done head
          if (ptype==1 )
                {pdur=30000; py=basey+10;}
          else

          //Done projectile--do hit
          if (pstat==1)
          {
                pframeptr   =&pa->frame[pa->moves[20].firstframe];
                pcurrframe=0; ptype=0;
                pdur=((frametype *)pframeptr)->dur;
          }
          //Finished projectile
          else pframeptr=0;

         }  else {pframeptr+=((frametype *)pframeptr)->size; pdur=((frametype *)pframeptr)->dur;}





if (pframeptr)
        if ( ((frametype *)pframeptr)->parm&256 && ((frametype *)pframeptr)->parm&512  )
         {
//     pause=1;
          register unsigned char *t=pframeptr+sizeof(frametype);
          while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
          t+=1+(*t *sizeof(rect));  //Skip vuln
          t+=1+(*t *sizeof(rect));  //Skip attack

          int etype=((*t)>>4);

          if (!pd) pa->PEffect(etype,px+*((short int *)(t+1)),py+ *((short *)(t+3)));
                 else   pa->PEffect(etype,px-*((short int *)(t+1)),py+ *((short *)(t+3)));
         }

        }

 if (px>350 || px<-30) pframeptr=0; //If off screen


 //If two hit each other
 if (pframeptr)
  for (projectilestat *psz=&pz->ps[0]; psz<&pz->ps[5]; psz++)
  if (psz->pframeptr)
        if ( abs(px-psz->px)<6 && abs(py-psz->py)<22 && !ptype && pstat==1 && psz->pstat==1 && !psz->ptype)
  {
         pstat=2;
         pframeptr   =&pa->frame[pa->moves[21].firstframe];
         pcurrframe=0;  pnumframes  =pa->moves[21].numframes;
         pdur=((frametype *)pframeptr)->dur;

         psz->pstat=2;
         psz->pframeptr   =&pb->frame[pb->moves[21].firstframe];
         psz->pcurrframe=0;  psz->pnumframes  =pb->moves[21].numframes;
         psz->pdur=((frametype *)psz->pframeptr)->dur;
  }

}

int lm;

//Returns value depending on amount of "getting out of holdness" done
//by input device. Requires button pushing along with l+r movement.
int playerstat::ShakeOff()
{
if (ctype==3) return((GetRnd(15)) ? 1 : 2);

 if ((!lrflipflop && (in.stat&1) ) || (lrflipflop && (in.stat&2) ))
  {
   lrflipflop^=1;
        lrbutmax=2;
  }

 if ( (in.stat&(16+32)) && lrbutmax)
  {
        lrbutmax--;

        return(12);
  }

 return((GetRnd(25)) ? 1 : 2);

}



extern int fstate;
//Change move if joystick or keyboard
void playerstat::ControlMove()
{

        //Check to see if a special move has been completed
  //if (!throwstat)
 for (int i=0; i<numjoymoves; i++)
  if
   (sm[i].Check(in.stat) && //if motion was entered...
   (
     (pz->energy || fatality==1) && //the other guy has energy or we are doing a fatality
     (y==basey || airmove)  &&   //we are on the ground or airmoves turned on
    ((cm<78 || cm>=88) && cm!=100) && //not already doing special move
      (
       stat&CONTROL ||        //we either have control or...
       stat&DONEATTACK ||      //we can combo of a previous hit or..
       (y!=basey && airmove) || //airmoves are on or...
       ((cm==45 || cm==46) && currframe==numframes-1 && !dizzydur) //last frame of getting up
      )
     || (supermove && !throwstat)
   )
   )
     if (i<numjm)
      {     //its a special move
       /*if (cm>=24 && cm<=26) {jumpptr=frameptr; jumpdur=dur;} */
       stat&=~(CONTROL+BLOCK+AIR);
       stat|=/*STANCE+*/ATTACKING;
       jumpptr=0;  cm=78+i; newmove=1;
       break;
      }
         //Fatality
         else
        if (moves[96].numframes && throwptr[3][pz->type] && pz->y==basey && *(pz->vrectptr) &&
                 ((pz->d==0 && x>pz->x) || (pz->d==1 && x<pz->x)) &&
                  abs(x-pz->x)<((frametype *)(frame+moves[96].firstframe))->energy)
         {
                  throwstat=1; pz->throwstat=3; throwgrab=0;
                  cm=96; newmove=1;
                  stat&=~(CONTROL+BLOCK); //Lose control & block
                  stat|=ATTACKING; if (pz->energy) rndcount[z]++;
                   //kill him and make him dizzy permanently
                  drawpriority=z; pz->dizzydur=1000; pz->energy=0; nbsize[z^1]=0; 
                  frameptr=frame+moves[cm].firstframe;

                  pz->frameptr=pz->frame+pz->moves[47].firstframe; //start him out on dizzy
                  pz->newmove=0; pz->cm=47; 

                  fstate|=1<<pz->type;  //mark it as done
                  slowdown=120;     //slow down for fatality
  //                PlaySysSE(17,x);

                  if (musicstat) stop_music();   //if (fatality) {fatality=3; pz->fatality=3;}
                  
         }

 //do hundred hands and continue if kept hitting
if (moves[86].numframes || moves[87].numframes)
  {
        if (hhmeter) hhmeter-=1;

        if (moves[86].numframes)
                 {if  (in.stat&16 && (!kbmove || kbmove==1)) hhmeter+=HHINC;} else
                 {if  (in.stat&32 && (!kbmove || kbmove==4)) hhmeter+=HHINC;}

        if (hhmeter>HHMAX) hhmeter=HHMAX;

        if (hhmeter>HHMIN && (stat&CONTROL) && !(stat&AIR) )
                {
        if (moves[86].numframes) cm=86; else cm=87;
        stat&=~(CONTROL+BLOCK); stat|=ATTACKING+STANCE;
                }
  }


if (!newmove  ) //If move wasn't changed by outside source (keyb/modem/special)
{
 if ((stat&CONTROL) && !(stat&AIR))
  {    //Turn around
         if ( ((d && x<pz->x-20) || (!d && x>pz->x+20)) && (!(stat&BLOCK) || pz->y==basey) )
         { if (! (in.stat&8)) cm=48; else cm=49;  d^=1;if (in.stat&3) in.stat^=3;  }
  }

        //If can do throw
 if (in.stat&(16+32) && in.stat&(1+2) && !(in.stat&8) && stat&CONTROL && (!(stat&AIR) || ((cm==45 || cm==46) && currframe==numframes-1 && !dizzydur))
          && pz->y==basey && *(pz->vrectptr) && !pz->throwvuln && ((d==0 && x<pz->x) || (d==1 && x>pz->x)) )
        {
         int tcm=93; if (in.stat&32) tcm++; //Get move for throw
         if (moves[tcm].numframes && throwptr[tcm-93][pz->type])  //If throw exist
                 if (abs(x-pz->x)<((frametype *)(frame+moves[tcm].firstframe))->energy)
                 {
        stat&=~(CONTROL+BLOCK); //Lose control & block
        drawpriority=z;
        cm=tcm;    frameptr=frame+moves[cm].firstframe; newmove=1;
        throwstat=1; pz->throwstat=3; throwgrab=0;
        if (in.stat&1 && !(((frametype *)frameptr)->parm&128) ) {d^=1; if (in.stat&3) in.stat^=3; }
        pz->stat&=~CONTROL; pz->cm=0; //pz->newmove=1;
        pz->frameptr=pz->frame+pz->moves[0].firstframe;
        if (fatality) {fatality=0; pz->fatality=3;}
                 }
        }



 //Do rapid weak moves
 if (in.stat&(16+32) && pz->energy && stat&ATTACKING && currframe==numframes-1 && hhmeter<HHMIN &&
                (cm==8 || cm==11 || cm==14 || cm==17)  )
  {
        int tcm;

        if (kbmove) { tcm=8+kbmove-1; kbmove=0;} /*        else
         {
          if (in.stat&2) tcm=8;  //Weak
                else
          if (in.stat&1) tcm=10; //Fierce
         else          tcm=9;  //Strong
          if (in.stat&32) tcm+=3;  //Kick
         }
        if (in.stat&8) tcm+=6;*/

        if (tcm==cm) newmove=1;

  }




 if ((stat&CONTROL) || (supermove && !throwstat))
  {

        //*************************DO ATTACKING MOVES
        if (in.stat & (16+32))
                if (!(stat&AIR))  //If on ground
                 {
        if (kbmove) { cm=8+kbmove-1; kbmove=0;  }
/*         else
          {
                if (in.stat&2) cm=8;  //Weak
                 else
                if (in.stat&1) cm=10; //Fierce
                         else          cm=9;  //Strong
                if (in.stat&32) cm+=3;  //Kick
          }*/

        if (in.stat&8) cm+=6;

        if (abs(x-pz->x)<70 && moves[cm+58].numframes) cm+=58;
        stat&=~(CONTROL+BLOCK); //Lose control & block
        stat|=ATTACKING;
        drawpriority=z;
                 }
                else
        if (y<basey-20)
          {
                jumpptr=frameptr;jumpdur=dur;
                cm=0;
                if (kbmove) { cm+=27+kbmove-1; kbmove=0; }
/*                else
                {
                 if (in.stat&2) cm+=27;  //Weak
                  else
                 if (in.stat&1) cm+=29;  //Fierce
                  else          cm+=28;  //Strong
                 if (in.stat&32) cm+=3;  //Kick
                }*/
                if (lm==24 && moves[cm+23].numframes) cm+=23;

                stat&=~(CONTROL+BLOCK); //Lose control & block
                stat|=ATTACKING;
                drawpriority=z;

          }
 }

  //Make guy move up/down when pushed back in block
if (cm>=60 && cm<=62 &&  (in.stat&8)) {cm+=3; lm=cm; stat&=~STANCE;}
if (cm>=63 && cm<=65 && !(in.stat&8)) {cm-=3; lm=cm; stat|= STANCE;}


 if (stat&CONTROL)
  {

        //**********************DO JOYSTICK MOVES
        if (!(stat&AIR))
        {

        //Make guy block/unblock
        if (NeedBlock())
                 //If not already blocking
          {if (!(stat&BLOCK)) { stat|=BLOCK; if (stat&STANCE) cm=56; else cm=58;}}
          else
                 //If already blocking
          {if (stat&BLOCK)  { stat&=~BLOCK; if (stat&STANCE) cm=57; else cm=59; }}

         //Joystick moving down
         if (in.stat&8)
         {
          if (stat&STANCE) //If we are standing, make us go to crouch
                {
                 if (stat&BLOCK) cm=7; else cm=2;
                 stat&=~STANCE;
                }
         } else
         //Joystick not moving down
         {
          if (!(stat&STANCE)) //If we are crouching, make us go to stance
                {
                 if (stat&BLOCK) cm=6; else cm=3;
                 stat|=STANCE;
                }

          //Joystick moving backward
          if (in.stat&1)
          {
                 if (in.stat&4) {cm=26; stat|=AIR; stat&=~BLOCK;} ///Jump back
        else if (!(stat&BLOCK) && cm!=57) cm=5;
          }
                else
          //Joystick moving foreward
          if (in.stat&2) { if (in.stat & 4) {cm=25; stat|=AIR; } else cm=4; }
                else
          //Joystick not moving left or right
          { if (in.stat & 4) {cm=24; stat|=AIR; } else if (cm!=90 && cm!=3 && (cm|1)!=49 && cm!=57) cm=0; }
         }
        }

  }

 if (cm!=lm) newmove=1;
}


}

unsigned char combo[][6][4]=
{
 {{67,79, 0, 0}, {15,81, 0, 0}, {15,78, 0, 0}, {14,15, 0, 0}, {82, 0, 0, 0}, { 0, 0, 0, 0}}, //moj
 {{ 9,79, 0, 0}, {16,80, 0, 0}, {10, 81,0, 0}, {86, 0, 0, 0}, { 8, 8, 0, 0}, {67, 0, 0, 0}}, //jin
 {{16,80, 0, 0}, {14,14, 0, 0}, {12,81, 0, 0}, {14,15, 0, 0}, {13, 0, 0, 0}, { 0, 0, 0, 0}}, //sav
 {{10,80, 0, 0}, {18,78, 0, 0}, { 8, 9,82, 0}, {83, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //pie
 {{12,12, 0, 0}, {12,78, 0, 0}, {14,14,79, 0}, {16, 79, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //spice
 {{10, 0, 0, 0}, {15,80, 0, 0}, {80, 0, 0, 0}, {79, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //vlad
 {{18,78, 0, 0}, {15,79, 0, 0}, {14,14,80, 0}, {14,81, 0, 0}, {81, 0, 0, 0}, { 0, 0, 0, 0}}, //asylum
 {{67,78, 0, 0}, {82,13, 0, 0}, { 9,80, 0, 0}, { 7, 7, 0, 0}, {86, 0, 0, 0}, { 0, 0, 0, 0}}, //chi
 {{19, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //chi
 {{19, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //chi
 {{19, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}}, //chi

};

unsigned char airdefense[][3]=
{
 {10,78,81}, //m
 {81,79,19},//j
 {10,79,81},//s
 {78,13,13},//p
 {79,10,19},//spice
 {10,79,82},//vlad
 {79,78,10},//as
 {80,10,86},//chi
 {10,79,80},//staine
 {79,16,12},//portal
 {78,81,13},//laz
 {79,81,13},//ug

};


unsigned char farmove[12][3]=  //moves to do when far away
 {
 {79,78,80}, //moj
 {78,80,81}, //jin
 {78,80,78}, //sav
 {80,81,80}, //pie
 {79,78,80}, //spice
 {78,79,81}, //vlad
 {80,82,83}, //asylum
 {80,81,82}, //chi
 {78,79,81},    //staine
 {78,80,79}, //portal
 {80,78,80}, //laz
 {78,80,81},  //ug
}   ;

void playerstat::ComputerMove()
{
 lm=cm;
 if (newmove) return;




int distx=abs(x-pz->x); //horizontal distance
int disty=abs(x-pz->x); //vertical distance

if (pz->stat&BEENHIT) combohit++;


if (comboptr && combohit && *comboptr>=78 && *comboptr<=85 && (y==basey || airmove) && !GetRnd(5) && cm!=100 && (pz->energy || fatality==1))
 {
   cm=*comboptr; comboptr++; combonum++;
   stat&=~(CONTROL+BLOCK); //Lose control & block
   stat|=ATTACKING;
   drawpriority=z;

 }

//if we have control, do strategy
if (stat&CONTROL)
 {
  if (pz->stat&ATTACKING)      //if they are attacking now
   { in.stat|=1; //make us block
      if (strategy<1) {strategy=5;sdur=50;} //hold block if we are not attacking
   }  
   else  //if they are not attacking
   {
     in.stat&=~1;  //dont block
     if (!(pz->stat&CONTROL) && strategy<1)  //if they are occupied, then atatck!
      {strategy=2; destx=65+random(30); sdur=50+random(50);}
   }  

  if (pz->ps[0].pframeptr &&  !pz->ps[0].ptype && strategy<3) //if they have a projectile at us
  {
//    if (random(100)<30) //block
//      {in.stat|=1; strategy=5; sdur=100;}
//    else
      {strategy=6;}// pause=1;} //jump over it
  }    

   //**********************DO JOYSTICK MOVES
   if (!(stat&AIR))  //if we aren't in the air
      {

         //Turn around
         if ( ((d && x<pz->x-20) || (!d && x>pz->x+20)) && (!(stat&BLOCK) || pz->y==basey) )
         { if (! (in.stat&8)) cm=48; else cm=49;  d^=1;if (in.stat&3) in.stat^=3;  }

          if (NeedBlock())
                {if (!(stat&BLOCK)) { stat|=BLOCK; if (stat&STANCE) cm=56; else cm=58;}
                 //Make us change stance/crouch for block
                  if (pz->stat&ATTACKING)
                  {
                        if (!(pz->stat&STANCE)) stat&=~STANCE;
                        if (pz->stat&AIR)       stat|=STANCE;
                  }
                }
          else        //If already blocking
                {if (stat&BLOCK)  { stat&=~BLOCK; if (stat&STANCE) cm=57; else cm=59; }}
      }


//if opponent falling 
if ((pz->cm|1)==43 && strategy!=1)
 {strategy=1; destx=110+random(50); sdur=25+random(70);} else //go far away;

//do air defense!
if (y==basey && pz->stat&AIR && strategy!=5 && (pz->cm>=24 && pz->cm<=32) && (distx<110) && disty<90) //&& (pz->y>basey-100))
     {
       if(random(100)<25) {strategy=5; in.stat|=1; sdur=50;}
        else
        {         
       cm=airdefense[type][GetRnd(3)];
       stat&=~(CONTROL+BLOCK); //Lose control & block
       stat|=ATTACKING;
       drawpriority=z;
        } 
     }
  else 


//CARRY OUT STRATEGY
  switch(strategy)
   {
     case 0:      //still figuring what to do (we just cochillin')
     if (distx<80 && !(pz->stat&AIR))
      {
         {
          if (GetRnd(2))
             {cm=8+GetRnd(6);stat|=STANCE; } //if (moves[86].numframes && GetRnd(3)) cm=86; }
           else
             {cm=14+GetRnd(6);stat&=~STANCE;}  
          stat|=ATTACKING;
         }  
      }

     if (distx>150 && !(pz->stat&AIR))
        {strategy=0; sdur=1; cm=farmove[type][random(3)]; stat|=ATTACKING;}
        
/*    if (fatality) //do fatality maybe if we can
      {
       //   pause=1;
        sdur=0;
       if (random(100)>50)
        {strategy=7; break;}
      }  */
     

      if (sdur)  sdur--;
       else
        {
         int c=random(100); //do something i guess
         if (c>50) {strategy=2; destx=65+random(30); sdur=25+random(50);} else  //get upclose;
         if (c>30) {strategy=1; destx=110+random(50); sdur=25+random(70);} else //go far away;
         if (c>0) {strategy=3; destx=70; desty=60;} //go jump kick
        } 
       break;

     case 1:  //go to a specific destination
       if (distx<destx-5) cm=5; else
       if (distx>destx+5) cm=4; else
        {strategy=0; sdur=1; cm=farmove[type][random(3)]; stat|=ATTACKING;}
       if (sdur) sdur--; else {strategy=0; sdur=0; cm=farmove[type][random(3)]; stat|=ATTACKING;} 
       if (x==LB || x==RB)  {strategy=0; sdur=30; cm=0;}
      break;

     case 2:  //go within a specific range
       if (sdur) sdur--; else {strategy=0; sdur=100; cm=0;} 
       if (distx>destx) cm=4; else
        {
         strategy=4; sdur=30;  //do combo
         cm=0; comboptr=0;
        }
      break;

     case 3: //jump within range
       if (!(stat&AIR)) //we haven't jumped yet
        {
         if (distx<60)    //jump up if real close
                { cm=24; stat|=AIR; stat&=~BLOCK; }
              else
         if (distx-(jumpwidth*5/7)<destx) //jump forward
                { cm=25; stat|=AIR; stat&=~BLOCK; }
              else cm=4;       
        } else //we have jumped
        {  //do jumping attack
          if (distx<destx && (pz->stat&AIR || (disty<desty && *((short int *)&((frametype *)frameptr)->tx)>0)) && y<basey) //if within range
                 {
                  sdur=1; strategy=0;
                  cm=random(2)+28+3*random(2);
                  jumpptr=frameptr;jumpdur=dur;
                  stat|=ATTACKING;
                 } 
        }    
      break;


     case 4: //do combo/move
       if (!comboptr) //we dont have a combo started yet
        {
         if (random(100)<25) {cm=16+random(2)*3; strategy=0; stat|=ATTACKING; sdur=5; break;} else
          if (random(100)<30) {strategy=0; sdur=30; break;}

         int x=GetRnd(6);
         int c=0;
         comboptr=&combo[type][0][0];
         for (int i=0; i<x; i++)
          {
           do {comboptr=&combo[type][c][0];
                c++; if (c>=6) c=0;
               } while (!*comboptr);
          }     
         
          combonum=0;
        } else
         if (!combohit) {strategy=0; sdur=5; comboptr=0; break;}
         
       cm=*comboptr; comboptr++; combonum++; combohit=0;
       if (cm) stat|=ATTACKING;
       
       if (cm==0 || combonum>4) {strategy=0; sdur=5; comboptr=0;}

      break;

     case 5: //block
       in.stat|=1;
       if (sdur) sdur--;
        else strategy=0;
      break;   

     case 6: //jump over projectile
//       if (!pz->pexist) {strategy=0;  break;}
//      int proj=0;
//      for (int i=0; i<PMAX; i++)
//        if (pz->ps[i].pframeptr && !pz->ps[i].ptype) //if its there!
//        { proj=1;
//         if (abs(pz->ps[i].px-x)<jumpwidth/2)
            { cm=25; stat|=AIR; stat&=~BLOCK; strategy=3; destx=70; desty=60; break;}
//        }
//        if (!proj) {strategy=0; pause=1;break;}    

//       break;


      case 7: //fatality
//       pause=1;
       if (!fatality) {strategy=0; break;}
       if (distx>((frametype *)(frame+moves[96].firstframe))->energy) cm=4; else
         {
                  throwstat=1; pz->throwstat=3; throwgrab=0;
                  cm=96; newmove=1;
                  stat&=~(CONTROL+BLOCK+DONEATTACK); //Lose control & block
                  stat&=~ATTACKING;
                  drawpriority=z; pz->dizzydur=1000;
                  frameptr=frame+moves[cm].firstframe;
                  pz->frameptr=pz->frame+pz->moves[47].firstframe;
                  pz->newmove=0;  pz->cm=47;
                  if (musicstat) stop_music();
                  slowdown=120;
                  strategy=0;
         }
        

      break; 

      
   }

 if (stat&ATTACKING)
   {
    stat&=~(CONTROL+BLOCK); //Lose control & block
    drawpriority=z;
   } 

   
 }


 if (cm!=lm) newmove=1;

}

extern int comconnected,complay;

void playerstat::ModemMove()
{


 cm=((framepacket *)prbuf)->cm;
 frameptr=((framepacket *)prbuf)->frameptr+frame;
 stat=(stat&(~255)) |((framepacket *)prbuf)->stat;
 GetRect();


  PlaySE( ((frametype *)frameptr)->se,0,1,x );

   //Make effect
     if ( ((frametype *)frameptr)->parm&256 && ((frametype *)frameptr)->parm&512  )
         {
          unsigned char *t=arectptr+ ((*arectptr)*sizeof(rect))+1;
          int etype=((*t)>>4);

          if ( ((*t)&15)>=GetRnd(16) ) //Do we need to generate it?
           if (!d) PEffect(etype,x+*((short int *)(t+1)),y+ *((short *)(t+3)));
                else   PEffect(etype,x-*((short *)(t+1)),y+ *((short  *)(t+3)));
         }


 if ((stat&CONTROL) && !(stat&AIR))
  {    //Turn around
         if ( ((d && x<pz->x-20) || (!d && x>pz->x+20)) && (!(stat&BLOCK) || pz->y==basey) )
          d^=1;
  }

}    


void playerstat::ModemPos()
{
 int oldy=y;   
 x=((int)((pospacket *)prbuf)->x)+160;
 y=180-(int)((pospacket *)prbuf)->y;
 newframe=1;

 if (oldy<180 && p[1].y==180) //we landed
  {
      if ( (stat&NEEDGETUP) || cm==91) //((cm>=42 && cm<=44) || cm==91 || cm==92 || cm==23)
                 {     //splat landing!
                        y=basey;
                        if (!(cm>=42 && cm<=44))  slowdown=0;
                        if (lastdusttime<uu)
                        {
                        PlaySysSE(2,x);
                         GenerateEffect(28,x,y,0,0,0,0);
                        gshakedur=30;
                        lastdusttime=uu+8;
                        }
                 }
                else   //land softly, as in done jumping
                 {
                        y=basey;
                        PlaySysSE(1,x);
                        GenerateEffect(28,x,y,-15,0,0,0);
                        if (stat&AIR && !pz->dizzydur)
                         {pz->throwvuln=35;     /*throwvuln=70;*/ }
                        stat&=~DONEATTACK;
                 }

  }      


}    




void  playerstat::Update()
{
playerstat *pz=::pz;

if (gamestat) stat&=~CONTROL;
if (throwvuln) throwvuln--;
if (blockholddelay)  blockholddelay--;

//If we aren't being thrown move
 if (throwstat!=3)
  {
   if (!comconnected || !z) Move();
  }  
  else
  {
         //else be moved by thrower
        if ( ((frametype *)pz->frameptr)->parm&8)
         {
          if (!d)  x=pz->x+*((short int *)(&((frametype *)frameptr)->dx));
          else     x=pz->x-*((short int *)(&((frametype *)frameptr)->dx));
          y=pz->y+*((short int *)(&((frametype *)frameptr)->tx));
          if (x>RB) {pz->x-=x-RB; x=RB;}
          if (x<LB) {pz->x+=LB-x; x=LB;}
         }
         //if held, try to shake off
        if (pz->holdframeptr)
         {
          int s=ShakeOff();
          if (s>1)
          {
                pz->holddur--;
                pz->dur-=s/2; if (pz->dur<0) pz->dur=0;
          }
         }
  }

 lm=cm;

 //Accept new moves by controller
 if (ctype==1)  ControlMove();
// if (ctype==2) ModemMove(); 
 if (ctype==3) ComputerMove();

 //See if valid move
// if (newmove && moves[cm].numframes==0)
//      {newmove=0; cm=lm; jumpptr=0;slowdown=0;}

 //If we are doing jumpkick, handle accordingly
 if (jumpptr)
  {
        if (currframe>=numframes-1) dur++; //Don't want to go over many jk frames
        //If we hit ground
        if (y>=basey)
         {
         if (cm>=78 && cm<=85)  jumpptr=0;
                else
          {frameptr=jumpptr; jumpptr=0; cm=lm=24; newmove=0; currframe=numframes-1; } //Let default take care of it.
         }
        else
         {  //Handle jump frames
          if (jumpdur) jumpdur--;
                else
                 {
                        jumpptr+=((frametype *)jumpptr)->size;
                        jumpdur= ((frametype *)jumpptr)->dur;
                 }
         }
  }


 //Do dizzy
 if (dizzy>0) { if ( !(uu&63) && !(stat&BEENHIT))  dizzy--;}
        else       { if ( !(uu&15) && !(stat&BEENHIT))  dizzy++;}

 if (dizzydur)
  {
        if (cm==47) //He's in dizzy
         {
          throwvuln=0;

          if (!fatality) //We can get out of dizzy if not fatality
          {
                /*
                if (in.stat&(16+32) ) {dizzydur-=10; dur-=8; if (dur<0) dur=0; }*/
                int s=ShakeOff();
                dizzydur-=s; dur-=s-1; if (dur<0) dur=0;
                if (dizzydur<=0) {cm=0; dizzydur=0; newmove=1;  stat|=CONTROL;}
          } else
          {
          dizzydur--;
          if (dizzydur<=0 && !throwstat)  //Fatality over if he runs out of time
                {cm=97; newmove=1; fatality=3; pz->fatality=0; pz->stat&=~CONTROL; }
          }
         }
  } else
        {if (eattach.e && eattach.en==21) eattach.e=0; if (death) p[z^1].numjoymoves=p[z^1].numjm;}


 in.stat&=15;

if (complay && z)  return;

 //*************************ADVANCE FRAME
 if (dur && y<=basey && !newmove) dur--;
  else
  {
        int advanceframe=1; //Variable that determines if we should advance frame
        if (newmove) advanceframe=0;


        //Land
        if (y>basey)
                if ( (stat&NEEDGETUP) || cm==91) //((cm>=42 && cm<=44) || cm==91 || cm==92 || cm==23)
                 {     //splat landing!
                        y=basey;
                        if (!(cm>=42 && cm<=44))  slowdown=0;

                        if (lastdusttime<uu)
                        {
                        PlaySysSE(2,x);
                        //GenerateEffect(6, x-25,y, 0,0 ,0,0);
                        //GenerateEffect(6, x   ,y, 0,0 ,0,0);
                        //GenerateEffect(6, x+25,y, 0,0 ,0,0);
                         GenerateEffect(28,x,y,0,0,0,0);
                        gshakedur=30;
                        lastdusttime=uu+8;
                        }
                 }
                else   //land softly, as in done jumping
                 {
                        y=basey;
                        PlaySysSE(1,x);
//                      GenerateEffect(6, x,y, -15,0,d,0);
                         GenerateEffect(28,x,y,-15,0,0,0);

                        if (stat&AIR && !pz->dizzydur)
                         {pz->throwvuln=35;     /*throwvuln=70;*/ }
                        stat&=~DONEATTACK;
                 }

        //See if we need to go back to beginning of hold
        if (throwstat==1 && ((frametype *)frameptr)->parm&64)
         {
          --holddur;
          if (timeleft)
          {
                //Do successive hits
                int elose=((frametype *)frameptr)->energy;
                if (!fatality)
                {
                elose=elose*hithurt[hitcnt[cm]]/100;
                if (hitcnt[cm]<8) hitcnt[cm]++;
                elose=elose* (pz->energy+ED)/ ((MAXENERGY)+ED);
                if (!elose) elose++;
                }

          pz->energy-=elose;
          nbsize[z^1]=pz->energy*BL/MAXENERGY;
          if (nbsize[z^1]<0) nbsize[z^1]=0;
          if (pz->energy<=0 && !fatality)
                 {
                  pz->energy=0; pz->stat&=~NEEDGETUP;
                  pz->dizzydur=dizzydur=0; dizzy=pz->dizzy=0;
                  slowdown=1000; pz->wallsound=1;
                  if (pz->fatality)  { fatality=0; pz->fatality=3; slowdown=0; }
                  holddur=0;
                 }
          if (pz->pain>=0) pz->pain=(MAXENERGY-pz->energy)/ (MAXENERGY/4);
          }
          if (holddur>0)
                 {
                  frameptr=(unsigned char *)holdframeptr; pz->frameptr=(unsigned char *)pz->holdframeptr;
                  dur=((frametype *)frameptr)->dur; pz->dur=5000;
                  currframe=pz->currframe=holdframe;
                  advanceframe=0;
                 } else {holdframeptr=pz->holdframeptr=0; }
         }


        //Go to next frame
        if (advanceframe) {currframe++; frameptr+=((frametype *)frameptr)->size;}
        newframe=1;         //We moved to a new frame
        if (!(stat&AIR)) stat&=~DONEATTACK;  //We haven't attacked yet



         //Doo hundred hands
        if ( (cm|1)==87 && currframe>=numframes-1 && hhmeter>70)
         {
          currframe=1;
          frameptr=frame+moves[cm].firstframe;
          frameptr+=((frametype *)frameptr)->size;
         }

//See if we need to let go of him
if (throwstat==1 && ((frametype *)frameptr)->parm&4) //&4 is let go of
 {
   throwstat=pz->throwstat=0;

   //Make guy fly off
   if ( ! (((frametype *)frameptr)->parm&128))   {pz->d^=1; if (pz->in.stat&3) pz->in.stat^=3;}

   int a=((frametype *)frameptr)->parm&3;  //find out type of let go
   pz->newmove=1;
     if (cm==96)  //if fatality
       {
        if (a==0) pz->cm=92;      //final falling
          else    pz->cm=96+a;   //various head flying
       } else
     switch (a)  //regular throw
       {
         case 0: pz->cm=92; pz->stat|=NEEDGETUP;break;           
         case 1: if (pz->energy) pz->cm=45; else pz->cm=91;  break;
         case 2: pz->cm=23; pz->stat|=NEEDGETUP;break;
       }

    if (timeleft && !pz->fatality)
     {
        //Do successive hits
        int elose=((frametype *)frameptr)->energy;

        elose=elose*hithurt[hitcnt[cm]]/100;
        if (hitcnt[cm]<8) hitcnt[cm]++;
        elose=elose* (pz->energy+ED)/ ((MAXENERGY)+ED);
        if (!elose) elose++;

        pz->energy-=elose;
        nbsize[z^1]=pz->energy*BL/MAXENERGY;
        if (nbsize[z^1]<0) nbsize[z^1]=0;
        if (pz->energy<=0)
          {
                if (pz->eattach.e && pz->eattach.en==21) pz->eattach.e=0;
                pz->energy=0; pz->stat&=~NEEDGETUP;
                pz->dizzydur=dizzydur=0; dizzy=pz->dizzy=0;
                slowdown=1000; pz->wallsound=1;
                if (pz->fatality)  { fatality=0; pz->fatality=3; slowdown=0;}
                pz->cm=91;
          }
        if (pz->pain>=0) pz->pain=(MAXENERGY-pz->energy)/ (MAXENERGY/4);
        }
}


        //Advance our throwee by one frame too
        if (throwstat==1 && currframe<numframes)
          {
                if (throwgrab)
                {
                 if (advanceframe)
        if ( (((frametype *)frameptr)->parm&16) )
        {
         pz->currframe++;
         pz->frameptr+=((frametype *)pz->frameptr)->size; pz->dur=30000;
         pz->GetRect();
        } else pz->currframe++;

                 if ( ((frametype *)frameptr)->parm&8)
                 {
        pz->y=y+*((short int *)(&((frametype *)pz->frameptr)->tx));
        if (pz->x>RB) {x-=pz->x-RB; pz->x=RB;}
        if (pz->x<LB) {x+=LB-pz->x; pz->x=LB;}
         if ( ((frametype *)frameptr)->parm&4096) drawpriority=z^1;
                         else drawpriority=z;
                 }
                } else
        if ( ((frametype *)frameptr)->parm&16 ) //See if we should grab him
        {
         //Set up opponent for throw
         if (pz->eattach.e && pz->eattach.en==21) pz->eattach.e=0;
         pz->stat&=~(CONTROL+BLOCK+PUSHOPB);
         pz->frameptr=(unsigned char *)(tframe+throwptr[cm-93][pz->type]); pz->GetRect();
         pz->cm=92; pz->dizzy=pz->dizzydur=0; pz->wallsound=0;
         pz->dur=7000;  pz->numframes=numframes;
         pz->currframe=currframe;

         if (pz->d!=d) {pz->d=d; if (pz->in.stat&3) pz->in.stat^=3; }
         if ( ((frametype *)frameptr)->parm&8)
         {
          if (!d)  pz->x=x+*((short int *)(&((frametype *)pz->frameptr)->dx));
                else   pz->x=x-*((short int *)(&((frametype *)pz->frameptr)->dx));
          pz->y=y+*((short int *)(&((frametype *)pz->frameptr)->tx));
          if (pz->x>RB) {x-=pz->x-RB; pz->x=RB;}
          if (pz->x<LB) {x+=LB-pz->x; pz->x=LB;}
         }
          if (cm==96 && !pz->energy) {  fatality=0; pz->fatality=3; }
         throwgrab=1;
        }

                if ( (((frametype *)frameptr)->parm&32) && !holdframeptr)
         {
          holdframeptr=(char *)frameptr; pz->holdframeptr=(char *)pz->frameptr;
          holdframe=currframe;
          holddur=(pz->energy)*HOLDDUR/MAXENERGY;
          holdshakemeter=15;
         }
          }


//-----------If a move has been completed or a new one initiated
         if ( (currframe>=numframes) || newmove)
         {

          //See if a move has been completed
          if (!newmove)
          {
                wallsound=0;
                stat&=~(BEENHIT+PUSHOPB+ATTACKING);

                //Do periodic inactivity
                if (lm==90) cm=lm=0;
                if (cm==0) {if (!--inactivitycnt)
                             if (!comconnected) {inactivitycnt=4; cm=lm=90;}}

                //Make opponent wait while dying
                if (!pz->energy && !fatality || !timeleft)
         {       if (cm!=3) cm=0; stat&=~(CONTROL+BLOCK); if (!(stat&STANCE)) cm=3; stat|=STANCE; /*if (x<pz->x) d=0; else d=1;*/}
                          else
                //If still playing, give control back to player
                if  ( (!(stat&CONTROL) || (lm&~1)==48) && !throwstat)
                 { //Go from doing move back to stance
        if (in.stat&8) {if (stat&STANCE)    cm=2; else cm=1; stat&=~STANCE; if (NeedBlock()) {cm=58;stat|=BLOCK;} }
                  else {if (!(stat&STANCE)) cm=3; else cm=0; stat|=STANCE;  if (NeedBlock()) {cm=56;stat|=BLOCK;} }
                  stat|=CONTROL;
                 }


                if (lm==2) cm=1;  //stance->crouch
                if (lm==3) cm=0;  //crouch to stance

                if (stat&AIR) //Stopped jumping
        { stat&=~(AIR+STANCE); if (!(in.stat&8)) {cm=3; stat|=STANCE;} else cm=1;
          if (((d && x<pz->x) || (!d && x>pz->x))) {d^=1;if (in.stat&3) in.stat^=3;  }
         }

                if (stat&BLOCK)  //done with blocks
                {
                 if (lm==56) cm=6;   
                 if (lm==58) cm=7;
                 if (lm>=60 && lm<=62) {cm=6; throwvuln=45;}
                 if (lm>=63 && lm<=65) {cm=7; throwvuln=45;}
                }
                if (lm==57) cm=0;
                if (lm==59) cm=1;

                if (lm>=33 && lm<=41) throwvuln=25;

             //Get up
       if (stat&NEEDGETUP)//  && pz->energy)
         if (!fatality)
                 {
                  stat&=~(CONTROL+AIR+DONEATTACK); cm=46;  throwvuln=60;
                  if  (dizzydur && moves[47].firstframe)  //Make stars as getting up
                        {
                         stat&=~BLOCK;
                         register unsigned char *t=frame+moves[47].firstframe+sizeof(frametype);
                         while (*t!=0xFF) t+=sizeof(image);  t++;    //Skip images
                         t+=1+(*t *sizeof(rect));  //Skip vuln
                         t+=1+(*t *sizeof(rect));  //Skip attack
                         if (!d) PEffect(1,x+*((short int *)(t+1)),y+ *((short int *)(t+3)));
                          else   PEffect(1,x-*((short int *)(t+1)),y+ *((short int *)(t+3)));
                        }
                  stat&=~NEEDGETUP;
                 }     //Can't get up after fatality
                else  {fatality=3; pz->fatality=0;dizzydur=0;stat&=~NEEDGETUP; }

                //Make him turn around once stood up
                if (lm==46 && !dizzydur)
                 {
                if (((d && x<pz->x) || (!d && x>pz->x))) {d^=1;if (in.stat&3) in.stat^=3;  }
                if ( !(in.stat&8)) stat|=STANCE;
                //if (ctype==3) in.stat=1;
                if (NeedBlock())
                 {stat|=BLOCK;if (stat&STANCE) cm=56; else cm=58;}
                 }

                if ( (lm|1)==87) if (hhmeter>HHMIN) cm=lm;

                //DONE throw
                if (lm>=93 && lm<=96)
        {
         if (((d && x<pz->x) || (!d && x>pz->x))) {d^=1;if (in.stat&3) in.stat^=3;}
         throwstat=0; pz->throwstat=0; //pause=1;
        }

                //Keep player in dizzy if dizzydur>0
                if (dizzydur && stat&CONTROL) {cm=47; stat&=~(CONTROL+BLOCK); stat|=STANCE; }


                if (gamestat) {stat&=~CONTROL; cm=lm;}  //Don't do anything
          }

          //if (moves[cm].numframes==0) {cm=0;  stat|=CONTROL; slowdown=0;}

          frameptr   =&frame[moves[cm].firstframe];
          numframes=moves[cm].numframes;
          currframe  =0;

          if (cm==100)
          if (((frametype *)frameptr)->parm&128) {stat|=CONTROL; stat&=~DONEATTACK;}
                else {stat&=~CONTROL;}

         }


        dur=((frametype *)frameptr)->dur;
//   if ((!z && fastp1) || (z && fastp2)) if (!(dur<<=1)) dur=1;
        newmove=0; GetRect();
        if ( ((frametype *)frameptr)->parm&16384) stat&=~ATTACKING;



        //Make sound effect
        PlaySE( ((frametype *)frameptr)->se,0,z,x );

        //Make effect
        if ( ((frametype *)frameptr)->parm&256 && ((frametype *)frameptr)->parm&512  )
         {
          unsigned char *t=arectptr+ ((*arectptr)*sizeof(rect))+1;
          int etype=((*t)>>4);

          if ( ((*t)&15)>=GetRnd(16) ) //Do we need to generate it?
          if (!d) PEffect(etype,x+*((short int *)(t+1)),y+ *((short *)(t+3)));
                else   PEffect(etype,x-*((short *)(t+1)),y+ *((short  *)(t+3)));
         }


        if (throwstat==1 && throwgrab)
        {
          PlaySE( ((frametype *)frameptr)->se,1,z,x );

         if ((((frametype *)frameptr)->parm&16))
         {
          PlaySE( ((frametype *)pz->frameptr)->se,0,z^1,x );
          //Make effect
          if ( ((frametype *)pz->frameptr)->parm&256 && ((frametype *)pz->frameptr)->parm&512  )
                {
                 unsigned char *t=pz->arectptr+ ((*pz->arectptr)*sizeof(rect))+1;
                 int etype=((*t)>>4);

                 if ( ((*t)&15)>=GetRnd(16) ) //Do we need to generate it?
                 if (!pz->d) pz->PEffect(etype,pz->x+*((short *)(t+1)),pz->y+ *((short  *)(t+3)));
        else   pz->PEffect(etype,pz->x-*((short *)(t+1)),pz->y+ *((short  *)(t+3)));
                }
          }
        }

  }






/*    //Make GetRnd drippings
  if (pain && !(--paindur))
   {
    if (pain==1) paindur=(100+GetRnd(30))>>bloodlevel;
         if (pain==2) paindur=( 50+GetRnd(30))>>bloodlevel;
         if (pain==3) paindur=( 20+GetRnd(20))>>bloodlevel;

    register image *i=(image *) (frameptr+sizeof(frametype));

    while (i->index!=0xFF && ( (i->orient>>2)!=pain || GetRnd(2)) ) i++;

    if (i->index!=0xFF)
          GenerateEffect(17,x+i->dispx[d],y-80+i->dispy,-(i->dispx[1]+i->dispx[0])/2 ,10, 0,0);

        }*/





//DO STUFF WHEN NO ENERGY
 if (!energy && !((dur)&3) && y<basey && *((short *)&((frametype *)frameptr)->tx)>=0 )
  {
        GenerateEffect(16,x,y, 10+GetRnd(20),-25,GetRnd(2),GetRnd(2) );
//   if (fatality) {fatality=3; pz->fatality=0;}
  }

 if (!energy  && fatality!=2 && !gamestat)
  {

  //If we are at end of our final fall

  if (currframe>=numframes-1)
                { dur=8000; newmove=0;
        pz->hhmeter=0;

        if (pz->energy) // If NOT double KO
         {
                pz->dizzydur=0; //Make sure opponent not dizzy
           if (pz->eattach.e && pz->eattach.en==21) pz->eattach.e=0;

                //If he's done with his winning pose
                if ( ((pz->cm|1)==89 || pz->cm==96) && (pz->currframe>=pz->numframes-1) && pz->dur==2)
                 { pz->dur=9000;  gamestat++;
                        if (timeleft) postwordtype=z^1;
                else        postwordtype=4+1;
                 }

                if (pz->cm==0)  //Wait till he's ready for winning move
                  {
                        pz->stat&=~CONTROL;
                        if (!fatality)
                         {rndcount[z^1]++;  //Add wins
                          GenerateEffect(27,160,6+13*rndcount[z^1],152,0,z,0); //Make skull
                         }
                        if (rndcount[z^1]<2 || fatality || cm==99) //Normal round winning
                         {
                  pz->cm=88+GetRnd(2);  //Make him pose
                  pz->newmove=1; if (x>pz->x) pz->d=0; else pz->d=1;
                         } else
                         {               //Fatality
                fatality=2;   dizzydur=550; dizzy=-25 ; //We dizzy
                cm=46; stat&=~(PUSHOPB+BLOCK+CONTROL+AIR+DONEATTACK); stat|=STANCE;
                newmove=1; stat&=~NEEDGETUP;

                pz->fatality=1;          //Him in control
                if (random(100)>50 && pz->throwptr[3][type]) pz->strategy=7; else pz->strategy=0;
                pz->stat|=CONTROL;

                pz->numjoymoves=pz->numfm;
                         }
                  }
         }
        else               //DOUBLE KO
         if (pz->currframe>=pz->numframes-1)
                  { gamestat++;
                        if (timeleft) postwordtype=4+2;
                else        postwordtype=4+1;
             }
      }
  }




}


void playerstat::Reset() //Resets basic variables
{
int i;

y=basey; //d=0;
cm=0;
stat=CONTROL+STANCE; throwstat=0;
frameptr   =&frame[moves[cm].firstframe];
currframe  =0;
numframes=moves[cm].numframes;
dur=((frametype *)frameptr)->dur;
tcountx=0;
tcounty=0;

lastdusttime=0;
hhmeter=0;
dizzy=20;
dizzydur=0;
paindur=50;

energy=MAXENERGY;
fatality=0;

for (i=0; i<PMAX; i++)
 {ps[i].pframeptr=0; ps[i].pstat=0; }
pexist=0;

jumpptr=0;
jumpdur=0;


ca=cai=0;
//in.stat=0;

pain=0;
inactivitycnt=1;//+GetRnd(5);

numjoymoves=numjm;

for (i=0; i<numfm; i++) sm[i].jpp=0;

GetRect();

for (i=0; i<130; i++)   hitcnt[i]=0;

strategy=0;
sdur=0;

 unsigned char *tf=frame+moves[25].firstframe;
 jumpwidth=jumpheight=0;
        
  for (i=0; i<moves[25].numframes; i++)
            {
             jumpwidth+=*((short int *)&((frametype *)tf)->dx) * ((frametype *)tf)->dur;
             jumpheight+=*((short int *)&((frametype *)tf)->tx) * ((frametype *)tf)->dur;
             tf+=((frametype *)tf)->size;   
            }
  jumpwidth>>=8; jumpheight>>=8;          

comboptr=0; combonum=0;
}






