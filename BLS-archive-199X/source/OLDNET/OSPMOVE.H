//---------------------------------------------------------

class objectmove:public GUIrect
{
 object *o; //object this is monitoring

 public:
 objectmove(object *to,int x,int y)
  :GUIrect(guiroot,x,y,x+65,y+45),o(to)
  {setmodal(this);}

 virtual ~objectmove() {setmodal(0);}

 virtual int drag(mouse &m)
  {
   return 1;
  }
 virtual int release(mouse &m) {delete this; return 1; }

 virtual void draw(char *dest)
  {
   if (o)
   {
    o->setrelx((m.x-o->osp->x1)<<16);
    if (!(kbstat&KB_SHIFT))
     {
      o->setrely(((m.y-o->osp->y1)<<16)-o->relz());
     }
     else
     {
      o->setrelz(((m.y-o->osp->y1)<<16)-o->rely());
      o->setrely(((m.y-o->osp->y1)<<16)-o->relz());
     }
    o->updated|=7;

    if (o->active && !o->lastforever)
    {
     o->getbasey();
     o->startseries(o->csnum);
     o->falldown();
    } else
    {
     o->getbasey();
     //if (o->y>o->basey) o->y=o->basey;
    }
   }

   fill(0);
   outline(2);
   font[2]->printf(x1+2,y1+2,"    X: %d",o->x>>16);
   font[2]->printf(x1+2,y1+12,"    Y: %d",o->y>>16);
   font[2]->printf(x1+2,y1+22,"    Z: %d",o->z>>16);
   font[2]->printf(x1+2,y1+32,"BASEY: %d",o->basey>>16);
  };
 virtual char *getname() {return "objmove";}
};

//------------------------------------------------------

class bgmove:public GUIrect
{
 bgobject *bg; //object this is monitoring

 public:
 bgmove(bgobject *tbg,int x,int y)
  :GUIrect(guiroot,x,y,x+65,y+25),bg(tbg) {setmodal(this);}

 virtual ~bgmove() { setmodal(0); }

 virtual int drag(mouse &m)
  {
    bg->move((m.x-m.oldx)<<16,(m.y-m.oldy)<<16);
    return 1;
  }
 virtual int release(mouse &m) {delete this; return 1; }

 virtual void draw(char *dest)
  {
   if (!bg) return;

   fill(0);
   outline(2);
   font[2]->printf(x1+2,y1+2," X: %d",bg->x>>16);
   font[2]->printf(x1+2,y1+12," Y: %d",bg->y>>16);
  };
 virtual char *getname() {return "bgmove";}
};

