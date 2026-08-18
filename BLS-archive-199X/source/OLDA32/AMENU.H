class menuitem
{
public:
 char *text;     //text for this menu item
 void (*func)(); //function to call
 char key;       //shortcut key
 class menu *submenu;  //submenu

 void draw(int x,int y,int sel); //returns true if selected
 int width(); //finds width of this item
};



class menu
{
public:
class menuitem *msel; //currently selected menu item
int xw,yw; //width/height of menu
char orient; //1horiz 0vertical
class menuitem m[];

void draw(int x,int y);
void drawhoriz(int x,int y);
void getwidthheight();

class menuitem *checkkey(char key);
};

