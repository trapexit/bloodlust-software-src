/* Run-length slice line drawing implementation for mode 0x13, the VGA's
320x200 256-color mode. Not optimized!
  Compile Borland C++ 4.02 in small model and link with L15-2.C
  Checked by Jim Mischel 11/30/94 */

#include "dd.h"

#define SCREEN_WIDTH PITCH


/* Draws a horizontal run of pixels, then advances the bitmap pointer to
   the first pixel of the next run. */
void DrawHorizontalRun(char **ScreenPtr, int XAdvance,
   int RunLength, int Color)
{
   int i;
   char *WorkingScreenPtr = *ScreenPtr;

   for (i=0; i<RunLength; i++)
   {
      *WorkingScreenPtr = Color;
      WorkingScreenPtr += XAdvance;
   }
   /* Advance to the next scan line */
   WorkingScreenPtr += SCREEN_WIDTH;
   *ScreenPtr = WorkingScreenPtr;
}
/* Draws a vertical run of pixels, then advances the bitmap pointer to
   the first pixel of the next run. */
void DrawVerticalRun(char **ScreenPtr, int XAdvance,
   int RunLength, int Color)
{
   int i;
   char *WorkingScreenPtr = *ScreenPtr;

   for (i=0; i<RunLength; i++)
   {
      *WorkingScreenPtr = Color;
      WorkingScreenPtr += SCREEN_WIDTH;
   }
   /* Advance to the next column */
   WorkingScreenPtr += XAdvance;
   *ScreenPtr = WorkingScreenPtr;
}



                       /* Draws a line between the specified endpoints in color Color. */
void line(char *dest,int x1, int y1, int x2, int y2, int Color)
{
   int Temp, AdjUp, AdjDown, ErrorTerm, XAdvance, XDelta, YDelta;
   int WholeStep, InitialPixelCount, FinalPixelCount, i, RunLength;
   char *ScreenPtr;

         /* Figure out how far we're going vertically */
   XDelta= x2 - x1;
   YDelta= y2 - y1;
//   if (YDelta<0) YDelta=-YDelta;

  if (x1<0)
   {
    if (x2<0) return;
     y1-=x1*YDelta/XDelta;
     x1=0;
   }
  if (x2<0)
   {
    if (x1<0) return;
    y2-=x2*YDelta/XDelta;
    x2=0;
   }
  if (x1>=SCREENX)
   {
    if (x2>=SCREENX) return;
    y1+=(SCREENX-x1)*YDelta/XDelta;
    x1=SCREENX-1;
   }
  if (x2>=SCREENX)
   {
    if (x1>=SCREENX) return;
    y2+=(SCREENX-x2)*YDelta/XDelta;
    x2=SCREENX-1;
   }


  if (y1<0)
   {
    if (y2<0) return;
    x1-=y1*XDelta/YDelta;
    y1=0;
   }
  if (y2<0)
   {
    if (y1<0) return;
    x2-=y2*XDelta/YDelta;
    y2=0;
   }
  if (y1>=SCREENY)
   {
    if (y2>=SCREENY) return;
    x1+=(SCREENY-y1)*XDelta/YDelta;
    y1=SCREENY-1;
   }
  if (y2>=SCREENY)
   {
    if (y1>=SCREENY) return;
    x2+=(SCREENY-y2)*XDelta/YDelta;
    y2=SCREENY-1;
   }

   /* We'll always draw top to bottom, to reduce the number of cases we have to
   handle, and to make lines between the same endpoints draw the same pixels */
   if (y1 > y2) {
      Temp = y1; y1 = y2; y2 = Temp;
      Temp = x1; x1 = x2; x2 = Temp;
   }
   if ((XDelta = x2 - x1) < 0)
   {
      XAdvance = -1;
      XDelta = -XDelta;
   }
   else
   {
      XAdvance = 1;
   }
   /* Figure out how far we're going vertically */
   YDelta = y2 - y1;



   /* Point to the bitmap address first pixel to draw */
   ScreenPtr =dest + y1 * SCREEN_WIDTH + x1;


   /* Special-case horizontal, vertical, and diagonal lines, for speed
      and to avoid nasty boundary conditions and division by 0 */
   if (XDelta == 0)
   {
      /* Vertical line */
      for (i=0; i<=YDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += SCREEN_WIDTH;
      }
      return;
   }
   if (YDelta == 0)
   {
      /* Horizontal line */
      for (i=0; i<=XDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += XAdvance;
      }
      return;
   }
   if (XDelta == YDelta)
   {
      /* Diagonal line */
      for (i=0; i<=XDelta; i++)
      {
         *ScreenPtr = Color;
         ScreenPtr += XAdvance + SCREEN_WIDTH;
      }
      return;
   }

   /* Determine whether the line is X or Y major, and handle accordingly */
   if (XDelta >= YDelta)
   {
      /* X major line */
      /* Minimum # of pixels in a run in this line */
      WholeStep = XDelta / YDelta;

      /* Error term adjust each time Y steps by 1; used to tell when one
         extra pixel should be drawn as part of a run, to account for
         fractional steps along the X axis per 1-pixel steps along Y */
      AdjUp = (XDelta % YDelta) * 2;

      /* Error term adjust when the error term turns over, used to factor
         out the X step made at that time */
      AdjDown = YDelta * 2;

      /* Initial error term; reflects an initial step of 0.5 along the Y
         axis */
      ErrorTerm = (XDelta % YDelta) - (YDelta * 2);

      /* The initial and last runs are partial, because Y advances only 0.5
         for these runs, rather than 1. Divide one full run, plus the
         initial pixel, between the initial and last runs */
      InitialPixelCount = (WholeStep / 2) + 1;
      FinalPixelCount = InitialPixelCount;

      /* If the basic run length is even and there's no fractional
         advance, we have one pixel that could go to either the initial
         or last partial run, which we'll arbitrarily allocate to the
         last run */
      if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
      {
         InitialPixelCount--;
      }
      /* If there're an odd number of pixels per run, we have 1 pixel that can't
         be allocated to either the initial or last partial run, so we'll add 0.5
         to error term so this pixel will be handled by the normal full-run loop */
         if ((WholeStep & 0x01) != 0)
      {
         ErrorTerm += YDelta;
      }
      /* Draw the first, partial run of pixels */
      DrawHorizontalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color);
      /* Draw all full runs */
      for (i=0; i<(YDelta-1); i++)
      {
         RunLength = WholeStep;  /* run is at least this long */
         /* Advance the error term and add an extra pixel if the error
            term so indicates */
         if ((ErrorTerm += AdjUp) > 0)
         {
            RunLength++;
            ErrorTerm -= AdjDown;   /* reset the error term */
         }
         /* Draw this scan line's run */
         DrawHorizontalRun(&ScreenPtr, XAdvance, RunLength, Color);
      }
      /* Draw the final run of pixels */
      DrawHorizontalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color);
      return;
   }
   else
   {
      /* Y major line */

      /* Minimum # of pixels in a run in this line */
      WholeStep = YDelta / XDelta;

      /* Error term adjust each time X steps by 1; used to tell when 1 extra
         pixel should be drawn as part of a run, to account for
         fractional steps along the Y axis per 1-pixel steps along X */
      AdjUp = (YDelta % XDelta) * 2;

      /* Error term adjust when the error term turns over, used to factor
         out the Y step made at that time */
      AdjDown = XDelta * 2;

      /* Initial error term; reflects initial step of 0.5 along the X axis */
      ErrorTerm = (YDelta % XDelta) - (XDelta * 2);

      /* The initial and last runs are partial, because X advances only 0.5
         for these runs, rather than 1. Divide one full run, plus the
         initial pixel, between the initial and last runs */
      InitialPixelCount = (WholeStep / 2) + 1;
      FinalPixelCount = InitialPixelCount;

      /* If the basic run length is even and there's no fractional advance, we
         have 1 pixel that could go to either the initial or last partial run,
         which we'll arbitrarily allocate to the last run */
      if ((AdjUp == 0) && ((WholeStep & 0x01) == 0))
      {
         InitialPixelCount--;
      }
      /* If there are an odd number of pixels per run, we have one pixel
         that can't be allocated to either the initial or last partial
         run, so we'll add 0.5 to the error term so this pixel will be
         handled by the normal full-run loop */
      if ((WholeStep & 0x01) != 0)
      {
         ErrorTerm += XDelta;
      }
      /* Draw the first, partial run of pixels */
      DrawVerticalRun(&ScreenPtr, XAdvance, InitialPixelCount, Color);

      /* Draw all full runs */
      for (i=0; i<(XDelta-1); i++)
      {
         RunLength = WholeStep;  /* run is at least this long */
         /* Advance the error term and add an extra pixel if the error
            term so indicates */
         if ((ErrorTerm += AdjUp) > 0)
         {
            RunLength++;
            ErrorTerm -= AdjDown;   /* reset the error term */
         }
         /* Draw this scan line's run */
         DrawVerticalRun(&ScreenPtr, XAdvance, RunLength, Color);
      }
      /* Draw the final run of pixels */
      DrawVerticalRun(&ScreenPtr, XAdvance, FinalPixelCount, Color);
      return;
   }
}

