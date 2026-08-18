/* лл OPL2FM.H лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

int  detect_fm(void);
void init_fm(int rhythm_mode);
void load_patches(char *f);

void note_on(BYTE channel, BYTE note, BYTE velocity);
void note_off(BYTE channel, BYTE note, BYTE velocity);
void patch_change(BYTE channel, BYTE newpatch);

void set_fm_volume(unsigned char volume);

void done_fm(void);
