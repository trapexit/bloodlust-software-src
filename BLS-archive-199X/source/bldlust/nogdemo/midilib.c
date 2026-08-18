/* ÛÛÛÛÛÛÛÛ Copyright 1994 by Ethan Brodsky.  All rights reserved. ÛÛÛÛÛÛÛÛ */

/* ÛÛ MIDILIB.C ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

/* ÛÛ Interface ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

typedef unsigned char BYTE;

typedef struct
  {
    unsigned int datasize;
    BYTE (*dataptr)[64000];
    int quarter_ticks;
  } MUSIC;

void init_music(int rhythm_mode);
void done_music(void);

//void load_music(char *filename, MUSIC **music);
void free_music(MUSIC **music);

void start_music(MUSIC *music, int loop);
void stop_music(void);

void set_channelmask(int channel, int value);
int  get_channelmask(int channel);

void set_multiplier(float mult);
void set_music_volume(unsigned char new_volume);

int  playing;

/* ÛÛÛ Implementation ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

void ModeText();

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

//extern "C"

#include "opl2fm.h"

#include "utils.c"

#include "ttimer.h"

/* Music information */

MUSIC         *curmusic;         // Pointer to current music record
BYTE          *curptr;           // Pointer to current MIDI data
BYTE          runningcmd;        // Current running command
float         musicspeed;        // Clocks per second
unsigned long clockcount;        // Total clock count since song start
unsigned int  deltatime;         // Delta time to next event
unsigned int  curdelta;          // Ticks since last event
unsigned int  channelmask;       // Bitmapped:  Channels to output
float         curmultiplier;     // Multiplier for music speed
unsigned char curvolume;         // Music volume
int           looping;           // Whether the music should loop at the end
int           busy;              // Insurance against reentrancy problems

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void midi_exitproc(void)
  {
    done_music();
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void midi_handler(void);

void init_music(int rhythm_mode)
  {
    FILE *patchfile;

    playing = 0;
    curmultiplier = 1.0;
    channelmask = 0xFFFF;  // Unmask all channels
    busy = 0;

    set_music_volume(255);

   /* Initialize timer */
  //  init_timer();
    SetMusicFunc(midi_handler);
    //set_handler(midi_handler);

   /* Initialize synth */
    init_fm(rhythm_mode);
    set_music_volume(255);    

   /* Load patches */
    patchfile = fopen("TIMBRES.VOL", "rb");
    load_patches(patchfile);
    fclose(patchfile);

   /* Install exit handler */
    atexit(midi_exitproc);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void done_music(void)
  {
    playing = 0;
//    musichandler=0;
SetMusicFunc(0);
    done_fm();
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void load_music(int h, MUSIC **music)
  {
    typedef struct
      {
        char          id[4];         // "MThd"
        MOTOROLA_LONG length;
        MOTOROLA_WORD filetype;
        MOTOROLA_WORD numtracks;
        MOTOROLA_WORD ticksquarter;
      } MIDI_HEADER;

    typedef struct
      {
        char          id[4];         // "MTrk"
        MOTOROLA_LONG length;
      } TRACK_HEADER;

    MIDI_HEADER  file_header;
    TRACK_HEADER track_header;
  //  long filesize;
    long tracksize;
    unsigned bytes;
    
    /* Load file header */
        // fread(&file_header, 1, sizeof(MIDI_HEADER), f);
        _dos_read(h,&file_header,sizeof(MIDI_HEADER),&bytes);
         if (get_motorola_word(&file_header.filetype) != 0)
                {
                   ModeText();
                  printf("ERROR:  Only single track MID files are supported\n");
                  exit(EXIT_FAILURE);
                }

        /* Load track header */
        // fread(&track_header, 1, sizeof(TRACK_HEADER), f);
         _dos_read(h,&track_header,sizeof(TRACK_HEADER),&bytes);
         tracksize = get_motorola_long(&track_header.length);

                /* Allocate music record and data area */
         *music = (MUSIC *)(malloc(sizeof(MUSIC)));
         (*music)->dataptr  =(unsigned char (*)[64000]) malloc(tracksize);
         (*music)->datasize = tracksize;
         (*music)->quarter_ticks =
                get_motorola_word(&file_header.ticksquarter);


        /* Load data */
         //fread((*music)->dataptr, 1, (*music)->datasize, f);
         _dos_read(h,(*music)->dataptr,(*music)->datasize,&bytes);

        /* Close data */
         //fclose(f); 

  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void free_music(MUSIC **music)
  {
    free((*music)->dataptr);
    free(*music);
    *music = NULL;
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

long get_deltatime(void)
  {
    long sum;
    int  done;

    sum = 0;

    do
      {
        done = !(*curptr & 0x80);
        sum  = sum << 7;
        sum += (*curptr & 0x7F);
        curptr++;
      }
    while (!done);

    return(sum);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void handle_delta(void)
  {
    deltatime = get_deltatime();
    curdelta  = 0;
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void set_tempo(BYTE *curptr)
  {
    musicspeed  = curmusic->quarter_ticks * 1000000;
    musicspeed /= get_motorola_trip((MOTOROLA_TRIP *)(curptr+3));

    SetMusicSpeed((unsigned int)(musicspeed*curmultiplier) );

  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void handle_event(void)
  {
    int runningmode;
    int command;
    int channel;
    int size;

    runningmode = !(curptr[0] & 0x80);

    if (runningmode) curptr--;  /* You didn't see this */
    if (runningmode)
      command = runningcmd;
    else
      command = curptr[0];

    runningcmd = command;
    channel    = command & 0x000F;

    switch (command & 0x00F0)
      {
        case 0x80:  /* Note off */
          if (get_channelmask(channel))
            note_off(channel, curptr[1], curptr[2]);
          size = 3;
          break;

        case 0x90:  /* Note on */
          if (get_channelmask(channel))
            note_on(channel, curptr[1], curptr[2]);
          size = 3;
          break;

        case 0xA0:  /* Key after-touch */
          /* Ignored */
          size = 3;
          break;

        case 0xB0:  /* Control change */
          /* Ignored */
          size = 3;
          break;

        case 0xC0:  /* Patch change */
          if (get_channelmask(channel))
            patch_change(channel, curptr[1]);
          size = 2;
          break;

        case 0xD0:  /* Channel after-touch */
          /* Ignored */
          size = 2;
          break;

        case 0xE0:  /* Pitch wheel change */
          /* Ignored */
          size = 3;
          break;

        case 0xF0:  /* Meta-event */
          switch(curptr[1])
            {
              case 0x51:  /* Set tempo */
                set_tempo(curptr);
                break;

              case 0x2F:  /* Track end */
                if (looping)
                  {
                    curptr = (BYTE *)(curmusic->dataptr);
                    return; /* Start music over and return */
                  }
                else
                  playing = 0; /* Done */
            }
          size = curptr[2] + 3;
          break;

        default:
          exit(EXIT_FAILURE);
          break;
      }
    curptr += size;
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void midi_handler(void)
  {
    if (!busy && playing)
      {
        busy = 1;
        clockcount++;
        curdelta++;
        if (curdelta >= deltatime)
          {
            do
              {
                handle_event();
                handle_delta();
              }
            while (!deltatime);
          }
        busy = 0;
      }
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void start_music(MUSIC *music, int loop)
  {
    int i;       
    for (i=0; i < 16; i++)    set_channelmask(i, 1);
    curmusic   = music;
    curptr     = (BYTE *)(music->dataptr);
    clockcount = 0;
    musicspeed = 0;
    looping    = loop;
    playing    = 1;
    deltatime  = 0;
    curdelta   = 0;
    handle_delta();
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */
void reset_synth();
void set_ch_reg(int reg, int channel, BYTE value);
//void reset_fm();
void stop_fm() ;


void stop_music(void)
  {playing    = 0;
    curmusic   = NULL;
    curptr     = NULL;
    
     stop_fm();
     SetMusicSpeed(0);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void set_channelmask(int channel, int value)
  {
    if (value)
      channelmask |= (1 << channel);
    else
      channelmask &= ~(1 << channel);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

int get_channelmask(int channel)
  {
    return(channelmask & (1 << channel));
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void set_multiplier(float mult)
  {
    curmultiplier = mult;
if (musicspeed)
                 SetMusicSpeed((unsigned int)(musicspeed*curmultiplier));
  }

void set_music_volume(unsigned char new_volume)
  {
    curvolume = new_volume;
    set_fm_volume(curvolume);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */
