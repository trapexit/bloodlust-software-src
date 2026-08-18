//winsock network layer functions
#include <stdio.h>

#define NETWORK

#include "net.h"
#include "netp.h"
#include "message.h"

#define IPPORT    6789
#define IPXPORT 0x6789


#define DEL(x) if (x) {delete x; x=0;}

extern int blah;

char localname[16]="Shitbag";

int winsockinstalled=0;
int maxudpsize;

netconnect *nc=0; //current net connection

//--------------------------------------------
//Winsock init/terminate

void initwinsock()
{
 if (winsockinstalled) return;

 //initialize winsock
 WSADATA wsadata;
 if (WSAStartup( 0x101,&wsadata)) return;
 msg.printf(6,"%s",wsadata.szDescription);

 maxudpsize=wsadata.iMaxUdpDg;
 winsockinstalled=1;
}

void terminatewinsock()
{
 if (!winsockinstalled) return;
 netdisconnect();
 WSACleanup(); //cleanup winsock
 winsockinstalled=0;
}

void netdisconnect()
{
 DEL(nc); //delete network connection
}


//---------------------------------------------
//net node

void nodeinfo::setname(char *s)
 {
  if (strlen(s)<16) strcpy(name,s);
    else {memcpy(name,s,15); name[15]=0;}
 }


netnode::netnode(netconnect *c,nodeinfo *i)
 :info(*i),nc(c),pingtimeout(1000)
{
 prev=next=0;

 for (netnode *t=nc->n; t && t->next; t=t->next); //find last node
 //insert it after
 if (t) {t->next=this; prev=t;}
    else {nc->n=this;}
}

netnode::~netnode()
{
 if (prev) prev->next=next; else nc->n=next;
 if (next) next->prev=prev;
}

//-----------------------------------------------
//net connection

netconnect::netconnect(int af,PORT port)
 :s(af),active(0),connected(0),n(0),
  pingtimer(500),pingtimeout(1000)
{
 DEL(nc); //delete old net connection

 buf=(char *)malloc(maxudpsize);
 if (s.error()) {msg.error("unable to create socket"); return;}
 s.async(WM_UDPSOCKET,FD_READ);

 NETADDR localaddr(af);
 if (af==AF_IPX) localaddr.setaddrany(port);
 if (af==AF_INET)
  {
   char           localname[50]; //local name of this machine
   hostent       *localhost; //local host description
    //get local machine address info
   if (gethostname(localname,50)) return;     //get name of local host
   localhost=gethostbyname(localname);
   if (!localhost) {msg.error("Could not get local internet host information."); return;}
    //get local ip address
   localaddr.setip(*((IPADDR *)localhost->h_addr_list[0]));
   localaddr.setport(port);
  }

 if (s.bind(&localaddr)) {msg.error("Unable to bind to %s",localaddr.getstr()); return;}

 //set all of local info
 info.a=*s.getlocaladdr();
 info.setname(localname);
 active=1;

 nc=this; //this is the current net connection
}

netconnect::~netconnect()
{
 free(buf);
 while (n) delete n;
 nc=0;
};

void netconnect::socketrecv(SOCKET t)
{
 if (t!=s.getsocket()) return;

 NETADDR a;
 if (!s.recvfrom(&a,buf,maxudpsize)) return;

 netnode *n=findnode(&a); //find the node that sent the data
 if (n) n->pingtimeout.reset();

 ((packet *)buf)->process(this,a,n);
// msg.printf(1,"%d bytes received",size);
}


void netconnect::socketerror(SOCKET t)
{
 if (t!=s.getsocket()) return;
 msg.error("Socket error.");
 connected=0;
 delete this;
}

void netconnect::tick()
{
}

netnode *netconnect::findnode(NETADDR *a)
{
 for (netnode *x=n; x; x=x->next)
  if (x->info.a==*a) return x;
 return 0;
}


//--------------------------------------------------
//server functions

netserver::netserver(int af,PORT port):netconnect(af,port)
{
 if (!active) return;
 msg.printf(6,"%s server: %s",getafstr(af),info.a.getstr());
}
netserver::~netserver()
{
 if (!active) return;
 p_quit p(PQ_SERVERSHUTDOWN,&info.a);   //send quits to all nodes
 for (netnode *t=n; t; t=t->next)
   send(t,&p,sizeof(p));

 msg.printf(6,"%s server shutdown.",getafstr(info.a.getaf()));
};


void netserver::tick()
{
 //we are connected
 if (pingtimer.check())
  {
   p_ping p(uu); //send ping to all nodes
   for (netnode *tnext,*t=n; t; t=tnext)
   {
    tnext=t->next;
    send(t,&p,sizeof(p));
    if (pingtimeout.check()) //have they ping timedout yet?
     {
      msg.error("%s has ping timedout",t->getname());
      delete t;
     }
   }
   pingtimer.reset();
  }
}

//--------------------------------------------------
//client functions

netclient::netclient(NETADDR *n)
 :netconnect(n->getaf(),n->getport()),serveraddr(*n),
  connecttimer(100),connecttimeout(1000)
{
 msg.printf(6,"Attempting connect to %s...",serveraddr.getstr());
}
netclient::~netclient()
{
 if (!connected)  {msg.error("Aborting connection"); return;}

 p_quit p(PQ_QUIT,&info.a);
 send(&serveraddr,&p,sizeof(p));
 msg.printf(6,"Closing connection.");
};


void netclient::tick()
{
 if (!connected) //not connected yet
 {
  if (connecttimer.check())  //send connection packet
  {
   //send connect packet
   p_connect p(&info);
   send(&serveraddr,&p,sizeof(p));

   msg.printf(6,"sending connect packet");
   connecttimer.reset();
   return;
  }
  if (connecttimeout.check()) //time to timeout?
   {
    msg.error("Unable to connect to server");
    delete this;
    return;
   }
  return;
 }

 //we are connected
 if (pingtimer.check())
  {
   p_ping p(uu);
   send(&serveraddr,&p,sizeof(p)); //send ping to server
   pingtimer.reset();
  }
 if (pingtimeout.check())
  {
   msg.error("Server pinged out");
   connected=0;
   delete this;
   return;
  }

 netconnect::tick();
}


//------------------------------------------------
// socket functions

//functions for receiving udp socket dgrams
void wm_udpsocket(SOCKET t,int flags)
{
 if (!nc) return; //no connection...strange
 if (flags==FD_READ)
  {
   nc->socketrecv(t);
   return;
  }
}
//function for error
void wm_socketerror(SOCKET t,int flags,int error)
{
 if (!nc) return;
 nc->socketerror(t);
}















