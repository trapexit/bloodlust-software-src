#include <conio.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "gfxmisc.h"

void gfxNewRotate(char *image, char *screen, int angle)
{
        register long rowU, rowV, dU, dV;
        unsigned long y, x, u, v, tx, ty;

        dU = sinTab[angle];
        dV = cosTab[angle];

        rowU = startingPointU[angle];
        rowV = startingPointV[angle];

        y=186;
        while (y--) {
                u = rowU;
                v = rowV;

                x = 320;
                while (x--) {
                        ty = (v >> 10);
                        tx = (u >> 10);

                        if (ty<=175 && tx<=319) {
                                *screen = *(image + yTab[ty] + tx);
                        }
                        screen++;

                        u+=dV;
                        v+=dU;
                }

                rowU -= dU;
                rowV += dV;
        }
}


