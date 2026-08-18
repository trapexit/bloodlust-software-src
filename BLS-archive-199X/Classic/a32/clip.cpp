#include <stdlib.h>
#include <mem.h>

#include "clip.h"

#include "object.h"
#include "objdef.h"
#include "objspace.h"

#include "message.h"

CLIPBOARD clipboard;

clipdata::clipdata()
{
 clipboard.adddata(this);
}

//----------------------------------

clip_images::clip_images(struct frame *fptr,class objectdef *tod)
 :clipdata()
{
 od=tod;
 numimages=fptr->numimages;
 size=numimages*sizeof(image);
 i=(image *)malloc(size);

 memcpy(i,fptr->getimgptr(),size);
 msg.printf(3,"%d images copied",numimages);
}
clip_images::~clip_images()
{
 if (i) free(i);
}

void clip_images::paste()
{
 if (!objspace) return;
 object *o=objspace->p;
 if (!o) return;
 frame *fptr=o->fptr;
 series *s=o->cs;
 if (!fptr || !s || o->od!=od) return;

 //remove old images
 int oldimgsize=fptr->numimages*sizeof(image);
 s->deletebytes(fptr->getimgptr(),oldimgsize); //delete old images
 fptr=o->resetfptr();
 fptr->size-=oldimgsize;
 fptr->numimages=0;

 //add new images
 s->insertbytes(fptr->getimgptr(),size);
 fptr=o->resetfptr();
 fptr->size+=size;
 fptr->numimages=numimages;
 memcpy(fptr->getimgptr(),i,size);
 objspace->resetodf(o->onum);
 msg.printf(3,"pasted %d image(s), %d bytes",numimages,size);
}


//----------------------------------
clip_frame::clip_frame(struct frame *fptr,int tnumf,class objectdef *tod)
 :clipdata(),od(tod)
{
 for (f=fptr,size=0,numf=0; numf<tnumf; numf++,size+=f->size,f=f->next());
 //get memory for frames
 f=(frame *)malloc(size);
 memcpy(f,fptr,size); //copy frames
 msg.printf(3,"%d frames copied",numf);

}
clip_frame::~clip_frame() {if (f) free(f);}


void clip_frame::paste()
{
 if (!objspace) return;
 object *o=objspace->p;
 if (!o) return;
 frame *fptr=o->fptr;
 series *s=o->cs;
 if (!s || o->od!=od) return;

 fptr=(frame *)s->insertbytes(fptr,size);
 memcpy(fptr,f,size);
 s->nf+=numf; //insert frame in there
 o->nf=s->nf;
 objspace->resetodf(o->onum);
 msg.printf(3,"pasted %d frame(s), %d bytes",numf,size);
}





//----------------------------------

clip_effects::clip_effects(struct frame *fptr,class objectdef *tod)
 :clipdata()
{
 od=tod;
 numeffects=fptr->numextra;

 size=0;
 effect *t=fptr->geteffectptr();
 for (int i=0; i<numeffects; i++,t=t->next())
   size+=t->size();

 e=(effect *)malloc(size);

 memcpy(e,fptr->geteffectptr(),size);
 msg.printf(3,"%d effects copied %d bytes",numeffects,size);
}
clip_effects::~clip_effects()
{
 if (e) free(e);
}

void clip_effects::paste()
{
 if (!objspace) return;
 object *o=objspace->p;
 if (!o) return;
 frame *fptr=o->fptr;
 series *s=o->cs;
 if (!fptr || !s) return;

 //add new effects
 memcpy(s->insertbytes(((char *)fptr)+fptr->size,size),e,size);
 fptr=o->resetfptr();
 fptr->size+=size;
 fptr->numextra+=numeffects;
 objspace->resetodf(o->onum);
 msg.printf(3,"pasted %d effects(s), %d bytes",numeffects,size);
}


