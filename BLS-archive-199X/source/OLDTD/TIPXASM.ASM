        .386
        .model flat
        LOCALS

       PUBLIC _initipxasm,_ipxgetmaxpacketsize,ipxopensocket_,ipxclosesocket_,ipxsend_,ipxlisten_
       PUBLIC ipxgetaddress_,reverse_,ipxrelinquish_,ipxcancel_

        .data
        
       PUBLIC _ipxrealmemseg 

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


_ipxrealmemseg dw ? ;real stack segments

ipxinuse dd 0


        .code
        
        
_initipxasm PROC 
        push edi
        push ds 
        pop es
        
        ;allocate realmode xfer area
        mov eax,100h ;allocate
        mov ebx,32768/16
        int 31h  ;get memory
        jnc GOTMEMORY
        mov eax,0
        jmp @@DONE  ;;abort
GOTMEMORY:
                
        mov [_ipxrealmemseg],ax
        
        ;set up real mode int call struct
        mov rm.XXXX,0
        mov rm.RCS,0        
        mov rm.RIP,0        
        mov rm.RFS,ax       
        mov rm.RGS,ax       
        mov rm.RSP,32000
        mov rm.RSS,ax
;        cli 
;        cld
        pushfd
        pop eax
        mov rm.RFLAGS,ax
;        sti
        
        
        ;check for existence of ipx
        mov rm.REAX,07A00h ;check existence
        mov eax,0300h
        mov ebx,2Fh
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h        
        
        mov ax,_ipxrealmemseg
        mov rm.RES,ax
        mov rm.RDS,ax
        
        

        mov eax,rm.REAX
        movzx eax,al
@@DONE:
        pop edi
        ret
_initipxasm ENDP


_ipxgetmaxpacketsize PROC
        push edi
        mov rm.REBX,0Dh
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h        
        mov eax,rm.REAX
        pop edi
        ret
_ipxgetmaxpacketsize ENDP

ipxrelinquish_ PROC
        cmp [ipxinuse],0
        jz @@OK
        ret
@@OK:   inc [ipxinuse]

        push edi
        mov rm.REBX,0Ah
 
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        push ds
        pop es
        int 31h        
        pop edi
        
        dec [ipxinuse]
        ret
ipxrelinquish_ ENDP


ipxopensocket_ PROC
        push edi
        mov rm.REBX,00h
        mov rm.REAX,0h
        mov rm.REDX,eax
        
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        mov eax,rm.REDX
        pop edi
        ret
ipxopensocket_ ENDP


ipxclosesocket_ PROC
        push edi
        xchg ah,al        
        mov rm.REDX,eax
        mov rm.REBX,01h
        
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        pop edi
        ret
ipxclosesocket_ ENDP

ipxlisten_ PROC
        cmp [ipxinuse],0
        jz @@OK
        ret
@@OK:   inc [ipxinuse]

        push edi
        mov rm.RESI,eax
        mov rm.REBX,04h

        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        push ds
        pop es
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        pop edi
        
        dec [ipxinuse]
        ret
ipxlisten_ ENDP

ipxsend_ PROC
        cmp [ipxinuse],0
        jz @@OK
        ret
@@OK:   inc [ipxinuse]


        push edi
        mov rm.RESI,eax
        mov rm.REBX,03h

        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        push ds
        pop es
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        pop edi

        dec [ipxinuse]
        ret
ipxsend_ ENDP

ipxcancel_ PROC
        cmp [ipxinuse],0
        jz @@OK
        ret
@@OK:   inc [ipxinuse]


        push edi
        mov rm.RESI,eax
        mov rm.REBX,06h
       
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        push ds
        pop es
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        pop edi

        dec [ipxinuse]
        ret
ipxcancel_ ENDP


ipxgetaddress_ PROC
        push edi
        mov rm.RESI,eax
        mov rm.REBX,09h
        
        mov eax,0300h
        mov ebx,7Ah
        xor ecx,ecx
        mov edi,OFFSET rm
        int 31h        
        movzx eax,[byte ptr rm.REAX]
        pop edi
        ret
ipxgetaddress_ ENDP


reverse_ PROC
        xchg al,ah
        ret
reverse_ ENDP        



        END
