//Copyright(c) 1996 Bloodlust Software All rights reserved
//menu code
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <direct.h>

#define ANIMATOR

#include "ttimer.h"
#include "a32.h"



extern image *dragimg;
extern dialog dlg;




extern FONT *font[];
extern int mx,my,mb,mstat;

extern object *p;


int findimage(objectdef *a,objectdef *b,int index)
{
 if (!a->imgnames || !b->imgnames) return index;
 char *s=a->imgnames[index]; //get pointer to source image name

 for (int i=0; i<b->numi; i++)
  if (!stricmp(s,b->imgnames[i]))  return i;
 return index;      //not found
}

void convertimage(image *sptr,image *dptr,objectdef *srcobj,objectdef *destobj,int numimages)
{
 memcpy(dptr,sptr,numimages*sizeof(image)); //copy over images
 if (srcobj==destobj) return;
 
 for (int i=0; i<numimages; i++,sptr++,dptr++) //go through all images
   dptr->index=findimage(srcobj,destobj,sptr->index); //get image number
}    


void convertframe(frame *source,frame *dest,objectdef *srcobj,objectdef *destobj)
{

 memcpy(dest,source,source->size); //copy over frame
 if (srcobj==destobj) return;

 //convert images
 convertimage(source->getimgptr(),dest->getimgptr(),srcobj,destobj,source->numimages);
}

void convertframes(frame *source,frame *dest,objectdef *srcobj,objectdef *destobj,int num)
{
 for (int i=0; i<num; i++)
  {
   convertframe(source,dest,srcobj,destobj);
   dest=(frame *)(((char *)dest)+source->size);   
   source=(frame *)(((char *)source)+source->size);
  }
}


//*******************************************
//***************menu stuff******************
//*******************************************
int menuitem::width()
{
 if (!text) return 5;
 return font[0]->getwidth(text)+10;
}

void menuitem::draw(int x, int y, int sel)
{
 DrawString(font[sel],screen,text,x,y);
}    

//menu
void menu::getwidthheight()
{
 class menuitem *t;
 if (!orient) //vertical
 {
  if (!xw) { for (t=m; t->text; t++) if (t->width()>xw) xw=t->width();  xw+=6;}//find width
  if (!yw) { for (t=m; t->text; t++) yw+=10; }    //find height
 } else
 {
  if (!xw) { for (t=m; t->text; t++) xw+=t->width();  }//find width     
  yw=10;
 }
}
int onmenu=0;
void menu::draw(int x,int y)
{
 if (!this) return;    
 int sel;
 getwidthheight();

// DrawBarMap(screen,SHADOWMAP,x,y,xw,yw);
 msel=0; 
 for (class menuitem *t=m; t->text; t++)
  {
   sel=(my>=y && my<y+10 && mx>x && mx<x+xw && !dragimg && !dlg.active);
   if (sel)
    {
     drawrect(screen,2,x,y,xw,10);
     t->draw(x+3,y,1);
     if (mb&1) msel=t; //this has been selected
     onmenu=1;
    } else  t->draw(x+3,y,0);
   DrawLetter(font[2],screen,t->key,x+xw-8,y);
   y+=10;  
  }
  
}    

void menu::drawhoriz(int x,int y)
{
 if (!this) return;
 int sel;
 getwidthheight();
// DrawBarMap(screen,SHADOWMAP,x,y,xw,yw);
 for (class menuitem *t=m; t->text; t++)
  {
   sel=(my>=y && my<y+10 && mx>x && mx<x+t->width());
   if (sel) onmenu=1;
   sel=sel && !dragimg && !dlg.active;
   if (sel)
    {
     drawrect(screen,2,x,y,t->width(),10);
     t->draw(x+3,y,1);
     if (msel)
     {
      if (mb&1) msel=t; //this has been selected
     } else if (mstat&1) msel=t; //this has been selected
    } else  t->draw(x+3,y,0);
   if (msel==t && !dragimg) msel->submenu->draw(x,y+10); 
   x+=t->width(); //horizontal
  }

  //if one of them IS selected
 if (msel)
  {
   if (!(mb&1)) msel=NULL;
//   mb=0; mstat=0;
  }
}    


class menuitem *menu::checkkey(char key)
{
 if (!this) return 0;
 for (class menuitem *t=m; t->text; t++)
  {
   if (t->key==key) return t;
   class menuitem *r=t->submenu->checkkey(key);
   if (r) return r;
  }
return 0; 
}    





extern int basex,basey;
void dlg_loadobject(objectdef *odefs,int i,void *context)
{
 object *t=new object(&odf[i],-1,basex,basey,0,0,0); //create an instance of the object
 t->startseries(0);
 if (!t->od->loaded) //error creating object
     {free(t); return;} //free it and abort

 //if previous object there
 if (p){t->x=p->x+(65<<16); }
 if (t->x<(-20<<16)) t->x=340<<16; //scoot it back if it goes offscreen
 if (t->x>(340<<16)) t->x=-20<<16;


 //add object to beginning of object list
 o->prev=t;
 t->prev=0; t->next=o;
 p=o=t;

 resetshadow();
}
void M_LOADOBJECT()
{
 rect r={80,20,320-80,170};
 dlg.init(&r,"Load object",odf,sizeof(objectdef),numodf,0,
    (void *)dlg_objectdef,(void *)dlg_loadobject,0);
}    



void dlg_loadbackground(backgrounddef *bgdefs,int i,void *context)
{
 if (bgobj) delete bgobj;
  //create bgobject
 bgobj=new bgobject(&bgdf[i]);
 p=bgobj;
}

void M_EDITBG() {p=bgobj; if (bgobj->active) bgobj->stop();}
void M_LOADBG()
{
 rect r={80,20,320-80,170};
 dlg.init(&r,"Load background",bgdf,sizeof(backgrounddef),numbgdf,0,
    (void *)dlg_objectdef,(void *)dlg_loadbackground,0);
}    
void M_VOLUMIZEBG()
{

}    




void dlg_listseries(series *ser,int i,object *context)
{
 context->startseries(i);
}
void M_SERIESLIST()
{
   if (!p || p->playing) return;
   resetshadow();
 char s[50];
 sprintf(s,"Select %s Series",p->od->name);

 rect r={50,15,320-50,170};
 dlg.init(&r,s,p->od->sd,sizeof(series),p->od->nums,p->csnum,
    (void *)dlg_series,(void *)dlg_listseries,p);
}


void M_SAVESDF()
{
  if (!p) return;
  p->od->SaveSDF();
  mprintf("%s saved",p->od->sdf);
}    


void M_KILLOBJECT()
  {
   if (!p) return;
   _disable();
   object *t=p;
   if (p==bgobj) bgobj=0;
   if (p->next) {p=p->next; delete t;}
    else
   if (p->prev) {p=p->prev; delete t;}
    else {delete t; p=o;}
   resetshadow();
   
   _enable();
  }


void M_NEWIMAGE()
  {
      if (!p || p->playing || !p->fptr || dragimg || p->od->readonly) return;
      emode=EM_IMAGE;
      p->resetfptr();
      int i=p->od->RequestImage();
      if (i!=-1)
        {
         _disable();
         image *im=p->fptr->getimgptr()+p->fptr->numimages; //get pointer to image locations
         
         p->fptr->size+=sizeof(image);
         p->fptr->numimages++;          //increase image counter
         im=(image *)p->cs->insertbytes((char *)im,sizeof(image)); //get memory for new image
         p->resetfptr();
         im->index=(unsigned char)i;  //store image index
         im->orient=0;
         im->dispx=0; im->dispy=0;
         im->exclusive=0; im->layer=p->layer;

         dragimg=im; //start draggin it!
         _enable();
        }
  }

void M_DELETEIMAGE()
 {
    if (!p || !dragimg || p->od->readonly) return;

    if (p!=bgobj)
    {
       resetshadow();
       _disable();
       p->fptr->size-=sizeof(image);
       p->fptr->numimages--;          //decrease image counter
       p->cs->deletebytes((char *)dragimg,sizeof(image)); //free memory for image
       _enable();
    } else
    {
     bgactiveimage *a=(bgactiveimage *)dragimg;
     bgimage *b=&((backgrounddef *)bgobj->od)->bgi[a->idx];
        
     b->index=0xFF;
     delete a;
//     bgobj->resssemoveto(bgobj->x,bgobj->y);
     
     
    }

       dragimg=0;
     
       kbscan=key=0;
//mb=0; mrelstat=0;
//       mstat=0;

 }  

//       if (!p->d) dragimg->dispx=(short)(dragrel.x+mx-(p->x>>16)); //move image
//            else  dragimg->dispx=(short)(dragrel.x-(mx-(p->x>>16))); //move image
//       dragimg->dispy=(short)(dragrel.y+my-(p->y>>16));

//  if (!d) dragrel.x=(p->x>>16)-mx+((int)found->dispx);
//   else dragrel.x=-((p->x>>16)-mx)+((int)found->dispx);
//  dragrel.y=(p->y>>16)-my+((int)found->dispy);

///     emode=EM_EFFECT;
///     int i=RequestExtra();
///     if (i==-1) return;
///     wait();


void dlg_addextra(char **extraname,int i,object *context)
{
 context->AddExtra(i);
}
  
extern char *extraname[];
int ExtraCount()
{
for (int i=0; extraname[i]; i++);
return i;
}    
void M_NEWEFFECT()
{
  if (!p || p->playing || !p->fptr || p->od->readonly) return;
  {
    rect r={50,15,320-50,170};
    dlg.init(&r,"Add Effect",extraname,4,ExtraCount(),-1,
      (void *)dlg_extra,(void *)dlg_addextra,p);
      emode=EM_EFFECT;
  }
}

void M_INSERTFRAME()
  {
       if (!p || p->playing || p->od->readonly) return;
        _disable();
        p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,sizeof(frame));
        p->cs->nf++; p->nf++; //p->resetfptr();
        p->fptr->size=sizeof(frame); //reset frame
        p->fptr->dur=10;
        _enable();
  }

void M_INSERTAFTER()
  {
   if (!p || p->playing || !p->cs->nf || p->od->readonly) return;
      p->cs->cf++; p->cf++; p->resetfptr();
    M_INSERTFRAME();
     p->cs->cf--; p->cf--; p->resetfptr();
  }

void M_SAVEOBJECT()
 {
  if (!p) return;
  _disable();
  p->od->Save();
  _enable();
 }

void M_QUIT()
 {
  if (dlg.active) {dlg.active=0; return;}
  _disable();
  while (o)
  {
   delete o;
  }

  if (bgobj) delete bgobj;
  p=0; o=0; bgobj=0;

  _enable();
  donea32=1;
 }


void M_ACTIVATEOBJECT()
{
if (!p) return;

if (p==bgobj)
 {
  bgobj->activate();
  return;
 }

//delete all objects created by this one
 for (object *t=o,*next; t; t=next)
   {
    next=t->next;
    if (t->creator==p) delete t;
   }

 inputdevice[0].bind(p);
 p->in->reset();
 p->activate(0);
 mprintf("%s activated",p->od->name);
}


void M_ACTIVATEALL()
{
if (p && !p->in)
{
 inputdevice[0].bind(p);
 p->in->reset();
}    

//delete all objects created by other ones
for (object *t=o,*next; t; t=next)
   {
    next=t->next;
    if (t->creator) delete t;
   }


for (t=o; t; t=t->next)
 t->activate(0);

 mprintf("all activated");
}


void M_RESETALL()
{
for (object *t=o; t; t=t->next)
   {
    t->y=0;
    t->getbasey();
    t->y=t->basey;
    t->z=0;
    }
 M_ACTIVATEALL();

 mprintf("all reset");
}



void M_PASTE()
{
 if (!p || p->od->readonly) return;
 _disable();
  switch (cliptype)
          {
           case EMPTY: break;
           case EFFECTS:
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr->getextraptr(),clipsize); //insert bytes
             p->resetfptr();
             p->fptr->size+=clipsize;
             p->fptr->numextra+=clipnum;
             memcpy((char *)p->fptr->getextraptr(),clipboard,clipsize);
             mprintf("pasted %d effects(s), %d bytes",clipnum,clipsize);

             break;
           case FRAMES:
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr,clipsize);
      //       memcpy((char *)p->fptr,clipboard,clipsize);
            convertframes((frame *)clipboard,p->fptr,clipobj,p->od,clipnum);

             p->cs->nf+=clipnum; //insert frame in there
             p->nf=p->cs->nf;
             mprintf("pasted %d frame(s), %d bytes",clipnum,clipsize);
             break;
           case IMAGES:
            {
             if (!p->nf) break;
             int imgsize=p->fptr->numimages*sizeof(image);
             p->cs->deletebytes((char *)p->fptr->getimgptr(),imgsize); //delete all images
             p->resetfptr();
             p->fptr->size-=imgsize;
             p->fptr->numimages=0; //clipnum;
            
             p->fptr=(frame *)p->cs->insertbytes((char *)p->fptr->getimgptr(),clipsize);
             p->resetfptr();
             p->fptr->size+=clipsize;
             p->fptr->numimages=clipnum;
             //memcpy((char *)p->fptr->getimgptr(),clipboard,clipsize);
             mprintf("pasted %d image(s), %d bytes",clipnum,clipsize);

             convertimage((image *)clipboard,p->fptr->getimgptr(),clipobj,p->od,clipnum);

            }
             break;
            
          }
 _enable();

} 

void M_FLIPXOBJECT()
{
 if (p) p->d^=1;
}



void M_STOP()
{
 if (!p || !p->playing) return;
 if (p==bgobj) {bgobj->stop(); return;}
 p->stop();
 resetshadow();
}    

void M_PLAY()
{
 if (p && p->fptr) p->play(p->csnum);
}    

void M_PLAYLOOPED()
{
 if (p && p->fptr) p->playlooped(p->csnum);
}    

void M_DELETEFRAME()
{
 if (p && p->fptr && !p->playing && !p->od->readonly) 
 {
 _disable();
  p->cs->deletebytes((char *)p->fptr,p->fptr->size);
  p->cs->nf--; p->nf--; 
  if (p->cf>=p->nf && p->cf>0) {p->cs->cf--; p->cf--;}
  p->resetfptr();
 _enable();
 }
}

void M_COPYEFFECTS()
{
if (!p || !p->fptr) return;
        cliptype=EFFECTS;
        clipnum=p->fptr->numextra;
        clipsize=p->fptr->size-sizeof(frame)-sizeof(image)*p->fptr->numimages; //get size of effects
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr->getextraptr(),clipsize);
        mprintf("copied %d effect(s), %d bytes",clipnum,clipsize);
}


void M_COPYIMAGES()
{
if (!p || !p->fptr) return;

if (emode==3) //effect
 { M_COPYEFFECTS(); return;}
        cliptype=IMAGES;
        clipnum=p->fptr->numimages;
        clipsize=clipnum*sizeof(image);
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr->getimgptr(),clipsize);
        mprintf("copied %d image(s), %d bytes",clipnum,clipsize);
}

void M_COPYFRAME()
{
if (!p || !p->fptr ) return;
       cliptype=FRAMES;
        clipnum=1;clipsize=p->fptr->size;
        clipobj=p->od;
        memcpy(clipboard,(char *)p->fptr,clipsize);
        mprintf("copied frame %dbytes",clipsize);
}

void M_COPYSERIES()
{
if (!p || !p->fptr) return;    
    cliptype=FRAMES;
    clipnum=p->cs->nf;
    clipsize=p->cs->size;
    clipobj=p->od;    
    memcpy(clipboard,p->cs->first,clipsize);
    mprintf("copied %s, %d frame(s), %d bytes",p->cs->name,clipnum,clipsize);
}      


extern int drawfps;
void M_VERBOSE() {verbose^=1;}
void M_DRAWSTAT() {drawstat^=1;}
void M_FPS() {drawfps^=1;}

void M_200SPEED() {speed=200; SetTimerSpeed(speed);}
void M_150SPEED() {speed=150; SetTimerSpeed(speed);}
void M_100SPEED() {speed=100; SetTimerSpeed(speed);}
void M_50SPEED() {speed=50; SetTimerSpeed(speed);}
void M_25SPEED() {speed=25; SetTimerSpeed(speed);}



void M_OBJECTINFO()
{
 if (!p) return;
 mprintf("%dseries %dwavs %dimages %dK",p->od->nums,p->od->numsounds,p->od->numi,p->od->mem/1024);
 if (p==bgobj)
   ((backgrounddef *)p->od)->generatemap();
}    

void M_VOLUMIZE()
{
 if (p || p->od->readonly) p->od->volumize();
}

void M_RENAMESERIES()
{
 if (!p || p->od->readonly) return;
 seriesedit=p->cs; //edit this series
}

extern int drawregions;
void M_DRAWREGIONS()
{
 drawregions++;
 drawregions%=3;
}    


void M_ADDNEWSERIES()
{
 if (!p) return;
  //add new series
 p->od->addnewseries();

  //go to it
 p->startseries(p->od->nums-1);
}

void M_DELETELASTSERIES()
{
 if (!p) return;

 p->od->nums--;
 if (p->csnum>=p->od->nums)
  p->startseries(p->od->nums-1);
}


void objectdef::addnewseries()
{
  //add new series at end
 sd=(series *)realloc(sd,(nums+1)*sizeof(series));
  //set to zero
 memset(&sd[nums],0,sizeof(series));

 sprintf(sd[nums].name,"series %d",nums);
 nums++;

}    


//********************************
//********************************
//********************************
//********************************
//********************************
//********************************
menu objectmenu=
{
 NULL, //no menu item selected
 0,0,0, //x,y,vertical
 {
  {"Load",M_LOADOBJECT,'l',0},
  {"Save",M_SAVEOBJECT,'s',0},
  {"Kill",M_KILLOBJECT,'k',0},
  {"Volumize",M_VOLUMIZE,'V',0},  
  {"---",NULL,0,0},
  {"Activate",M_ACTIVATEOBJECT,'a',0},
  {"FlipX",M_FLIPXOBJECT,'x',0},
  {"Info",M_OBJECTINFO,0,0},
  {"Move Base",0,'b',0},  
  {"---",NULL,0,0},
  {"Exit",M_QUIT,'q',0},  
  {0,NULL,0,0},
 }
};

menu bgmenu=
{
 NULL, //no menu item selected
 0,0,0, //x,y,vertical
 {
  {"Load",M_LOADBG,0,0},
  {"Edit",M_EDITBG,0,0},
  {0,NULL,0,0},
 }
};


menu editmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Copy Images",M_COPYIMAGES,'[',0},
  {"Copy Effects",M_COPYEFFECTS,0,0},  
  {"Copy Frame",M_COPYFRAME,'{',0},
  {"Copy Series",M_COPYSERIES,'c',0},
  {"---",NULL,0,0},
  {"Paste",M_PASTE,']',0},
  {0,NULL,0,0},
 }
};

menu seriesmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Play",M_PLAY,'p',0},
  {"Play Looped",M_PLAYLOOPED,'P',0},  
  {"Stop",M_STOP,0,0},
  {"-----",0,0,0},
  {"List",M_SERIESLIST,'m',0},
  {"-----",0,0,0},
  {"Rename",M_RENAMESERIES,'R',0},
  {"Add New",M_ADDNEWSERIES,0,0},    
  {"Delete Last",M_DELETELASTSERIES,0,0},    
  {"Save SDF",M_SAVESDF,0,0},    
  {0,NULL,0,0},
 }
};

menu framemenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"New Image",M_NEWIMAGE,'n',0},
  {"New Effect",M_NEWEFFECT,'e',0},
  {"------",NULL,0,0},
  {"Insert",M_INSERTFRAME,'i',0},
  {"Insert After",M_INSERTAFTER,'I',0},
  {"Delete",M_DELETEFRAME,'D',0},
  {0,NULL,0,0},
 }
};

menu miscmenu=
{
 NULL,0,0,0, //x,y,vertical
 {
  {"Toggle Status",M_DRAWSTAT,'h',0},
  {"Toggle Verbose",M_VERBOSE,'v',0},
  {"Toggle DrawRegions",M_DRAWREGIONS,0,0},
  {"Toggle FPS",M_FPS,0,0},
  {"------",0,0,0},
  {"ActivateAll",M_ACTIVATEALL,'A',0},
  {"ResetAll",M_RESETALL,'r',0},
  {"------",0,0,0},
  {"200% speed",M_200SPEED,0,0},
  {"150% speed",M_150SPEED,0,0},
  {"100% speed",M_100SPEED,0,0},
  {" 50% speed",M_50SPEED,0,0},
  {" 25% speed",M_25SPEED,0,0},
  {0,NULL,0,0},
 }
};



menu mainmenu=
{
 NULL, //no menu item selected
 0,0,1, //x,y,vertical
 {
  {"Object",NULL,0,&objectmenu},
  {"Bg",NULL,0,&bgmenu},
  {"Edit",NULL,0,&editmenu},
  {"Series",NULL,0,&seriesmenu},
  {"Frame",NULL,0,&framemenu},
  {"Misc",NULL,0,&miscmenu},
  {0,NULL,0,0},
 }
};









