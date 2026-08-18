#ifdef WIN95

#ifndef _NET_
#define _NET_

#define NETWORK

#include "types.h"
#include "uutimer.h"
#include "socket.h"

#define WM_UDPSOCKET (WM_USER+1)

extern int winsockinstalled;
void initwinsock();
void terminatewinsock();

void netdisconnect();

extern class netconnect *nc; //main network connection

struct nodeinfo
{
 NETADDR a;     //address
 char name[16]; //name of node

 void setname(char *s);
 nodeinfo():a() {name[0]=0;}
};

class netnode
{
 friend class netconnect;
 public:
 netconnect *nc;      //what we're connected through
 netnode *prev,*next; //linked list of nodes

 nodeinfo info;    //node info

 uutimer  pingtimeout; //time to ping timeout

 char *getname() {return info.name;}
 NETADDR *getaddr() {return &info.a;}
 netnode(class netconnect *c,nodeinfo *i);
 ~netnode();
};




//abstract class for all net connections (client or server)
class netconnect
{
 friend class netnode;
 udpsocket s;      //socket for data xfer
 char *buf;  //data of last packet

 public:
 uutimer pingtimer;   //timer to ping all nodes (or just server)
 uutimer pingtimeout; //timer to ping timeout (client only)

 nodeinfo info;    //local info
 int connected;
 int active;  //is connection active?

 class netnode *n; //list of netnodes connected

 virtual void tick(); //tick

 virtual int isclient()=0;
 virtual int isserver()=0;

 netnode *findnode(NETADDR *n);

 int send(NETADDR *a,void *b,int size) {return s.sendto(a,b,size);}
 int send(netnode *n,void *b,int size) {return s.sendto(&n->info.a,b,size);}

 // int sendsecure(NETADDR *a,ushort pid,void *b,int size) {return s.sendto(a,b,size);}

  //funcs called when data is ready
 void socketrecv(SOCKET t);
 void socketerror(SOCKET t);

 netconnect(int af,PORT port);
 virtual ~netconnect();
};

//-----------------------
//server, waits for connections
class netserver:public netconnect
{
 public:
 netserver(int af,PORT port); //start server on port in af
 virtual ~netserver(); //shutdown server

 virtual void tick();

 virtual int isclient() {return 0;}
 virtual int isserver() {return 1;}
};


//-----------------------------------------
//client, connects to server
class netclient:public netconnect
{
 public:
 NETADDR serveraddr; //address of server

 uutimer connecttimer;   //timer for connect msgs
 uutimer connecttimeout; //time to stop connecting

 virtual void tick();

 virtual int isclient() {return 1;}
 virtual int isserver() {return 0;}

 netclient(NETADDR *n); //create client connect to n
 virtual ~netclient(); //shutdown client
};

#endif

#endif


/*
//item in packet cache
class packetcache
{
 class netconnect *nc;
 packetcache *prev,*next;

 ushort pid;  //id of this packet
 NETADDR *n;  //dest address

 unsigned resendtime; //time to resend
 unsigned timeout;    //time to timeout
};*/

