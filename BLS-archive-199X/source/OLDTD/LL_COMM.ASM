;; ************************************************************************
;; LL_COMM (R) Asynchronous Communication Routines
;; Copyright (C) 1994 by James P. Ketrenos
;;
;; VERSION      :       1.0
;; DATE         :       7-4-94
;; CODED BY :   James P. Ketrenos [ketrenoj@cs.pdx.edu]
;;
;; Special thanks to all of the contributors to the Serial.FAQ
;;
;; NOTE:    These are single port routines.  16550+ have not been activated.
;; ************************************************************************
.386

;; **  D A T A  ***********************************************************
DGROUP  group   _DATA
_DATA   segment 'DATA'
;; ***********************************************************

;; ***********************************************************
;; Typical EQUates for easier to read code
;; ***********************************************************
ON                      equ 1                                   ;; Boolean EQUates for easier to
OFF             equ 0                                   ;; : follow code
TRUE            equ 1                                   ;; :
FALSE           equ 0                                   ;; :
PIC             equ 020h                                ;; 8259 Programmable Int. Controller Port
PICM            equ     021h                            ;; 8259 PIC Mask Port
EOI                     equ     020h                            ;; 8259 End-Of-Interrupt

comfunc dd ?

;; ***********************************************************
;; ISR Service Jump Table
;; ***********************************************************
ISR_Service     dd      offset  ISR_Exit                ;; xxxxx00xb
                                dd      offset  ISR_Transmit    ;; xxxxx01xb
                                dd      offset  ISR_Receive     ;; xxxxx10xb
                                dd      offset  ISR_Exit                ;; xxxxx11xb

;; ***********************************************************
;; Register Indexes for the 8250 UART
;; NOTE:        Indexes have been multiplied by 2 to get the
;;                      correct location in the register table.
;; ***********************************************************
THR                             equ     0                               ;; Transmit Holding Register    (DLAB==0)
RBR                             equ     0                               ;; Recieve Buffer Register              (DLAB==0)
BAUDL                   equ     0                               ;; Baud Rate Divisor, Low byte  (DLAB==1)
BAUDH                   equ     2                               ;; Baud Rate Divisor, High Byte (DLAB==1)
IER                             equ     2                               ;; Interrupt Enable Register
IIR                             equ     4                               ;; Interrupt Identification Register
FCR                     equ 4                           ;; FIFO Control Register (UARTS 16550+)
LCR                             equ     6                               ;; Line Control Register
MCR                             equ     8                               ;; Modem Control Register
LSR                             equ     10                              ;; Line Status Register
MSR                             equ     12                              ;; Modem Status Register

;; ***********************************************************
;; COMM Routine Variables
;; ***********************************************************

        ;; ***********************************************************
        ;; Multi-Port Asynchronous Variables (later version)
        ;; ***********************************************************
        PortID          STRUCT
                Base    WORD    0                               ;; Port Base
                IRQ     BYTE    0                               ;; Port IRQ
                Flags   BYTE    00000000b               ;; Port Bit-Flags
        PortID          ENDS                                    ;; :
                UART    equ     00000011b               ;; UART Flags
                MODE    equ     00001100b               ;; Port Mode

        PortTable       PortID  <0,0,0>                 ;; Single Port Routines
        PortCode        dd              0b                              ;; Port Used Bit-Field (1-used)

        ;; ***********************************************************
    ;; Variables set by ioOpenPort()
        ;; ***********************************************************
        PortInUse               dw      OFF                             ;; Initialized COMM port flag
        Port                    dw      ?                               ;; Port Base Address
        Registers               dw      7 dup (?)               ;; Register Index Map
        Vector                  db      ?                               ;; Vector Number
        EnableIRQ               db      ?                               ;; Mask to enable 8259 IRQ
        DisableIRQ              db      ?                               ;; Mask to disable 8259 IRQ
        ISR_OldS                dw      ?                               ;; Old Vector Segment
        ISR_OldO                dd      ?                               ;; Old Vector Offset

;; ***********************************************************
;; Send and Receive Buffers
;; NOTE:        The buffer sizes (SendSize & RecvSize) must be of a
;;                      size X such that log2(X+1)=Integer.  In otherwords,
;;                      take a power of 2, subtract one, and that is a legal
;;                      buffer size. (1, 3, 7, 15, 31, 63, 127 ... 2^X-1)
;; ***********************************************************
;SendSize                equ 0000111111111111b   ;; 2^X-1 Send Buffer Size (4095)
SendSize                equ 0000111111111111b   ;; 2^X-1 Send Buffer Size (4095)

SendBuffer              db      SendSize DUP (?)        ;; Send FIFO Buffer
SendHead                dw      0                                       ;; Send FIFO Buffer Head Index
SendTail                dw      0                                       ;; Send FIFO Buffer Tail Index

;RecvSize                equ 0000111111111111b   ;; 2^X-1 Receive Buffer Size (4095)
RecvSize                equ 0000111111111111b   ;; 2^X-1 Receive Buffer Size (4095)
RecvBuffer              db      RecvSize DUP (?)        ;; Receive FIFO Buffer
RecvHead                dw      0                                       ;; Receive FIFO Buffer Head Index
RecvTail                dw      0                                       ;; Receive FIFO Buffer Tail Index



_DATA   ends
;; ************************************************************************

;; **  C O D E  3 2  ******************************************************
_TEXT   SEGMENT BYTE PUBLIC USE32 'CODE'
        ASSUME  CS:_TEXT
        ASSUME  DS:DGROUP

;; ***********************************************************
;; Routines callable from Watcom C/C++ 32
;; ***********************************************************
PUBLIC  ioOpenPort_, ioClosePort_                               ;; Set/Reset Serial I/O
PUBLIC  ioClearWrite_, ioClearRead_                     ;; Reset Serial Buffers
PUBLIC  ioReadByte_, ioWriteByte_                               ;; I/O with Serial Port
PUBLIC  ioReadStatus_, ioWriteStatus_                   ;; Check I/O Status
PUBLIC  ioGetBaud_, ioSetBaud_                                  ;; Get/Set Baud Rate
PUBLIC  ioSetHandShake_, ioGetHandShake_                ;; Get/Set Hand Shaking
PUBLIC  ioGetStatus_                                                    ;; Get Modem/Line Status
PUBLIC  ioGetControl_, ioSetControl_                    ;; Get/Set Line Control
PUBLIC  ioGetMode_, ioSetMode_                                  ;; Get/Set ISR Mode
PUBLIC ioSetComFunc_

ioGetMode_      PROC
        ret
ioGetMode_      ENDP

ioSetMode_      PROC
        ret
ioSetMode_      ENDP

;; ************************************************************************
;; Interrupt Service Routine for Serial I/O (in protected mode)
;; ************************************************************************
ISR_PM  PROC 
;USES EAX EBX ECX EDX DS
        pushad
        push es        
        push ds
        mov             ax, DGROUP
        mov             ds, ax
        mov             es, ax

        ;; ***********************************************************
        ;; Determine cause of interrupt, and act accordingly
        ;; ***********************************************************
        mov             dx,Registers[IIR]       ;; Interrupt Identification Register
        in              al,dx                           ;; :
ISR:and         eax,00000110b           ;; Use input to index a jump table
        jmp     ISR_Service[eax*2]      ;; :

;; ***********************************************************
;; IRQ Receive Data Request
;; ***********************************************************
ISR_Receive:
        ;; ***********************************************************
        ;; Get the data from the port
        ;; ***********************************************************
        mov             dx,Registers[RBR]       ;; Receive Buffer Register
        in              al,dx                           ;; : Get byte from port

        ;; ***********************************************************
        ;; Store the data in the Receive Buffer
        ;; ***********************************************************
        movzx   ebx,RecvHead            ;; Store Byte in buffer
        mov             RecvBuffer[ebx],al      ;; :
        inc             ebx                                     ;; Increment to next slot
        and     bx,RecvSize                     ;; : Perform Buffer-Wrap

        cmp             bx,RecvTail                     ;; See if buffer is Overflowing
        jne             @F                                      ;; : and if not, then continue
        mov             bx,RecvHead                     ;; : otherwise, hold position
@@:     mov             RecvHead,bx                     ;; Store new Receive Head Index

        ;; ***********************************************************
        ;; Check to see if another interrupt is pending . . .
        ;; ***********************************************************
        mov             dx,Registers[IIR]       ;; Interrupt Identification Register
        in              al,dx                           ;; :
        test    al,00000001b            ;; Check if Bit-0 is CLEAR
        jz              ISR                             ;; : If so, then another int. is pending.

        mov     al,EOI                          ;; Send End-Of-Interrupt to PIC
        out             PIC,al                          ;; :
        
;        pushfd
        call  [comfunc]
        pop ds
        pop es        
        popad
        iretd                                           ;; Return to where CS:EIP belongs

;; ***********************************************************
;; IRQ Transmit Data Request
;; ***********************************************************
ISR_Transmit:
        ;; ***********************************************************
        ;; Check if buffer is EMPTY
        ;; ***********************************************************
        movzx   ebx,SendTail            ;; If buffer is not empty,
        cmp             bx,SendHead                     ;; : then remove the data from the
        jne             @F                                      ;; : send queue and send another byte

        ;; ***********************************************************
        ;; Disable the THRE Interrupt
        ;; ***********************************************************
        mov             dx,Registers[IER]       ;; Interrupt Enable Register
        in              al,dx                           ;; : Disable:
        and     al,11111101b            ;; : Transmit Hold Register Emtpy Interrupt
        out             dx,al                           ;; :

        mov             al,EOI                          ;; Send End-Of-Interrupt to PIC
        out             PIC,al                          ;; :
        pop ds
        pop es        
        popad
        iretd                                           ;; Return to where CS:EIP belongs

        ;; ***********************************************************
        ;; Send the next byte from buffer
        ;; ***********************************************************
@@:     mov             al,SendBuffer[ebx]      ;; Fetch current byte from buffer
        inc     ebx                             ;; : Increment to next byte in buffer
        and     ebx,SendSize            ;; : Perform Buffer-Wrap
        mov     SendTail,bx                     ;; Store new Send Tail Index

        mov     dx,Registers[THR]       ;; Transmit Holding Register
        out             dx,al                           ;; : Send byte to port

        ;; ***********************************************************
        ;; Check to see if another interrupt is pending . . .
        ;; ***********************************************************
        mov             dx,Registers[IIR]       ;; Interrupt Identification Register
        in              al,dx                           ;; :
        test    al,00000001b            ;; Check if Bit-0 is CLEAR
        jz              ISR                             ;; : If so, then another int. is pending.

        mov     al,EOI                          ;; Send End-Of-Interrupt to PIC
        out             PIC,al                          ;; :
        pop ds
        pop es        
        popad 
        iretd                                           ;; Return to where CS:EIP belongs

        ;; ***********************************************************
        ;; Exit the ISR
        ;; ***********************************************************
ISR_Exit:
        ;; ***********************************************************
        ;; Turn interrupts OFF and ON to make interrupt edge
        ;; NOTE:  I'm not sure if this is necessary.
        ;; ***********************************************************
        mov     dx,Registers[IER]       ;; Interrupt Enable Register
        in              al,dx                           ;; Fetch current state
        xor             ah,ah                           ;; Disable ALL interrupts
        xchg    ah,al                           ;; : while saving current state
        out             dx,al                           ;; :
        xchg    ah,al                           ;; Renable prior state of interrupts
        out             dx,al                           ;; :

        mov             al,EOI                          ;; Send End-Of-Interrupt to PIC
        out             PIC,al                          ;; :
        pop ds
        pop es        
        popad
        iretd                                           ;; Return to where CS:EIP belongs
ISR_PM  ENDP



ioSetComFunc_ PROC
    mov [comfunc],eax
    ret
ioSetComFunc_ ENDP    


;;      ************************************************************************
;;      int             ioOpenPort(int BASE, int IRQ);
;;      PURPOSE:        Initializes COMM port and sets up the ISR for it.
;;      PASS:           BASE = EDX, IRQ = ECX
;;              Defaults are:
;;              COM1 = 03F8h 4  (INT 0Ch)
;;              COM2 = 02F8h 3  (INT 0Bh)
;;              COM3 = 03E8h 4  (INT 0Ch)
;;              COM4 = 02E8h 3  (INT 0Bh)
;;      RETURNS:        Returns a Port ID Structure Address for use when these
;;                              routines are multi-port based.
;;                              0 Failure | > 0 Success
;;      ************************************************************************
ioOpenPort_ PROC USES EBX ES
        ;; ***********************************************************
        ;; Check to see if COM Routines installed
        ;; ***********************************************************
        cmp     PortInUse,ON            ;; Check to PortInUse flag is set to ON
        jnz     @F                                      ;; : and if not, then install
        xor             eax,eax                         ;; else exit with INSTALLED error
        ret                                             ;; :

        ;; ***********************************************************
        ;; Verify that all passed info is good
        ;; ***********************************************************
@@:     cmp             cl,0                            ;; If IRQ >= 0 then continue
        jge             @F                                      ;; : else exit
        xor     eax,eax                         ;; : with BAD IRQ error
        ret                                             ;; :
@@: cmp         cl,8                            ;; If IRQ < 8 then continue
        jl              @F                                      ;; : else exit
        xor             eax,eax                         ;; : with BAD IRQ error
        ret                                             ;; :

@@: ;; ***********************************************************
        ;; Set up COMM variables
        ;; ***********************************************************
        mov     PortInUse,ON                    ;; Set PortInUse flag to ON
        mov     Port,dx                                 ;; Save PORT BASE
        mov     Vector,cl                               ;; Save IRQ Vector
        add     Vector,8                                ;; : (IRQ + 8)
        mov     al,00000001b                    ;; Figure out IRQ Masks
        shl     al,cl                                   ;; :
        mov     DisableIRQ,al                   ;; Save IRQ Disable Mask
        not     al                                              ;; :
        mov     EnableIRQ,al                    ;; Save IRQ Enable Mask

        ;; ***********************************************************
        ;; Set up the Register Port Indexes
        ;; ***********************************************************
        mov             ecx,6                                   ;; Loop thru all 6 registers
@@: mov         ax,dx                                   ;; Get the Port Base
        add     ax,cx                                   ;; : Add the Index to the Base
        mov             Registers[ecx*2],ax             ;; : to make an Index for each register
        dec     cx                                              ;; : into the UART Ports.
        jns             @B                                              ;; :

        ;; ***********************************************************
        ;; Initialize the Read/Write Buffers
        ;; ***********************************************************
        call    ioClearWrite_                   ;;
        call    ioClearRead_                    ;;

        ;; ***********************************************************
        ;; Install the ISR
        ;; ***********************************************************
        mov             al,Vector                       ;; Next, save old ISR vector
        mov             ah,35h                          ;; : INT 21h Service 35h
        int             21h                             ;; : AL         = Vector Number
        mov             ISR_OldS,es             ;; : ES:EBX = Vector Address
        mov     ISR_OldO,ebx            ;; :

        cli                                             ;; Shut off interrupts
        mov             cx,ds                           ;; Now, install new ISR vector
        mov             ax,_TEXT                        ;; : INT 21h Service 25h
        mov             ds,ax                           ;; : DS:EDX = Vector Address
        mov             ah,25h                          ;; : AL         = Vector Number
        mov             al,Vector                       ;; :
        mov             edx,offset ISR_PM       ;; :
        int             21h                             ;; :
        mov     ds,cx                           ;; :

        mov     dx,Registers[MCR]       ;; Modem Control Register:
        in              al,dx                           ;; : Set the following:
        or              al,00001000b ;11b                ;; : OUT #2 (8), RTS (2), and DTR (1)
        out     dx,al                           ;; :

        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; Clear the Divisor Latch Access Bit (DLAB)
        and             al,01111111b            ;; :
        out             dx,al                           ;; :
        mov     dx,Registers[IER]       ;; Interrupt Enable Register
        mov     al,00000001b            ;; : Set Data Ready (1) Interrupt
        out     dx,al                           ;; : (THRE only set when needed)

        in              al,PICM                         ;; Finally, enable IRQ via 8259 MASK
        and             al,EnableIRQ            ;; :
        out             PICM,al                         ;; :

        sti                                             ;; Turn interrupts all back on

    ;; ***********************************************************
    ;; Set IRQ flags via 8250 (or maybe not) :) [v1.2]
        ;; ***********************************************************



        ;; ***********************************************************
        ;; Clear the buffers again, just in case . . .
        ;; ***********************************************************
        call    ioClearWrite_                           ;;
        call    ioClearRead_                            ;;

        ;; ***********************************************************
        ;; Exit with grace . . .
        ;; ***********************************************************
        mov     eax,offset      PortTable       ;; Return PortID Address
        mov     PortCode,eax                    ;; :
        ret                                                     ;; :
ioOpenPort_     ENDP



;;      ************************************************************************
;;      int             ioClosePort(COMM Address);
;;      PURPOSE:        Shuts off the COMM port and resets the ISR for it.
;;      PASS:           Address = EAX  Address of the PortID structure
;;      RETURNS:        -1 Failure | 0 Success
;;
;;      NOTE:   In this release, the passed address does nothing (no multi port).
;;      ************************************************************************
ioClosePort_  PROC USES EDX
        ;; ***********************************************************
        ;; First, make sure there is a COMM port initialized
        ;; ***********************************************************
        cmp     PortInUse,ON            ;; Make sure PortInUse flag is ON
        je              @F                                      ;; : and if so, then continue on.
        mov     eax,-1                          ;; else return w/ exit code
        ret                                             ;; :

        ;; ***********************************************************
        ;; Shut off the 8259 IRQ
        ;; ***********************************************************
@@:     in              al,PICM                         ;; Fetch PIC Mask
        or              al,DisableIRQ           ;; : Toggle OFF the ISR's IRQ line
        out             PICM,al                         ;; :

        ;; ***********************************************************
        ;; Disable the 8250 Interrupt (DLAB clear perhaps before this)
        ;; ***********************************************************
        mov             dx,Registers[IER]       ;; Interrupt Enable Register
        xor             al,al                           ;; Disable ALL Interrupts
        out             dx,al                           ;; :

        mov             dx,Registers[MCR]       ;; Modem Control Register
        in              al,dx                           ;; : Clear:
        and             al,11110111b            ;; : OUT #2 (8)
        out             dx,al                           ;; :

        ;; ***********************************************************
        ;; ISR is disabled, so reset old ISR Vector
        ;; ***********************************************************
        push    ds                                      ;; Dowaditty . . .
        mov             al,Vector                       ;; : You know how to reset a vector
        mov             ah,25h                          ;; : so why are you reading these
        mov             edx,ISR_OldO            ;; : comments?
        mov             ds,ISR_OldS             ;; : Don't I just comment too damn
        int             21h                             ;; : much?      Oh well.  At least its easy
        pop             ds                                      ;; : to follow code this way.

        mov     PortInUse,OFF           ;; Set PortInUse flag to OFF

        ret                                             ;; Return to caller
ioClosePort_    ENDP



;;      ************************************************************************
;;      void            ioClearRead(COMM Address);
;;      PASS:           EAX = Address Address of the PortID Structure
;;      PURPOSE:        Clears the Read buffer
;;
;;      NOTE:   In this release, the passed address does nothing (no multi port).
;;      ************************************************************************
ioClearRead_    PROC
        cli
        xor             ax,ax
        mov             RecvHead, ax
        mov             RecvTail, ax
        sti
        ret
ioClearRead_    ENDP



;;      ************************************************************************
;;      void            ioClearWrite(COMM Address);
;;      PASS:           EAX = Address Address of the PortID Structure
;;      PURPOSE:        Clears the Write buffer
;;
;;      NOTE:   In this release, the passed address does nothing (no multi port).
;;      ************************************************************************
ioClearWrite_   PROC
        cli
        xor             ax,ax
        mov             SendHead, ax
        mov             SendTail, ax
        sti
        ret
ioClearWrite_   ENDP



;;      ************************************************************************
;;      char            ioReadByte(COMM Address);
;;      PURPOSE:        Fetches a byte from the Read buffer.
;;      PASS:           EAX = Address Address of PortID Structure
;;      RETURNS:        duh....  If buffer is empty, then it returns NUL, else ...
;;
;;      NOTE:   Blah!!!!
;;      ************************************************************************
ioReadByte_ PROC USES EBX
        xor             eax, eax                        ;; Like I said, default return is NUL

        movzx   ebx,RecvTail            ;; Get Index into the FIFO buffer
        cmp             bx,RecvHead             ;; : And if nothing is in the buffer,
        je              @F                                      ;; : then exit . . .

        mov             al,RecvBuffer[ebx]      ;; Get current byte from buffer
        inc     bx                                      ;; : Increment index to next byte
        and     bx,RecvSize             ;; : Perform Buffer-Wrap
        mov     RecvTail,bx                     ;; : Save new Index

@@: ret                                                 ;; Exit away ya putz!
ioReadByte_ ENDP



;;      ************************************************************************
;;      char            int ioWriteByte(COMM Address, char byte);
;;      PURPOSE:        Places a byte into the Send buffer.
;;      PASS:           Address = EAX, BYTE = ECX
;;      RETURNS:        !0 = Success | 0 == Failure (buffer full)
;;
;;      NOTE:   Same old thang . . . get the next version for multi-port.
;;      ************************************************************************
ioWriteByte_    PROC USES EBX EDX
        ;; ***********************************************************
        ;; Check if buffer is FULL
        ;; ***********************************************************
        movzx   ebx,SendHead            ;; Get current Index into buffer
        mov     dx,bx                           ;; :
        inc     dx                                      ;; Figure out next index position
        and     dx,SendSize             ;; Perform Buffer-Wrap
        cmp     dx,SendTail             ;; Make sure we aren't Overflowing
        jne     @F                                      ;; :
        xor             eax,eax                         ;; : But, if we are, then exit w/
        ret                                             ;; : FULL error

@@: mov         SendBuffer[ebx],cl      ;; Place byte in Send Buffer
        mov     SendHead,dx             ;; Update index to next slot

        ;; ***********************************************************
        ;; Enable the THRE Interrupt
        ;; ***********************************************************
        mov             dx,Registers[IER]       ;; Interrupt Enable Register
        in              al,dx                           ;; : Enable:
        or              al,00000010b            ;; : Transmit Hold Register Emtpy Interrupt
        out             dx,al                           ;; :

        mov             eax,-1                          ;; Return w/ no probs . . .
        ret                                             ;; Return (duh!  Like you didn't know!)
ioWriteByte_  ENDP



;;      ************************************************************************
;;      int             ioReadStatus(COMM Address);
;;      PURPOSE:        Reports the number of bytes in the Receive Buffer
;;      PASS:           You know the number by now . . .
;;      RETURNS:        Number of bytes in buffer
;;      ************************************************************************
ioReadStatus_   PROC
        movzx   eax,RecvHead
        sub             ax,RecvTail
        jge     @F
        add             ax,RecvSize
@@:     ret
ioReadStatus_   ENDP



;;      ************************************************************************
;;      int             ioWriteStatus();
;;      PURPOSE:        Reports the number of bytes in the Send Buffer
;;      PASS:           Blah . . .
;;      RETURNS:        Number of bytes in buffer
;;      ************************************************************************
ioWriteStatus_ PROC
        movzx   eax,SendHead
        sub             ax,SendTail
        jge     @F
        add             ax,SendSize
@@:     ret
ioWriteStatus_ ENDP



;;      ************************************************************************
;;      void            void ioSetBaud(COMM Address, int BAUD);
;;      PURPOSE:        Set the BAUD rate for the initialized port.
;;      PASS:           Address = EAX, BAUD = EBX
;;      RATES:          150, 300, 600, 1200, 2400, 4800, 9600,
;;                              19200, 28800, 38400, 57600, 115200
;;      ************************************************************************
ioSetBaud_      PROC USES EDX
        or              ebx,ebx                         ;; Make sure it is not a divide by zero
        jz              SB_Exit                         ;; :

        mov             eax,115200                      ;; Baud rate divisor = 115200 / Baud
        xor             edx,edx                         ;; :
        div             ebx                                     ;; :
        mov             bx,ax                           ;; :

        cli                                             ;; Shut off hardware interrupts
        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; Set the Divisor Latch Access Bit (DLAB)
        or              al,10000000b            ;; :
        out             dx,al                           ;; :

        mov             dx,Registers[BAUDL]     ;; Baud Divisor Latch Low-Byte
        mov             al,bl                           ;; : Set it to Lower 8-bits of BAUD
        out             dx,al                           ;; :
        mov             dx,Registers[BAUDH] ;; Baud Divisor Latch High-Byte
        mov             al,bh                           ;; : Set it to Upper 8-bits of BAUD
        out             dx,al                           ;; :

        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; Clear the Divisor Latch Access Bit (DLAB)
        and             al,01111111b            ;; :
        out             dx,al                           ;; :

        sti                                             ;; Turn hardware interrupts back on

SB_Exit:
        ret                                             ;; Guess . . .
ioSetBaud_      ENDP



;;      ************************************************************************
;;      int             ioGetBaud(COMM Address);
;;      PURPOSE:        Get the BAUD rate from the initialized port.
;;      PASS:           Address = EAX
;;      RETURNS:        BAUD Rate = EAX
;;      ************************************************************************
ioGetBaud_      PROC USES EBX EDX
        cli                                             ;; Shut off hardware interrupts
        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; Set the Divisor Latch Access Bit (DLAB)
        or              al,10000000b            ;; :
        out             dx,al                           ;; :

        mov             dx,Registers[BAUDH] ;; Baud Divisor Latch High-Byte
        in              al,dx                           ;; : Get Upper 8-bits of BAUD
        mov     ah,al                           ;; :
        mov             dx,Registers[BAUDL]     ;; Baud Divisor Latch Low-Byte
        in              al,dx                           ;; : Get Lower 8-bits of BAUD
        movzx   ebx,ax

        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; Clear the Divisor Latch Access Bit (DLAB)
        and             al,01111111b            ;; :
        out             dx,al                           ;; :

        sti                                             ;; Turn hardware interrupts back on

        or              ebx,ebx                         ;; Make sure its not Divide by Zero
        jnz             @F                                      ;; :
        xor     eax,eax                         ;; :
        ret                                             ;; :

@@:     mov             eax,115200                      ;; Baud = 115200 / Baud Rate Divisor
        xor             edx,edx                         ;; :
        div             ebx                             ;; :

        ret                                             ;; Guess . . .
ioGetBaud_      ENDP



;;      ************************************************************************
;;      void            ioSetHandShake(COMM Address, int HAND);
;;      PURPOSE:        Set various handshaking lines.
;;      PASS:           Address = ECX, HAND = EAX
;;      Bit fields are:
;;      (80h)   (40h)   (20h)   (10h)   (08h)   (04h)   (02h)   (01h)
;;      Bit 7   Bit 6   Bit 5   Bit 4   Bit 3   Bit 2   Bit 1   Bit 0
;;      0               0               0               Loop    OUT2    OUT1    RTS     DTR
;;      ************************************************************************
ioSetHandShake_ PROC USES EDX
        mov             dx,Registers[MCR]       ;; Modem Control Register
        or              al,00001000b            ;; : Make sure IE is kept ON
        out     dx,al                           ;; : Set MCR Status
        ret                                             ;; La-de-da
ioSetHandShake_ ENDP



;;      ************************************************************************
;;      int             ioGetHandShake(COMM Address);
;;      PURPOSE:        Get current handshaking status.
;;      PASS:           ...
;;      RETURNS:        Handshaking bitfield (See SetHandShake() for field info)
;;      ************************************************************************
ioGetHandShake_ PROC USES EDX
        xor     eax,eax                         ;; Clear all of EAX
        mov             dx,Registers[MCR]       ;; Modem Control Register
        in              al,dx                           ;; : Fetch MCR Status
        ret                                             ;; La-de-da
ioGetHandShake_ ENDP



;;      ************************************************************************
;;      int             ioGetStatus(COMM Address);
;;      PURPOSE:        Fetches Modem/Line Status Register
;;      RETURNS:        Upper 8-bits==MSR & Lower 8-bits==LSR
;;      ************************************************************************
ioGetStatus_    PROC USES EDX
        mov             dx,Registers[MSR]       ;; Modem Status Register
        in              al,dx                           ;; : Fetch MSR Status
        mov             ah,al                           ;; Store in Upper 8-bits
        mov             dx,Registers[LSR]       ;; Line Status Register
        in              al,dx                           ;; : Fetch LSR Status
        and     eax,00ffh                       ;; Clear upper 16 bits
        ret                                             ;; What?  Return?  No way . . .
ioGetStatus_    ENDP



;;      ************************************************************************
;;      int             GetControl(COMM Address);
;;      PURPOSE:        Fetches Line Control Register
;;      PASS:           Blah blah blah.  Give me money and I'll let u pass sometin.
;;      RETURNS:        Lower 8-bits==LCR
;;      ************************************************************************
ioGetControl_   PROC USES EDX
        mov             dx,Registers[LCR]       ;; Line Control Register
        in              al,dx                           ;; : Fetch LCR Status
        and     eax,000fh                       ;; Clear upper 16 bits
        ret                                             ;; What?  Return?  No way . . .
ioGetControl_   ENDP



;;      ************************************************************************
;;      void            ioSetControl(COMM Address, int Control);
;;      PURPOSE:        Sets the Line Control Register
;;      PASS:           ECX = Address, Control = Lower 8-bits==LCR = EAX
;;      ************************************************************************
ioSetControl_   PROC USES EDX
        mov             dx,Registers[LCR]       ;; Line Control Register
        and     al,01111111b            ;; : Keep DLAB clear
        out             dx,al                           ;; : Set LCR Status
        ret                                             ;; What?  Return?  No way . . .
ioSetControl_   ENDP
_TEXT   ENDS
                END
