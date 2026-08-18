

enum ETYPE {E_SOUND,E_GROUNDSHAKE,E_OBJECTSHAKE,E_NEWOBJECT,E_GOTO,E_BLOOD,E_REGION,E_CHILDPOS,E_DIR};
extern int extrasize[];
struct e_sound
{
 byte etype; //E_SOUND
 byte sound; //ordinal number of sound, if high bit set, use System sounds
}; 

struct e_shake
{
 byte etype; //E_OBJECTSHAKE
 byte intensity; //amount of shakage
};

struct e_dir
{
 byte etype; //E_DIR
 byte d; //d=0,d=1,d=2(flip)
};


struct e_newobject
{
byte etype; //E_NEWOBJECT,E_CHILDPOS
uchar onum; //object number
zdpoint pos;  //position of the instantiated object
uchar s;     //series of the object
};


struct e_goto
{
 byte etype; //E_GOTO
 uchar s; //series
}; 

struct e_blood
{
byte etype; //E_BLOOD
uchar bloodtype; //type of blood to fly out
zdpoint pos;  //position of the blood
};


//region types
#define R_IMPERM 0
#define R_VULN   1
#define R_ATTACK 2
#define R_ERASE 0x80

struct e_region
{
byte  etype; //e_region
uchar rtype; //region type
uchar rfunc; //region function (when intersection occurs)
uchar afterseries; //series to go to after hit
zrect r; //rectangle
};



