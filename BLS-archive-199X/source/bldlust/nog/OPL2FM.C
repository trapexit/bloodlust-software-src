/* ÛÛÛÛÛÛÛÛ Copyright 1994 by Ethan Brodsky.  All rights reserved. ÛÛÛÛÛÛÛÛ */

/* ÛÛ OPL2FM.C ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fcntl.h>

/* ÛÛ Interface ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

typedef unsigned char BYTE;

int  detect_fm(void);
void init_fm(int rhythm_mode);
void load_patches(char *f);

void note_on(BYTE channel, BYTE note, BYTE velocity);
void note_off(BYTE channel, BYTE note, BYTE velocity);
void patch_change(BYTE channel, BYTE newpatch);

void set_fm_volume(unsigned char volume);

void done_fm(void);

/* ÛÛÛ Implementation ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))

#define PERCUSSION_CHANNEL 9

#define ADDRESSPORT 0x388
#define STATUSPORT  0x388
#define DATAPORT    0x389

#define lo(value) ((unsigned char)((value) & 0x00FF))
#define hi(value) ((unsigned char)((value) >> 8))

typedef struct
  {
    BYTE characteristics[2];
    BYTE level[2];
    BYTE attackdecay[2];
    BYTE sustainrelease[2];
    BYTE waveformselect[2];
    BYTE feedback;
    signed char keymapping;
    BYTE volume;
  } TIMBRE;

unsigned int notes[128] =
  {
    0x20AB, 0x20B6, 0x20C0, 0x20CC,
    0x20D8, 0x20E5, 0x20F2, 0x2101,
    0x2110, 0x2120, 0x2132, 0x2144,

    0x2157, 0x216B, 0x2181, 0x2198,
    0x21B0, 0x21CA, 0x21E5, 0x2202,
    0x2220, 0x2241, 0x2263, 0x2287,

    0x2557, 0x256B, 0x2581, 0x2598,
    0x25B0, 0x25CA, 0x25E5, 0x2602,
    0x2620, 0x2641, 0x2663, 0x2687,

    0x2957, 0x296B, 0x2981, 0x2998,
    0x29B0, 0x29CA, 0x29E5, 0x2A02,
    0x2A20, 0x2A41, 0x2A63, 0x2A87,

    0x2D57, 0x2D6B, 0x2D81, 0x2D98,
    0x2DB0, 0x2DCA, 0x2DE5, 0x2E02,
    0x2E20, 0x2E41, 0x2E63, 0x2E87,

    0x3157, 0x316B, 0x3181, 0x3198,
    0x31B0, 0x31CA, 0x31E5, 0x3202,
    0x3220, 0x3241, 0x3263, 0x3287,

    0x3557, 0x356B, 0x3581, 0x3598,
    0x35B0, 0x35CA, 0x35E5, 0x3602,
    0x3620, 0x3641, 0x3663, 0x3687,

    0x3957, 0x396B, 0x3981, 0x3998,
    0x39B0, 0x39CA, 0x39E5, 0x3A02,
    0x3A20, 0x3A41, 0x3A63, 0x3A87,

    0x3D57, 0x3D6B, 0x3D81, 0x3D98,
    0x3DB0, 0x3DCA, 0x3DE5, 0x3E02,
    0x3E20, 0x3E41, 0x3E63, 0x3E87,

    0x4157, 0x416B, 0x4181, 0x4198,
    0x41B0, 0x41CA, 0x41E5, 0x4202,
    0x4220, 0x4241, 0x4263, 0x4287,

    0x4557, 0x456B, 0x4581, 0x4598,
    0x45B0, 0x45CA, 0x45E5, 0x4602
  };

typedef struct
  {
    char patch;
  } LOGICALCHANNEL;

typedef struct
  {
    BYTE ksl;
    BYTE modlevel;
    signed char keymapping;
    BYTE insvolume;
  } PHYSICALCHANNEL;

LOGICALCHANNEL logchinfo[16];

PHYSICALCHANNEL phychinfo[11];
int  channelmap[11];
BYTE notemap[11];
BYTE velocitymap[11];

TIMBRE *instrumentmap[128];
TIMBRE *percussionmap[128];

int quietness = 0;           // 0-63: added to carrier attenuation

int rhythm;
int melodic_voices;
int percussion_channel;

unsigned char percussion_control;

long ch_lastused[11];        // Keeps track of when each channel was last used
long cur_note;               // Total notes that have occurred

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void set_reg(int regnumber, BYTE value)
  {
    int  i;
//    char temp;

    outp(ADDRESSPORT, regnumber);
    for (i=0; i < 6; i++) inp(ADDRESSPORT);
    outp(DATAPORT, value);
    for (i=0; i < 35; i++) inp(ADDRESSPORT);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void set_ch_reg(int reg, int channel, BYTE value)
  {
    set_reg(reg + channel, value);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

int op_ofs(int channel, int op)
  {
    int offset;

    offset = channel;
    if (channel > 2) offset += 5;
    if (channel > 5) offset += 5;
    if (op)          offset += 3;

    return(offset);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void set_op_reg(int reg, int channel, int op, BYTE value)
  {
    set_reg(reg + op_ofs(channel, op), value);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

unsigned char get_status(void)
  {
    return(inp(STATUSPORT));
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

unsigned char slot_reg(int reg, int slotnum)
  {
    int offset;

    offset = slotnum;

    if (slotnum > 5)  offset += 2;
    if (slotnum > 11) offset += 2;

    return(reg+offset);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void set_slot_reg(int reg, int slotnum, BYTE value)
  {
    set_reg(slot_reg(reg, slotnum), value);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void set_percussion_slot(int phychannel, int slotnum, TIMBRE *timbre)
  {
    set_slot_reg(0x20, slotnum, timbre->characteristics[0]);
    set_slot_reg(0x40, slotnum, timbre->level[0]);
    set_slot_reg(0x60, slotnum, timbre->attackdecay[0]);
    set_slot_reg(0x80, slotnum, timbre->sustainrelease[0]);
    set_slot_reg(0xE0, slotnum, timbre->waveformselect[0]);
    set_ch_reg(0xC0, phychannel, timbre->feedback);

    phychinfo[phychannel].ksl        = timbre->level[0] & 0xC0;
    phychinfo[phychannel].keymapping = timbre->keymapping;
    phychinfo[phychannel].insvolume  = timbre->volume;
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void set_timbre(int phychannel, TIMBRE *timbre)
  {
    int op;

   // If the channel is still releasing the previous
   // note, release it immediately to eliminate pops.
    set_op_reg(0x80, phychannel, 1, 0xFF);
    set_op_reg(0x80, phychannel, 0, 0xFF);

    for (op=0; op < 2; op++)
      {
        set_op_reg(0x20, phychannel, op, timbre->characteristics[op]);
        set_op_reg(0x40, phychannel, op, timbre->level[op]);
        set_op_reg(0x60, phychannel, op, timbre->attackdecay[op]);
        set_op_reg(0x80, phychannel, op, timbre->sustainrelease[op]);
        set_op_reg(0xE0, phychannel, op, timbre->waveformselect[op]);
      }
    set_ch_reg(0xC0, phychannel, timbre->feedback);

    phychinfo[phychannel].ksl        = timbre->level[1] & 0xC0;
    phychinfo[phychannel].modlevel   = timbre->level[0];
    phychinfo[phychannel].keymapping = timbre->keymapping;
    phychinfo[phychannel].insvolume  = timbre->volume;
  }


void stop_fm()
{
int i;

     //    reset_synth();

    for (i=0; i < 9; i++)
      set_ch_reg(0xB0, i, 0x00);
     
         for (i=0; i < 11; i++)
                {
                  phychinfo[i].ksl        = 0xFF;
                  phychinfo[i].modlevel   = 0xFF;
                  phychinfo[i].keymapping = 0;
                  phychinfo[i].insvolume  = 0;

                  channelmap[i]  = 0xFF;
                  notemap[i]     = 0xFF;
                  velocitymap[i] = 0xFF;
                }

}
  

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

int detect_fm(void)
  {
    int i, status1, status2;

    set_reg(4, 0x60);
    set_reg(4, 0x80);
    status1 = get_status();
    set_reg(2, 0xFF);
    set_reg(4, 0x21);
    for (i=1; i < 125; i++)  inp(ADDRESSPORT);
    status2 = get_status();
    set_reg(4, 0x60);
    set_reg(4, 0x80);

    return((((status1 & 0xE0) == 0) && ((status2 & 0xE0) == 0xC0)));
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void fm_exitproc(void)
  {
    done_fm();
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void reset_synth(void)
  {
    int i;

    for (i=0x01; i <= 0xF5; i++) set_reg(i, 0x00);
    for (i=0x40; i <= 0x55; i++) set_reg(i, 0x3F);
    if (rhythm)
      set_reg(0xBD, (percussion_control = 0x20));
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void init_fm(int rhythm_mode)
  {
    int i;

    rhythm = rhythm_mode;
    if (rhythm)
      {
        percussion_channel = PERCUSSION_CHANNEL;
        melodic_voices     = 6;
      }
    else
      {
        percussion_channel = 255;
        melodic_voices     = 9;
      }

    reset_synth();
    for (i=0;  i < 128; i++) instrumentmap[i] = NULL;
    for (i=35; i < 82;  i++) instrumentmap[i-35] = NULL;
    for (i=0;  i < 16;  i++)
      {
        logchinfo[i].patch = 0xFF;
      }
    for (i=0; i < 11; i++)
      {
        phychinfo[i].ksl        = 0xFF;
        phychinfo[i].modlevel   = 0xFF;
        phychinfo[i].keymapping = 0;
        phychinfo[i].insvolume  = 0;

        channelmap[i]  = 0xFF;
        notemap[i]     = 0xFF;
        velocitymap[i] = 0xFF;
      }
    atexit(fm_exitproc);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void set_fm_volume(unsigned char volume)
  {
    quietness = 63 - (volume >> 2);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

char *patchmem;

void load_patches(char *f)
  {
    static char id[5] = {'O', 'P', 'L', '2', '\x1A'};

    static BYTE numpatches, i, curpatch;
    TIMBRE *curtimbre;
    char *t;

    //my shit
    unsigned int bytes,size;
    int h;


    //read file into t
    _dos_open(f,O_BINARY | O_RDONLY,&h);
    size=filelength(h);     patchmem=(char *)malloc(size+1000);
    _dos_read(h,patchmem,size,&bytes);
    _dos_close(h);
    
    
    for (i=0; i < 5; i++)
      if (patchmem[i] != id[i])
        {
          printf("ERROR:  Invalid timbres data file\n");
          exit(EXIT_FAILURE);
        }

    numpatches=patchmem[5];
    t=patchmem+6;
    

//    fread(&numpatches, 1, sizeof(numpatches), f);
    for (i=1; i <= numpatches; i++)
      {
//        curtimbre = (TIMBRE *)(malloc(sizeof(TIMBRE)));
//        fread(&curpatch, 1, sizeof(curpatch), f);
        curpatch=*t; t++;
        curtimbre=(TIMBRE *)t; t+=sizeof(TIMBRE);
//        fread(curtimbre, 1, sizeof(TIMBRE),   f);

        if (curpatch < 128)
          instrumentmap[curpatch] = curtimbre;
        else
        if (curpatch>(128+35))
          percussionmap[curpatch-128-35] = curtimbre;

        if (t+sizeof(TIMBRE)>(patchmem+size)) break;
      }
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

int allocate_channel(int logchannel)
  {
    int i, phychannel, best_lastused;

    phychannel = -1;

    for (i=0; (i < melodic_voices); i++)
        if (channelmap[i] == 0xFF)
          if ((phychannel == -1) || (ch_lastused[i] < best_lastused))
            best_lastused = ch_lastused[phychannel = i];

    if (phychannel != -1)
      {
        set_timbre(phychannel, instrumentmap[logchinfo[logchannel].patch]);
        channelmap[phychannel] = logchannel;
      }

    return(phychannel);

  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

int deallocate_channel(int logchannel, char note)
  {
    int i;
    int phychannel;

    phychannel = -1;

    for (i=0; (i < melodic_voices) && (phychannel == -1); i++)
      if ((channelmap[i] == logchannel) && (notemap[i] == note))
        phychannel = i;

    if (phychannel != -1)
      {
        channelmap[phychannel]  = 0xFF;
        notemap[phychannel]     = 0xFF;
        velocitymap[phychannel] = 0xFF;
      }

    ch_lastused[phychannel] = ++cur_note;

    return(phychannel);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void note_on(BYTE channel, BYTE note, BYTE velocity)
  {
//    int i;
    int phychannel;

    if (channel != percussion_channel)
      { /* Melodic */
        if (velocity == 0)
          {
            note_off(channel, note, 0);
            return;
          }
        if ((phychannel = allocate_channel(channel)) == -1)
          return; /* Can't allocate channel */

        notemap[phychannel]     = note;
        velocitymap[phychannel] = velocity;

        set_op_reg(0x40, phychannel, 1, phychinfo[phychannel].ksl | MIN(63, ((63 - phychinfo[phychannel].insvolume) + quietness)));
        set_ch_reg(0xA0, phychannel, lo(notes[note+phychinfo[phychannel].keymapping]));
        set_ch_reg(0xB0, phychannel, hi(notes[note+phychinfo[phychannel].keymapping]));
      }
    else
      { /* Rhythmic */
        switch (note)
          {
            case 36:  /* Base Drum */
              set_op_reg(0x40, 6, 1, phychinfo[6].ksl | MIN(63, ((63 - phychinfo[phychannel].insvolume) + quietness)));
              set_ch_reg(0xA0, 6, lo(notes[60+phychinfo[6].keymapping]));
              set_ch_reg(0xB0, 6, hi(notes[60+phychinfo[6].keymapping]) & 0xDF); /* Mask Kon bit */
              set_reg(0xBD, (percussion_control |= 0x10));
              break;
            case 38:  /* Acoustical snare */
              set_slot_reg(0x40, 16, phychinfo[7].ksl | MIN(63, ((63 - phychinfo[7].insvolume) + quietness)));
              set_reg(0xBD, (percussion_control |= 0x08));
              break;
            case 39:  /* Hand clap */
            case 42:  /* Hi Hats   */
              set_slot_reg(0x40, 13, phychinfo[10].ksl | MIN(63, ((63 - phychinfo[10].insvolume) + quietness)));
              set_reg(0xBD, (percussion_control |= 0x01));
              break;
          }
      }
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void note_off(BYTE channel, BYTE note, BYTE velocity)
  {
    int phychannel;

    velocity = velocity;  /* Prevent compiler from complaining */
    if (channel != percussion_channel)
      { /* Melodic */
       if ((phychannel = deallocate_channel(channel, note)) == -1)
         return;  /* Note not on */
       set_ch_reg(0xB0, phychannel, hi(notes[note+phychinfo[phychannel].keymapping]) & 0xDF);
      }
    else
      { /* Rhythmic */
        switch(note)
          {
            case 36:  /* Base drum */
              set_reg(0xBD, (percussion_control &= ~0x10));
              break;
            case 38:  /* Acoustical snare */
              set_reg(0xBD, (percussion_control &= ~0x08));
              break;
            case 39:  /* Hand clap (Hi hats)  */
              set_reg(0xBD, (percussion_control &= ~0x01));
              break;
          }
      }
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void patch_change(BYTE channel, BYTE newpatch)
  {
//    int i;

    if (channel != percussion_channel)
      {  /* Melodic */
        if (instrumentmap[newpatch] == NULL)
          return; /* Patch not loaded */
        logchinfo[channel].patch = newpatch;
      }
    else
      { /* Rhythmic */
        set_timbre(6, percussionmap[36-35]);                /* Base drum  */
        set_percussion_slot(7, 16, percussionmap[38-35]);   /* Snare drum */
        set_percussion_slot(10, 13, percussionmap[42-35]);  /* Hi hats    */

        set_reg(0xA7, (50*3) & 0xFF);
        set_reg(0xB7, (50*3) >> 8);

        set_reg(0xA8, (50) & 0xFF);
        set_reg(0xB8, (50) >> 8);
      }
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void done_fm(void)
  {
    int i;

    for (i=0; i < 9; i++)
      set_ch_reg(0xB0, i, 0x00);
    free(patchmem);  

  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */
