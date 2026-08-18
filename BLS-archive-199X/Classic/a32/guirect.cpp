#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"

#include "r2img.h"
#include "font.h"
#include "dd.h"

#include "mouse.h"

#include "keyb.h"

#include "guirect.h"
#include "guiroot.h"
#include "guicolor.h"

//default x and y for box startings
int GUIdefx=15,GUIdefy=15;

void resetGUIdef()
{
 GUIdefx=SCREENX/14;
 GUIdefy=SCREENY/14;
}

void nextGUIdef()
{
 GUIdefx+=20; GUIdefy+=20;
}


//*************************************
//           Basic GUI rect
//*************************************

GUIrect *GUIrect::modal=0;
void GUIrect::setmodal(GUIrect *newmodal)
{
 modal=newmodal;
 m.capture=modal;
};


void GUIrect::reparent(GUIrect *p)
{
 if (parent) unlink();
 parent=p;
 if (!parent) return;
  //relative coordinates
 moverel(parent->x1,parent->y1);

 link(0);
 bringtofront();
}

GUIrect::GUIrect(GUIrect *p,int rx1,int ry1,int rx2,int ry2)
{
 //sort coords x1<x2 and y1<y2
 if (rx1<rx2) {x1=rx1; x2=rx2;} else {x1=rx2; x2=rx1;}
 if (ry1<ry2) {y1=ry1; y2=ry2;} else {y1=ry2; y2=ry1;}

 focus=0;
 parent=0; child=lastchild=0; prev=next=0;
 lastfocus=0;
 reparent(p);
}

GUIrect::~GUIrect()
{
 if (modal==this) setmodal(0);
 if (parent && parent->lastfocus==this) parent->lastfocus=0;

 //delete children
 while (child) delete child;
 //unlink from parent
 unlink();
}

//unlink from parent's linked list of children
void GUIrect::unlink()
{
 if (!parent) return; //no parent, no link

 //set up other node's relation to us
 if (prev) prev->next=next; else parent->child=next;
 if (next) next->prev=prev; else parent->lastchild=prev;

 //set up our relation to other nodes
 prev=next=0;
}

//link to node AFTER t, if t==NULL then put at head of list
void GUIrect::link(GUIrect *t)
{
 if (!parent) return; //no parent, no link

 //set up our relation to other nodes
 prev=t;
 if (!prev) next=parent->child;
       else next=prev->next;

 //set up other node's relation to us
 if (prev) prev->next=this; else parent->child=this;
 if (next) next->prev=this; else parent->lastchild=this;
}



//do hit testing
GUIrect *GUIrect::hittest(int x,int y)
{
 if (x<x1 || x>x2 || y<y1 || y>y2) return 0; //not over us

  //test children first
 for (GUIrect *g=child; g; g=g->next)
  {
   GUIrect *result=g->hittest(x,y);
   if (result)
    {
     //setfocus(result);
     return result;
    }
  }

 if (this==guiroot)
  {
   losechildfocus();
   return 0;
  }
  //not over children, but over us
 return this;
}

void GUIrect::moveto(int nx,int ny)
{
 moverel(nx-x1,ny-y1);
}

void GUIrect::moverel(int dx,int dy)
{
 x1+=dx; x2+=dx;
 y1+=dy; y2+=dy;
  //move children
 for (GUIrect *g=child; g; g=g->next)   g->moverel(dx,dy);
}

void GUIrect::bringtofront()
{
 if (!parent) return; //no parent, no sort
 if (!prev) return; //already on top

 unlink();
 link(0);

 //parent->bringtofront();
}

void GUIrect::sendtoback()
{
 if (!parent) return; //no parent, no sort
 if (!next) return; //already on back

 unlink();
 link(lastchild); //link after last
}

void GUIrect::draw(char *dest)
{
  //draw children in reverse order
 for (GUIrect *g=lastchild; g; g=g->prev)   g->draw(dest);
}


//do hit testing
int GUIrect::keyhit(char kbscan,char key)
{
 if (!focus) return 0;
  //test children
 for (GUIrect *g=child; g; g=g->next)
   if (g->focus && g->keyhit(kbscan,key)) return 1;
 return 0;
}


//focus things
void GUIrect::losefocus()
{
 focus=0; //no more focus for us
}

void GUIrect::receivefocus()
{
 focus=acceptfocus();
 if (parent) parent->lastfocus=this;
}

int GUIrect::setfocus(GUIrect *f)
{
 if (!f) return 0;
// msg.printf(3,"setfocus %s",f->getname());
 //lose focus of all children
 root->losechildfocus();

 for (GUIrect *t=f; f && !f->acceptfocus(); f=f->parent); //go up levels until something can accept focus
 for ( ; f && f->lastfocus; f=f->lastfocus); //set the focus of its last child that had focus...
 for ( ; f; f=f->parent) f->receivefocus(); //set focus up through all levels to root

 return 0;
}


void GUIrect::losechildfocus()
{
 if (!focus) return;
 losefocus();


// lastfocus=0;
 for (GUIrect *g=child; g; g=g->next)
  {
  // if (!lastfocus && g->focus) lastfocus=g;
   g->losechildfocus(); //all our children lose focus
  }
}

//cycle the focus of our children
void GUIrect::cyclefocus(int dir)
{
if (!child) return;

for (GUIrect *g=child; g; g=g->next) g->losechildfocus();

g=lastfocus;
do
{
 if (!dir) g=g ? g->prev : lastchild;
      else g=g ? g->next : child;
 if (g && g->acceptfocus())
   {
    setfocus(g);
    return;
   }
} while (g!=lastfocus);
};






void GUIrect::fill(char color)
{
 drawrect(screen,color,x1,y1,width(),height());
}

void GUIrect::outline(char color)
{
 drawbox(screen,color,x1,y1-1,x2,y2);
}



//*************************************
//            GUI root
//*************************************

GUIroot::GUIroot(ROOT *p):GUIrect(p,0,0,p->width(),p->height()) {guiroot=this;}
GUIroot::~GUIroot() {guiroot=0;}

int GUIroot::keyhit(char kbscan,char key)
{  //TAB
 if ((kbstat&KB_CTRL) && kbscan==KB_TAB) {cyclefocus(0); return 1;}
 return GUIrect::keyhit(kbscan,key);
}



