//tcp/ip winsock network layer functions

#include <winsock.h>
#include <stdio.h>

#include "glib.h"
#include "input.h"
#include "misc.h"
#include "effect.h"
#include "object.h"
#include "net.h"

#include "netobj.h"

#include "misc.h"

#define IPSERVERPORT 6789
#define IPXSERVERPORT 0x6789
extern int blah;

int networkinstalled=0;
int internetinstalled=0;
int ipxinstalled=0;

//local port info
PORT		   localport_inet=IPSERVERPORT; //local port for server (internet)

PORT		   localport_ipxserver=IPXSERVERPORT; //local port for server (ipx)
PORT		   localport_spx=IPXSERVERPORT+1; //local port for server (spx)
PORT		   localport_ipx=IPXSERVERPORT+2; //local port for unreliable transfer


//local address info
NETADDR  localaddr_inet(AF_INET); //local internet address
NETADDR  localaddr_ipx(AF_IPX);   //local IPX address


//internet sockets for connecting/listening
tcpsocket *gameserver_inet=0; //internet server
tcpsocket *connectsocket=0;  //socket used for initiating new connections over internet

//ipx sockets for connecting/listening
udpsocket *gameserver_ipx=0; //ipx unreliable server (receives broadcasts)
tcpsocket *gameserver_spx=0; //spx   reliable server (receives connections)

NETADDR broadcastaddr(AF_IPX);

netnode *netnodes=0; //list of network nodes

char defaultlocalname[]="Dukeytaster";
char localnodename[16];

int maxudpsize;
UDPPACKET *udppacket;




//**************************
//**************************
//**Network Initialization**
//**************************
//**************************


void initinternet()
{
 if (internetinstalled) return;

 //local machine info
 char           localname[50]; //local name of this machine
 hostent       *localhost; //local host description

 //get local machine address info
 if (gethostname(localname,50)) return;     //get name of local host
 localhost=gethostbyname(localname);
 if (!localhost) {msg.printf(2,"Could not get local internet host information."); return;}
   //get local ip address
 localaddr_inet.setip(*((IPADDR *)localhost->h_addr_list[0]));
 localaddr_inet.setport(IPSERVERPORT);

 //--------SETUP GAME SERVER FOR INTERNET
 //create a socket listening on port
 gameserver_inet=new tcpsocket(AF_INET);
 if (gameserver_inet->error())
  {msg.printf(2,"Unable to create internet socket"); delete gameserver_inet; gameserver_inet=0; return;}

 gameserver_inet->async(WM_SERVERSOCKET);
 while (gameserver_inet->listen(localport_inet) && localport_inet<IPSERVERPORT+10) localport_inet++;
 if (!gameserver_inet->getlocalport())
     {msg.printf(2,"Unable to create a listening socket on internet"); return;}
 //successful!
 msg.printf(2,"TCP/IP: Listening on %s",localaddr_inet.getstr());

 internetinstalled=1;
}


void initipx()
{
 if (ipxinstalled) return;

 //setup ipx listener
 gameserver_ipx=new udpsocket(AF_IPX);
 if (
     gameserver_ipx->error() ||
     gameserver_ipx->async(WM_SERVERSOCKET) ||
     gameserver_ipx->listen(localport_ipxserver)
    ) {msg.printf(2,"Unable to create ipx socket"); delete gameserver_ipx; gameserver_ipx=0; return;}
 msg.printf(2,"IPX: Listening on %s",gameserver_ipx->getlocaladdr()->getstr());

 //setup spx listener
 gameserver_spx=new tcpsocket(AF_IPX);
 if (
     gameserver_spx->error() ||
     gameserver_spx->async(WM_SERVERSOCKET) ||
     gameserver_spx->listen(localport_spx)
    ) {msg.printf(2,"Unable to create spx socket"); delete gameserver_spx; gameserver_spx=0; return;}
// msg.printf(2,"SPX: Listening on %s",gameserver_spx->getlocaladdr()->getstr());

 //get local ethernet(ipx) address
 localaddr_ipx=*gameserver_ipx->getlocaladdr();
 localaddr_ipx.setport(localport_ipx);

 //setup broadcast address
 broadcastaddr.broadcast(localport_ipxserver);

 //successful!
 ipxinstalled=1;
}

void initnetwork()
{
 if (networkinstalled) return;

 //initialize winsock
 WSADATA wsadata;
 if (WSAStartup( 0x101,&wsadata))
   {msg.printf(2,"Unable to initialize winsock."); return;}
 msg.printf(2,"%s",wsadata.szDescription);

 initipx();
 initinternet();

 maxudpsize=wsadata.iMaxUdpDg;
 udppacket=(UDPPACKET *)malloc(maxudpsize);
 udppacket->clear();

 //now, sit on our ass and wait for some fuck to connect to us
 networkinstalled=1;
}

void terminatenetwork()
{
 if (!networkinstalled) return;
 networkinstalled=0;
 internetinstalled=ipxinstalled=0;

 if (gameserver_inet) delete gameserver_inet;
 if (connectsocket) delete connectsocket;

 if (gameserver_ipx) delete gameserver_ipx;
 if (gameserver_spx) delete gameserver_spx;

 while (netnodes) delete netnodes;

 free (udppacket);

 //cleanup winsock
 WSACleanup();

}

//**************************
//**************************
//** Socket Callback functions  **
//**************************
//**************************


//functions for receiving server socket msgs
void wm_serversocket(SOCKET t,int flags)
{
 //we've established a tcp connection
 if (flags==FD_CONNECT)
  {
   if (t!=connectsocket->getsocket()) return;

   if (!netnode::findnode_addr(connectsocket->getremoteaddr()))
    {
     netnode *n=new netnode(connectsocket);
     msg.printf(2,"Connection established to %s",n->getremoteaddr()->getstr());
    }
   connectsocket=0;
  }
 //someone is trying to connect to us
 if (flags==FD_ACCEPT)
  {
   tcpsocket *remote;
   if (t==gameserver_inet->getsocket()) {remote=gameserver_inet->accept();}   else
   if (t==gameserver_spx->getsocket())  {remote=gameserver_spx->accept();} else  return;

   if (!netnode::findnode_addr(remote->getremoteaddr()))
    {
    netnode *n=new netnode(remote);
     msg.printf(2,"Accepted connection from %s",n->getremoteaddr()->getstr());
    }// else
   //if (remote->getremoteaddr()!=localaddr_inet) delete remote;
  }

 //we are receiving an ipx broadcast
 if (flags==FD_READ)
  {
   if (t!=gameserver_ipx->getsocket()) return;
   char t[256];
   NETADDR n;
   if (gameserver_ipx->recvfrom(&n,t,256)!=4) return; //get broadcast data

   msg.printf(2,"Broadcast received from %s",n.getstr());

   if (connectsocket) return; //we're already trying to connect to someone else
   if (n==localaddr_ipx) return; //it's a broadcast from ourselves

   n.setport(localport_spx);

   //if we're not connecting now.. and if node for this guy doesn't exist, we must connect spx!
   if (!netnode::findnode_addr(&n))
    {
     connectsocket=new tcpsocket(AF_IPX);
     connectsocket->async(WM_SERVERSOCKET);
     if (connectsocket->connect(&n) && WSAGetLastError()!=WSAEWOULDBLOCK) //connect
       {msg.printf(2,"Could not create connection socket. %d",WSAGetLastError()); delete connectsocket; connectsocket=0;return;}
     msg.printf(2,"Connecting to %s...",n.getstr() );
    }
  }
}
///functions for errors
void wm_socketerror(SOCKET t,int flags,int error)
{
 if (flags==FD_CONNECT)
  {
   if (connectsocket)
   {
    msg.printf(2,"Unable to establish connection");
    delete connectsocket; connectsocket=0;
   }
   return;
  }

}


//functions for receiving tcp socket msgs
void wm_tcpsocket(SOCKET t,int flags)
{
 if (flags==FD_READ)
  {
   netnode *x=netnode::findnode_tcp(t);
   if (x)  x->recv_tcp();
  }

 if (flags==FD_CLOSE)
  {
   netnode *x=netnode::findnode_tcp(t);
   if (x) delete x;
  }
}


//functions for receiving udp socket msgs
void wm_udpsocket(SOCKET t,int flags)
{
 if (flags==FD_READ)
  {
   netnode *x=netnode::findnode_udp(t);
   if (x)  x->recv_udp();
  }
}




void netdisconnect()
{
 msg.printf(2,"Disconnecting...");
 if (connectsocket) {delete connectsocket; connectsocket=0;}
 while(netnodes) delete netnodes;
}



//attempt to connect to an ip address
void netconnect_inet(char *str)
{
if (!internetinstalled) return;

 //see if this internet address is valid
IPADDR a=inet_addr(str);
if (a==INADDR_NONE || !a) {msg.printf(2,"Invalid ip address: %s",str); return;}

 //it's okay, so use it
NETADDR n(a,IPSERVERPORT);

if (netnode::findnode_addr(&n))
 {
  msg.printf(2,"Already connected to %s",n.getstr());
  return;
 }

if (n==localaddr_inet)
 {
  msg.printf(2,"Cannot connect to self.");
  return;
 }

if (connectsocket)
 {
  msg.printf(2,"Aborting connection attempt");
  delete connectsocket;
  connectsocket=0;
 }

//make connecting socket
connectsocket=new tcpsocket(AF_INET);
connectsocket->async(WM_SERVERSOCKET);
if (connectsocket->connect(&n) && WSAGetLastError()!=WSAEWOULDBLOCK) //connect
  {msg.printf(2,"Could not create connection socket. %d",WSAGetLastError()); delete connectsocket; connectsocket=0;return;}

msg.printf(2,"Connecting to %s...",n.getstr() );
}


//attempt to connect to an ipx address
void netconnect_ipx()
{
 if (!ipxinstalled) return;

 static char t[]="FUCK";
 msg.printf(2,"Broadcasting...");
 gameserver_ipx->sendto(&broadcastaddr,t,4);
}


//**************************
//**************************
//**  Network node stuff  **
//**************************
//**************************

//creates a new node
netnode::netnode(tcpsocket *t):addr()
{
 strcpy(name,"<noname>");

 tcp=t;
   //point tcp msg handler to tcp routine
 tcp->async(WM_TCPSOCKET);
   //get address of this player (tcp)
 addr=*tcp->getremoteaddr();

  //create udp socket for unreliable data send to this node
 udp=new udpsocket(addr.getaf());
 udp->async(WM_UDPSOCKET); //point udp msg handler to udp routine

   //if internet address, bind udp to localaddr_inet and 'connect' to remote addr
 if (addr.getaf()==AF_INET)
  {
   udp->bind(&localaddr_inet);
   addr.setport(localport_inet);
   udp->connect(&addr);
  }
  //if ipx address, bind udp to localaddr_ipx and 'connect' to remote addr at IPX port
 if (addr.getaf()==AF_IPX)
  {
   udp->bind(&localaddr_ipx);
   addr.setport(localport_ipx);
   udp->connect(&addr);
  }

 ro=0; //no remote objects

   //add netnode to END of netnode list, to make it pretty
 prev=next=0;
 for (netnode *i=netnodes; i && i->next; i=i->next);
 if (!i) netnodes=this;
    else {i->next=this; prev=i;}

  //send local name
 TCP_nodename tcpnn(localnodename);
 tcpnn.sendto(this);

    //send local objects
 for (localobject *x=lo; x; x=x->lnext)
  {
   TCP_newobject tcppacket(x,1);
   tcppacket.sendto(this);
  }

}

netnode::~netnode()
{
  //detach from object list
 if (next) next->prev=prev;
 if (prev) prev->next=next;
 if (this==netnodes) netnodes=next;

 msg.printf(2,"Disconnected from %s.",addr.getstr());

  //delete sockets
 delete tcp;
 delete udp;

 //delete the netobjects that this is attached to
 while (ro) delete ro;
}

void netnode::recv_tcp()
{
 static char buf[4096];
 int num=tcp->recv(buf,4096);
 char *b=buf;

  //process all tcppackets
 while (num>0)
  {
   int tcpsize=((TCPPACKET *)b)->size();

   if (!tcpsize)
    {
     msg.printf(1,"Invalid tcp packet! %d Disconnecting...",b[0]);
     netdisconnect();
     return;
    }

   num-=tcpsize;
   if (num<0) //partial packet at the end here
     {
      msg.printf(1,"Partial packet %d",num);
      return;
     };

     //process this tcppacket
   ((TCPPACKET *)b)->process(this);
   b+=tcpsize;
  }
}
void netnode::recv_udp()
{
 static char buf[1024];
 if (!udp->recv(buf,1024)) return;

 if (!((UDPPACKET *)buf)->isvalid()) return;

 //go through all remote objs controlled by this node
 for (remoteobject *t=ro; t; t=t->rnext)
   t->netrecvpos((UDPPACKET *)buf);
}


void netnode::send_tcp(void *b,int n)
{
 tcp->send(b,n);
}

void netnode::send_udp(void *b,int n)
{
 udp->send(b,n);
}

void netnode::sendtoall_tcp(void *b,int n)
{
 for (netnode *x=netnodes; x; x=x->next)
    x->tcp->send(b,n);
}


void netnode::sendtoall_udp(void *b,int n)
{
 for (netnode *x=netnodes; x; x=x->next)
  x->udp->send(b,n);

}

//--------
void netupdate()
{
 udppacket->clear();
 for (localobject *t=lo; t; t=t->lnext)
   t->netsendpos(udppacket);
 udppacket->send();
}


netnode *netnode::findnode_udp(SOCKET s)
{
 for (netnode *x=netnodes; x; x=x->next)
  if (s==x->udp->getsocket()) return x;
 return 0;
}

netnode *netnode::findnode_tcp(SOCKET s)
{
 for (netnode *x=netnodes; x; x=x->next)
  if (s==x->tcp->getsocket()) return x;
 return 0;
}

netnode *netnode::findnode_addr(NETADDR *n)
{
 for (netnode *x=netnodes; x; x=x->next)
  if (*n==*x->getremoteaddr()) return x;
 return 0;
}

int netnode::getnumremote()
{
 int num=0;
 for (remoteobject *t=ro; t; t=t->rnext) num++;
 return num;
}

char *netnode::getinfo()
{
 static char s[80];
 sprintf(s,"%s %s:%s  sent:%d/%d recv:%d/%d",name,getaf()==AF_INET ? "IP" : "IPX",getremoteaddr()->getstr(),tcp->sent,udp->sent,
      tcp->recvd,udp->recvd);
 return s;
}
















