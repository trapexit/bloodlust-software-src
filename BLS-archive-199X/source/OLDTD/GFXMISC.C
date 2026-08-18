#include <conio.h>
#include <dos.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "gfxmisc.h"

long *cosTab;
long *sinTab;
long *scaleTab;
long *yTab;
long startingPointU[256];
long startingPointV[256];


void
gfxExtendMode(void)
{
  //      outportb(0x3C2, 0xE3);
}


void
gfxInitTables(void)
{
        int i;
        unsigned long dU, dV;
        unsigned bytes;
        int h;

        sinTab = (long *) malloc(256 * 4);
        cosTab = (long *) malloc(256 * 4);
        scaleTab = (long *) malloc(128 * 4);
        yTab     = (long *) malloc(200 * 4);

        _dos_open("spin.vol",O_BINARY | O_RDONLY,&h);
        _dos_read(h,sinTab,256*4,&bytes);
        _dos_read(h,cosTab,256*4,&bytes);
        _dos_close(h);

        for (i=0; i < 200; i++) 
                yTab[i] = i * 320;
        

        for (i=0; i < 128; i++) {
                scaleTab[i] = (i << 4);
        }

        for (i=0; i<255; i++) {
                dU = sinTab[i];
                dV = cosTab[i];
                startingPointU[i] = ((-160 * dV) + (100 * dU)) + (160 << 10);
                startingPointV[i] = ((-160 * dU) - (100 * dV)) + (100 << 10);
        }


}


void
gfxCleanup(void)
{
        free(sinTab);
        free(cosTab);
        free(scaleTab);
        free(yTab);
}

