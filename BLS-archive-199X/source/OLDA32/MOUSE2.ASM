        .386
        .model flat
        LOCALS

        PUBLIC _initmouse,_readmouse

        .data
        
        REG                   STRUC
          REDI         dd     ?    ; offset 0
          RESI         dd     ?    ; offset 4
          REBP         dd     ?    ; offset 8
          XXXX         dd     ?    ; offset 12
          REBX         dd     ?    ; offset 16
          REDX         dd     ?    ; offset 20
          RECX         dd     ?    ; offset 24
          REAX         dd     ?    ; offset 28
          RFLAGS       dw     ?    ; offset 32
          RES          dw     ?    ; offset 30
          RDS          dw     ?    ; offset 32
          RFS          dw     ?    ; offset 34
          RGS          dw     ?    ; offset 36
          RIP          dw     ?    ; offset 38
          RCS          dw     ?    ; offset 40
          RSP          dw     ?    ; offset 42
          RSS          dw     ?    ; offset 44
        REG                   ENDS

rm     REG ? ; real mode regs        


        .code
        
        
_initmouse PROC 
        push edi
        push ds 
        pop es
        
        
        ;set up real mode int call struct
        mov rm.XXXX,0
        mov rm.RCS,0        
        mov rm.RIP,0        
        mov rm.RFS,0       
        mov rm.RGS,0       
        mov rm.RSP,0
        mov rm.RSS,0
        pushfd
        pop eax
        mov rm.RFLAGS,ax
        
        mov rm.REAX,0
        mov eax,0300h
        mov ebx,33h
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h   
             
        mov rm.REAX,4
        mov rm.RECX,160
        mov rm.REDX,100
        mov eax,0300h
        mov ebx,33h
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h   

        mov rm.REAX,0Fh
        mov rm.RECX,4
        mov rm.REDX,16
        mov eax,0300h
        mov ebx,33h
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h   


        pop edi
        ret
_initmouse ENDP


_readmouse PROC
        ARG X:DWORD,Y:DWORD
        push ebp
        mov ebp,esp
        push edi
        push ds
        pop es

        mov rm.REAX,03h
        mov eax,0300h
        mov ebx,33h
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h
        
        mov ebx,X        
        mov eax,rm.RECX
        and eax,0FFFFh
        shr eax,1
        mov [ebx],eax

        mov ebx,Y        
        mov eax,rm.REDX
        and eax,0FFFFh
        mov [ebx],eax
        
        mov eax,rm.REBX ;;buttons
        and eax,0FFh
       
       
        pop edi
        pop ebp
        ret
_readmouse ENDP
        END

