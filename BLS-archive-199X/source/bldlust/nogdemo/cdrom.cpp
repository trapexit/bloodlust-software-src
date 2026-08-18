
/*                          CDROM AUDIO ROUTINES
                              By Barry Egerter

                            Written July 18, 1994

                            Using Borland C++ 3.1

                   Code : FREEWARE - alter and use at will.


                Internet Email:      barry.egerter@softnet.com
*/
#include <dos.h>
#include <io.h>
#include <mem.h>
#include <fcntl.h>
#include <stdio.h>

#define CDROM 0x21
#define EJECT_TRAY 0
#define RESET 2
#define CLOSE_TRAY 5
#define MEDIA_CHANGE 9
#define BUSY  512
#define TRACK_MASK 208

void cd_get_audio_info (void);

typedef struct playinfo {
  unsigned char control;
  unsigned char adr;
  unsigned char track;
  unsigned char index;
  unsigned char min;
  unsigned char sec;
  unsigned char frame;
  unsigned char zero;
  unsigned char amin;
  unsigned char asec;
  unsigned char aframe;
} playinfo;


typedef struct volumeinfo {
    unsigned char mode;
    unsigned char input0;
    unsigned char volume0;
    unsigned char input1;
    unsigned char volume1;
    unsigned char input2;
    unsigned char volume2;
    unsigned char input3;
    unsigned char volume3;
} volumeinfo;


struct {
  unsigned short drives;
  unsigned char  first_drive;
  unsigned short current_track;
  unsigned long  track_position;
  unsigned char  track_type;
  unsigned char  low_audio;
  unsigned char  high_audio;
  unsigned char  disk_length_min;
  unsigned char  disk_length_sec;
  unsigned char  disk_length_frames;
  unsigned long  endofdisk;
  unsigned char  upc[7];
  unsigned char  diskid[6];
  unsigned long  status;
  unsigned short error;      /* See description below */
} cdrom_data;

typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
    unsigned char media;
    unsigned long address;
    unsigned short bytes;
    unsigned short sector;
    unsigned long  volid;
    unsigned char unused[4];
  } tray_request;

typedef  struct {
    unsigned char mode;
    unsigned char adr_mode;
    unsigned long address;
  } head_data;


typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
    unsigned char media;
    unsigned long address;
    unsigned short bytes;
    unsigned char unused[4];
  } cd_request;

typedef   struct {
    unsigned char mode;
    unsigned char adr;
    unsigned char upc[7];
    unsigned char zero;
    unsigned char aframe;
  } upc_data;


typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
    unsigned char media;
    long address;
    short bytes;
    short sector;
    long volid;
  } ioctli;

typedef  struct {
    unsigned char mode;
    unsigned char lowest;
    unsigned char highest;
    unsigned long address;
  } tracks_data;

typedef  struct {
    unsigned char mode;
    unsigned char track;
    unsigned long address;
    unsigned char control;
  } track_data;


typedef   struct {
    unsigned char mode;
    unsigned long status;
  } cd_data;


static union REGS inregs, outregs;
//static struct SREGS sregs;


extern "C"{
 void initcdasm();
 void cdrequest();
 void cdsetdrive(int x);
 extern unsigned short cdrealmemseg;
}


unsigned char *cdmem,*cdmem2;
unsigned long cdmemptr,cdmemptr2;

unsigned char cddrives[32];

void device_request ()
{
  cdrequest();
}


short cdrom_installed (void)
{
  inregs.h.ah = 0x15;
  inregs.h.al = 0x00;
  inregs.x.ebx = 0;
  int386 (0x2f, &inregs, &outregs);
  if (outregs.x.ebx&0xFFFF == 0)    return (-1); //not installed

  //is installed, set drives
  cdrom_data.drives = outregs.x.ebx&0xFFFF;
  cdrom_data.first_drive = outregs.x.ecx&0xFFFF;

  //get cd memory
  initcdasm();
  if (!cdrealmemseg) return(-2); //not enough real memory
  //set up protected mode pointers to xfer areas
  cdmem=(unsigned char *)( ((int)cdrealmemseg)<<4);
  cdmem2=cdmem+256;
  //set up real mode far pointers to xfer areas
  cdmemptr=cdrealmemseg<<16;
  cdmemptr2=cdmemptr+256;

  for (int i=0; i<cdrom_data.drives; i++)
   cddrives[i]=cdmem[i]+1;

  //get audio info
  cdsetdrive(cdrom_data.first_drive);
  cd_get_audio_info();
  return (cdrom_data.drives);
}



void red_book (unsigned long value, unsigned char *min, unsigned char *sec, unsigned char *frame)
{
  *frame = value & 0x000000ff;
  *sec = (value & 0x0000ff00) >> 8;
  *min = (value & 0x00ff0000) >> 16;
}


unsigned long hsg (unsigned long value)
{
  unsigned char min, sec, frame;

  red_book (value, &min, &sec, &frame);
  value = (unsigned long)min * 4500;
  value += (short)sec * 75;
  value += frame - 150;
  return value;
}


unsigned long cd_head_position (void)
{
tray_request *tr=(tray_request *)cdmem;
head_data *hd=(head_data *)cdmem2;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 6;
  hd->mode = 0x01;
  hd->adr_mode = 0x00;
  device_request ();
  cdrom_data.error = tr->status;
  return hd->address;
}


void cd_get_volume (struct volumeinfo *vol)
{
tray_request *tr=(tray_request *)cdmem;
volumeinfo *v=(volumeinfo *)cdmem2;
*v=*vol;
    

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 9;
  v->mode = 0x04;
  device_request ();
  cdrom_data.error = tr->status;

  *vol=*v;  
}


void cd_set_volume (struct volumeinfo *vol)
{
cd_request *cdr=(cd_request *)cdmem;
volumeinfo *v=(volumeinfo *)cdmem2;
*v=*vol;

  v->mode = 3;
  cdr->length = sizeof (cd_request);
  cdr->subunit = 0;
  cdr->comcode = 12;
  cdr->media = 0;
  cdr->address = cdmemptr2;
  cdr->bytes = 9;
  device_request ();
  cdrom_data.error = cdr->status;
  
*vol=*v;  
}


short cd_getupc (void)
{
tray_request *tr=(tray_request *)cdmem;
upc_data *upc=(upc_data *)cdmem2;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 11;
  upc->mode = 0x0e;
  upc->adr = 2;
  device_request ();
  cdrom_data.error = tr->status;
  if (upc->adr == 0)
    memset (&upc->upc, 0, 7);
  memcpy (&cdrom_data.upc[0], &upc->upc[0], 7);
  return 1;
}


void cd_get_audio_info (void)
{
ioctli *io=(ioctli *)cdmem;
tracks_data *td=(tracks_data *)cdmem2;


  io->length = sizeof (ioctli);
  io->subunit = 0;
  io->comcode = 3;
  io->media = 0;
  io->sector = 0;
  io->volid = 0;
  io->address = cdmemptr2;
  io->bytes = sizeof (tracks_data);
  td->mode = 0x0a;
  device_request ();
  memcpy (&cdrom_data.diskid, &td->lowest, 6);
  cdrom_data.low_audio = td->lowest;
  cdrom_data.high_audio = td->highest;
  red_book (td->address, &cdrom_data.disk_length_min, &cdrom_data.disk_length_sec, &cdrom_data.disk_length_frames);
  cdrom_data.endofdisk = hsg (td->address);
  cdrom_data.error = io->status;
}


void cd_set_track (short tracknum)
{
tray_request *tr=(tray_request *)cdmem;    
track_data *td=(track_data *)cdmem2;
memset(tr,0,sizeof(tray_request));
memset(td,0,sizeof(track_data));

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = sizeof(track_data);
  td->mode = 0x0b;
  td->track = tracknum;
  device_request ();
  cdrom_data.error = tr->status;
  cdrom_data.track_position = hsg (td->address);
  cdrom_data.current_track = tracknum;
  cdrom_data.track_type = td->control & TRACK_MASK;
}


unsigned long get_track_length (short tracknum)
{
  unsigned long start, finish;
  unsigned short ct;

  ct = cdrom_data.current_track;
  cd_set_track (tracknum);
  start = cdrom_data.track_position;
  if (tracknum < cdrom_data.high_audio)
  {
    cd_set_track (tracknum+1);
    finish = cdrom_data.track_position;
  }
  else finish = cdrom_data.endofdisk;

  cd_set_track (ct);

  finish -= start;
  return finish;
}


void cd_track_length (short tracknum, unsigned char *min, unsigned char *sec, unsigned char *frame)
{
  unsigned long value;

  value = get_track_length (tracknum);
  value += 150;
  *frame = value % 75;
  value -= *frame;
  value /= 75;
  *sec = value % 60;
  value -= *sec;
  value /= 60;
  *min = value;
}


void cd_status (void)
{
tray_request *tr=(tray_request *)cdmem;    
cd_data *cdd=(cd_data *)cdmem2;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 5;

  cdd->mode = 0x06;
  device_request ();
  cdrom_data.status = cdd->status;
  cdrom_data.error = tr->status;
}


typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
    unsigned char addressmode;
    unsigned long transfer;
    unsigned short sectors;
    unsigned long seekpos;
  } seek_request;


void cd_seek (unsigned long location)
{
//  unsigned char min, sec, frame;
seek_request *sr=(seek_request *)cdmem;


  sr->length = sizeof (seek_request);
  sr->subunit = 0;
  sr->comcode = 131;
  sr->addressmode = 0;
  sr->transfer = 0;
  sr->sectors = 0;
  sr->seekpos = location;
  device_request ();
  cdrom_data.error = sr->status;
}


typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
    unsigned char addressmode;
    unsigned long start;
    unsigned long playlength;
  } play_request;


void cd_play_audio (unsigned long begin, unsigned long end)
{
//  unsigned long leng;
play_request *pr=(play_request *)cdmem;

  memset(pr,0,sizeof(play_request));
  pr->length = sizeof (play_request);
  pr->subunit = 0;
  pr->comcode = 132;
  pr->addressmode = 0;
  pr->start = begin;
  pr->playlength = end-begin;
  device_request ();
  cdrom_data.error = pr->status;
}


typedef  struct {
    unsigned char length;
    unsigned char subunit;
    unsigned char comcode;
    unsigned short status;
    char ununsed[8];
  } stop_request;


void cd_stop_audio (void)
{
stop_request *sr=(stop_request *)cdmem;
  memset(sr,0,sizeof(stop_request));

  sr->length = sizeof (stop_request);
  sr->subunit = 0;
  sr->comcode = 133;
  device_request ();
  cdrom_data.error = sr->status;
}


void cd_resume_audio (void)
{
stop_request *sr=(stop_request *)cdmem;

  sr->length = sizeof (stop_request);
  sr->subunit = 0;
  sr->comcode = 136;
  device_request ();
  cdrom_data.error = sr->status;
}


void cd_cmd (unsigned char mode)
{
tray_request *tr=(tray_request *)cdmem;    

 unsigned char *cd_mode=(unsigned char *)cdmem2;

  *cd_mode = mode;
  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 12;
  tr->media = 0;
  tr->address = cdmemptr2;
  tr->bytes = 1;
  device_request ();
  cdrom_data.error = tr->status;
}


void cd_getpos (struct playinfo *info)
{
tray_request *tr=(tray_request *)cdmem;    
playinfo *pi=(playinfo *)cdmem2;
*pi=*info;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 6;
  pi->control = 12;
  device_request ();
  cdrom_data.error = tr->status;
*info=*pi;  
}



short cd_done_play (void)
{
  cd_cmd (CLOSE_TRAY);
  return ((cdrom_data.error & BUSY) == 0);
}


typedef   struct {
    unsigned char mode;
    unsigned char media;
  } md_data;


short cd_mediach (void)
{
tray_request *tr=(tray_request *)cdmem;        
md_data *mdd=(md_data *)cdmem2;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 3;
  tr->media = 0;
  tr->media = tr->sector = tr->volid = 0;
  tr->address = cdmemptr2;
  tr->bytes = 2;

  mdd->mode = 0x09;
  device_request ();
  cdrom_data.error = tr->status;
  return mdd->media;
}


void cd_lock (unsigned char doormode)
{
tray_request *tr=(tray_request *)cdmem;        
md_data *mdd=(md_data *)cdmem2;

  tr->length = sizeof (tray_request);
  tr->subunit = 0;
  tr->comcode = 12;
  tr->media = 0;
  tr->address = cdmemptr2;
  tr->bytes = 2;
  mdd->mode = 1;
  mdd->media = doormode;
  device_request ();
  cdrom_data.error = tr->status;
}

