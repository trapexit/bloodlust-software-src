
#define P_NONE      0
#define P_SECURE    1
#define P_ACK       2

#define P_CONNECT   3
#define P_ACCEPT    4
#define P_NEWNODE   5
#define P_QUIT      6

#define P_PING      7
#define P_PONG      8

#define P_GETGAMEINFO  9
#define P_STARTGAME   10

#define P_NEWOBJECT   11
#define P_KILLOBJECT   12
#define P_OBJPOS   13

#define P_CHAT   14


struct packet;
typedef void (packet::*PACKETFUNCPTR)(class netconnect *c,NETADDR &a,class netnode *n);
extern PACKETFUNCPTR packetfuncptrs[];

extern char *packetnames[];


//general packet type
struct packet
{
 char type;

 packet(char _type):type(_type) {}

 void process(class netconnect *c,NETADDR &a,class netnode *n)
      {(this->*packetfuncptrs[type])(c,a,n);}
 char *getname() {return packetnames[type];}
};

struct p_none:public packet
{
 p_none():packet(P_NONE) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//-------------------------------------
//secure transfer headers

struct p_secure:public packet
{
 ushort pid; //packet id
 packet *getpacketptr() {return (packet *)(this+1);}

 p_secure(class netnode *n,void *b,int size);

 void process(class netconnect *c,NETADDR &a,class netnode *n);
 void processdata(class netconnect *c,NETADDR &a,class netnode *n);

 void * operator new(size_t, void * buf) {return buf;}

};

struct p_ack:public packet
{
 ushort pid; //packet id

 p_ack(ushort packetid):packet(P_ACK),pid(packetid) {};
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};


//-------------------------------------
//node connection packets

//packet to connect to server
struct p_connect:public packet
{
 nodeinfo info; //our info (to be sent to server);

 p_connect(nodeinfo *i):packet(P_CONNECT),info(*i) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//connection to server is accepted
struct p_accept:public packet
{
 char num; //assigned number for client

 p_accept():packet(P_ACCEPT) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};


//quit
struct p_quit:public packet
{
 #define PQ_SERVERSHUTDOWN 1
 #define PQ_TIMEOUT        2
 #define PQ_QUIT           3
 #define PQ_ERROR          4
 char reason; //reason for quitting
 char num;    //player number

 p_quit(char r,char n):packet(P_QUIT),reason(r),num(n) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//new person has joined (s->c)
struct p_newnode:public packet
{
 nodeinfo info;

 p_newnode(nodeinfo &i):packet(P_NEWNODE) {info=i;}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//---------------------------------------------------
//game management

//request list of all nodes (c->s)
struct p_getgameinfo:public packet
{
 p_getgameinfo():packet(P_GETGAMEINFO) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//start game (s->c)
struct p_startgame:public packet
{
 char bgname[16]; //name of the bg to use

 p_startgame():packet(P_STARTGAME) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//--------------------------------------------------------------
//game objects

//new object (s<->c) (c<->c)
struct p_newobject:public packet
{
  //make independant nonnettable object
 #define NOTNETOBJECT 0
   //instructs receiver to create remote object from this
 #define REMOTEOBJECT 1
  //instructs receiver to create a local object
 #define LOCALOBJECT 2
 uchar type; //net type
 uchar node;
 ushort objid;
 uchar onum;
 short x,y,z;
 uchar series;
 uchar d:1;

 p_newobject(class object *o,uchar _node,uchar _type); //create new p_newobject from o
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};


//object position data for one object
struct objpos
{
 ushort objid; //object id
 uchar cf;
 uchar csnum;
 short int x,y;
 signed char z:7;
 uchar d:1;
};


struct p_objpos:packet
{
 uchar node; //node #
 uchar num; //number of objpos data's

 objpos *getop() {return (objpos *)(this+1);}

 p_objpos(uchar _node):packet(P_OBJPOS),node(_node),num(0) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);

 objpos *allocate()  { return num<15 ? &getop()[num++] : 0; } //allocate new objpos
 int getsize() {return sizeof(p_objpos)+num*sizeof(objpos);}

 void * operator new(size_t, void * buf) {return buf;}
};


//kill object (s<->c) (c<->c)
struct p_killobject:packet
{
 uchar node;
 ushort objid;
 p_killobject(uchar _node,ushort _objid):
     packet(P_KILLOBJECT),objid(_objid),node(_node) {};
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//--------------------------------------------------
//ping/pong

//ping
struct p_ping:public packet
{
 unsigned time; //time sent

 p_ping(unsigned t):packet(P_PING),time(t) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};

//pong
struct p_pong:public packet
{
 unsigned time;

 p_pong(unsigned t):packet(P_PONG),time(t) {}
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};


//chat (s<->c) (c<->c)
struct p_chat:packet
{
 uchar node;
 char s[128];

 int getsize() {return sizeof(packet)+1+strlen(s)+1;}
 p_chat(uchar _node,char *chatmsg):
     packet(P_CHAT),node(_node) {strcpy(s,chatmsg);};
 void process(class netconnect *c,NETADDR &a,class netnode *n);
};






