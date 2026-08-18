//winsock network layer functions
#include <stdio.h>
#include <stdlib.h>

#define NETWORK

#include "net.h"
#include "netp.h"
#include "netobj.h"
#include "message.h"

#include "grunt.h"

#include "config.h"

#define DEL(x) if (x) {delete x; x=0;}

//#define NETDIAGNOSE

void netdlgopen();
void netdlgclose();
void netdlgrefresh();

extern int blah;


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
 if (WSAStartup(0x101,&wsadata)) return;
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
 if (nc) nc->disconnect();
}


//---------------------------------------------
//net node

void nodeinfo::setname(char *s)
 {
  if (strlen(s)<16) strcpy(name,s);
    else {memcpy(name,s,15); name[15]=0;}
 }


netnode::netnode(netconnect *c,nodeinfo *i)
 :info(*i),nc(c),
  o(0),objidcount(0),numobj(0),
  pingtimeout(2000),pingtime(0),resends(0),
  pid_out(0),pid_in(0), //next pid
  p_in(0),p_out(0),
  num_in(0),num_out(0)
{
 prev=next=0;

 for (netnode *t=nc->n; t && t->next; t=t->next)
     if (t->next->getnum()>getnum()) break;
 //insert it after
 if (t)
 {
  next=t->next;  if (next) next->prev=this;
  prev=t;        if (prev) prev->next=this;
 } else nc->n=this; //first node
 nc->numnodes++;

 netdlgrefresh();
}

netnode::~netnode()
{
 nc->numnodes--;
 if (prev) prev->next=next; else nc->n=next;
 if (next) next->prev=prev;

 while (p_in) delete p_in;
 while (p_out) delete p_out;

 while (o) delete o;
 netdlgrefresh();
}


void netnode::sendobjpos()
{
 static char buf[512];
 p_objpos *p=new (buf) p_objpos(getnum()); //make objpos packet

  //go through all local objects
 for (netobject *lo=o; lo; lo=lo->nnext)
  if (lo->netupdated) //has it been updated since ?
  {
   objpos *op=p->allocate();
   if (op) lo->sendobjpos(op);
  }
 if (!p->num) return; //no objects need updating

 //send packet now
 nc->sendtoall(p,p->getsize());
}


//-----------------------------------------------
//net connection

netconnect::netconnect(int af,PORT port)
 :s(af),active(0),connected(0),disconnecting(0),
  pingtimer(300),
  n(0),numnodes(0),server(0),local(0),
  objupdatetimer(5) //update every 5 ticks
{
 DEL(nc); //delete old net connection

 buf=(char *)malloc(maxudpsize);
 if (s.error()) {msg.error("unable to create socket"); return;}
 s.async(WM_UDPSOCKET,FD_READ|FD_WRITE);

 //get local address
 NETADDR localaddr(af);
 if (af==AF_IPX) localaddr.setaddrany(port);
 if (af==AF_INET)
  {
   char     localname[50]; //local name of this machine
   hostent *localhost; //local host description
    //get local machine address info
   if (gethostname(localname,50)) return;     //get name of local host
   localhost=gethostbyname(localname);
   if (!localhost) {msg.error("Could not get local internet host information."); return;}

    //get local ip address(s)
   localaddr.setport(port);
   localaddr.setip(*((IPADDR *)localhost->h_addr_list[0]));
  }

  //bind to local address
 if (s.bind(&localaddr)) {msg.error("Unable to bind to %s",localaddr.getstr()); return;}

 //set all of local info
 localinfo.num=0;
 localinfo.a=*s.getlocaladdr();
 localinfo.setname(cfg->localname);
 active=1;
}

netconnect::~netconnect()
{
 free(buf);
 while (n) delete n;   //delete all netnodes
 netdlgclose();
 nc=0;
 endgame();
};


void netserver::disconnect()
{
 if (disconnecting || !connected) return;
 msg.printf(6,"Shutting down server...");
 p_quit p(PQ_SERVERSHUTDOWN,0);   //send quits to all nodes
 sendtoall(&p,sizeof(p));
 active=0;
 disconnecting=1;
// delete this;
}

void netclient::disconnect()
{
 if (disconnecting) return;
 msg.printf(6,"Disconnecting...");
 p_quit p(PQ_QUIT,localinfo.num);
 sendtoall(&p,sizeof(p));
 active=0;
 disconnecting=1;
// delete this;
}





void netconnect::socketrecv(SOCKET t)
{
 NETADDR a;
 if  ((size=s.recvfrom(&a,buf,maxudpsize))>0)
 {
  netnode *n=findnode(&a); //find the node that sent the data
  if (n) n->pingtimeout.reset(); //reset timeout

  packet *p=(packet *)buf;
  p->process(this,a,n);
/// msg.printf(1,"%s received",p->getname());
// msg.printf(1,"%d bytes received",size);
 }

}


void netconnect::socketerror(SOCKET t)
{
 msg.error("Socket error.");
 connected=0;
}

void netnode::sendsecure(struct packet *b,int size)
{
 static char buf[512];
 p_secure *ps=new (buf) p_secure(this,b,size);
 new securepacket_out(this,ps,sizeof(ps)+size);

 #ifdef NETDIAGNOSE
 msg.printf(1,"send pid=%d %s",ps->pid,b->getname());
 #endif
}


void netnode::processsecure_out()
{
 for (securepacket_out *p=p_out; p; p=(securepacket_out *)p->next) //go through all pending outs
  if (p->resendtime.check())
    {
     p->send();
//     msg.printf(1,"resend pid=%d",p->pid);
     resends++;
    }
}

void netnode::processsecure_in()
{
 while (p_in && p_in->pid==pid_in)
  {
   #ifdef NETDIAGNOSE
   msg.printf(2,"process pid=%d (cached) %s",p_in->pid,p_in->ps->getpacketptr()->getname());
   #endif

   //process the pending packet
   p_in->ps->processdata(nc,info.a,this);
   delete p_in;

   pid_in++; //next packet
  }
}

//extern int blah;
void netconnect::tick()
{
 if (disconnecting) {delete this; return;}

// socketrecv(0);

  //send object pos data
 if (objupdatetimer.check())
  {
   local->sendobjpos();
   objupdatetimer.reset();
  }

 //ping our connections
 if (pingtimer.check())
  {
   sendpings();
   checktimeout();
   pingtimer.reset();
  }

 //resend secure packets if needed
 for (netnode *t=n; t; t=t->next)
  if (t!=local) t->processsecure_out();

}

netnode *netconnect::findnode(NETADDR *a)
{
 for (netnode *x=n; x; x=x->next)
  if (x->info.a==*a) return x;
 return 0;
}

netnode *netconnect::findnode(char nodenum)
{
 for (netnode *x=n; x; x=x->next)
  if (x->getnum()==nodenum) return x;
 return 0;
}


void netconnect::sendto(NETADDR *a,struct packet *b,int size)
{
 static char buf[512];
 memcpy(buf,b,size);

 if (s.sendto(a,buf,size)!=size) {msg.error("error sending");}
}

//--------------------------------------------------
//server functions

netserver::netserver(int af,PORT port):netconnect(af,port)
{
 if (!active) return;
 startgame("bgtest",1);

 msg.printf(6,"%s server started: %s",getafstr(af),localinfo.a.getstr());
 local=new netnode(this,&localinfo); //create server node (server=0)

 connected=1;
 nc=this; //this is the current net connection
 netdlgopen();
}
netserver::~netserver()
{
 msg.printf(6,"%s server shutdown",getafstr(localinfo.a.getaf()));
};

//sends to all nodes besides ourself
void netconnect::sendtoall(struct packet *b,int num)
{
 for (netnode *t=n; t; t=t->next)
  if (t!=local) t->send(b,num);//dont send to ourselves
}

void netconnect::sendtoallsecure(struct packet *b,int num)
{
 for (netnode *t=n; t; t=t->next)
  if (t!=local) t->sendsecure(b,num); //dont send to ourselves
}




void netserver::tick()
{
 netconnect::tick();
}

void netserver::sendpings()
{
 p_ping p(uu); //send ping to all nodes
 sendtoall(&p,sizeof(p));
}

void netserver::checktimeout()
{
 for (netnode *tnext,*t=n; t; t=tnext)
  {
   tnext=t->next;
   if (t!=local && t->pingtimeout.check()) //have they ping timedout yet?
     {
      p_quit p(PQ_TIMEOUT,t->getnum()); //send quit notices to all nodes
      sendtoall(&p,sizeof(p));
      msg.error("%s has timedout",t->getname());
      delete t;
     }
  }
}



//--------------------------------------------------
//client functions

netclient::netclient(NETADDR *n)
 :netconnect(n->getaf(),n->getport()),
  connecttimer(200),connecttimeout(1500)
{
 nodeinfo serverinfo;
 serverinfo.num=0; //server is number 0
 serverinfo.setname("-server-");
 serverinfo.a=*n;
 server=new netnode(this,&serverinfo); //create server node

 local=0; //not created yet...until accepted connection
 nc=this;
 msg.printf(6,"Attempting connect to %s...",server->getaddr()->getstr());
 netdlgopen();
}
netclient::~netclient()
{
 if (connected) msg.printf(6,"Closing connection.");
};


void netclient::tick()
{
 if (!connected)  //not connected yet
 {
  if (disconnecting)
   {
    msg.error("Aborting connection attempt");
    delete this;
    return;
   }

  if (connecttimer.check())  //attempt connection
  {   //send connect packet
   p_connect p(&localinfo);
   server->send(&p,sizeof(p));
   connecttimer.reset();
  }
  if (connecttimeout.check()) //time to timeout?
   {
    msg.error("Unable to connect to server");
    disconnect();
    return;
   }
  return;
 }
 netconnect::tick();
}

void netclient::sendpings()
{
 p_ping p(uu);
 sendtoall(&p,sizeof(p)); //send ping to our node friends
}

void netclient::checktimeout()
{
 if (server->pingtimeout.check())
  {
   msg.error("Server timed out");
   connected=0;
   netdisconnect();
   return;
  }
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
// msg.error("unknown socket message");
}
//function for error
void wm_socketerror(SOCKET t,int flags,int error)
{
 if (!nc) return;
 nc->socketerror(t);
}


//----------------------------------
//secure packet shit


securepacket::securepacket(netnode *_n,p_secure *_ps,int _size,securepacket **_list)
  :n(_n),ps(0),size(_size),list(_list)
  {
   //duplicate packet
   ps=(p_secure *)malloc(size);
   memcpy(ps,_ps,size);
   pid=ps->pid;

   //link to list
   securepacket *ip=0; //insertion point
   for (securepacket *t=*list; t; ip=t,t=t->next)
     if (pid < t->pid) break;

   //insert after ip
   next=ip ? ip->next : *list;
   prev=ip;
   if (next) next->prev=this; //else
   if (prev) prev->next=this; else *list=this;
  }

securepacket::~securepacket()
  {
   if (ps) free(ps);

   if (prev) prev->next=next; else *list=next;
   if (next) next->prev=prev;
  }

securepacket_in::securepacket_in(netnode *_n,p_secure *_ps,int _size)
  :securepacket(_n,_ps,_size,(securepacket **)&_n->p_in) {n->num_in++;}

securepacket_in::~securepacket_in() {n->num_in--;}

void securepacket_out::send()
{
 nc->s.sendto(n->getaddr(),ps,size);
 resendtime.reset();
}

securepacket_out::securepacket_out(netnode *_n,p_secure *_ps,int _size)
  :securepacket(_n,_ps,_size,(securepacket **)&_n->p_out),
   resendtime(max((int)_n->pingtime,100)) //resend time
     {send(); n->num_out++;}

securepacket_out::~securepacket_out() {n->num_out--;}








