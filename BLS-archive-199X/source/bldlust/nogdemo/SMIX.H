/*      SMIXW is Copyright 1995 by Ethan Brodsky.  All rights reserved      */

/* лл SMIX.H лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

#define TRUE  1
#define FALSE 0

#define ON  1
#define OFF 0

typedef struct
  {
    signed   char *soundptr;
    unsigned long soundsize;
  } SOUND;

int  init_sb(int baseio, int irq, int dma, int dma16);
  /* Initializes control parameters, resets DSP and installs int. handler   */
  /*  Parameters:                                                           */
  /*   baseio    Sound card base IO address                                 */
  /*   irq       Sound card IRQ setting                                     */
  /*   dma       Sound card 8-bit DMA channel                               */
  /*   dma16     Sound card 16-bit DMA channel                              */
  /*  Returns:                                                              */
  /*   TRUE      Sound card successfully initialized                        */
  /*   FALSE     Sound card could not be initialized                        */

void shutdown_sb(void);
  /* Removes interrupt handler and resets DSP                               */


void init_mixing(void);
  /* Allocates internal buffers and starts digitized sound output           */

void shutdown_mixing(void);
  /* Deallocates internal buffers and stops digitized sound output          */


void open_sound_resource_file(char *filename);
  /* Opens a resource file for loading sounds.  After this has been called, */
  /* the Key parameter in the LoadSound function is used as a resource key  */
  /* to locate the sound data in this file.                                 */
  /*  Parameters:                                                           */
  /*   FileName: File name of resource file                                 */

void close_sound_resource_file(void);
  /* Close sound resource file.  If you have called this, the Key parameter */
  /* to the LoadSound function will act as a filename instead of a resource */
  /* key.                                                                   */


void load_sound(SOUND **sound, char *key);
  /* Allocates an memory buffer and loads a sound from a file               */
  /*  Parameters:                                                           */
  /*   sound     Pointer to pointer to unallocated sound data structure     */
  /*   key       If a resource file has been opened then key is a resource  */
  /*             identifier.  Use the same ID as you used when adding the   */
  /*             sound resource with SNDLIB.  If a resource file is not     */
  /*             opened, then key is the filename from which to load the    */
  /*             sound data.                                                */

void free_sound(SOUND **sound);
  /* Frees sound data structure and extended memory block                   */
  /*  Parameters:                                                           */
  /*   sound     Pointer to pointer to allocated sound data structure       */


void start_sound(SOUND *sound, int index, unsigned char volume, int loop);
  /* Starts playing a sound                                                 */
  /*  Parameters:                                                           */
  /*   sound     Pointer to sound data structure                            */
  /*   index     A number to keep track of the sound with (Used to stop it) */
  /*   loop      Indicates whether sound should be continuously looped      */

void stop_sound(int index);
  /* Stops playing a sound                                                  */
  /*  Parameters:                                                           */
  /*   index     Index of sound to stop (All with given index are stopped)  */

int  sound_playing(int index);
  /* Checks if a sound is still playing                                     */
  /*  Parameters:                                                           */
  /*   index     Index used when the sound was started                      */
  /*  Returns:                                                              */
  /*   TRUE      At least one sound with the specified index is playing     */
  /*   FALSE     No sounds with the specified index are playing             */

void set_sound_volume(unsigned char new_volume);
  /* Sets overall sound volume                                              */
  /*  Parameters:                                                           */
  /*   new_volume  New overall sound volume (0-255)                         */

extern volatile long intcount;         /* Current count of sound interrupts */
extern volatile int  voicecount;       /* Number of voices currently in use */

extern float dspversion;
extern int   autoinit;
extern int   sixteenbit;

/* лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл */

