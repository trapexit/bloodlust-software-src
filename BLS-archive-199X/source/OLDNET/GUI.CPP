#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"

#include "r2img.h"
#include "font.h"
#include "dd.h"
#include "config.h"

#include "mouse.h"

#include "keyb.h"
#include "gui.h"
#include "message.h"
#include "guicolor.h"
#include "a32.h"

#define umark a95vol.umark
#define dmark a95vol.dmark
#define lmark a95vol.lmark
#define rmark a95vol.rmark
#define checkmark a95vol.checkmark


//*************************************
//            GUI static
//*************************************
void GUIbar::draw(char *d)
{drawrect(d,color,x1,y1,width(),height());}

void GUIstatictext::settext(char *s) {if (s) strcpy(text,s); else text[0]=0;}

GUIstatictext::GUIstatictext(GUIrect *p,int f,char *str,int x,int y)
 :GUIrect(p,x,y,x,y)
{fontnum=f; settext(str);}

void GUIstatictext::draw(char *dest)
{
 font[fontnum]->draw(text,dest,x1,y1);
}

GUIstaticimage::GUIstaticimage(GUIrect *p,struct IMG *i,int x,int y)
 :GUIrect(p,x,y,x,y) {img=i;}

void GUIstaticimage::draw(char *dest)
{
 img->draw(dest,x1,y1);
}

void GUIstaticcenteredtext::draw(char *dest)
{
 font[fontnum]->drawcentered(text,dest,(x1+x2)/2,y1);
}



//*************************************
//            GUI button
//*************************************

GUIbutton::GUIbutton(GUIrect *p,int x,int y,int xw,int yw)
 :GUIrect(p,x,y,x+xw,y+yw),btimer(20) {depressed=0; }

void GUIbutton::draw(char *dest)
{
 //outer rect
// drawrect(dest,!depressed ? CLR_BUTTONOUTER : CLR_BUTTONOUTERDOWN,x1,y1,x2-x1,y2-y1);
 //inner rect
// drawrect(dest,!depressed ? CLR_BUTTONINNER : CLR_BUTTONINNERDOWN,x1+1,y1+1,x2-x1-2,y2-y1-2);

if (!depressed)
{
 fill(16+9);
 drawhline(dest,16+7,x1,y1,x2-1);
 drawvline(dest,16+7,x2-1,y1,y2-1);
 drawvline(dest,16+11,x1,y1,y2-1);
 drawhline(dest,16+11,x1,y2-1,x2-1);
} else
{
 fill(16+11);
 drawhline(dest,16+13,x1,y1,x2-1);
 drawvline(dest,16+13,x2-1,y1,y2-1);
 drawvline(dest,16+7,x1,y1,y2-1);
 drawhline(dest,16+7,x1,y2-1,x2-1);
}
/*
 drawhline(dest,16+3,x1-1,y1-1,x2+1);
 drawvline(dest,16+3,x2+1,y1-1,y2+1);
 drawvline(dest,16+15,x1-1,y1,y2+1);
 drawhline(dest,16+15,x1-1,y2+1,x2);
  */



 //send held down message
 if (depressed && parent && btimer.check() && hittest(m.x,m.y))
  {
   parent->sendmessage(this,GUIMSG_HELDDOWN);
   btimer.set(7); //btimer.dur>5 ? btimer.dur-2 : 5);
  }
}

GUIrect *GUIbutton::click(mouse &m)
{
 if (!(m.click&MBLEFT)) return 0;
 depressed=1; //left button clicked
 btimer.set(28); //time till first repeat
 if (parent) parent->sendmessage(this,GUIMSG_HELDDOWN);

 GUIrect::click(m);
 return this; //capture
}

int GUIbutton::release(mouse &m)
{
 if (!(m.rel&MBLEFT)) return 0;
 depressed=0; //left button released
 if (parent && hittest(m.x,m.y)) //if mouse over us...
   parent->sendmessage(this,GUIMSG_PUSHED);  //call button function
 return 1; //release
}

void GUItextbutton::settext(char *s) {if (s) strcpy(text,s); else text[0]=0;}

GUItextbutton::GUItextbutton(GUIrect *p,char *str,int x,int y)
 :bwidth(font[0]->getwidth(str)+12),GUIbutton(p,x,y,0,0)
     {
      moverel(-bwidth/2,0);
      resize(bwidth,12);
      strcpy(text,str);
     }

void GUItextbutton::draw(char *dest)
{
 GUIbutton::draw(dest);
 font[depressed ? FNT_TEXTBUTTONDOWN : FNT_TEXTBUTTON]->draw(text,dest,x1+6,y1+2);
}


GUIimagebutton::GUIimagebutton(GUIrect *p,IMG *i,int x,int y)
 :GUIbutton(p,x,y,0,0)
     {
      img=i;
      moverel(-i->xw/2,0);
      resize(i->xw+2,i->yw+2);
//      msg.printf(2,"newguiimagebutton");
     }

void GUIimagebutton::draw(char *dest)
{
 GUIbutton::draw(dest);
 img->draw(dest,x1+1,y1+1);
}



//*************************************
//            GUI checkbox
//*************************************


GUIcheckbox::GUIcheckbox(GUIrect *p,char *s,int x,int y,int initialstate)
  :GUIrect(p,x,y,x,y)
{
 checked=initialstate;
 depressed=0;
 resize(font[0]->getwidth(s)+12,10);
 strcpy(text,s);
}

void GUIcheckbox::draw(char *dest)
{              //9*16+1
 if (focus) drawbox(dest,2,x1-1,y1-1,x2,y2);

 drawrect(dest,CLR_CHECKBOXOUTER,x1,y1,8,8);
 drawrect(dest,CLR_CHECKBOXINNER ,x1+1,y1+1,6,6);
 if (checked) checkmark->draw(dest,x1+1,y1+1);
 font[depressed ? FNT_CHECKBOXDOWN : FNT_CHECKBOX]->draw(text,dest,x1+10,y1);
}

GUIrect *GUIcheckbox::click(mouse &m)
{
 if (!(m.click&MBLEFT)) return 0;
 depressed=1;

 GUIrect::click(m);
 return this; //capture
}

int GUIcheckbox::release(mouse &m)
{
 if (!(m.rel&MBLEFT)) return 0;
 depressed=0;
 if (hittest(m.x,m.y))
  {
   checked^=1; //left button released
   if (parent)
     if (!checked) parent->sendmessage(this,GUIMSG_UNCHECKED);
             else parent->sendmessage(this,GUIMSG_CHECKED);
  }
 return 1; //release
}

int GUIcheckbox::keyhit(char kbscan,char key)
{
 if (key==' ') {checked^=1; return 1;}
 return 0;
}


//*************************************
//            GUI edit control
//*************************************

GUIedit::GUIedit(GUIrect *p,char *pmpt,int x,int y,int xw)
 :GUIrect(p,x,y,x,y),disabled(0)
{
 prompt[0]=0;
 if (pmpt)
  {
   strcpy(prompt,pmpt);
   promptw=font[0]->getwidth(prompt)+1;
  } else promptw=0;
 inputmaxw=xw; //-promptw;
 resize(promptw+inputmaxw,10);
}

GUIrect *GUIedit::hittest(int x,int y) {if (disabled) return 0; else return GUIrect::hittest(x,y);}
GUIrect *GUIedit::click(mouse &m) {GUIrect::click(m); return 0;}

void GUIedit::draw(char *dest)
{
 font[FNT_EDITPROMPT]->draw(prompt,dest,x1,y1);

 drawrect(dest,CLR_EDITINPUT,x1+promptw-1,y1-1,inputmaxw,10);
 if (!disabled)
 {
  drawdata(dest,x1+promptw,y1,inputmaxw-2);
  if (focus) drawbox(dest,2,x1+promptw-2,y1-2,x1+promptw+inputmaxw,y1+10);


 }
 GUIrect::draw(dest);
}

//*************************************
//        GUI text edit control
//*************************************

void GUItextedit::getinputw()
{
 inputw=font[0]->getwidth(input);
 if (parent) parent->sendmessage(this,GUIMSG_EDITCHANGED);
}
void GUItextedit::setinput(char *s)
{
 if (s) strcpy(input,s); else input[0]=0;
 inputlen=strlen(input);
 inputw=font[0]->getwidth(input);
// getinputw();
}

GUItextedit::GUItextedit(GUIrect *p,char *pmpt, char *initialinput,int x,int y,int xw,int milen)
 :GUIedit(p,pmpt,x,y,xw)
{
 setinput(initialinput);
 maxinputlen=milen;
}

void GUItextedit::drawdata(char *dest,int x,int y,int xw)
{
/*int clip=inputw-(xw-5);
if (clip<0) clip=0;

font[FNT_EDITINPUT]->draw(input,dest+x,-clip,y);
if (focus && (uu&32)) font[FNT_EDITINPUT]->draw('_',dest,x+inputw-clip,y);*/
CLIP c(dest,x,y,x+xw,y+12);

int clip=inputw-(xw-5);
if (clip<0) clip=0;
font[FNT_EDITINPUT]->draw(input,dest,-clip,0);
if (focus && (uu&32)) font[FNT_EDITINPUT]->draw('_',dest,inputw-clip,0);


}


void GUItextedit::addchar(char c)
{
 if (inputlen>=maxinputlen) return;
 input[inputlen++]=c;
 input[inputlen]=0;
 getinputw();
}

void GUItextedit::backspace()
{
 if (inputlen>0) inputlen--;
 input[inputlen]=0;
 getinputw();
}

int GUItextedit::keyhit(char kbscan,char key)
{
 if (kbscan==KB_BKSP)  {backspace(); return 1;} //backspace
 if (isvalidkey(key)) {addchar(key); return 1;}
 return 0;
}
int GUItextedit::isvalidkey(char key) {return isprint(key);}

GUInumbertextedit::GUInumbertextedit(GUIrect *p,char *pmpt,int num,int x,int y,int xw,int milen)
:GUItextedit(p,pmpt,itoa(num,input,10),x,y,xw,milen) {};

int GUInumbertextedit::isvalidkey(char key) {return isdigit(key) || (key=='-' && inputlen==0);}

int GUInumbertextedit::getstate() {return atoi(input);}


GUInumberedit::GUInumberedit(GUIrect *p,char *pmpt,int x,int y,int xw)
 :GUIedit(p,pmpt,x,y-2,xw)
{
 x2+=10; //make space on right for button
 y2+=10;

 up=new GUIimagebutton(this,umark,0,0);
 up->moveto(x1+width()-up->width()+1,y1);
 down=new GUIimagebutton(this,dmark,0,0);
 down->moveto(x1+width()-down->width()+1,y1+8);
}

void GUInumberedit::draw(char *dest)
{
 font[FNT_EDITPROMPT]->draw(prompt,dest,x1,y1+2);

 drawrect(dest,CLR_EDITINPUT,x1+promptw-1,y1+2,inputmaxw,10);
 if (!disabled)
 {
  drawdata(dest,x1+promptw,y1+3,inputmaxw-2);
  if (focus)
   drawbox(dest,2,x1+promptw-2,y1+1,x1+promptw+inputmaxw,y1+3+10);
 }
 GUIrect::draw(dest);
}


int GUInumberedit::keyhit(char kbscan,char key)
{
 if (kbscan==KB_BKSP)  {clear(); clip(); return 1;} //backspace
 if (key=='?') {setmax(); clip(); return 1;}
 if (key=='+' || key=='=' || key=='.' || key=='>')// || kbscan==KB_UP)
    {sendmessage(up,GUIMSG_HELDDOWN); return 1;}
 if (key=='-' || key=='_' || key==',' || key=='<')// || kbscan==KB_DOWN)
   {sendmessage(down,GUIMSG_HELDDOWN); return 1;}
 return 0;
}

//int edit
GUIintedit::GUIintedit(GUIrect *p,char *pmpt,int x,int y,int xw,int num,int tmin,int tmax)
  :GUInumberedit(p,pmpt,x,y,xw) { min=tmin; max=tmax; n=num;/*set(num);*/ clip();}
void GUIintedit::clear() {set(0); }
void GUIintedit::setmax() {set(max);}
void GUIintedit::clip(){if (n<min) n=min; if (n>max) n=max;}
void GUIintedit::add(int num) {set(n+num); }
void GUIintedit::minus(int num) {set(n-=num); }
void GUIintedit::drawdata(char *dest,int x,int y,int xw)
{
 font[FNT_EDITINPUT]->printf(x,y,"%d",n);
}
int GUIintedit::sendmessage(GUIrect *c,int msg)
{
 if (msg==GUIMSG_HELDDOWN)
  {
   if (!(kbstat&KB_SHIFT))
   {
    if (c==up) add(1);
    if (c==down) minus(1);
   } else
   {
    if (c==up) add(10);
    if (c==down) minus(10);
   }
  }
 return 0;
}

//float edit
GUIfloatedit::GUIfloatedit(GUIrect *p,char *pmpt,int x,int y,int xw,float num,float tmin,float tmax)
 :GUInumberedit(p,pmpt,x,y,xw) {min=tmin; max=tmax; n=num; clip();/*set(num);*/}
void GUIfloatedit::clear() {set(0);}
void GUIfloatedit::setmax() {set(max);}
void GUIfloatedit::clip() {if (n<min) n=min; if (n>max) n=max;}
void GUIfloatedit::add(float num) {set(n+num);}
void GUIfloatedit::minus(float num) {set(n-num);}
void GUIfloatedit::drawdata(char *dest,int x,int y,int xw)
{
 font[FNT_EDITINPUT]->printf(x,y,"%.1f",n);
}
int GUIfloatedit::sendmessage(GUIrect *c,int msg)
{
 if (msg==GUIMSG_HELDDOWN)
  {
   if (!(kbstat&KB_SHIFT))
   {
    if (c==up) add(.1);
    if (c==down) minus(.1);
   } else
   {
    if (c==up) add(1.0);
    if (c==down) minus(1.0);
   }
  }
 return 0;
}

void GUIcontents::resize(int xw,int yw)
  {
   if (parent) ((class GUIbox *)parent)->resize(parent->width()+xw-width(),parent->height()+yw-height());
   GUIrect::resize(xw,yw);
  }


int GUIcontents::keyhit(char kbscan,char key)
{
 //ESC
 if (kbscan==KB_ESC)
   {
    if (parent) delete parent;
    return 1;
   }

  //TAB
 if (kbscan==KB_TAB) {cyclefocus((kbstat&KB_SHIFT)); return 1;}
 return GUIrect::keyhit(kbscan,key);
}

//*************************************
//            GUI box
//*************************************
#define TOPBARHEIGHT 12

void GUIbox::settitle(char *s)
{
 if (s) strcpy(title,s); else s[0]=0;
}

void GUIbox::reposclosebutton()
{
 close->moveto(x2-10,y1+1);
}

GUIbox::GUIbox(GUIrect *p,char *str,GUIcontents *c,int x,int y)
 :contents(c),GUIrect(p,x,y,x+c->width(),y+TOPBARHEIGHT+c->height()+2)
{
 //copy title of box
 settitle(str);
 //create close button
 close=new GUIbutton(this,0,0,8,8);
 reposclosebutton();

 //position contents
 contents->reparent(this);
 contents->moverel(0,TOPBARHEIGHT);
 setfocus(contents);
 contents->cyclefocus(0);
}

GUIrect *GUIbox::click(mouse &m)
{
// msg.printf(2,"clicked %d,%d",m.x,m.y);
 if ((m.click&MBLEFT) && m.y<y1+TOPBARHEIGHT) return this; //clicked on title bar
 return GUIrect::click(m);
}
int GUIbox::release(mouse &m) {return 1;}
int GUIbox::drag(mouse &m) {moverel(m.x-m.oldx,m.y-m.oldy); return 1;}

void GUIbox::draw(char *dest)
{
// int top=!topmost() ? 0 : 0;
// if (topmost())
 drawbox(dest,focus ? CLR_BOXTITLE-4 : 0 ,x1-1,y1-1,x2+1,y2+1);

 drawrect(dest,CLR_BOXTITLE,x1,y1,x2-x1,TOPBARHEIGHT); //top black bar
 font[focus ? FNT_BOXTITLE : 3]->draw(title,dest,x1+5,y1+1);

// drawrectmap(dest,shadowmap,x1,y1+TOPBARHEIGHT,x2-x1,y2-y1-TOPBARHEIGHT-2); //main area
// drawrect(dest,CLR_BOX,x1,y1+TOPBARHEIGHT,x2-x1,y2-y1-TOPBARHEIGHT-2); //main area
 drawrect(dest,CLR_BOXTITLE,x1,y2-2,x2-x1,2); //bottom black bar

 //draw contents of box
 GUIrect::draw(dest);
}

int GUIbox::sendmessage(GUIrect *c,int msg)
{
 //someone pushed the close button
 if (c==close)
   if (msg==GUIMSG_PUSHED) delete this;

 //contents sent us a message...
 if (c==contents)
  if (parent) parent->sendmessage(this,msg); //relay it to our parent
 return 1;
}

int GUIbox::keyhit(char kbscan,char key)
{
 if (GUIrect::keyhit(kbscan,key)) return 1;

 return 0; //not processed
}


//*************************************
//         GUI maximize box
//*************************************
GUImaximizebox::GUImaximizebox(GUIrect *p,char *str,GUIcontents *c,int x,int y)
 :GUIbox(p,str,c,x,y)
{
 maximized=0;
 max=new GUIbutton(this,0,0,8,8);
 reposmaxbutton();
 contents->bringtofront();
}

void GUImaximizebox::draw(char *dest)
{
 if (!maximized) //draw box like normal
    GUIbox::draw(dest);
 else
  {
   contents->draw(dest);
   max->draw(dest);
  }
}

void GUImaximizebox::maximize()
{
 if (maximized) return;
 contents->maximize();
 reparent(root);

 unlink();
 parent=root;
 link(root->lastchild);
 maximized=1;
 reposmaxbutton();
}

void GUImaximizebox::restore()
{
 if (!maximized) return;
 contents->moveto(x1,y1+TOPBARHEIGHT);
 contents->restore();

 reparent(guiroot);
 maximized=0;
 bringtofront();
 reposmaxbutton();
}

void GUImaximizebox::reposmaxbutton()
{
if (maximized) max->moveto(contents->x2-10,contents->y1+2);
          else max->moveto(close->x1-10,close->y1);
}


GUIrect *GUImaximizebox::hittest(int x,int y)
{
 if (maximized)
  {
   if (max->hittest(x,y)) return max;
   return contents->hittest(x,y);
  }
  else return GUIbox::hittest(x,y);
}

int GUImaximizebox::sendmessage(GUIrect *c,int guimsg)
{
 if (c==max)
 {
  if (guimsg==GUIMSG_PUSHED)
   {
     if (!maximized) maximize(); else restore();
   }
  return 1;
 }
return GUIbox::sendmessage(c,guimsg);
}

//*************************************
//         GUI one button box
//*************************************
GUIonebuttonbox::GUIonebuttonbox(GUIrect *p,char *str,GUIcontents *c,char *b1name,int x,int y)
 :GUIbox(p,str,c,x,y)
{
 y2+=10;
 bar=new GUIbar(this,CLR_BOXTITLE,0,height()-12,width(),12);
 b1=new GUItextbutton(this,b1name,width()/2,height()-12);
 contents->bringtofront();
}

int GUIonebuttonbox::sendmessage(GUIrect *c,int msg)
{
 if (c==b1)
  if (msg==GUIMSG_PUSHED)
  {if (contents->sendmessage(this,GUIMSG_OK)) delete this; return 1; }

 return GUIbox::sendmessage(c,msg);
}

int GUIonebuttonbox::keyhit(char kbscan,char key)
{
 //Enter
 if (kbscan==KB_ENTER) {if (contents->sendmessage(this,GUIMSG_OK)) delete this; return 1;}
 return GUIbox::keyhit(kbscan,key);
}


//*************************************
//         GUI two button box
//*************************************
GUItwobuttonbox::GUItwobuttonbox(GUIrect *p,char *str,GUIcontents *c,char *b1name,char *b2name,int x,int y)
 :GUIbox(p,str,c,x,y)
{
 y2+=10;
 bar=new GUIbar(this,CLR_BOXTITLE,0,height()-12,width(),12);

 b1=new GUItextbutton(this,b1name,0,0);
 b2=new GUItextbutton(this,b2name,0,0);
 b1->moverel(width()/4,height()-12);
 b2->moverel(width()*3/4,height()-12);

 contents->bringtofront();
}

int GUItwobuttonbox::sendmessage(GUIrect *c,int msg)
{
 if (c==b1)
  if (msg==GUIMSG_PUSHED)
   {if (contents->sendmessage(this,GUIMSG_B1)) delete this; return 1; }
 if (c==b2)
  if (msg==GUIMSG_PUSHED)
   {if (contents->sendmessage(this,GUIMSG_B2)) delete this; return 1; }
 return GUIbox::sendmessage(c,msg);
}

int GUItwobuttonbox::keyhit(char kbscan,char key)
{
 //Enter
 if (kbscan==KB_ENTER) {if (contents->sendmessage(this,GUIMSG_B1)) delete this; return 1;}
 return GUIbox::keyhit(kbscan,key);
}




//*************************************
//         GUI scroll bar
//*************************************
void GUIscrollbar::clip()
{
 if (pos<min) pos=min;
 if (pos>max) pos=max;
}

GUIscrollbar::GUIscrollbar(GUIrect *p,int x,int y,int xw,int yw)
 : GUIrect(p,x,y,x+xw,y+yw)
{ min=0;
  max=0;
  pos=0;
}

void GUIscrollbar::draw(char *dest)
{
//  font[0]->printf(50,60,"pos=%d min=%d max=%d",pos,min,max);

 if (min==max) return;
 GUIrect::draw(dest);
 if (focus) drawbox(dest,2,x1-1,y1-1,x2+2,y2+2);
}

int GUIvscrollbar::keyhit(char kbscan,char key)
{
 if (kbscan==KB_UP) {sendmessage(up,GUIMSG_HELDDOWN); return 1;}
 if (kbscan==KB_DOWN) {sendmessage(down,GUIMSG_HELDDOWN); return 1;}
 return 0;
}

int GUIhscrollbar::keyhit(char kbscan,char key)
{
 if (kbscan==KB_LEFT) {sendmessage(up,GUIMSG_HELDDOWN); return 1;}
 if (kbscan==KB_RIGHT) {sendmessage(down,GUIMSG_HELDDOWN); return 1;}
 return 0;
}

int GUIscrollbar::sendmessage(GUIrect *c,int msg)
{
 if (msg==GUIMSG_HELDDOWN)
  {
   if (c==up) scrollup();
   if (c==down) scrolldown();
  }
 return 0;
}

void GUIscrollbar::setposfromtrack(int tpos,int tlen)
{
// return;
 int x=(min<<4)+(tpos<<4)*(max-min)/tlen;
 pos=x>>4;
 if ((x&15)>=8) pos++;
// pos=min+tpos*(max-min)/tlen;
//msg.printf(2,"setposfromtrack");
}

void GUIscrollbar::setrange(int tmin, int tmax)
  {
   min=tmin; max=tmax;
   if (min>max) max=min;
   clip();
   if (min==max) return;
   track->settrackfrompos(pos,min,max);
   track->setthumbsize(max-min+1);
  }
void GUIscrollbar::setpos(int p)
  {
   pos=p; clip();
   if (min==max) return;
   track->settrackfrompos(pos,min,max);
  }



//------------------------------------
//trackbutton
GUItrackbutton::GUItrackbutton(class GUItrack *p,int size):
  GUIbutton(p,0,0,size,size),ptrack(p) {};

GUIvtrackbutton::GUIvtrackbutton(class GUIvtrack *p,int size):GUItrackbutton(p,size) {};
GUIrect *GUIvtrackbutton::click(class mouse &m){tpos=m.y-y1; return GUIbutton::click(m);}
int GUIvtrackbutton::drag(mouse &m){ptrack->movethumb(m.y-tpos-ptrack->y1); return 1;}

GUIhtrackbutton::GUIhtrackbutton(class GUIhtrack *p,int size):GUItrackbutton(p,size) {};
GUIrect *GUIhtrackbutton::click(class mouse &m) {tpos=m.x-x1; return GUIbutton::click(m);}
int GUIhtrackbutton::drag(mouse &m) { ptrack->movethumb(m.x-tpos-ptrack->x1); return 1;}



//------------------------------------
//track
GUItrack::GUItrack(GUIscrollbar *p,int x,int y,int xw,int yw):
  GUIrect(p,x,y,x+xw,y+yw),pscroll(p) {tracklen=yw; trackpos=0;};

GUIvtrack::GUIvtrack(GUIvscrollbar *p):
 GUItrack(p,0,p->up->height(),p->width(),(p->down->y1)-(p->up->y2))
{thumb=new GUIvtrackbutton(this,width());}

GUIhtrack::GUIhtrack(GUIhscrollbar *p):
 GUItrack(p,p->up->width(),0,(p->down->x1)-(p->up->x2),p->height())
{thumb=new GUIhtrackbutton(this,height());}

void GUItrack::draw(char *dest)
{
 drawrect(dest,16+14,x1,y1,width(),height());
 GUIrect::draw(dest); //draw thumb specifically

// font[0]->printf(50,50,"trackpos=%d tracklen=%d",trackpos,tracklen);
}

//moves thumb, smoothly
void GUItrack::movethumb(int tpos)
{
 if (tpos<0) tpos=0;
 if (tpos>tracklen) tpos=tracklen;
 trackpos=tpos;
 positionthumb();
 pscroll->setposfromtrack(trackpos,tracklen);
}

/*
//force thumb into rigid stepping
void GUItrack::setthumb()
{
 pscroll->setposfromtrack(trackpos,tracklen);
} */

void GUItrack::settrackfrompos(int pos,int min,int max)
{
 trackpos=(pos-min)*tracklen/(max-min);
 positionthumb();
}

void GUIvtrack::setthumbsize(int posrange)
{
 int size=height()/posrange;
 if (size<8) size=8;
 thumb->resize(width(),size);
 tracklen=height()-size;
 movethumb(trackpos);
}

void GUIhtrack::setthumbsize(int posrange)
{
 int size=width()/posrange;
 if (size<8) size=8;
 thumb->resize(size,height());
 tracklen=width()-size;
 movethumb(trackpos);
}


void GUIvtrack::positionthumb()
{
 thumb->moveto(x1,y1+trackpos);
}

void GUIhtrack::positionthumb()
{
 thumb->moveto(x1+trackpos,y1);
}


//vertical scroll bar
GUIvscrollbar::GUIvscrollbar(GUIrect *p,int x,int y,int height)
 :GUIscrollbar(p,x,y,11,height)
{
 up=new GUIimagebutton(this,umark,0,0);
 down=new GUIimagebutton(this,dmark,0,0);
 up->moveto(x1,y1);
 down->moveto(x1,y2-down->height());

 track=new GUIvtrack(this);
 setrange(0,0);
 setpos(0);
}


//horizontal scroll bar
GUIhscrollbar::GUIhscrollbar(GUIrect *p,int x,int y,int width)
 :GUIscrollbar(p,x,y,width,11)
{
 up=new GUIimagebutton(this,lmark,0,0);
 down=new GUIimagebutton(this,rmark,0,0);

 up->moveto(x1,y1);
 down->moveto(x2-down->width(),y1);

 track=new GUIhtrack(this);
 setrange(0,0);
 setpos(0);
}


//*************************************
//         GUI list box
//*************************************

void GUIlistbox::clearsel()
{
 sel=-1;
}

void GUIlistbox::setsel(int s)
{
 if (s<0) s=0;
 if (s>numitems-1) s=numitems-1;
 if (s==sel) return;
 sel=s;

 if (sel<scroll->getpos()) scroll->setpos(sel);
 if (sel>=scroll->getpos()+itemv) scroll->setpos(sel-itemv+1);

 if (parent) parent->sendmessage(this,GUIMSG_LISTBOXSELCHANGED);
}

GUIlistbox::GUIlistbox(GUIrect *p,int x,int y,int xw,int iy,int iheight)
 :GUIrect(p,x,y,x+xw,y+iy*iheight),itemheight(iheight)
{
 items=0;
 numitems=0;
 itemv=iy; //number of displayable items vertically
 depressed=0;

  //scroll bar on right of box
 scroll=new GUIvscrollbar(this,0,0,height());
 scroll->moveto(x2-scroll->width(),y1);
 clearsel();

 dblclick.set(0);
}

void GUIlistbox::freeitems()
{
 if (items) free(items);
 items=0; numitems=0; clearsel();
}

ITEMPTR *GUIlistbox::resizeitems(int n)
{
 items=(ITEMPTR *)realloc(items,n*sizeof(ITEMPTR));
 while (numitems<n) items[numitems++]=0; //clear pointers
 numitems=n;

 if (sel>=numitems) setsel(numitems-1);

 scroll->setrange(0,numitems-itemv);

 return items;
}

void GUIlistbox::draw(char *dest)
{
 drawbox(dest,7,x1-1,y1-1,x2+1,y2+1);
 drawrect(dest,0,x1,y1,width(),height());
 drawitems(dest,x1+1,y1+1);
 GUIrect::draw(dest);

 if (depressed)
  {
 int iy=(m.y-y1)/itemheight;
 if (iy<0) //mouse above
   {
    if (timer.check())
     {
      if (sel>0) setsel(sel-1);
      timer.reset();
     }
    return ;
   }
 if (iy>=itemv) //mouse below
   {
    if (timer.check())
     {
      if (sel<numitems-1) setsel(sel+1);
      timer.reset();
     }
    return;
   }
 //mouse in box
 setsel(scroll->getpos()+iy);
  }

}

int GUIlistbox::keyhit(char kbscan,char key)
{
 if (kbscan==KB_HOME) {setsel(0); return 1;} //home
 if (kbscan==KB_END) {setsel(numitems-1); return 1;} //end

 if (kbscan==KB_PGUP) {setsel(sel-itemv); return 1;} //pageup
 if (kbscan==KB_PGDN) {setsel(sel+itemv); return 1;} //pagedown


 if (kbscan==KB_UP) {if (sel>0) setsel(sel-1); return 1;}
 if (kbscan==KB_DOWN) {if (sel<numitems-1) setsel(sel+1); return 1;}
 return 0;
}


GUIrect * GUIlistbox::click(mouse &m)
{
 if (!(m.click&MBLEFT)) return 0;
 if (!dblclick.check()) //doubleclicked
  {
   if (parent) parent->sendmessage(this,GUIMSG_LISTBOXDBLCLICKED);
  }
 dblclick.set(30);

 timer.set(10); depressed=1;
 return this;
}
int GUIlistbox::release(mouse &m)
{

depressed=0; return 1;
}
int GUIlistbox::drag(mouse &m) {return 1;}


void GUIstringlistbox::drawitems(char *dest,int x,int y)
{
 int i=scroll->getpos();
 for (int j=itemv; j>0 && i<numitems; j--,i++,y+=itemheight)
 {
  if (sel==i) //currently selected
   {
    drawrect(dest,2,x,y,width()-2,itemheight);
    if (items[i]) font[1]->draw((char *)items[i],dest,x,y);
   }
  else
  if (items[i])  font[1]->draw((char *)items[i],dest,x,y);
 }

}












