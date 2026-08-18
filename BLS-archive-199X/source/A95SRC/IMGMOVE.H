
#ifdef ANIMATOR
class imagemove:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  object *o;       //object bound to
 class objectdefw *od;   //object def
 struct series *cs;      //series of object that this frame is in
 struct  frame *fptr;     //frame of object that this image is in
 struct  image *i;        //image
 public:
 imagemove(object *to,image *ti,int x,int y);
 imagemove(objectdefw *od,image *ti, int x,int y);
 ~imagemove();
 virtual int drag(mouse &m);
 virtual int release(mouse &m);
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "imagemove";}
 virtual void draw(char *dest);
 virtual int keyhit(char kbscan,char key);
 void bind(object *to);
 void deleteimage();
 void insertimage();
 void forward(),backward(),sendtoback(),bringtofront();
};


class bgimagemove:public GUIrect
{
 class  objectspace *osp;//objectspace bound to
 class  bgobject *bg;       //bgobject bound to
 class  bgdef *bd;          //bgobject def
 struct  bgimage *i;        //bgimage
 public:
 bgimagemove(bgobject *tbgo,bgimage *ti,int x,int y);
 bgimagemove(bgdef *bd,bgimage *ti, int x,int y);
 ~bgimagemove();
 virtual int drag(mouse &m);
 virtual int release(mouse &m);
 virtual int acceptfocus() {return 1;}
 virtual char *getname() {return "bgimagemove";}
 virtual void draw(char *dest);
 virtual int keyhit(char kbscan,char key);
 void bind(bgobject *tbgo);
 void deleteimage();
 void insertimage();
};
#endif
