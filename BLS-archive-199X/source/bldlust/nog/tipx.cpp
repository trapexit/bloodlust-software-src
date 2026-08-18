#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "tipx.h"
extern volatile unsigned int uu;
extern int packetsends, packetrecvs,pbytes;
extern int pcount[8];

//asm functions
extern "C" {
short int __cdecl initipxasm();
short int __cdecl ipxgetmaxpacketsize();
extern short ipxrealmemseg;
int ipxopensocket(int socket);
int ipxclosesocket(int socket);

int ipxsend(int);
int ipxlisten(int);
int ipxcancel(int);

int ipxgetaddress(int x);
void ipxrelinquish();
int reverse(int);
}
int ipxinstalled;
char *ipxmem;   

#define IPXCHATSOCKET 0x4300
#define IPXGAMESOCKET 0x4301

unsigned short int ipxchatsocket,ipxgamesocket;
int ipxmode;

ipxpacket *ip; //pointer to first packet

char ipxbuffer[256];
int ipxsize=0;


//C functions
//index for our address
#define FIRSTSENDECB 10
#define  LASTSENDECB 29
#define FIRSTRECVECB 30
#define  LASTRECVECB 49


#define MYADDRESS 25000
IPXADDRESS *myaddress;


//------------------
char *hex="0123456789ABCDEF";

void PrintAddress (IPXADDRESS *adr, char *str)
{
 int     i;

 for (i=0 ; i<6 ; i++)
  {
   *str++ = hex[adr->nodeadd[i]>>4];
   *str++ = hex[adr->nodeadd[i]&15];
   *str++ = ':';
  }
 str--; 
 *str = 0;
}

int CompareIPXAddr(IPXADDRESS *a, IPXADDRESS *b)
{
int i;
for (i=0; i<6; i++)
 if (a->nodeadd[i]!=b->nodeadd[i]) return(0);

return(1);
}    



void RelinquishIPX()
{
ipxrelinquish();
}    

//cancel all packeets
void ResetIPX()
{
int i;    
for (i=0; i<=LASTRECVECB; i++) //free it all, baby
  ipxcancel(i*sizeof(ipxpacket));

if (ipxgamesocket) {ipxclosesocket(ipxgamesocket); ipxgamesocket=0;}
if (ipxchatsocket) {ipxclosesocket(ipxchatsocket); ipxchatsocket=0;}

}    


void CancelIPXFixed()
{
for (int i=0; i<8; i++)
 if (ip[i].ecb.inuse)
  ipxcancel(i*sizeof(ipxpacket));

}    


//----
int InitIPX()
{
ipxinstalled=0;
if (initipxasm()!=0xFF)  return(1); //if memory allocation/ipx check failed, then leave
ipxinstalled=1; //set install flag

ipxmem=(char *)  (((int)ipxrealmemseg)<<4);      //pointer to real mem xfer area

myaddress=(IPXADDRESS *)(ipxmem+MYADDRESS); //get pointer to our local address storage area
ipxgetaddress(MYADDRESS);                   //find our node
ip=(ipxpacket *)ipxmem;                     //pointer to first packet

ipxgamesocket=ipxchatsocket=0; //haven't been opened yet


/*
printf("\nmaxpacketsize: %d\n",ipxgetmaxpacketsize());
printf("ipxmem: %X\n",ipxmem);
//printf("ipxsocket: %04X\n",reverse(ipxsocket));
*/
char s[30];
PrintAddress(myaddress,s);
printf("node: %s...",s);
//getch();


atexit(TerminateIPX);

return(0); 
}    

void TerminateIPX()
{
ResetIPX();
}    


//set all ecbs to chatsocket
//set all to broadcast as default
void IPXChatMode(int offset)
{
_disable();    

//find socket
ipxchatsocket=IPXCHATSOCKET+offset*2;
ipxchatsocket=(unsigned short)ipxopensocket(reverse(ipxchatsocket)); //open socket

char broadcast[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};  //broadcast to everyone
int i;
for (i=FIRSTSENDECB; i<=LASTSENDECB; i++) //create send ecbs
 CreateIPXSendECB(i,ipxchatsocket,broadcast);  

for (i=FIRSTRECVECB; i<=LASTRECVECB; i++) //create recv ecbs
 CreateIPXRecvECB(i,ipxchatsocket);
ipxmode=0; //chat mode
_enable();
}    


//set all ecbs go to gamesocket
//all sends go to one place only
void IPXGameMode(char *dest,int offset)
{
_disable();

//find socket
ipxgamesocket=IPXGAMESOCKET+offset*2;
ipxgamesocket=(unsigned short)ipxopensocket(reverse(ipxgamesocket)); //open socket


int i;
for (i=0; i<=LASTSENDECB; i++) //create send ecbs
 CreateIPXSendECB(i,ipxgamesocket,dest);  

for (i=FIRSTRECVECB; i<=LASTRECVECB; i++) //create recv ecbs
 CreateIPXRecvECB(i,ipxgamesocket);
ipxmode=1; //game mode
_enable();
}    



//prepares a memory area to be used as an ECB
//the ecb will be used to listen for packets
void CreateIPXRecvECB(int i,unsigned short socket)
{
//recv in ith
memset (&ip[i],0,sizeof(IPXECB)+sizeof(IPXHEADER));  //clear all of ECB & packet
ip[i].ecb.socket=socket;
ip[i].ecb.inuse=0x1d;

//first fragment 
ip[i].ecb.fragcount=1;
ip[i].ecb.fragoff1=(unsigned short)(i*sizeof(ipxpacket)+sizeof(IPXECB));
ip[i].ecb.fragseg1=ipxrealmemseg;
ip[i].ecb.fragsize1=sizeof(IPXHEADER)+128+4; //size of packet, send time, and header
ipxlisten(i*sizeof(ipxpacket));
}


//prepares a memory area to be used as an ECB
//the ecb will be used to send packets (defaulting to the dest)
void CreateIPXSendECB(int i,unsigned short socket,char *dest)
{
//---------------------------------
//send in 1st
memset (&ip[i],0,sizeof(IPXECB)+sizeof(IPXHEADER));
ip[i].ecb.socket=socket;         //sending socket

//send to our network & given destination
int j;
for (j=0; j<4; j++) ip[i].ipx.dest.netadd[j]=myaddress->netadd[j];

for (j=0; j<6; j++) ip[i].ipx.dest.nodeadd[j]=dest[j];

for (j=0; j<6; j++) ip[i].ecb.immedaddr[j]=dest[j]; //myaddress->netadd[j]; ///?????
ip[i].ipx.dest.socket=socket;
ip[i].ipx.type=4;

ip[i].ecb.fragcount=1;
ip[i].ecb.fragoff1=(unsigned short)(i*sizeof(ipxpacket)+sizeof(IPXECB));
ip[i].ecb.fragseg1=ipxrealmemseg;
ip[i].ecb.fragsize1=sizeof(IPXHEADER)+128+4;
}


//find a free sender ECB
//send a packet to FF:FF:FF:FF:FF:FF
int BroadcastIPXPacket(char *d, int size)
{
int i;    
for (i=FIRSTSENDECB; i<=LASTSENDECB; i++)
 if (!ip[i].ecb.inuse) //find a not inuse
 {
//  ip[i].ecb.inuse=1;  
  ip[i].time=uu;   
  ip[i].ecb.fragsize1=(unsigned short)(sizeof(IPXHEADER)+size)+4;
  memset(&ip[i].ipx.dest.nodeadd,0xFF,6);
  memset(&ip[i].ecb.immedaddr,0xFF,6);  
  memcpy(ip[i].data,d,size);               //prepare packet

  ipxsend(i*sizeof(ipxpacket));
  return(i);
 }

return(0);
}    


//find a free sender ECB
//send a packet to default
int  SendIPXPacket(char *d,int size)
{
int i;    
for (i=FIRSTSENDECB; i<=LASTSENDECB; i++)
 if (!ip[i].ecb.inuse) //find a not inuse
 {
//  ip[i].ecb.inuse=1;  
  ip[i].time=uu;   
  ip[i].ecb.fragsize1=(unsigned short)(sizeof(IPXHEADER)+size)+4;
  memcpy(ip[i].data,d,size);               //prepare packet

  ipxsend(i*sizeof(ipxpacket));
  return(i);
 }
return(0);
}    


//send a packet to default, with a fixed ecb
//no caching!
int  SendIPXPacketFixed(int ecb, char *d,int size)
{

// _disable();
// _enable();

 ip[ecb].time=uu;   
 ip[ecb].ecb.fragsize1=(unsigned short)(sizeof(IPXHEADER)+size)+4;
 memcpy(ip[ecb].data,d,size);               //prepare packet

    
 if (!ip[ecb].ecb.inuse) //if ecb inuse
 {
  //ip[ecb].ecb.inuse=1;
   
  pcount[ecb]++;
  packetsends++;   
  ipxsend(ecb*sizeof(ipxpacket));

  return(1);
 }

return(0);
}    


//find a free sender ECB
//send a packet to *dest
int  SendIPXPacketDest(char *d,int size,char *dest)
{
int i;    
for (i=FIRSTSENDECB; i<=LASTSENDECB; i++)
 if (!ip[i].ecb.inuse) //find a not inuse
 {
  ip[i].ecb.inuse=1;  
  ip[i].time=uu;   
  ip[i].ecb.fragsize1=(unsigned short)(sizeof(IPXHEADER)+size)+4;

  memcpy(&ip[i].ipx.dest.nodeadd,dest,6);
  memcpy(&ip[i].ecb.immedaddr   ,dest,6);
  memcpy(ip[i].data,d,size);               //prepare packet

  ipxsend(i*sizeof(ipxpacket));
  return(i);
 }

return(0);
}    




extern int pr;
extern unsigned char prbuf[];
void PacketIPXFunc();

int RecvIPXPacket()
{
unsigned int besttime=0xFFFFFFFF;
int bestpacket=0;

for (int i=FIRSTRECVECB; i<=LASTRECVECB; i++)
 if (!ip[i].ecb.inuse && ip[i].time<besttime) //find a not inuse and best time
      {besttime=ip[i].time; bestpacket=i;}
i=bestpacket;

if (i) //if we found a good packet 
 {
  //return the bitch
  packetrecvs++;
  pbytes += reverse(ip[i].ipx.length) - sizeof(IPXHEADER) -4 ;
  return(i);
 }

 
return(0); 
        
}

//see if there is a free sender ecb
int SendECBInUse()
{
int j=0;
for (int i=FIRSTSENDECB; i<=LASTSENDECB; i++)
 if (ip[i].ecb.inuse) //if one is inuse
   j++; //inc counter

return(j);

}

void WaitIPXPacket()
{
unsigned timeout=uu+500;

while (SendECBInUse() && uu<timeout);
 
}

    
