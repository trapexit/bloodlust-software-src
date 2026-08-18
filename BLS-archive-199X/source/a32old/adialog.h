class dialog {
 int x,y,xw,yw;

 char title[50];

 //list stuff
 void *context;
 char *array;   //pointer to all the data elements in the array
 int size,num;  //size of each element, number of elements
 void (*getstr)(char *s,void *element); //gets the string for the element

 void (*selfunc)(void *array,int num,void *context);


 int basey; //basey coord
 int selected; //the selected element
 unsigned dblclicktime;
 char buttonregion; //last region that button was held down on
 unsigned scrolltime;

public:    
 char active; //is it active?

 void draw(); //draw the dialog, process mouse
 void init(rect *tpos,char *ttitle,void *tarray,int tsize,int tnum,int selnum,void *gsf,
    void *sf, void *context);
 
};




void dlg_objectdef(char *s,objectdef *e);
void dlg_series(char *s,series *e);
void dlg_extra(char *s,char **a);
void dlg_cfunc(char *s,char *e);


