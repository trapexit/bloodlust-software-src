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


#include "keyb.h"

#include "config.h"

#include "net.h"
#include "netp.h"

//default ports
#define IPPORT   6789
#define IPXPORT  25000


#define DEL(x) if (x) {delete x; x=0;}

extern netconnect *nc; //server (if listening)


class startserverdlg:public GUIcontents
{
 GUItextedit *name;
 GUIstringlistbox *protocol;

 GUInumbertextedit *port;

 GUIstatictext *haha;

 public:
 startserverdlg():GUIcontents(210,70)
  {
   name=new GUItextedit(this,"Name: ",cfg->localname,5,3,140,15);

   new GUIstatictext(this,3,"Protocol:",5,15);
   protocol=new GUIstringlistbox(this,5,25,50,3,10);
   ITEMPTR *a=protocol->resizeitems(3);
   a[0]="IPX"; a[1]="TCP/IP"; a[2]="Modem";
   port=0;
   haha=0;
  };
 virtual void draw(char *dest)
 {fill(CLR_BOX); GUIrect::draw(dest); };
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (c==protocol && guimsg==GUIMSG_LISTBOXSELCHANGED)
   {
    DEL(port);
    DEL(haha);
    switch (protocol->getselnum())
     {
      case 0: //IPX
       port=new GUInumbertextedit(this,"IPX Port: ",IPXPORT,60,25,60,0xFFFF);
       break;
      case 1: //TCP
       port=new GUInumbertextedit(this,"UDP Port: ",IPPORT,60,25,60,0xFFFF);
       break;
      case 2: //Modem
       haha=new GUIstatictext(this,3,"Haha, yeah right.",60,25);
       break;
     }
    return 1;
   }
  if (guimsg==GUIMSG_OK)
  {
   if (strlen(name->getinput())<1)
    {
     msg.error("Local name too short.");
     return 0;
    }

   strcpy(cfg->localname,name->getinput());
   switch (protocol->getselnum())
    {
     case 0: //IPX
       new netserver(AF_IPX,port->getstate());
      return 1;
     case 1: //TCPIP
       new netserver(AF_INET,port->getstate());
      return 1;
     case 2: //Modem
       msg.error("Modem play not implemented yet.");
      return 1;
     default: return 0;
    };
  }
  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Start game server",new startserverdlg(),"Start",0,0));
  };
 virtual ~startserverdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS startserverdlg::pos;

void m_startserver()
{
 if (!nc) startserverdlg::open();
}

//start server from commandline
int cmd_server(char *p)
{
 char type[32];
 int port=0;
 sscanf(p,"%s %s %d",type,&port);
 if (!stricmp(type,"ipx")) //ipx server
  {
   if (!port) port=IPXPORT;
   new netserver(AF_IPX,port);
   return 1;
  }
 if (!stricmp(type,"tcp") || !stricmp(type,"udp") || !stricmp(type,"inet")) //ip server
  {
   if (!port) port=IPPORT;
   new netserver(AF_INET,port);
   return 1;
  }
 return 0;
}




//----------------------------------------------


class connectdlg:public GUIcontents
{
 GUItextedit *name;
 GUIstringlistbox *protocol;

 GUInumbertextedit *port;
 GUItextedit *addr;
 GUIstatictext *haha;

 public:
 connectdlg():GUIcontents(210,70)
  {
   name=new GUItextedit(this,"Name: ",cfg->localname,5,3,140,15);

   new GUIstatictext(this,3,"Protocol:",5,15);
   protocol=new GUIstringlistbox(this,5,25,50,3,10);
   ITEMPTR *a=protocol->resizeitems(3);
   a[0]="IPX"; a[1]="TCP/IP"; a[2]="Modem";
   port=0;  addr=0;
   haha=0;
  };
 virtual void draw(char *dest) {fill(CLR_BOX); GUIrect::draw(dest);};
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (c==protocol && guimsg==GUIMSG_LISTBOXSELCHANGED)
   {
    DEL(port);
    DEL(addr);
    DEL(haha);
    switch (protocol->getselnum())
     {
      case 0: //IPX
       port=new GUInumbertextedit(this,"IPX Port: ",IPXPORT,60,25,60,0xFFFF);
       break;
      case 1: //TCP
       port=new GUInumbertextedit(this,"UDP Port: ",IPPORT,60,25,60,0xFFFF);
       addr=new GUItextedit(this," IP Addr: ","",60,38,90,16);
       break;
      case 2: //modem
       haha=new GUIstatictext(this,3,"Haha, yeah right.",60,25);
     }
    return 1;
   }
  if (guimsg==GUIMSG_OK)
  {
   if (strlen(name->getinput())<1)
    {
     msg.error("Local name too short.");
     return 0;
    }
   strcpy(cfg->localname,name->getinput());
   switch (protocol->getselnum())
    {
     case 0: //IPX
      {
        //setup broadcast address
       NETADDR broadcastaddr(AF_IPX);
       broadcastaddr.setbroadcast(port->getstate());
       new netclient(&broadcastaddr);
       return 1;
      }
     case 1: //TCPIP
      {
       IPADDR a=inet_addr(addr->getinput());
       if (a==INADDR_NONE || !a) {msg.error("Invalid ip address"); return 0;}
       NETADDR n(a,port->getstate());
       new netclient(&n);
       return 1;
      }
     case 2: //Modem
       msg.error("Modem play not implemented yet.");
      return 1;
     default: return 0;
    };
  }
  return 1;
 }
 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIonebuttonbox(guiroot,"Connect",new connectdlg(),"Connect",0,0));
  };
 virtual ~connectdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS connectdlg::pos;

void m_connect() {if (!nc) connectdlg::open();}




//---------------------------------------

class netnodelistbox:public GUIlistbox
{
 public:
 virtual void drawitems(char *dest, int x,int y)
 {
  for (int j=itemv,i=scroll->getpos(); j>0 && i<numitems; j--,i++,y+=itemheight)
  {
   if (sel==i) drawrect(dest,2,x,y,width()-2,itemheight);
   if (items[i])
    {
     netnode *t=(netnode *)items[i];

     int c=1; //default color
//     if (t->getnum()==0) c=3;
     if (t==nc->local) c=2;

     char s[80];
     font[c]->draw(itoa(t->getnum(),s,10),dest,x,y);
     font[c]->draw(t->getname(),dest,x+12,y);

     font[c]->draw(itoa(t->pingtime,s,10),dest,x+115,y);
     font[c]->draw(itoa(t->resends,s,10),dest,x+155,y);
     font[c]->draw(itoa(t->numobj,s,10),dest,x+195,y);
    }
  }
 }

 netnodelistbox(GUIrect *p,int x,int y,int xw,int iy)
  :GUIlistbox(p,x,y,xw,iy,10) {};
 netnode *getselptr() {return (netnode *)GUIlistbox::getselptr();}

 virtual char *getname() {return "netnodelistbox";}
};


class netdlg *NETDLG=0;

class netdlg:public GUIcontents
{
 netnodelistbox *nodelist;
 GUIstatictext *addr;
 GUIintedit *update;

 public:
 void refreshnetnodes()
  {
   if (!nc)  {nodelist->resizeitems(0); return;}

   int num=nc->numnodes;
   ITEMPTR *a=nodelist->resizeitems(num);
   int i=0;
   for (netnode *t=nc->n; t && i<num; t=t->next, i++)
       a[i]=(ITEMPTR)t;
//   if (serveraddr)
//    serveraddr->settext(nc->server->getaddr()->getstr());
  }

 netdlg():GUIcontents(250,115)
  {
   addr=0;
   new GUIstatictext(this,3,"# Name",5,3);
   new GUIstatictext(this,3,"Ping",5+110,3);
   new GUIstatictext(this,3,"Retry",5+150,3);
   new GUIstatictext(this,3,"#Objs",5+190,3);

   update=new GUIintedit(this,"Update rate: ",125,95,20,
    nc->objupdatetimer.dur,1,65535);

   nodelist=new netnodelistbox(this,5,13,width()-10,8);
   refreshnetnodes();

   NETDLG=this;

  };
 virtual void draw(char *dest)
  {
   fill(CLR_BOX); GUIrect::draw(dest);
   char s[64];
   sprintf(s,"Sent:%d Recv:%d",nc->s.sent,nc->s.recvd);
   font[2]->draw(s,dest,x1+10,y1+95);
  };
 virtual int acceptfocus() {return 1;}
 virtual int sendmessage(GUIrect *c,int guimsg)
 {
  if (c==update && guimsg==GUIMSG_EDITCHANGED)
   {
    nc->objupdatetimer.set(update->get());
    return 1;
   }

  if (c==nodelist && guimsg==GUIMSG_LISTBOXSELCHANGED)
   {
    DEL(addr);

    netnode *t=(netnode *)nodelist->getselptr();
    if (!t) return 1;

    char s[80];
    sprintf(s,"%s Addr: %s",getafstr(t->getaddr()->getaf()),t->getaddr()->getstr());
    addr=new GUIstatictext(this,1,s,5,106);
    return 1;
   }

  if (guimsg==GUIMSG_OK) //shutdown
   {
    netdisconnect();//DEL(nc);
    return 0;
   }
  return 0;
 }

 virtual int keyhit(char kbscan,char key)
  {
   if (kbscan==KB_F1)
    {
     netnode *t=nodelist->getselptr();
     if (!t) return 0;

     //send shit to him
     p_none p;
     t->sendsecure(&p,sizeof(p));
     return 1;
    }
   if (kbscan==KB_F2)
    {
     netnode *t=nodelist->getselptr();
     if (!t) return 0;

     msg.printf(3,"pending in:");
     for (securepacket *s=t->p_in; s; s=s->next)
        msg.printf(3,"%d",s->pid);
     return 1;
    }
   if (kbscan==KB_F3)
    {
     netnode *t=nodelist->getselptr();
     if (!t) return 0;
     msg.printf(3,"pending out:");
     for (securepacket *s=t->p_out; s; s=s->next)
        msg.printf(3,"%d",s->pid);
     return 1;
    }
   return 0;
  }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   if (!nc) return;
   if (nc->isserver())
    {
     pos.open(new GUIonebuttonbox(guiroot,"Game server",new netdlg(),"Shutdown",0,0));
     return;
    }
   if (nc->isclient())
    {
     pos.open(new GUIonebuttonbox(guiroot,"Game client",new netdlg(),"Disconnect",0,0));
     return;
    }
  };
 virtual ~netdlg() {NETDLG=0; pos.close((GUIbox *)parent);}
};
DLGPOS netdlg::pos;

void netdlgclose()
{
 if (NETDLG) delete netdlg::pos.opened;
}

void netdlgopen()
{
 if (!nc) return;
// netdlgclose();
 netdlg::open();
}

void netdlgrefresh()
{
 if (NETDLG) NETDLG->refreshnetnodes();
}


//----------------------------------------------------------
//chat window



msgbufferwrap netchat(260-12); //netwokr chat buffer (wrapped)


GUIrect *newguimessage(msgbuffer *tm);

#define MSGYW 8 //height in lines



class netchatdlg:public GUIcontents
{
 GUItextedit *prompt;

 public:
 netchatdlg():GUIcontents(261,MSGYW*10+10)
 {
  newguimessage(&netchat)->reparent(this);
  prompt=new GUItextedit(this,"",0,0,MSGYW*10,width(),80);
 }
 virtual int acceptfocus() {return 1;}

 virtual int keyhit(char kbscan, char key)
 {
  if (kbscan==KB_ENTER)
   {
    char *s=prompt->getinput();
    if (!strlen(s)) return 0;

    if (nc)
    {
     p_chat p(nc->localinfo.num,s);
     nc->sendtoallsecure(&p,p.getsize());
    }
    netchat.printf(1,"> %s",s);
    prompt->setinput("");
    return 1;
   }
  return GUIrect::keyhit(kbscan,key);
 }

 //open functions
 static DLGPOS pos; //saved last position of dialog
 static void open()
  {
   pos.open(new GUIbox(guiroot,"Network chat",new netchatdlg(),0,0));
  };
 virtual ~netchatdlg() {pos.close((GUIbox *)parent);}
};
DLGPOS netchatdlg::pos;


void m_opennetchat()
{
 netchatdlg::open();
}

void m_closenetchat()
{
 DEL(netchatdlg::pos.opened);
}
























