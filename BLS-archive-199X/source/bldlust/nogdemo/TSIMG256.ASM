                .386
                .MODEL  FLAT
                LOCALS
                .DATA
YLINE           dd ?
DYLINE  dd ?
LSW             dd ?
SRC             dd ?
SLANT           dw ?
SLANTCNT        dw ?

XCLIP           dd ?


_SHADOWMAP       db 256 DUP (0)

PUBLIC _SHADOWMAP

                .CODE
                PUBLIC  _DrawImageShadow
                ;,_PutImageFlipClip256



;*********************************************************
;       Purpose:
;               Puts an image on the screen in mode 13h.
;       Call:
;               ds:si=Address of image
;               es:di=Destination Address
;               bx=LSW
;               dl=XYb orient
;       Returns:
;               None
;       Destroyed:
;               ax,bx,cx,dx,si,di
;*********************************************************


_putimageflip256        PROC   near
                push    ebp
                mov     [LSW],ebx

                mov     [SRC],esi
                xor eax,eax
                xor ecx,ecx

                inc     esi
                lodsw
                mov     cx,ax           ;Copy X,Y
                inc     esi


;               mov     dl,0

                test    dl, 1b          ;Test Y Orientation
                jz      @@TYO
                push    dx

;               movzx   ax,ah          ;ax=ylength
                shr     ah,2
                xor     al,al
                movzx   dx,ah
                shl     dx,6
                add     ax,dx

;               mul     bx

                add     edi,eax         ;Move dest down ylines

                movzx   ax,ch   ;ylen
                shr     ax,2
                imul    [SLANT]
                movsx   eax,ah
                add     edi,eax
                xor eax,eax

                pop     dx


                neg     [LSW]
                neg     [SLANT]
@@TYO:

                mov     [YLINE],esi ;;Point to address of first line

                test    dl,10b          ;Test X orientation
                jz      BMON
                jmp     BMONFLIPX

                ret


_putimageflip256        ENDP

;****************************************************************
;********************** BMask ON *******************************
;****************************************************************

BMON            PROC  near
                mov     ebx,ecx         ;Y,X
                shr     bh,2
;               or      bh,1
                mov     dl,cl           ;X => dl HVALUE
                xor     ch,ch
                sub     [LSW],ecx               ; DISKIP - X

@@VLOOP:        mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,[SRC]
                add     [YLINE],4*2



@@CMPLOOP:      lodsb
                test    al,80h
                jnz     @@TCOLOR


                ;Data run
@@DCOLOR:       sub     bl,al           ;decrement x counter
                mov     cl,al

                add     esi,ecx

                jecxz @@NLP
@@LP:           movzx  eax, byte ptr [edi] ;get color
                mov   al,[eax+_SHADOWMAP]   ;get "shadowed color"
                stosb                      ;store it
                loop @@LP
 @@NLP:

                and     bl,bl
                jnz     @@CMPLOOP
                jmp     @@DONELINE

                ;Transparent Run
@@TCOLOR:       and     ax,7Fh          ;mask off upper bits
                add     edi,eax
                sub     bl,al
                jnz     @@CMPLOOP

@@DONELINE:     mov     bl,dl
                add     edi,[LSW]

                mov     ax,[SLANTCNT]
                add     ax,[SLANT]
                mov     [byte ptr SLANTCNT],al
                movsx   eax,ah
                add     edi,eax
                xor eax,eax


                dec     bh
                jnz     @@VLOOP
                pop     ebp
                ret
BMON            ENDP


;****************************************************************
;********************** BMask ON *******************************
;****************************************************************

BMONFLIPX       PROC   near
                mov     ebx,ecx         ;Y,X
                shr     bh,2
;               or      bh,1
                mov     dl,cl           ;X => dl HVALUE
                xor     ch,ch

                add     edi,ecx         ;;Go to right of scan line


@@VLOOP:        push    edi

                mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,[SRC]
                add     [YLINE],4*2


@@HLOOP:        lodsb
                test    al,80h
                jnz     @@TCOLOR


                ;Data run
@@DCOLOR:       sub     bl,al           ;decrement x counter
                mov     cl,al

                add     esi,ecx
                sub     edi,ecx
                push    ecx
                jecxz @@NLP
@@LP:           movzx  eax, byte ptr [edi] ;get color
                mov   al,[eax+_SHADOWMAP]   ;get "shadowed color"
                stosb                      ;store it
                loop @@LP
@@NLP:
                pop     ecx
                sub     edi,ecx



                and     bl,bl
                jnz     @@HLOOP
                jmp     @@DONELINE

                ;Transparent Run
@@TCOLOR:       and     ax,7Fh          ;mask off upper bits
                sub     edi,eax
                sub     bl,al
                jnz     @@HLOOP

@@DONELINE:     mov     bl,dl

                pop     edi
                add     edi,[LSW]

                mov     ax,[SLANTCNT]
                add     ax,[SLANT]
                mov     [byte ptr SLANTCNT],al
                movsx   eax,ah
                add     edi,eax
                xor eax,eax

                dec     bh
                jnz     @@VLOOP
                pop     ebp
                ret
BMONFLIPX       ENDP



_PutImageFlipClip256 PROC near
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD
                push    ebp
                mov     ebp,esp
                push    esi
                push    edi



                mov     esi,@@SRC
                mov     edi,@@DEST

                xor eax,eax

                inc     esi
                lodsw
                mov     ecx,eax         ;Copy X,Y
                inc     esi


                mov     ebx,ecx         ;Y,X
                xor     ch,ch

                mov     [DYLINE],8


                test    @@O, 1b          ;Test Y Orientation
                jz      @@TYO
                ;redo YST and YT stuff
                movzx   ax,bh
                dec     ax
                shl     ax,1
                add     esi,eax
                mov     [DYLINE],-8

;               shr     [@@YTOTAL],1

@@TYO:

                mov     [YLINE],esi



                test    @@O, 10b          ;Test X Orientation
                jz      @@TXO
                ;redo XST and XT stuff
;               add     di,[@@XSTART]
;               add     di,[@@XSTART]

                mov     eax,ecx          ;ax=xlength
                sub     eax,@@XTOTAL
                sub     eax,@@XSTART
                mov     @@XSTART,eax

                add     edi,@@XTOTAL    ;;Go to right of scan line
@@TXO:



                test    @@O,10b         ;Test X orientation
                jnz     @@FLIP
                call    BMCLIP
                jmp     @@DONE

@@FLIP:         call    BMCLIPFLIP


@@DONE:         pop     edi
                pop     esi
                pop     ebp
                ret
_PutImageFlipClip256 ENDP



BMCLIP          PROC     near
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD

                mov     edx,[@@XSTART]
                mov     ebx,[@@XTOTAL]
 ;              dec [@@YTOTAL]

                mov     [XCLIP],ecx

                sub     edi,edx

@@VLOOP:        push    ebx
                push    edx
                push    edi

                mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,@@SRC
                mov     eax,[DYLINE]
                add     [YLINE],eax

                xor     eax,eax

                ;skip past xstart
                and     ebx,ebx
                jle     @@DONELINE

                and     edx,edx
                jle     @@PHASE2

                add     edi,edx

                nop

@@PHASE1:
  xor eax,eax
        lodsb
                test    al,80h
                jnz     @@TCOLOR1
@@DCOLOR1:      add     esi,eax
                sub     edx,eax
                jg      @@PHASE1
                jz      @@PHASE2
                mov     eax,edx
                neg     eax
                sub     esi,eax
                jmp     @@DCOLOR2
@@TCOLOR1:      and     eax,7Fh
                sub     edx,eax
                jg      @@PHASE1
                jz      @@PHASE2
                mov     eax,edx
                neg     eax
                jmp     @@TCOLOR2


                ;------------------------------------
@@PHASE2:
xor eax,eax
        lodsb
                test    al,80h
                jnz     @@TCOLOR2

@@DCOLOR2:      sub     ebx,eax
                jge     @@NORIGHTCLIP
                add     eax,ebx
@@NORIGHTCLIP:
                mov     ecx,eax
                add     esi,eax

                jecxz @@NLP
@@LP:           movzx  eax, byte ptr [edi] ;get color
                mov   al,[eax+_SHADOWMAP]   ;get "shadowed color"
                stosb                      ;store it
                loop @@LP
 @@NLP:


                and     ebx,ebx
                jg      @@PHASE2
                jmp     @@DONELINE

@@TCOLOR2:      and     eax,7fh
                add     edi,eax
                sub     ebx,eax
                jg      @@PHASE2

@@DONELINE:

                pop     edi
                add     edi,@@LSW

                mov     ax,[SLANTCNT]
                add     ax,[SLANT]
                mov     [byte ptr SLANTCNT],al
                movsx   eax,ah
                add     edi,eax

                pop     edx
                pop     ebx

                mov ecx,eax

                and     eax,eax
                jl      @@SNEG
                sub     edx,eax
                add     ebx,eax
                jmp     @@NOYO
@@SNEG: sub     ebx,eax
@@NOYO:

                cmp     ebx,[XCLIP]
                jle     @@NOOVERBX
                mov     ebx,[XCLIP]
@@NOOVERBX:



                xor     eax,eax

                dec     [@@YTOTAL]      ;Decrease Y total
                jg      @@VLOOP         ;



@@DONE:         ret
BMCLIP          ENDP







BMCLIPFLIP              PROC NEAR
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD

                mov     edx,[@@XSTART]
                mov     ebx,[@@XTOTAL]
                mov     [XCLIP],ecx
;dec [@@YTOTAL]

                add     edi,edx
                xor     ecx,ecx

@@VLOOP:        push    ebx
                push    edx
                push    edi

                mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,@@SRC
                mov     eax,[DYLINE]
                add     [YLINE],eax
                xor     eax,eax

                ;skip past xstart
                and     ebx,ebx
                jle     @@DONELINE


                and     edx,edx
                jle     @@PHASE2

                sub     edi,edx


@@PHASE1:       lodsb
                test    al,80h
                jnz     @@TCOLOR1
@@DCOLOR1:      add     esi,eax
                sub     edx,eax
                jg      @@PHASE1
                jz      @@PHASE2
                mov     eax,edx
                neg     eax
                sub     esi,eax
                jmp     @@DCOLOR2
@@TCOLOR1:      and     eax,7Fh
                sub     edx,eax
                jg      @@PHASE1
                jz      @@PHASE2
                mov     eax,edx
                neg     eax
                jmp     @@TCOLOR2


                ;------------------------------------
@@PHASE2:
xor eax,eax
                lodsb
                test    al,80h
                jnz     @@TCOLOR2

@@DCOLOR2:      sub     ebx,eax
                jge     @@NORIGHTCLIP
                add     eax,ebx
@@NORIGHTCLIP:

                mov     ecx,eax
                add     esi,eax
                sub     edi,eax
                push    eax

                jecxz @@NLP
@@LP:           movzx  eax, byte ptr [edi] ;get color
                mov   al,[eax+_SHADOWMAP]   ;get "shadowed color"
                stosb                      ;store it
                loop @@LP
@@NLP:

                pop     eax
                sub     edi,eax


                xor     eax,eax

                and     ebx,ebx
                jg      @@PHASE2
                jmp     @@DONELINE


@@TCOLOR2:      and     eax,7fh
                sub     edi,eax
                sub     ebx,eax
                jg      @@PHASE2


@@DONELINE:     pop     edi
                add     edi,@@LSW
                mov     ax,[SLANTCNT]
                add     ax,[SLANT]
                mov     [byte ptr SLANTCNT],al
                movsx   eax,ah
                add     edi,eax

                pop     edx
                pop     ebx

                and     eax,eax
                jl      @@SNEG
                add     ebx,eax
                jmp     @@DONES
@@SNEG:         add     edx,eax

                sub     ebx,eax
@@DONES:

                cmp     ebx,[XCLIP]
                jle     @@NOOVERBX
                mov     ebx,[XCLIP]
@@NOOVERBX:

                xor     eax,eax

                dec     [@@YTOTAL]      ;Decrease Y total
                jg      @@VLOOP         ;


@@DONE:         ret
BMCLIPFLIP      ENDP


;                EXTRN  _scry:WORD

;void DrawImage(memptr *src,char far *dest,int x,int y,int o,int slant);
_DrawImageShadow PROC   near
        ARG     @@SRC:DWORD,@@DEST:DWORD,@@X:DWORD,@@Y:DWORD,@@O:DWORD,@@SLANT:DWORD
;       ret
        push    ebp
        mov     ebp,esp
        push    ebx
        push    esi
        push  edi

;or     [@@O],1b
        ;and    @@O,NOT 1b
;       mov @@O,0
;       mov @@SLANT,0



        mov     edi,[@@DEST]
        mov     esi,@@SRC ;ptr

        and     esi,esi
        jz              @@DONE

        xor     ebx,ebx ;Clip flag
        movzx   edx,byte ptr [esi+1] ;cx=XST  dx=XLEN
        xor   ecx,ecx   ;

        mov     al,[esi+2]
        shr     al,2
        jz      @@DONE

        ;adjust x for slanting
        mov     eax,[@@Y]
        sub     eax,180     ;;relative pixels from base


        push    edx
        imul    [@@SLANT]
        movsx   eax,ah
        add   [@@X],eax
        xor eax,eax
        pop     edx


        cmp     [@@X],0
        jge     @@NOXL
        inc     bl
        sub     ecx,[@@X]  ;xst=0-x
        add     edx,[@@X]           ;xt =xst+xl
        mov     [@@X],0
@@NOXL:

        mov     eax,edx
        add     eax,@@X
        cmp     eax,320
        jle     @@NOXR
        inc     bl

        mov     edx,320
        sub     edx,[@@X]
        xor     ecx,ecx
@@NOXR:

        push    edx
        xor eax,eax
                mov     ah,byte ptr @@Y
                xor     al,al
                movzx   dx,ah
                shl     dx,6
                add     ax,dx

;       imul    ax,@@Y,320
        add     eax,@@X
        add     edi,eax
        pop     edx

        mov     eax,[@@SLANT]
        mov     [SLANT],ax
        mov     [SLANTCNT],0


        and     bl,bl
        jnz     @@CLIP
        mov     ebx,320
        mov     edx,@@O
        call    _putimageflip256
        jmp     @@DONE

@@CLIP:

cmp ecx,0ffh
ja  @@DONE

        push    @@O
        movzx   eax,byte ptr [esi+2]
        shr     eax,2
        push    eax     ;yt
        push    edx     ;xt
        push    0      ;yst
        push    ecx     ;xst
        push    320
        push    edi
        push    esi
        call    _PutImageFlipClip256
        add     sp,32

@@DONE:

;       mov     ax,0A000h
;       mov     es,ax
;       mov     di,64000-320+4
;       mov     byte ptr [es:di],0

        pop     edi
        pop     esi
        pop     ebx
        pop     ebp
        ret
_DrawImageShadow ENDP








                END
