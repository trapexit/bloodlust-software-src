/* лл MIDILIB.H ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

#define ON  1
#define OFF 0

typedef unsigned char BYTE;

typedef struct MUSIC
  {
    unsigned int datasize;
    BYTE (*dataptr)[64000];
    int quarter_ticks;
  } MUSIC;

int init_music(int rhythm_mode);
void done_music(void);

void load_music(unsigned int h, MUSIC **music);
void free_music(MUSIC **music);

void start_music(MUSIC *music, int loop);
void stop_music(void);

void set_channelmask(int channel, int value);
int  get_channelmask(int channel);

void set_multiplier(float mult);
void set_music_volume(unsigned char new_volume);

extern volatile int playing;
