#ifndef _gfxmisc_h_
#define _gfxmisc_h_

void gfxSetMode(int mode);
void gfxExtendMode(void);

void gfxSetPalette(unsigned char *palette);
char *gfxLoadPCX(char *pcxFName, char *image);

extern long *cosTab;
extern long *sinTab;
extern long *scaleTab;
extern long *yTab;
extern long startingPointU[256];
extern long startingPointV[256];

void gfxInitTables(void);
void gfxCleanup(void);

#endif
