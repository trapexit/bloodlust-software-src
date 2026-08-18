/*
 *      PMWFIX.C
 *
 *      Source file for PMODE/W -fix.
 *
 *      HackRight 1995 Teemu Kalvas
 *
 */


#include <conio.h>
#include <dos.h>
#include <i86.h>
#include <stdlib.h>
#include <stdio.h>


extern char __begtext;

void (__interrupt __far *oldint[0xf])();
char *name[0xf] =
{
        "(divide error)",
        "(debug exception)",
        "(non-maskable interrupt)",
        "(breakpoint trap)",
        "(overflow trap)",
        "(bounds check fault)",
        "(invalid opcode #UD)",
        "(no coprocessor #NM)",
        "(double fault #DF)",
        "(coprocessor segment overrun)",
        "(invalid TSS #TS)",
        "(segment not present #NP)",
        "(stack fault #SS)",
        "(general protection #GP)",
        "(page fault #PF)"
};
char ec[0xf] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1 };


int user_main( int, char** );


void setmode3( void );
#pragma aux setmode3 = \
        "mov    eax,3" \
        "int    10h" \
        modify exact [eax];


int getcr2( void );
#pragma aux getcr2 = \
        "mov    eax,cr2" \
        value [eax];


/*      This function needs some stack.  Sorry for arranging things this
        way.  malloc() enthusiasts will rewrite some code.      */
#pragma aux exception aborts;
void exception( int num, union INTPACK __far *regs )
{
        int c, d, e, f;
        char buffer[5*80];
        char *ptr = buffer;
        short *scr = (short*)0x000b8000;

/*      Reset text mode.  God knows what state the display card is in when
        the exception strikes.  */
        setmode3();

        if( !ec[num] )
        {
                c = regs->x.cs;
                d = 0;
                e = regs->x.eip;
                f = regs->x.flags;
        }
        else
        {
                c = regs->x.flags;
                d = regs->x.eip;
                e = regs->x.cs;
                f = *(int*)&regs[1];
        }
/*      I'm proud of the next function call.  */
        sprintf( buffer,"exception %02x %-29s  cs:eip %04x:%08x      [iopl=%x]  " \
                                        "eflags %08x  [cf=%x pf=%x af=%x zf=%x sf=%x tf=%x " \
                                        "if=%x df=%x of=%x nt=%x rf=%x vm=%x]  " \
                                        "eax %08x  ecx %08x  edx %08x  ebx %08x  " \
                                        "                        " \
                                        "esp %08x  ebp %08x  esi %08x  edi %08x  " \
                                        "                        " \
                                        "ds %04x  es %04x  fs %04x  gs %04x      " \
                                        "unrelocated crash address %08x      " \
                                        "cr2 %08x  error code %04x",
                                        num, name[num], c, e, f>>12&3,
                                        f, f&1, f>>2&1, f>>4&1, f>>6&1, f>>7&1, f>>8&1,
                                        f>>9&1, f>>10&1, f>>11&1, f>>14&1, f>>16&1, f>>17&1,
                                        regs->x.eax, regs->x.ecx, regs->x.edx, regs->x.ebx,
                                        regs->x.esp, regs->x.ebp, regs->x.esi, regs->x.edi,
                                        regs->x.ds & 0xffff, regs->x.es & 0xffff,
                                        regs->x.fs & 0xffff, regs->x.gs & 0xffff,
                                        e - (int)&__begtext + 3, getcr2(), d );
/*      Dump the exception dump string to screen; clear the keyboard buffer;
        wait for a keypress; exit with an error code.  */
        while( *ptr )
                *scr++ = *ptr++ + 0x0d00;
        while( kbhit() )
                getch();
        getch();
        exit( 1 );
}


/*      This interrupt is not an IRQ.  */
void __interrupt __far int00( union INTPACK regs )
{
        exception( 0, &regs );
}


void __interrupt __far int01( union INTPACK regs )
{
        exception( 1, &regs );
}


void __interrupt __far int02( union INTPACK regs )
{
        exception( 2, &regs );
}


void __interrupt __far int03( union INTPACK regs )
{
        exception( 3, &regs );
}


void __interrupt __far int04( union INTPACK regs )
{
        exception( 4, &regs );
}


void __interrupt __far int05( union INTPACK regs )
{
        exception( 5, &regs );
}


void __interrupt __far int06( union INTPACK regs )
{
        exception( 6, &regs );
}


void __interrupt __far int07( union INTPACK regs )
{
        exception( 7, &regs );
}


/*      This interrupt might be an IRQ. The PIC knows, so we ask him.  */
void __interrupt __far int08( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x01 )
                (*oldint[8])();
        else
                exception( 8, &regs );
}


void __interrupt __far int09( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x02 )
                (*oldint[9])();
        else
                exception( 9, &regs );
}


void __interrupt __far int0a( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x04 )
                (*oldint[0xa])();
        else
                exception( 0xa, &regs );
}


void __interrupt __far int0b( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x08 )
                (*oldint[0xb])();
        else
                exception( 0xb, &regs );
}


void __interrupt __far int0c( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x10 )
                (*oldint[0xc])();
        else
                exception( 0xc, &regs );
}


void __interrupt __far int0d( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x20 )
                (*oldint[0xd])();
        else
                exception( 0xd, &regs );
}


void __interrupt __far int0e( union INTPACK regs )
{
        outp( 0x20, 0x0b );
        if( inp( 0x20 ) == 0x40 )
                (*oldint[0xe])();
        else
                exception( 0xe, &regs );
}


void (__interrupt __far *newint[0xf])(union INTPACK regs) =
{
        int00, int01, int02, int03,
        int04, int05, int06, int07,
        int08, int09, int0a, int0b,
        int0c, int0d, int0e
};


/*      I didn't want to start messing around with startup files.  Therefore
        the fix module contains main() which calls user_main().  */
void init_exceptions()
{
 for(int a = 0; a < 0xf; a++ )
 {
  oldint[a] = _dos_getvect( a );
  _dos_setvect( a, (void __interrupt (far *)()) newint[a] );
 }
}
