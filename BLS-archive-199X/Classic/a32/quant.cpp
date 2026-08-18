
void quantify(char *path,char *src,char *dest);
void setview(char *palfile);

extern PALETTE *pal;



struct qentry
{
 int count;
 int distance; //distance from closest color in default palette
 COLOR c;

 double rating;

 void findrating()
  {
   rating=((double)count)*((double)distance);
  }
 
 void print()
 {
  printf("color=(%2X,%2X,%2X) count=%d dist=%d rating=%.0f\n",c.r,c.g,c.b,count,distance,rating);
 }     
};


int qentryratingcomp(qentry *a,qentry *b)
{
 if (a->rating>b->rating) return -1;
 if (a->rating<b->rating) return 1;
 return 0;
}    

int qentrycountcomp(qentry *a,qentry *b)
{
 if (a->count>b->count) return -1;
 if (a->count<b->count) return 1;
 return 0;
}    

int qentrydistancecomp(qentry *a,qentry *b)
{
 if (a->distance>b->distance) return -1;
 if (a->distance<b->distance) return 1;
 return 0;
}    


struct quantification
{
 int num;  //number of qentries
 qentry *q; //quantification entries

 quantification() {num=0; q=0;}
      
 int findcolor(COLOR *t) //returns index of color (-1 if not found)
  {
  for (int i=0; i<num; i++)
    if (!memcmp(&q[i].c,t,sizeof(COLOR))) return i;
   return -1;
  }

 int newcolor(COLOR *t)
  {
   q=(qentry *)realloc(q,(num+1)*sizeof(qentry));
   q[num].c=*t;     //next entry
   q[num].count=0;
   return num++;
  }

 int addcolor(COLOR *t,int tcount)
 {
  int index=findcolor(t);
  if (index==-1) //if not found....
    index=newcolor(t); //make new color
  q[index].count+=tcount; //increment
  return index;
 }

 //finds the distances to the closest color for each entry
 void finddistances(PALETTE *p)
 {
  for (int i=0; i<num; i++)
  {
   int idx=p->findclosestmatch(q[i].c,256);
   q[i].distance=(int)sqrt((double)colordistance(p->c[idx],q[i].c));
  }

  //resort by distances
  qsort(q,num,sizeof(qentry),
   (int (*)(const void *,const void *))qentrydistancecomp);
  for (i=0; i<num; i++)
    if (!q[i].distance) break;
  num=i;
 }

 void findratings()
 {
  for (int i=0; i<num; i++)
    q[i].findrating();

  //resort by ratings
  qsort(q,num,sizeof(qentry),
   (int (*)(const void *,const void *))qentryratingcomp);
 }     

 void print()
 {
  for (int i=0; i<num; i++)
   if (q[i].count) q[i].print();
 }
};    


void getcolorcounts(SCR *t,int *cc)
{
 int num=t->xw*t->yw;
 char *s=t->data();
 
 memset(cc,0,256*sizeof(int)); 
 for (int i=0; i<num; i++,s++) cc[*s]++;
}    



int addpalette(char *name,quantification *q)
{
 //get palette
 PALETTE *p=extractpalette(name);
 if (!p) return 1;
 //get bitmap
 SCR *t=ReadLBMFile(name);
 if (!t) return 1;

 printf("  processing %13s\r",name);

 int cc[256]; //color counts
 getcolorcounts(t,cc);
 for (int i=0; i<256; i++)
  if (cc[i]) q->addcolor(&p->c[i],cc[i]);

 return 1;
}    

void quantify(char *path,char *srcname,char *destname)
{
 setview(srcname); //get palette
 if (!pal) return;
 memset(&pal->c[14*15],0,31*sizeof(COLOR)); //zero out unused colors

 PALETTE *def=pal;                  //default palette
 PALETTE *best=pal->duplicate(256); //best palette

 printf("Preforming quantification...\n");

 //determine new palette
 quantification q;
 enumdir(path,(DIRFUNCPTR)addpalette,&q);
 printf("\n");

 printf("Quantification: \n");

 printf("  %3d individual colors found\n",q.num);
 q.finddistances(def);
 q.findratings();
 printf("  %3d colors not in default palette\n",q.num);

 //find 31 best colors
 for (int i=0; i<31; i++)
  {
   best->c[14*15+i]=q.q[0].c; //get most needed color
   q.q[0].print();
   q.finddistances(best);
   q.findratings();
  }
 
// q.print();
 printf("\n");
 
 printf("writing palette to %s...",destname);
 //write finished palette
 FILEIO g;
 g.create(destname);
 g.write(best,256*3);
 g.close();
 printf("done\n");
}    









