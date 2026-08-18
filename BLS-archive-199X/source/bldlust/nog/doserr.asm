                .386
                .MODEL flat
                LOCALS
                JUMPS

                .CODE

                PUBLIC InitDosError_



InitDosError_   PROC NEAR
                push    ebx

                mov ax,205h
                mov bl,24h
                mov cx,cs
                mov edx,OFFSET _DosErrorInterrupt
                int  31h

                pop ebx
                ret
InitDosError_   ENDP


_DosErrorInterrupt PROC  
                mov al,3h ;abort system call
                iretd
_DosErrorInterrupt ENDP

                END
