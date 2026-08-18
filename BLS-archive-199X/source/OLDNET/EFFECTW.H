#ifndef _EFFECTW_
#define _EFFECTW_

//---------------------------------------------
#include "guirect.h"
#include "guicolor.h"
class panelist:public GUIcontents
{
 protected:
 class pane *first,*last;
 friend class pane;
 public:
 panelist():GUIcontents(0,0) {first=last=0;}
 void refreshsize();
 virtual void draw(char *dest)
  {
   fill(CLR_BOX);
   GUIrect::draw(dest);
  }
 virtual int keyhit(char key,char kbscan);
};

class pane:public GUIrect
{
 friend class panelist;
 protected:
 class panelist *pl; //our daddy
 class pane *pprev,*pnext; //our siblings

 public:
 pane(class panelist *tpl,int xw,int yw):GUIrect(0,0,0,xw,yw),pl(tpl)
  {
   if (!pl) return;
   //link up to end of parent
   pprev=pl->last; pnext=0;
   if (pprev) pprev->pnext=this; else pl->first=this;
   pl->last=this;
   //move to rightmost of parent
   moverel(pl->width(),0);
   //reparent us
   reparent(pl);
   //resize parent
   pl->refreshsize();
  }
 virtual ~pane()
  {
   if (!pl) return;
   if (pnext) delete pnext; // return;} //delete all panes to the right of us
   if (pprev) pprev->pnext=0; else pl->first=0;
   pl->last=pprev;
   pl->refreshsize();
  }
};

#define GUIRFR_EFFECT 0x8000
//pane for effect
class epane:public pane
{
 protected:
 class effectedit *ee;

 public:
 epane(class effectedit *tee);
 virtual ~epane();
 virtual int acceptfocus() {return 1;}
 virtual void draweffect(char *dest) {}; //draw effect to objectspace
};


#endif