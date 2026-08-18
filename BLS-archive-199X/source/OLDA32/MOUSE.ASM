        .386
        .model flat
        LOCALS

        PUBLIC _initmouse,_readmouse

        .code
        
        
_initmouse PROC 
        push edi
        mov eax,0
        int 33h
             
        mov eax,4
        mov ecx,160
        mov edx,100
        int 33h   

        mov eax,0Fh
        mov ecx,4
        mov edx,16
        int 33h   


        pop edi
        ret
_initmouse ENDP


_readmouse PROC
        ARG X:DWORD,Y:DWORD
        push ebp
        mov ebp,esp
        push edi

        mov eax,03h
        int 33h
        
        mov edi,X        
        mov eax,ecx
        and eax,0FFFFh
        shr eax,1
        mov [edi],eax

        mov edi,Y        
        mov eax,edx
        and eax,0FFFFh
        mov [edi],eax
        
        mov eax,ebx ;;buttons
        and eax,0FFh
       
       
        pop edi
        pop ebp
        ret
_readmouse ENDP
        END

