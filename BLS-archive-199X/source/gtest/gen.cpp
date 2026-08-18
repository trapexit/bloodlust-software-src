#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include "types.h"

#include "file.h"

#include "gen.h"


//----------------------------------------------

#define CYCLESPERSEC  8000000
#define CYCLESPERINST 15
#define FRAMESPERSEC  60

#define TOTALLINES    262
#define VBLANKLINES   38
#define FRAMELINES    224
#define INSTPERLINE (CYCLESPERSEC/FRAMESPERSEC/TOTALLINES)

//#define VBLANKINST   (INSTPERLINE*VBLANKLINES)
//#define FRAMEINST    (INSTPERLINE*FRAMELINES)

#define VBLANKINST   7700
#define FRAMEINST    1300

int gmachine::emulateframe(char *dest)
{
 vdp.startvblank();
 if (vdp.Vintenable) gcpu.doint(0x78);
 if (gcpu.execute(VBLANKINST)) {gcpu.dumpinvalid(); return -1;}

 vdp.startframe();
 if (gcpu.execute(FRAMEINST)) {gcpu.dumpinvalid(); return -1;}

 return 0;
}



//-----------------------------------------------

gmachine::gmachine()
{
 //reset
 ROM=RAM=0;
 romloaded=0;
 romname[0]=0;

 printf("Genesis machine created\n");
}

gmachine::~gmachine()
{
 freerom();
 printf("Genesis machine destroyed\n");
}

void gmachine::reset()
{
 if (!romloaded) return;
 //reset CPU
 gcpu.reset();
 printf("CPU reset\n");
}


int gmachine::loadrom(char *name)
{
 if (romloaded) freerom();

 FILEIO f;
 if (f.open(name)) {printf("Unable to open %s\n",name); return -1;}
 strcpy(romname,name);

 gcpu.rom=ROM=(byte *)calloc(0x400010,1);
 if (ROM) printf("4MB ROM space allocated\n"); else return -2;
 gcpu.ram=RAM=(byte *)calloc(0x10010,1);
 if (RAM) printf("64K RAM space allocated\n"); else {free(ROM); return -2;}

 romsize=f.size();
 f.read(ROM,romsize);
 f.close();
 printf("%d bytes read from %s\n",romsize,romname);
 romloaded=1;

 reset();
 return 0;
}

void gmachine::freerom()
{
 if (!romloaded) return;
 free(ROM);
 free(RAM);
 romloaded=0;
}


//-----------------------------------

void gmachine::writeram(char *fname)
{
 if (!romloaded) return;
 FILEIO g;
 g.create(fname);
 g.write(RAM,0x10000);
 g.close();
 printf("Writing address space\n");
}
