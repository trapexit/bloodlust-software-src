
#define P_NONE      0
#define P_CONNECT   1
#define P_ACCEPT    2
#define P_QUIT      3

#define P_PING      4
#define P_PONG      5



class packet;
typedef void (packet::*PACKETFUNCPTR)(class netconnect *c,NETADDR &a,class netnode *n);
extern PACKETFUNCPTR packetfuncptrs[];


//general packet type
class packet
{
 char type;
 public:
 packet(char _type):type(_type) {}

 void process(class netconnect *c,NETADDR &a,class netnode *n)
      {(this->*packetfuncptrs[type])(c,a,n);}
};

class p_none:public packet
{
 public:
 p_none():packet(P_NONE) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};


//packet to connect to server
class p_connect:public packet
{
 nodeinfo info; //our info (to be sent to server);
 public:
 p_connect(nodeinfo *i):packet(P_CONNECT),info(*i) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//connection to server is accepted
class p_accept:public packet
{
 int num;
 nodeinfo infos[16]; //infos of all players currently connected
 public:
 p_accept():packet(P_ACCEPT) {num=0;}

 void operator += (nodeinfo &i) {infos[num]=i; num++;}
 int getsize() {return sizeof(p_accept)+num*sizeof(nodeinfo);}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//quit
class p_quit:public packet
{
 #define PQ_SERVERSHUTDOWN 1
 #define PQ_PINGTIMEOUT    2
 #define PQ_QUIT           3
 #define PQ_ERROR          4
 char   reason;
 NETADDR addr; //address of player who quit
 public:
 p_quit(char r,NETADDR *a):packet(P_QUIT),reason(r),addr(*a) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//ping
class p_ping:public packet
{
 unsigned time; //time sent
 public:
 p_ping(unsigned t):packet(P_PING),time(t) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//pong
class p_pong:public packet
{
 unsigned time;
 public:
 p_pong(unsigned t):packet(P_PONG),time(t) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};








