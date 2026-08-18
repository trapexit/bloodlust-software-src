#ifndef _EFFECT_
#define _EFFECT_

#include "types.h"

#define E_SOUND         0
#define E_GROUNDSHAKE   1
#define E_OBJECTSHAKE   2
#define E_NEWOBJECT     3
#define E_GOTOSERIES    4
#define E_BLOOD         5
#define E_OLDREGION     6
#define E_DIR           7
#define E_GOTOSERIESNAME 8
#define E_REGION        9
#define E_ERASEREGION   10
#define E_SUICIDE       11
#define E_LOSEENERGY    12
#define E_NONE          13
#define E_ONENERGY      14
#define E_RANDOMTRAJ    15
#define E_ONHITGROUND   16


//on energy
//layer changing
//random trajectory effect

extern char *effectnames[];

#include "guirect.h"

//-------------------------------------------------


//class describing an effect, which can be attched to a frame for
//triggers and the like
class effect
{
 uchar type:7;     //type of this effect
 uchar onhit:1;   //does this effect only occur on a hit?
 public:
 friend class object;

 effect(uchar t):type(t),onhit(0) {};
 //get size of this effect
 int size();
 //get pointer to next effect
 effect *next() {return (effect *)(((char *)this)+size());}
 //activate effect
 void trigger(class object *o);
 //activate affect after region intersection
 void hittrigger(class object *o,struct lpoint *p);
 //get name
 char *getname();

 int gettype() {return type;}
};


//object sound effect
class e_sound:public effect
{
 friend class e_sound_edit;
 uchar sound;   //number of sound
 public:
 e_sound();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};

//shake effect
class e_shake:public effect
{
 friend class e_shake_edit;
 protected:
 uchar intensity;
 public:
 e_shake(uchar t);
};
class e_objectshake:public e_shake
{
 public:
 e_objectshake();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};
class e_groundshake:public e_shake
{
 public:
 e_groundshake();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};


//direction change
class e_dir:public effect
{
 friend class e_dir_edit;
 uchar d; //direction (2==flip)
 public:
 e_dir();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};

//goto
class e_goto:public effect
{
 friend class e_goto_edit;
 uchar s; //series to goto
 public:
 e_goto();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};


//goto series name
class e_gotoseriesname:public effect
{
 friend class e_gotoseriesname_edit;
 char name[16]; //series to goto
 public:
 e_gotoseriesname();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};




class e_newobject:public effect
{
 friend class e_newobject_edit;
 uchar onum; //object number
 zdpoint pos; //relative position
 uchar s; //series
 public:
 e_newobject();
 void trigger(class object *o);
 void hittrigger(class object *o,struct lpoint *p);

 int size() {return sizeof(*this);}

 #ifdef ANIMATOR
 void draw(char *dest,class object *o);
 #endif
};

class e_blood:public effect
{
 friend class e_blood_edit;
 uchar bloodtype; //bloodtype
 zdpoint pos; //relative position
 public:
 e_blood();

 void hittrigger(class object *o,struct lpoint *p);
 void trigger(class object *o);
 int size() {return sizeof(*this);}

 #ifdef ANIMATOR
 void draw(char *dest,class object *o);
 #endif
};


class e_oldregion:public effect
{
 uchar rtype; //region type
 uchar rfunc; //region function (when intersection occurs)
 uchar afterseries; //series to go to after hit
 zrect r; //rectangle
 public:
 e_oldregion();
 void trigger(class object *o);
 int size() {return sizeof(*this);}

 #ifdef ANIMATOR
 void draw(char *dest,class object *o);
 #endif
};



//type of region
#define R_IMPERM 0
#define R_VULN   1
#define R_ATTACK 2
#define R_ALL 3


//region flags
#define R_PLAYER 1
#define R_ENEMY  2
#define R_OTHER  4
#define R_FORCEFACE 8

#define R_PUSH           1
#define R_PUSHABLE       2
#define R_STANDONTOPABLE 4
#define R_BOUNCE         8
#define R_STATIONARY     16


class e_region:public effect
{
 friend class region;
 friend class object;
 uchar type:3;   //imperm,vuln,attack
 uchar append:1; //append to existing obj regions?

 zrect r; //rectangle describing region
 uchar flags; //flags

 public:
 uchar numle;   //number of local effects
 uchar numre;  //number of remote effects
 int getlesize();    //get size of all local effects
 int getresize();    //get size of all remote effects

 effect *getleptr() {return (effect *)(((char *)this)+sizeof(*this)); }; //get ptr to local effects
 effect *getreptr() {return (effect *)(((char *)this)+sizeof(*this)+getlesize());} //get ptr to remote effects

 e_region();
 int size();
 void settype(int t);
 int getflags() {return flags;}
 void trigger(class object *o);
// void hittrigger(object *o,object *r,lpoint *p);
 void hitlocaltrigger(object *o,lpoint *p);
 void hitremotetrigger(object *r,lpoint *p);

 #ifdef ANIMATOR
 friend class e_region_edit;
 void draw(char *dest,class object *o);
 #endif
};





class e_eraseregion:public effect
{
 friend class e_eraseregion_edit;
 uchar type; //type of region to erase from object
 public:
 e_eraseregion();
 void trigger(class object *o);
 int size() {return sizeof(*this);}

};


class e_suicide:public effect
{
 public:
 e_suicide();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};



class e_loseenergy:public effect
{
 friend class e_loseenergy_edit;
 short energy;
 public:
 e_loseenergy();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};

class e_none:public effect
{
 public:
 e_none();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};

//effect conditions
#define EC_EQUALS  0
#define EC_LESS    1
#define EC_GREATER 2
class e_onenergy:public effect
{
 friend class e_onenergy_edit;
 uchar condition;
 short energy;
 effect e; //effect to be generated if condition succeeds
 public:
 e_onenergy();
 void trigger(class object *o);
 int size() {return (sizeof(*this)-1+e.size());}
};




class e_onhitground:public effect
{
 friend class e_onhitground_edit;
 effect e; //effect to be generated if condition succeeds
 public:
 e_onhitground();
 void trigger(class object *o);
 void hittrigger(class object *o,struct lpoint *p);

 int size() {return (sizeof(*this)-1+e.size());}
};


//do random trajectory
class e_randomtraj:public effect
{
 friend class e_randomtraj_edit;
 short dur;
 short dxmin,dxmax; //delta ranges
 short dymin,dymax;
 short dzmin,dzmax;

 public:
 e_randomtraj();
 void trigger(class object *o);
 int size() {return sizeof(*this);}
};




effect *neweffect(int type);


#endif
