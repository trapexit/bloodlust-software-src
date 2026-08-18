#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


void main()
{


do
{

unsigned t=inp(0x201);
printf("%01d%01d%01d%01d:%01d%01d%01d%01d\r",
   (t&0x80) ? 1 : 0,(t&0x40) ? 1 : 0,(t&0x20) ? 1 : 0,(t&0x10) ? 1 : 0,(t&0x8) ? 1 : 0,(t&0x4) ? 1 : 0,(t&0x2) ? 1 : 0,(t&0x1) ? 1 : 0);

} while (!kbhit());    

printf("\n");

}    

