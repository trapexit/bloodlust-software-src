typedef unsigned IPADDR;
typedef unsigned short PORT;


//structure for defining an address on the internet
//backwards compatible with winsock sockaddr struct
struct NETADDR:sockaddr
{
 int getaf() {return sa_family;}; //gets the address family
 char *getstr();   //gets a text string representing this address
 void broadcast(PORT p); //sets to broadcast on a port
    //gets and sets the port
 PORT getport();
 void setport(PORT p);
};





char *ip2str(IPADDR a);

class Socket {
 SOCKET s;

 public:
 int sent,recvd;

 Socket(int type); //stream/dgram
 Socket(SOCKET t) {s=t; sent=recvd=0; } //existing socket
 ~Socket();

 PORT getport();
 PORT getremoteport();
 void getremoteaddr(NETADDR *n);
 void getlocaladdr(NETADDR *n);
 SOCKET getsocket() {return s;};

 int async(UINT msg);
 int bind(IPADDR a,PORT p);
 int listen(PORT p);
 int connect(IPADDR a,PORT p);
 int connect(char *str,PORT p);
 class tcpsocket *accept();
 int send(void *b,int n);
 int recv(void *b,int n);

};

class tcpsocket:public Socket
{
 public:
 tcpsocket():Socket(SOCK_STREAM) {};

};

class udpsocket:public Socket
{
 public:
 udpsocket():Socket(SOCK_DGRAM) {};

};
