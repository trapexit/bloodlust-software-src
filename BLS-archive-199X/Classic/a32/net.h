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

//info for one node
struct nodeinfo
{
 char num;      //ordinal player number
 NETADDR a;     //address
 char name[16]; //name of node

 void setname(char *s);
 nodeinfo():a() {num=0; name[0]=0;}
};



//abstract class for all net connections (client or server)
class netconnect
{
 friend class netnode;
 public:
 udpsocket s;      //socket for data xfer
 char *buf;  //data of last packet
 int size;   //size of last packet

 netnode *server; //server connected to (0 if server)
 netnode *local;  //local node (if we are playing)
 nodeinfo localinfo;    //local info

 uutimer pingtimer;   //timer to ping all nodes or server

 int connected; //are we connected to server?
 int active;  //is connection active?

 class netnode *n; //list of netnodes connected
 int numnodes; //number of nodes connected

 virtual void tick(); //tick
 uutimer objupdatetimer;

 int disconnecting; //are we disconnecting?
 virtual void disconnect()=0; //disconnect (will eventually self-destroy)

 virtual int isclient()=0;
 virtual int isserver()=0;

 netnode *findnode(NETADDR *a); //find node at addr a
 netnode *findnode(char num); //find node number

 //send to all nodes (except ourself)
 void sendtoall(struct packet  *b,int num);
 void sendtoallsecure(struct packet *b,int num);

   //send to specific addr
// int sendto(NETADDR *a,struct packet *b,int size) {return s.sendto(a,b,size);}
 void sendto(NETADDR *a,struct packet *b,int size);
  //funcs called when data is ready
 void socketrecv(SOCKET t);
 void socketerror(SOCKET t);

 virtual void sendpings()=0;
 virtual void checktimeout()=0;

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

 virtual void sendpings();
 virtual void checktimeout();
 virtual void disconnect(); //disconnect (will eventually self-destroy)
};


//-----------------------------------------
//client, connects to server
class netclient:public netconnect
{
 public:
// NETADDR serveraddr; //address of server
 uutimer connecttimer;   //timer for connect msgs
 uutimer connecttimeout; //time to stop connecting

 virtual void tick();

 virtual int isclient() {return 1;}
 virtual int isserver() {return 0;}

 virtual void sendpings();
 virtual void checktimeout();

 netclient(NETADDR *n); //create client connect to n
 virtual ~netclient(); //shutdown client
 virtual void disconnect(); //disconnect (will eventually self-destroy) 
};




//--------------------------------------------------
//single network node

class netnode
{
 friend class netconnect;
 public:
 netconnect *nc;      //what we're connected through
 netnode *prev,*next; //linked list of nodes

 nodeinfo info;    //node info

 unsigned pingtime; //last pingtime
 uutimer  pingtimeout; //time to ping timeout

 int resends;// number of resends

 ushort objidcount;  //next object id
 class netobject *o; //netobjects controlled by this node
 int numobj; //number of netobjects
 class netobject *findobject(ushort objid);

 void sendobjpos(); //send object pos's to all nodes

 //secure stuff----------------------------
 ushort       pid_out; //next pid to send
 int num_out; //number pending out
 struct securepacket_out *p_out; //linked list of pending packets to send
 void processsecure_out();

 ushort       pid_in; //next pid expected to receive
 int num_in;
 struct securepacket_in *p_in; //linked list of pending packets to process
 void processsecure_in();

 char *getname() {return info.name;}
 NETADDR *getaddr() {return &info.a;}
 char getnum() {return info.num;}

  //send data to this node
 void send(struct packet *b,int size) {nc->sendto(&info.a,b,size);}
  //send to node secure
 void sendsecure(struct packet *b,int size);

 netnode(class netconnect *c,nodeinfo *i);
 ~netnode();
};


//secure packet
struct securepacket
{
 securepacket *prev,*next;

 netnode *n;   //node
 ushort pid;   //pid
 class p_secure *ps; //actual packet
 int size;     //size of packet

 securepacket **list;

 securepacket(netnode *_n,p_secure *_ps,int _size,securepacket **_list);
 virtual ~securepacket();
};

//packet waiting to be processed
struct securepacket_in:securepacket
{
 securepacket_in(netnode *_n,p_secure *_ps,int _size);
 virtual ~securepacket_in();
 void process(class netconnect *c,class netnode *n);
};

//packet waiting to be sent
struct securepacket_out:securepacket
{
 uutimer resendtime; //time to resend
 void send();
 securepacket_out(netnode *_n,p_secure *_ps,int _size);
 virtual ~securepacket_out(); 
};

#endif

#endif




