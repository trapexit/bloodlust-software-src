                .486
                .MODEL  flat
                LOCALS

                .DATA
                EXTRN _PITCH:DWORD,_SCREENX:DWORD,_SCREENY:DWORD

YLINE           dd ?
COUNT dw ?
GMODE         dd  0

                .CODE
                PUBLIC  _DrawImage,_ColorMapImage
                PUBLIC _Mode256,_ModeText,_LoadPalette

_Mode256 PROC NEAR
  cmp GMODE,1 ;;already in gfix mode
  je @@DONE
  mov ax,13h
  int 10h
  mov GMODE,1
@@DONE:  
  ret
_Mode256 ENDP

_ModeText PROC NEAR
  cmp GMODE,0  ;;already in text mode
  je @@DONE
  mov ax,3h
  int 10h
  mov GMODE,0
@@DONE:  
  ret
_ModeText ENDP


;;stack based version
_PutImageFlip256        PROC NEAR
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@O:DWORD
                push    ebp
                mov     ebp,esp

                mov     esi,@@SRC
                mov     edi,@@DEST

                mov    ecx,[esi]
                shr    ecx,8    ;cl=xw ch=yw
                add    esi,4   

                xor     eax,eax

                test    [@@O], 1b          ;Test Y Orientation
                jz      @@TYO
                mov    al,ch
                mul    @@LSW
                add    edi,eax             ;Move dest down ylines
                neg    [@@LSW]             ;flip direction
                xor    eax,eax
@@TYO:
                mov     al,ch
                lea     esi,[esi+eax*2]
                
                mov     ebp,@@LSW

                test    [@@O],10b       ;Test X orientation
                jnz     @@FLIP
                call    BMON
                jmp     @@DONE
@@FLIP:         call    BMONFLIPX

@@DONE:         pop     ebp
                ret
_PutImageFlip256        ENDP



;esi=src.yline
;edi=dest
;ch=XLEN
;dh=YLEN
;ebp=LSW
;ebx=O
;register based version
putimageflipreg PROC

                xor     eax,eax
                mov     al,ch
                lea     esi,[esi+eax*2+4]

                test    ebx, 1b          ;Test Y Orientation
                jz      @@TYO
                mul    ebp
                add    edi,eax             ;Move dest down ylines
                neg    ebp                 ;flip direction
@@TYO:

                test   ebx,10b       ;Test X orientation
                jz     BMON
                jmp    BMONFLIPX

putimageflipreg ENDP





;****************************************************************
;****************************************************************

;;21864 5.904sec


BMON            PROC             NEAR

                xor   edx,edx
                xor   ebx,ebx
                mov   bl,cl          ;ebx=xwidth
                mov   dl,ch          ;edx=ywidth

                xor    eax,eax
                sub    ebp,ebx       ; DISKIP - X


@@VLOOP:        push    ebx
@@HLOOP:        mov     al,[esi]
                inc     esi
                test    al,80h
                jnz     @@TCOLOR


                ;Data run
@@DCOLOR:
                sub     ebx,eax         ;decrement x counter

    ;;forwards memory copy!-------------------------------
                mov     ecx,edi
                neg     ecx
                and     ecx, 3
                sub     eax, ecx
                jle     short @@LEndBytes
               rep movsb
                mov     ecx, eax
                and     eax, 3
                shr     ecx, 2
               rep movsd
@@LEndBytes:    add     ecx, eax
               rep movsb 
   ;;----------------------------------------------------
                xor eax,eax

                test    ebx,ebx
                jle      @@DONELINE
                mov     al,[esi]
                inc     esi
                test    al,80h
                jz      @@DCOLOR ;2d
                               ;d then t      

                ;Transparent Run
@@TCOLOR:       and     al,7Fh          ;mask off upper bits
                add     edi,eax
                sub     ebx,eax
                jle     @@DONELINE
                mov     al,[esi]
                inc     esi
                test    al,80h
                jz      @@DCOLOR ;t then d               
                jmp     @@TCOLOR ;2 t in row
                

@@DONELINE:     pop     ebx
                add     edi,ebp
                dec     edx
                jnz     @@VLOOP

                ret
BMON            ENDP


;****************************************************************
;********************** BMask ON *******************************
;****************************************************************

BMONFLIPX       PROC          NEAR

                xor   edx,edx
                xor   ebx,ebx
                mov   bl,cl          ;ebx=xwidth
                mov   dl,ch          ;edx=ywidth

                xor    eax,eax
                add    ebp,ebx       ; DISKIP - X
                
                add   edi,ebx   ;move to right of scanline


@@VLOOP:        push  ebx

@@HLOOP:        mov    al,[esi]
                inc    esi
                test   al,80h
                jnz    @@TCOLOR


                ;Data run
@@DCOLOR:       sub     ebx,eax         ;decrement x counter


;;;backwards memory copy! esi->~edi ecx=count
 ;;precopy
                mov     ecx,edi
                and     ecx, 3
                jz      @@NOC1
                sub     eax, ecx
                jle     short @@LEndBytes
                push  eax
@@C1:           dec   edi
                mov   al,[esi]       ; rep movsb
                inc   esi
                mov   [edi],al
                dec   ecx
                jg   @@C1
                pop   eax
 ;;copy                
@@NOC1:         mov     ecx, eax
                and     eax, 3
                shr     ecx, 2
             
                jz @@LEndBytes ;rep movsd
                push eax

@@COPY:         sub  edi,4
                mov  eax,[esi]
                add  esi,4
                bswap eax
                dec  ecx
                mov  [edi],eax
                jnz  @@COPY
                pop eax
 ;;postcopy              
@@LEndBytes:    add     ecx, eax
             
                jle @@DONEC3 ;  rep movsb
@@C3:           dec   edi
                mov   al,[esi]       ; rep movsb
                inc   esi
                mov   [edi],al
                dec   ecx
                jg  @@C3
@@DONEC3:
;;------------------------------------

               xor eax,eax

                and     ebx,ebx
                jg     @@HLOOP
                jmp     @@DONELINE

                ;Transparent Run
@@TCOLOR:       and     al,7Fh          ;mask off upper bits
                sub     edi,eax
                sub     ebx,eax
                jg      @@HLOOP

@@DONELINE:
                pop     ebx
                add     edi,ebp

                dec     edx
                jnz     @@VLOOP


@@DONE:         
                ret
BMONFLIPX       ENDP


_PutImageFlipClip256 PROC   NEAR
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD
                push    ebp
                mov     ebp,esp
                push    esi
                push    edi

                mov     esi,@@SRC
                mov   edi,@@DEST

                inc     esi
                lodsw
                movzx   ecx,ax          ;Copy X,Y
                inc     esi



                mov     bx,cx           ;Y,X
                xor     ch,ch
                xor     eax,eax

                test    @@O, 1b          ;Test Y Orientation
                jz      @@TYO
                ;redo YST and YT stuff
                mov   al,bh          ;ax=ylength
                sub     eax,@@YTOTAL ;YST=y-yt-yst
                sub     eax,@@YSTART
                mov     @@YSTART,eax

                push  edx
                mov     eax,@@YTOTAL          ;ax=ylength
                mul     @@LSW
                pop     edx

                add   edi,eax           ;Move dest down ylines
                neg     @@LSW
@@TYO:

                mov     eax,@@YSTART   ;;Point to address of first line
                lea    eax,[eax*2+esi]
                ;shl     eax,1
                ;add     eax,esi
                mov     [YLINE],eax


                test    @@O, 10b          ;Test X Orientation
                jz      @@TXO
                ;redo XST and XT stuff
                mov     eax,ecx          ;ax=xlength
                sub     eax,@@XTOTAL
                sub     eax,@@XSTART
                mov     @@XSTART,eax

                add     edi,@@XTOTAL            ;;Go to right of scan line
@@TXO:



                test    @@O,10b         ;Test X orientation
                jnz     @@FLIP
                call    BMCLIP
                jmp     @@DONE

@@FLIP:         call    BMCLIPFLIP


@@DONE:
                 pop    edi
                pop     esi
                pop     ebp
                ret
_PutImageFlipClip256 ENDP



BMCLIP          PROC       NEAR
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD
                mov     edx,[@@XSTART]
                mov     ebx,[@@XTOTAL]


                xor     eax,eax
                xor     ecx,ecx


@@VLOOP:
                push    ebx
                push    edx
                push    edi

                mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,@@SRC
                inc     [YLINE]
                inc     [YLINE]

                ;skip past xstart
                and     edx,edx
                jle     @@PHASE2


@@PHASE1:
                xor     eax,eax
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
@@TCOLOR1:      and     ax,7Fh
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
    ;;forwards memory copy!-------------------------------
                mov     ecx,eax
                sub     ecx,edi
                sub     ecx, eax
                                         and     ecx, 3
                sub     eax, ecx
                jle     short @@LEndBytes
               rep movsb
                mov     ecx, eax
                and     eax, 3
                shr     ecx, 2
               rep movsd
@@LEndBytes:    add     ecx, eax
                                   rep movsb
   ;;----------------------------------------------------
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

                pop     edx
                pop     ebx

                dec     [@@YTOTAL]      ;Decrease Y total
                jnz     @@VLOOP         ;


@@DONE:         ret
BMCLIP          ENDP







BMCLIPFLIP              PROC     NEAR
        ARG @@SRC:DWORD,@@DEST:DWORD,@@LSW:DWORD,@@XSTART:DWORD,@@YSTART:DWORD,@@XTOTAL:DWORD,@@YTOTAL:DWORD,@@O:DWORD


                mov     edx,[@@XSTART]
                mov     ebx,[@@XTOTAL]
                xor     eax,eax
                xor     ecx,ecx
;               add     di,bx   ;;Go to right of scan line


@@VLOOP:
                push    ebx
                push    edx
                push    edi

                mov     esi,[YLINE]
                movzx   esi,word ptr [esi]
                add     esi,@@SRC
                inc     [YLINE]
                inc     [YLINE]

                ;skip past xstart
                and     edx,edx
                jle     @@PHASE2

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
@@TCOLOR1:      and     ax,7Fh
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




;;;backwards memory copy! esi->~edi ecx=count
                mov     ecx,edi
                                         and     ecx, 3
                sub     eax, ecx
                jle     short @@LEndBytes
              ; rep movsb
              jecxz @@NOC1
@@C1:         dec  edi
                        movsb
              dec  edi
              loop @@C1
@@NOC1:       mov     ecx, eax
                and     eax, 3
                shr     ecx, 2
               ;rep movsd
              jz @@LEndBytes
              push eax
              sub  edi,4
@@COPY:       lodsd
              bswap eax
              stosd
              sub edi,8
              loop @@COPY
              add edi,4
              pop eax
@@LEndBytes:    add     ecx, eax
                                 ;  rep movsb
              jle @@DONEC3
@@C3:         dec  edi
                        movsb
              dec  edi
              loop @@C3
@@DONEC3:
;;memory copy------------------

                and     ebx,ebx
                jg         @@PHASE2
                jmp     @@DONELINE


@@TCOLOR2:      and     eax,7fh
                sub     edi,eax
                sub     ebx,eax
                jg      @@PHASE2



@@DONELINE:     pop     edi
                add     edi,@@LSW

                pop     edx
                pop     ebx

                dec     [@@YTOTAL]      ;Decrease Y total
                jg      @@VLOOP         ;


@@DONE:         ret
BMCLIPFLIP      ENDP



;void DrawImage(char far *src,char far *dest,int x,int y,int o);
_DrawImage PROC     NEAR
        ARG     @@SRC:DWORD,@@DEST:DWORD,@@X:DWORD,@@Y:DWORD,@@O:DWORD
;       ret
        push    ebp
        mov     ebp,esp
        push    ebx
        push    esi
        push    edi

        mov     esi,[@@SRC]
        mov     edi,[@@DEST]
        test    esi,esi
        jz      @@DONE

        ;clipping vars
        xor     eax,eax
        xor     ebx,ebx      ;Clip flag
        xor     edx,edx      ;dl=XSTART  dh=YSTART

        mov     ecx,[esi+1]  ;cl=XLEN    ch=YLEN
        ;add     esi,4

        ;left clipping
        cmp     [@@X],eax
        jge     @@NOXL
        mov     al,cl         ;EAX=XLEN
        inc     ebx
        add     eax,[@@X]     ;xt =xst+xl
        jle     @@DONE

        mov     cl,al         ;XLEN=XLEN+X   (x is negative)
        xor     eax,eax
        sub     dl,[byte ptr @@X]   ;XSTART=0-x
        mov     [@@X],eax
@@NOXL:
        ;top clipping
        cmp     [@@Y],eax
        jge     @@NOYL
        mov     al,ch         ;EAX=YLEN
        inc     ebx
        add     eax,[@@Y]     ;yt =yst+yl
        jle     @@DONE

        mov     ch,al         ;YLEN=YLEN+Y   (x is negative)
        xor     eax,eax
        sub     dh,[byte ptr @@Y]   ;YSTART=0-Y
        mov     [@@Y],eax
@@NOYL:
        
        ;right clipping
        mov     al,cl 
        add     eax,@@X       ;EAX=XLEN+X
        cmp     eax,_SCREENX
        jle     @@NOXR
        mov     eax,_SCREENX
        inc     ebx
        sub     eax,[@@X]
        jle     @@DONE
        mov     cl,al  ;XLEN=SCREENX-X
@@NOXR: xor     eax,eax

        ;bottom clipping
        mov     al,ch 
        add     eax,@@Y       ;EAX=Y+YLEN
        cmp     eax,_SCREENY
        jle     @@NOYR
        mov     eax,_SCREENY
        inc     ebx
        sub     eax,[@@Y]
        jle     @@DONE
        mov     ch,al  ;YLEN=SCREENY-Y
@@NOYR: 

        ;edi+=Y*PITCH+X
        push  edx
        mov     eax,@@Y
        mul     _PITCH
        add     eax,@@X
        pop     edx
        add     edi,eax   ;get dest


        test    ebx,ebx
        jnz     @@CLIP
        mov     ebx,@@O
        mov     ebp,_PITCH
        call    putimageflipreg
        jmp     @@DONE
@@CLIP:

        xor     eax,eax

        push    @@O
        mov     al,ch
        push    eax
        mov     al,cl
        push    eax
        mov     al,dh
        push    eax
        mov     al,dl
        push    eax
        mov     eax,_PITCH
        push    eax
        push    edi
        push    esi
        call    _PutImageFlipClip256
        add     esp,8*4

@@DONE:

        pop     edi
        pop     esi
        pop     ebx
        pop     ebp
        ret
_DrawImage ENDP




_ColorMapImage  PROC   NEAR
        ARG     @@IMG:DWORD,@@MAP:DWORD

                push    ebp
                mov     ebp,esp

                push ebx
                push    esi
                push    edi

                mov     esi,@@IMG
                mov     edi,@@MAP

                inc esi
                lodsw
                mov     dx,ax  ;dl-x  dh-y
                inc esi

                xor     eax,eax
                mov   al,dh
                add     esi,eax
                add     esi,eax


                xor     ecx,ecx
                xor     ebx,ebx

                mov     ah,dl

@@VLOOP:        lodsb
                test    al,80h
                jnz     @@TCOLOR


                ;Data run
@@DCOLOR:       sub     ah,al           ;decrement x counter
                movzx   cx,al


;               add     si,cx

@@CLOOP:        mov     bl,[esi]
                mov     al,[edi+ebx]
                mov     [esi],al
                inc     esi
                loop    @@CLOOP


@@DONELOOP:     and     ah,ah
                jnz     @@VLOOP
                jmp     @@DONELINE

                ;Transparent Run
@@TCOLOR:       and     al,7fh
                sub     ah,al
                jnz     @@VLOOP

@@DONELINE:     mov     ah,dl
                dec     dh
                jnz     @@VLOOP

                pop     edi
                pop     esi
                pop     ebx
                pop     ebp
                ret
_ColorMapImage  ENDP




_LoadPalette    PROC    NEAR
                ARG     @@P:DWORD,@@FIRST:DWORD,@@CNT:DWORD

                push    ebp
                mov     ebp,esp
                push    esi
                mov     esi,@@P

                mov     eax,@@FIRST
                add esi,eax
                add esi,eax
                add esi,eax

                mov     dx,3C8h
                out     dx,al   ;Set first

                cld
                inc     dx


                mov     ecx,@@CNT
                shl     ecx,1
                add     ecx,@@CNT

                rep     outsb

                pop     esi
                pop     ebp
                ret
_LoadPalette    ENDP






                END
