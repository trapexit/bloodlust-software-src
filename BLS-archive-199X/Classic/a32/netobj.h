#ifndef _NETOBJ_
#define _NETOBJ_

#include "object.h"

//networkable object
class netobject:public object
{
 public:
 netnode *n;
 netobject *nprev,*nnext; //linked list of netobjects

 ushort objid;        //id # of this object
 uchar netupdated:1; //has this netobject been updated ?

 void recvobjpos(struct objpos *op);
 void sendobjpos(struct objpos *op);
 
 netobject(objectdef *objdef,int s,int tx,int ty,int tz,int td,netnode *tnode);
 virtual ~netobject();
};


//object controlled by local node
class localobject:public netobject
{
  public:
  localobject(objectdef *objdef,int s,int tx,int ty,int tz,int td);
  virtual ~localobject();

  virtual void tick()
   {
    object::tick(); //do regular tick
    if (updated) netupdated=1; //it has been updated
   }
  virtual object *spawn(int objnum,int series,int tx,int ty,int tz, int td);
};


//object controlled by remote node
class remoteobject:public netobject
{
  public:
  remoteobject(class netnode *tnode,struct p_newobject *p); //create remote object from p_newobject

  virtual void tick() {}; //do nothing on ticks
  virtual int advanceframe();
  virtual void effect() { } //remote object can't do effects
  virtual object *spawn(int objnum,int series,int tx,int ty,int tz, int td)
       {return 0;} //remote object can't spawn anything
};


#endif


