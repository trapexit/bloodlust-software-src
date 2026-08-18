        .386
        .model flat
        LOCALS



        .code


;void COPYSOUND8(

PUBLIC _COPYSOUND8,_MIXASM,_SILENCEASM,_COPYASM,_loades


_COPYSOUND8 PROC NEAR
                ARG @@DEST:DWORD,@@SRC:DWORD,@@CLIP8:DWORD

                push    ebp
                mov     ebp,esp

                push    esi
                push    edi
                push ebx
                mov edi,@@DEST
                mov esi,@@SRC
                mov ebx,@@CLIP8
                mov ecx,512
                xor eax,eax


@@LP:           lodsw             ;get 16-bit sample      
                cwde              ;convert to 32-bit
                sar  eax,8 ;     ;divide
                mov  al,[eax+ebx] ;get 8 bit sample
                stosb             ;write it
                loop @@LP

                pop ebx
                pop edi
                pop esi
                pop ebp
                ret
_COPYSOUND8 ENDP


_MIXASM  PROC NEAR
                ARG @@DEST:DWORD,@@SRC:DWORD,@@NUM:DWORD,@@VOL:DWORD

                push    ebp
                mov     ebp,esp

                push    esi
                push    edi
                push ebx
                mov   edi,@@DEST
                mov   esi,@@SRC
                mov   ebx,@@VOL
                mov   ecx,@@NUM
                xor eax,eax

@@LP:           lodsb   ;get sample
                mov     edx,[eax*2+ebx]
                add     [edi],dx ; mix
                jo      @@OVER
@@CONT:         inc     edi
                inc     edi
                loop    @@LP
                jmp @@DONE
                
@@OVER:         js  @@SIGN
                mov word ptr [edi],08000h
                jmp @@CONT

@@SIGN:         mov word ptr [edi],07FFFh
                jmp @@CONT


@@DONE:         pop ebx
                pop  edi
                pop esi
                pop ebp
                ret
_MIXASM  ENDP


_COPYASM  PROC NEAR
                ARG @@DEST:DWORD,@@SRC:DWORD,@@NUM:DWORD,@@VOL:DWORD

                push    ebp
                mov     ebp,esp

                push    esi
                push    edi
                push ebx
                mov   edi,@@DEST
                mov   esi,@@SRC
                mov   ebx,@@VOL
                mov   ecx,@@NUM

@@LP:           xor eax,eax
                lodsb            ;get sample
                mov     eax,[eax*2+ebx]
                stosw
                loop    @@LP

                pop ebx
                pop  edi
                pop esi
                pop ebp
                ret
_COPYASM  ENDP


_loades proc
  push ds
  pop  es
  ret
_loades endp  

_SILENCEASM PROC
                ARG @@DEST:DWORD,@@NUM:DWORD

                push    ebp
                mov     ebp,esp
                push    edi
                mov   edi,@@DEST
                mov   ecx,@@NUM
                xor   eax,eax
                
                shl     ecx,1
                shr     ecx,2
@@NOEXTRA2:     rep     stosd
                pop edi
                pop ebp
                ret
_SILENCEASM ENDP





        END
