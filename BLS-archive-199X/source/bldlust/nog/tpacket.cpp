#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <direct.h>

#define REVOL
#define CE

extern volatile unsigned int uu;


#include "tipx.h"
#include "tpacket.h"

extern int ipxinstalled;

void (*packethandler[16])()=
{0,0,0,0,
 0,0,0,0,
 0,0,0,0,
 0,0,0,0};

unsigned char psbuf[256]; //temp packet storage
int packeterror;
int packetsends,packetrecvs;
int pbytes=0;
unsigned int packettime=0xFFFF; //time of last packet

int psize[16]=
{4,4,4,4, 4,4,4,4,
 4,4,sizeof(pospacket),sizeof(framepacket), sizeof(chatpacket),sizeof(attackpacket),4,4 }  ;

int pe[4]={0,0,0,0};

unsigned char prbuf[256]; //temp packet storage

unsigned char packettype;  //type of packet being received
int pr=0;          //bytes recv'd



//sends data to remote computer
void SendData(unsigned char *d, int num)
{
if (ipxinstalled)   //send packet through ipx
{
 SendIPXPacket((char *)d,num);
}    

   
}


void StoreCRC(unsigned char *d,int num)
{
unsigned char CRC=0;    
for (int i=0; i<num-1; i++,d++) CRC+=*d;

*d=CRC;  //store CRC at end
}

int CheckCRC(unsigned char *d,int num)
{
unsigned char CRC=0;    
for (int i=0; i<num-1; i++,d++) CRC+=*d;

if(*d!=CRC) return(0);  //store CRC at end

return(1);
}




//sends a standard packet
//type=0-15
void SendStandardPacket(unsigned char type,unsigned short data)
{
((packet *)psbuf)->type=type|0xF0;
((packet *)psbuf)->data=data;
StoreCRC(psbuf,sizeof(packet));

SendData(psbuf,sizeof(packet));
packetsends++;
}    



//sends a standard packet
//type=0-15
void SendPosPacket(signed char  x, unsigned char y)
//void SendPosPacket(unsigned short  x, unsigned short y)
{
((pospacket *)psbuf)->type=0xFA;
((pospacket *)psbuf)->x=x;
((pospacket *)psbuf)->y=y;
StoreCRC(psbuf,sizeof(pospacket));

SendData(psbuf,sizeof(pospacket));
packetsends++;
}    

/*
void SendEffectPacket(signed char  x, unsigned char y,unsigned char t)
//void SendPosPacket(unsigned short  x, unsigned short y)
{
((effectpacket *)psbuf)->type=0xF8;
((effectpacket *)psbuf)->x=x;
((effectpacket *)psbuf)->y=y;
((effectpacket *)psbuf)->y=t;
StoreCRC(psbuf,sizeof(effectpacket));

SendData(psbuf,sizeof(effectpacket));
packetsends++;
}    */

void SendFramePacket(unsigned char  cm, unsigned short fp,unsigned short stat)
{
((framepacket *)psbuf)->type=0xFB;
((framepacket *)psbuf)->cm=cm;
((framepacket *)psbuf)->frameptr=fp;
((framepacket *)psbuf)->stat=stat;

StoreCRC(psbuf,sizeof(framepacket));

SendData(psbuf,sizeof(framepacket));
packetsends++;
}    

//void SendAttackPacket(unsigned char  cm, unsigned char astat,unsigned char bstat,unsigned char f,unsigned char energy)
void SendAttackPacket(unsigned char  cma, unsigned short fpa,unsigned short stata,signed char  x, unsigned char y, unsigned char  cmb, unsigned short fpb,unsigned short statb,signed char  xb, unsigned char yb)
{
((attackpacket *)psbuf)->type=0xFD;
((attackpacket *)psbuf)->cma=cma;
((attackpacket *)psbuf)->cmb=cmb;

((attackpacket *)psbuf)->frameptra=fpa;
((attackpacket *)psbuf)->frameptrb=fpb;

((attackpacket *)psbuf)->stata=stata;
((attackpacket *)psbuf)->statb=statb;

((attackpacket *)psbuf)->xb=xb;
((attackpacket *)psbuf)->yb=yb;
((attackpacket *)psbuf)->xa=x;
((attackpacket *)psbuf)->ya=y;


StoreCRC(psbuf,sizeof(attackpacket));
SendData(psbuf,sizeof(attackpacket));
packetsends++;
}    


void SendChatPacket(char *s)
{
((chatpacket *)psbuf)->type=0xFC;
strcpy(((chatpacket *)psbuf)->s,s);

StoreCRC(psbuf,sizeof(chatpacket));

SendData(psbuf,sizeof(chatpacket));
packetsends++;
}    



static unsigned char x;


void PacketIPXFunc()
{
if ((prbuf[0]&0xF0)==0xF0) //if valid packet
 {
   packettype=prbuf[0]&0xF; //get type
   if (packethandler[packettype])    packethandler[packettype](); //call da handler
   pbytes+=pr;
   packetrecvs++;
 }
pr=0;
packettime=uu;
}    


void SetPacketFunc(int num, void(*handler)())
{
 packethandler[num]=handler;

}    

void InitPacket()
{
 for (int i=0; i<16; i++)    packethandler[i]=0;
// ioSetComFunc(PacketComFunc);
 packeterror=packetsends=packetrecvs=0;
 pr=0; pbytes=0; packettime=0xFFFF0000;
}

void ResetPacket()
{
 for (int i=0; i<16; i++)    packethandler[i]=0;
 packeterror=packetsends=packetrecvs=0;
 pr=0; pbytes=0; packettime=0xFFFF0000;
}

void DefaultComTick();

void TerminatePacket()
{
// ioSetComFunc(DefaultComTick);
}    

  
  


  
