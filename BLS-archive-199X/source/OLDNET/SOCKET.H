#ifndef _SOCKET_
#define _SOCKET_

#include <winsock.h>

typedef unsigned IPADDR;
typedef unsigned short PORT;


//structure for defining an address
struct NETADDR:sockaddr
{
 NETADDR() {};
 NETADDR(int af) {sa_family=af;};
 NETADDR(IPADDR i,PORT p) {sa_family=AF_INET; setip(i); setport(p);};

 int getaf() {return sa_family;}; //gets the address family
 char *getstr();   //gets a text string representing this address
 void setbroadcast(PORT p); //sets to broadcast on a port
 void setaddrany(PORT p); //sets to 000000 on a port
    //gets and sets the port
 PORT getport();
 void setport(PORT p);
 void setip(IPADDR i);
};

int operator ==(NETADDR &a,NETADDR &b);

class tcpsocket;

class Socket {
 int af;
 int protocol;
 SOCKET s;

 public:
 int sent,recvd;

 Socket(int taf,int protocol); //AF_INET/AF_IPX  stream/dgram
 Socket(int taf,SOCKET t) {af=taf; s=t; sent=recvd=0; } //existing socket
 ~Socket();

 int error();

 PORT getlocalport();
 PORT getremoteport();
 NETADDR *getremoteaddr();
 NETADDR *getlocaladdr();
 SOCKET getsocket() {if (!this) return 0; return s;};

 int async(UINT msg,int flags);
 int bind(NETADDR *n);
 int listen(PORT p);
 int connect(NETADDR *n);

 class tcpsocket *accept();

 int send(void *b,int n);
 int recv(void *b,int n);

 int sendto(NETADDR *d,void *b,int n);
 int recvfrom(NETADDR *s,void *b,int n);

};
extern int totalsent,totalrecv;

class tcpsocket:public Socket
{
 public:
 tcpsocket(int af):Socket(af,SOCK_STREAM) {};
};

class udpsocket:public Socket
{
 public:
 udpsocket(int af):Socket(af,SOCK_DGRAM) {};
};

char *getafstr(int af);
#endif






