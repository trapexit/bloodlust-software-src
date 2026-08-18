//packet layer functiosn
#include "net.h"
#include "netp.h"

#include "message.h"


PACKETFUNCPTR packetfuncptrs[]=
 {
  (PACKETFUNCPTR)p_none      ::process, //P_NONE
  (PACKETFUNCPTR)p_connect   ::process, //P_CONNECT
  (PACKETFUNCPTR)p_accept    ::process, //P_ACCEPT
  (PACKETFUNCPTR)p_quit      ::process, //P_QUIT
  (PACKETFUNCPTR)p_ping      ::process, //P_PING
  (PACKETFUNCPTR)p_pong      ::process, //P_PONG
 };



 //do nothing
void p_none::process(netconnect *c,NETADDR &a,netnode *n) {}

//connect
void p_connect::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (c->isserver())
 if (!n) //node does not exist already
  {
   p_accept p; //reply accept
   p+=c->info; //store local info
   for (netnode *t=nc->n; t; t=t->next) //store all the node infos
       p+=t->info;
   c->send(&a,&p,p.getsize());

   n=new netnode(c,&info); //create node for this new client
   msg.printf(6,"%s has joined",n->getname());
  }
}

//accept
void p_accept::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!c->isclient()) return; //only applies to clients
 if (c->connected) return; //already connected

 netclient *client=(netclient *)c;
 client->connected=1;
 client->serveraddr=a;

 msg.printf(6,"connected to %s...",a.getstr());

 for (int i=0; i<num; i++)
  {
   netnode *n=new netnode(c,&infos[i]);
   msg.printf(6,"%s has joined",n->getname());
  }
}

//quit
void p_quit::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (reason==PQ_SERVERSHUTDOWN)
   {
    msg.printf(6,"Server has shutdown.");
    delete c;
    return;
   }

 if (!n) return; //not a player anyway

 if (reason==PQ_QUIT) msg.printf(6,"%s has quit.",n->getname());
 if (reason==PQ_PINGTIMEOUT) msg.error("%s has timedout.",n->getname());
 delete n;
}

//ping
void p_ping::process(netconnect *c,NETADDR &a,netnode *n)
{
 p_pong p(time);
 c->send(&a,&p,sizeof(p));

 msg.printf(6,"[%s ping]",n ? n->getname() : "???");
}

//pong
void p_pong::process(netconnect *c,NETADDR &a,netnode *n)
{
 unsigned pingtime=uu-time;
 msg.printf(6,"[%s ping reply %d]",n ? n->getname() : "???",pingtime);

 if (c->isclient())
  {
   netclient *client=(netclient *)c; //server has shutdown
   if (a==client->serveraddr)  client->pingtimeout.reset();
   return;
  }
}




