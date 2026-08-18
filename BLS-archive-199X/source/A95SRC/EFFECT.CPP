
#include "region.h"
#include "effect.h"
#include "message.h"

//size of each effect structure
typedef int (effect::*E_GETSIZEPTR)();
E_GETSIZEPTR effectgetsizeptrs[]=
{
 (E_GETSIZEPTR)e_sound::size,
 (E_GETSIZEPTR)e_groundshake::size,
 (E_GETSIZEPTR)e_objectshake::size,
 (E_GETSIZEPTR)e_newobject::size,
 (E_GETSIZEPTR)e_goto::size,
 (E_GETSIZEPTR)e_blood::size,
 (E_GETSIZEPTR)e_oldregion::size,
 (E_GETSIZEPTR)e_dir::size,
 (E_GETSIZEPTR)e_gotoseriesname::size,
 (E_GETSIZEPTR)e_region::size,
 (E_GETSIZEPTR)e_eraseregion::size,
 (E_GETSIZEPTR)e_suicide::size,
 (E_GETSIZEPTR)e_loseenergy::size,
 (E_GETSIZEPTR)e_none::size,
 (E_GETSIZEPTR)e_onenergy::size,
 0
};
int effect::size() {return (this->*effectgetsizeptrs[type])();}

//effect triggers
typedef void (effect::*E_TRIGGERPTR)(class object *);
E_TRIGGERPTR e_triggerptrs[]=
{
 (E_TRIGGERPTR)e_sound::trigger,
 (E_TRIGGERPTR)e_groundshake::trigger,
 (E_TRIGGERPTR)e_objectshake::trigger,
 (E_TRIGGERPTR)e_newobject::trigger,
 (E_TRIGGERPTR)e_goto::trigger,
 (E_TRIGGERPTR)e_blood::trigger,
 (E_TRIGGERPTR)e_oldregion::trigger,
 (E_TRIGGERPTR)e_dir::trigger,
 (E_TRIGGERPTR)e_gotoseriesname::trigger,
 (E_TRIGGERPTR)e_region::trigger,
 (E_TRIGGERPTR)e_eraseregion::trigger,
 (E_TRIGGERPTR)e_suicide::trigger,
 (E_TRIGGERPTR)e_loseenergy::trigger,
 (E_TRIGGERPTR)e_none::trigger,
 (E_TRIGGERPTR)e_onenergy::trigger,
 0
};
void effect::trigger(class object *o) {(this->*e_triggerptrs[type])(o);}



//hiteffect triggers
typedef void (effect::*E_HITTRIGGERPTR)(class object *,lpoint *);
E_HITTRIGGERPTR e_hittriggerptrs[]=
{
 0, 0, 0,
 (E_HITTRIGGERPTR)e_newobject::hittrigger, 0,
 (E_HITTRIGGERPTR)e_blood::hittrigger,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
void effect::hittrigger(class object *o,lpoint *p)
{
 if (e_hittriggerptrs[type])
       (this->*e_hittriggerptrs[type])(o,p);
  else (this->*e_triggerptrs[type])(o);
}






//effect names
char *effectnames[]=
 {
  "Sound effect",
  "Ground shake",
  "Object shake",
  "New object",
  "Goto series",
  "Blood",
  "oldregion",
  "Direction",
  "Gotoseriesname",
  "Region",
  "Eraseregion",
  "Suicide",
  "Loseenergy",
  "None",
  "On Energy",
  0
 };
char *effect::getname() {return effectnames[type];};


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

#include "object.h"

//object sound effect
e_sound::e_sound():effect(E_SOUND) {sound=0;}
void e_sound::trigger(object *o)
{
 o->sound(sound); //play sound
}

//shake
e_shake::e_shake(uchar t):effect(t) {intensity=0; }

//object shake
e_objectshake::e_objectshake():e_shake(E_OBJECTSHAKE) {}
void e_objectshake::trigger(object *o)
{
 static int shakelength[4]={5,25,50,150};
 new shaketrajectory(shakelength[intensity],o);
}
//ground shake
e_groundshake::e_groundshake():e_shake(E_GROUNDSHAKE) {}
void e_groundshake::trigger(object *o)
{
}



//goto series
e_goto::e_goto():effect(E_GOTOSERIES) {s=0;}
void e_goto::trigger(object *o)
{
 o->startseries(s);
}


//goto series
e_gotoseriesname::e_gotoseriesname():effect(E_GOTOSERIESNAME) {name[0]=0;}
void e_gotoseriesname::trigger(object *o)
{
 o->startseries(name);
}



//change directions
e_dir::e_dir():effect(E_DIR) {d=0;}
void e_dir::trigger(object *o)
{
 if (d==0) {o->d=0; return;}
 if (d==1) {o->d=1; return;}
 if (d==2) {o->flip(); return;}
}

//e_newobject
e_newobject::e_newobject():effect(E_NEWOBJECT)
   { onum=0; pos.clear(); s=0;}
void e_newobject::trigger(object *o)
{
 o->spawn(onum ? onum : o->onum,s,
   !o->d ? (o->x>>16)+pos.x : (o->x>>16)-pos.x,
  (o->y>>16)+pos.y,(o->z>>16)+pos.z,
  !pos.d ? o->d : o->d^1);
}

void e_newobject::hittrigger(object *o,lpoint *p)
{
 o->spawn(onum ? onum : o->onum,s,
   !o->d ? p->x+pos.x : p->x-pos.x,
  p->y+pos.y,(o->z>>16)+pos.z,
  !pos.d ? o->d : o->d^1);
}


//e_blood
e_blood::e_blood():effect(E_BLOOD)
   { bloodtype=0; pos.clear();}
void e_blood::trigger(object *o)
{
}
void e_blood::hittrigger(object *o,lpoint *p) {}


//e_oldregion
e_oldregion::e_oldregion():effect(E_OLDREGION) {;}
void e_oldregion::trigger(object *o) {}




//e_region
void e_region::settype(int t)
{
 switch (t)
  {
   case R_IMPERM:
       type=t;
       flags=0;
       break;
   case R_VULN:
   case R_ATTACK:
     type=t;
     flags=0;
     break;
  }
}

e_region::e_region():effect(E_REGION)
{
 append=0;
 settype(R_IMPERM);
 r.clear();
 numle=numre=0;
}

int e_region::getlesize()
{
 int size=0;
 effect *t=getleptr();
 for (int i=numle; i>0; i--,t=t->next()) size+=t->size();
 return size;
}
int e_region::getresize()
{
 int size=0;
 effect *t=getreptr();
 for (int i=numre; i>0; i--,t=t->next()) size+=t->size();
 return size;
}
int e_region::size()
{
 int size=sizeof(e_region);
 effect *t=getleptr();
 for (int i=numle+numre; i>0; i--,t=t->next()) size+=t->size();
 return size;
}

void e_region::trigger(object *o)
{
 if (o->r[type]) o->r[type]->update(this);
        else    new region(o,this);
}

void e_region::hittrigger(object *o,object *r,lpoint *p)
{
 int i;
 effect *t;
 //do local effects
 t=getleptr();
 for (i=numle; i>0; i--, t=t->next()) t->hittrigger(o,p);

 //do remote effects
 t=getreptr();
 for (i=numre; i>0; i--, t=t->next()) t->hittrigger(r,p);
}



e_eraseregion::e_eraseregion():effect(E_ERASEREGION) {type=0;}
void e_eraseregion::trigger(object *o)
{
 if (type!=R_ALL)
   {if (o->r[type]) delete o->r[type];}
 else
  { //delete all
   if (o->r[0]) delete o->r[0];
   if (o->r[1]) delete o->r[1];
   if (o->r[2]) delete o->r[2];
  }
}

e_suicide::e_suicide():effect(E_SUICIDE) {}
void e_suicide::trigger(object *o)
{
 o->kill();
}


e_loseenergy::e_loseenergy():effect(E_LOSEENERGY) {energy=0;}
void e_loseenergy::trigger(object *o)
{
 o->loseenergy(energy);
}


e_none::e_none():effect(E_NONE) {;}
void e_none::trigger(object *o)
{
}



e_onenergy::e_onenergy():effect(E_ONENERGY),e(E_NONE)
{
 condition=EC_LESS;
 energy=0;
}
void e_onenergy::trigger(object *o)
{
// msg.printf(2,"onenergy %d ? %d",o->energy,energy);
 switch(condition)
  {
   case EC_LESS:    if (o->energy<energy) e.trigger(o); break;
   case EC_EQUALS:  if (o->energy==energy) e.trigger(o); break;
   case EC_GREATER: if (o->energy>energy) e.trigger(o); break;
  }
}


/*
//
e_::e_():effect(E_) {;}
void e_::trigger(object *o)
{
}
*/

effect *neweffect(int type)
{
 switch(type)
 {
   case E_SOUND: return new e_sound();
   case E_OBJECTSHAKE: return new e_objectshake();
   case E_GROUNDSHAKE: return new e_groundshake();
   case E_GOTOSERIES: return new e_goto();
   case E_DIR: return new e_dir();
   case E_NEWOBJECT: return new e_newobject();
   case E_BLOOD: return new e_blood();
   case E_GOTOSERIESNAME: return new e_gotoseriesname();
   case E_REGION: return new e_region();
   case E_ERASEREGION: return new e_eraseregion();
   case E_SUICIDE: return new e_suicide();
   case E_LOSEENERGY: return new e_loseenergy();
   case E_NONE: return new e_none();
   case E_ONENERGY: return new e_onenergy();
 };
 return 0;
};























