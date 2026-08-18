int  InitSB(int card);
void TerminateSB();


typedef struct VOC
{
unsigned short size;
short int rate;
unsigned char tc;
short int length; //in 1/100 ticks
char data;
};


extern "C"
{
void InitVoice();
void TerminateVoice();
void OutputVoice(VOC *data,int disp);
void StopVoice();
int MixVoc(VOC *dest,VOC *voc1,int disp1,VOC *voc2,int disp2);
}
extern volatile short int DMAstat;

void PlayMixedSoundEffect(VOC *se,int priority);
void PlayModalSoundEffect(VOC *se);
void *realread(int h,int size);
void *realvolread(int h);
extern char *rbaseptr;

