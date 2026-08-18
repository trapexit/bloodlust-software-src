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
        push ebx

        mov eax,03h
        int 33h
        
        mov eax,X        
        and ecx,0FFFFh
        shr ecx,1
        mov [eax],ecx

        mov eax,Y        
        and edx,0FFFFh
        mov [eax],edx
        
        mov eax,ebx ;;buttons
        and eax,0FFh
       
        pop ebx
        pop ebp
        ret
_readmouse ENDP
        END

