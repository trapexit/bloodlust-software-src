#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <direct.h>
#include <dos.h>
//#include <sbcvoice.h>
//#include <sbcmusic.h>

#define REVOL
#define CE
#define MAXMARKS 40

//#include "sb2.h"
#include "tgraph.h"
#include "tinput.h"
#include "time.h"
#include "tkeyb.h"

//#include "ll_comm.h"
#include "tipx.h"
#include "tpacket.h"
#include "tchat.h"


extern int wins,losses;
void GenerateEffect(int en, int x, int y, int dx, int dy, char d,char p);

extern effect effects[];

extern bground bg;
extern char *screen;
extern char *background;
extern volatile char kbscan;
extern volatile int quit;
extern char *font,*font2,*font3,*font4,*font5,*font6;
extern volatile unsigned int  uu;
extern int sbstat;
extern playerstat p[2];
extern int game,debug,kbstat;
extern int fadelevel,oldfadelevel;
extern color pal1[256],pal2[256];
void UpdatePalette2();

extern int ipxoffset;

extern int gigerlevel,oldgiger,dgiger;
extern int fadeout,fadein;
int done;

static int ps=0,prbroad=0,prtext=0,pa=0,pbroad=0;

int getname=1;
int nameptr;
char chatdebug=0;
extern char ipxmaster;
unsigned lastbroadcast;


netplayer *netplayers,*opponent; //all players in chatroom
static netplayer *sp; //selected player

//local packets to be sent
idpacket   idp;
ackpacket  ap;
actionpacket actp;
killpacket killp;


char cmsg[100];
int cmsgdur;


netplayer::netplayer(IPXADDRESS *addr,char *n,int w, int l,int m)
{
memset (this,0,sizeof(netplayer));  //clear all of ECB & packet
strcpy(name,n);
a=*addr; wins=w; losses=l; master=m;
}    


//store a character to send to this dude
void netplayer::OutputChar(char c)
{
if (endctr-startctr>80) {if (this!=netplayers) timeout=uu-1; return;} 
sendbuf[endctr-startctr]=c; //store it in buffer
endctr++;        //increase number of characters that have been in buffer of all time
}    


char t[256];
//actually send the necessary text to the dude
void netplayer::SendText()
{
if (startctr>=endctr) return; //if there are NO characters in the buffer, say fuck it! and leave


/* if a certain amount of time has expired since the last time we sent something
  then the amount of characters sent can only be the amount that has been acknowledged
  because any additional characters were probably lost and need to be resent*/
if (stime<uu) //last send time
 {
   sentctr=startctr; //resend chars from beginning of sendbuf
   stime+=100;
 }

/*endctr is the counter of the last character in buffer. if it is greater than the last char sent
  then it is time to send those extra characters from sent to end*/
 
if (sentctr<endctr) //if we haven't sent them all already, we need to send them dude!
 {
  ((textpacket *)t)->type=1;
  ((textpacket *)t)->index=sentctr;             //sentctr is the counter at the beginning of chars to send
  ((textpacket *)t)->len=endctr-sentctr;        //number of chars to send
  memcpy(&((textpacket *)t)->t,sendbuf+sentctr-startctr,endctr-sentctr);
  
  if (SendIPXPacketDest((char *)&t,sizeof(textpacket)+(endctr-sentctr),(char *)&a.nodeadd))  //send the chars to the dude!
  {
    uu++; ps++;
    sentctr=endctr; //move sent to end of buffer
    stime=uu+100;    //resend time (if the guy never acknowledges)
  } else stime=uu+50; //resend if packet didn't get send
 }     
  
}

//remote player acknowledged that he received something up to the 'a' index
void netplayer::Ack(unsigned a)
{
if (a<=startctr)
 {
   stime=uu-1; //force send of all data
   return; //we've already acked so far, ignore
 }
if (a>endctr)  a=endctr;   //impossible case, cut off

//cut off chars from front of send buffer that have been acked
memcpy(sendbuf,sendbuf+a-startctr,endctr-a);

startctr=a; //acknowledge pointer moves up
}    


//process received text
void netplayer::Process(textpacket *t)
{
 char *keys=(char *)&t->t; //get pointer to txt


 if (this!=netplayers) timeout=uu+2500; //timeout period     

 if (t->index>rctr)
  { 
//   strcpy(cmsg,"**PACKET LOST**");
//   cmsgdur=50;
     goto ack;
  }
 

    
 if (t->index<rctr) //if we've already acked some of these
   {
    unsigned int skip=(rctr-t->index); //skip the acked chars
    if (skip>=t->len) //we've already collected all of these
          goto ack;
    t->len-=skip;
    keys+=skip;
    t->index=rctr;
//    cmsg="TEXT SKIP";

// strcpy(cmsg,"**PACKET COMPENSATED**");
// cmsgdur=50;

    
   }

 while (t->len>0) //go through all of keys
  {
   char *s=text+slen;                             //find end of string
   char key=*keys++; t->len--;  rctr++;    //get char typed
   if (key!=8) //if not backspace
    {
      *s=key; s++; *s=0; //store key 
      slen++; tlen+=GetLetterWidth(font4,(unsigned char)key); //add to length
    }     
      else
    if (slen>0)
     {
      s--; tlen-=GetLetterWidth(font4,(unsigned char)*s); *s=0; slen--;
      tlen+=ltime; ltime=0;
     }  //backspace
   }


GenerateEffect(10,tlen+5,19+y,0,0, 0,0);

ScrollLeft();

ack:
 //send ack  
 ap.startctr=rctr;
 SendIPXPacketDest((char *)&ap,sizeof(ackpacket),(char *)&a.nodeadd);  //send the ack
 uu++; pa++;
}

void netplayer::ScrollLeft()
{
   if (kill)
    {
     if (kill<500)
       {
        if (!(kill&15)) //make another blood thing
          {
           GenerateEffect(25,random(320),y+19,0,0,random(2),random(2));
           for (int i=0; i<8; i++)
             GenerateEffect(16,random(300)+10,y+13,0,0,random(2),random(2));
          }
        kill++;
       } else if (this==netplayers) {if (!quit) timeout=uu+30; quit=1;}
    }  
  
  if (tlen>290) // we must scroll left now cause we are too long
   {
    if (!ltime) lw=GetLetterWidth(font4,(unsigned char )text[0]);
    ltime++; //scroll the left letter once
    tlen--;  //lessen width of all of text

    if (tlen>302) {ltime+=1; tlen-=1;}    
    if (tlen>308) {ltime+=tlen-308; tlen=308;}    
    while (ltime>=lw) //if we have scrolled through all of letter
     {
      memcpy(text,text+1,slen); //scoot left
      slen--; ltime-=lw; 
      lw=GetLetterWidth(font4,(unsigned char )text[0]);
     }
     
   }

}    




void netplayer::Draw(int x, int y)
{
char s[64];
char *d=screen+y*320+x;

if (rchallenge)
 memset(d-x-320,14*16+10,320*19);

if (schallenge) //we sent them a challenge
 {
  char *t=d-x-320;
  char color=6*16+15;
//  if (schallenge!=1 || (uu&8))
//  for (int i=0; i<19; i+=2,t+=320*2)
    memset(t,color,320*19);
 }


if (this==sp && this!=netplayers) //this is the selected dude
 {
//  char color=(uu&64) ? 1 : 9;
  char color=6*16;
  
  char *t=d-x-320;
  memset(t,color,640); t+=640-2;

  for (int i=0; i<16; i++,t+=320)
     memset(t,color,4);
  t+=2-320;
  memset(t,color,640);
 }



DrawStringSP(font4,d,(unsigned char *)name,320);

sprintf(s,"%02d",wins);
DrawStringSP(font6,d+100,(unsigned char *)s,320);

sprintf(s,"%02d",losses);
DrawStringSP(font6,d+147,(unsigned char *)s,320);

sprintf(s,"  WINS    LOSSES");
DrawStringSP(font4,d+102,(unsigned char *)s,320);


if (chatdebug==1)
{
sprintf(s,"%dB %dS %dE %dR",startctr,sentctr,endctr,rctr);
DrawStringSP(font4,d+70,(unsigned char *)s,320);

PrintAddress(&a,s);
DrawStringSP(font4,d+220,(unsigned char *)s,320);
}

if (chatdebug==2)
{
if (this!=netplayers)
 {
  sprintf(s,"timeout: %d",timeout-uu);
  DrawStringSP(font4,d+70,(unsigned char *)s,320);
 } 
}    

//do textical stuff
_disable();
d+=9*320+3;
if (!ltime || !slen) DrawStringSP(font6,d,(unsigned char*)text,320);
 else
 {
  unsigned short leftletter=((unsigned short *)font4)[text[0]];
  DrawImage(font6+leftletter,d,-ltime,0,0);
  DrawStringSP(font6,d-ltime+lw,(unsigned char*)text+1,320);
 }     
  if (uu&64)
  DrawStringSP(font6,d+GetStringWidthSP(font6,(unsigned char *)text)-ltime,(unsigned char *)"_",320);
_enable();  
  
d-=3;
d+=9*320;
memset(d-x ,14*16+2,320); 
}






int dobroadcast=0;




extern "C" int ipxlisten(int);

void ChatTick()
{
 uu++;
//SCROLL AND DO FADING THINGS ---------------------
if (uu<32) fadelevel=uu;
   else
if (fadeout) if  (fadelevel) fadelevel--; else done=1;

if (cmsgdur) cmsgdur--;

if (!(uu&1))
{
bg.scrx+=1;
if (bg.scrx>320) bg.scrx=0;
}

gigerlevel=uu&63;
if (uu&64) {gigerlevel^=63; }
gigerlevel>>=1;


if (!getname)  //broadcast over and over
{
 if (!netplayers->quit) if (!(uu&127))  dobroadcast=1;
// if (!(uu&31))  dobroadcast=1;

 //scroll text left
 netplayer *n=netplayers;
 while (n)  { n->ScrollLeft(); n=n->next; }

 // make bloody effects
 for (effect *eptr=effects; eptr<&effects[MAXMARKS*2]; eptr++)
         if (eptr->e) eptr->Tick();

        

/* if (masterstat.spewshower) //we gotta spew as our master tells us
  if (!(uu&31))
   GenerateEffect(12,random(320),-10,0,0, 0,0);
*/
}
  
}


extern void (interrupt far *oldkeybintvector)(void);
void ChatKeyb()
{
 unsigned char kbscan=inp(0x60);
 if (kbscan==1) quit=1;
  else
 (*oldkeybintvector)();
}    


void DrawTimeslaughter()
{
char *s="NOGGIN KNOCKERS CHAT";
char *d=screen+7*320+07;


DrawString(font2,d,(unsigned char *)s,320);

char t[10];
if (ipxoffset>=0) sprintf(t,"%d",ipxoffset);
       else sprintf(t,"%d",ipxoffset);
DrawStringSP(font5,screen+30*320+300,(unsigned char *)t,320);
 
/*
int y=((gigerlevel-32)/8)*320;
while (*s)
 {
  DrawLetter(font2,d+y,(unsigned char)*s,320);
  y=-y;
  int index=((unsigned short *)font2)[*s];
  if (!index) d+=6;
   else  d+=(int) ((unsigned char)font2[index+1]);
  s++;
 }     
*/

}    


void ResetY()
{
netplayer *n=netplayers;
while (n)
{
 if (n->previous) n->y=n->previous->y+20;
 n=n->next;
}
}


void SearchFile(char *str, char *f, char *ret)
{

char *s=str;
while (*f)
 {
  if (toupper(*f)==toupper(*s)) //found this char!
    {
     s++;
     if (!*s)  //we found all of str!
      { f++; break;}
    } else s=str; //start search from beginning again
  f++;  
 }

while (isspace(*f) && *f!='\n') f++;
for (s=ret; *f && *f!='\n'; s++,f++) *s=*f;
*s=0;

}

void Snatch(netplayer *n)
{
char s[80];    
char *file;
int h;
unsigned size,bytes;
int found=0;
if (!_dos_open("c:\\kali95\\kali95.ini",O_TEXT | O_RDONLY,&h)) found=1;
else if (_dos_open("c:\\kali\\kali.ini",O_TEXT | O_RDONLY,&h)) found=1;

if (!found) strcpy(s,"Not found.");
   else
 {
  size=filelength(h); file=(char *)malloc(size);
  _dos_read(h,file,size,&bytes);
  _dos_close(h);
  file[size-1]=0; //'end of string'

  char ser[50],skey[50];
  SearchFile("Serial=",file,ser);
  SearchFile("Skey=",file,skey);  
  sprintf(s,"SN:%s SKEY:%s",ser,skey);
  
  free(file);
 }


for (int i=0; s[i]; i++)
  n->OutputChar(s[i]); //output all characters
}    


void QuitChatScreen()
{
_disable();    
//free all nodes
actp.type=3; //quit
netplayer *n=netplayers->next;
netplayers->next=0;
while (n)
 {
  netplayer *next=n->next;

  if (n!=opponent) //save the opponent and ourselves
   {
    SendIPXPacketDest((char *)&actp,sizeof(actionpacket),(char *)&n->a.nodeadd);  //send the quit
    _disable();
    free(n);
   }
  n=next;
 }
_enable(); 


if (prbroad>0) //if we do seem to be connected
  WaitIPXPacket(); //send all packet
}    



netplayer *ChatScreen()
{
    int h;
    unsigned int bytes,size;
int i;

    
Solid((char *)0xa0000,64000,0);
bg.ReadBackground("chatbg.vol",0);
for (i=1; i<100; i++) bg.ylist1a[i]=bg.ylist1a[0];

for (i=0; i<MAXMARKS*2; i++) effects[i].e=NULL;

_dos_open("stuff.vol",O_BINARY | O_RDONLY,&h);
//_dos_read(h,&size,4,&bytes); //Skip versus
//lseek(h,size,SEEK_CUR);
_dos_read(h,pal1,256*3,&bytes);
_dos_close(h);
/*
_dos_creat("stuff.vol",0,&h);
_dos_write(h,pal1,256*3,&bytes);
_dos_close(h);
*/

uu=0;
bg.scrx=0; bg.scry=10; 

oldfadelevel=-1;fadelevel=0; gigerlevel=0; oldgiger=-1; dgiger=1;
UpdatePalette2();
fadeout=0;

kbscan=0; quit=0;
p[0].in.Reset(); p[1].in.Reset();
done=0;

cmsgdur=0;

//chat stuff
nameptr=0;

//set up our sender id packet
if (getname)
{
 idp.type=0;
 idp.name[0]=0;
 idp.master=ipxmaster;
}

idp.wins=wins; idp.losses=losses;


//set up our player on chat screen
netplayers=new netplayer(myaddress,idp.name,idp.wins,idp.losses,ipxmaster);
netplayers->timeout=0xFFFFFFFF; //timeout period (never)
netplayers->y=28;



//set up ack packet
ap.type=2;


//set up kill packet
killp.type=7;
killp.data=0;


lastbroadcast=0xFFFFFFFF;

if (!getname) lastbroadcast=uu+2000;

ResetIPX();   
IPXChatMode(ipxoffset); 

timerhandler=ChatTick;
int oldkeyb=(int)keyboardhandler;
keyboardhandler=ChatKeyb;

char key=0;

sp=netplayers;
opponent=0;

do
{

PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, 0, gigerlevel/4  ,  bg.scrx,  0,  0,   0+2, bg.ylist1a, bg.ylist2);
PutBackgroundTriOverlap(bg.bg1a,bg.bg2,bg.bg3,screen, 0, 0, 0, 100+gigerlevel/4, bg.scrx, 100,  0,  100+2, bg.ylist1a, bg.ylist2);

//draw behind effects
for (effect *eptr=&effects[0]; eptr<&effects[MAXMARKS]; eptr++) eptr->Draw();




if (getname) //we are asking for their name
 {
  while (kbhit())
   { 
    char key=getch();
    if (!key)
      {if (kbhit()) key=getch(); }
    else
    {
     if (isalpha(key) && nameptr<15) //add to string
      {
        idp.name[nameptr++]=key;
        idp.name[nameptr]=0;
      }
     if (key==8 && nameptr>0) {nameptr--; idp.name[nameptr]=0;} 
     if (key==13 && nameptr>1)
       {
        //make our local player name
        strcpy(netplayers->name, idp.name);
        getname=0;  //stop getting name
        dobroadcast=1; //send it out to everyone
        lastbroadcast=uu+2000;
       } //done with name
    } 
   }
     
 //draw prompt
 DrawStringSP(font2,screen+60*320+40,(unsigned char *)"ENTER YOUR NAME: ",320);
 DrawStringSP(font6,screen+90*320+150-GetStringWidthSP(font4,(unsigned char *)idp.name)/2,(unsigned char *)idp.name,320);
 if (uu&64)
 DrawStringSP(font6,screen+90*320+151+GetStringWidthSP(font4,(unsigned char *)idp.name)/2,(unsigned char *)"_",320);
 }


if (!getname)  //we are in the chatroom
{



DrawTimeslaughter();


  //read keyboard     
  while (kbhit())
    {
      key=getch();
      if (!key)
       { if (kbhit()) key=getch();
        //move selection bar thing
        if (key==72) //up
         if (sp->previous /*&& sp->previous!=netplayers*/) sp=sp->previous;

        if (key==80) //up
         if (sp->next) sp=sp->next;

        if (key==0x49 || key==0x51) //if page down or page up
         {
           if (key==0x49) ipxoffset--; else ipxoffset++;  //change our socket offset
           QuitChatScreen();             //quit the chatscreen, free all players
           ResetIPX();                   //stop all packets
           IPXChatMode(ipxoffset);       //restart chatscreen on different socket
           sprintf(cmsg,"IPX Socket 0x%02X%02X",ipxchatsocket&0xFF,ipxchatsocket>>8);
           cmsgdur=200;
           dobroadcast=1;
         } 

        if (netplayers->master)
        {
         if (key==0x5E) //kill CTRL-F1
        if (sp!=netplayers) //no suicide
         { 
          killp.a=sp->a; //store the fucker's address

          netplayer *n=netplayers;
          while (n)
          {
           SendIPXPacketDest((char *)&killp,sizeof(killpacket),(char *)&n->a.nodeadd);  //send the kill!
           n=n->next;
          }
         }

         if (key==0x5F) //snatch CTRL-F2
         { 
           actp.type=8; SendIPXPacketDest((char *)&actp,sizeof(actionpacket),(char *)&sp->a.nodeadd);
         }

         if (key==59) //ctrl-f3
          {idp.wins++; wins++; netplayers->wins++;}
         if (key==60) //ctrl-f4
          {idp.losses--;  losses--; netplayers->losses--;}  
        }
       }
    //normal keys----------
            else
      {
       if (isprint(key) || key==8) 
       {
        netplayer *np=netplayers; //store key in every player's buffer
        while (np) { np->OutputChar(key); np=np->next; } 
       }

      if (key==13) //enter, do challenge
        {
         if (sp!=netplayers || ipxmaster) //if not ourself
          if (!(sp->schallenge&1)) //if not in an intermedia phase
           {
            sp->schallenge++;
            if (sp->schallenge==1) sprintf(cmsg,"**Challenge issued to %s**",sp->name);
            if (sp->schallenge==3) sprintf(cmsg,"**Revoking challenge from %s**",sp->name);
            cmsgdur=400;
           } //begin challenge issuence
        }
        
     if (netplayers->master)
      {
       if (key=='\t') //tab
        {chatdebug++; if (chatdebug>2) chatdebug=0;}
      }
       
/*
      if (key=='.')
       {
        netplayer *n=netplayers,*nprev=0;
        while (n) { nprev=n;  n=n->next; }  

        n=new netplayer((IPXADDRESS *)&cmsg,"FUCK",0,0,0); 
        nprev->next=n; n->previous=nprev; n->next=0; //add to end 
        n->timeout=0xFFFFFFFF;
       }*/
       
      }
    }


  //send stuff
  {
   netplayer *np=netplayers; //store key in every player's buffer
   while (np)
     {
       if (np->schallenge&1 && !(uu&63) )
         { actp.type=4;
           actp.data=np->schallenge;
           SendIPXPacketDest((char *)&actp,sizeof(actionpacket),(char *)&np->a.nodeadd);
         } //send the challenge
       np->SendText();
       np=np->next;
     }
  }
 


  //sendbroadcast
  if (dobroadcast)
  {
   BroadcastIPXPacket((char *)&idp,sizeof(idp));

   dobroadcast=0;
   pbroad++;

   if (lastbroadcast<uu)
  {strcpy(cmsg,"No broadcasts received. Check your network connection.");
       cmsgdur=500; }

  }




//read chars...........
int x;

while ( (x=RecvIPXPacket())!=0 )
  {
   
   //search through all players, figure who sent it
   netplayer *n=netplayers,*nprev=0,*kp;
   while (n && !CompareIPXAddr(&n->a,&ip[x].ipx.source) ) ///is this the dude?
      { nprev=n;  n=n->next; }  

   char *ipd=(char *)&ip[x].data; //address of data
   
   //do something with it    
   switch (ipd[0]) //packet type
   {
    case 0: //id
      prbroad++; lastbroadcast=uu+2000;
     if (!n) //if we didn't find a matching node
     {    //ADD it!
       n=new netplayer(&ip[x].ipx.source,((idpacket *)ipd)->name,
          ((idpacket *)ipd)->wins,((idpacket *)ipd)->losses,((idpacket *)ipd)->master);
       
       if (!nprev) {n->previous=n->next=0; netplayers=n;} //first node
         else {nprev->next=n; n->previous=nprev; n->next=0;} //add to end
       ResetY();

       SendIPXPacketDest((char *)&idp,sizeof(idp),(char *)&n->a.nodeadd);

       if (!n->master)  sprintf(cmsg,"**%s joined Noggin Chat**",n->name);
                else    sprintf(cmsg,"**Master %s graces you with his presence**",n->name);
       cmsgdur=300;
     }
     if (n!=netplayers)  n->timeout=uu+2500; //timeout period
     break;

    case 1: //text
     prtext++;
     if (n) n->Process((textpacket *)ipd);
     break;

    case 2:  //ack
     if (n) n->Ack( ((ackpacket *)ipd)->startctr);
     break;

    case 3:  //quit
     if (!n) break;
     n->quit=1; n->timeout=uu+30; //force a timeout
     
     if (!n->kill) //not been killed
     {
      if (!n->master)  sprintf(cmsg,"**%s quit Noggin Chat**",n->name);
               else    sprintf(cmsg,"Master %s urinates on you as he departs",n->name);
     } else sprintf(cmsg,"**Evisceration of %s Complete**",n->name);

     cmsgdur=300;
     break;

//we will receive this packet if a challenge was sent to us
//we must acknowledge it
    case 4:
     if (!n || opponent) break;
     if (((actionpacket *)ipd)->data==1)  n->rchallenge=2;   //challenge request/revoke received successfully
     if (((actionpacket *)ipd)->data==3)  n->rchallenge=0; //revoke
     actp.type=5;  //send the challenge ack
//     for (i=0; i<5; i++)
     SendIPXPacketDest((char *)&actp,sizeof(actionpacket),(char *)&n->a.nodeadd);
     
     if (n->rchallenge)
      {
        if (n->schallenge)
           {sprintf(cmsg,"**Challenge accepted by %s**",n->name); opponent=n; done=1;}
        else sprintf(cmsg,"**Challenged by %s**",n->name);
      } else sprintf(cmsg,"**Challenge revoked by %s**",n->name);
     cmsgdur=500;
     break;

//we will receive this packet if a challenge sent by US was
//received successfully by the opponent
    case 5:
     if (!n || opponent) break;

     if (n->schallenge==1) n->schallenge=2; //sent challenge successfully
     if (n->schallenge==3) n->schallenge=0; //revoked successfully
     
     if (n->schallenge)
      {
       if (n->rchallenge) //if we had received a challenge too
           {sprintf(cmsg,"**Accepted challenge from %s**",n->name); opponent=n; done=1;}
       else sprintf(cmsg,"**Challenge sent to %s**",n->name);
      } else sprintf(cmsg,"**Revoked challenge from %s**",n->name);
     cmsgdur=400;
     break;


    case 7:
      //find the guy that should be killed
     kp=netplayers;
     while (kp)
      {
       if (CompareIPXAddr(&kp->a,&((killpacket *)ipd)->a )) ///is this the dude?
        {
         if (!kp->kill) kp->kill++; //start their kill counter
         if (!CompareIPXAddr(&kp->a,&netplayers->a)) //if its not ourselves that is being killed
          {
           SendIPXPacketDest(ipd,sizeof(killpacket),(char *)&kp->a.nodeadd);  //send the kill to him!
           sprintf(cmsg,"**Eviscerating %s**",sp->name);
          } else strcpy(cmsg,"Ahh, the suffering, the sweet suffering...");
         cmsgdur=500;
        }

       kp=kp->next; //next guy
      }

     break;

     case 8: //snatch
       if (n) Snatch(n);
       break;
    
          
   }   //switch

   //listen again
   ipxlisten(x*sizeof(ipxpacket));
  }


//684-1832
    
  //draw people
  
  netplayer *np=netplayers; //get first pointer
  int i=0;
  while (np)
    {
//      for (i=0; i<8; i++)
      np->Draw(5,i*20+30);

      if (uu>np->timeout) //timeout
        {
         if (np==netplayers) //We timed out
             {quit=1; break;}
            else
           {

            if (!cmsgdur && !np->kill)
              sprintf(cmsg,"**%s timed out**",np->name);
            cmsgdur=300;
               
          _disable();
          netplayer *next=0;
          if (sp==np) //if this is the selected guy
            {if (np->next) sp=np->next; else sp=np->previous;}
            
          if (np->previous) np->previous->next=np->next;  
          if (np->next)     next=np->next->previous=np->previous; 
          free(np);
          np=next;
          ResetY();
         _enable();
           }
        } else   np=np->next;   
       i++; 
    }
   
 }

if (chatdebug)
{
char s[30];
sprintf(s,"SEND%03d TRCV%03d ACK%03d BRSEND%03d BRRECV%03d",ps,prtext,pa,pbroad,prbroad);
DrawStringSP(font4,screen+173*320+20,(unsigned char *)s,320);
}


if (cmsgdur) 
 DrawStringSP(font6,screen+185*320+160-GetStringWidthSP(font6,(unsigned char *)cmsg)/2,(unsigned char *)cmsg,320);

//draw front effects
for (eptr=&effects[MAXMARKS]; eptr<&effects[MAXMARKS*2]; eptr++) eptr->Draw();


MemoryCopy((char *)(0xA0000+2*320),screen+2*320,320*192);
UpdatePalette2();


//if ((p[0].in.ReadButt(0)|p[1].in.ReadButt(1))&(~0xF))  fadeout=1;
if (quit) fadeout=1;

RelinquishIPX();

} while (!done);

keyboardhandler=(void (*)())oldkeyb;
if (sbstat==2) StopVoice();

int evisceration=netplayers->kill; //save evisceration stat

for (unsigned tu=uu+50; uu<tu; );

QuitChatScreen(); //free all nodes
free(netplayers); //free ourself
ResetIPX();       //cancel all events

timerhandler=0; //turn off timer


bg.KillBackground();

if (evisceration)
 {
  ModeText();
  printf("May all your children have stunted growth.\n");
  exit(1);
 }

return(opponent);

}


void UpdatePalette2()
{
int i;

if (oldfadelevel!=fadelevel)
 {
  unsigned int  fl=fadelevel;
  unsigned char *p1=(unsigned char *)pal1;
  unsigned char *p2=(unsigned char *)pal2;

  if (fl<32)
         for (i=0; i<256*3; i++,p1++,p2++)  *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);
  else  MemoryCopy(pal2,pal1,256*3);

  WaitVSync();
  if (!fadeout)
  {
  LoadPalette(pal2,0,16*6);          //0->16*16
  LoadPalette(pal2,16*6+1,14*16-(16*6+1));     //16*6+1->14*16
  LoadPalette(pal2,15*16,16);
  }  else LoadPalette(pal2,0,256);

  oldfadelevel=fadelevel;
 }


if (!fadeout && oldgiger!=gigerlevel)
 {
  unsigned int  fl=gigerlevel/2+16;
  unsigned char *p1=(unsigned char *)(pal1+14*16);
  unsigned char *p2=(unsigned char *)(pal2+14*16);

  for (i=0; i<16*3; i++,p2++,p1++)
        *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);

  p1=(unsigned char *)(pal1+6*16);
  p2=(unsigned char *)(pal2+6*16);
  fl^=31;
  for (i=0; i<3; i++,p2++,p1++)
        *p2=(unsigned char) ( (((unsigned int)(*p1)) * fl)/32);



  WaitVSync();
  LoadPalette(pal2,14*16,16);
  LoadPalette(pal2,6*16,1);

  oldgiger=gigerlevel;
 }


}

