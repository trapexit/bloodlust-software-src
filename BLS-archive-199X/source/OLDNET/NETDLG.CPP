#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "types.h"

#include "r2img.h"
#include "font.h"
#include "dd.h"

#include "mouse.h"
#include "message.h"
#include "gui.h"
#include "guimenu.h"
#include "guicolor.h"


#include "config.h"

#include "net.h"

//default ports
#define IPPORT   6789
#define IPXPORT  25000


extern netconnect *nc; //server (if listening)

extern char localname[];

class startipxserverdlg:public GUIcontents
{
 GUItextedit *name;
 GUInumbertextedit *port;
 public:

 startipxserverdlg():GUIcontents(250,32)
  {
   name=new GUItextedit(this,"Name: ","Shitface",5,3,140,15);
   port=new GUInumbertextedit(this,"IPX Port: ",IPXPORT,5,15,60,0xFFFF);
  };
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
  {
   strcpy(localname,name->getinput());
   new netserver(AF_IPX,port->getstate());
  }

  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"IPX server",new startipxserverdlg(),"Start",0,0));
  };
 virtual ~startipxserverdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS startipxserverdlg::pos;

void m_startipxserver()
{
 startipxserverdlg::open();
}






//------------------------------


class startinetserverdlg:public GUIcontents
{
 GUItextedit *name;
 GUInumbertextedit *port;
 public:

 startinetserverdlg():GUIcontents(250,32)
  {
   name=new GUItextedit(this,"Name: ","Shitface",5,3,140,15);
   port=new GUInumbertextedit(this,"UDP Port: ",IPPORT,5,15,60,0xFFFF);
  };
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
   {
    strcpy(localname,name->getinput());
    new netserver(AF_INET,port->getstate());
   }
  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Internet server",new startinetserverdlg(),"Start",0,0));
  };
 virtual ~startinetserverdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS startinetserverdlg::pos;

void m_startinetserver()
{
 startinetserverdlg::open();
}

//----------------------------------------------


class connectinetdlg:public GUIcontents
{
 GUItextedit *name;
 GUItextedit *ipaddr;
 GUInumbertextedit *port;
 public:

 connectinetdlg():GUIcontents(250,40)
  {
   name=new GUItextedit(this,"Name: ","Shitface",5,3,140,15);
   ipaddr=new GUItextedit(this,"IP Address: ","",5,15,140,16);
   port=new GUInumbertextedit(this,"UDP Port: ",IPPORT,5,27,60,4);
  };
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
   {
    strcpy(localname,name->getinput());
    IPADDR a=inet_addr(ipaddr->getinput());
    if (a==INADDR_NONE || !a) {msg.error("Invalid ip address"); return 1;}
    NETADDR n(a,port->getstate());
    new netclient(&n);
   }
  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Internet",new connectinetdlg(),"Connect",0,0));
  };
 virtual ~connectinetdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS connectinetdlg::pos;

void m_connectinet()
{
 connectinetdlg::open();
}

//-----------------------------------------------

class connectipxdlg:public GUIcontents
{
 GUItextedit *name;
 GUInumbertextedit *port;
 public:

 connectipxdlg():GUIcontents(250,40)
  {
   name=new GUItextedit(this,"Name: ","Shitface",5,3,140,15);
   port=new GUInumbertextedit(this,"IPX Port: ",IPXPORT,5,15,60,4);
  };
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (guimsg==GUIMSG_OK)
   {
    strcpy(localname,name->getinput());
      //setup broadcast address
    NETADDR broadcastaddr(AF_IPX);
    broadcastaddr.setbroadcast(port->getstate());
    new netclient(&broadcastaddr);
   }
  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"IPX",new connectipxdlg(),"Connect",0,0));
  };
 virtual ~connectipxdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS connectipxdlg::pos;

void m_connectipx()
{
 connectipxdlg::open();
}








