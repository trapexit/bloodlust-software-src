
#include "mouse.h"

extern "C" {
    void __cdecl initmouse();
    int  __cdecl readmouse(int *x,int*y);
};


mouse::mouse()
{
 initmouse();
 oldb=0;
 x=y=0; click=rel=b=0;
}


void mouse::refresh()
{
 oldb=b;
 b=readmouse(&x,&y);
 click=b&(oldb^7); //press triggers
 rel= oldb&(b^7); //release triggers
}    




