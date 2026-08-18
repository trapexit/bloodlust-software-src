/* ÛÛÛÛÛÛÛÛ Copyright 1994 by Ethan Brodsky.  All rights reserved. ÛÛÛÛÛÛÛÛ */

/* ÛÛ MIDILIB.C ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

/* ÛÛ Interface ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

typedef unsigned char BYTE;

#include "midilib.h"


volatile int  playing;

/* ÛÛÛ Implementation ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

extern "C"
{
#include "opl2fm.h"
}
#include "utils.c"

#include "ttimer.h"

/* Music information */

MUSIC         *curmusic;         // Pointer to current music record
BYTE          *curptr;           // Pointer to current MIDI data
BYTE          runningcmd;        // Current running command
int         musicspeed;        // Clocks per second
unsigned long clockcount;        // Total clock count since song start
unsigned int  deltatime;         // Delta time to next event
unsigned int  curdelta;          // Ticks since last event
unsigned int  channelmask;       // Bitmapped:  Channels to output
int         curmultiplier;     // Multiplier for music speed
unsigned char curvolume;         // Music volume
int           looping;           // Whether the music should loop at the end
//extern "C"
int busy;              // Insurance against reentrancy problems

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void midi_exitproc(void)
  {
    done_music();
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void midi_handler(void);

#include "ctaweapi.h"

int awe32;
extern int doawe32;

int init_music(int rhythm_mode)
  {
    playing = 0;
    curmultiplier = 1;
    channelmask = 0xFFFF;  // Unmask all channels
    busy = 0;


//printf("detecting awe32...");
if (!doawe32) //(awe32Detect(0x620))
  {
    awe32=0;
    cprintf("Using FM..."); 

   /* Initialize synth */
    init_fm(rhythm_mode);
    set_music_volume(255);    

   /* Load patches */
    load_patches("TIMBRES.VOL");
  }
    else
  {
    awe32=1;
    
    cprintf("initializing awe32...");
    if (awe32InitHardware()) {printf("failed.\n"); return(1);}
    cprintf("successful...");
        
    awe32SoundPad.SPad1 = awe32SPad1Obj;
    awe32SoundPad.SPad2 = awe32SPad2Obj;
    awe32SoundPad.SPad3 = awe32SPad3Obj;
    awe32SoundPad.SPad4 = awe32SPad4Obj;
    awe32SoundPad.SPad5 = awe32SPad5Obj;
    awe32SoundPad.SPad6 = awe32SPad6Obj;
    awe32SoundPad.SPad7 = awe32SPad7Obj;


//    printf("initializing midi...");
    if (awe32InitMIDI()) {printf("failed.\n"); return(1);}
//    printf("successful\n");
  } 

   /* Initialize timer */
    musichandler=midi_handler;

   /* Install exit handler */
   atexit(midi_exitproc);

    return(0);
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

void done_music(void)
  {
    playing = 0;
    musichandler=0;

    if (!awe32)  done_fm();
     else awe32Terminate();
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */



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
long tracksize;
extern     unsigned bytes;

void load_music(unsigned int h, MUSIC **music)
  {


    
    /* Load file header */
        // fread(&file_header, 1, sizeof(MIDI_HEADER), f);
        _dos_read(h,&file_header,sizeof(MIDI_HEADER),&bytes);
         if (get_motorola_word(&file_header.filetype) != 0)
                {
//                  ModeText();  
//                  printf("corrupt BG file\n");
//                  exit(EXIT_FAILURE);
                    *music=0;
                    return;    
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
   if (!*music) return;   
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

    return((sum>>2));
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

    SetMusicSpeed((unsigned int)(musicspeed*curmultiplier)>>2 );

  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */

static int runningmode;
static int command;
static int channel;
static int size;


void handle_event(void)
  {
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
//                _disable();                
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

void handle_eventAWE32(void)
  {
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
//         _disable();
  //       *((char *)(0xA0000+50))=1;
          if (get_channelmask(channel))
            awe32NoteOff(channel, (short int)curptr[1], (short int)curptr[2]);
//         _enable();
//         *((char *)(0xA0000+50))=0;
          size = 3;
          break;

        case 0x90:  /* Note on */
//         _disable();
//        *((char *)(0xA0000+50))=2;
          if (get_channelmask(channel))
            awe32NoteOn(channel, (short int) curptr[1], (short int)curptr[2]*curvolume/256);
//         _enable();
//         *((char *)(0xA0000+50))=0;
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
  //       _disable();
//         *((char *)(0xA0000+50))=3;
          if (get_channelmask(channel))
            awe32ProgramChange(channel, (short int)curptr[1]);
  //        *((char *)(0xA0000+50))=0;  
//         _enable();   
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
      //   _disable();
          switch(curptr[1])
            {
              case 0x51:  /* Set tempo */
                set_tempo(curptr);
//                _disable();                
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
    //     _enable(); 
          break;

        default:
          playing=0; //exit(EXIT_FAILURE);
          break;
      }
    curptr += size;
  }


void midi_handler(void)
  {
//    if (!busy && playing)
    if (playing)
      {
//      _disable();
//        *((char *)0xA0000)=255;
           
        clockcount++;
        curdelta++;
        if (curdelta >= deltatime)
          {
            do
              {
  //            _disable();
               
                if (!awe32) handle_event();
                  else handle_eventAWE32();
                handle_delta();
              }
            while (!deltatime);
          }
//        *((char *)0xA0000)=0x0;
//      _enable();

      }
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */

void start_music(MUSIC *music, int loop)
  {
    if (!music) return;  
    for (int i=0; i < 16; i++)  set_channelmask(i, 1);
    curmusic   = music;
    curptr     = (BYTE *)(music->dataptr);
    clockcount = 0;
    musicspeed = 0;
    looping    = loop;

    playing    = 1;
    deltatime  = 0;
    curdelta   = 0;
    handle_delta();

    SetMusicSpeed(100);
    
  }

/* ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ */
extern "C" {
void reset_synth();
void set_ch_reg(int reg, int channel, BYTE value);
//void reset_fm();
void stop_fm() ;
}

void stop_music(void)
  {
    playing    = 0;
    curmusic   = NULL;
    curptr     = NULL;

    if (!awe32)    stop_fm();
     else
      {
         //awe32InitMIDI();
        for (int i=0; i<16; i++)
         {
          awe32Controller(i,120,0);
         } 
      }   

     
     SetMusicSpeed(10);
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
  if (!awe32)  set_fm_volume(curvolume);
  }

/* ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ */
