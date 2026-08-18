//packet layer functiosn
#include "net.h"
#include "netp.h"

#include "message.h"
#include "grunt.h"

#include "netobj.h"

//#define NETDIAGNOSE

void netdlgrefresh();

char *packetnames[]=
{
 "none",
 "secure",
 "ack",
 "connect",     
 "accept",
 "newnode",
 "quit",
 "ping",
 "pong",
 "getgameinfo",
 "startgame",

 "newobject",
 "killobject",
 "objpos",
 "chat"
};


PACKETFUNCPTR packetfuncptrs[]=
 {
  (PACKETFUNCPTR)p_none      ::process, //P_NONE
  (PACKETFUNCPTR)p_secure    ::process, //P_SECURE
  (PACKETFUNCPTR)p_ack       ::process, //P_ACK

  (PACKETFUNCPTR)p_connect   ::process, //P_CONNECT
  (PACKETFUNCPTR)p_accept    ::process, //P_ACCEPT
  (PACKETFUNCPTR)p_newnode   ::process, //P_NEWNODe
  (PACKETFUNCPTR)p_quit      ::process, //P_QUIT
  (PACKETFUNCPTR)p_ping      ::process, //P_PING
  (PACKETFUNCPTR)p_pong      ::process, //P_PONG

  (PACKETFUNCPTR)p_getgameinfo::process, //P_GETGAMEINFO
  (PACKETFUNCPTR)p_startgame ::process, //P_STARTGAME
  (PACKETFUNCPTR)p_newobject ::process, //P_NEWOBJECT
  (PACKETFUNCPTR)p_killobject::process, //P_KILLOBJECT
  (PACKETFUNCPTR)p_objpos    ::process, //P_OBJPOS
  (PACKETFUNCPTR)p_chat      ::process, //P_CHAT
  (PACKETFUNCPTR)p_none      ::process, //P_NONE
 };



 //do nothing
void p_none::process(netconnect *c,NETADDR &a,netnode *n) {}



//---------------------------------------------------------

p_secure::p_secure(netnode *n,void *b,int size)
 :packet(P_SECURE)
{
 pid=n->pid_out++; //get next pid
 memcpy(getpacketptr(),b,size); //copy actual packet data over
}

void p_secure::processdata(class netconnect *c,NETADDR &a,class netnode *n)
{
 getpacketptr()->process(c,a,n);
}


//node receives a secure packet
void p_secure::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!n)  return; //must be from node

 p_ack p(pid);
 n->send(&p,sizeof(p)); //send ack

 if (pid<n->pid_in)
  {
//   msg.printf(1,"processed pid=%d already",pid);
   return; //we've received and processed this before
  }
 if (pid==n->pid_in) //this is next in line
  {
   processdata(c,a,n);
   n->pid_in++;
   #ifdef NETDIAGNOSE
   msg.printf(1,"process pid=%d %s",pid,getpacketptr()->getname());
   #endif
   if (n->p_in) n->processsecure_in(); //keep processing...
   return;
  }
 //this packet should be processed later....

 for (securepacket_in *t=n->p_in; t; t=(securepacket_in *)t->next)
  if (t->pid==pid)
   {
//    msg.printf(1,"process pid=%d",pid);
    return; //this packet is already pending
   }


  //create pending packet to be processed
 new securepacket_in(n,this,c->size);
}

//node receives an ack after sending secure packet
void p_ack::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!n)  return; //must be to node

  //go through all send-pending packets
 for (securepacket *t=n->p_out; t; t=t->next)
  if (t->pid==pid) //this pending packet has been ack'ed
   {
    #ifdef NETDIAGNOSE
    msg.printf(1,"ack pid=%d",pid);
    #endif
    delete t;     //so delete it!
    return;
   }
 #ifdef NETDIAGNOSE
 msg.printf(1,"ack pid=%d (notfound)",pid);
 #endif

}


//--------------------------------------------------------

//connect
//client->server
void p_connect::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (c->isclient()) return;

 if (!n)// return; //node does not exist already
 {
  //find unused node num
  for (info.num=1; c->findnode(info.num); info.num++);

  //tell other nodes about connection
  p_newnode pn(info);
  c->sendtoallsecure(&pn,sizeof(pn));

  //reply accept
  p_accept pa; //reply accept
  pa.num=info.num;
  n=new netnode(c,&info); //create node for this new client
  n->send(&pa,sizeof(pa));

  msg.printf(6,"%s has joined",n->getname());
 } else //already exists
 {
  //reply accept
  p_accept pa; //reply accept
  pa.num=n->getnum();
  n->send(&pa,sizeof(pa));
 }

}

//accept
//server->client
void p_accept::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!c->isclient()) return; //only applies to clients
 if (c->connected) return; //already connected

 c->connected=1;
 c->localinfo.num=num; //set our player number

 msg.printf(6,"connected to %s!",a.getstr());

// c->server->info=
 c->server->info.a=a; //store server address
 c->server->pingtimeout.reset();

 c->local=new netnode(c,&c->localinfo); //create local game node

// msg.printf(6,"requesting list of players...");
 p_getgameinfo p;
 c->server->sendsecure(&p,sizeof(p));
}

 //startgame("bgtest",1);

//quit
//signals when a node has left the game
//server<->client
void p_quit::process(netconnect *c,NETADDR &a,netnode *n)
{
 n=c->findnode(num); //get player number
 if (!n) return; //not a player anyway

 if (n==c->local)
  {
   c->disconnect();
   return;
  }

 //quit node
 switch(reason)
 {
  case PQ_SERVERSHUTDOWN:
    msg.printf(6,"Server has shutdown.");
    c->disconnect();
    //netdisconnect();
    //delete c;
    return;
  case PQ_QUIT:
    msg.printf(6,"%s has quit.",n->getname());
    delete n;
   break;
  case PQ_TIMEOUT:
    msg.error("%s has timedout.",n->getname());
    delete n;
   break;
 };

 //tell other nodes about quit
 c->sendtoallsecure(this,sizeof(this));

}

//get nodes (c->s)
//requests all nodes to be sent
void p_getgameinfo::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!c->isserver()) return;
 if (!n) return;

 //send info about game level
 p_startgame pgame;
 strcpy(pgame.bgname,"bgtest");
 n->sendsecure(&pgame,sizeof(pgame));

 //send info about all nodes
// for (netnode *t=c->n->next; t; t=t->next)
 for (netnode *t=c->n; t; t=t->next)
  if (t!=n) //dont send info about the requester himself
  {
   p_newnode p(t->info);
   n->sendsecure(&p,sizeof(p));
   //send info about all objects in node
   for (netobject *o=t->o; o; o=o->nnext)
     {
      p_newobject p(o,t->getnum(),REMOTEOBJECT);
      n->sendsecure(&p,sizeof(p));
     }
  }
}


//new node (s->c)
//new node has joined game
void p_newnode::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!c->isclient()) return;
 n=c->findnode(info.num);
 if (n) //already exists
  {
   n->info=info; //copy info over
   return;
  }
 n=new netnode(c,&info);
 msg.printf(6,"%s has joined.",n->getname());
 netdlgrefresh();
}

//startgame (s->c)
//start new game
void p_startgame::process(netconnect *c,NETADDR &a,netnode *n)
{
 startgame(bgname,1);
 msg.printf(6,"Starting bg %s",bgname);
}


//---------------------------------------

//ping
//client<->server
void p_ping::process(netconnect *c,NETADDR &a,netnode *n)
{
 p_pong p(time);
 c->sendto(&a,&p,sizeof(p));
// msg.printf(6,"[%s ping]",n ? n->getname() : "???");
}

//pong
//client<->server
void p_pong::process(netconnect *c,NETADDR &a,netnode *n)
{
 unsigned pingtime=uu-time;
 if (n) n->pingtime=pingtime;
// msg.printf(6,"[%s ping reply %d]",n ? n->getname() : "???",pingtime);
}


extern msgbufferwrap netchat;

//chat
//client<->server
void p_chat::process(netconnect *c,NETADDR &a,netnode *n)
{
 if (!n) return;
 netchat.printf(1,"<%s> %s",n->getname(),s);
}

