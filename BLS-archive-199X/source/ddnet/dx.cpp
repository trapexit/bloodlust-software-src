#include <windows.h>
#include <ddraw.h>
#include <dsound.h>

#include <stdio.h>
#include "glib.h"

#include "net.h"

#include "keyb.h"

#include "input.h"
#include "config.h"

#include "dsound.h"
#include "misc.h"
#include "dd.h"


//functions that need to be supplied by the game
int  initgame();
void updatescreen();
void terminategame();
void gametimer();

int PITCH=0;
int SCREENX=640;
int SCREENY=480;

DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName);


//Global variables and shit
char appname[]="Disgruntled";
HINSTANCE hInst;

//direct draw objects
LPDIRECTDRAW DDO	=0;
LPDIRECTDRAWSURFACE ddprimary=0;  //DD primary surface
LPDIRECTDRAWSURFACE ddbackbuffer=0,ddoff=0;  //DD backbuffer surface
LPDIRECTDRAWPALETTE ddpalette=0; //DD palette

//direct sound objects
LPDIRECTSOUND DSOUND=0; //directsound object
PCMWAVEFORMAT pcmwf; //wave format of sounds
LPDIRECTSOUNDBUFFER dsprimary; //primary sound buffer

//window handle
HWND        hwnd ;
//window function
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
UINT timerid=0;

BOOL  ActiveApp; //Is this program active?
char *video;   // Pointer to video memory
char *screen; // Pointer to virtual screen

PALETTEENTRY palentries[256]; //palette entries for all colors

//config info
config *cfg;

//input devices
input inputdevice[2];


//mouse coords
int mx,my,mb;


//message box printf
void winprintf(char *format, ...)
{
char s[200];
char *args=(char *)&format+sizeof(format);
vsprintf(s,format,args);
MessageBox(hwnd,s,appname,MB_OK|MB_SETFOREGROUND);
}


char errstr[80];
void cleanexit(int x)
{
 DestroyWindow( hwnd);
}


void terminateddraw();
//directdraw initialization/termination funcs
//returns <0 on failure
int initddraw(HWND hwnd)
{
  HRESULT err;

  //create ddraw object
  if (DirectDrawCreate(NULL,&DDO,NULL)!=DD_OK) return -1;
  if ((err=DDO->SetCooperativeLevel( hwnd,
   DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX|DDSCL_ALLOWREBOOT))!=DD_OK) return err;

  //set video mode
  if ((err=DDO->SetDisplayMode( SCREENX,SCREENY,8))!=DD_OK) return err;

  //create surface(s)
  DDSURFACEDESC   ddsd;
  ddsd.dwSize = sizeof ( ddsd );
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  ddsd.dwBackBufferCount=1;
  if ((err=DDO->CreateSurface( &ddsd,&ddprimary, NULL ))!=DD_OK) return -3;

  //get pointer to backbuffer
  DDSCAPS ddscaps;
  ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
  if (ddprimary->GetAttachedSurface(&ddscaps,&ddbackbuffer)!=DD_OK) return -4;


  //create palette
  DDO->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256,palentries,&ddpalette,0);

  msg.printf(2,"DirectDraw initialized. %dx%dx%d",SCREENX,SCREENY,8);

  return 0;
}

void terminateddraw()
{
 if (!DDO) return;
 if (ddbackbuffer) ddbackbuffer->Release();
 if (ddprimary) ddprimary->Release();
 if (ddpalette) ddpalette->Release();

 DDO->RestoreDisplayMode();
 DDO->Release();
 DDO=0;
}

void setpalette(color *pal)
{
 //copy from color format to palleteentry format
 for (int i=0; i<256; i++)
  {
   palentries[i].peRed=pal[i].r<<2;
   palentries[i].peGreen=pal[i].g<<2;
   palentries[i].peBlue=pal[i].b<<2;
   palentries[i].peFlags=0;
  }
 ddpalette->SetEntries(0,0,256,palentries);
 ddprimary->SetPalette(ddpalette);

}





//initialize direct sound
int initdsound(HWND hwnd)
{
 HRESULT err;
 //create directsound object
 if ((err=DirectSoundCreate(NULL,&DSOUND,NULL))!=DS_OK) return err;

 DSOUND->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE);

 //get caps
 DSCAPS dscaps;
 dscaps.dwSize=sizeof(dscaps);
 if ((err=DSOUND->GetCaps(&dscaps))!=DS_OK) return err;

 // Set up primary sound wave format structure.
 memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
 pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
 pcmwf.wf.nChannels = 2;
 pcmwf.wf.nSamplesPerSec = 16000;
 pcmwf.wf.nBlockAlign = 4;
 pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
 pcmwf.wBitsPerSample =16;

 //create primary sound buffer
 DSBUFFERDESC dsbdesc;
 memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
 dsbdesc.dwSize = sizeof(DSBUFFERDESC);
 dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
 dsbdesc.dwBufferBytes = 0;
 dsbdesc.lpwfxFormat = NULL;
 err = DSOUND->CreateSoundBuffer(&dsbdesc, &dsprimary, NULL);
 if (err!=DS_OK) return -1;
 err=dsprimary->SetFormat((LPWAVEFORMATEX)&pcmwf); //set primary wave format
 //if (err!=DS_OK)  return -1;

 DWORD pcmsize;
 dsprimary->GetFormat((LPWAVEFORMATEX)&pcmwf,sizeof(pcmwf),&pcmsize);

 msg.printf(2,"DirectSound initialized. Output: %dhz %dbit %s",pcmwf.wf.nSamplesPerSec,pcmwf.wBitsPerSample,
    pcmwf.wf.nChannels==1 ? "Mono" : "Stereo");


  // Set up wave format structure for secondary sounds.
 memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
 pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
 pcmwf.wf.nChannels = 1;
 pcmwf.wf.nSamplesPerSec = 16000;
 pcmwf.wf.nBlockAlign = 1;
 pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
 pcmwf.wBitsPerSample =8;

 return 0;
}

void terminatedsound()
{
 if (!DSOUND) return;
 dsprimary->Release();
 DSOUND->Release();
}



 //creates a directsound buffer from existing old SOUND format
LPDIRECTSOUNDBUFFER createdsoundbuffer(struct SOUND *s)
{
  HRESULT err;

  // Set up DSBUFFERDESC structure.
  DSBUFFERDESC dsbdesc;
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
  dsbdesc.dwSize = sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN;
  dsbdesc.dwBufferBytes = s->soundsize;
  dsbdesc.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf;

  // Create buffer.
  LPDIRECTSOUNDBUFFER dsb;
  err = DSOUND->CreateSoundBuffer(&dsbdesc, &dsb, NULL);
  if (err!=DS_OK)  return 0;

  //lock memory
  LPVOID b,b2;
  DWORD size,size2;
  err=dsb->Lock(0,s->soundsize,&b,&size,&b2,&size2,0);
  for (int i=0; i<size; i++) ((char *)b)[i]=s->soundptr[i]+0x80;
  dsb->Unlock(b,size,b2,size2);
  return dsb;
}






//TIMER STUFF
volatile int timerbusy=0,timerdisabled=0;

void _disable() {timerdisabled=1;}
void _enable() {timerdisabled=0;}

void CALLBACK mmtimer(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1,DWORD dw2)
{
 if (timerbusy || timerdisabled || !ActiveApp) return;
 timerbusy=1;
 gametimer();
 timerbusy=0;
}


//----------------------------------------------------
//main initialization function
//initializes config, inputs, ddraw, dsound, timer
//----------------------------------------------------
void cleanup();

int initialize()
{
 //get configuration
 cfg=load_config("a32.cfg");
 if  (!cfg)
  {
   cleanup();
   winprintf("Cannot open configuration file.");
   return -1;
  }

 //initialize input devices
 ids=&cfg->ids; //copy over input settings
 inputdevice[0].init(cfg->pinput[0]);
 inputdevice[1].init(cfg->pinput[1]);
 refreshinputmain();

 playMIDIFile(hwnd,"grunt.mid");

 //initialize directdraw
 int err;
 if ((err=initddraw(hwnd))!=0)
    {
     cleanup();
     winprintf("Failure to initialize DirectDraw %X Do you know fucking why? I'll tell you fucking why, because no one at Microsoft can program worth shit and they all should be thrown into a pile and burned skinless like the niggers they are.",err);
     return -1;
    }

 //initialize directsound
 if ((err=initdsound(hwnd))!=0)
    {
     cleanup();
     winprintf("Failure to initialize DirectSound %X",err);
     return -1;
    }

 //initialize timer
 timerid=(UINT)timeSetEvent(10,5,mmtimer,0,TIME_PERIODIC);
 if (!timerid)
     {
      cleanup();
      winprintf("Unable to initialize multimedia timer");
      return -1;
     }

 //Success!
 return 0;
}

void cleanup()
{
 terminategame();
 terminatedsound();
 terminateddraw();
 if (timerid) {timeKillEvent(timerid); timerid=0;}
 free(cfg);
 ShowCursor(TRUE);
}







extern int blah;
extern int fps; void fuck();
//Windows main func
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
  {
   hInst=hInstance;

  //int *b=(int *)loadresource("#1");
//  winprintf("%d %d",b[0],b[1]);

   /*
      //void *t=loadresource("object");
   HRSRC a=FindResource(hInst,"#1",RT_RCDATA);
   HGLOBAL t=LoadResource(hInst,a);
   if (!t)
    {
     winprintf("error %d",GetLastError());
     return -1;
    }
    winprintf("no error %X",t);
  int *b=(int *)t;
  winprintf("%d %d",b[0],b[1]);


    return 0;
*/
   //register window class
   WNDCLASSEX  wndclass ;
   wndclass.cbSize        = sizeof (wndclass) ;
   wndclass.style         = 0;
   wndclass.lpfnWndProc   = WndProc ;
   wndclass.cbClsExtra    = 0 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInstance ;
   wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = NULL; //(HBRUSH) GetStockObject (WHITE_BRUSH) ;
   wndclass.lpszMenuName  = NULL;
   wndclass.lpszClassName = appname;
   wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;
   RegisterClassEx (&wndclass) ;

   //create window for app
   hwnd = CreateWindowEx(
        0, //WS_EX_TOPMOST,
        appname, appname,
        WS_POPUP, 0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL );

   if (!hwnd) return (FALSE);

   UpdateWindow (hwnd) ;
   SetFocus(hwnd);
   ShowCursor( FALSE );

    //initialze system shit
   if (initialize()!=0) return FALSE;

    //initialize game
   if (initgame()!=0)
    {
     cleanup();
     return -1;
    };

   //Main loop
   MSG  msg;
   while ( 1 )
    {
     if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
         if( !GetMessage( &msg, NULL, 0, 0 ) )  break;
         TranslateMessage(&msg);
         DispatchMessage(&msg);
        }
     else
     if( ActiveApp )
        {
          //clear backbuffer
         DDBLTFX bltfx;
         bltfx.dwSize=sizeof(bltfx);
         bltfx.dwFillColor=1;
         ddbackbuffer->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT,&bltfx);

         //draw next frame
         DDSURFACEDESC  ddsd;
         memset(&ddsd,0,sizeof(ddsd));
         ddsd.dwSize = sizeof( ddsd );
         //Lock it
         HRESULT lockerr=ddbackbuffer->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
         if (lockerr==DD_OK)
         {
          screen=video=(char *)ddsd.lpSurface; //get pointer to video memory
          PITCH=ddsd.lPitch; //set pitch

          //draw next frame
          updatescreen();
          // Unlock the video memory.
          ddbackbuffer->Unlock( NULL );

          //Flip the surfaces
          ddprimary->Flip(NULL,DDFLIP_WAIT);
          refreshinputmain();

 //         memcpy(video,screen,640*480);
         } else
         if (lockerr==DDERR_SURFACELOST) ddprimary->Restore();
          else break;
        }
    }


 //cleanup everything
 cleanup();
 return msg.wParam;
}

void quitgame()
{
 DestroyWindow( hwnd );
 ActiveApp=0;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     HDC         hdc ;

     switch (iMsg)
          {
      case WM_ACTIVATEAPP:
            ActiveApp = wParam;
            break;

      case WM_KEYDOWN:
//         if (lParam&(1<<30)) break; //it was previously down
         wm_keydown((lParam>>16)&0xFF);
        break;

      case WM_KEYUP:
         wm_keyup((lParam>>16)&0xFF);
         break;

      case WM_MOUSEMOVE:
           //store mouse coords
           mx=LOWORD(lParam)*SCREENX/640;
           my=HIWORD(lParam)*SCREENY/480;
           mb=0;
           if (wParam&MK_LBUTTON) mb|=1;
           if (wParam&MK_RBUTTON) mb|=2;
     		return 0;

      case WM_TCPSOCKET:
        if (!WSAGETSELECTERROR(lParam))
             wm_tcpsocket(wParam,WSAGETSELECTEVENT(lParam));
        else wm_socketerror(wParam,WSAGETSELECTEVENT(lParam),WSAGETSELECTERROR(lParam));
       break;

      case WM_UDPSOCKET:
        if (!WSAGETSELECTERROR(lParam))
             wm_udpsocket(wParam,WSAGETSELECTEVENT(lParam));
        else wm_socketerror(wParam,WSAGETSELECTEVENT(lParam),WSAGETSELECTERROR(lParam));
       break;

      case WM_SERVERSOCKET:
        if (!WSAGETSELECTERROR(lParam))
             wm_serversocket(wParam,WSAGETSELECTEVENT(lParam));
        else wm_socketerror(wParam,WSAGETSELECTEVENT(lParam),WSAGETSELECTERROR(lParam));
       break;

      case WM_CREATE :
      break;
      case WM_DESTROY :
       PostQuitMessage (0) ;
       if (timerid) {timeKillEvent(timerid); timerid=0;}
       ::hwnd=0;
      break;

      }


   return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}


//loads a resource from the module
void *loadresource(char *name)
{

HRSRC r=FindResource(hInst,name,RT_RCDATA);
if (!r)  return 0;
HGLOBAL h=LoadResource(hInst,r);
if (!h)  return 0; //failure
//return (void *)h;

//allocate memory to copy it
int size=SizeofResource(hInst,r);       //get size
char *t=(char *)malloc(size); //get memory
if (!t) return 0;
memcpy(t,h,size); //copy it
return t;
}








DWORD playMIDIFile(HWND hWndNotify, LPSTR lpszMIDIFileName)
{
    UINT wDeviceID;
    DWORD dwReturn;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;
    MCI_STATUS_PARMS mciStatusParms;
    MCI_SEQ_SET_PARMS mciSeqSetParms;

    // Open the device by specifying the device and filename.
    // MCI will attempt to choose the MIDI mapper as the output port.
    mciOpenParms.lpstrDeviceType = "sequencer";
    mciOpenParms.lpstrElementName = lpszMIDIFileName;
    if ((dwReturn = mciSendCommand(NULL, MCI_OPEN,
        MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
        (DWORD)(LPVOID) &mciOpenParms))!=0)
    {
        // Failed to open device. Don't close it; just return error.
        return (dwReturn);
    }

    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Check if the output port is the MIDI mapper.
    mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
    if
    (
    (dwReturn = mciSendCommand(wDeviceID, MCI_STATUS,MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatusParms))!=0
    )
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

    // Begin playback. The window procedure function for the parent
    // window will be notified with an MM_MCINOTIFY message when
    // playback is complete. At this time, the window procedure closes
    // the device.
    mciPlayParms.dwCallback = (DWORD) hWndNotify;
    if ((dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
        (DWORD)(LPVOID) &mciPlayParms))!=0)
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }

    return (0L);
}









