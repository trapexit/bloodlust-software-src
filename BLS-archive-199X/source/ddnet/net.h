#include "socket.h"

#define WM_SERVERSOCKET (WM_USER+0)
#define WM_TCPSOCKET (WM_USER+1)
#define WM_UDPSOCKET (WM_USER+2)

extern int networkinstalled;

void initnetwork();
void terminatenetwork();
void netconnect_inet(char *); //connect to ip
void netconnect_ipx(); //essentially broadcast
void netdisconnect();

void netupdate();

void wm_serversocket(SOCKET s,int flags);
void wm_tcpsocket(SOCKET s,int flags);
void wm_udpsocket(SOCKET s,int flags);

void wm_socketerror(SOCKET t,int flags,int error);

//node for an internet player
class netnode {
 public:
 netnode *prev,*next;
 private:
 NETADDR addr;  //ip/ipx address of this player

 udpsocket *udp; //udp socket for sending unreliable packets to this node
 tcpsocket *tcp; //tcp socket for sending   reliable packets to this node

 public:
 char name[16]; //name of player

 class remoteobject *ro; //linked list of remote objects controlled by this node

 //constructor
 netnode(tcpsocket *t);
 ~netnode();

 NETADDR *getremoteaddr() {return &addr;};
 int getaf() {return addr.getaf();}


 void recv_tcp();
 void recv_udp();

 void send_tcp(void *b,int num);
 void send_udp(void *b,int num);

 static void sendtoall_tcp(void *b,int n);
 static void sendtoall_udp(void *b,int n);
 static netnode *findnode_udp(SOCKET s);
 static netnode *findnode_tcp(SOCKET s);
 static netnode *findnode_addr(NETADDR *a);

 int getnumremote(); //calculate the number of remote objects

 char *getinfo();
};


extern netnode *netnodes;






