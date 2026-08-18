

//object controlled by local node
class localobject:public object
{
  public:
  localobject *lprev,*lnext;

  unsigned objid; //unique object id on local side

  uchar netupdated:1; //has this netobject been updated since last send?

  localobject(objectdef *objdef,int s,int tx,int ty,int tz,int td, object *c);
  virtual ~localobject();
  void netsendpos(struct UDPPACKET *u);

  virtual void tick()
   {
    object::tick(); //do regular tick
    if (updated) netupdated=1; //it has been updated
   }
  virtual object *spawn(int objnum,int series,int tx,int ty,int tz, int td);

};

extern localobject *lo;

//object controlled by remote node
class remoteobject:public object
{
  public:
  remoteobject *rprev,*rnext;

  //node that this netobject uses to communicate, 0 for local netnode
  class netnode *n;
  unsigned objid; //unique object id as referenced from our netnode

  remoteobject(objectdef *objdef,int s,int tx,int ty,int tz,int td, netnode *tnode, unsigned tobjid);
  virtual ~remoteobject();
  void netrecvpos(struct UDPPACKET *u);
  virtual void tick() {}; //do nothing on ticks
  virtual int advanceframe();
  virtual void effect() { } //remote object can't do effects
  virtual object *spawn(int objnum,int series,int tx,int ty,int tz, int td)
       {return 0;} //remote object can't spawn anything

};



#define UDPPACKETTYPE 69

struct pospacket
{
 ushort objid; //object id
 short int x,y;
 signed char z;
 uchar cf;
 uchar csnum;
 uchar d:1;
};


struct UDPPACKET
{
  uchar type;
  uchar num; //number of pos's
  pospacket p[];

  void clear() {type=UDPPACKETTYPE; num=0;}
  int size() {return sizeof(UDPPACKET)+num*sizeof(pospacket);}
  pospacket *allocate() {return &p[num++];} //allocates a new pospacket
  int isvalid() {if (type==UDPPACKETTYPE) return 1; else return 0;}

  void send(); //sends to all nodes

  pospacket *find(unsigned objid); //finds a pospacket with a particular object id

};

//Tcp Packet crap
struct TCPPACKET
{
 uchar type;

 int size();
 int process(netnode *n);
 int sendtoall();
 int sendto(netnode *);
 TCPPACKET(uchar t) :type(t) {}
};

struct TCP_nodename:TCPPACKET
{
 char name[16];

 int process(netnode *n);
 TCP_nodename(char *s);
};

struct TCP_othernode:TCPPACKET
{
 NETADDR a;

 int process(netnode *n);
 TCP_othernode(netnode *n);
};

struct TCP_newobject:TCPPACKET
{
 ushort	 objid;
 short x,y,z;
 uchar objtype;
 uchar series;
 uchar d:1;
 uchar remote:1; //is it a remote object? or autonomous

 int process(netnode *n);
 TCP_newobject(object *o,int remote); //creates packet based on existing o
};


struct TCP_killobject:TCPPACKET
{
 ushort objid;

 int process(netnode *n);
 TCP_killobject(localobject *o);
};


struct TCP_chat:TCPPACKET
{
 char s[80];

 int process(netnode *n);
 TCP_chat(char *s);
};





