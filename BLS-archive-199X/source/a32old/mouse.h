

#define MB_LEFT   1
#define MB_RIGHT  2
#define MB_MIDDLE 4

class mouse
{
 int oldb; //last mouse button state

public:
 int x,y; //current mouse position
 int b;   //current holding down of mouse buttons
 char click; //click state of buttons
 char rel;  //release state of buttons

 mouse(); //constructor
 
 void refresh();
};


